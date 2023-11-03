#include "stdafx.h"
#include "Message.h"
#include "MemScript.h"
#include "Util.h"

CMessage gMessage;

CMessage::CMessage()
{
	memset(this->m_DefaultMessage, 0, sizeof(this->m_DefaultMessage));

	for (int i = 0; i < MAX_LANGUAGE; i++)
	{
		this->m_MessageInfo[i].clear();
	}
}

CMessage::~CMessage()
{

}

void CMessage::Load(char* path, int lang)
{
	if (lang < 0 || lang >= MAX_LANGUAGE)
	{
		ErrorMessageBox("Invalid language number on Message::Load: %d", lang);

		return;
	}

	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());

		delete lpMemScript;

		return;
	}

	this->m_MessageInfo[lang].clear();

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if (strcmp("end", lpMemScript->GetString()) == 0)
			{
				break;
			}

			MESSAGE_INFO info;

			info.Index = lpMemScript->GetNumber();

			strcpy_s(info.Message, lpMemScript->GetAsString());

			this->m_MessageInfo[lang].insert(std::pair<int, MESSAGE_INFO>(info.Index, info));
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

char* CMessage::GetTextMessage(int index, int lang)
{
	if (lang >= 0 && lang < MAX_LANGUAGE)
	{
		std::map<int, MESSAGE_INFO>::iterator it = this->m_MessageInfo[lang].find(index);

		if (it != this->m_MessageInfo[lang].end())
		{
			return it->second.Message;
		}
	}

	wsprintf(this->m_DefaultMessage, "Could not find message %d, lang %d!", index, lang);

	return this->m_DefaultMessage;
}