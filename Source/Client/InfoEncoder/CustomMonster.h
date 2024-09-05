#pragma once

struct CUSTOM_MONSTER_INFO
{
	int Index;
	short MonsterIndex;
	BYTE MonsterType;
	bool GoldenType;
	float Scale;
	char FolderName[128];
	char ModelName[128];
};

class CCustomMonster
{
public:

	CCustomMonster();

	virtual ~CCustomMonster();

	void Init();

	void Load(char* path);

	void SetInfo(CUSTOM_MONSTER_INFO info);

public:

	CUSTOM_MONSTER_INFO m_CustomMonsterInfo[MAX_MONSTER];
};

extern CCustomMonster gCustomMonster;