#include "stdafx.h"
#include "PrintPlayer.h"
#include "Protect.h"

DWORD ViewIndex = 0;

DWORD ViewReset = 0;

DWORD ViewGrandReset = 0;

DWORD ViewValue = 0;

DWORD ViewPoint = 0;

DWORD ViewCurHP = 0;

DWORD ViewMaxHP = 0;

DWORD ViewCurMP = 0;

DWORD ViewMaxMP = 0;

DWORD ViewCurBP = 0;

DWORD ViewMaxBP = 0;

DWORD ViewDamageHP = 0;

DWORD ViewExperience = 0;

DWORD ViewNextExperience = 0;

DWORD ViewStrength = 0;

DWORD ViewDexterity = 0;

DWORD ViewVitality = 0;

DWORD ViewEnergy = 0;

DWORD ViewAddStrength = 0;

DWORD ViewAddDexterity = 0;

DWORD ViewAddVitality = 0;

DWORD ViewAddEnergy = 0;

DWORD ViewPhysiSpeed = 0;

DWORD ViewMagicSpeed = 0;

DWORD gLevelExperience[MAX_CHARACTER_LEVEL + 1];

void InitPrintPlayer()
{
	SetCompleteHook(0xE8, 0x004BD98F, &PrintDrawCircleHPMP);

	SetCompleteHook(0xE8, 0x004BDA56, &PrintDrawCircleHPMP);

	SetCompleteHook(0xE8, 0x004BCFF2, &PrintDrawCircleAG);

	SetCompleteHook(0xE8, 0x004BD9E7, &PrintPlayerViewHP1); //case 358: "Life: %d/%d"

	SetCompleteHook(0xE8, 0x004EE8E9, &PrintPlayerViewHP1); //case 211: "HP: %d / %d"

	SetCompleteHook(0xE8, 0x004BE2C5, &PrintPlayerViewHP2);

	SetCompleteHook(0xE8, 0x004BDAAE, &PrintPlayerViewMP1); //case 359: "Mana: %d/%d"

	SetCompleteHook(0xE8, 0x004EE9DC, &PrintPlayerViewMP1); //case 213: "Mana: %d / %d"

	SetCompleteHook(0xE8, 0x004BE2EC, &PrintPlayerViewMP2);

	SetCompleteHook(0xE8, 0x004BD055, &PrintPlayerViewBP1); //case 214: "A G: %d / %d"

	SetCompleteHook(0xE8, 0x004BD00C, &PrintPlayerViewBP2);

	SetCompleteHook(0xE8, 0x004ED48F, &PrintPlayerViewExperience); //case 201: "Exp: %u/%u"

	SetCompleteHook(0xE8, 0x004BFCD3, &PrintPlayerViewExperience); //case 357: "Exp: %u/%u"

	SetCompleteHook(0xE8, 0x004EEE67, &PrintPlayerViewLevelUpPoint); //case 217: "Point: %d"

	SetCompleteHook(0xE8, 0x004ED7E3, &PrintPlayerViewStrength); //case 202: "Strength: %d"

	SetCompleteHook(0xE8, 0x004EE452, &PrintPlayerViewDexterity); //case 205: "Agility: %d"

	SetCompleteHook(0xE8, 0x004EE85B, &PrintPlayerViewVitality); //case 210: "Vitality: %d"

	SetCompleteHook(0xE8, 0x004EE95E, &PrintPlayerViewEnergy); //case 212: "Energy: %d"

	SetCompleteHook(0xE9, 0x0047DD95, &PrintPlayerSetAttackSpeed);

	SetCompleteHook(0xE8, 0x0042AD92, &PrintDamageOnScreenHP);

	SetCompleteHook(0xE8, 0x0042B38E, &PrintDamageOnScreenHP);

	SetCompleteHook(0xE8, 0x0042B402, &PrintDamageOnScreenHP);

	SetCompleteHook(0xE8, 0x0042DA3D, &PrintDamageOnScreenHP);

	SetCompleteHook(0xE8, 0x0042E2CA, &PrintDamageOnScreenHP);

	SetCompleteHook(0xE8, 0x0042E2FE, &PrintDamageOnScreenHP);

	SetCompleteHook(0xE8, 0x0042ED2C, &PrintDamageOnScreenHP);

	SetCompleteHook(0xE8, 0x0042ED60, &PrintDamageOnScreenHP);

	gObjSetExperienceTable();

	SetCompleteHook(0xE8, 0x004BFC26, &PrintExperienceBar);

	SetCompleteHook(0xE8, 0x004BFC5B, &PrintExperienceNumber);
}

void PrintDrawCircleHPMP(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale)
{
	float HP = (float)(ViewMaxHP - ViewCurHP) / (float)ViewMaxHP;

	float MP = (float)(ViewMaxMP - ViewCurMP) / (float)ViewMaxMP;

	y = ((Texture == 0xEB) ? MP : HP) * 48.0f + 432.0f;

	Height = 48.0f - ((Texture == 0xEB) ? MP : HP) * 48.0f;

	v = ((Texture == 0xEB) ? MP : HP) * 48.0f / 64.0f;

	vHeight = (1.0f - ((Texture == 0xEB) ? MP : HP)) * 48.0f / 64.0f;

	return RenderBitmap(Texture, x, y, Width, Height, u, v, uWidth, vHeight, Scale, StartScale);
}

