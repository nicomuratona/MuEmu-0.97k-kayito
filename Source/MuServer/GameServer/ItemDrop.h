#pragma once

#include "User.h"

struct ITEM_DROP_INFO
{
	int Index;
	int Level;
	int Grade;
	int LevelOptionRate;
	int SkillOptionRate;
	int LuckOptionRate;
	int AddOptionRate;
	int ExceOptionRate;
	int MapNumber;
	int MonsterClass;
	int MonsterLevelMin;
	int MonsterLevelMax;
	int DropRate;
};

class CItemDrop
{
public:

	CItemDrop();

	~CItemDrop();

	void Load(char* path);

	int DropItem(LPOBJ lpObj, LPOBJ lpTarget);

	int GetItemDropRate(LPOBJ lpObj, LPOBJ lpTarget, int ItemIndex, int ItemLevel, int DropRate);

private:

	std::vector<ITEM_DROP_INFO> m_ItemDropInfo;
};

extern CItemDrop gItemDrop;