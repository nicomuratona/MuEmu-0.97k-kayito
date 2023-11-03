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
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		printf(MEM_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (!lpMemScript->SetBuffer(path))
	{
		printf(lpMemScript->GetLastError());

		delete lpMemScript;

		return;
	}

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

			CUSTOM_GLOW_INFO info;

			info.ItemIndex = GET_ITEM(lpMemScript->GetNumber(), lpMemScript->GetAsNumber());

			info.ColorR = lpMemScript->GetAsNumber();

			info.ColorG = lpMemScript->GetAsNumber();

			info.ColorB = lpMemScript->GetAsNumber();

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		printf(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CCustomGlow::SetInfo(CUSTOM_GLOW_INFO info)
{
	if (info.ItemIndex < 0 || info.ItemIndex >= MAX_ITEM)
	{
		return;
	}

	this->m_CustomGlowInfo[info.ItemIndex] = info;
}