#include "stdafx.h"
#include "Message.h"
#include "ReadScript.h"
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

	CReadScript* lpReadScript = new CReadScript;

	if (lpReadScript == NULL)
	{
		ErrorMessageBox(READ_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (!lpReadScript->Load(path))
	{
		ErrorMessageBox(READ_SCRIPT_FILE_ERROR, path);

		delete lpReadScript;

		return;
	}

	this->m_MessageInfo[lang].clear();

	try
	{
		eTokenResult token;

		while (true)
		{
			token = lpReadScript->GetToken();

			if (token == TOKEN_END || token == TOKEN_END_SECTION)
			{
				break;
			}

			MESSAGE_INFO info;

			info.Index = lpReadScript->GetNumber();

			strcpy_s(info.Message, lpReadScript->GetAsString());

			this->m_MessageInfo[lang].insert(std::pair<int, MESSAGE_INFO>(info.Index, info));
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript->GetError());
	}

	delete lpReadScript;
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