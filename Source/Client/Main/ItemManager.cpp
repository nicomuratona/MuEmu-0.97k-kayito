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

int CItemManager::GetTargetSlotEquiped(ITEM* lpItem, int slot)
{
	if (!lpItem)
	{
		return -1;
	}

	if (lpItem->Type == -1)
	{
		return -1;
	}

	if (INVENTORY_WEAR_RANGE(slot) == 0)
	{
		return -1;
	}

	STRUCT_DECRYPT;

	ITEM* PlayerRightHand = &*(ITEM*)(*(DWORD*)(CharacterMachine)+(536 + (68 * EQUIPMENT_WEAPON_RIGHT)));

	ITEM* PlayerLeftHand = &*(ITEM*)(*(DWORD*)(CharacterMachine)+(536 + (68 * EQUIPMENT_WEAPON_LEFT)));

	ITEM* PlayerRightRing = &*(ITEM*)(*(DWORD*)(CharacterMachine)+(536 + (68 * EQUIPMENT_RING_RIGHT)));

	ITEM* PlayerLeftRing = &*(ITEM*)(*(DWORD*)(CharacterMachine)+(536 + (68 * EQUIPMENT_RING_LEFT)));

	BYTE PlayerClass = *(BYTE*)(*(DWORD*)(CharacterAttribute)+0x0B) & 7;

	BYTE PlayerEvo = ((*(BYTE*)(Hero + 0x1BC) >> 3) & 0x1);

	WORD PlayerLevel = *(WORD*)(*(DWORD*)(CharacterAttribute)+0x0E);

	WORD PlayerStrength = *(WORD*)(*(DWORD*)(CharacterAttribute)+0x14);

	WORD PlayerDexterity = *(WORD*)(*(DWORD*)(CharacterAttribute)+0x16);

	WORD PlayerEnergy = *(WORD*)(*(DWORD*)(CharacterAttribute)+0x1A);

	if (slot == EQUIPMENT_WEAPON_RIGHT && PlayerRightHand->Type != -1 && PlayerLeftHand->Type == -1)
	{
		slot = EQUIPMENT_WEAPON_LEFT;
	}
	else if (slot == EQUIPMENT_RING_RIGHT && PlayerRightRing->Type != -1 && PlayerLeftRing->Type == -1)
	{
		slot = EQUIPMENT_RING_LEFT;
	}

	ITEM* TargetSlotItem = &*(ITEM*)(*(DWORD*)(CharacterMachine)+(536 + (68 * slot)));

	if (TargetSlotItem->Type != -1)
	{
		slot = -1;

		goto EXIT;
	}

	ITEM_ATTRIBUTE* ItemInfo = (ITEM_ATTRIBUTE*)(ItemAttribute + lpItem->Type * sizeof(ITEM_ATTRIBUTE));

	if (lpItem->RequireLevel > PlayerLevel)
	{
		slot = -1;

		goto EXIT;
	}

	if (lpItem->RequireStrength > PlayerStrength)
	{
		slot = -1;

		goto EXIT;
	}

	if (lpItem->RequireDexterity > PlayerDexterity)
	{
		slot = -1;

		goto EXIT;
	}

	if (lpItem->RequireEnergy > PlayerEnergy)
	{
		slot = -1;

		goto EXIT;
	}

	if (ItemInfo->RequireClass[PlayerClass] == 0 || (ItemInfo->RequireClass[PlayerClass] - 1) > PlayerEvo)
	{
		slot = -1;

		goto EXIT;
	}

	if (lpItem->Part != slot && slot != EQUIPMENT_WEAPON_RIGHT && slot != EQUIPMENT_WEAPON_LEFT && slot != EQUIPMENT_RING_RIGHT && slot != EQUIPMENT_RING_LEFT)
	{
		slot = -1;

		goto EXIT;
	}

	if (lpItem->Part != slot && ((slot == EQUIPMENT_WEAPON_RIGHT || slot == EQUIPMENT_WEAPON_LEFT) && lpItem->Part != EQUIPMENT_WEAPON_RIGHT && lpItem->Part != EQUIPMENT_WEAPON_LEFT))
	{
		slot = -1;

		goto EXIT;
	}

	if (lpItem->Part != slot && ((slot == EQUIPMENT_RING_RIGHT || slot == EQUIPMENT_RING_LEFT) && lpItem->Part != EQUIPMENT_RING_RIGHT && lpItem->Part != EQUIPMENT_RING_LEFT))
	{
		slot = -1;

		goto EXIT;
	}

	if (PlayerClass == 0) // DW
	{
		if (slot == EQUIPMENT_WEAPON_LEFT && !(lpItem->Type >= GET_ITEM(6, 0) && lpItem->Type < GET_ITEM(7, 0)))
		{
			slot = -1;

			goto EXIT;
		}
	}

	if (PlayerClass == 2) // FE
	{
		if (slot == EQUIPMENT_WEAPON_RIGHT && ((lpItem->Type >= GET_ITEM(4, 0) && lpItem->Type < GET_ITEM(4, 8)) || lpItem->Type == GET_ITEM(4, 17))) // BOWS
		{
			slot = -1;

			goto EXIT;
		}

		if (slot == EQUIPMENT_WEAPON_LEFT && ((lpItem->Type >= GET_ITEM(4, 8) && lpItem->Type < GET_ITEM(4, 17)) || lpItem->Type == GET_ITEM(4, 18))) // CROSSBOWS
		{
			slot = -1;

			goto EXIT;
		}

		if (lpItem->Type == GET_ITEM(4, 7) && PlayerRightHand->Type == GET_ITEM(4, 15)) // Bolts when using Arrows
		{
			slot = -1;

			goto EXIT;
		}

		if (lpItem->Type == GET_ITEM(4, 15) && PlayerLeftHand->Type == GET_ITEM(4, 7)) // Arrows when using Bolts
		{
			slot = -1;

			goto EXIT;
		}
	}

	if (World == MAP_ATLANS)
	{
		if (lpItem->Type == GET_ITEM(13, 2) || lpItem->Type == GET_ITEM(13, 3)) // Uniria,Dinorant
		{
			slot = -1;

			goto EXIT;
		}
	}

EXIT:

	STRUCT_ENCRYPT;

	return slot;
}