#pragma once

#include "Protocol.h"

enum eLanguages
{
	LANGUAGE_ENGLISH = 0,
	LANGUAGE_SPANISH = 1,
	LANGUAGE_PORTUGUESE = 2,
	MAX_LANGUAGE
};

class Language
{
public:

	Language();

	virtual ~Language();

	void Init();

	void SendLanguage();

public:

	int LangNum;

	char m_Language[4];

	char m_TextBMD[100];

	char m_DialogBMD[100];
};

extern Language gLanguage;