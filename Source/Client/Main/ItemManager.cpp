#include "stdafx.h"
#include "ItemManager.h"
#include "CustomBow.h"
#include "CustomWing.h"

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

	BYTE slot = this->InterfaceRectCheck(SlotX, SlotY, ItemInfo->Width, ItemInfo->Height, InterfaceOffset, InterfaceWidth, InterfaceHeight);

	return (slot != 0xFF);
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

	ITEM* PlayerRightHand = &*(ITEM*)(CharacterMachine + (536 + (68 * EQUIPMENT_WEAPON_RIGHT)));

	ITEM* PlayerLeftHand = &*(ITEM*)(CharacterMachine + (536 + (68 * EQUIPMENT_WEAPON_LEFT)));

	ITEM* PlayerRightRing = &*(ITEM*)(CharacterMachine + (536 + (68 * EQUIPMENT_RING_RIGHT)));

	ITEM* PlayerLeftRing = &*(ITEM*)(CharacterMachine + (536 + (68 * EQUIPMENT_RING_LEFT)));

	BYTE PlayerClass = *(BYTE*)(CharacterAttribute + 0x0B) & 7;

	BYTE PlayerEvo = ((*(BYTE*)(Hero + 0x1BC) >> 3) & 0x1);

	WORD PlayerLevel = *(WORD*)(CharacterAttribute + 0x0E);

	WORD PlayerStrength = *(WORD*)(CharacterAttribute + 0x14);

	WORD PlayerDexterity = *(WORD*)(CharacterAttribute + 0x16);

	WORD PlayerEnergy = *(WORD*)(CharacterAttribute + 0x1A);

	if (slot == EQUIPMENT_WEAPON_RIGHT && PlayerRightHand->Type != -1 && PlayerLeftHand->Type == -1)
	{
		if (lpItem->Type >= GET_ITEM(0, 0) && lpItem->Type < GET_ITEM(4, 0)) // If its a sword, axe, mace or spear
		{
			slot = EQUIPMENT_WEAPON_LEFT;
		}
	}
	else if (slot == EQUIPMENT_RING_RIGHT && PlayerRightRing->Type != -1 && PlayerLeftRing->Type == -1)
	{
		slot = EQUIPMENT_RING_LEFT;
	}

	ITEM* TargetSlotItem = &*(ITEM*)(CharacterMachine + (536 + (68 * slot)));

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
		if (slot == EQUIPMENT_WEAPON_RIGHT && ((lpItem->Type >= GET_ITEM(4, 0) && lpItem->Type < GET_ITEM(4, 8)) || lpItem->Type == GET_ITEM(4, 17) || gCustomBow.IsCustomBow(lpItem->Type))) // BOWS
		{
			slot = -1;

			goto EXIT;
		}

		if (slot == EQUIPMENT_WEAPON_LEFT && ((lpItem->Type >= GET_ITEM(4, 8) && lpItem->Type < GET_ITEM(4, 17)) || lpItem->Type == GET_ITEM(4, 18) || gCustomBow.IsCustomCrossbow(lpItem->Type))) // CROSSBOWS
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

	if (World == MAP_ICARUS)
	{
		if (lpItem->Type == GET_ITEM(13, 2)) // Uniria
		{
			slot = -1;

			goto EXIT;
		}
	}

EXIT:

	STRUCT_ENCRYPT;

	return slot;
}

