#include "stdafx.h"
#include "PrintPlayer.h"
#include "Protect.h"

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

	this->ViewPing = 0;
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

	SetCompleteHook(0xE8, 0x004EE8E9, &this->RenderTextHP); // text.bmd 211: "HP: %d / %d"

	SetCompleteHook(0xE8, 0x004BE2C5, &this->RenderNumberHP); // Render the number of HP

	SetCompleteHook(0xE8, 0x004BDAAE, &this->RenderTextMP); // text.bmd 359: "Mana: %d/%d"

	SetCompleteHook(0xE8, 0x004EE9DC, &this->RenderTextMP); // text.bmd 213: "Mana: %d / %d"

	SetCompleteHook(0xE8, 0x004BE2EC, &this->RenderNumberMP); // Render the number of MP

	SetCompleteHook(0xE8, 0x004BD055, &this->RenderTextBP); // text.bmd 214: "A G: %d / %d"

	SetCompleteHook(0xE8, 0x004BD00C, &this->RenderNumberBP); // Render the number of BP

	SetCompleteHook(0xE8, 0x004ED48F, &this->RenderTextExperience); // text.bmd 201: "Exp: %u/%u"

	SetCompleteHook(0xE8, 0x004BC0F8, &this->RenderExperience); // Render the experience bar and number

	SetCompleteHook(0xE8, 0x004EEE67, &this->RenderTextLevelUpPoints); // text.bmd 217: "Point: %d"

	SetCompleteHook(0xE8, 0x004ED7E3, &this->RenderTextStrength); // text.bmd 202: "Strength: %d"

	SetCompleteHook(0xE8, 0x004EE452, &this->RenderTextDexterity); // text.bmd 205: "Agility: %d"

	SetCompleteHook(0xE8, 0x004EE85B, &this->RenderTextVitality); // text.bmd 210: "Vitality: %d"

	SetCompleteHook(0xE8, 0x004EE95E, &this->RenderTextEnergy); // text.bmd 212: "Energy: %d"

	SetCompleteHook(0xE9, 0x0047DD80, &this->CalculateAttackSpeed); // Fix the attack speed

	SetCompleteHook(0xE9, 0x00443E70, &this->SetAttackSpeed); // Set the attack speed

	SetCompleteHook(0xE8, 0x0042AD92, &this->RenderDamageHP); // Damage number on attack

	SetCompleteHook(0xE8, 0x0042B38E, &this->RenderDamageHP); // Damage number on attack

	SetCompleteHook(0xE8, 0x0042B402, &this->RenderDamageHP); // Damage number on attack

	SetCompleteHook(0xE8, 0x0042DA3D, &this->RenderDamageHP); // Damage number on attack

	SetCompleteHook(0xE8, 0x0042E2CA, &this->RenderDamageHP); // Damage number on attack

	SetCompleteHook(0xE8, 0x0042E2FE, &this->RenderDamageHP); // Damage number on attack

	SetCompleteHook(0xE8, 0x0042ED2C, &this->RenderDamageHP); // Damage number on attack

	SetCompleteHook(0xE8, 0x0042ED60, &this->RenderDamageHP); // Damage number on attack
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

void CPrintPlayer::RenderTextExperience(char* Dest, char* Format)
{
	wsprintf(Dest, Format, gPrintPlayer.ViewExperience, gPrintPlayer.ViewNextExperience);
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
		dwPriorExperience = (((wPriorLevel + 9) * wPriorLevel) * wPriorLevel) * 2;

		if (wPriorLevel > 255)
		{
			int iLevelOverN = wPriorLevel - 255;

			dwPriorExperience += (((iLevelOverN + 9) * iLevelOverN) * iLevelOverN) * 5;
		}
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

		wsprintf(strTipText, GetTextLine(201), gPrintPlayer.ViewExperience, gPrintPlayer.ViewNextExperience); // text.bmd 201: "Exp: %u/%u"

		RenderTipText(X + 2, Y - 15, strTipText);
	}
}

void CPrintPlayer::RenderTextLevelUpPoints(char* Dest, char* Format)
{
	wsprintf(Dest, Format, gPrintPlayer.ViewPoint);
}

void CPrintPlayer::RenderTextStrength(char* Dest, char* Format)
{
	wsprintf(Dest, Format, gPrintPlayer.ViewStrength);
}

void CPrintPlayer::RenderTextDexterity(char* Dest, char* Format)
{
	wsprintf(Dest, Format, gPrintPlayer.ViewDexterity);
}

void CPrintPlayer::RenderTextVitality(char* Dest, char* Format)
{
	wsprintf(Dest, Format, gPrintPlayer.ViewVitality);
}

void CPrintPlayer::RenderTextEnergy(char* Dest, char* Format)
{
	wsprintf(Dest, Format, gPrintPlayer.ViewEnergy);
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

	DWORD b = *(DWORD*)(Models + 73368); // &Models[MODEL_PLAYER]

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