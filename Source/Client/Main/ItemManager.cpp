#include "stdafx.h"
#include "ItemManager.h"

CItemManager gItemManager;

CItemManager::CItemManager()
{

}

CItemManager::~CItemManager()
{

}

int CItemManager::GetInventoryEmptySlot(int ItemWidth, int ItemHeight)
{
	ITEM* item = nullptr;

	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			item = GetInventoryItem((x + (y * 8)));

			if (item->Type == -1)
			{
				BYTE slot = this->InterfaceRectCheck(x, y, ItemWidth, ItemHeight, OffsetInventoryItems, 8, 8);

				if (slot != 0xFF)
				{
					return slot + INVENTORY_WEAR_SIZE;
				}
			}
		}
	}

	return -1;
}

bool CItemManager::CheckPickedItemOverlay(int MousePosX, int MousePosY, DWORD InterfaceOffset, int InterfaceWidth, int InterfaceHeight)
{
	ITEM_ATTRIBUTE* ItemInfo = (ITEM_ATTRIBUTE*)(ItemAttribute + pPickedItem->Type * sizeof(ITEM_ATTRIBUTE));

	int SlotX = (int)((MouseX - MousePosX) * 0.05f - ItemInfo->Width * 0.5f + 0.5f);

	int SlotY = (int)((MouseY - MousePosY) * 0.05f - ItemInfo->Height * 0.5f + 0.5f);

	BYTE slot = gItemManager.InterfaceRectCheck(SlotX, SlotY, ItemInfo->Width, ItemInfo->Height, InterfaceOffset, InterfaceWidth, InterfaceHeight);

	if (slot != 0xFF)
	{
		return true;
	}

	return false;
}

bool CItemManager::GetInterfaceEmptySlot(int MousePosX, int MousePosY, DWORD InterfaceOffset, int InterfaceWidth, int InterfaceHeight)
{
	ITEM_ATTRIBUTE* ItemInfo = (ITEM_ATTRIBUTE*)(ItemAttribute + pPickedItem->Type * sizeof(ITEM_ATTRIBUTE));

	ITEM* item = nullptr;

	for (int y = 0; y < InterfaceHeight; y++)
	{
		for (int x = 0; x < InterfaceWidth; x++)
		{
			item = GetItemFromInventory(InterfaceOffset, (x + (y * InterfaceWidth)));

			if (item->Type == -1)
			{
				BYTE slot = gItemManager.InterfaceRectCheck(x, y, ItemInfo->Width, ItemInfo->Height, InterfaceOffset, InterfaceWidth, InterfaceHeight);

				if (slot != 0xFF)
				{
					MouseX = (int)((ItemInfo->Width * 0.5f + (slot % InterfaceWidth) - 0.5f) * 20.0f + MousePosX);

					MouseY = (int)((ItemInfo->Height * 0.5f + (slot / InterfaceWidth) - 0.5f) * 20.0f + MousePosY);

					return true;
				}
			}
		}
	}

	return false;
}

BYTE CItemManager::InterfaceRectCheck(int x, int y, int width, int height, DWORD InterfaceOffset, int InterfaceWidth, int InterfaceHeight)
{
	if (((x + width) > InterfaceWidth || (y + height) > InterfaceHeight))
	{
		return 0xFF;
	}

	ITEM* item = nullptr;

	for (int sy = 0; sy < height; sy++)
	{
		for (int sx = 0; sx < width; sx++)
		{
			item = GetItemFromInventory(InterfaceOffset, (((sy + y) * InterfaceWidth) + (sx + x)));

			if (item->Type != -1)
			{
				return 0xFF;
			}
		}
	}

	return ((y * InterfaceWidth) + x);
}

int CItemManager::GetTargetSlotEquiped(short itemIndex, int slot)
{
	STRUCT_DECRYPT;

	WORD index = *(WORD*)(*(DWORD*)(CharacterMachine)+(536 + (68 * slot)));

	if (slot == 0 && index != 0xFFFF && *(WORD*)(*(DWORD*)(CharacterMachine)+(536 + (68 * 1))) == 0xFFFF)
	{
		if (!((itemIndex >= GET_ITEM(4, 8) && itemIndex <= GET_ITEM(4, 16)) || itemIndex == GET_ITEM(4, 18)) // Crossbows, Arrows
		    && !(itemIndex >= GET_ITEM(5, 0) && itemIndex < GET_ITEM(6, 0))) // Staffs
		{
			slot = 1;
		}
	}
	else if (slot == 10 && index != 0xFFFF && *(WORD*)(*(DWORD*)(CharacterMachine)+(536 + (68 * 11))) == 0xFFFF)
	{
		slot = 11;
	}

	STRUCT_ENCRYPT;

	return slot;
}