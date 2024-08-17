#include "stdafx.h"
#include "Shop.h"
#include "ReadScript.h"
#include "User.h"
#include "Util.h"

CShop::CShop()
{
	this->Init();
}

CShop::~CShop()
{

}

void CShop::Init()
{
	for (int n = 0; n < SHOP_SIZE; n++)
	{
		this->m_Item[n].Clear();

		this->m_InventoryMap[n] = -1;
	}
}

void CShop::Load(char* path)
{
	CReadScript* lpReadScript = new CReadScript;

	if (lpReadScript == NULL)
	{
		ErrorMessageBox(READ_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (!lpReadScript->Load(path))
	{
		ErrorMessageBox(READ_SCRIPT_FILE_ERROR, path);

		delete lpReadScript;

		return;
	}

	this->Init();

	try
	{
		eTokenResult token;

		while (true)
		{
			token = lpReadScript->GetToken();

			if (token == TOKEN_END || token == TOKEN_END_SECTION)
			{
				break;
			}

			int ItemIndex = lpReadScript->GetNumber();

			ItemIndex = SafeGetItem(GET_ITEM(ItemIndex, lpReadScript->GetAsNumber()));

			int ItemLevel = lpReadScript->GetAsNumber();

			int ItemDurability = lpReadScript->GetAsNumber();

			int ItemSkillOption = lpReadScript->GetAsNumber();

			int ItemLuckOption = lpReadScript->GetAsNumber();

			int ItemAddOption = lpReadScript->GetAsNumber();

			int ItemExceOption = lpReadScript->GetAsNumber();

			int ItemSlotX = lpReadScript->GetAsNumber();

			int ItemSlotY = lpReadScript->GetAsNumber();

			if (ItemSlotX == -1 || ItemSlotY == -1)
			{
				this->InsertItem(ItemIndex, ItemLevel, ItemDurability, ItemSkillOption, ItemLuckOption, ItemAddOption, ItemExceOption);
			}
			else
			{
				this->InsertItem(ItemIndex, ItemLevel, ItemDurability, ItemSkillOption, ItemLuckOption, ItemAddOption, ItemExceOption, ItemSlotX, ItemSlotY);
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript->GetError());
	}

	delete lpReadScript;
}

void CShop::ShopItemSet(int slot, BYTE type)
{
	if (SHOP_INVENTORY_RANGE(slot) == false)
	{
		return;
	}

	ITEM_INFO ItemInfo;

	if (gItemManager.GetInfo(this->m_Item[slot].m_Index, &ItemInfo) == 0)
	{
		return;
	}

	int x = slot % 8;

	int y = slot / 8;

	if ((x + ItemInfo.Width) > 8 || (y + ItemInfo.Height) > 15)
	{
		return;
	}

	for (int sy = 0; sy < ItemInfo.Height; sy++)
	{
		for (int sx = 0; sx < ItemInfo.Width; sx++)
		{
			this->m_InventoryMap[(((sy + y) * 8) + (sx + x))] = type;
		}
	}
}

BYTE CShop::ShopRectCheck(int x, int y, int width, int height)
{
	if ((x + width) > 8 || (y + height) > 15)
	{
		return 0xFF;
	}

	for (int sy = 0; sy < height; sy++)
	{
		for (int sx = 0; sx < width; sx++)
		{
			if (this->m_InventoryMap[(((sy + y) * 8) + (sx + x))] != 0xFF)
			{
				return 0xFF;
			}
		}
	}

	return ((y * 8) + x);
}

void CShop::InsertItem(int ItemIndex, int ItemLevel, int ItemDurability, int ItemSkillOption, int ItemLuckOption, int ItemAddOption, int ItemExceOption)
{
	ITEM_INFO ItemInfo;

	if (gItemManager.GetInfo(ItemIndex, &ItemInfo) == 0)
	{
		return;
	}

	for (int y = 0; y < 15; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (this->m_InventoryMap[((y * 8) + x)] == 0xFF)
			{
				BYTE slot = this->ShopRectCheck(x, y, ItemInfo.Width, ItemInfo.Height);

				if (slot != 0xFF)
				{
					this->m_Item[slot].m_Level = ItemLevel;

					this->m_Item[slot].m_Durability = (float)((ItemDurability == -1) ? gItemManager.GetItemDurability(ItemIndex, ItemLevel, ItemExceOption) : ItemDurability);

					this->m_Item[slot].Convert(ItemIndex, ItemSkillOption, ItemLuckOption, ItemAddOption, ItemExceOption);

					this->ShopItemSet(slot, 1);

					return;
				}
			}
		}
	}
}

void CShop::InsertItem(int ItemIndex, int ItemLevel, int ItemDurability, int ItemSkillOption, int ItemLuckOption, int ItemAddOption, int ItemExceOption, int ItemSlotX, int ItemSlotY)
{
	ITEM_INFO ItemInfo;

	if (gItemManager.GetInfo(ItemIndex, &ItemInfo) == 0)
	{
		return;
	}

	if (this->m_InventoryMap[((ItemSlotY * 8) + ItemSlotX)] == 0xFF)
	{
		BYTE slot = this->ShopRectCheck(ItemSlotX, ItemSlotY, ItemInfo.Width, ItemInfo.Height);

		if (slot != 0xFF)
		{
			this->m_Item[slot].m_Level = ItemLevel;

			this->m_Item[slot].m_Durability = (float)((ItemDurability == -1) ? gItemManager.GetItemDurability(ItemIndex, ItemLevel, ItemExceOption) : ItemDurability);

			this->m_Item[slot].Convert(ItemIndex, ItemSkillOption, ItemLuckOption, ItemAddOption, ItemExceOption);

			this->ShopItemSet(slot, 1);

			return;
		}
	}
}

bool CShop::GetItem(CItem* lpItem, int slot)
{
	if (SHOP_INVENTORY_RANGE(slot) != false)
	{
		if (this->m_Item[slot].IsItem() != 0)
		{
			(*lpItem) = this->m_Item[slot];

			return 1;
		}
	}

	return 0;
}

long CShop::GetItemCount()
{
	int count = 0;

	for (int n = 0; n < SHOP_SIZE; n++)
	{
		if (this->m_Item[n].IsItem() != 0)
		{
			count++;
		}
	}

	return count;
}

bool CShop::GCShopItemListSend(int aIndex)
{
	BYTE send[2048];

	PMSG_SHOP_ITEM_LIST_SEND pMsg;

	pMsg.header.set(0x31, 0);

	int size = sizeof(pMsg);

	pMsg.type = 0;

	pMsg.count = 0;

	PMSG_SHOP_ITEM_LIST info;

	for (int n = 0; n < SHOP_SIZE; n++)
	{
		if (this->m_Item[n].IsItem() != 0)
		{
			info.slot = n;

			gItemManager.ItemByteConvert(info.ItemInfo, this->m_Item[n]);

			memcpy(&send[size], &info, sizeof(info));

			size += sizeof(info);

			pMsg.count++;
		}
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(aIndex, send, size);

	return 1;
}