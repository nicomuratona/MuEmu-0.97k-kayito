#include "stdafx.h"
#include "PrintPlayer.h"
#include "LoadModels.h"
#include "Protect.h"
#include "ServerList.h"

CPrintPlayer gPrintPlayer;

CPrintPlayer::CPrintPlayer()
{
	this->ViewIndex = 0;

	this->ViewReset = 0;

	this->ViewGrandReset = 0;

	this->ViewValue = 0;

	this->ViewPoint = 0;

	this->ViewCurHP = 0;

	this->ViewMaxHP = 0;

	this->ViewCurMP = 0;

	this->ViewMaxMP = 0;

	this->ViewCurBP = 0;

	this->ViewMaxBP = 0;

	this->ViewDamageHP = 0;

	this->ViewExperience = 0;

	this->ViewNextExperience = 0;

	this->ViewStrength = 0;

	this->ViewDexterity = 0;

	this->ViewVitality = 0;

	this->ViewEnergy = 0;

	this->ViewPhysiSpeed = 0;

	this->ViewMagicSpeed = 0;

	this->ViewPhysiDamageMin = 0;

	this->ViewPhysiDamageMax = 0;

	this->ViewMagicDamageMin = 0;

	this->ViewMagicDamageMax = 0;

	this->ViewMagicDamageRate = 0;

	this->ViewAttackSuccessRate = 0;

	this->ViewDamageMultiplier = 0;

	this->ViewDefense = 0;

	this->ViewDefenseSuccessRate = 0;

	this->MaxCharacterLevel = MAX_CHARACTER_LEVEL;

	this->MaxCharacterDeleteLevel = 220;
}

CPrintPlayer::~CPrintPlayer()
{

}

void CPrintPlayer::Init()
{
	SetCompleteHook(0xE8, 0x004BD98F, &this->RenderCircleHPMP); // HP bottle

	SetCompleteHook(0xE8, 0x004BDA56, &this->RenderCircleHPMP); // MP bottle

	SetCompleteHook(0xE8, 0x004BCFF2, &this->RenderCircleAG); // BP bottle

	SetCompleteHook(0xE8, 0x004BD9E7, &this->RenderTextHP); // text.bmd 358: "Life: %d/%d"

	SetCompleteHook(0xE8, 0x004BE2C5, &this->RenderNumberHP); // Render the number of HP

	SetCompleteHook(0xE8, 0x004BDAAE, &this->RenderTextMP); // text.bmd 359: "Mana: %d/%d"

	SetCompleteHook(0xE8, 0x004BE2EC, &this->RenderNumberMP); // Render the number of MP

	SetCompleteHook(0xE8, 0x004BD055, &this->RenderTextBP); // text.bmd 214: "A G: %d / %d"

	SetCompleteHook(0xE8, 0x004BD00C, &this->RenderNumberBP); // Render the number of BP

	SetCompleteHook(0xE8, 0x004BC0F8, &this->RenderExperience); // Render the experience bar and number

	SetCompleteHook(0xE9, 0x0047DD80, &this->CalculateAttackSpeed); // Fix the attack speed

	SetCompleteHook(0xE9, 0x00443E70, &this->SetAttackSpeed); // Set Attack Speed

	SetCompleteHook(0xE8, 0x0042AD92, &this->RenderDamageHP); // Damage number on attack

	SetCompleteHook(0xE8, 0x0042B38E, &this->RenderDamageHP); // Damage number on attack

	SetCompleteHook(0xE8, 0x0042B402, &this->RenderDamageHP); // Damage number on attack

	SetCompleteHook(0xE8, 0x0042DA3D, &this->RenderDamageHP); // Damage number on attack

	SetCompleteHook(0xE8, 0x0042E2CA, &this->RenderDamageHP); // Damage number on attack

	SetCompleteHook(0xE8, 0x0042E2FE, &this->RenderDamageHP); // Damage number on attack

	SetCompleteHook(0xE8, 0x0042ED2C, &this->RenderDamageHP); // Damage number on attack

	SetCompleteHook(0xE8, 0x0042ED60, &this->RenderDamageHP); // Damage number on attack

	SetCompleteHook(0xE9, 0x004ECC60, &this->RenderCharacterInfoWindow); // Character Info Window
}

