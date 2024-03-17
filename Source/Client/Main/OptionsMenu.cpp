#include "stdafx.h"
#include "OptionsMenu.h"
#include "Font.h"
#include "Language.h"
#include "Sound.h"
#include "Window.h"

COptionsMenu gOptionsMenu;

COptionsMenu::COptionsMenu()
{
	this->CurrentOption = OPTION_NONE;

	memset(this->Resolutions, 0, sizeof(this->Resolutions));

	this->BoxWidth = 120;

	this->BoxHeight = 22;

	this->MainPosX = ImgCenterScreenPosX((float)this->BoxWidth);

	this->MainPosY = 60.0f;
}

COptionsMenu::~COptionsMenu()
{

}

void COptionsMenu::Init()
{
	SetCompleteHook(0xE8, 0x0051AF65, &this->CheckErrorMessage);

	SetCompleteHook(0xE9, 0x0051B3CA, &this->OptionsWindowUI);

	SetCompleteHook(0xE9, 0x00515954, &this->OptionsWindowControl);

	MemorySet(0x004C0026, 0x90, 13);
}

void COptionsMenu::CheckErrorMessage()
{
	if (!ErrorMessage)
	{
		gOptionsMenu.CurrentOption = OPTION_NONE;
	}

	DisableAlphaBlend();
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

	if (gOptionsMenu.CheckOptionsMenu())
	{
		goto EXIT;
	}

	MouseLButton = false;

	MouseLButtonPush = false;

	MouseUpdateTime = 0;

	MouseUpdateTimeMax = 6;

EXIT:

	_asm
	{
		Popad;
		Jmp[jmpBack];
	}
}

void COptionsMenu::RenderOptionsMenu()
{
	this->backupBgTextColor = SetBackgroundTextColor;

	this->backupTextColor = SetTextColor;

	switch (this->CurrentOption)
	{
		case OPTION_GENERAL:
		{
			this->RenderGeneral();

			break;
		}

		case OPTION_WINDOW:
		{
			this->RenderScreen();

			break;
		}

		case OPTION_FONT:
		{
			this->RenderFont();

			break;
		}

		default:
		{
			this->RenderAllOptions();

			break;
		}
	}

	SetBackgroundTextColor = this->backupBgTextColor;

	SetTextColor = this->backupTextColor;
}

bool COptionsMenu::CheckOptionsMenu()
{
	switch (this->CurrentOption)
	{
		case OPTION_GENERAL:
		{
			return this->CheckGeneral();

			break;
		}

		case OPTION_WINDOW:
		{
			return this->CheckScreen();

			break;
		}

		case OPTION_FONT:
		{
			return this->CheckFont();

			break;
		}

		default:
		{
			return this->CheckAllOptions();

			break;
		}
	}

	return false;
}

void COptionsMenu::RenderAllOptions()
{
	float Width = (float)this->BoxWidth;

	float Height = (float)this->BoxHeight;

	float PosX = this->MainPosX;

	float PosY = this->MainPosY;

	SetBackgroundTextColor = Color4b(255, 255, 255, 0);
	SetTextColor = Color4b(255, 255, 255, 255);

	char Text[32] = { 0 };

	/* OPTIONS */
	DisableAlphaBlend();
	glColor3f(0.8f, 0.6f, 0.4f);
	RenderBitmap(240, PosX, PosY, Width, Height, 0.0f, 0.0f, 213.0f / 256.0f, 64.0f / 64.0f, true, true);
	sprintf_s(Text, "%s", GetTextLine(385));
	EnableAlphaTest(true);
	RenderText((int)PosX, CenterTextPosY(Text, (int)(PosY + (Height / 2.0f))), Text, REAL_WIDTH((int)Width), RT3_SORT_CENTER, NULL);
	PosY += (Height + 7.0f);

	/* GENERAL */
	this->RenderBox(PosX, PosY, Width, Height);
	sprintf_s(Text, "%s", GetTextLine(910));
	EnableAlphaTest(true);
	RenderText((int)PosX, CenterTextPosY(Text, (int)(PosY + (Height / 2.0f))), Text, REAL_WIDTH((int)Width), RT3_SORT_CENTER, NULL);
	PosY += (Height + 7.0f);

	/* SCREEN */
	this->RenderBox(PosX, PosY, Width, Height);
	sprintf_s(Text, "%s", GetTextLine(911));
	EnableAlphaTest(true);
	RenderText((int)PosX, CenterTextPosY(Text, (int)(PosY + (Height / 2.0f))), Text, REAL_WIDTH((int)Width), RT3_SORT_CENTER, NULL);
	PosY += (Height + 7.0f);

	/* FONT */
	this->RenderBox(PosX, PosY, Width, Height);
	sprintf_s(Text, "%s", GetTextLine(912));
	EnableAlphaTest(true);
	RenderText((int)PosX, CenterTextPosY(Text, (int)(PosY + (Height / 2.0f))), Text, REAL_WIDTH((int)Width), RT3_SORT_CENTER, NULL);
	PosY += (Height + 7.0f);

	/* CLOSE */
	this->RenderBox(PosX, PosY, Width, Height);
	sprintf_s(Text, "%s", GetTextLine(388));
	EnableAlphaTest(true);
	RenderText((int)PosX, CenterTextPosY(Text, (int)(PosY + (Height / 2.0f))), Text, REAL_WIDTH((int)Width), RT3_SORT_CENTER, NULL);
	PosY += (Height + 7.0f);
}

bool COptionsMenu::CheckAllOptions()
{
	int Width = this->BoxWidth;

	int Height = this->BoxHeight;

	int PosX = (int)this->MainPosX;

	int PosY = (int)this->MainPosY + (Height + 7);

	for (int i = OPTION_GENERAL; i < MAX_OPTIONS; i++)
	{
		if (IsWorkZone(PosX, PosY, Width, Height))
		{
			if (MouseLButton && MouseLButtonPush)
			{
				MouseLButtonPush = false;

				MouseUpdateTime = 0;

				MouseUpdateTimeMax = 6;

				PlayBuffer(25, 0, 0);

				this->CurrentOption = (eMenuOptions)i;

				return true;
			}
		}

		PosY += (Height + 7);
	}

	if (this->CheckClose(PosX, PosY))
	{
		return true;
	}

	return false;
}

