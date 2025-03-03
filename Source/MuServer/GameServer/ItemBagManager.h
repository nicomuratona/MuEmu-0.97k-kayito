#pragma once

#include "ItemBag.h"
#include "ItemBagEx.h"
#include "User.h"

enum eItemBagSpecialValue
{
	ITEM_BAG_DEVIL_SQUARE1 = 0,
	ITEM_BAG_DEVIL_SQUARE2 = 1,
	ITEM_BAG_DEVIL_SQUARE3 = 2,
	ITEM_BAG_DEVIL_SQUARE4 = 3,

	ITEM_BAG_BLOOD_CASTLE1 = 4,
	ITEM_BAG_BLOOD_CASTLE2 = 5,
	ITEM_BAG_BLOOD_CASTLE3 = 6,
	ITEM_BAG_BLOOD_CASTLE4 = 7,
	ITEM_BAG_BLOOD_CASTLE5 = 8,
	ITEM_BAG_BLOOD_CASTLE6 = 9,

	ITEM_BAG_GOLDEN_ARCHER1 = 10,
	ITEM_BAG_GOLDEN_ARCHER2 = 11,
	ITEM_BAG_GOLDEN_ARCHER3 = 12,
	ITEM_BAG_GOLDEN_ARCHER4 = 13,

	ITEM_BAG_GOLDEN_ARCHER_BINGO = 14,
};

struct ITEM_BAG_MANAGER_INFO
{
	int Index;
	int ItemIndex;
	int ItemLevel;
	int MonsterClass;
	int SpecialValue;
	CItemBag ItemBag;
};

class CItemBagManager
{
public:

	CItemBagManager();

	~CItemBagManager();

	void Load(char* path);

	void LoadEventItemBag();

	bool GetItemByItemIndex(int ItemIndex, int ItemLevel, LPOBJ lpObj, CItem* lpItem);

	bool GetItemByMonsterClass(int MonsterClass, LPOBJ lpObj, CItem* lpItem);

	bool GetItemBySpecialValue(int SpecialValue, LPOBJ lpObj, CItem* lpItem);

	bool DropItemByItemIndex(int ItemIndex, int ItemLevel, LPOBJ lpObj, int map, int x, int y);

	bool DropItemByMonsterClass(int MonsterClass, LPOBJ lpObj, int map, int x, int y);

	bool DropItemBySpecialValue(int SpecialValue, LPOBJ lpObj, int map, int x, int y);

private:

	std::map<int, ITEM_BAG_MANAGER_INFO> m_ItemBagManagerInfo;
};

extern CItemBagManager gItemBagManager;