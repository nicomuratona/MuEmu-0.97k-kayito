#include "stdafx.h"
#include "OptionsMenu.h"
#include "Sound.h"

COptionsMenu gOptionsMenu;

COptionsMenu::COptionsMenu()
{
	this->GeneralWidth = 120.0f;

	this->GeneralHeight = 22.0f;

	this->GeneralPosX = ImgCenterScreenPosX(this->GeneralWidth);

	this->GeneralPosY = 30.0f;
}

COptionsMenu::~COptionsMenu()
{

}

void COptionsMenu::Init()
{
	SetCompleteHook(0xE9, 0x0051B3CA, &this->OptionsWindowUI);

	SetCompleteHook(0xE9, 0x00515954, &this->OptionsWindowControl);
}

_declspec(naked) void COptionsMenu::OptionsWindowUI()
{
	static DWORD jmpBack = 0x0051C173;

	_asm
	{
		Pushad;
	}

	gOptionsMenu.RenderOptionsMenu();

	_asm
	{
		Popad;
		Jmp[jmpBack];
	}
}

__declspec(naked) void COptionsMenu::OptionsWindowControl()
{
	static DWORD jmpBack = 0x0051AE80;

	_asm
	{
		Pushad;
	}

	if (gOptionsMenu.CheckOptionTitle())
	{
		goto EXIT;
	}

	if (gOptionsMenu.CheckAutomaticAttack())
	{
		goto EXIT;
	}

	if (gOptionsMenu.CheckSoundOnWhisper())
	{
		goto EXIT;
	}

	if (gOptionsMenu.CheckVolume())
	{
		goto EXIT;
	}

	if (gOptionsMenu.CheckClose())
	{
		goto EXIT;
	}

EXIT:

	_asm
	{
		Popad;
		Jmp[jmpBack];
	}
}

void COptionsMenu::RenderOptionsMenu()
{
	this->RenderOptionTitle();

	this->RenderAutomaticAttack();

	this->RenderSoundOnWhisper();

	this->RenderVolume();

	this->RenderClose();
}

void COptionsMenu::RenderOptionTitle()
{
	RenderBitmap(240, this->GeneralPosX, this->GeneralPosY, this->GeneralWidth, this->GeneralHeight, 0.0f, 0.0f, 213.0f / 256.0f, 1.0f, true, true);

	if (IsWorkZone((int)this->GeneralPosX, (int)this->GeneralPosY, (int)this->GeneralWidth, (int)this->GeneralHeight))
	{
		glColor3f(0.8f, 0.6f, 0.4f);

		EnableAlphaBlend();

		RenderBitmap(240, this->GeneralPosX, this->GeneralPosY, this->GeneralWidth, this->GeneralHeight, 0.0f, 0.0f, 213.0f / 256.0f, 1.0f, true, true);

		glColor3f(1.0f, 1.0f, 1.0f);

		DisableAlphaBlend();
	}

	EnableAlphaTest(true);

	RenderText((int)this->GeneralPosX, (int)this->GeneralPosY + 5, GetTextLine(385), (int)this->GeneralWidth * WindowWidth / 640, 1, NULL);
}

void COptionsMenu::RenderAutomaticAttack()
{
	float PosY = this->GeneralPosY + 30.0f;

	RenderBitmap(240, this->GeneralPosX, PosY, this->GeneralWidth, this->GeneralHeight, 0.0f, 0.0f, 213.0f / 256.0f, 1.0f, true, true);

	if (IsWorkZone((int)this->GeneralPosX, (int)PosY, (int)this->GeneralWidth, (int)this->GeneralHeight))
	{
		glColor3f(0.8f, 0.6f, 0.4f);

		EnableAlphaBlend();

		RenderBitmap(240, this->GeneralPosX, PosY, this->GeneralWidth, this->GeneralHeight, 0.0f, 0.0f, 213.0f / 256.0f, 1.0f, true, true);

		glColor3f(1.0f, 1.0f, 1.0f);

		DisableAlphaBlend();
	}

	char Text[64] = { 0 };

	sprintf_s(Text, "%s: %s", GetTextLine(386), m_bAutoAttack ? "On" : "Off");

	EnableAlphaTest(true);

	RenderText((int)this->GeneralPosX, (int)PosY + 5, Text, (int)this->GeneralWidth * WindowWidth / 640, 1, NULL);
}