void CPrintPlayer::RenderCircleHPMP(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale)
{
	float HP = (float)(gPrintPlayer.ViewMaxHP - gPrintPlayer.ViewCurHP) / (float)gPrintPlayer.ViewMaxHP;

	float MP = (float)(gPrintPlayer.ViewMaxMP - gPrintPlayer.ViewCurMP) / (float)gPrintPlayer.ViewMaxMP;

	y = ((Texture == 0xEB) ? MP : HP) * 48.0f + 432.0f;

	Height = 48.0f - ((Texture == 0xEB) ? MP : HP) * 48.0f;

	v = ((Texture == 0xEB) ? MP : HP) * 48.0f / 64.0f;

	vHeight = (1.0f - ((Texture == 0xEB) ? MP : HP)) * 48.0f / 64.0f;

	return RenderBitmap(Texture, x, y, Width, Height, u, v, uWidth, vHeight, Scale, StartScale);
}

void CPrintPlayer::RenderCircleAG(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale)
{
	float BP = (float)(gPrintPlayer.ViewMaxBP - gPrintPlayer.ViewCurBP) / (float)gPrintPlayer.ViewMaxBP;

	y = BP * 36.0f + 438.0f;

	Height = 36.0f - BP * 36.0f;

	v = BP * 36.0f / 64.0f;

	vHeight = (1.0f - BP) * 36.0f / 64.0f;

	return RenderBitmap(Texture, x, y, Width, Height, u, v, uWidth, vHeight, Scale, StartScale);
}

void CPrintPlayer::RenderTextHP(char* Dest, char* Format)
{
	wsprintf(Dest, Format, gPrintPlayer.ViewCurHP, gPrintPlayer.ViewMaxHP);
}

float CPrintPlayer::RenderNumberHP(float x, float y, int Num, float Width, float Height)
{
	return RenderNumber2D(x, y, gPrintPlayer.ViewCurHP, 9.0f, 10.0f);
}

void CPrintPlayer::RenderTextMP(char* Dest, char* Format)
{
	wsprintf(Dest, Format, gPrintPlayer.ViewCurMP, gPrintPlayer.ViewMaxMP);
}

float CPrintPlayer::RenderNumberMP(float x, float y, int Num, float Width, float Height)
{
	return RenderNumber2D(x - 20, y, gPrintPlayer.ViewCurMP, 9.0f, 10.0f);
}

void CPrintPlayer::RenderTextBP(char* Dest, char* Format)
{
	wsprintf(Dest, Format, gPrintPlayer.ViewCurBP, gPrintPlayer.ViewMaxBP);
}

float CPrintPlayer::RenderNumberBP(float x, float y, int Num, float Width, float Height)
{
	return RenderNumber2D(x, y, gPrintPlayer.ViewCurBP, 9.0f, 10.0f);
}

void CPrintPlayer::RenderExperience()
{
	STRUCT_DECRYPT;

	WORD wLevel = *(WORD*)(CharacterAttribute + 0x0E); // current level

	STRUCT_ENCRYPT;

	WORD wPriorLevel = wLevel - 1;

	DWORD dwPriorExperience = 0;

	if (wPriorLevel > 0)
	{
		double maxExperience = DWORD_MAX * 0.95;

		double scaleFactor = maxExperience / pow(gPrintPlayer.MaxCharacterLevel, 3);

		dwPriorExperience = (DWORD)(scaleFactor * pow(wPriorLevel, 3));
	}

	DWORD RequiredExp = gPrintPlayer.ViewNextExperience - dwPriorExperience;

	DWORD ActualExp = gPrintPlayer.ViewExperience - dwPriorExperience;

	float NumberBarRate = (float)ActualExp / (float)RequiredExp * 10.0f;

	int Number = (int)NumberBarRate;

	float Width = (NumberBarRate - (float)Number) * 198.0f;

	int Height = 4;

	int X = 221;

	int Y = 439;

	glColor3f(0.92f, 0.8f, 0.34f);

	RenderColor((float)X, (float)Y, Width, (float)Height);

	EnableAlphaTest(true);

	glColor3f(0.91f, 0.81f, 0.6f);

	// experience bar number
	RenderNumber2D(425.0f, 434.0f, Number, 9.0f, 10.0f);

	DisableAlphaBlend();

	glColor3f(1.0f, 1.0f, 1.0f);

	Width = 198.0f;

	if (IsWorkZone(X, Y, (int)Width, Height))
	{
		char strTipText[256];

		sprintf_s(strTipText, sizeof(strTipText), GlobalText[201], gPrintPlayer.ViewExperience, gPrintPlayer.ViewNextExperience); // text.bmd 201: "Exp: %u/%u"

		RenderTipText(X + 2, Y - 15, strTipText);
	}
}

void CPrintPlayer::CalculateAttackSpeed(DWORD This)
{
	*(WORD*)(CharacterAttribute + 0x38) = GET_MAX_WORD_VALUE(gPrintPlayer.ViewPhysiSpeed);

	*(WORD*)(CharacterAttribute + 0x44) = GET_MAX_WORD_VALUE(gPrintPlayer.ViewMagicSpeed);
}