void COptionsMenu::RenderGeneral()
{
	float PosX = this->MainPosX;

	float PosY = this->MainPosY;

	SetBackgroundTextColor = Color4b(255, 255, 255, 0);
	SetTextColor = Color4b(255, 255, 255, 255);

	this->RenderGeneralTitle(PosX, PosY);
	PosY += ((float)this->BoxHeight + 7.0f);

	if (SceneFlag == CHARACTER_SCENE || SceneFlag == MAIN_SCENE)
	{
		this->RenderLanguage(PosX, PosY);
		PosY += ((float)this->BoxHeight + 7.0f);
	}

	this->RenderAutomaticAttack(PosX, PosY);
	PosY += ((float)this->BoxHeight + 7.0f);

	this->RenderSoundOnWhisper(PosX, PosY);
	PosY += ((float)this->BoxHeight + 7.0f);

	this->RenderSoundVolume(PosX, PosY);
	PosY += ((float)this->BoxHeight + 7.0f);

	this->RenderMusicVolume(PosX, PosY);
	PosY += ((float)this->BoxHeight + 7.0f);

	this->RenderMusicControls(PosX, PosY);
	PosY += ((float)this->BoxHeight + 7.0f);

	this->RenderBack(PosX, PosY);
}

bool COptionsMenu::CheckGeneral()
{
	int PosX = (int)this->MainPosX;

	int PosY = (int)this->MainPosY + (this->BoxHeight + 7);

	if (SceneFlag == CHARACTER_SCENE || SceneFlag == MAIN_SCENE)
	{
		if (this->CheckLanguage(PosX, PosY))
		{
			return true;
		}

		PosY += (this->BoxHeight + 7);
	}

	if (this->CheckAutomaticAttack(PosX, PosY))
	{
		return true;
	}

	PosY += (this->BoxHeight + 7);

	if (this->CheckSoundOnWhisper(PosX, PosY))
	{
		return true;
	}

	PosY += (this->BoxHeight + 7);

	if (this->CheckSoundVolume(PosX, PosY))
	{
		return true;
	}

	PosY += (this->BoxHeight + 7);

	if (this->CheckMusicVolume(PosX, PosY))
	{
		return true;
	}

	PosY += (this->BoxHeight + 7);

	if (this->CheckMusicControls(PosX, PosY))
	{
		return true;
	}

	PosY += (this->BoxHeight + 7);

	if (this->CheckBack(PosX, PosY))
	{
		return true;
	}

	return false;
}

void COptionsMenu::RenderGeneralTitle(float PosX, float PosY)
{
	DisableAlphaBlend();

	glColor3f(0.8f, 0.6f, 0.4f);

	RenderBitmap(240, PosX, PosY, (float)this->BoxWidth, (float)this->BoxHeight, 0.0f, 0.0f, 213.0f / 256.0f, 64.0f / 64.0f, true, true);

	char Text[64] = { 0 };

	sprintf_s(Text, "%s", GetTextLine(910));

	EnableAlphaTest(true);

	RenderText((int)PosX, CenterTextPosY(Text, ((int)PosY + (this->BoxHeight / 2))), Text, REAL_WIDTH(this->BoxWidth), RT3_SORT_CENTER, NULL);
}

void COptionsMenu::RenderLanguage(float PosX, float PosY)
{
	DisableAlphaBlend();

	glColor3f(1.0f, 1.0f, 1.0f);

	RenderBitmap(240, PosX, PosY, (float)this->BoxWidth, (float)this->BoxHeight, 0.0f, 0.0f, 213.0f / 256.0f, 64.0f / 64.0f, true, true);

	char Text[64] = { 0 };

	sprintf_s(Text, "%s", gLanguage.iLanguageValues[gLanguage.LangNum].first.c_str());

	EnableAlphaTest(true);

	RenderText((int)PosX, CenterTextPosY(Text, ((int)PosY + (this->BoxHeight / 2))), Text, REAL_WIDTH(this->BoxWidth), RT3_SORT_CENTER, NULL);

	if (gLanguage.LangNum > LANGUAGE_ENGLISH)
	{
		if (IsWorkZone((int)PosX + 2, (int)PosY + 2, this->BoxHeight, this->BoxHeight))
		{
			if (MouseLButtonPush)
			{
				RenderBitmap(0x100, PosX + 2.0f, PosY + 2.0f, (float)this->BoxHeight - 4.0f, (float)this->BoxHeight - 4.0f, 0.0f, 0.0f, 32.0f / 32.0f, 32.0f / 32.0f, true, true);
			}
			else
			{
				RenderBitmap(0xFF, PosX + 2.0f, PosY + 2.0f, (float)this->BoxHeight - 4.0f, (float)this->BoxHeight - 4.0f, 0.0f, 0.0f, 32.0f / 32.0f, 32.0f / 32.0f, true, true);
			}
		}
		else
		{
			RenderBitmap(0xFE, PosX + 2.0f, PosY + 2.0f, (float)this->BoxHeight - 4.0f, (float)this->BoxHeight - 4.0f, 0.0f, 0.0f, 32.0f / 32.0f, 32.0f / 32.0f, true, true);
		}
	}

	if (gLanguage.LangNum < LANGUAGE_PORTUGUESE)
	{
		if (IsWorkZone((int)PosX + (this->BoxWidth - this->BoxHeight) - 2, (int)PosY + 2, this->BoxHeight, this->BoxHeight))
		{
			if (MouseLButtonPush)
			{
				MyRenderBitmapRotate(0x100, (PosX + 2.0f) + (float)(this->BoxWidth - this->BoxHeight), PosY + 2.0f, (float)this->BoxHeight - 4.0f, (float)this->BoxHeight - 4.0f, 180.0f, 0.0f, 0.0f, 32.0f / 32.0f, 32.0f / 32.0f);
			}
			else
			{
				MyRenderBitmapRotate(0xFF, (PosX + 2.0f) + (float)(this->BoxWidth - this->BoxHeight), PosY + 2.0f, (float)this->BoxHeight - 4.0f, (float)this->BoxHeight - 4.0f, 180.0f, 0.0f, 0.0f, 32.0f / 32.0f, 32.0f / 32.0f);
			}
		}
		else
		{
			MyRenderBitmapRotate(0xFE, (PosX + 2.0f) + (float)(this->BoxWidth - this->BoxHeight), PosY + 2.0f, (float)this->BoxHeight - 4.0f, (float)this->BoxHeight - 4.0f, 180.0f, 0.0f, 0.0f, 32.0f / 32.0f, 32.0f / 32.0f);
		}
	}
}

