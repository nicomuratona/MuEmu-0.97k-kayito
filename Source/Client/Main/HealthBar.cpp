#include "stdafx.h"
#include "HealthBar.h"
#include "Protect.h"

CHealthBar gHealthBar;

CHealthBar::CHealthBar()
{
	this->DeleteHealthBar = (GetPrivateProfileInt("Antilag", "DeleteHealthBar", 0, ".\\Config.ini") != 0);

	this->ClearHealthBar();
}

CHealthBar::~CHealthBar()
{
	char Text[10] = { 0 };

	wsprintf(Text, "%d", (this->DeleteHealthBar) ? 1 : 0);

	WritePrivateProfileString("Antilag", "DeleteHealthBar", Text, ".\\Config.ini");
}

void CHealthBar::ClearHealthBar()
{
	for (int n = 0; n < MAX_MAIN_VIEWPORT; n++)
	{
		this->m_MonsterHealthBar[n].index = 0xFFFF;

		this->m_MonsterHealthBar[n].type = 0;

		this->m_MonsterHealthBar[n].rateHP = 0;
	}
}

void CHealthBar::InsertHealthBar(WORD index, BYTE type, BYTE HP)
{
	for (int n = 0; n < MAX_MAIN_VIEWPORT; n++)
	{
		if (this->m_MonsterHealthBar[n].index == 0xFFFF)
		{
			this->m_MonsterHealthBar[n].index = index;

			this->m_MonsterHealthBar[n].type = type;

			this->m_MonsterHealthBar[n].rateHP = HP;

			return;
		}
	}
}

MONSTER_HEALTH_BAR* CHealthBar::GetHealthBar(WORD index, BYTE type)
{
	for (int n = 0; n < MAX_MAIN_VIEWPORT; n++)
	{
		if (this->m_MonsterHealthBar[n].index != 0xFFFF)
		{
			if (this->m_MonsterHealthBar[n].index == index && this->m_MonsterHealthBar[n].type == type)
			{
				return &this->m_MonsterHealthBar[n];
			}
		}
	}

	return 0;
}

void CHealthBar::Init()
{
	if ((gProtect.m_MainInfo.HealthBarType & 1) == 1)
	{
		SetCompleteHook(0xE8, 0x004BC0AE, &this->DrawHealthBar);
	}

	SetByte(0x004CB739, 0x4F);

	SetCompleteHook(0xE9, 0x004CB7AD, &this->DrawPointingHealthBar);
}

void CHealthBar::DrawHealthBar()
{
	((void(_cdecl*)())0x004BCA20)();

	if (gHealthBar.DeleteHealthBar)
	{
		return;
	}

	float LifeBarWidth = 70.0f;

	for (int n = 0; n < MAX_MAIN_VIEWPORT; n++)
	{
		DWORD ViewportAddress = *(DWORD*)(0x07ABF5D0) + (n * 916);

		if (!ViewportAddress)
		{
			continue;
		}

		if (*(BYTE*)(ViewportAddress) == 0)
		{
			continue;
		}

		MONSTER_HEALTH_BAR* lpHealthBar = gHealthBar.GetHealthBar(*(WORD*)(ViewportAddress + 0x1DC), *(BYTE*)(ViewportAddress + 0x84));

		if (lpHealthBar == 0)
		{
			continue;
		}

		if (*(WORD*)(ViewportAddress + 0x2EB) == 200) //Soccer Ball
		{
			continue;
		}

		vec3_t Angle = { 0.0f, 0.0f, 0.0f };

		Angle[0] = *(float*)(ViewportAddress + 0x10);

		Angle[1] = *(float*)(ViewportAddress + 0x14);

		Angle[2] = *(float*)(ViewportAddress + 0x12C) + *(float*)(ViewportAddress + 0x18) + 100.0f;

		int PosX = 0, PosY = 0;

		Projection(Angle, &PosX, &PosY);

		PosX -= (int)floor((double)LifeBarWidth / 2.0);

		if (IsWorkZone(PosX, PosY, (int)LifeBarWidth, 6))
		{
			EnableAlphaTest(true);

			char LifeDisplay[64];

			wsprintf(LifeDisplay, "%s: %d%%", (char*)(ViewportAddress + 0x1C1), lpHealthBar->rateHP);

			SelectObject(m_hFontDC, g_hFont);

			SetBackgroundTextColor = Color4b(0, 0, 0, 128);

			SetTextColor = Color4b(255, 255, 255, 255);

			RenderText(CenterTextPosX(LifeDisplay, PosX + ((int)LifeBarWidth / 2)), PosY - 8, LifeDisplay, 0, RT3_SORT_LEFT, NULL);
		}

		EnableAlphaTest(true);

		glColor4f(0.0f, 0.0f, 0.0f, 0.5f);

		RenderColor((float)PosX, (float)PosY, LifeBarWidth, 6.0f);

		glColor4f(0.8f, 0.0f, 0.0f, 1.0f);

		RenderColor((float)PosX + 2, (float)PosY + 2, ((LifeBarWidth - 4) * lpHealthBar->rateHP) / 100, 2.0f);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		glEnable(GL_TEXTURE_2D);

		DisableAlphaBlend();
	}
}