void CPrintPlayer::SetAttackSpeed()
{
	// formula for calculating attack speed
	float AttackSpeed1 = gPrintPlayer.ViewPhysiSpeed * 0.004f; // Knight, Normal attack

	float MagicSpeed1 = gPrintPlayer.ViewMagicSpeed * 0.004f; // Fairy

	float MagicSpeed2 = gPrintPlayer.ViewMagicSpeed * 0.002f; // Xuan

	DWORD b = *(DWORD*)(gLoadModels.GetModels() + 73368); // &Models[MODEL_PLAYER]

	int i;

	// fist attack
	*(float*)(b + 548) = 0.6f + AttackSpeed1; // PLAYER_ATTACK_FIST

	// normal attack
	for (i = 0; i <= 20; i++) // PLAYER_ATTACK_SWORD_RIGHT1 ~ PLAYER_ATTACK_RIDE_CROSSBOW
	{
		*(float*)(b + 564 + 16 * i) = 0.25f + AttackSpeed1;
	}

	// Skill attack
	*(float*)(b + 900) = 0.30f + AttackSpeed1; // PLAYER_ATTACK_SKILL_SWORD1

	*(float*)(b + 916) = 0.30f + AttackSpeed1; // PLAYER_ATTACK_SKILL_SWORD2

	*(float*)(b + 932) = 0.27f + AttackSpeed1; // PLAYER_ATTACK_SKILL_SWORD3

	*(float*)(b + 948) = 0.30f + AttackSpeed1; // PLAYER_ATTACK_SKILL_SWORD4

	*(float*)(b + 964) = 0.24f + AttackSpeed1; // PLAYER_ATTACK_SKILL_SWORD5

	*(float*)(b + 980) = 0.24f + AttackSpeed1; // PLAYER_ATTACK_SKILL_WHEEL

	*(float*)(b + 1076) = 0.25f + AttackSpeed1; // PLAYER_ATTACK_ONETOONE

	*(float*)(b + 1060) = 0.30f + AttackSpeed1; // PLAYER_ATTACK_SKILL_SPEAR

	// riding state
	*(float*)(b + 1028) = 0.3f + AttackSpeed1; // PLAYER_SKILL_RIDER

	*(float*)(b + 1044) = 0.3f + AttackSpeed1; // PLAYER_SKILL_RIDER_FLY

	*(float*)(b + 1300) = 0.25f + AttackSpeed1; // PLAYER_ATTACK_TWO_HAND_SWORD_TWO

	// bow attack
	for (i = 0; i <= 3; i++) // PLAYER_ATTACK_BOW ~ PLAYER_ATTACK_FLY_CROSSBOW
	{
		*(float*)(b + 740 + 16 * i) = 0.30f + AttackSpeed1;
	}

	for (i = 0; i <= 1; i++) // PLAYER_ATTACK_RIDE_BOW ~ PLAYER_ATTACK_RIDE_CROSSBOW
	{
		*(float*)(b + 868 + 16 * i) = 0.30f + AttackSpeed1;
	}

	// fairy magic
	*(float*)(b + 1380) = 0.25f + MagicSpeed1; // PLAYER_SKILL_ELF1

	// normal magic
	for (i = 0; i <= 3; i++) // PLAYER_SKILL_HAND1 ~ PLAYER_SKILL_WEAPON2
	{
		*(float*)(b + 1316 + 16 * i) = 0.29f + MagicSpeed2;
	}

	// other magic
	*(float*)(b + 1396) = 0.30f + MagicSpeed2; // PLAYER_SKILL_TELEPORT

	*(float*)(b + 1412) = 0.40f + MagicSpeed2; // PLAYER_SKILL_FLASH

	*(float*)(b + 1428) = 0.60f + MagicSpeed2; // PLAYER_SKILL_INFERNO

	*(float*)(b + 1444) = 0.50f + MagicSpeed2; // PLAYER_SKILL_HELL

	*(float*)(b + 1460) = 0.30f + MagicSpeed2; // PLAYER_RIDE_SKILL

	*(float*)(b + 996) = 0.38f; // PLAYER_ATTACK_SKILL_FURY_STRIKE

	*(float*)(b + 1012) = 0.34f; // PLAYER_SKILL_VITALITY
}

void CPrintPlayer::RenderDamageHP(float Position[3], int Value, float Color[3], float scale, bool bMove)
{
	if (Value > 0)
	{
		Value = gPrintPlayer.ViewDamageHP;
	}

	CreatePoint(Position, Value, Color, scale, bMove);
}