void COptionsMenu::RenderSoundOnWhisper()
{
	float PosY = this->GeneralPosY + 60;

	RenderBitmap(240, this->GeneralPosX, PosY, this->GeneralWidth, this->GeneralHeight, 0.0f, 0.0f, 213.0f / 256.0f, 1.0f, true, true);

	if (IsWorkZone((int)this->GeneralPosX, (int)PosY, (int)this->GeneralWidth, (int)this->GeneralHeight))
	{
		glColor3f(0.8f, 0.6f, 0.4f);

		EnableAlphaBlend();

		RenderBitmap(240, this->GeneralPosX, PosY, this->GeneralWidth, this->GeneralHeight, 0.0f, 0.0f, 213.0f / 256.0f, 1.0f, true, true);

		glColor3f(1.0f, 1.0f, 1.0f);

		DisableAlphaBlend();
	}

	char Text[64] = { 0 };

	sprintf_s(Text, "%s: %s", GetTextLine(387), m_bWhisperSound ? "On" : "Off");

	EnableAlphaTest(true);

	RenderText((int)this->GeneralPosX, (int)PosY + 5, Text, (int)this->GeneralWidth* WindowWidth / 640, 1, NULL);
}

void COptionsMenu::RenderVolume()
{
	float PosY = this->GeneralPosY + 90;

	RenderBitmap(240, this->GeneralPosX, PosY, this->GeneralWidth, this->GeneralHeight, 0.0f, 0.0f, 213.0f / 256.0f, 1.0f, true, true);

	if (IsWorkZone((int)this->GeneralPosX, (int)PosY, (int)this->GeneralWidth, (int)this->GeneralHeight))
	{
		glColor3f(0.8f, 0.6f, 0.4f);

		EnableAlphaBlend();

		RenderBitmap(240, this->GeneralPosX, PosY, this->GeneralWidth, this->GeneralHeight, 0.0f, 0.0f, 213.0f / 256.0f, 1.0f, true, true);

		glColor3f(1.0f, 1.0f, 1.0f);

		DisableAlphaBlend();
	}

	PosY += 5;

	EnableAlphaTest(true);

	RenderText((int)this->GeneralPosX, (int)PosY, "Volume:", 50 * WindowWidth / 640, 1, NULL);

	float PosX = this->GeneralPosX + 51.0f;

	float Width = 58.0f;

	float Height = 8.0f;

	EnableAlphaTest(true);

	glColor3f(0.2f, 0.2f, 0.2f);

	RenderColor(PosX - 1, (float)PosY + 1.0f, Width + 2.0f, Height);

	glColor3f(0.0f, 0.0f, 0.0f);

	RenderColor(PosX, (float)PosY + 2.0f, Width, Height - 2.0f);

	glColor3f(1.0f, 1.0f, 1.0f);

	for (int i = 0; i < MAX_VOLUME_LEVEL; i++)
	{
		if (i >= gSound.VolumeLevel)
		{
			glColor3f(0.0f, 0.0f, 0.0f);
		}
		else
		{
			glColor3f(1.0f, 1.0f, 1.0f);
		}

		RenderColor(PosX, (float)PosY + 2.0f, (Width / MAX_VOLUME_LEVEL), Height - 2.0f);

		PosX += (Width / MAX_VOLUME_LEVEL);
	}

	DisableAlphaBlend();

	glColor3f(1.0f, 1.0f, 1.0f);
}