bool COptionsMenu::CheckLanguage(int PosX, int PosY)
{
	if (gLanguage.LangNum > LANGUAGE_ENGLISH)
	{
		if (IsWorkZone(PosX + 2, PosY + 2, this->BoxHeight, this->BoxHeight))
		{
			if (MouseLButton && MouseLButtonPush)
			{
				MouseLButtonPush = false;

				MouseUpdateTime = 0;

				MouseUpdateTimeMax = 6;

				PlayBuffer(25, 0, 0);

				sprintf_s(gLanguage.m_Language, "%s", gLanguage.iLanguageValues[gLanguage.LangNum - 1].second.c_str());

				gLanguage.ReloadLanguage();
			}

			return true;
		}
	}

	if (gLanguage.LangNum < LANGUAGE_PORTUGUESE)
	{
		if (IsWorkZone(PosX + (this->BoxWidth - this->BoxHeight) - 2, PosY + 2, this->BoxHeight, this->BoxHeight))
		{
			if (MouseLButton && MouseLButtonPush)
			{
				MouseLButtonPush = false;

				MouseUpdateTime = 0;

				MouseUpdateTimeMax = 6;

				PlayBuffer(25, 0, 0);

				sprintf_s(gLanguage.m_Language, "%s", gLanguage.iLanguageValues[gLanguage.LangNum + 1].second.c_str());

				gLanguage.ReloadLanguage();
			}

			return true;
		}
	}

	return false;
}

void COptionsMenu::RenderAutomaticAttack(float PosX, float PosY)
{
	this->RenderBox(PosX, PosY, (float)this->BoxWidth, (float)this->BoxHeight);

	char Text[64] = { 0 };

	sprintf_s(Text, "%s: %s", GetTextLine(386), m_bAutoAttack ? "On" : "Off");

	EnableAlphaTest(true);

	RenderText((int)PosX, CenterTextPosY(Text, ((int)PosY + (this->BoxHeight / 2))), Text, REAL_WIDTH(this->BoxWidth), RT3_SORT_CENTER, NULL);
}

bool COptionsMenu::CheckAutomaticAttack(int PosX, int PosY)
{
	if (IsWorkZone(PosX, PosY, this->BoxWidth, this->BoxHeight))
	{
		if (MouseLButton && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;

			MouseUpdateTimeMax = 6;

			PlayBuffer(25, 0, 0);

			m_bAutoAttack ^= 1u;
		}

		return true;
	}

	return false;
}

void COptionsMenu::RenderSoundOnWhisper(float PosX, float PosY)
{
	this->RenderBox(PosX, PosY, (float)this->BoxWidth, (float)this->BoxHeight);

	char Text[64] = { 0 };

	sprintf_s(Text, "%s: %s", GetTextLine(387), m_bWhisperSound ? "On" : "Off");

	EnableAlphaTest(true);

	RenderText((int)PosX, CenterTextPosY(Text, ((int)PosY + (this->BoxHeight / 2))), Text, REAL_WIDTH(this->BoxWidth), RT3_SORT_CENTER, NULL);
}

bool COptionsMenu::CheckSoundOnWhisper(int PosX, int PosY)
{
	if (IsWorkZone(PosX, PosY, this->BoxWidth, this->BoxHeight))
	{
		if (MouseLButton && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;

			MouseUpdateTimeMax = 6;

			PlayBuffer(25, 0, 0);

			m_bWhisperSound ^= 1u;
		}

		return true;
	}

	return false;
}

void COptionsMenu::RenderSoundVolume(float PosX, float PosY)
{
	this->RenderBox(PosX, PosY, (float)this->BoxWidth, (float)this->BoxHeight);

	float Width = (float)this->BoxWidth / 3.0f;

	char Text[64] = { 0 };

	sprintf_s(Text, "%s", GetTextLine(913));

	EnableAlphaTest(true);

	RenderText((int)PosX, CenterTextPosY(Text, ((int)PosY + (this->BoxHeight / 2))), Text, REAL_WIDTH((int)Width), RT3_SORT_CENTER, NULL);

	PosX += Width;

	Width *= 2.0f;

	Width -= 8.0f;

	float Height = 8.0f;

	PosY += (((float)this->BoxHeight - Height) / 2.0f);

	EnableAlphaTest(true);

	glColor3f(0.2f, 0.2f, 0.2f);

	RenderColor(PosX, PosY, Width + 2.0f, Height);

	glColor3f(0.0f, 0.0f, 0.0f);

	RenderColor(PosX + 1, PosY + 1.0f, Width, Height - 2.0f);

	glColor3f(1.0f, 1.0f, 1.0f);

	for (int i = 0; i < MAX_SOUND_LEVEL; i++)
	{
		if (i >= gSound.SoundVolumeLevel)
		{
			glColor3f(0.0f, 0.0f, 0.0f);
		}
		else
		{
			glColor3f(1.0f, 1.0f, 1.0f);
		}

		RenderColor(PosX + 1, PosY + 1.0f, (Width / MAX_SOUND_LEVEL), Height - 2.0f);

		PosX += (Width / MAX_SOUND_LEVEL);
	}

	DisableAlphaBlend();

	glColor3f(1.0f, 1.0f, 1.0f);
}

bool COptionsMenu::CheckSoundVolume(int PosX, int PosY)
{
	float Width = ((float)this->BoxWidth / 3.0f * 2.0f) - 8.0f;

	float Height = 8.0f;

	PosX += (this->BoxWidth / 3);

	PosY += ((this->BoxHeight - (int)Height) / 2);

	int newVolumeLevel = 0;

	int cursorInicial = PosX;

	while (MouseX < cursorInicial || MouseX > cursorInicial + (int)(Width / MAX_SOUND_LEVEL) || MouseY < PosY || MouseY > PosY + ((int)Height - 2) || !MouseLButtonPush)
	{
		cursorInicial += (int)(Width / MAX_SOUND_LEVEL);

		newVolumeLevel++;

		if (cursorInicial >= (PosX + (int)Width + 2))
		{
			return false;
		}
	}

	gSound.UpdateSoundVolumeLevel(newVolumeLevel);

	return true;
}