void CPrintPlayer::RenderCharacterInfoWindow(int StartPosX, int StartPosY)
{
	/***********************/
	// Set Window Position
	/***********************/
	*(int*)0x07EA982C = StartPosX;
	*(int*)0x07EA9830 = StartPosY;

	/***********************/
	// Render Frame
	/***********************/
	gPrintPlayer.RenderCharInfo_Frame((float)StartPosX, (float)StartPosY);

	/***********************/
	// Decrypt struct values
	/***********************/
	STRUCT_DECRYPT;

	/***********************/
	// Render Title
	/***********************/
	gPrintPlayer.RenderCharInfo_Title(StartPosX, StartPosY);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	/***********************/
	// Render Level, Points, Experience
	/***********************/
	gPrintPlayer.RenderCharInfo_Level(StartPosX, StartPosY);

	StartPosY += 116;
	/***********************/
	// Render Strength, Damage
	/***********************/
	gPrintPlayer.RenderCharInfo_Strength(StartPosX, StartPosY);

	StartPosY += 60;
	/***********************/
	// Render Agility, Defense, Attack Speed
	/***********************/
	gPrintPlayer.RenderCharInfo_Agility(StartPosX, StartPosY);

	StartPosY += 60;
	/***********************/
	// Render Vitality, Health
	/***********************/
	gPrintPlayer.RenderCharInfo_Vitality(StartPosX, StartPosY);

	StartPosY += 60;
	/***********************/
	// Render Energy, Mana, Skill Damage
	/***********************/
	gPrintPlayer.RenderCharInfo_Energy(StartPosX, StartPosY);

	/***********************/
	// Encrypt struct values
	/***********************/
	STRUCT_ENCRYPT;
}

void CPrintPlayer::RenderCharInfo_Frame(float fPosX, float fPosY)
{
	glColor3f(1.0f, 1.0f, 1.0f);

	DisableAlphaBlend();

	RenderBitmap(260, fPosX, fPosY, 190.0f, 256.0f, (0.0f / 256.0f), (0.0f / 256.0f), (190.0f / 256.0f), (256.0f / 256.0f), true, true);

	fPosY += 256.0f;

	RenderBitmap(261, fPosX, fPosY, 190.0f, 177.0f, (0.0f / 256.0f), (0.0f / 256.0f), (190.0f / 256.0f), (177.0f / 256.0f), true, true);
}

void CPrintPlayer::RenderCharInfo_Title(int StartPosX, int StartPosY)
{
	EnableAlphaTest(true);

	SelectObject(m_hFontDC, g_hFontBold);

	SetBackgroundTextColor = Color4b(20, 20, 20, 255);

	StartPosY += 12;

	// Render Character Name
	char CharacterName[11];
	sprintf_s(CharacterName, sizeof(CharacterName), "%s", (char*)(CharacterAttribute + 0x00));

	SetPlayerColor(*(BYTE*)(Hero + 0x2EA));
	RenderText(StartPosX + 35, CenterTextPosY(CharacterName, StartPosY), CharacterName, REAL_WIDTH(120), RT3_SORT_CENTER, NULL);

	float fAlpha = sinf(WorldTime * 0.001f) + 1.0f;

	StartPosY += 10;

	// Render Character Class
	BYTE CharacterClass = *(BYTE*)(CharacterAttribute + 0x0B);
	glColor4f(1.0f, 1.0f, 1.0f, (2.0f - fAlpha));
	char CharClass[128];
	sprintf_s(CharClass, sizeof(CharClass), "(%s)", GlobalText[20 + ((CharacterClass >> 3) * 4) + (CharacterClass & 7)]);
	RenderText(StartPosX + 22, CenterTextPosY(CharClass, StartPosY), CharClass, REAL_WIDTH(150), RT3_SORT_CENTER, NULL);

	// Render Server Name
	glColor4f(1.0f, 1.0f, 1.0f, fAlpha);
	char ServerName[128];
	gServerList.ServerNameChange(ServerName, "%s", 0, 0);
	RenderText(StartPosX + 22, CenterTextPosY(ServerName, StartPosY), ServerName, REAL_WIDTH(150), RT3_SORT_CENTER, NULL);
}