__declspec(naked) void CHealthBar::DrawPointingHealthBar()
{
	static DWORD jmpBack = 0x004CB9D5;
	static DWORD ViewportAddress;
	static float PosX, PosY;
	static char* MonsterName;

	static MONSTER_HEALTH_BAR* lpHealthBar;
	static float LifeBarWidth = 200.0f;
	static char LifeDisplay[64];

	_asm
	{
		Pushad;
	}

	if (gHealthBar.DeleteHealthBar)
	{
		goto EXIT;
	}

	ViewportAddress = CharactersClient + (SelectedCharacter * 916);

	if (!ViewportAddress)
	{
		goto EXIT;
	}

	if (*(WORD*)(ViewportAddress + 0x2EB) == 200) //Soccer Ball
	{
		goto EXIT;
	}

	if ((gProtect.m_MainInfo.HealthBarType & 2) != 2)
	{
		goto NORMAL_NAME;
	}

	lpHealthBar = gHealthBar.GetHealthBar(*(WORD*)(ViewportAddress + 0x1DC), *(BYTE*)(ViewportAddress + 0x84));

	if (lpHealthBar == 0)
	{
		goto EXIT;
	}

	PosX = ImgCenterScreenPosX(LifeBarWidth);

	PosY = 10.0f;

	EnableAlphaTest(true);

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

	RenderColor(PosX, PosY, LifeBarWidth, 12.0f);

	glColor4f(0.5f, 0.0f, 0.0f, 1.0f);

	PosX += 2.0f;

	PosY += 2.0f;

	RenderColor(PosX, PosY, ((LifeBarWidth - 4.0f) * lpHealthBar->rateHP) / 100.0f, 8.0f);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);

	MonsterName = (char*)(ViewportAddress + 0x1C1);

	EnableAlphaTest(true);

	wsprintf(LifeDisplay, "%s: %d%%", MonsterName, lpHealthBar->rateHP);

	SelectObject(m_hFontDC, g_hFont);

	SetBackgroundTextColor = Color4b(0, 0, 0, 0);

	SetTextColor = Color4b(255, 255, 255, 255);

	RenderText((int)PosX, (int)PosY, LifeDisplay, REAL_WIDTH((int)LifeBarWidth), RT3_SORT_CENTER, NULL);

	DisableAlphaBlend();

	goto EXIT;

NORMAL_NAME:

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);

	MonsterName = (char*)(ViewportAddress + 0x1C1);

	EnableAlphaTest(true);

	PosY = 10.0f;

	SelectObject(m_hFontDC, g_hFont);

	SetBackgroundTextColor = Color4b(100, 0, 0, 255);

	SetTextColor = Color4b(255, 255, 255, 255);

	RenderText(CenterTextPosX(MonsterName, 320), (int)PosY, MonsterName, 0, RT3_SORT_LEFT, NULL);

	DisableAlphaBlend();

	SetBackgroundTextColor = Color4b(0, 0, 0, 0);

EXIT:

	_asm
	{
		Popad;
		Jmp[jmpBack];
	}
}