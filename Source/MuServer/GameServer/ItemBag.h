#pragma once

#include "Item.h"
#include "ItemBagEx.h"
#include "User.h"

#define MAX_BAG_ITEM 500

struct ITEM_BAG_INFO
{
	int Index;
	int MinLevel;
	int MaxLevel;
	int SkillOption;
	int LuckOption;
	int AddOption;
	int ExceOption;
};

class CItemBag
{
public:

	CItemBag();

	~CItemBag();

	void Init();

	void Load(char* path);

	void SetInfo(ITEM_BAG_INFO info);

	ITEM_BAG_INFO* GetInfo(int index);

	int GetItemLevel(ITEM_BAG_INFO* lpInfo);

	int GetItemExceOption(ITEM_BAG_INFO* lpInfo, int dropType);

	bool GetRandomItemDropLocation(int map, int* ox, int* oy, int tx, int ty, int count);

	bool GetItem(LPOBJ lpObj, CItem* lpItem);

	bool DropItem(LPOBJ lpObj, int map, int x, int y);

private:

	char m_EventName[32];

	int m_DropZen;

	int m_ItemDropRate;

	int m_ItemDropCount;

	int m_ItemDropType;

	int m_SendFirework;

	ITEM_BAG_INFO m_BagInfo[MAX_BAG_ITEM];

	int m_count;

	CItemBagEx m_ItemBagEx;
};