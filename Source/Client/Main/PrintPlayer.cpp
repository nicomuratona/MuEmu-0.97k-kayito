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

	this->ViewAddStrength = 0;

	this->ViewAddDexterity = 0;

	this->ViewAddVitality = 0;

	this->ViewAddEnergy = 0;

	this->ViewPhysiSpeed = 0;

	this->ViewMagicSpeed = 0;
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

	SetCompleteHook(0xE9, 0x0047DD95, &this->PrintPlayerSetAttackSpeed); // Set the attack speed

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

	WORD wLevel = *(WORD*)(*(DWORD*)(CharacterAttribute)+0x0E); // current level

	STRUCT_ENCRYPT;

	WORD wPriorLevel = wLevel - 1;

	DWORD Experiences[2] = { 0, 0 };

	for (int n = wPriorLevel; n <= wLevel; n++)
	{
		if (n > 0)
		{
			Experiences[n - wPriorLevel] = (((n + 9) * n) * n) * 10;

			if (n > 255)
			{
				int iLevelOverN = n - 255;

				Experiences[n - wPriorLevel] += (((iLevelOverN + 9) * iLevelOverN) * iLevelOverN) * 1000;
			}
		}
	}

	DWORD RequiredExp = Experiences[1] - Experiences[0];

	DWORD ActualExp = gPrintPlayer.ViewExperience - Experiences[0];

	float TotalBarRate = (float)ActualExp / (float)RequiredExp;

	float Width = TotalBarRate * 198.0f;

	int Number = (int)((TotalBarRate * 100.0f) / 10.0f);

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

__declspec(naked) void CPrintPlayer::PrintPlayerSetAttackSpeed()
{
	static DWORD PrintPlayerSetAttackSpeedAddress1 = 0x0047DE20;

	_asm
	{
		Mov Edx, gPrintPlayer.ViewPhysiSpeed;
		Movzx Eax, Al;
		Mov Ecx, Dword Ptr Ds : [gProtect.m_MainInfo.DWMaxAttackSpeed + Eax * 4] ;
		And Ecx, 0xFFFF;
		Cmp Edx, Ecx;
		Jle NEXT1;
		Mov Edx, Ecx;
	NEXT1:
		Lea Esi, [Ebx + 0x38];
		Mov Word Ptr Ds : [Esi] , Dx;
		Mov Edx, gPrintPlayer.ViewMagicSpeed;
		Mov Ecx, Dword Ptr Ds : [gProtect.m_MainInfo.DWMaxAttackSpeed + Eax * 4] ;
		And Ecx, 0xFFFF;
		Cmp Edx, Ecx;
		Jle NEXT2;
		Mov Edx, Ecx;
	NEXT2:
		Lea Edi, [Ebx + 0x44];
		Jmp[PrintPlayerSetAttackSpeedAddress1];
	}
}

void CPrintPlayer::RenderDamageHP(float Position[3], int Value, float Color[3], float scale, bool bMove)
{
	if (Value > 0)
	{
		Value = gPrintPlayer.ViewDamageHP;
	}

	CreatePoint(Position, Value, Color, scale, bMove);
}