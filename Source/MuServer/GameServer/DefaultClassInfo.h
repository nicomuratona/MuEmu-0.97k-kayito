#pragma once

#define MAX_CLASS 4

enum eClassNumber
{
	CLASS_DW = 0,
	CLASS_DK = 1,
	CLASS_FE = 2,
	CLASS_MG = 3,
};

enum eDBClassNumber
{
	DB_CLASS_DW = 0,
	DB_CLASS_SM = 1,
	DB_CLASS_DK = 16,
	DB_CLASS_BK = 17,
	DB_CLASS_FE = 32,
	DB_CLASS_ME = 33,
	DB_CLASS_MG = 48,
};

struct DEFAULT_CLASS_INFO
{
	int Class;
	int Strength;
	int Dexterity;
	int Vitality;
	int Energy;
	float MaxLife;
	float MaxMana;
	float LevelLife;
	float LevelMana;
	float VitalityToLife;
	float EnergyToMana;
};

class CDefaultClassInfo
{
public:

	CDefaultClassInfo();

	~CDefaultClassInfo();

	void Init();

	void Load(char* path);

	void SetInfo(DEFAULT_CLASS_INFO info);

	int GetCharacterDefaultStat(int Class, int stat);

public:

	DEFAULT_CLASS_INFO m_DefaultClassInfo[MAX_CLASS];
};

extern CDefaultClassInfo gDefaultClassInfo;