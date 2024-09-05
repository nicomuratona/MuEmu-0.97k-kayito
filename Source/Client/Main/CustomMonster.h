#pragma once

struct CUSTOM_MONSTER_INFO
{
	int Index;
	short MonsterIndex;
	BYTE MonsterType;
	BYTE GoldenType;
	float Scale;
	char FolderName[128];
	char ModelName[128];
};

class CCustomMonster
{
public:

	CCustomMonster();

	virtual ~CCustomMonster();

	void Load(CUSTOM_MONSTER_INFO* info);

private:

	void SetInfo(CUSTOM_MONSTER_INFO info);

	CUSTOM_MONSTER_INFO* GetInfo(int index);

	CUSTOM_MONSTER_INFO* GetInfoByIndex(int Index);

	void Init();

	static DWORD CreateCustomMonster(int Type, int PositionX, int PositionY, int Key);

	static void CustomMonsterDie();

	static void CustomMonsterGolden();

	static void CustomNpcTalk();

private:

	CUSTOM_MONSTER_INFO m_CustomMonsterInfo[MAX_MONSTER];
};

extern CCustomMonster gCustomMonster;