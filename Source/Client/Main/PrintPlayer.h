#pragma once

#define GET_MAX_WORD_VALUE(x) (((x)>65000)?65000:((WORD)(x)))

class CPrintPlayer
{
public:

	CPrintPlayer();

	virtual ~CPrintPlayer();

	void Init();

private:

	static void RenderCircleHPMP(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale);

	static void RenderCircleAG(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale);

	static void RenderTextHP(char* Dest, char* Format);

	static float RenderNumberHP(float x, float y, int Num, float Width, float Height);

	static void RenderTextMP(char* Dest, char* Format);

	static float RenderNumberMP(float x, float y, int Num, float Width, float Height);

	static void RenderTextBP(char* Dest, char* Format);

	static float RenderNumberBP(float x, float y, int Num, float Width, float Height);

	static void RenderTextExperience(char* Dest, char* Format);

	static void RenderExperience();

	static void RenderTextLevelUpPoints(char* Dest, char* Format);

	static void RenderTextStrength(char* Dest, char* Format);

	static void RenderTextDexterity(char* Dest, char* Format);

	static void RenderTextVitality(char* Dest, char* Format);

	static void RenderTextEnergy(char* Dest, char* Format);

	static void CalculateAttackSpeed(DWORD This);

	static void SetAttackSpeed();

	static void RenderDamageHP(float Position[3], int Value, float Color[3], float scale, bool bMove);

public:

	DWORD ViewIndex;

	DWORD ViewReset;

	DWORD ViewGrandReset;

	DWORD ViewValue;

	DWORD ViewPoint;

	DWORD ViewCurHP;

	DWORD ViewMaxHP;

	DWORD ViewCurMP;

	DWORD ViewMaxMP;

	DWORD ViewCurBP;

	DWORD ViewMaxBP;

	DWORD ViewDamageHP;

	DWORD ViewExperience;

	DWORD ViewNextExperience;

	DWORD ViewStrength;

	DWORD ViewDexterity;

	DWORD ViewVitality;

	DWORD ViewEnergy;

	DWORD ViewPhysiSpeed;

	DWORD ViewMagicSpeed;

	DWORD MaxCharacterLevel;
};

extern CPrintPlayer gPrintPlayer;