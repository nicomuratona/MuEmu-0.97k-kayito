#pragma once

#define MAX_MONSTER_INFO 1000
#define MAX_MONSTER_LEVEL 150
#define MAX_MONSTER_ITEM 100

struct MONSTER_INFO
{
	int Index;
	int Rate;
	char Name[32];
	int Level;
	int AINumber;
	int ScriptLife;
	int Life;
	int Mana;
	int DamageMin;
	int DamageMax;
	int Defense;
	int MagicDefense;
	int AttackRate;
	int DefenseRate;
	int MoveRange;
	int AttackRange;
	int AttackType;
	int ViewRange;
	int MoveSpeed;
	int AttackSpeed;
	int RegenTime;
	int Attribute;
	int ItemRate;
	int MoneyRate;
	int MaxItemLevel;
	int Resistance[4];
	int MonsterSkill;
};

struct MONSTER_ITEM_INFO
{
	int IndexTable[MAX_MONSTER_ITEM];
	int IndexCount;
};

class CMonsterManager
{
public:

	CMonsterManager();

	~CMonsterManager();

	void Init();

	void Load(char* path);

	void SetInfo(MONSTER_INFO info);

	MONSTER_INFO* GetInfo(int index);

	void InitMonsterItem();

	void InsertMonsterItem(int level);

	long GetMonsterItem(int level, int excellent);

	void SetMonsterData();

	char* GetMonsterName(int index);

private:

	MONSTER_INFO m_MonsterInfo[MAX_MONSTER_INFO];

	MONSTER_ITEM_INFO m_MonsterItemInfo[MAX_MONSTER_LEVEL];

	int m_MonsterItemCount;
};

extern CMonsterManager gMonsterManager;