void CPrintPlayer::RenderCharInfo_Level(int StartPosX, int StartPosY)
{
	// Render Level Box
	float fPosX = (float)StartPosX + 13.0f;
	float fPosY = (float)StartPosY + 56.0f;

	RenderBitmap(245, fPosX, fPosY, 75.0f, 21.0f, (0.0f / 128.0f), (0.0f / 32.0f), (75.0f / 128.0f), (21.0f / 32.0f), true, true);
	
	// Render Level Text
	WORD CharacterLevel = *(WORD*)(CharacterAttribute + 0x0E);
	SelectObject(m_hFontDC, g_hFont);
	SetBackgroundTextColor = Color4b(255, 255, 255, 0);
	SetTextColor = Color4b(230, 230, 230, 255);

	char Buffer[256];
	sprintf_s(Buffer, sizeof(Buffer), GlobalText[200], CharacterLevel, this->MaxCharacterLevel);
	RenderText((int)fPosX, CenterTextPosY(Buffer, (int)fPosY + 10), Buffer, REAL_WIDTH(75), RT3_SORT_CENTER, NULL);

	// Render Resets
	if (!gProtect.m_MainInfo.DisableResets)
	{
		// Render Resets Box
		fPosX = (float)StartPosX + 190.0f - 13.0f - 75.0f;

		RenderBitmap(245, fPosX, fPosY, 75.0f, 21.0f, (75.0f / 128.0f), (0.0f / 32.0f), (-75.0f / 128.0f), (21.0f / 32.0f), true, true);

		// Render Resets Text
		sprintf_s(Buffer, sizeof(Buffer), "Resets: %d", this->ViewReset);
		RenderText((int)fPosX, CenterTextPosY(Buffer, (int)fPosY + 10), Buffer, REAL_WIDTH(75), RT3_SORT_CENTER, NULL);
	}

	// Render Levelup Points Text
	if (this->ViewPoint > 0)
	{
		SetBackgroundTextColor = Color4b(30, 110, 200, 255);
		SetTextColor = Color4b(0, 0, 0, 255);

		fPosX = (float)StartPosX + (190.0f * 0.5f);
		fPosY = (float)StartPosY + 49.0f;

		sprintf_s(Buffer, sizeof(Buffer), GlobalText[217], this->ViewPoint);
		RenderText((int)fPosX, CenterTextPosY(Buffer, (int)fPosY), Buffer, REAL_WIDTH(80), RT3_SORT_CENTER, NULL);
	}

	// Render Experience Text
	SetBackgroundTextColor = Color4b(0, 0, 0, 128);
	SetTextColor = Color4b(255, 255, 255, 255);

	fPosX = (float)StartPosX + 20.0f;
	fPosY = (float)StartPosY + 85.0f;

	sprintf_s(Buffer, sizeof(Buffer), GlobalText[201], this->ViewExperience, this->ViewNextExperience);
	RenderText((int)fPosX, CenterTextPosY(Buffer, (int)fPosY), Buffer, 0, RT3_SORT_LEFT, NULL);

	// Render Spare Points Text
	SetTextColor = Color4b(100, 150, 255, 255);

	fPosY += 15.0f;

	if (CharacterLevel <= 5)
	{
		sprintf_s(Buffer, sizeof(Buffer), GlobalText[199], 0, 0);
	}
	else
	{
		sprintf_s(Buffer, sizeof(Buffer), GlobalText[199], *(short*)(CharacterAttribute + 0x2E), *(short*)(CharacterAttribute + 0x30));
	}

	RenderText((int)fPosX, CenterTextPosY(Buffer, (int)fPosY), Buffer, REAL_WIDTH(130), RT3_SORT_LEFT, NULL);
}

