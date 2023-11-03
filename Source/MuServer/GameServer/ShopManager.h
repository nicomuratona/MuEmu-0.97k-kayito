#pragma once

#include "Shop.h"

#define MAX_SHOP 50

#define SHOP_RANGE(x) (((x)<0)?0:((x)>=MAX_SHOP)?0:1)

struct SHOP_MANAGER_INFO
{
	int Index;
	int MonsterClass;
	CShop Shop;
};

class CShopManager
{
public:

	CShopManager();

	~CShopManager();

	void Load(char* path);

	void LoadShop();

	void ReloadShopInterface();

	long GetShopNumber(int MonsterClass);

	bool GetItemByIndex(int index, CItem* lpItem, int slot);

	bool GetItemByMonsterClass(int MonsterClass, CItem* lpItem, int slot);

	long GetItemCountByIndex(int index);

	long GetItemCountByMonsterClass(int MonsterClass);

	bool GCShopItemListSendByIndex(int index, int aIndex);

	bool GCShopItemListSendByMonsterClass(int MonsterClass, int aIndex);

private:

	std::map<int, SHOP_MANAGER_INFO> m_ShopManagerInfo;
};

extern CShopManager gShopManager;