void COptionsMenu::RenderMusicVolume(float PosX, float PosY)
{
	this->RenderBox(PosX, PosY, (float)this->BoxWidth, (float)this->BoxHeight);

	float Width = (float)this->BoxWidth / 3.0f;

	char Text[64] = { 0 };

	sprintf_s(Text, "%s", GetTextLine(914));

	EnableAlphaTest(true);

	RenderText((int)PosX, CenterTextPosY(Text, ((int)PosY + (this->BoxHeight / 2))), Text, REAL_WIDTH((int)Width), RT3_SORT_CENTER, NULL);

	PosX += Width;

	Width *= 2.0f;

	Width -= 8.0f;

	float Height = 8.0f;

	PosY += (((float)this->BoxHeight - Height) / 2.0f);

	EnableAlphaTest(true);

	glColor3f(0.2f, 0.2f, 0.2f);

	RenderColor(PosX, PosY, Width + 2.0f, Height);

	glColor3f(0.0f, 0.0f, 0.0f);

	RenderColor(PosX + 1, PosY + 1.0f, Width, Height - 2.0f);

	glColor3f(1.0f, 1.0f, 1.0f);

	for (int i = 0; i < MAX_MUSIC_LEVEL; i++)
	{
		if (i >= gSound.MusicVolumeLevel)
		{
			glColor3f(0.0f, 0.0f, 0.0f);
		}
		else
		{
			glColor3f(1.0f, 1.0f, 1.0f);
		}

		RenderColor(PosX + 1, PosY + 1.0f, (Width / MAX_MUSIC_LEVEL), Height - 2.0f);

		PosX += (Width / MAX_MUSIC_LEVEL);
	}

	DisableAlphaBlend();

	glColor3f(1.0f, 1.0f, 1.0f);
}

bool COptionsMenu::CheckMusicVolume(int PosX, int PosY)
{
	float Width = ((float)this->BoxWidth / 3.0f * 2.0f) - 8.0f;

	float Height = 8.0f;

	PosX += (this->BoxWidth / 3);

	PosY += ((this->BoxHeight - (int)Height) / 2);

	int newVolumeLevel = 0;

	int cursorInicial = PosX;

	while (MouseX < cursorInicial || MouseX > cursorInicial + (int)(Width / MAX_MUSIC_LEVEL) || MouseY < PosY || MouseY > PosY + ((int)Height - 2) || !MouseLButtonPush)
	{
		cursorInicial += (int)(Width / MAX_MUSIC_LEVEL);

		newVolumeLevel++;

		if (cursorInicial >= (PosX + (int)Width + 2))
		{
			return false;
		}
	}

	gSound.UpdateMusicVolumeLevel(newVolumeLevel);

	return true;
}

void COptionsMenu::RenderMusicControls(float PosX, float PosY)
{
	float MaxWidth = (float)this->BoxWidth / 2.0f;

	float StartPosX = PosX + (MaxWidth / 2.0f);

	this->RenderBox(StartPosX, PosY, MaxWidth, (float)this->BoxHeight);

	float Width = ((float)this->BoxHeight / 2.0f);

	StartPosX += ((MaxWidth - Width) / 2.0f);

	PosY += (((float)this->BoxHeight - Width) / 2.0f);

	EnableAlphaTest(true);

	if (gSound.MusicPlaying)
	{
		glColor3f(1.0f, 0.0f, 0.0f);

		RenderColor(StartPosX, PosY, Width, Width);
	}
	else
	{
		glColor3f(0.0f, 1.0f, 0.0f);

		RenderTriangleColor(StartPosX, PosY, Width, Width);
	}

	DisableAlphaBlend();

	glColor3f(1.0f, 1.0f, 1.0f);
}

bool COptionsMenu::CheckMusicControls(int PosX, int PosY)
{
	int Width = this->BoxWidth / 2;

	int StartPosX = (int)PosX + (Width / 2);

	if (IsWorkZone(StartPosX, PosY, Width, this->BoxHeight))
	{
		if (MouseLButton && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;

			MouseUpdateTimeMax = 6;

			PlayBuffer(25, 0, 0);

			if (gSound.MusicPlaying)
			{
				gSound.ButtonStopMusic();
			}
			else
			{
				gSound.ButtonPlayMusic();
			}
		}

		return true;
	}

	return false;
}

void COptionsMenu::RenderScreen()
{
	float PosX = this->MainPosX;

	float PosY = this->MainPosY;

	SetBackgroundTextColor = Color4b(255, 255, 255, 0);
	SetTextColor = Color4b(255, 255, 255, 255);

	this->RenderScreenTitle(PosX, PosY);
	PosY += ((float)this->BoxHeight + 7.0f);

	this->RenderWindowMode(PosX, PosY);
	PosY += ((float)this->BoxHeight + 7.0f);

	this->RenderResolutions(PosX, PosY);

	this->RenderBack(PosX, PosY);
}

bool COptionsMenu::CheckScreen()
{
	int PosX = (int)this->MainPosX;

	int PosY = (int)this->MainPosY + (this->BoxHeight + 7);

	if (this->CheckWindowMode(PosX, PosY))
	{
		return true;
	}

	PosY += (this->BoxHeight + 7);

	if (this->CheckResolutions(PosX, PosY))
	{
		return true;
	}

	if (this->CheckBack(PosX, PosY))
	{
		return true;
	}

	return false;
}

void COptionsMenu::RenderScreenTitle(float PosX, float PosY)
{
	DisableAlphaBlend();

	glColor3f(0.8f, 0.6f, 0.4f);

	RenderBitmap(240, PosX, PosY, (float)this->BoxWidth, (float)this->BoxHeight, 0.0f, 0.0f, 213.0f / 256.0f, 64.0f / 64.0f, true, true);

	char Text[64] = { 0 };

	sprintf_s(Text, "%s", GetTextLine(911));

	EnableAlphaTest(true);

	RenderText((int)PosX, CenterTextPosY(Text, ((int)PosY + (this->BoxHeight / 2))), Text, REAL_WIDTH(this->BoxWidth), RT3_SORT_CENTER, NULL);
}