void PrintDrawCircleAG(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale)
{
	float BP = (float)(ViewMaxBP - ViewCurBP) / (float)ViewMaxBP;

	y = BP * 36.0f + 438.0f;

	Height = 36.0f - BP * 36.0f;

	v = BP * 36.0f / 64.0f;

	vHeight = (1.0f - BP) * 36.0f / 64.0f;

	return RenderBitmap(Texture, x, y, Width, Height, u, v, uWidth, vHeight, Scale, StartScale);
}

void PrintPlayerViewHP1(char* Dest, char* Format)
{
	wsprintf(Dest, Format, ViewCurHP, ViewMaxHP);
}

void PrintPlayerViewHP2(float x, float y, int Num, float Width, float Height)
{
	RenderNumber2D(x, y, ViewCurHP, 9.0f, 10.0f);
}

void PrintPlayerViewMP1(char* Dest, char* Format)
{
	wsprintf(Dest, Format, ViewCurMP, ViewMaxMP);
}

void PrintPlayerViewMP2(float x, float y, int Num, float Width, float Height)
{
	RenderNumber2D(x - 20, y, ViewCurMP, 9.0f, 10.0f);
}

void PrintPlayerViewBP1(char* Dest, char* Format)
{
	wsprintf(Dest, Format, ViewCurBP, ViewMaxBP);
}

void PrintPlayerViewBP2(float x, float y, int Num, float Width, float Height)
{
	RenderNumber2D(x, y, ViewCurBP, 9.0f, 10.0f);
}

void PrintPlayerViewExperience(char* Dest, char* Format)
{
	wsprintf(Dest, Format, ViewExperience, ViewNextExperience);
}

void PrintPlayerViewLevelUpPoint(char* Dest, char* Format)
{
	wsprintf(Dest, Format, ViewPoint);
}

void PrintPlayerViewStrength(char* Dest, char* Format)
{
	wsprintf(Dest, Format, ViewStrength);
}

void PrintPlayerViewDexterity(char* Dest, char* Format)
{
	wsprintf(Dest, Format, ViewDexterity);
}

void PrintPlayerViewVitality(char* Dest, char* Format)
{
	wsprintf(Dest, Format, ViewVitality);
}

void PrintPlayerViewEnergy(char* Dest, char* Format)
{
	wsprintf(Dest, Format, ViewEnergy);
}

__declspec(naked) void PrintPlayerSetAttackSpeed()
{
	static DWORD PrintPlayerSetAttackSpeedAddress1 = 0x0047DE20;

	_asm
	{
		Mov Edx, ViewPhysiSpeed;
		Movzx Eax, Al;
		Mov Ecx, Dword Ptr Ds : [gProtect.m_MainInfo.DWMaxAttackSpeed + Eax * 4] ;
		And Ecx, 0xFFFF;
		Cmp Edx, Ecx;
		Jle NEXT1;
		Mov Edx, Ecx;
	NEXT1:
		Lea Esi, [Ebx + 0x38];
		Mov Word Ptr Ds : [Esi] , Dx;
		Mov Edx, ViewMagicSpeed;
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

void PrintDamageOnScreenHP(float Position[3], int Value, float Color[3], float scale, bool bMove)
{
	if (Value > 0)
	{
		Value = ViewDamageHP;
	}

	CreatePoint(Position, Value, Color, scale, bMove);
}

void gObjSetExperienceTable()
{
	gLevelExperience[0] = 0;

	DWORD over = 1;

	for (int n = 1; n <= MAX_CHARACTER_LEVEL; n++)
	{
		gLevelExperience[n] = (((n + 9) * n) * n) * 10;

		if (n > 255)
		{
			gLevelExperience[n] += (((over + 9) * over) * over) * 1000;

			over++;
		}
	}
}

void PrintExperienceBar(float x, float y, float width, float height)
{
	DWORD RequiredExp = gLevelExperience[*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x0E)] - gLevelExperience[*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x0E) - 1];

	DWORD ActualExp = gLevelExperience[*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x0E)] - ViewExperience;

	float TotalBarRate = (float)ActualExp / (float)RequiredExp;

	float TotalBarNumber = 1782.0f * TotalBarRate;

	float Number = TotalBarNumber / 198.0f;

	float parteDecimal, parteEntera;

	parteDecimal = modf(Number, &parteEntera);

	float size = (parteDecimal == 0) ? 0 : (198.0f - (parteDecimal * 198.0f));

	if (*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x0E) == MAX_CHARACTER_LEVEL)
	{
		size = 198.0f;
	}

	RenderColor(x, y, size, height);
}

void PrintExperienceNumber(float x, float y, int Num, float Width, float Height)
{
	DWORD RequiredExp = gLevelExperience[*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x0E)] - gLevelExperience[*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x0E) - 1];

	DWORD ActualExp = gLevelExperience[*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x0E)] - ViewExperience;

	float TotalBarRate = (float)ActualExp / (float)RequiredExp;

	float TotalBarNumber = 1782.0f * TotalBarRate;

	float Number = TotalBarNumber / 198.0f;

	float parteDecimal, parteEntera;

	parteDecimal = modf(Number, &parteEntera);

	parteEntera = 9 - parteEntera;

	if (*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x0E) == MAX_CHARACTER_LEVEL)
	{
		parteEntera = 9;
	}

	RenderNumber2D(x, y, (DWORD)parteEntera, Width, Height);
}