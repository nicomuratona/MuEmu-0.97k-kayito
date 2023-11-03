#pragma once

#define GET_MAX_WORD_VALUE(x) (((x)>65000)?65000:((WORD)(x)))

#define MAX_CHARACTER_LEVEL 400

void InitPrintPlayer();

void PrintDrawCircleHPMP(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale);

void PrintDrawCircleAG(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale);

void PrintPlayerViewHP1(char* Dest, char* Format);

void PrintPlayerViewHP2(float x, float y, int Num, float Width, float Height);

void PrintPlayerViewMP1(char* Dest, char* Format);

void PrintPlayerViewMP2(float x, float y, int Num, float Width, float Height);

void PrintPlayerViewBP1(char* Dest, char* Format);

void PrintPlayerViewBP2(float x, float y, int Num, float Width, float Height);

void PrintPlayerViewExperience(char* Dest, char* Format);

void PrintPlayerViewLevelUpPoint(char* Dest, char* Format);

void PrintPlayerViewStrength(char* Dest, char* Format);

void PrintPlayerViewDexterity(char* Dest, char* Format);

void PrintPlayerViewVitality(char* Dest, char* Format);

void PrintPlayerViewEnergy(char* Dest, char* Format);

void PrintPlayerSetAttackSpeed();

void PrintDamageOnScreenHP(float Position[3], int Value, float Color[3], float scale, bool bMove);

void gObjSetExperienceTable();

void PrintExperienceBar(float x, float y, float width, float height);

void PrintExperienceNumber(float x, float y, int Num, float Width, float Height);

extern DWORD ViewIndex;

extern DWORD ViewReset;

extern DWORD ViewGrandReset;

extern DWORD ViewValue;

extern DWORD ViewPoint;

extern DWORD ViewCurHP;

extern DWORD ViewMaxHP;

extern DWORD ViewCurMP;

extern DWORD ViewMaxMP;

extern DWORD ViewCurBP;

extern DWORD ViewMaxBP;

extern DWORD ViewDamageHP;

extern DWORD ViewExperience;

extern DWORD ViewNextExperience;

extern DWORD ViewStrength;

extern DWORD ViewDexterity;

extern DWORD ViewVitality;

extern DWORD ViewEnergy;

extern DWORD ViewAddStrength;

extern DWORD ViewAddDexterity;

extern DWORD ViewAddVitality;

extern DWORD ViewAddEnergy;

extern DWORD ViewPhysiSpeed;

extern DWORD ViewMagicSpeed;