void CPrintPlayer::RenderCharInfo_Strength(int StartPosX, int StartPosY)
{
	// Render Strength Box
	float fPosX = (float)StartPosX + 13.0f;
	float fPosY = (float)StartPosY;

	RenderBitmap(245, fPosX, fPosY, 75.0f, 21.0f, (0.0f / 128.0f), (0.0f / 32.0f), (75.0f / 128.0f), (21.0f / 32.0f), true, true);

	// Render Plus Button
	if (this->ViewPoint > 0)
	{
		float ButtonPosX = (float)StartPosX + 125.0f;

		if (IsWorkZone((int)ButtonPosX, (int)fPosY, 21, 21) && MouseLButton)
		{
			RenderBitmap(0x121, ButtonPosX, fPosY, 21.0f, 21.0f, (0.0f / 32.0f), (0.0f / 32.0f), (24.0f / 32.0f), (24.0f / 32.0f), true, true);
		}
		else
		{
			RenderBitmap(0x120, ButtonPosX, fPosY, 21.0f, 21.0f, (0.0f / 32.0f), (0.0f / 32.0f), (24.0f / 32.0f), (24.0f / 32.0f), true, true);
		}
	}

	// Render Strength Text
	SelectObject(m_hFontDC, g_hFont);
	SetBackgroundTextColor = Color4b(255, 255, 255, 0);
	SetTextColor = Color4b(223, 191, 103, 255);

	char Buffer[256];
	sprintf_s(Buffer, sizeof(Buffer), GlobalText[202], this->ViewStrength);
	RenderText((int)fPosX, CenterTextPosY(Buffer, (int)fPosY + 10), Buffer, REAL_WIDTH(75), RT3_SORT_CENTER, NULL);

	// Render Damage Text
	SetBackgroundTextColor = Color4b(0, 0, 0, 128);

	ITEM* PlayerRightHand = &*(ITEM*)(CharacterMachine + (536 + (68 * EQUIPMENT_WEAPON_RIGHT)));

	ITEM* PlayerLeftHand = &*(ITEM*)(CharacterMachine + (536 + (68 * EQUIPMENT_WEAPON_LEFT)));

	bool BonusEnabled = false;

	BYTE Class = *(BYTE*)(Hero + 0x1BC) & 7;

	if (Class == CLASS_DARK_KNIGHT || Class == CLASS_MAGIC_GLADIATOR)
	{
		if ((PlayerLeftHand->Type >= GET_ITEM(0, 0) && PlayerLeftHand->Type < GET_ITEM(4, 0)) // First Hand Weapon
		    && (PlayerRightHand->Type >= GET_ITEM(0, 0) && PlayerRightHand->Type < GET_ITEM(4, 0))) // Second Hand Weapon
		{
			BonusEnabled = true;
		}
	}
	else if (Class == CLASS_FAIRY_ELF)
	{
		if ((PlayerRightHand->Type >= GET_ITEM(4, 0) && PlayerRightHand->Type < GET_ITEM(5, 0)) && (PlayerLeftHand->Type >= GET_ITEM(4, 0) && PlayerLeftHand->Type < GET_ITEM(5, 0)))
		{
			// If ARROW's LEVEL is 1 or more, attack power increases.
			if ((PlayerLeftHand->Type == GET_ITEM(4, 7) && GET_ITEM_OPT_LEVEL(PlayerLeftHand->Level) >= 1) // Arrow +1 ~ +11
			    || (PlayerRightHand->Type == GET_ITEM(4, 15) && GET_ITEM_OPT_LEVEL(PlayerRightHand->Level) >= 1)) // Bolt +1 ~ +11
			{
				BonusEnabled = true;
			}
		}
	}

	if (BonusEnabled)
	{
		SetTextColor = Color4b(100, 150, 255, 255);
	}
	else
	{
		SetTextColor = Color4b(255, 255, 255, 255);
	}

	fPosX = (float)StartPosX + 20.0f;
	fPosY = (float)StartPosY + 31.0f;

	if (this->ViewAttackSuccessRate > 0)
	{
		sprintf_s(Buffer, sizeof(Buffer), GlobalText[203], this->ViewPhysiDamageMin, this->ViewPhysiDamageMax, this->ViewAttackSuccessRate);
	}
	else
	{
		sprintf_s(Buffer, sizeof(Buffer), GlobalText[204], this->ViewPhysiDamageMin, this->ViewPhysiDamageMax);
	}

	RenderText((int)fPosX, CenterTextPosY(Buffer, (int)fPosY), Buffer, REAL_WIDTH(130), RT3_SORT_LEFT, NULL);
}