void CItemManager::GetItemName(int iType, int iLevel, char* Text)
{
	ITEM_ATTRIBUTE* ItemInfo = (ITEM_ATTRIBUTE*)(ItemAttribute + iType * sizeof(ITEM_ATTRIBUTE));

	// The name of the item.
	if (iType >= GET_ITEM(14, 23) && iType <= GET_ITEM(14, 26)) // Quest item.
	{
		wsprintf(Text, "%s", ItemInfo->Name);
	}
	else if (iType == GET_ITEM(14, 12)) // Event item
	{
		switch (iLevel)
		{
			case 0:
			{
				wsprintf(Text, "%s", GlobalText[100]);

				break;
			}

			case 1:
			{
				wsprintf(Text, "%s", GlobalText[101]);

				break;
			}

			case 2:
			{
				wsprintf(Text, "%s", ChaosEventName[ItemInfo->Durability]);

				break;
			}
		}
	}
	else if (iType == GET_ITEM(14, 11)) // Lucky Box/Christmas Star/Firecracker/Magic Pouch./Love Heart//Commendation
	{
		switch (iLevel)
		{
			case 0:
			{
				wsprintf(Text, "%s", ItemInfo->Name);

				break;
			}

			case 1:
			{
				wsprintf(Text, "%s", GlobalText[105]);

				break;
			}

			case 2:
			{
				wsprintf(Text, "%s", GlobalText[106]);

				break;
			}

			case 3:
			{
				wsprintf(Text, "%s", GlobalText[107]);

				break;
			}

			case 5:
			{
				wsprintf(Text, "%s", GlobalText[109]);

				break;
			}

			case 6:
			{
				wsprintf(Text, "%s", GlobalText[110]);

				break;
			}

			case 7:
			{
				wsprintf(Text, "%s", GlobalText[111]);

				break;
			}

			case 8:
			case 9:
			case 10:
			case 11:
			case 12: // sky box
			{
				wsprintf(Text, "%s +%d", GlobalText[115], iLevel - 7);

				break;
			}
		}
	}
	else if (iType == GET_ITEM(13, 15)) // circle. ( Energy / Stamina / Agility / Strength / Command )
	{
		switch (iLevel)
		{
			case 0:
			{
				wsprintf(Text, "%s %s", GlobalText[168], ItemInfo->Name);

				break;
			}

			case 1:
			{
				wsprintf(Text, "%s %s", GlobalText[169], ItemInfo->Name);

				break;
			}

			case 2:
			{
				wsprintf(Text, "%s %s", GlobalText[167], ItemInfo->Name);

				break;
			}

			case 3:
			{
				wsprintf(Text, "%s %s", GlobalText[166], ItemInfo->Name);

				break;
			}
		}
	}
	else if (iType == GET_ITEM(13, 14)) // Locke's Feather.
	{
		wsprintf(Text, "%s", ItemInfo->Name);
	}
	else if (iType == GET_ITEM(14, 21)) // Lena. (1:stone).
	{
		switch (iLevel)
		{
			case 0:
			{
				wsprintf(Text, "%s", ItemInfo->Name);

				break;
			}

			case 1: // stone.
			{
				wsprintf(Text, "%s", GlobalText[810]);

				break;
			}
		}
	}
	else if (iType == GET_ITEM(13, 19)) // Archangel's Absolute Weapon
	{
		switch (iLevel)
		{
			case 0:
			{
				wsprintf(Text, "%s", GlobalText[811]);

				break;
			}

			case 1:
			{
				wsprintf(Text, "%s", GlobalText[812]);

				break;
			}

			case 2:
			{
				wsprintf(Text, "%s", GlobalText[817]);

				break;
			}

			default:
			{
				wsprintf(Text, "%s", GlobalText[809]);

				break;
			}
		}
	}
	else if (iType == GET_ITEM(14, 9)) // wine, olives of love
	{
		switch (iLevel)
		{
			case 0:
			{
				wsprintf(Text, "%s", ItemInfo->Name);

				break;
			}

			case 1:
			{
				wsprintf(Text, "%s", GlobalText[108]);

				break;
			}
		}
	}
	/*
	else if (iType == GET_ITEM(12, 11)) // summon orb
	{
		wsprintf(Text, "%s %s", SkillAttribute[30 + iLevel].Name, GlobalText[102]);
	}
	*/
	else if (iType == GET_ITEM(13, 10)) // transformation ring
	{
		wsprintf(Text, "%s %s", MonsterScript[SommonTable[iLevel]].Name, GlobalText[103]);
	}
	else if ((iType >= GET_ITEM(12, 3) && iType <= GET_ITEM(12, 6)) // Wings of Spirit ~ Wings of Darkness.
		|| gCustomWing.GetInfoByIndex(iType) != NULL) // Custom Wings
	{
		if (iLevel == 0)
		{
			wsprintf(Text, "%s", ItemInfo->Name);
		}
		else
		{
			wsprintf(Text, "%s +%d", ItemInfo->Name, iLevel);
		}
	}
	else
	{
		if (iLevel == 0)
		{
			wsprintf(Text, "%s", ItemInfo->Name);
		}
		else
		{
			wsprintf(Text, "%s +%d", ItemInfo->Name, iLevel);
		}
	}
}