void COptionsMenu::RenderWindowMode(float PosX, float PosY)
{
	DisableAlphaBlend();

	glColor3f(1.0f, 0.4f, 0.4f);

	RenderBitmap(240, PosX, PosY, (float)this->BoxWidth, (float)this->BoxHeight, 0.0f, 0.0f, 213.0f / 256.0f, 64.0f / 64.0f, true, true);

	char Text[64] = { 0 };

	sprintf_s(Text, "%s", gWindow.iWindowValues[gWindow.m_WindowMode].c_str());

	EnableAlphaTest(true);

	RenderText((int)PosX, CenterTextPosY(Text, ((int)PosY + (BoxHeight / 2))), Text, REAL_WIDTH(this->BoxWidth), RT3_SORT_CENTER, NULL);

	if (gWindow.m_WindowMode > FULL_SCREEN)
	{
		if (IsWorkZone((int)PosX + 2, (int)PosY + 2, this->BoxHeight, this->BoxHeight))
		{
			if (MouseLButtonPush)
			{
				RenderBitmap(0x100, PosX + 2.0f, PosY + 2.0f, (float)this->BoxHeight - 4.0f, (float)this->BoxHeight - 4.0f, 0.0f, 0.0f, 32.0f / 32.0f, 32.0f / 32.0f, true, true);
			}
			else
			{
				RenderBitmap(0xFF, PosX + 2.0f, PosY + 2.0f, (float)this->BoxHeight - 4.0f, (float)this->BoxHeight - 4.0f, 0.0f, 0.0f, 32.0f / 32.0f, 32.0f / 32.0f, true, true);
			}
		}
		else
		{
			RenderBitmap(0xFE, PosX + 2.0f, PosY + 2.0f, (float)this->BoxHeight - 4.0f, (float)this->BoxHeight - 4.0f, 0.0f, 0.0f, 32.0f / 32.0f, 32.0f / 32.0f, true, true);
		}
	}

	if (gWindow.m_WindowMode < BORDERLESS)
	{
		if (IsWorkZone((int)PosX + (this->BoxWidth - this->BoxHeight) - 2, (int)PosY + 2, this->BoxHeight, this->BoxHeight))
		{
			if (MouseLButtonPush)
			{
				MyRenderBitmapRotate(0x100, (PosX + 2.0f) + (float)(this->BoxWidth - this->BoxHeight), PosY + 2.0f, (float)this->BoxHeight - 4.0f, (float)this->BoxHeight - 4.0f, 180.0f, 0.0f, 0.0f, 32.0f / 32.0f, 32.0f / 32.0f);
			}
			else
			{
				MyRenderBitmapRotate(0xFF, (PosX + 2.0f) + (float)(this->BoxWidth - this->BoxHeight), PosY + 2.0f, (float)this->BoxHeight - 4.0f, (float)this->BoxHeight - 4.0f, 180.0f, 0.0f, 0.0f, 32.0f / 32.0f, 32.0f / 32.0f);
			}
		}
		else
		{
			MyRenderBitmapRotate(0xFE, (PosX + 2.0f) + (float)(this->BoxWidth - this->BoxHeight), PosY + 2.0f, (float)this->BoxHeight - 4.0f, (float)this->BoxHeight - 4.0f, 180.0f, 0.0f, 0.0f, 32.0f / 32.0f, 32.0f / 32.0f);
		}
	}
}

bool COptionsMenu::CheckWindowMode(int PosX, int PosY)
{
	if (gWindow.m_WindowMode > FULL_SCREEN)
	{
		if (IsWorkZone(PosX + 2, PosY + 2, this->BoxHeight, this->BoxHeight))
		{
			if (MouseLButton && MouseLButtonPush)
			{
				MouseLButtonPush = false;

				MouseUpdateTime = 0;

				MouseUpdateTimeMax = 6;

				PlayBuffer(25, 0, 0);

				gWindow.SetWindowMode(gWindow.m_WindowMode - 1);

				gWindow.ChangeWindowState();
			}

			return true;
		}
	}

	if (gWindow.m_WindowMode < BORDERLESS)
	{
		if (IsWorkZone(PosX + (this->BoxWidth - this->BoxHeight) - 2, PosY + 2, this->BoxHeight, this->BoxHeight))
		{
			if (MouseLButton && MouseLButtonPush)
			{
				MouseLButtonPush = false;

				MouseUpdateTime = 0;

				MouseUpdateTimeMax = 6;

				PlayBuffer(25, 0, 0);

				gWindow.SetWindowMode(gWindow.m_WindowMode + 1);

				gWindow.ChangeWindowState();
			}

			return true;
		}
	}

	return false;
}

void COptionsMenu::RenderResolutions(float PosX, float& PosY)
{
	this->Resolutions[m_Resolution] = 1;

	char Text[64] = { 0 };

	for (int i = 0; i < MAX_RESOLUTION_VALUE; i++)
	{
		DisableAlphaBlend();

		glColor3f(1.0f, 1.0f, 1.0f);

		RenderBitmap(240, PosX, PosY, (float)this->BoxWidth, (float)this->BoxHeight, 0.0f, 0.0f, 213.0f / 256.0f, 64.0f / 64.0f, true, true);

		if (this->Resolutions[i])
		{
			glColor3f(1.0f, 1.0f, 0.0f);

			RenderBitmap(240, PosX, PosY, (float)this->BoxWidth, (float)this->BoxHeight, 0.0f, 0.0f, 213.0f / 256.0f, 64.0f / 64.0f, true, true);

			glColor3f(1.0f, 1.0f, 1.0f);

			DisableAlphaBlend();
		}
		else if (IsWorkZone((int)PosX, (int)PosY, this->BoxWidth, this->BoxHeight))
		{
			EnableAlphaBlend();

			glColor3f(0.8f, 0.6f, 0.4f);

			RenderBitmap(240, PosX, PosY, (float)this->BoxWidth, (float)this->BoxHeight, 0.0f, 0.0f, 213.0f / 256.0f, 64.0f / 64.0f, true, true);

			glColor3f(1.0f, 1.0f, 1.0f);

			DisableAlphaBlend();
		}

		sprintf_s(Text, "%d x %d", gWindow.iResolutionValues[i].first, gWindow.iResolutionValues[i].second);

		EnableAlphaTest(true);

		RenderText((int)PosX, CenterTextPosY(Text, ((int)PosY + (this->BoxHeight / 2))), Text, REAL_WIDTH(this->BoxWidth), RT3_SORT_CENTER, NULL);

		PosY += ((float)this->BoxHeight + 7.0f);
	}
}

