#include "stdafx.h"
#include "Language.h"

Language gLanguage;

Language::Language()
{
	HKEY key;

	if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Webzen\\Mu\\Config", 0, KEY_ALL_ACCESS, &key) == ERROR_SUCCESS)
	{
		DWORD type = REG_SZ, size = sizeof(this->m_Language);

		if (RegQueryValueEx(key, "LangSelection", nullptr, &type, (BYTE*)this->m_Language, &size) != ERROR_SUCCESS)
		{
			strcpy_s(this->m_Language, "Eng");
		}

		RegCloseKey(key);
	}
}

Language::~Language()
{

}

void Language::Init()
{
	if (_strcmpi(this->m_Language, "Eng") == 0)
	{
		strcpy_s(this->m_TextBMD, "Data\\Local\\Text_Eng.bmd");

		strcpy_s(this->m_DialogBMD, "Data\\Local\\Dialog_Eng.bmd");

		this->LangNum = LANGUAGE_ENGLISH;
	}
	else if (_strcmpi(this->m_Language, "Spn") == 0)
	{
		strcpy_s(this->m_TextBMD, "Data\\Local\\Text_Spn.bmd");

		strcpy_s(this->m_DialogBMD, "Data\\Local\\Dialog_Spn.bmd");

		this->LangNum = LANGUAGE_SPANISH;
	}
	else if (_strcmpi(this->m_Language, "Por") == 0)
	{
		strcpy_s(this->m_TextBMD, "Data\\Local\\Text_Por.bmd");

		strcpy_s(this->m_DialogBMD, "Data\\Local\\Dialog_Por.bmd");

		this->LangNum = LANGUAGE_PORTUGUESE;
	}
	else
	{
		strcpy_s(this->m_TextBMD, "Data\\Local\\Text_Eng.bmd");

		strcpy_s(this->m_DialogBMD, "Data\\Local\\Dialog_Eng.bmd");

		this->LangNum = LANGUAGE_ENGLISH;
	}

	SetDword(0x00511039, (DWORD)&this->m_TextBMD);

	SetDword(0x00510F26, (DWORD)&this->m_DialogBMD);
}

void Language::SendLanguage()
{
	PMSG_SET_LANG_SEND pMsg;

	pMsg.header.setE(0xF1, 0x03, sizeof(pMsg));

	pMsg.lang = this->LangNum;

	DataSend((BYTE*)&pMsg, pMsg.header.size);
}