void COptionsMenu::RenderClose()
{
	float PosY = this->GeneralPosY + 120;

	RenderBitmap(240, this->GeneralPosX, PosY, this->GeneralWidth, this->GeneralHeight, 0.0f, 0.0f, 213.0f / 256.0f, 1.0f, true, true);

	if (IsWorkZone((int)this->GeneralPosX, (int)PosY, (int)this->GeneralWidth, (int)this->GeneralHeight))
	{
		glColor3f(0.8f, 0.6f, 0.4f);

		EnableAlphaBlend();

		RenderBitmap(240, this->GeneralPosX, PosY, this->GeneralWidth, this->GeneralHeight, 0.0f, 0.0f, 213.0f / 256.0f, 1.0f, true, true);

		glColor3f(1.0f, 1.0f, 1.0f);

		DisableAlphaBlend();
	}

	EnableAlphaTest(true);

	RenderText((int)this->GeneralPosX, (int)PosY + 5, GetTextLine(388), (int)this->GeneralWidth * WindowWidth / 640, 1, NULL);
}

bool COptionsMenu::CheckOptionTitle()
{
	if (IsWorkZone((int)this->GeneralPosX, (int)this->GeneralPosY, (int)this->GeneralWidth, (int)this->GeneralHeight) && MouseLButtonPush)
	{
		MouseLButtonPush = 0;

		MouseUpdateTime = 0;

		MouseUpdateTimeMax = 6;

		PlayBuffer(25, 0, 0);

		return true;
	}

	return false;
}

bool COptionsMenu::CheckAutomaticAttack()
{
	if (IsWorkZone((int)this->GeneralPosX, (int)this->GeneralPosY + 30, (int)this->GeneralWidth, (int)this->GeneralHeight) && MouseLButtonPush)
	{
		MouseLButtonPush = 0;

		MouseUpdateTime = 0;

		MouseUpdateTimeMax = 6;

		PlayBuffer(25, 0, 0);

		m_bAutoAttack ^= 1u;

		return true;
	}

	return false;
}

bool COptionsMenu::CheckSoundOnWhisper()
{
	if (IsWorkZone((int)this->GeneralPosX, (int)this->GeneralPosY + 60, (int)this->GeneralWidth, (int)this->GeneralHeight) && MouseLButtonPush)
	{
		MouseLButtonPush = 0;

		MouseUpdateTime = 0;

		MouseUpdateTimeMax = 6;

		PlayBuffer(25, 0, 0);

		m_bWhisperSound ^= 1u;

		return true;
	}

	return false;
}

bool COptionsMenu::CheckVolume()
{
	float PosX = this->GeneralPosX + 51.0f;

	int PosY = (int)this->GeneralPosY + 97;

	float Width = 58.0f;

	float Height = 8.0f;

	int newVolumeLevel = 0;

	int cursorInicial = (int)this->GeneralPosX + 51;

	while (MouseX < cursorInicial || MouseX > cursorInicial + (int)(Width / MAX_VOLUME_LEVEL) || MouseY < PosY || MouseY >(int)PosY + ((int)Height - 2) || !MouseLButtonPush)
	{
		cursorInicial += (int)(Width / MAX_VOLUME_LEVEL);

		newVolumeLevel++;

		if (cursorInicial >= (int)(PosX + Width + 2.0f))
		{
			return false;
		}
	}

	if (newVolumeLevel > MAX_VOLUME_LEVEL)
	{
		newVolumeLevel = MAX_VOLUME_LEVEL;
	}

	if (newVolumeLevel < 0)
	{
		newVolumeLevel = 0;
	}

	gSound.UpdateVolume(newVolumeLevel);

	return true;
}

bool COptionsMenu::CheckClose()
{
	if (IsWorkZone((int)this->GeneralPosX, (int)this->GeneralPosY + 120, (int)this->GeneralWidth, (int)this->GeneralHeight) && MouseLButtonPush)
	{
		MouseLButtonPush = 0;

		MouseUpdateTime = 0;

		MouseUpdateTimeMax = 6;

		PlayBuffer(25, 0, 0);

		ErrorMessage = *(BYTE*)0x083A7C28;

		*(BYTE*)0x083A7C28 = 0;

		return true;
	}

	return false;
}