bool COptionsMenu::CheckResolutions(int PosX, int& PosY)
{
	for (int i = 0; i < MAX_RESOLUTION_VALUE; i++)
	{
		if (IsWorkZone(PosX, PosY, this->BoxWidth, this->BoxHeight))
		{
			if (MouseLButton && MouseLButtonPush)
			{
				if (this->Resolutions[i] == 1)
				{
					return true;
				}

				MouseLButtonPush = false;

				MouseUpdateTime = 0;

				MouseUpdateTimeMax = 6;

				PlayBuffer(25, 0, 0);

				this->Resolutions[m_Resolution] = 0;

				gWindow.SetResolution(i);

				gWindow.ChangeWindowState();
			}

			return true;
		}

		PosY += (this->BoxHeight + 7);
	}

	return false;
}

void COptionsMenu::RenderFont()
{
	float PosX = this->MainPosX;

	float PosY = this->MainPosY;

	SetBackgroundTextColor = Color4b(255, 255, 255, 0);
	SetTextColor = Color4b(255, 255, 255, 255);

	this->RenderFontTitle(PosX, PosY);
	PosY += ((float)this->BoxHeight + 7.0f);

	this->RenderFontStyle(PosX, PosY);
	PosY += ((float)this->BoxHeight + 7.0f);

	this->RenderFontBold(PosX, PosY);
	PosY += ((float)this->BoxHeight + 7.0f);

	this->RenderFontItalic(PosX, PosY);
	PosY += ((float)this->BoxHeight + 7.0f);

	this->RenderFontButtons(PosX, PosY);
	PosY += ((float)this->BoxHeight + 7.0f);

	this->RenderResetFonts(PosX, PosY);
	PosY += ((float)this->BoxHeight + 7.0f);

	this->RenderBack(PosX, PosY);
}

bool COptionsMenu::CheckFont()
{
	int PosX = (int)this->MainPosX;

	int PosY = (int)this->MainPosY + (this->BoxHeight + 7);

	if (this->CheckFontStyle(PosX, PosY))
	{
		return true;
	}

	PosY += (this->BoxHeight + 7);

	if (this->CheckFontBold(PosX, PosY))
	{
		return true;
	}

	PosY += (this->BoxHeight + 7);

	if (this->CheckFontItalic(PosX, PosY))
	{
		return true;
	}

	PosY += (this->BoxHeight + 7);

	if (this->CheckFontButtons(PosX, PosY))
	{
		return true;
	}

	PosY += (this->BoxHeight + 7);

	if (this->CheckResetFonts(PosX, PosY))
	{
		return true;
	}

	PosY += (this->BoxHeight + 7);

	if (this->CheckBack(PosX, PosY))
	{
		return true;
	}

	return false;
}

void COptionsMenu::RenderFontTitle(float PosX, float PosY)
{
	DisableAlphaBlend();

	glColor3f(0.8f, 0.6f, 0.4f);

	RenderBitmap(240, PosX, PosY, (float)this->BoxWidth, (float)this->BoxHeight, 0.0f, 0.0f, 213.0f / 256.0f, 64.0f / 64.0f, true, true);

	char Text[64] = { 0 };

	sprintf_s(Text, "%s", GetTextLine(912));

	EnableAlphaTest(true);

	RenderText((int)PosX, CenterTextPosY(Text, ((int)PosY + (this->BoxHeight / 2))), Text, REAL_WIDTH(this->BoxWidth), RT3_SORT_CENTER, NULL);
}

void COptionsMenu::RenderFontStyle(float PosX, float& PosY)
{
	DisableAlphaBlend();

	glColor3f(1.0f, 1.0f, 1.0f);

	RenderBitmap(240, PosX, PosY, (float)this->BoxWidth, (float)this->BoxHeight, 0.0f, 0.0f, 213.0f / 256.0f, 64.0f / 64.0f, true, true);

	//GetTextFace(m_hFontDC, sizeof(gFont.MyFontFaceName), gFont.MyFontFaceName);

	char Text[64] = { 0 };

	try
	{
		sprintf_s(Text, "%s", gFont.AllFonts.at(gFont.CurrentFontNumber).c_str());
	}
	catch (...)
	{
		sprintf_s(Text, "%s", gFont.MyFontFaceName);
	}

	EnableAlphaTest(true);

	RenderText((int)PosX, CenterTextPosY(Text, ((int)PosY + (this->BoxHeight / 2))), Text, REAL_WIDTH(this->BoxWidth), RT3_SORT_CENTER, NULL);

	if (gFont.CurrentFontNumber > 0)
	{
		if (IsWorkZone((int)PosX + 2, (int)PosY + 2, this->BoxHeight, this->BoxHeight))
		{
			if (MouseLButtonPush)
			{
				RenderBitmap(0x100, PosX + 2.0f, PosY + 2.0f, (float)this->BoxHeight - 4.0f, (float)this->BoxHeight - 4.0f, 0.0f, 0.0f, 32.0f / 32.0f, 32.0f / 32.0f, true, true);
			}
			else
			{
				RenderBitmap(0xFF, PosX + 2.0f, PosY + 2.0f, (float)this->BoxHeight - 4.0f, (float)this->BoxHeight - 4.0f, 0.0f, 0.0f, 32.0f / 32.0f, 32.0f / 32.0f, true, true);
			}
		}
		else
		{
			RenderBitmap(0xFE, PosX + 2.0f, PosY + 2.0f, (float)this->BoxHeight - 4.0f, (float)this->BoxHeight - 4.0f, 0.0f, 0.0f, 32.0f / 32.0f, 32.0f / 32.0f, true, true);
		}
	}

	if (gFont.CurrentFontNumber < (int)gFont.AllFonts.size() - 1)
	{
		if (IsWorkZone((int)PosX + (this->BoxWidth - this->BoxHeight) - 2, (int)PosY + 2, this->BoxHeight, this->BoxHeight))
		{
			if (MouseLButtonPush)
			{
				MyRenderBitmapRotate(0x100, (PosX + 2.0f) + (float)(this->BoxWidth - this->BoxHeight), PosY + 2.0f, (float)this->BoxHeight - 4.0f, (float)this->BoxHeight - 4.0f, 180.0f, 0.0f, 0.0f, 32.0f / 32.0f, 32.0f / 32.0f);
			}
			else
			{
				MyRenderBitmapRotate(0xFF, (PosX + 2.0f) + (float)(this->BoxWidth - this->BoxHeight), PosY + 2.0f, (float)this->BoxHeight - 4.0f, (float)this->BoxHeight - 4.0f, 180.0f, 0.0f, 0.0f, 32.0f / 32.0f, 32.0f / 32.0f);
			}
		}
		else
		{
			MyRenderBitmapRotate(0xFE, (PosX + 2.0f) + (float)(this->BoxWidth - this->BoxHeight), PosY + 2.0f, (float)this->BoxHeight - 4.0f, (float)this->BoxHeight - 4.0f, 180.0f, 0.0f, 0.0f, 32.0f / 32.0f, 32.0f / 32.0f);
		}
	}
}

