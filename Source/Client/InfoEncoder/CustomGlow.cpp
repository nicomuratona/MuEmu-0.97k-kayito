#include "stdafx.h"
#include "CustomGlow.h"

CCustomGlow gCustomGlow;

CCustomGlow::CCustomGlow()
{
	this->Init();
}

CCustomGlow::~CCustomGlow()
{

}

void CCustomGlow::Init()
{
	for (int n = 0; n < MAX_ITEM; n++)
	{
		this->m_CustomGlowInfo[n].ItemIndex = -1;
	}
}

void CCustomGlow::Load(char* path)
{
	CReadScript* lpReadScript = new CReadScript;

	if (lpReadScript == NULL)
	{
		printf(READ_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (!lpReadScript->Load(path))
	{
		printf(READ_SCRIPT_FILE_ERROR, path);

		delete lpReadScript;

		return;
	}

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

			CUSTOM_GLOW_INFO info;

			info.ItemIndex = GET_ITEM(lpReadScript->GetNumber(), lpReadScript->GetAsNumber());

			info.ColorR = lpReadScript->GetAsNumber();

			info.ColorG = lpReadScript->GetAsNumber();

			info.ColorB = lpReadScript->GetAsNumber();

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		printf(lpReadScript->GetError());
	}

	delete lpReadScript;
}

void CCustomGlow::SetInfo(CUSTOM_GLOW_INFO info)
{
	if (info.ItemIndex < 0 || info.ItemIndex >= MAX_ITEM)
	{
		return;
	}

	this->m_CustomGlowInfo[info.ItemIndex] = info;
}