#pragma once

#include "Item.h"
#include "ItemManager.h"
#include "ProtocolDefines.h"

#define SHOP_SIZE 120

#define SHOP_INVENTORY_RANGE(x) (((x)<0)?0:((x)>=SHOP_SIZE)?0:1)

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_SHOP_ITEM_LIST_SEND
{
	PWMSG_HEAD header; // C2:31
	BYTE type;
	BYTE count;
};

struct PMSG_SHOP_ITEM_LIST
{
	BYTE slot;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

//**********************************************//
//**********************************************//
//**********************************************//

class CShop
{
public:

	CShop();

	~CShop();

	void Init();

	void Load(char* path);

	void ShopItemSet(int slot, BYTE type);

	BYTE ShopRectCheck(int x, int y, int width, int height);

	void InsertItem(int ItemIndex, int ItemLevel, int ItemDurability, int ItemSkillOption, int ItemLuckOption, int ItemAddOption, int ItemExceOption);

	void InsertItem(int ItemIndex, int ItemLevel, int ItemDurability, int ItemSkillOption, int ItemLuckOption, int ItemAddOption, int ItemExceOption, int ItemSlotX, int ItemSlotY);

	bool GetItem(CItem* lpItem, int slot);

	long GetItemCount();

	bool GCShopItemListSend(int aIndex);

private:

	CItem m_Item[SHOP_SIZE];

	BYTE m_InventoryMap[SHOP_SIZE];
};