bool COptionsMenu::CheckFontStyle(int PosX, int PosY)
{
	if (gFont.CurrentFontNumber > 0)
	{
		if (IsWorkZone(PosX + 2, PosY + 2, this->BoxHeight, this->BoxHeight))
		{
			if (MouseLButton && MouseLButtonPush)
			{
				MouseLButtonPush = false;

				MouseUpdateTime = 0;

				MouseUpdateTimeMax = 6;

				PlayBuffer(25, 0, 0);

				gFont.CurrentFontNumber--;

				try
				{
					sprintf_s(gFont.MyFontFaceName, "%s", gFont.AllFonts.at(gFont.CurrentFontNumber).c_str());

					gFont.ReloadFont();
				}
				catch (...)
				{
					gFont.CurrentFontNumber++;
				}
			}

			return true;
		}
	}

	if (gFont.CurrentFontNumber < (int)gFont.AllFonts.size() - 1)
	{
		if (IsWorkZone(PosX + (this->BoxWidth - this->BoxHeight) - 2, PosY + 2, this->BoxHeight, this->BoxHeight))
		{
			if (MouseLButton && MouseLButtonPush)
			{
				MouseLButtonPush = false;

				MouseUpdateTime = 0;

				MouseUpdateTimeMax = 6;

				PlayBuffer(25, 0, 0);

				gFont.CurrentFontNumber++;

				try
				{
					sprintf_s(gFont.MyFontFaceName, "%s", gFont.AllFonts.at(gFont.CurrentFontNumber).c_str());

					gFont.ReloadFont();
				}
				catch (...)
				{
					gFont.CurrentFontNumber--;
				}
			}

			return true;
		}
	}

	return false;
}

void COptionsMenu::RenderFontBold(float PosX, float& PosY)
{
	this->RenderBox(PosX, PosY, (float)this->BoxWidth, (float)this->BoxHeight);

	char Text[64] = { 0 };

	sprintf_s(Text, "Bold: %s", (gFont.Bold) ? "On" : "Off");

	EnableAlphaTest(true);

	RenderText((int)PosX, CenterTextPosY(Text, ((int)PosY + (this->BoxHeight / 2))), Text, REAL_WIDTH(this->BoxWidth), RT3_SORT_CENTER, NULL);
}

bool COptionsMenu::CheckFontBold(int PosX, int& PosY)
{
	if (IsWorkZone(PosX, PosY, this->BoxWidth, this->BoxHeight))
	{
		if (MouseLButton && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;

			MouseUpdateTimeMax = 6;

			PlayBuffer(25, 0, 0);

			gFont.Bold ^= 1;

			gFont.ReloadFont();
		}

		return true;
	}

	return false;
}

void COptionsMenu::RenderFontItalic(float PosX, float& PosY)
{
	this->RenderBox(PosX, PosY, (float)this->BoxWidth, (float)this->BoxHeight);

	char Text[64] = { 0 };

	sprintf_s(Text, "Italic: %s", (gFont.Italic) ? "On" : "Off");

	EnableAlphaTest(true);

	RenderText((int)PosX, CenterTextPosY(Text, ((int)PosY + (this->BoxHeight / 2))), Text, REAL_WIDTH(this->BoxWidth), RT3_SORT_CENTER, NULL);
}

bool COptionsMenu::CheckFontItalic(int PosX, int& PosY)
{
	if (IsWorkZone(PosX, PosY, this->BoxWidth, this->BoxHeight))
	{
		if (MouseLButton && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;

			MouseUpdateTimeMax = 6;

			PlayBuffer(25, 0, 0);

			gFont.Italic ^= 1;

			gFont.ReloadFont();
		}

		return true;
	}

	return false;
}

void COptionsMenu::RenderFontButtons(float PosX, float& PosY)
{
	DisableAlphaBlend();

	glColor3f(1.0f, 1.0f, 1.0f);

	RenderBitmap(240, PosX, PosY, (float)this->BoxWidth, (float)this->BoxHeight, 0.0f, 0.0f, 213.0f / 256.0f, 64.0f / 64.0f, true, true);

	char Text[3] = { 0 };

	sprintf_s(Text, "%d", FontHeight);

	EnableAlphaTest(true);

	RenderText((int)PosX, CenterTextPosY(Text, ((int)PosY + (this->BoxHeight / 2))), Text, REAL_WIDTH(this->BoxWidth), RT3_SORT_CENTER, NULL);

	if (FontHeight > 1)
	{
		if (IsWorkZone((int)PosX + 2, (int)PosY + 2, this->BoxHeight, this->BoxHeight))
		{
			if (MouseLButtonPush)
			{
				RenderBitmap(0x100, PosX + 2.0f, PosY + 2.0f, (float)this->BoxHeight - 4.0f, (float)this->BoxHeight - 4.0f, 0.0f, 0.0f, 32.0f / 32.0f, 32.0f / 32.0f, true, true);
			}
			else
			{
				RenderBitmap(0xFF, PosX + 2.0f, PosY + 2.0f, (float)this->BoxHeight - 4.0f, (float)this->BoxHeight - 4.0f, 0.0f, 0.0f, 32.0f / 32.0f, 32.0f / 32.0f, true, true);
			}
		}
		else
		{
			RenderBitmap(0xFE, PosX + 2.0f, PosY + 2.0f, (float)this->BoxHeight - 4.0f, (float)this->BoxHeight - 4.0f, 0.0f, 0.0f, 32.0f / 32.0f, 32.0f / 32.0f, true, true);
		}
	}

	if (IsWorkZone((int)PosX + (this->BoxWidth - this->BoxHeight) - 2, (int)PosY + 2, this->BoxHeight, this->BoxHeight))
	{
		if (MouseLButtonPush)
		{
			MyRenderBitmapRotate(0x100, (PosX + 2.0f) + (float)(this->BoxWidth - this->BoxHeight), PosY + 2.0f, (float)this->BoxHeight - 4.0f, (float)this->BoxHeight - 4.0f, 180.0f, 0.0f, 0.0f, 32.0f / 32.0f, 32.0f / 32.0f);
		}
		else
		{
			MyRenderBitmapRotate(0xFF, (PosX + 2.0f) + (float)(this->BoxWidth - this->BoxHeight), PosY + 2.0f, (float)this->BoxHeight - 4.0f, (float)this->BoxHeight - 4.0f, 180.0f, 0.0f, 0.0f, 32.0f / 32.0f, 32.0f / 32.0f);
		}
	}
	else
	{
		MyRenderBitmapRotate(0xFE, (PosX + 2.0f) + (float)(this->BoxWidth - this->BoxHeight), PosY + 2.0f, (float)this->BoxHeight - 4.0f, (float)this->BoxHeight - 4.0f, 180.0f, 0.0f, 0.0f, 32.0f / 32.0f, 32.0f / 32.0f);
	}
}

