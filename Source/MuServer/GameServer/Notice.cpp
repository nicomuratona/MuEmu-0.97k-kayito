#include "stdafx.h"
#include "Notice.h"
#include "MemScript.h"
#include "Message.h"
#include "Util.h"

CNotice gNotice;

CNotice::CNotice()
{
	this->m_count = 0;

	this->m_NoticeValue = 0;

	this->m_NoticeTime = GetTickCount();
}

CNotice::~CNotice()
{

}

void CNotice::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == NULL)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (!lpMemScript->SetBuffer(path))
	{
		ErrorMessageBox(lpMemScript->GetLastError());

		delete lpMemScript;

		return;
	}

	this->m_count = 0;

	try
	{
		eTokenResult token;

		while (true)
		{
			token = lpMemScript->GetToken();

			if (token == TOKEN_END || token == TOKEN_END_SECTION)
			{
				break;
			}

			NOTICE_INFO info;

			memset(&info, 0, sizeof(info));

			strcpy_s(info.Message, lpMemScript->GetString());

			info.Type = lpMemScript->GetAsNumber();

			info.RepeatTime = lpMemScript->GetAsNumber() * 1000;

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CNotice::SetInfo(NOTICE_INFO info)
{
	if (this->m_count < 0 || this->m_count >= MAX_NOTICE)
	{
		return;
	}

	this->m_NoticeInfo[this->m_count++] = info;
}

void CNotice::MainProc()
{
	if (this->m_count == 0)
	{
		return;
	}

	NOTICE_INFO* lpInfo = &this->m_NoticeInfo[this->m_NoticeValue];

	if ((GetTickCount() - this->m_NoticeTime) >= ((DWORD)lpInfo->RepeatTime))
	{
		this->m_NoticeValue = (((this->m_NoticeValue + 1) >= this->m_count) ? 0 : (this->m_NoticeValue + 1));

		this->m_NoticeTime = GetTickCount();

		this->GCNoticeSendToAll(lpInfo->Type, "%s", lpInfo->Message);
	}
}

void CNotice::GCNoticeSend(int aIndex, BYTE type, char* message, ...)
{
	char buff[256] = { 0 };

	va_list arg;

	va_start(arg, message);

	vsprintf_s(buff, message, arg);

	va_end(arg);

	int size = strlen(buff);

	size = ((size > MAX_MESSAGE_SIZE) ? MAX_MESSAGE_SIZE : size);

	PMSG_NOTICE_SEND pMsg;

	pMsg.header.set(0x0D, (sizeof(pMsg) - (sizeof(pMsg.message) - (size + 1))));

	pMsg.type = type;

	memcpy(pMsg.message, buff, size);

	pMsg.message[size] = 0;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CNotice::GCNoticeSendToAll(BYTE type, char* message, ...)
{
	char buff[256] = { 0 };

	va_list arg;

	va_start(arg, message);

	vsprintf_s(buff, message, arg);

	va_end(arg);

	int size = strlen(buff);

	size = ((size > MAX_MESSAGE_SIZE) ? MAX_MESSAGE_SIZE : size);

	PMSG_NOTICE_SEND pMsg;

	pMsg.header.set(0x0D, (sizeof(pMsg) - (sizeof(pMsg.message) - (size + 1))));

	pMsg.type = type;

	memcpy(pMsg.message, buff, size);

	pMsg.message[size] = 0;

	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnectedGP(n) != 0)
		{
			DataSend(n, (BYTE*)&pMsg, pMsg.header.size);
		}
	}
}

void CNotice::GCNoticeSendToAll(BYTE type, int message, ...)
{
	PMSG_NOTICE_SEND pMsg;

	pMsg.type = type;

	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnectedGP(n) != 0)
		{
			char buff[256] = { 0 };

			va_list arg;

			va_start(arg, message);

			vsprintf_s(buff, gMessage.GetTextMessage(message, gObj[n].Lang), arg);

			va_end(arg);

			int size = strlen(buff);

			size = ((size > MAX_MESSAGE_SIZE) ? MAX_MESSAGE_SIZE : size);

			pMsg.header.set(0x0D, (sizeof(pMsg) - (sizeof(pMsg.message) - (size + 1))));

			memcpy(pMsg.message, buff, size);

			pMsg.message[size] = 0;

			DataSend(n, (BYTE*)&pMsg, pMsg.header.size);
		}
	}
}