void CPrintPlayer::RenderCharInfo_Agility(int StartPosX, int StartPosY)
{
	// Render Agility Box
	float fPosX = (float)StartPosX + 13.0f;
	float fPosY = (float)StartPosY;

	RenderBitmap(245, fPosX, fPosY, 75.0f, 21.0f, (0.0f / 128.0f), (0.0f / 32.0f), (75.0f / 128.0f), (21.0f / 32.0f), true, true);

	// Render Plus Button
	if (this->ViewPoint > 0)
	{
		float ButtonPosX = (float)StartPosX + 125.0f;

		if (IsWorkZone((int)ButtonPosX, (int)fPosY, 21, 21) && MouseLButton)
		{
			RenderBitmap(0x121, ButtonPosX, fPosY, 21.0f, 21.0f, (0.0f / 32.0f), (0.0f / 32.0f), (24.0f / 32.0f), (24.0f / 32.0f), true, true);
		}
		else
		{
			RenderBitmap(0x120, ButtonPosX, fPosY, 21.0f, 21.0f, (0.0f / 32.0f), (0.0f / 32.0f), (24.0f / 32.0f), (24.0f / 32.0f), true, true);
		}
	}

	// Render Agility Text
	SelectObject(m_hFontDC, g_hFont);
	SetBackgroundTextColor = Color4b(255, 255, 255, 0);
	SetTextColor = Color4b(223, 191, 103, 255);

	char Buffer[256];
	sprintf_s(Buffer, sizeof(Buffer), GlobalText[205], gPrintPlayer.ViewDexterity);
	RenderText((int)fPosX, CenterTextPosY(Buffer, (int)fPosY + 10), Buffer, REAL_WIDTH(75), RT3_SORT_CENTER, NULL);

	// Render Defense Text
	SetBackgroundTextColor = Color4b(0, 0, 0, 128);
	SetTextColor = Color4b(255, 255, 255, 255);

	bool BonusEnabled = true;

	BYTE Class = *(BYTE*)(Hero + 0x1BC) & 7;

	int LastItemIndex = (&*(ITEM*)(CharacterMachine + (536 + (68 * EQUIPMENT_ARMOR))))->Type % MAX_ITEM_TYPE;

	int ItemLevelCount = 0;

	for (int i = EQUIPMENT_HELM; i <= EQUIPMENT_BOOTS; i++)
	{
		if (i == EQUIPMENT_HELM && Class == CLASS_MAGIC_GLADIATOR)
		{
			ItemLevelCount++;

			continue;
		}

		ITEM* EquipItem = &*(ITEM*)(CharacterMachine + (536 + (68 * i)));

		if (EquipItem->Type == -1)
		{
			BonusEnabled = false;

			break;
		}

		if (EquipItem->Durability <= 0 || (EquipItem->Type % MAX_ITEM_TYPE) != LastItemIndex)
		{
			BonusEnabled = false;

			break;
		}

		LastItemIndex = EquipItem->Type % MAX_ITEM_TYPE;

		if (GET_ITEM_OPT_LEVEL(EquipItem->Level) > 9)
		{
			ItemLevelCount++;
		}
	}

	if (ItemLevelCount < 5)
	{
		BonusEnabled = false;
	}

	if (BonusEnabled)
	{
		SetTextColor = Color4b(100, 150, 255, 255);
	}
	else
	{
		SetTextColor = Color4b(255, 255, 255, 255);
	}

	fPosX = (float)StartPosX + 20.0f;
	fPosY = (float)StartPosY + 31.0f;

	if (this->ViewDefenseSuccessRate > 0)
	{
		sprintf_s(Buffer, sizeof(Buffer), GlobalText[208], this->ViewDefense, this->ViewDefenseSuccessRate);
	}
	else
	{
		sprintf_s(Buffer, sizeof(Buffer), GlobalText[209], this->ViewDefense);
	}

	RenderText((int)fPosX, CenterTextPosY(Buffer, (int)fPosY), Buffer, REAL_WIDTH(130), RT3_SORT_LEFT, NULL);

	// Render Attack Speed Text
	SetBackgroundTextColor = Color4b(0, 0, 0, 128);
	SetTextColor = Color4b(255, 255, 255, 255);

	fPosY += 15.0f;

	sprintf_s(Buffer, sizeof(Buffer), GlobalText[64], (Class == CLASS_DARK_WIZARD) ? this->ViewMagicSpeed : this->ViewPhysiSpeed);
	RenderText((int)fPosX, CenterTextPosY(Buffer, (int)fPosY), Buffer, REAL_WIDTH(130), RT3_SORT_LEFT, NULL);
}

void CPrintPlayer::RenderCharInfo_Vitality(int StartPosX, int StartPosY)
{
	// Render Vitality Box
	float fPosX = (float)StartPosX + 13.0f;
	float fPosY = (float)StartPosY;

	RenderBitmap(245, fPosX, fPosY, 75.0f, 21.0f, (0.0f / 128.0f), (0.0f / 32.0f), (75.0f / 128.0f), (21.0f / 32.0f), true, true);

	// Render Plus Button
	if (this->ViewPoint > 0)
	{
		float ButtonPosX = (float)StartPosX + 125.0f;

		if (IsWorkZone((int)ButtonPosX, (int)fPosY, 21, 21) && MouseLButton)
		{
			RenderBitmap(0x121, ButtonPosX, fPosY, 21.0f, 21.0f, (0.0f / 32.0f), (0.0f / 32.0f), (24.0f / 32.0f), (24.0f / 32.0f), true, true);
		}
		else
		{
			RenderBitmap(0x120, ButtonPosX, fPosY, 21.0f, 21.0f, (0.0f / 32.0f), (0.0f / 32.0f), (24.0f / 32.0f), (24.0f / 32.0f), true, true);
		}
	}

	// Render Vitality Text
	SelectObject(m_hFontDC, g_hFont);
	SetBackgroundTextColor = Color4b(255, 255, 255, 0);
	SetTextColor = Color4b(223, 191, 103, 255);

	char Buffer[256];
	sprintf_s(Buffer, sizeof(Buffer), GlobalText[210], gPrintPlayer.ViewVitality);
	RenderText((int)fPosX, CenterTextPosY(Buffer, (int)fPosY + 10), Buffer, REAL_WIDTH(75), RT3_SORT_CENTER, NULL);

	// Render Health Text
	SetBackgroundTextColor = Color4b(0, 0, 0, 128);
	SetTextColor = Color4b(255, 255, 255, 255);

	fPosX = (float)StartPosX + 20.0f;
	fPosY = (float)StartPosY + 31.0f;

	sprintf_s(Buffer, sizeof(Buffer), GlobalText[211], this->ViewCurHP, this->ViewMaxHP);
	RenderText((int)fPosX, CenterTextPosY(Buffer, (int)fPosY), Buffer, REAL_WIDTH(130), RT3_SORT_LEFT, NULL);
}

