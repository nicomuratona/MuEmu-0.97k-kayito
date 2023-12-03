#include "stdafx.h"
#include "Language.h"
#include "Protocol.h"

Language gLanguage;

Language::Language()
{
	this->iLanguageValues[LANGUAGE_ENGLISH] = std::make_pair<std::string, std::string>("English", "Eng");
	this->iLanguageValues[LANGUAGE_SPANISH] = std::make_pair<std::string, std::string>("Spanish", "Spn");
	this->iLanguageValues[LANGUAGE_PORTUGUESE] = std::make_pair<std::string, std::string>("Portuguese", "Por");

	this->LangNum = LANGUAGE_ENGLISH;

	GetPrivateProfileString("Language", "LangSelection", "Eng", this->m_Language, 4, ".\\Config.ini");
}

Language::~Language()
{
	WritePrivateProfileString("Language", "LangSelection", this->m_Language, ".\\Config.ini");
}

void Language::Init()
{
	this->SetLanguage();

	SetDword(0x00511039, (DWORD)&this->m_TextBMD);

	SetDword(0x00510F26, (DWORD)&this->m_DialogBMD);
}

void Language::ReloadLanguage()
{
	this->SetLanguage();

	OpenTextData();

	OpenDialogFile(this->m_DialogBMD);

	this->SendLanguage();
}

void Language::SetLanguage()
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
}

void Language::SendLanguage()
{
	PMSG_SET_LANG_SEND pMsg;

	pMsg.header.setE(0xF1, 0x03, sizeof(pMsg));

	pMsg.lang = this->LangNum;

	gProtocol.DataSend((BYTE*)&pMsg, pMsg.header.size);
}