bool COptionsMenu::CheckFontButtons(int PosX, int& PosY)
{
	if (FontHeight > 1)
	{
		if (IsWorkZone(PosX + 2, PosY + 2, this->BoxHeight, this->BoxHeight))
		{
			if (MouseLButton && MouseLButtonPush)
			{
				MouseLButtonPush = false;

				MouseUpdateTime = 0;

				MouseUpdateTimeMax = 6;

				PlayBuffer(25, 0, 0);

				FontHeight -= 1;

				gFont.ReloadFont();
			}

			return true;
		}
	}

	if (IsWorkZone(PosX + (this->BoxWidth - this->BoxHeight) - 2, PosY + 2, this->BoxHeight, this->BoxHeight))
	{
		if (MouseLButton && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;

			MouseUpdateTimeMax = 6;

			PlayBuffer(25, 0, 0);

			FontHeight += 1;

			gFont.ReloadFont();
		}

		return true;
	}

	return false;
}

void COptionsMenu::RenderResetFonts(float PosX, float& PosY)
{
	this->RenderBox(PosX, PosY, (float)this->BoxWidth, (float)this->BoxHeight);

	char Text[64] = { 0 };

	sprintf_s(Text, "%s", GetTextLine(915));

	EnableAlphaTest(true);

	RenderText((int)PosX, CenterTextPosY(Text, ((int)PosY + (this->BoxHeight / 2))), Text, REAL_WIDTH(this->BoxWidth), RT3_SORT_CENTER, NULL);
}

bool COptionsMenu::CheckResetFonts(int PosX, int& PosY)
{
	if (IsWorkZone(PosX, PosY, this->BoxWidth, this->BoxHeight))
	{
		if (MouseLButton && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;

			MouseUpdateTimeMax = 6;

			PlayBuffer(25, 0, 0);

			gFont.Bold = 0;

			gFont.Italic = 0;

			FontHeight = 13;

			gFont.CurrentFontNumber = 0;

			sprintf_s(gFont.MyFontFaceName, "%s", gFont.AllFonts[gFont.CurrentFontNumber].c_str());

			gFont.ReloadFont();
		}

		return true;
	}

	return false;
}

void COptionsMenu::RenderClose(float PosX, float PosY)
{
	this->RenderBox(PosX, PosY, (float)this->BoxWidth, (float)this->BoxHeight);

	char Text[64] = { 0 };

	sprintf_s(Text, "%s", GetTextLine(388));

	EnableAlphaTest(true);

	RenderText((int)PosX, CenterTextPosY(Text, ((int)PosY + (BoxHeight / 2))), Text, REAL_WIDTH(this->BoxWidth), RT3_SORT_CENTER, NULL);
}

bool COptionsMenu::CheckClose(int PosX, int PosY)
{
	if (IsWorkZone(PosX, PosY, this->BoxWidth, this->BoxHeight))
	{
		if (MouseLButton && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;

			MouseUpdateTimeMax = 6;

			PlayBuffer(25, 0, 0);

			this->CurrentOption = OPTION_NONE;

			ErrorMessage = *(BYTE*)0x083A7C28;

			*(BYTE*)0x083A7C28 = 0;
		}

		return true;
	}

	return false;
}

void COptionsMenu::RenderBack(float PosX, float PosY)
{
	this->RenderBox(PosX, PosY, (float)this->BoxWidth, (float)this->BoxHeight);

	char Text[64] = { 0 };

	sprintf_s(Text, "%s", GetTextLine(916));

	EnableAlphaTest(true);

	RenderText((int)PosX, CenterTextPosY(Text, ((int)PosY + (BoxHeight / 2))), Text, REAL_WIDTH(this->BoxWidth), RT3_SORT_CENTER, NULL);
}

bool COptionsMenu::CheckBack(int PosX, int PosY)
{
	if (IsWorkZone(PosX, PosY, this->BoxWidth, this->BoxHeight))
	{
		if (MouseLButton && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;

			MouseUpdateTimeMax = 6;

			PlayBuffer(25, 0, 0);

			this->CurrentOption = OPTION_NONE;
		}

		return true;
	}

	return false;
}

void COptionsMenu::RenderBox(float PosX, float PosY, float Width, float Height)
{
	DisableAlphaBlend();

	glColor3f(1.0f, 1.0f, 1.0f);

	RenderBitmap(240, PosX, PosY, Width, Height, 0.0f, 0.0f, 213.0f / 256.0f, 64.0f / 64.0f, true, true);

	if (IsWorkZone((int)PosX, (int)PosY, (int)Width, (int)Height))
	{
		glColor3f(0.8f, 0.6f, 0.4f);

		EnableAlphaBlend();

		RenderBitmap(240, PosX, PosY, Width, Height, 0.0f, 0.0f, 213.0f / 256.0f, 64.0f / 64.0f, true, true);

		glColor3f(1.0f, 1.0f, 1.0f);

		DisableAlphaBlend();
	}
}