void CPrintPlayer::RenderCharInfo_Energy(int StartPosX, int StartPosY)
{
	// Render Energy Box
	float fPosX = (float)StartPosX + 13.0f;
	float fPosY = (float)StartPosY;

	RenderBitmap(245, fPosX, fPosY, 75.0f, 21.0f, (0.0f / 128.0f), (0.0f / 32.0f), (75.0f / 128.0f), (21.0f / 32.0f), true, true);

	// Render Plus Button
	if (this->ViewPoint > 0)
	{
		float ButtonPosX = (float)StartPosX + 125.0f;

		if (IsWorkZone((int)ButtonPosX, (int)fPosY, 21, 21) && MouseLButton)
		{
			RenderBitmap(0x121, ButtonPosX, fPosY, 21.0f, 21.0f, (0.0f / 32.0f), (0.0f / 32.0f), (24.0f / 32.0f), (24.0f / 32.0f), true, true);
		}
		else
		{
			RenderBitmap(0x120, ButtonPosX, fPosY, 21.0f, 21.0f, (0.0f / 32.0f), (0.0f / 32.0f), (24.0f / 32.0f), (24.0f / 32.0f), true, true);
		}
	}

	// Render Energy Text
	SelectObject(m_hFontDC, g_hFont);
	SetBackgroundTextColor = Color4b(255, 255, 255, 0);
	SetTextColor = Color4b(223, 191, 103, 255);

	char Buffer[256];
	sprintf_s(Buffer, sizeof(Buffer), GlobalText[212], gPrintPlayer.ViewEnergy);
	RenderText((int)fPosX, CenterTextPosY(Buffer, (int)fPosY + 10), Buffer, REAL_WIDTH(75), RT3_SORT_CENTER, NULL);

	// Render Mana Text
	SetBackgroundTextColor = Color4b(0, 0, 0, 128);
	SetTextColor = Color4b(255, 255, 255, 255);

	fPosX = (float)StartPosX + 20.0f;
	fPosY = (float)StartPosY + 31.0f;

	sprintf_s(Buffer, sizeof(Buffer), GlobalText[213], this->ViewCurMP, this->ViewMaxMP);
	RenderText((int)fPosX, CenterTextPosY(Buffer, (int)fPosY), Buffer, REAL_WIDTH(130), RT3_SORT_LEFT, NULL);

	// Render Magic Damage Text
	BYTE Class = *(BYTE*)(Hero + 0x1BC) & 7;

	if (Class == CLASS_DARK_WIZARD || Class == CLASS_MAGIC_GLADIATOR)
	{
		SetBackgroundTextColor = Color4b(0, 0, 0, 128);
		SetTextColor = Color4b(255, 255, 255, 255);

		fPosY += 15.0f;

		if (this->ViewMagicDamageRate > 0)
		{
			sprintf_s(Buffer, sizeof(Buffer), GlobalText[215], this->ViewMagicDamageMin, this->ViewMagicDamageMax, this->ViewMagicDamageRate);
		}
		else
		{
			sprintf_s(Buffer, sizeof(Buffer), GlobalText[216], this->ViewMagicDamageMin, this->ViewMagicDamageMax);
		}

		RenderText((int)fPosX, CenterTextPosY(Buffer, (int)fPosY), Buffer, REAL_WIDTH(130), RT3_SORT_LEFT, NULL);
	}

	// Render Skill Damage Text
	if (Class == CLASS_DARK_KNIGHT || Class == CLASS_MAGIC_GLADIATOR)
	{
		SetBackgroundTextColor = Color4b(0, 0, 0, 128);
		SetTextColor = Color4b(255, 255, 255, 255);

		fPosY += 15.0f;

		sprintf_s(Buffer, sizeof(Buffer), GlobalText[582], this->ViewDamageMultiplier);

		RenderText((int)fPosX, CenterTextPosY(Buffer, (int)fPosY), Buffer, REAL_WIDTH(130), RT3_SORT_LEFT, NULL);
	}
}