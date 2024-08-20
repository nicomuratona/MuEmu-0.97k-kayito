#include "stdafx.h"
#include "ItemManager.h"
#include "BloodCastle.h"
#include "ChaosBox.h"
#include "DSProtocol.h"
#include "EffectManager.h"
#include "Fruit.h"
#include "ItemBagManager.h"
#include "ItemMove.h"
#include "ItemStack.h"
#include "Log.h"
#include "Map.h"
#include "ReadScript.h"
#include "Message.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "Quest.h"
#include "QuestObjective.h"
#include "ServerInfo.h"
#include "ShopManager.h"
#include "SkillManager.h"
#include "Trade.h"
#include "Util.h"
#include "Viewport.h"
#include "Warehouse.h"

CItemManager gItemManager;

CItemManager::CItemManager()
{
	this->m_ItemInfo.clear();
}

CItemManager::~CItemManager()
{

}

void CItemManager::Load(char* path)
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

	this->m_ItemInfo.clear();

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

			int section = lpReadScript->GetNumber();

			while (true)
			{
				token = lpReadScript->GetToken();

				if (token == TOKEN_END || token == TOKEN_END_SECTION)
				{
					break;
				}

				ITEM_INFO info;

				memset(&info, 0, sizeof(info));

				info.Index = SafeGetItem(GET_ITEM(section, lpReadScript->GetNumber()));

				info.Slot = lpReadScript->GetAsNumber();

				info.Skill = lpReadScript->GetAsNumber();

				info.Width = lpReadScript->GetAsNumber();

				info.Height = lpReadScript->GetAsNumber();

				info.HaveSerial = lpReadScript->GetAsNumber();

				info.HaveOption = lpReadScript->GetAsNumber();

				info.DropItem = lpReadScript->GetAsNumber();

				strcpy_s(info.Name, lpReadScript->GetAsString());

				if (section >= 0 && section <= 5)
				{
					info.Level = lpReadScript->GetAsNumber();

					info.DamageMin = lpReadScript->GetAsNumber();

					info.DamageMax = lpReadScript->GetAsNumber();

					info.AttackSpeed = lpReadScript->GetAsNumber();

					info.Durability = lpReadScript->GetAsNumber();

					info.MagicDurability = lpReadScript->GetAsNumber();

					info.MagicDamageRate = lpReadScript->GetAsNumber();

					info.RequireLevel = lpReadScript->GetAsNumber();

					info.RequireStrength = lpReadScript->GetAsNumber();

					info.RequireDexterity = lpReadScript->GetAsNumber();

					info.RequireEnergy = lpReadScript->GetAsNumber();

					info.RequireVitality = lpReadScript->GetAsNumber();

					info.TwoHand = ((info.Width >= 2) ? 1 : 0);
				}
				else if (section >= 6 && section <= 11)
				{
					info.Level = lpReadScript->GetAsNumber();

					if (section == 6)
					{
						info.Defense = lpReadScript->GetAsNumber();

						info.DefenseSuccessRate = lpReadScript->GetAsNumber();
					}
					else if (section >= 7 && section <= 9)
					{
						info.Defense = lpReadScript->GetAsNumber();

						info.MagicDefense = lpReadScript->GetAsNumber();
					}
					else if (section == 10)
					{
						info.Defense = lpReadScript->GetAsNumber();

						info.AttackSpeed = lpReadScript->GetAsNumber();
					}
					else if (section == 11)
					{
						info.Defense = lpReadScript->GetAsNumber();

						info.WalkSpeed = lpReadScript->GetAsNumber();
					}

					info.Durability = lpReadScript->GetAsNumber();

					info.RequireLevel = lpReadScript->GetAsNumber();

					info.RequireStrength = lpReadScript->GetAsNumber();

					info.RequireDexterity = lpReadScript->GetAsNumber();

					info.RequireEnergy = lpReadScript->GetAsNumber();

					info.RequireVitality = lpReadScript->GetAsNumber();
				}
				else if (section == 12)
				{
					info.Level = lpReadScript->GetAsNumber();

					info.Defense = lpReadScript->GetAsNumber();

					info.Durability = lpReadScript->GetAsNumber();

					info.RequireLevel = lpReadScript->GetAsNumber();

					info.RequireEnergy = lpReadScript->GetAsNumber();

					info.RequireStrength = lpReadScript->GetAsNumber();

					info.RequireDexterity = lpReadScript->GetAsNumber();

					info.BuyMoney = lpReadScript->GetAsNumber();
				}
				else if (section == 13)
				{
					info.Level = lpReadScript->GetAsNumber();

					info.Durability = lpReadScript->GetAsNumber();

					for (int n = 0; n < MAX_RESISTANCE_TYPE; n++)
					{
						info.Resistance[n] = lpReadScript->GetAsNumber();
					}

					info.RequireLevel = info.Level;
				}
				else if (section == 14)
				{
					info.Value = lpReadScript->GetAsNumber();

					info.Level = lpReadScript->GetAsNumber();

					info.Durability = 1;
				}
				else if (section == 15)
				{
					info.Level = lpReadScript->GetAsNumber();

					info.RequireLevel = lpReadScript->GetAsNumber();

					info.RequireEnergy = lpReadScript->GetAsNumber();

					info.BuyMoney = lpReadScript->GetAsNumber();

					info.DamageMin = info.Level;

					info.DamageMax = info.Level + (info.Level / 2);
				}

				if (section <= 11 || section == 13)
				{
					lpReadScript->GetAsNumber();
				}

				if (section != 14)
				{
					for (int n = 0; n < MAX_CLASS; n++)
					{
						info.RequireClass[n] = lpReadScript->GetAsNumber();
					}
				}

				this->m_ItemInfo.insert(std::pair<int, ITEM_INFO>(info.Index, info));
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript->GetError());
	}

	delete lpReadScript;
}

bool CItemManager::GetInfo(int index, ITEM_INFO* lpInfo)
{
	std::map<int, ITEM_INFO>::iterator it = this->m_ItemInfo.find(index);

	if (it == this->m_ItemInfo.end())
	{
		return 0;
	}
	else
	{
		(*lpInfo) = it->second;

		return 1;
	}
}

int CItemManager::GetItemSkill(int index)
{
	ITEM_INFO ItemInfo;

	if (this->GetInfo(index, &ItemInfo) == 0)
	{
		return -1;
	}
	else
	{
		return ItemInfo.Skill;
	}
}

int CItemManager::GetItemTwoHand(int index)
{
	ITEM_INFO ItemInfo;

	if (this->GetInfo(index, &ItemInfo) == 0)
	{
		return -1;
	}
	else
	{
		return ItemInfo.TwoHand;
	}
}

int CItemManager::GetItemDurability(int index, int level, int ExceOption)
{
	ITEM_INFO ItemInfo;

	if (this->GetInfo(index, &ItemInfo) == 0)
	{
		return 0;
	}

	if (index == GET_ITEM(14, 21)) // Rena
	{
		return 1;
	}

	if (index == GET_ITEM(13, 18) || index == GET_ITEM(14, 19)) // Invisibility Cloak, Devil's Invitation
	{
		return 1;
	}

	int ItemDurability = ItemInfo.Durability;

	if (index >= GET_ITEM(5, 0) && index < GET_ITEM(6, 0)) // Staffs
	{
		ItemDurability = ItemInfo.MagicDurability;
	}

	int dur = 0;

	if (level >= 5)
	{
		if (level == 10)
		{
			dur = ItemDurability + ((level * 2) - 3);
		}
		else if (level == 11)
		{
			dur = ItemDurability + ((level * 2) - 1);
		}
		else
		{
			dur = ItemDurability + ((level * 2) - 4);
		}
	}
	else
	{
		dur = ItemDurability + level;
	}

	if (index != GET_ITEM(0, 19) && index != GET_ITEM(2, 13) && index != GET_ITEM(4, 18) && index != GET_ITEM(5, 10) && ItemInfo.Slot != 7) // Sword of Archangel,Scepter of Archangel,Crossbow of Archangel,Staff of Archangel
	{
		if (ExceOption != 0)
		{
			dur += 15;
		}
	}

	return ((dur > 255) ? 255 : dur);
}

int CItemManager::GetItemRepairMoney(CItem* lpItem, int type)
{
	if (lpItem->IsItem() == 0)
	{
		return 0;
	}

	if (lpItem->m_Durability == lpItem->m_BaseDurability)
	{
		return 0;
	}

	ITEM_INFO ItemInfo;

	if (this->GetInfo(lpItem->m_Index, &ItemInfo) == 0)
	{
		return 0;
	}

	if (ItemInfo.Durability == 0)
	{
		return 0;
	}

	int RepairMoney = 0;

	RepairMoney = lpItem->m_BuyMoney / 3;

	RepairMoney = ((RepairMoney > MAX_MONEY) ? MAX_MONEY : RepairMoney);

	RepairMoney = ((RepairMoney >= 100) ? ((RepairMoney / 10) * 10) : RepairMoney);

	RepairMoney = ((RepairMoney >= 1000) ? ((RepairMoney / 100) * 100) : RepairMoney);

	float sq1 = sqrt((float)RepairMoney);

	float sq2 = sqrt(sq1);

	float value = ((((3.0f * sq1) * sq2) * (1.0f - (lpItem->m_Durability / lpItem->m_BaseDurability))) + 1.0f);

	if (lpItem->m_Durability <= 0)
	{
		value *= 1.4f;
	}

	int money = (int)((type == 1) ? (value * 2.5f) : value);

	money = ((money >= 100) ? ((money / 10) * 10) : money);

	money = ((money >= 1000) ? ((money / 100) * 100) : money);

	return money;
}

int CItemManager::GetInventoryItemSlot(LPOBJ lpObj, int index, int level)
{
	int MaxValue = this->GetInventoryMaxValue(lpObj);

	for (int n = 0; n < MaxValue; n++)
	{
		if (lpObj->Inventory[n].IsItem() != 0)
		{
			if (lpObj->Inventory[n].m_Index == index && (level == -1 || lpObj->Inventory[n].m_Level == level))
			{
				return n;
			}
		}
	}

	return -1;
}

int CItemManager::GetInventoryItemCount(LPOBJ lpObj, int index, int level)
{
	int count = 0;

	int MaxValue = this->GetInventoryMaxValue(lpObj);

	for (int n = 0; n < MaxValue; n++)
	{
		if (lpObj->Inventory[n].IsItem() != 0)
		{
			if (lpObj->Inventory[n].m_Index == index && (level == -1 || lpObj->Inventory[n].m_Level == level))
			{
				if (gItemStack.GetItemMaxStack(lpObj->Inventory[n].m_Index, lpObj->Inventory[n].m_Level) == 0)
				{
					count++;
				}
				else
				{
					count += (int)lpObj->Inventory[n].m_Durability;
				}
			}
		}
	}

	return count;
}

int CItemManager::GetInventoryEmptySlotCount(LPOBJ lpObj)
{
	int count = 0;

	int MaxValue = this->GetInventoryMaxValue(lpObj) - INVENTORY_WEAR_SIZE;

	for (int n = 0; n < MaxValue; n++)
	{
		if (lpObj->InventoryMap[n] == 0xFF)
		{
			count++;
		}
	}

	return count;
}

int CItemManager::GetInventoryMaxValue(LPOBJ lpObj)
{
	return INVENTORY_SIZE;
}

bool CItemManager::CheckItemRequireLevel(LPOBJ lpObj, CItem* lpItem)
{
	if (lpObj->Level >= lpItem->m_RequireLevel)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CItemManager::CheckItemRequireStrength(LPOBJ lpObj, CItem* lpItem)
{
	if (lpObj->Strength >= lpItem->m_RequireStrength)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CItemManager::CheckItemRequireDexterity(LPOBJ lpObj, CItem* lpItem)
{
	if (lpObj->Dexterity >= lpItem->m_RequireDexterity)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CItemManager::CheckItemRequireVitality(LPOBJ lpObj, CItem* lpItem)
{
	if (lpObj->Vitality >= lpItem->m_RequireVitality)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CItemManager::CheckItemRequireEnergy(LPOBJ lpObj, CItem* lpItem)
{
	if (lpObj->Energy >= lpItem->m_RequireEnergy)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CItemManager::CheckItemRequireClass(LPOBJ lpObj, int index)
{
	ITEM_INFO ItemInfo;

	if (this->GetInfo(index, &ItemInfo) == 0)
	{
		return 0;
	}

	if (CHECK_RANGE(lpObj->Class, MAX_CLASS) == 0)
	{
		return 0;
	}

	if (ItemInfo.RequireClass[lpObj->Class] == 0)
	{
		return 0;
	}

	if ((lpObj->ChangeUp + 1) >= ItemInfo.RequireClass[lpObj->Class])
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CItemManager::CheckItemMoveToInventory(LPOBJ lpObj, CItem* lpItem, int slot)
{
	if (lpItem->IsItem() == 0)
	{
		return 0;
	}

	if (INVENTORY_WEAR_RANGE(slot) == 0)
	{
		return 1;
	}

	if (this->CheckItemRequireLevel(lpObj, lpItem) == 0)
	{
		return 0;
	}

	if (this->CheckItemRequireStrength(lpObj, lpItem) == 0)
	{
		return 0;
	}

	if (this->CheckItemRequireDexterity(lpObj, lpItem) == 0)
	{
		return 0;
	}

	if (this->CheckItemRequireVitality(lpObj, lpItem) == 0)
	{
		return 0;
	}

	if (this->CheckItemRequireEnergy(lpObj, lpItem) == 0)
	{
		return 0;
	}

	if (this->CheckItemRequireClass(lpObj, lpItem->m_Index) == 0)
	{
		return 0;
	}

	if (lpItem->m_Slot != slot && slot != 0 && slot != 1 && slot != 10 && slot != 11)
	{
		return 0;
	}

	if (lpItem->m_Slot != slot && ((slot == 0 || slot == 1) && lpItem->m_Slot != 0 && lpItem->m_Slot != 1))
	{
		return 0;
	}

	if (lpItem->m_Slot != slot && ((slot == 10 || slot == 11) && lpItem->m_Slot != 10 && lpItem->m_Slot != 11))
	{
		return 0;
	}

	if (lpObj->Class == CLASS_DW) // DW
	{
		if (slot == 1 && !(lpItem->m_Index >= GET_ITEM(6, 0) && lpItem->m_Index < GET_ITEM(7, 0)))
		{
			return 0;
		}
	}

	if (lpObj->Class == CLASS_FE) // FE
	{
		if (slot == 0 && ((lpItem->m_Index >= GET_ITEM(4, 0) && lpItem->m_Index < GET_ITEM(4, 8)) || lpItem->m_Index == GET_ITEM(4, 17))) // BOWS
		{
			return 0;
		}

		if (slot == 1 && ((lpItem->m_Index >= GET_ITEM(4, 8) && lpItem->m_Index < GET_ITEM(4, 17)) || lpItem->m_Index == GET_ITEM(4, 18))) // CROSSBOWS
		{
			return 0;
		}

		if (lpItem->m_Index == GET_ITEM(4, 7) && lpObj->Inventory[1].IsItem() != 0) // Bolts when using Arrows
		{
			if (lpObj->Inventory[1].m_Index == GET_ITEM(4, 15))
			{
				return 0;
			}
		}

		if (lpItem->m_Index == GET_ITEM(4, 15) && lpObj->Inventory[0].IsItem() != 0) // Arrows when using Bolts
		{
			if (lpObj->Inventory[0].m_Index == GET_ITEM(4, 7))
			{
				return 0;
			}
		}
	}

	if (slot == 0 && lpObj->Inventory[1].IsItem() != 0)
	{
		if ((lpItem->m_TwoHand != 0 && lpObj->Inventory[1].m_Index != GET_ITEM(4, 7)) || (lpObj->Inventory[1].m_TwoHand != 0 && lpItem->m_Index != GET_ITEM(4, 15)))
		{
			return 0;
		}
	}

	if (slot == 1 && lpObj->Inventory[0].IsItem() != 0)
	{
		if ((lpItem->m_TwoHand != 0 && lpObj->Inventory[0].m_Index != GET_ITEM(4, 15)) || (lpObj->Inventory[0].m_TwoHand != 0 && lpItem->m_Index != GET_ITEM(4, 7)))
		{
			return 0;
		}
	}

	if (lpObj->Map == MAP_ATLANS)
	{
		if (lpItem->m_Index == GET_ITEM(13, 2) || lpItem->m_Index == GET_ITEM(13, 3)) // Uniria,Dinorant
		{
			return 0;
		}
	}

	if (lpObj->Map == MAP_ICARUS)
	{
		if (lpItem->m_Index == GET_ITEM(13, 2)) // Uniria
		{
			return 0;
		}
	}

	if (lpItem->m_Index == GET_ITEM(13, 10)) // Rings
	{
		if (lpObj->Inventory[slot ^ 1].IsItem() != 0 && lpObj->Inventory[slot ^ 1].m_Index == lpItem->m_Index)
		{
			return 0;
		}
	}

	return 1;
}

bool CItemManager::CheckItemMoveToTrade(LPOBJ lpObj, CItem* lpItem, BYTE TargetFlag)
{
	if (lpItem->IsItem() == 0)
	{
		return 0;
	}

	if (gItemMove.CheckItemMoveAllowTrade(lpItem->m_Index) == 0)
	{
		return 0;
	}

	if (this->CheckItemMoveToBlock(lpObj, lpItem) == 0)
	{
		return 0;
	}

	return 1;
}

bool CItemManager::CheckItemMoveToVault(LPOBJ lpObj, CItem* lpItem, BYTE TargetFlag)
{
	if (lpItem->IsItem() == 0)
	{
		return 0;
	}

	if (gItemMove.CheckItemMoveAllowVault(lpItem->m_Index) == 0)
	{
		return 0;
	}

	return 1;
}

bool CItemManager::CheckItemMoveToChaos(LPOBJ lpObj, CItem* lpItem, BYTE TargetFlag)
{
	if (lpItem->IsItem() == 0)
	{
		return 0;
	}

	if (gItemStack.GetItemMaxStack(lpItem->m_Index, lpItem->m_Level) != 0)
	{
		if (lpItem->m_Durability > 1)
		{
			return 0;
		}
	}

	return 1;
}

bool CItemManager::CheckItemMoveToBlock(LPOBJ lpObj, CItem* lpItem)
{
	if (gServerInfo.m_TradeItemBlock == 0 || lpObj->Authority == 32)
	{
		return 1;
	}

	if (gServerInfo.m_TradeItemBlockExc != 0 && lpItem->IsExcItem() != 0 && GetExceOptionCount(lpItem->m_ExceOption) >= gServerInfo.m_TradeItemBlockExc)
	{
		return 0;
	}

	return 1;
}

bool CItemManager::CheckItemInventorySpace(LPOBJ lpObj, int index)
{
	ITEM_INFO ItemInfo;

	if (this->GetInfo(index, &ItemInfo) == 0)
	{
		return 0;
	}

	int MaxY = (this->GetInventoryMaxValue(lpObj) - INVENTORY_WEAR_SIZE) / 8;

	for (int y = 0; y < MaxY; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (lpObj->InventoryMap[((y * 8) + x)] == 0xFF)
			{
				if (this->InventoryRectCheck(lpObj->Index, x, y, ItemInfo.Width, ItemInfo.Height) != 0xFF)
				{
					return 1;
				}
			}
		}
	}

	return 0;
}

bool CItemManager::CheckItemInventorySpace(LPOBJ lpObj, int width, int height)
{
	int MaxY = (this->GetInventoryMaxValue(lpObj) - INVENTORY_WEAR_SIZE) / 8;

	for (int y = 0; y < MaxY; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (lpObj->InventoryMap[((y * 8) + x)] == 0xFF)
			{
				if (this->InventoryRectCheck(lpObj->Index, x, y, width, height) != 0xFF)
				{
					return 1;
				}
			}
		}
	}

	return 0;
}

void CItemManager::InventoryItemSet(int aIndex, int slot, BYTE type)
{
	if (INVENTORY_MAIN_RANGE(slot) == 0)
	{
		return;
	}

	ITEM_INFO ItemInfo;

	if (this->GetInfo(gObj[aIndex].Inventory[slot].m_Index, &ItemInfo) == 0)
	{
		return;
	}

	int x = (slot - INVENTORY_WEAR_SIZE) % 8;

	int y = (slot - INVENTORY_WEAR_SIZE) / 8;

	if (INVENTORY_MAIN_RANGE(slot) != false && ((x + ItemInfo.Width) > 8 || (y + ItemInfo.Height) > 8))
	{
		return;
	}

	for (int sy = 0; sy < ItemInfo.Height; sy++)
	{
		for (int sx = 0; sx < ItemInfo.Width; sx++)
		{
			gObj[aIndex].InventoryMap[(((sy + y) * 8) + (sx + x))] = type;
		}
	}
}

BYTE CItemManager::InventoryRectCheck(int aIndex, int x, int y, int width, int height)
{
	int slot = ((y * 8) + x) + INVENTORY_WEAR_SIZE;

	if (INVENTORY_MAIN_RANGE(slot) != 0 && ((x + width) > 8 || (y + height) > 8))
	{
		return 0xFF;
	}

	for (int sy = 0; sy < height; sy++)
	{
		for (int sx = 0; sx < width; sx++)
		{
			if (gObj[aIndex].InventoryMap[(((sy + y) * 8) + (sx + x))] != 0xFF)
			{
				return 0xFF;
			}
		}
	}

	return slot;
}

BYTE CItemManager::InventoryInsertItem(int aIndex, CItem item)
{
	ITEM_INFO ItemInfo;

	if (this->GetInfo(item.m_Index, &ItemInfo) == 0)
	{
		return 0xFF;
	}

	int MaxY = (this->GetInventoryMaxValue(&gObj[aIndex]) - INVENTORY_WEAR_SIZE) / 8;

	for (int y = 0; y < MaxY; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (gObj[aIndex].InventoryMap[((y * 8) + x)] == 0xFF)
			{
				BYTE slot = this->InventoryRectCheck(aIndex, x, y, ItemInfo.Width, ItemInfo.Height);

				if (slot != 0xFF)
				{
					gObj[aIndex].Inventory[slot] = item;

					this->InventoryItemSet(aIndex, slot, 1);

					return slot;
				}
			}
		}
	}

	return 0xFF;
}

BYTE CItemManager::InventoryAddItem(int aIndex, CItem item, int slot)
{
	if (INVENTORY_RANGE(slot) == 0)
	{
		return 0xFF;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Inventory[slot].IsItem() != 0 || item.IsItem() == 0)
	{
		return 0xFF;
	}

	if (INVENTORY_WEAR_RANGE(slot) == 0)
	{
		ITEM_INFO ItemInfo;

		if (this->GetInfo(item.m_Index, &ItemInfo) == 0)
		{
			return 0xFF;
		}

		int x = (slot - INVENTORY_WEAR_SIZE) % 8;

		int y = (slot - INVENTORY_WEAR_SIZE) / 8;

		if (this->InventoryRectCheck(aIndex, x, y, ItemInfo.Width, ItemInfo.Height) == 0xFF)
		{
			return 0xFF;
		}

		lpObj->Inventory[slot] = item;

		this->InventoryItemSet(aIndex, slot, 1);

		return slot;
	}
	else
	{
		if (this->CheckItemRequireClass(lpObj, item.m_Index) == 0)
		{
			return 0xFF;
		}

		lpObj->Inventory[slot] = item;

		if (lpObj->Inventory[slot].m_SkillOption != 0)
		{
			gSkillManager.AddSkillWeapon(lpObj, lpObj->Inventory[slot].GetItemSkill(), lpObj->Inventory[slot].m_Level);
		}

		return slot;
	}
}

void CItemManager::InventoryDelItem(int aIndex, int slot)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (INVENTORY_WEAR_RANGE(slot) == 0)
	{
		this->InventoryItemSet(aIndex, slot, 0xFF);

		lpObj->Inventory[slot].Clear();
	}
	else
	{
		if (lpObj->Inventory[slot].m_SkillOption != 0)
		{
			gSkillManager.DelSkillWeapon(lpObj, lpObj->Inventory[slot].GetItemSkill(), lpObj->Inventory[slot].m_Level);
		}

		lpObj->Inventory[slot].Clear();
	}
}

bool CItemManager::InventoryInsertItemStack(LPOBJ lpObj, CItem* lpItem)
{
	int MaxStack, CreateItemIndex;

	if (lpItem->IsItem() == 0)
	{
		return 0;
	}

	if ((MaxStack = gItemStack.GetItemMaxStack(lpItem->m_Index, lpItem->m_Level)) <= 0)
	{
		return 0;
	}

	int MaxValue = this->GetInventoryMaxValue(lpObj);

	for (int n = INVENTORY_WEAR_SIZE; n < MaxValue; n++)
	{
		if (lpObj->Inventory[n].IsItem() != 0)
		{
			if (lpObj->Inventory[n].m_Index == lpItem->m_Index && lpObj->Inventory[n].m_Level == lpItem->m_Level)
			{
				if (lpObj->Inventory[n].m_Durability < MaxStack)
				{
					int AddDur = (int)(MaxStack - lpObj->Inventory[n].m_Durability);

					AddDur = ((AddDur > lpItem->m_Durability) ? (int)lpItem->m_Durability : AddDur);

					lpItem->m_Durability -= AddDur;

					lpObj->Inventory[n].m_Durability += AddDur;

					if (lpObj->Inventory[n].m_Durability == MaxStack && (CreateItemIndex = gItemStack.GetCreateItemIndex(lpItem->m_Index)) != -1)
					{
						this->InventoryDelItem(lpObj->Index, n);

						this->GCItemDeleteSend(lpObj->Index, n, 1);

						GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, CreateItemIndex, (BYTE)lpItem->m_Level, 1, 0, 0, 0, lpObj->Index, 0);

						if (lpItem->m_Durability < 1)
						{
							return 1;
						}
					}
					else
					{
						this->GCItemDurSend(lpObj->Index, n, (BYTE)lpObj->Inventory[n].m_Durability, 0);

						if (lpItem->m_Durability < 1)
						{
							return 1;
						}
					}
				}
			}
		}
	}

	return 0;
}

bool CItemManager::InventoryAddItemStack(LPOBJ lpObj, int SourceSlot, int TargetSlot)
{
	int MaxStack, CreateItemIndex;

	if (lpObj->Inventory[SourceSlot].IsItem() == 0 || lpObj->Inventory[TargetSlot].IsItem() == 0)
	{
		return 0;
	}

	if (lpObj->Inventory[SourceSlot].m_Index != lpObj->Inventory[TargetSlot].m_Index || lpObj->Inventory[SourceSlot].m_Level != lpObj->Inventory[TargetSlot].m_Level)
	{
		return 0;
	}

	if ((MaxStack = gItemStack.GetItemMaxStack(lpObj->Inventory[SourceSlot].m_Index, lpObj->Inventory[SourceSlot].m_Level)) <= 0)
	{
		return 0;
	}

	if (lpObj->Inventory[TargetSlot].m_Durability >= MaxStack)
	{
		return 0;
	}

	int AddDur = (int)(MaxStack - lpObj->Inventory[TargetSlot].m_Durability);

	AddDur = ((AddDur > lpObj->Inventory[SourceSlot].m_Durability) ? (int)lpObj->Inventory[SourceSlot].m_Durability : AddDur);

	lpObj->Inventory[SourceSlot].m_Durability -= AddDur;

	lpObj->Inventory[TargetSlot].m_Durability += AddDur;

	if (lpObj->Inventory[TargetSlot].m_Durability == MaxStack && (CreateItemIndex = gItemStack.GetCreateItemIndex(lpObj->Inventory[SourceSlot].m_Index)) != -1)
	{
		this->InventoryDelItem(lpObj->Index, TargetSlot);

		this->GCItemDeleteSend(lpObj->Index, TargetSlot, 1);

		GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, CreateItemIndex, (BYTE)lpObj->Inventory[SourceSlot].m_Level, 0, 0, 0, 0, lpObj->Index, 0);
	}
	else
	{
		this->GCItemDurSend(lpObj->Index, TargetSlot, (BYTE)lpObj->Inventory[TargetSlot].m_Durability, 0);
	}

	if (lpObj->Inventory[SourceSlot].m_Durability < 1)
	{
		this->InventoryDelItem(lpObj->Index, SourceSlot);

		this->GCItemDeleteSend(lpObj->Index, SourceSlot, 1);

		this->GCItemDropSend(lpObj->Index, SourceSlot);
	}

	return 1;
}

void CItemManager::TradeItemSet(int aIndex, int slot, BYTE type)
{
	if (TRADE_RANGE(slot) == 0)
	{
		return;
	}

	ITEM_INFO ItemInfo;

	if (this->GetInfo(gObj[aIndex].Trade[slot].m_Index, &ItemInfo) == 0)
	{
		return;
	}

	int x = slot % 8;

	int y = slot / 8;

	if ((x + ItemInfo.Width) > 8 || (y + ItemInfo.Height) > 4)
	{
		return;
	}

	for (int sy = 0; sy < ItemInfo.Height; sy++)
	{
		for (int sx = 0; sx < ItemInfo.Width; sx++)
		{
			gObj[aIndex].TradeMap[(((sy + y) * 8) + (sx + x))] = type;
		}
	}
}

BYTE CItemManager::TradeRectCheck(int aIndex, int x, int y, int width, int height)
{
	if ((x + width) > 8 || (y + height) > 4)
	{
		return 0xFF;
	}

	for (int sy = 0; sy < height; sy++)
	{
		for (int sx = 0; sx < width; sx++)
		{
			if (gObj[aIndex].TradeMap[(((sy + y) * 8) + (sx + x))] != 0xFF)
			{
				return 0xFF;
			}
		}
	}

	return ((y * 8) + x);
}

BYTE CItemManager::TradeInsertItem(int aIndex, CItem item)
{
	ITEM_INFO ItemInfo;

	if (this->GetInfo(item.m_Index, &ItemInfo) == 0)
	{
		return 0xFF;
	}

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (gObj[aIndex].TradeMap[((y * 8) + x)] == 0xFF)
			{
				BYTE slot = this->TradeRectCheck(aIndex, x, y, ItemInfo.Width, ItemInfo.Height);

				if (slot != 0xFF)
				{
					gObj[aIndex].Trade[slot] = item;

					this->TradeItemSet(aIndex, slot, 1);

					return slot;
				}
			}
		}
	}

	return 0xFF;
}

BYTE CItemManager::TradeAddItem(int aIndex, CItem item, int slot)
{
	if (TRADE_RANGE(slot) == 0)
	{
		return 0xFF;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Trade[slot].IsItem() != 0 || item.IsItem() == 0)
	{
		return 0xFF;
	}

	ITEM_INFO ItemInfo;

	if (this->GetInfo(item.m_Index, &ItemInfo) == 0)
	{
		return 0xFF;
	}

	int x = slot % 8;

	int y = slot / 8;

	if (this->TradeRectCheck(aIndex, x, y, ItemInfo.Width, ItemInfo.Height) == 0xFF)
	{
		return 0xFF;
	}

	lpObj->Trade[slot] = item;

	this->TradeItemSet(aIndex, slot, 1);

	return slot;
}

void CItemManager::TradeDelItem(int aIndex, int slot)
{
	this->TradeItemSet(aIndex, slot, 0xFF);

	gObj[aIndex].Trade[slot].Clear();
}

void CItemManager::WarehouseItemSet(int aIndex, int slot, BYTE type)
{
	if (WAREHOUSE_RANGE(slot) == 0)
	{
		return;
	}

	ITEM_INFO ItemInfo;

	if (this->GetInfo(gObj[aIndex].Warehouse[slot].m_Index, &ItemInfo) == 0)
	{
		return;
	}

	int x = slot % 8;

	int y = slot / 8;

	if (WAREHOUSE_RANGE(slot) != false && ((x + ItemInfo.Width) > 8 || (y + ItemInfo.Height) > 15))
	{
		return;
	}

	if (WAREHOUSE_RANGE(slot) != false && ((x + ItemInfo.Width) > 8 || (y + ItemInfo.Height) > 30))
	{
		return;
	}

	for (int sy = 0; sy < ItemInfo.Height; sy++)
	{
		for (int sx = 0; sx < ItemInfo.Width; sx++)
		{
			gObj[aIndex].WarehouseMap[(((sy + y) * 8) + (sx + x))] = type;
		}
	}
}

BYTE CItemManager::WarehouseRectCheck(int aIndex, int x, int y, int width, int height)
{
	int slot = ((y * 8) + x);

	if (WAREHOUSE_RANGE(slot) != false && ((x + width) > 8 || (y + height) > 15))
	{
		return 0xFF;
	}

	if (WAREHOUSE_RANGE(slot) != false && ((x + width) > 8 || (y + height) > 30))
	{
		return 0xFF;
	}

	for (int sy = 0; sy < height; sy++)
	{
		for (int sx = 0; sx < width; sx++)
		{
			if (gObj[aIndex].WarehouseMap[(((sy + y) * 8) + (sx + x))] != 0xFF)
			{
				return 0xFF;
			}
		}
	}

	return slot;
}

BYTE CItemManager::WarehouseInsertItem(int aIndex, CItem item)
{
	ITEM_INFO ItemInfo;

	if (this->GetInfo(item.m_Index, &ItemInfo) == 0)
	{
		return 0xFF;
	}

	int MaxY = WAREHOUSE_SIZE / 8;

	for (int y = 0; y < MaxY; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (gObj[aIndex].WarehouseMap[((y * 8) + x)] == 0xFF)
			{
				BYTE slot = this->WarehouseRectCheck(aIndex, x, y, ItemInfo.Width, ItemInfo.Height);

				if (slot != 0xFF)
				{
					gObj[aIndex].Warehouse[slot] = item;

					this->WarehouseItemSet(aIndex, slot, 1);

					return slot;
				}
			}
		}
	}

	return 0xFF;
}

BYTE CItemManager::WarehouseAddItem(int aIndex, CItem item, int slot)
{
	if (WAREHOUSE_RANGE(slot) == 0)
	{
		return 0xFF;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Warehouse[slot].IsItem() != 0 || item.IsItem() == 0)
	{
		return 0xFF;
	}

	ITEM_INFO ItemInfo;

	if (this->GetInfo(item.m_Index, &ItemInfo) == 0)
	{
		return 0xFF;
	}

	int x = slot % 8;

	int y = slot / 8;

	if (this->WarehouseRectCheck(aIndex, x, y, ItemInfo.Width, ItemInfo.Height) == 0xFF)
	{
		return 0xFF;
	}

	lpObj->Warehouse[slot] = item;

	this->WarehouseItemSet(aIndex, slot, 1);

	return slot;
}

void CItemManager::WarehouseDelItem(int aIndex, int slot)
{
	this->WarehouseItemSet(aIndex, slot, 0xFF);

	gObj[aIndex].Warehouse[slot].Clear();
}

void CItemManager::ChaosBoxItemSet(int aIndex, int slot, BYTE type)
{
	if (CHAOS_BOX_RANGE(slot) == 0)
	{
		return;
	}

	ITEM_INFO ItemInfo;

	if (this->GetInfo(gObj[aIndex].ChaosBox[slot].m_Index, &ItemInfo) == 0)
	{
		return;
	}

	int x = slot % 8;

	int y = slot / 8;

	if ((x + ItemInfo.Width) > 8 || (y + ItemInfo.Height) > 4)
	{
		return;
	}

	for (int sy = 0; sy < ItemInfo.Height; sy++)
	{
		for (int sx = 0; sx < ItemInfo.Width; sx++)
		{
			gObj[aIndex].ChaosBoxMap[(((sy + y) * 8) + (sx + x))] = type;
		}
	}
}

BYTE CItemManager::ChaosBoxRectCheck(int aIndex, int x, int y, int width, int height)
{
	if ((x + width) > 8 || (y + height) > 4)
	{
		return 0xFF;
	}

	for (int sy = 0; sy < height; sy++)
	{
		for (int sx = 0; sx < width; sx++)
		{
			if (gObj[aIndex].ChaosBoxMap[(((sy + y) * 8) + (sx + x))] != 0xFF)
			{
				return 0xFF;
			}
		}
	}

	return ((y * 8) + x);
}

BYTE CItemManager::ChaosBoxInsertItem(int aIndex, CItem item)
{
	ITEM_INFO ItemInfo;

	if (this->GetInfo(item.m_Index, &ItemInfo) == 0)
	{
		return 0xFF;
	}

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (gObj[aIndex].ChaosBoxMap[((y * 8) + x)] == 0xFF)
			{
				BYTE slot = this->ChaosBoxRectCheck(aIndex, x, y, ItemInfo.Width, ItemInfo.Height);

				if (slot != 0xFF)
				{
					gObj[aIndex].ChaosBox[slot] = item;

					this->ChaosBoxItemSet(aIndex, slot, 1);

					return slot;
				}
			}
		}
	}

	return 0xFF;
}

BYTE CItemManager::ChaosBoxAddItem(int aIndex, CItem item, int slot)
{
	if (CHAOS_BOX_RANGE(slot) == 0)
	{
		return 0xFF;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->ChaosBox[slot].IsItem() != 0 || item.IsItem() == 0)
	{
		return 0xFF;
	}

	ITEM_INFO ItemInfo;

	if (this->GetInfo(item.m_Index, &ItemInfo) == 0)
	{
		return 0xFF;
	}

	int x = slot % 8;

	int y = slot / 8;

	if (this->ChaosBoxRectCheck(aIndex, x, y, ItemInfo.Width, ItemInfo.Height) == 0xFF)
	{
		return 0xFF;
	}

	lpObj->ChaosBox[slot] = item;

	this->ChaosBoxItemSet(aIndex, slot, 1);

	return slot;
}

void CItemManager::ChaosBoxDelItem(int aIndex, int slot)
{
	this->ChaosBoxItemSet(aIndex, slot, 0xFF);

	gObj[aIndex].ChaosBox[slot].Clear();
}

void CItemManager::ItemByteConvert(BYTE* lpMsg, CItem item)
{
	lpMsg[0] = item.m_Index & 0xFF;

	lpMsg[1] = 0;
	lpMsg[1] |= item.m_Level * 8;
	lpMsg[1] |= item.m_SkillOption * 128;
	lpMsg[1] |= item.m_LuckOption * 4;
	lpMsg[1] |= item.m_AddOption & 3;

	lpMsg[2] = (BYTE)item.m_Durability;

	lpMsg[3] = 0;
	lpMsg[3] |= (item.m_Index & 256) >> 1;
	lpMsg[3] |= ((item.m_AddOption > 3) ? 64 : 0);
	lpMsg[3] |= item.m_ExceOption;
}

void CItemManager::DBItemByteConvert(BYTE* lpMsg, CItem* lpItem)
{
	if (lpItem->IsItem() == 0)
	{
		memset(lpMsg, 0xFF, MAX_ITEM_BYTE_SIZE);

		return;
	}

	if (lpItem->m_Index == GET_ITEM(13, 19))
	{
		memset(lpMsg, 0xFF, MAX_ITEM_BYTE_SIZE);

		return;
	}

	lpMsg[0] = lpItem->m_Index & 0xFF;

	lpMsg[1] = 0;
	lpMsg[1] |= lpItem->m_Level * 8;
	lpMsg[1] |= lpItem->m_SkillOption * 128;
	lpMsg[1] |= lpItem->m_LuckOption * 4;
	lpMsg[1] |= lpItem->m_AddOption & 3;

	lpMsg[2] = (BYTE)lpItem->m_Durability;

	lpMsg[3] = SET_NUMBERHB(SET_NUMBERHW(lpItem->m_Serial));
	lpMsg[4] = SET_NUMBERLB(SET_NUMBERHW(lpItem->m_Serial));
	lpMsg[5] = SET_NUMBERHB(SET_NUMBERLW(lpItem->m_Serial));
	lpMsg[6] = SET_NUMBERLB(SET_NUMBERLW(lpItem->m_Serial));

	lpMsg[7] = 0;
	lpMsg[7] |= (lpItem->m_Index & 256) >> 1;
	lpMsg[7] |= ((lpItem->m_AddOption > 3) ? 64 : 0);
	lpMsg[7] |= lpItem->m_ExceOption;

	lpMsg[8] = 0;

	lpMsg[9] = 0;
	lpMsg[9] |= (lpItem->m_Index & GET_ITEM(15, 0)) >> 5;
}

bool CItemManager::ConvertItemByte(CItem* lpItem, BYTE* lpMsg)
{
	if (lpMsg[0] == 0xFF && (lpMsg[7] & 0x80) == 0x80 && (lpMsg[9] & 0xF0) == 0xF0)
	{
		return 0;
	}

	lpItem->Clear();

	lpItem->m_Index = lpMsg[0] | ((lpMsg[9] & 0xF0) * 32) | ((lpMsg[7] & 0x80) * 2);

	lpItem->m_SkillOption = (lpMsg[1] / 128) & 1;

	lpItem->m_LuckOption = (lpMsg[1] / 4) & 1;

	lpItem->m_AddOption = (lpMsg[1] & 3) + ((lpMsg[7] & 64) / 16);

	lpItem->m_Level = (lpMsg[1] / 8) & 15;

	lpItem->m_Durability = lpMsg[2];

	int MaxStack = gItemStack.GetItemMaxStack(lpItem->m_Index, lpItem->m_Level);

	lpItem->m_Durability = ((MaxStack == 0) ? lpItem->m_Durability : ((lpItem->m_Durability > MaxStack) ? MaxStack : lpItem->m_Durability));

	lpItem->m_Serial = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg[3], lpMsg[4]), MAKE_NUMBERW(lpMsg[5], lpMsg[6]));

	lpItem->m_ExceOption = lpMsg[7] & 63;

	lpItem->Convert(lpItem->m_Index, lpItem->m_SkillOption, lpItem->m_LuckOption, lpItem->m_AddOption, lpItem->m_ExceOption);

	return 1;
}

bool CItemManager::IsValidItem(LPOBJ lpObj, CItem* lpItem)
{
	if (lpItem->IsItem() == 0)
	{
		return 0;
	}

	if (lpItem->m_Durability == 0)
	{
		return 0;
	}

	if (this->CheckItemRequireLevel(lpObj, lpItem) == 0)
	{
		return 0;
	}

	if (this->CheckItemRequireStrength(lpObj, lpItem) == 0)
	{
		return 0;
	}

	if (this->CheckItemRequireDexterity(lpObj, lpItem) == 0)
	{
		return 0;
	}

	if (this->CheckItemRequireVitality(lpObj, lpItem) == 0)
	{
		return 0;
	}

	if (this->CheckItemRequireEnergy(lpObj, lpItem) == 0)
	{
		return 0;
	}

	if (this->CheckItemRequireClass(lpObj, lpItem->m_Index) == 0)
	{
		return 0;
	}

	return 1;
}

void CItemManager::UpdateInventoryViewport(int aIndex, int slot)
{
	if (INVENTORY_WEAR_RANGE(slot) == 0)
	{
		return;
	}

	if (slot == 10 || slot == 11)
	{
		if (gSkillManager.SkillChangeUse(aIndex))
		{
			gObjViewportListProtocolCreate(&gObj[aIndex]);

			gObjectManager.CharacterUpdateMapEffect(&gObj[aIndex]);
		}
	}

	gObjectManager.CharacterMakePreviewCharSet(aIndex);

	this->GCItemChangeSend(aIndex, slot);
}

void CItemManager::DeleteInventoryItemCount(LPOBJ lpObj, int index, int level, int count)
{
	int MaxValue = this->GetInventoryMaxValue(lpObj);

	for (int n = 0; n < MaxValue, count > 0; n++)
	{
		if (lpObj->Inventory[n].IsItem() != 0)
		{
			if (lpObj->Inventory[n].m_Index == index && lpObj->Inventory[n].m_Level == level)
			{
				if (gItemStack.GetItemMaxStack(lpObj->Inventory[n].m_Index, lpObj->Inventory[n].m_Level) <= 0)
				{
					this->InventoryDelItem(lpObj->Index, n);

					this->GCItemDeleteSend(lpObj->Index, n, 1);

					count--;
				}
				else
				{
					if (lpObj->Inventory[n].m_Durability > count)
					{
						lpObj->Inventory[n].m_Durability -= count;

						this->GCItemDurSend(lpObj->Index, n, (BYTE)lpObj->Inventory[n].m_Durability, 0);

						count = 0;
					}
					else
					{
						count -= (int)lpObj->Inventory[n].m_Durability;

						this->InventoryDelItem(lpObj->Index, n);

						this->GCItemDeleteSend(lpObj->Index, n, 1);
					}
				}
			}
		}
	}
}

void CItemManager::DecreaseItemDur(LPOBJ lpObj, int slot, int dur)
{
	if (INVENTORY_RANGE(slot) == 0)
	{
		return;
	}

	if (lpObj->Inventory[slot].IsItem() == 0)
	{
		return;
	}

	if (lpObj->Inventory[slot].m_Durability > dur)
	{
		lpObj->Inventory[slot].m_Durability -= dur;

		this->GCItemDurSend(lpObj->Index, slot, (BYTE)lpObj->Inventory[slot].m_Durability, 0);
	}
	else
	{
		this->InventoryDelItem(lpObj->Index, slot);

		this->GCItemDeleteSend(lpObj->Index, slot, 1);
	}
}

bool CItemManager::RepairItem(LPOBJ lpObj, CItem* lpItem, int slot, int type, DWORD* money)
{
	if (lpItem->m_Slot == 0xFF || INVENTORY_WEAR_RANGE(lpItem->m_Slot) == 0)
	{
		return false;
	}

	if (lpItem->m_Slot == 8 && lpItem->m_Index != GET_ITEM(13, 4))
	{
		return false;
	}

	if (lpItem->m_Index == GET_ITEM(4, 7) || lpItem->m_Index == GET_ITEM(4, 15)) // Bolt,Arrow
	{
		return false;
	}

	if (lpItem->m_Index == GET_ITEM(13, 10)) // Rings
	{
		return false;
	}

	int cost = this->GetItemRepairMoney(lpItem, type);

	cost = ((cost < 0) ? 0 : cost);

	if (lpObj->Money < ((DWORD)cost))
	{
		return false;
	}

	lpObj->Money -= cost;

	lpItem->Convert(lpItem->m_Index, lpItem->m_SkillOption, lpItem->m_LuckOption, lpItem->m_AddOption, lpItem->m_ExceOption);

	lpItem->m_Durability = lpItem->m_BaseDurability;

	if (INVENTORY_WEAR_RANGE(slot) != false && lpItem->CheckDurabilityState() != 0)
	{
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
	}

	this->GCItemDurSend(lpObj->Index, slot, (BYTE)lpItem->m_Durability, 0);

	(*money) = lpObj->Money;

	if (lpObj->Money == 0)
	{
		GCMoneySend(lpObj->Index, lpObj->Money);
	}

	return true;
}

BYTE CItemManager::MoveItemToInventoryFromInventory(LPOBJ lpObj, BYTE SourceSlot, BYTE TargetSlot, BYTE TargetFlag)
{
	if (INVENTORY_RANGE(SourceSlot) == 0)
	{
		return 0xFF;
	}

	if (INVENTORY_RANGE(TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (SourceSlot == TargetSlot)
	{
		return 0xFF;
	}

	if (gObjInventorySearchSerialNumber(lpObj, lpObj->Inventory[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (gObjWarehouseSearchSerialNumber(lpObj, lpObj->Inventory[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (this->CheckItemMoveToInventory(lpObj, &lpObj->Inventory[SourceSlot], TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (this->InventoryAddItemStack(lpObj, SourceSlot, TargetSlot) != 0)
	{
		return 0xFF;
	}

	BYTE map[INVENTORY_SIZE];

	memcpy(map, lpObj->InventoryMap, INVENTORY_SIZE);

	this->InventoryItemSet(lpObj->Index, SourceSlot, 0xFF);

	if (this->InventoryAddItem(lpObj->Index, lpObj->Inventory[SourceSlot], TargetSlot) == 0xFF)
	{
		memcpy(lpObj->InventoryMap, map, INVENTORY_SIZE);

		return 0xFF;
	}

	this->InventoryDelItem(lpObj->Index, SourceSlot);

	this->InventoryItemSet(lpObj->Index, TargetSlot, 1);

	this->UpdateInventoryViewport(lpObj->Index, SourceSlot);

	this->UpdateInventoryViewport(lpObj->Index, TargetSlot);

	return TargetFlag;
}

BYTE CItemManager::MoveItemToInventoryFromTrade(LPOBJ lpObj, BYTE SourceSlot, BYTE TargetSlot, BYTE TargetFlag)
{
	if (TRADE_RANGE(SourceSlot) == 0)
	{
		return 0xFF;
	}

	if (INVENTORY_RANGE(TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (gObjInventorySearchSerialNumber(lpObj, lpObj->Trade[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (gObjWarehouseSearchSerialNumber(lpObj, lpObj->Trade[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (this->CheckItemMoveToInventory(lpObj, &lpObj->Trade[SourceSlot], TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (this->InventoryAddItem(lpObj->Index, lpObj->Trade[SourceSlot], TargetSlot) == 0xFF)
	{
		return 0xFF;
	}

	this->TradeDelItem(lpObj->Index, SourceSlot);

	if (OBJECT_RANGE(lpObj->TargetNumber) != false)
	{
		lpObj->TradeOk = 0;

		lpObj->TradeOkTime = GetTickCount();

		gTrade.GCTradeOkButtonSend(lpObj->Index, 0);

		gObj[lpObj->TargetNumber].TradeOk = 0;

		gObj[lpObj->TargetNumber].TradeOkTime = GetTickCount();

		gTrade.GCTradeOkButtonSend(lpObj->TargetNumber, 2);

		gTrade.GCTradeItemDelSend(lpObj->TargetNumber, SourceSlot);
	}

	this->UpdateInventoryViewport(lpObj->Index, TargetSlot);

	return TargetFlag;
}

BYTE CItemManager::MoveItemToInventoryFromWarehouse(LPOBJ lpObj, BYTE SourceSlot, BYTE TargetSlot, BYTE TargetFlag)
{
	if (WAREHOUSE_RANGE(SourceSlot) == 0)
	{
		return 0xFF;
	}

	if (INVENTORY_RANGE(TargetSlot) == 0)
	{
		return 0xFF;
	}

	int tax = gWarehouse.GetWarehouseTaxMoney(lpObj->Level, lpObj->WarehousePW);

	if (lpObj->Money < ((DWORD)tax) && lpObj->WarehouseMoney < tax)
	{
		return 0xFF;
	}

	if (gObjInventorySearchSerialNumber(lpObj, lpObj->Warehouse[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (gObjWarehouseSearchSerialNumber(lpObj, lpObj->Warehouse[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (this->CheckItemMoveToInventory(lpObj, &lpObj->Warehouse[SourceSlot], TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (this->InventoryAddItem(lpObj->Index, lpObj->Warehouse[SourceSlot], TargetSlot) == 0xFF)
	{
		return 0xFF;
	}

	this->WarehouseDelItem(lpObj->Index, SourceSlot);

	if (lpObj->Money >= ((DWORD)tax))
	{
		lpObj->Money -= tax;

		gWarehouse.GCWarehouseMoneySend(lpObj->Index, 1, lpObj->Money, lpObj->WarehouseMoney);
	}
	else if (lpObj->WarehouseMoney >= tax)
	{
		lpObj->WarehouseMoney -= tax;

		gWarehouse.GCWarehouseMoneySend(lpObj->Index, 1, lpObj->Money, lpObj->WarehouseMoney);
	}

	this->UpdateInventoryViewport(lpObj->Index, TargetSlot);

	return TargetFlag;
}

BYTE CItemManager::MoveItemToInventoryFromChaosBox(LPOBJ lpObj, BYTE SourceSlot, BYTE TargetSlot, BYTE TargetFlag)
{
	if (CHAOS_BOX_RANGE(SourceSlot) == 0)
	{
		return 0xFF;
	}

	if (INVENTORY_RANGE(TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (gObjInventorySearchSerialNumber(lpObj, lpObj->ChaosBox[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (gObjWarehouseSearchSerialNumber(lpObj, lpObj->ChaosBox[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (this->CheckItemMoveToInventory(lpObj, &lpObj->ChaosBox[SourceSlot], TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (this->InventoryAddItem(lpObj->Index, lpObj->ChaosBox[SourceSlot], TargetSlot) == 0xFF)
	{
		return 0xFF;
	}

	this->ChaosBoxDelItem(lpObj->Index, SourceSlot);

	this->UpdateInventoryViewport(lpObj->Index, TargetSlot);

	return TargetFlag;
}

BYTE CItemManager::MoveItemToTradeFromInventory(LPOBJ lpObj, BYTE SourceSlot, BYTE TargetSlot, BYTE TargetFlag)
{
	if (INVENTORY_RANGE(SourceSlot) == 0)
	{
		return 0xFF;
	}

	if (TRADE_RANGE(TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (this->CheckItemMoveToTrade(lpObj, &lpObj->Inventory[SourceSlot], TargetFlag) == 0)
	{
		return 0xFF;
	}

	if (gObjInventorySearchSerialNumber(lpObj, lpObj->Inventory[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (gObjWarehouseSearchSerialNumber(lpObj, lpObj->Inventory[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (this->TradeAddItem(lpObj->Index, lpObj->Inventory[SourceSlot], TargetSlot) == 0xFF)
	{
		return 0xFF;
	}

	this->InventoryDelItem(lpObj->Index, SourceSlot);

	BYTE ItemInfo[MAX_ITEM_INFO];

	this->ItemByteConvert(ItemInfo, lpObj->Trade[TargetSlot]);

	gTrade.GCTradeItemAddSend(lpObj->TargetNumber, TargetSlot, ItemInfo);

	this->UpdateInventoryViewport(lpObj->Index, SourceSlot);

	return 1;
}

BYTE CItemManager::MoveItemToTradeFromTrade(LPOBJ lpObj, BYTE SourceSlot, BYTE TargetSlot, BYTE TargetFlag)
{
	if (TRADE_RANGE(SourceSlot) == 0)
	{
		return 0xFF;
	}

	if (TRADE_RANGE(TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (SourceSlot == TargetSlot)
	{
		return 0xFF;
	}

	if (gObjInventorySearchSerialNumber(lpObj, lpObj->Trade[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (gObjWarehouseSearchSerialNumber(lpObj, lpObj->Trade[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	BYTE map[TRADE_SIZE];

	memcpy(map, lpObj->TradeMap, TRADE_SIZE);

	this->TradeItemSet(lpObj->Index, SourceSlot, 0xFF);

	if (this->TradeAddItem(lpObj->Index, lpObj->Trade[SourceSlot], TargetSlot) == 0xFF)
	{
		memcpy(lpObj->TradeMap, map, TRADE_SIZE);

		return 0xFF;
	}

	this->TradeDelItem(lpObj->Index, SourceSlot);

	this->TradeItemSet(lpObj->Index, TargetSlot, 1);

	gTrade.GCTradeItemDelSend(lpObj->TargetNumber, SourceSlot);

	BYTE ItemInfo[MAX_ITEM_INFO];

	this->ItemByteConvert(ItemInfo, lpObj->Trade[TargetSlot]);

	gTrade.GCTradeItemAddSend(lpObj->TargetNumber, TargetSlot, ItemInfo);

	return 1;
}

BYTE CItemManager::MoveItemToWarehouseFromInventory(LPOBJ lpObj, BYTE SourceSlot, BYTE TargetSlot, BYTE TargetFlag)
{
	if (INVENTORY_RANGE(SourceSlot) == 0)
	{
		return 0xFF;
	}

	if (WAREHOUSE_RANGE(TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (this->CheckItemMoveToVault(lpObj, &lpObj->Inventory[SourceSlot], TargetFlag) == 0)
	{
		return 0xFF;
	}

	if (gObjInventorySearchSerialNumber(lpObj, lpObj->Inventory[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (gObjWarehouseSearchSerialNumber(lpObj, lpObj->Inventory[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (this->WarehouseAddItem(lpObj->Index, lpObj->Inventory[SourceSlot], TargetSlot) == 0xFF)
	{
		return 0xFF;
	}

	this->InventoryDelItem(lpObj->Index, SourceSlot);

	this->UpdateInventoryViewport(lpObj->Index, SourceSlot);

	return 2;
}

BYTE CItemManager::MoveItemToWarehouseFromWarehouse(LPOBJ lpObj, BYTE SourceSlot, BYTE TargetSlot, BYTE TargetFlag)
{
	if (WAREHOUSE_RANGE(SourceSlot) == 0)
	{
		return 0xFF;
	}

	if (WAREHOUSE_RANGE(TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (SourceSlot == TargetSlot)
	{
		return 0xFF;
	}

	if (gObjInventorySearchSerialNumber(lpObj, lpObj->Warehouse[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (gObjWarehouseSearchSerialNumber(lpObj, lpObj->Warehouse[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	BYTE map[WAREHOUSE_SIZE];

	memcpy(map, lpObj->WarehouseMap, WAREHOUSE_SIZE);

	this->WarehouseItemSet(lpObj->Index, SourceSlot, 0xFF);

	if (this->WarehouseAddItem(lpObj->Index, lpObj->Warehouse[SourceSlot], TargetSlot) == 0xFF)
	{
		memcpy(lpObj->WarehouseMap, map, WAREHOUSE_SIZE);

		return 0xFF;
	}

	this->WarehouseDelItem(lpObj->Index, SourceSlot);

	this->WarehouseItemSet(lpObj->Index, TargetSlot, 1);

	return 2;
}

BYTE CItemManager::MoveItemToChaosBoxFromInventory(LPOBJ lpObj, BYTE SourceSlot, BYTE TargetSlot, BYTE TargetFlag)
{
	if (INVENTORY_RANGE(SourceSlot) == 0)
	{
		return 0xFF;
	}

	if (CHAOS_BOX_RANGE(TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (this->CheckItemMoveToChaos(lpObj, &lpObj->Inventory[SourceSlot], TargetFlag) == 0)
	{
		return 0xFF;
	}

	if (gObjInventorySearchSerialNumber(lpObj, lpObj->Inventory[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (gObjWarehouseSearchSerialNumber(lpObj, lpObj->Inventory[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (this->ChaosBoxAddItem(lpObj->Index, lpObj->Inventory[SourceSlot], TargetSlot) == 0xFF)
	{
		return 0xFF;
	}

	this->InventoryDelItem(lpObj->Index, SourceSlot);

	this->UpdateInventoryViewport(lpObj->Index, SourceSlot);

	return TargetFlag;
}

BYTE CItemManager::MoveItemToChaosBoxFromChaosBox(LPOBJ lpObj, BYTE SourceSlot, BYTE TargetSlot, BYTE TargetFlag)
{
	if (CHAOS_BOX_RANGE(SourceSlot) == 0)
	{
		return 0xFF;
	}

	if (CHAOS_BOX_RANGE(TargetSlot) == 0)
	{
		return 0xFF;
	}

	if (SourceSlot == TargetSlot)
	{
		return 0xFF;
	}

	if (gObjInventorySearchSerialNumber(lpObj, lpObj->ChaosBox[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	if (gObjWarehouseSearchSerialNumber(lpObj, lpObj->ChaosBox[SourceSlot].m_Serial) == 0)
	{
		return 0xFF;
	}

	BYTE map[CHAOS_BOX_SIZE];

	memcpy(map, lpObj->ChaosBoxMap, CHAOS_BOX_SIZE);

	this->ChaosBoxItemSet(lpObj->Index, SourceSlot, 0xFF);

	if (this->ChaosBoxAddItem(lpObj->Index, lpObj->ChaosBox[SourceSlot], TargetSlot) == 0xFF)
	{
		memcpy(lpObj->ChaosBoxMap, map, CHAOS_BOX_SIZE);

		return 0xFF;
	}

	this->ChaosBoxDelItem(lpObj->Index, SourceSlot);

	this->ChaosBoxItemSet(lpObj->Index, TargetSlot, 1);

	return TargetFlag;
}

void CItemManager::CGItemGetRecv(PMSG_ITEM_GET_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_ITEM_GET_SEND pMsg;

	pMsg.header.setE(0x22, sizeof(pMsg));

	pMsg.result = 0xFF;

	memset(pMsg.ItemInfo, 0, sizeof(pMsg.ItemInfo));

	if (lpObj->DieRegen != 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (lpObj->Interface.use != 0 && lpObj->Interface.type != INTERFACE_SHOP)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	gObjFixInventoryPointer(aIndex);

	if (lpObj->Transaction == 1)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	int index = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]);

	if (MAP_ITEM_RANGE(index) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (MAP_RANGE(lpObj->Map) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (gMap[lpObj->Map].CheckItemGive(aIndex, index) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	CMapItem* lpItem = &gMap[lpObj->Map].m_Item[index];

	if (gQuestObjective.CanGetItem(lpObj, lpItem->m_Index, lpItem->m_Level) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (gQuestObjective.CheckQuestObjectiveItemCount(lpObj, lpItem->m_Index, lpItem->m_Level) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (lpItem->m_Index == GET_ITEM(14, 15)) // Money
	{
		gMap[lpObj->Map].ItemGive(aIndex, index);

		if (gObjCheckMaxMoney(aIndex, lpItem->m_BuyMoney) == 0)
		{
			lpObj->Money = MAX_MONEY;
		}
		else
		{
			lpObj->Money += lpItem->m_BuyMoney;
		}

		pMsg.result = 0xFE;

		pMsg.ItemInfo[0] = SET_NUMBERHB(SET_NUMBERHW(lpObj->Money));

		pMsg.ItemInfo[1] = SET_NUMBERLB(SET_NUMBERHW(lpObj->Money));

		pMsg.ItemInfo[2] = SET_NUMBERHB(SET_NUMBERLW(lpObj->Money));

		pMsg.ItemInfo[3] = SET_NUMBERLB(SET_NUMBERLW(lpObj->Money));

		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	CItem item = (*lpItem);

	if (this->InventoryInsertItemStack(lpObj, &item) != 0)
	{
		gMap[lpObj->Map].ItemGive(aIndex, index);

		pMsg.result = 0xFD;

		this->ItemByteConvert(pMsg.ItemInfo, item);

		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	pMsg.result = this->InventoryInsertItem(aIndex, item);

	if (pMsg.result == 0xFF)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	gMap[lpObj->Map].ItemGive(aIndex, index);

	this->ItemByteConvert(pMsg.ItemInfo, item);

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

	if (BC_MAP_RANGE(lpObj->Map) != false)
	{
		if (gBloodCastle.CheckEventItemSerial(lpObj->Map, lpItem) != false)
		{
			gBloodCastle.GetEventItem(lpObj->Map, aIndex, lpItem);
		}
	}
}

void CItemManager::CGItemDropRecv(PMSG_ITEM_DROP_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_ITEM_DROP_SEND pMsg;

	pMsg.header.set(0x23, sizeof(pMsg));

	pMsg.result = 0;

	pMsg.slot = lpMsg->slot;

	if (lpObj->DieRegen != 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (lpObj->Interface.use != 0 && lpObj->Interface.type != INTERFACE_SHOP)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	gObjFixInventoryPointer(aIndex);

	if (lpObj->Transaction == 1)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (INVENTORY_RANGE(lpMsg->slot) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	CItem* lpItem = &lpObj->Inventory[lpMsg->slot];

	if (lpItem->IsItem() == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (gItemMove.CheckItemMoveAllowDrop(lpItem->m_Index) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if ((lpItem->m_Index < GET_ITEM(12, 0) && lpItem->m_Level > 4) || lpItem->IsExcItem() != 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (gItemBagManager.DropItemByItemIndex(lpItem->m_Index, lpItem->m_Level, lpObj, lpObj->Map, lpMsg->x, lpMsg->y) != 0)
	{
		this->InventoryDelItem(aIndex, lpMsg->slot);
	}
	else
	{
		if (gMap[lpObj->Map].ItemDrop(lpItem->m_Index, lpItem->m_Level, lpItem->m_Durability, lpMsg->x, lpMsg->y, lpItem->m_SkillOption, lpItem->m_LuckOption, lpItem->m_AddOption, lpItem->m_ExceOption, lpItem->m_Serial, aIndex) == 0)
		{
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

			return;
		}

		this->InventoryDelItem(aIndex, lpMsg->slot);
	}

	pMsg.result = 1;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

	this->UpdateInventoryViewport(aIndex, lpMsg->slot);
}

void CItemManager::CGItemMoveRecv(PMSG_ITEM_MOVE_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_ITEM_MOVE_SEND pMsg;

	pMsg.header.setE(0x24, sizeof(pMsg));

	pMsg.result = 0xFF;

	pMsg.slot = lpMsg->TargetSlot;

	memset(pMsg.ItemInfo, 0xFF, sizeof(pMsg.ItemInfo));

	if (lpObj->DieRegen != 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (lpObj->Live == 0 || lpObj->RegenOk > 0 || lpObj->Teleport != 0 || lpObj->State != OBJECT_PLAYING)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	int index = lpMsg->ItemInfo[0] + ((lpMsg->ItemInfo[3] & 0x80) * 2);

	if (lpMsg->SourceFlag == 1 || lpMsg->TargetFlag == 1) // Trade
	{
		if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_TRADE || lpObj->Interface.state == 0 || OBJECT_RANGE(lpObj->TargetNumber) == 0)
		{
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

			return;
		}
	}

	if (lpMsg->SourceFlag == 2 || lpMsg->TargetFlag == 2) // Warehouse
	{
		if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_WAREHOUSE || lpObj->Interface.state == 0 || lpObj->LoadWarehouse == 0 || lpObj->WarehouseLock == 1)
		{
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

			return;
		}
	}

	if (lpMsg->SourceFlag == 3 || (lpMsg->SourceFlag >= 6 && lpMsg->SourceFlag <= 20) || lpMsg->TargetFlag == 3 || (lpMsg->TargetFlag >= 6 && lpMsg->TargetFlag <= 20)) // Chaos Box
	{
		if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_CHAOS_BOX || lpObj->ChaosLock != 0)
		{
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

			return;
		}
	}

	if (lpMsg->SourceFlag == 0 && lpMsg->TargetFlag == 0) // Inventory -> Inventory
	{
		if ((pMsg.result = this->MoveItemToInventoryFromInventory(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot, lpMsg->TargetFlag)) != 0xFF)
		{
			this->ItemByteConvert(pMsg.ItemInfo, lpObj->Inventory[lpMsg->TargetSlot]);
		}
	}
	else if (lpMsg->SourceFlag == 0 && lpMsg->TargetFlag == 1) // Inventory -> Trade
	{
		if ((pMsg.result = this->MoveItemToTradeFromInventory(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot, lpMsg->TargetFlag)) != 0xFF)
		{
			this->ItemByteConvert(pMsg.ItemInfo, lpObj->Trade[lpMsg->TargetSlot]);
		}
	}
	else if (lpMsg->SourceFlag == 0 && lpMsg->TargetFlag == 2) // Inventory -> Warehouse
	{
		if ((pMsg.result = this->MoveItemToWarehouseFromInventory(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot, lpMsg->TargetFlag)) != 0xFF)
		{
			this->ItemByteConvert(pMsg.ItemInfo, lpObj->Warehouse[lpMsg->TargetSlot]);
		}
	}
	else if (lpMsg->SourceFlag == 0 && (lpMsg->TargetFlag == 3 || (lpMsg->TargetFlag >= 5 && lpMsg->TargetFlag <= 20))) // Inventory -> Chaos Box
	{
		if ((pMsg.result = this->MoveItemToChaosBoxFromInventory(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot, lpMsg->TargetFlag)) != 0xFF)
		{
			this->ItemByteConvert(pMsg.ItemInfo, lpObj->ChaosBox[lpMsg->TargetSlot]);
		}
	}
	else if (lpMsg->SourceFlag == 1 && lpMsg->TargetFlag == 0) // Trade -> Inventory
	{
		if ((pMsg.result = this->MoveItemToInventoryFromTrade(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot, lpMsg->TargetFlag)) != 0xFF)
		{
			this->ItemByteConvert(pMsg.ItemInfo, lpObj->Inventory[lpMsg->TargetSlot]);
		}
	}
	else if (lpMsg->SourceFlag == 1 && lpMsg->TargetFlag == 1) // Trade -> Trade
	{
		if ((pMsg.result = this->MoveItemToTradeFromTrade(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot, lpMsg->TargetFlag)) != 0xFF)
		{
			this->ItemByteConvert(pMsg.ItemInfo, lpObj->Trade[lpMsg->TargetSlot]);
		}
	}
	else if (lpMsg->SourceFlag == 2 && lpMsg->TargetFlag == 0) // Warehouse -> Inventory
	{
		if ((pMsg.result = this->MoveItemToInventoryFromWarehouse(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot, lpMsg->TargetFlag)) != 0xFF)
		{
			this->ItemByteConvert(pMsg.ItemInfo, lpObj->Inventory[lpMsg->TargetSlot]);
		}
	}
	else if (lpMsg->SourceFlag == 2 && lpMsg->TargetFlag == 2) // Warehouse -> Warehouse
	{
		if ((pMsg.result = this->MoveItemToWarehouseFromWarehouse(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot, lpMsg->TargetFlag)) != 0xFF)
		{
			this->ItemByteConvert(pMsg.ItemInfo, lpObj->Warehouse[lpMsg->TargetSlot]);
		}
	}
	else if ((lpMsg->SourceFlag == 3 || (lpMsg->SourceFlag >= 5 && lpMsg->SourceFlag <= 20)) && lpMsg->TargetFlag == 0) // Chaos Box -> Inventory
	{
		if ((pMsg.result = this->MoveItemToInventoryFromChaosBox(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot, lpMsg->TargetFlag)) != 0xFF)
		{
			this->ItemByteConvert(pMsg.ItemInfo, lpObj->Inventory[lpMsg->TargetSlot]);
		}
	}
	else if ((lpMsg->SourceFlag == 3 || (lpMsg->SourceFlag >= 5 && lpMsg->SourceFlag <= 20)) && (lpMsg->TargetFlag == 3 || (lpMsg->TargetFlag >= 5 && lpMsg->TargetFlag <= 20))) // Chaos Box -> Chaos Box
	{
		if ((pMsg.result = this->MoveItemToChaosBoxFromChaosBox(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot, lpMsg->TargetFlag)) != 0xFF)
		{
			this->ItemByteConvert(pMsg.ItemInfo, lpObj->ChaosBox[lpMsg->TargetSlot]);
		}
	}

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

	if (pMsg.result == 0xFF && lpMsg->SourceFlag == 0 && lpMsg->TargetFlag == 0)
	{
		if (lpObj->Inventory[lpMsg->SourceSlot].IsItem() != 0)
		{
			this->GCItemDurSend(lpObj->Index, lpMsg->SourceSlot, (BYTE)lpObj->Inventory[lpMsg->SourceSlot].m_Durability, 0);
		}
	}
}

void CItemManager::CGItemUseRecv(PMSG_ITEM_USE_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	GCLifeSend(aIndex, 0xFD, (BYTE)lpObj->Life);

	if (lpObj->DieRegen != 0)
	{
		return;
	}

	if (lpObj->Interface.use != 0 && lpObj->Interface.type != INTERFACE_SHOP)
	{
		return;
	}

	gObjFixInventoryPointer(aIndex);

	if (lpObj->Transaction == 1)
	{
		return;
	}

	if (INVENTORY_RANGE(lpMsg->SourceSlot) == 0)
	{
		return;
	}

	if (lpMsg->SourceSlot == lpMsg->TargetSlot)
	{
		return;
	}

	CItem* lpItem = &lpObj->Inventory[lpMsg->SourceSlot];

	if (lpItem->IsItem() == 0)
	{
		return;
	}

	if ((lpItem->m_Index >= GET_ITEM(12, 7) && lpItem->m_Index <= GET_ITEM(12, 19)) || lpItem->m_Index >= GET_ITEM(15, 0))
	{
		if (gObjectManager.CharacterUseScroll(lpObj, lpItem) != 0)
		{
			this->InventoryDelItem(aIndex, lpMsg->SourceSlot);

			this->GCItemDeleteSend(aIndex, lpMsg->SourceSlot, 1);
		}
	}
	else if (lpItem->m_Index == GET_ITEM(13, 15)) // Fruit
	{
		if (gFruit.UseFruit(lpObj, lpItem->m_Level) != 0)
		{
			this->InventoryDelItem(aIndex, lpMsg->SourceSlot);

			this->GCItemDeleteSend(aIndex, lpMsg->SourceSlot, 1);
		}
	}
	else if ((lpItem->m_Index >= GET_ITEM(14, 0) && lpItem->m_Index <= GET_ITEM(14, 6)))
	{
		if (gObjectManager.CharacterUsePotion(lpObj, lpItem) != 0)
		{
			this->DecreaseItemDur(lpObj, lpMsg->SourceSlot, 1);
		}
	}
	else if (lpItem->m_Index == GET_ITEM(14, 8)) // Antidote
	{
		gEffectManager.DelEffect(lpObj, EFFECT_POISON);

		gEffectManager.DelEffect(lpObj, EFFECT_ICE);

		this->DecreaseItemDur(lpObj, lpMsg->SourceSlot, 1);
	}
	else if (lpItem->m_Index == GET_ITEM(14, 9)) // Ale
	{
		gObjUseDrink(lpObj, lpItem->m_Level);

		this->InventoryDelItem(aIndex, lpMsg->SourceSlot);

		this->GCItemDeleteSend(aIndex, lpMsg->SourceSlot, 1);
	}
	else if (lpItem->m_Index == GET_ITEM(14, 10)) // Town Portal Scroll
	{
		if (gObjectManager.CharacterUsePortal(lpObj, lpItem) != 0)
		{
			this->InventoryDelItem(aIndex, lpMsg->SourceSlot);

			this->GCItemDeleteSend(aIndex, lpMsg->SourceSlot, 1);
		}
	}
	else if (lpItem->m_Index == GET_ITEM(14, 13)) // Jewel of Bless
	{
		if (gObjectManager.CharacterUseJewelOfBles(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot) != 0)
		{
			this->InventoryDelItem(aIndex, lpMsg->SourceSlot);

			this->GCItemDeleteSend(aIndex, lpMsg->SourceSlot, 1);

			this->GCItemModifySend(aIndex, lpMsg->TargetSlot);
		}
	}
	else if (lpItem->m_Index == GET_ITEM(14, 14)) // Jewel of Soul
	{
		if (gObjectManager.CharacterUseJewelOfSoul(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot) != 0)
		{
			this->InventoryDelItem(aIndex, lpMsg->SourceSlot);

			this->GCItemDeleteSend(aIndex, lpMsg->SourceSlot, 1);

			this->GCItemModifySend(aIndex, lpMsg->TargetSlot);
		}
	}
	else if (lpItem->m_Index == GET_ITEM(14, 16)) // Jewel of Life
	{
		if (gObjectManager.CharacterUseJewelOfLife(lpObj, lpMsg->SourceSlot, lpMsg->TargetSlot) != 0)
		{
			this->InventoryDelItem(aIndex, lpMsg->SourceSlot);

			this->GCItemDeleteSend(aIndex, lpMsg->SourceSlot, 1);

			this->GCItemModifySend(aIndex, lpMsg->TargetSlot);
		}
	}
	else if (lpItem->m_Index == GET_ITEM(14, 20)) // Remedy of Love
	{
		if (lpItem->m_Level == 0)
		{
			gObjUseDrink(lpObj, 2);

			this->InventoryDelItem(aIndex, lpMsg->SourceSlot);

			this->GCItemDeleteSend(aIndex, lpMsg->SourceSlot, 1);
		}
	}
	else
	{
		EFFECT_INFO* lpInfo = gEffectManager.GetInfoByItem(lpItem->m_Index);

		if (lpInfo != 0)
		{
			if (gEffectManager.AddEffect(lpObj, 0, lpInfo->Index, 0, 0, 0, 0, 0) != 0)
			{
				this->DecreaseItemDur(lpObj, lpMsg->SourceSlot, 1);
			}
		}
	}
}

void CItemManager::CGItemStackUnstackRecv(PMSG_ITEM_UNSTACK_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if (lpObj->DieRegen != 0)
	{
		return;
	}

	if (lpObj->Interface.use != 0)
	{
		return;
	}

	gObjFixInventoryPointer(aIndex);

	if (lpObj->Transaction == 1)
	{
		return;
	}

	if (INVENTORY_RANGE(lpMsg->SourceSlot) == 0)
	{
		return;
	}

	CItem* lpItem = &lpObj->Inventory[lpMsg->SourceSlot];

	if (lpItem->IsItem() == 0)
	{
		return;
	}

	if (lpItem->m_Durability <= lpMsg->Count)
	{
		return;
	}

	int MaxStack;

	if ((MaxStack = gItemStack.GetItemMaxStack(lpItem->m_Index, lpItem->m_Level)) == 0)
	{
		return;
	}

	if (MaxStack <= lpMsg->Count)
	{
		return;
	}

	CItem newItem;
	newItem.m_Level = lpItem->m_Level;
	newItem.m_Durability = 1;
	newItem.Convert(lpItem->m_Index, lpItem->m_SkillOption, lpItem->m_LuckOption, lpItem->m_AddOption, lpItem->m_ExceOption);

	BYTE map[INVENTORY_SIZE];

	int i;

	for (i = 0; i < lpMsg->Count; i++)
	{
		if (this->CheckItemInventorySpace(lpObj, lpItem->m_Index) == 0)
		{
			break;
		}

		memcpy(map, lpObj->InventoryMap, INVENTORY_SIZE);

		BYTE Slot = this->InventoryInsertItem(lpObj->Index, newItem);

		if (Slot == 0xFF)
		{
			memcpy(lpObj->InventoryMap, map, INVENTORY_SIZE);

			break;
		}

		this->UpdateInventoryViewport(lpObj->Index, Slot);
	}

	this->DecreaseItemDur(lpObj, lpMsg->SourceSlot, i);

	this->GCItemListSend(lpObj->Index);
}

void CItemManager::CGItemStackStackRecv(PMSG_ITEM_UNSTACK_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if (lpObj->DieRegen != 0)
	{
		return;
	}

	if (lpObj->Interface.use != 0)
	{
		return;
	}

	gObjFixInventoryPointer(aIndex);

	if (lpObj->Transaction == 1)
	{
		return;
	}

	if (INVENTORY_RANGE(lpMsg->SourceSlot) == 0)
	{
		return;
	}

	CItem* lpItem = &lpObj->Inventory[lpMsg->SourceSlot];

	if (lpItem->IsItem() == 0)
	{
		return;
	}

	int MaxStack;

	if ((MaxStack = gItemStack.GetItemMaxStack(lpItem->m_Index, lpItem->m_Level)) == 0)
	{
		return;
	}

	if (lpItem->m_Durability >= MaxStack)
	{
		return;
	}

	int MaxValue = this->GetInventoryMaxValue(lpObj);

	for (int n = INVENTORY_WEAR_SIZE; n < MaxValue; n++)
	{
		if (lpMsg->SourceSlot == n)
		{
			continue;
		}

		if (this->InventoryAddItemStack(lpObj, n, lpMsg->SourceSlot) != 0)
		{
			if (lpObj->Inventory[n].IsItem() != 0)
			{
				this->GCItemDurSend(lpObj->Index, n, (BYTE)lpObj->Inventory[n].m_Durability, 0);
			}
		}
	}
}

void CItemManager::CGItemBuyRecv(PMSG_ITEM_BUY_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_ITEM_BUY_SEND pMsg;

	pMsg.header.set(0x32, sizeof(pMsg));

	pMsg.result = 0xFF;

	if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_SHOP)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (SHOP_RANGE(lpObj->TargetShopNumber) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	gObjFixInventoryPointer(aIndex);

	if (lpObj->Transaction == 1)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	CItem item;

	if (gShopManager.GetItemByIndex(lpObj->TargetShopNumber, &item, lpMsg->slot) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (lpObj->Money < item.m_BuyMoney)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (this->InventoryInsertItemStack(lpObj, &item) != 0)
	{
		lpObj->Money -= item.m_BuyMoney;

		GCMoneySend(aIndex, lpObj->Money);

		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	pMsg.result = this->InventoryInsertItem(aIndex, item);

	if (pMsg.result == 0xFF)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	lpObj->Money -= item.m_BuyMoney;

	GCMoneySend(aIndex, lpObj->Money);

	this->ItemByteConvert(pMsg.ItemInfo, item);

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CItemManager::CGItemSellRecv(PMSG_ITEM_SELL_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_ITEM_SELL_SEND pMsg;

	pMsg.header.set(0x33, sizeof(pMsg));

	pMsg.result = 0;

	pMsg.money = 0;

	if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_SHOP)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (SHOP_RANGE(lpObj->TargetShopNumber) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (INVENTORY_RANGE(lpMsg->slot) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	gObjFixInventoryPointer(aIndex);

	if (lpObj->Transaction == 1)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	CItem* lpItem = &lpObj->Inventory[lpMsg->slot];

	if (lpItem->IsItem() == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (gItemMove.CheckItemMoveAllowSell(lpItem->m_Index) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (gServerInfo.m_TradeItemBlockSell != 0 && this->CheckItemMoveToBlock(lpObj, lpItem) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	lpItem->Value();

	if (gObjCheckMaxMoney(aIndex, lpItem->m_SellMoney) == 0)
	{
		if (gServerInfo.m_ShopItemBlockSellOnMaxMoney != 0)
		{
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

			return;
		}
		else
		{
			lpObj->Money = MAX_MONEY;
		}
	}
	else
	{
		lpObj->Money += lpItem->m_SellMoney;
	}

	this->InventoryDelItem(aIndex, lpMsg->slot);

	pMsg.result = 1;

	pMsg.money = lpObj->Money;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

	this->UpdateInventoryViewport(aIndex, lpMsg->slot);
}

void CItemManager::CGItemRepairRecv(PMSG_ITEM_REPAIR_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_ITEM_REPAIR_SEND pMsg;

	pMsg.header.set(0x34, sizeof(pMsg));

	pMsg.money = 0;

	if (lpMsg->type == 1 && lpObj->Level < 50)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (lpMsg->slot == 0xFF)
	{
		int MaxValue = this->GetInventoryMaxValue(lpObj);

		for (int n = 0; n < MaxValue; n++)
		{
			if (lpObj->Inventory[n].IsItem() == 0)
			{
				continue;
			}

			if (this->RepairItem(lpObj, &lpObj->Inventory[n], n, lpMsg->type, &pMsg.money))
			{
				DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
			}
		}

		gObjectManager.CharacterCalcAttribute(aIndex);

		return;
	}

	if (INVENTORY_RANGE(lpMsg->slot) == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (this->RepairItem(lpObj, &lpObj->Inventory[lpMsg->slot], lpMsg->slot, lpMsg->type, &pMsg.money))
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		gObjectManager.CharacterCalcAttribute(aIndex);
	}
}

void CItemManager::GCItemMoveSend(int aIndex, BYTE result, BYTE slot, BYTE* ItemInfo)
{
	PMSG_ITEM_MOVE_SEND pMsg;

	pMsg.header.setE(0x24, sizeof(pMsg));

	pMsg.result = result;

	pMsg.slot = slot;

	memcpy(pMsg.ItemInfo, ItemInfo, MAX_ITEM_INFO);

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CItemManager::GCItemChangeSend(int aIndex, BYTE slot)
{
	LPOBJ lpObj = &gObj[aIndex];

	PMSG_ITEM_CHANGE_SEND pMsg;

	pMsg.header.set(0x25, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	this->ItemByteConvert(pMsg.ItemInfo, lpObj->Inventory[slot]);

	pMsg.ItemInfo[1] = slot * 16;

	pMsg.ItemInfo[1] |= (LevelSmallConvert(lpObj->Inventory[slot].m_Level) & 0x0F);

	MsgSendV2(lpObj, (BYTE*)&pMsg, pMsg.header.size);
}

void CItemManager::GCItemDeleteSend(int aIndex, BYTE slot, BYTE flag)
{
	PMSG_ITEM_DELETE_SEND pMsg;

	pMsg.header.set(0x28, sizeof(pMsg));

	pMsg.slot = slot;

	pMsg.flag = flag;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CItemManager::GCItemDurSend(int aIndex, BYTE slot, BYTE dur, BYTE flag)
{
	PMSG_ITEM_DUR_SEND pMsg;

	pMsg.header.set(0x2A, sizeof(pMsg));

	pMsg.slot = slot;

	pMsg.dur = dur;

	pMsg.flag = flag;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CItemManager::GCItemListSend(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	BYTE send[4096];

	PMSG_ITEM_LIST_SEND pMsg;

	pMsg.header.setE(0xF3, 0x10, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_ITEM_LIST info;

	for (int n = 0; n < INVENTORY_SIZE; n++)
	{
		if (lpObj->Inventory[n].IsItem() != 0)
		{
			if (lpObj->Inventory[n].m_ItemExist != 0)
			{
				info.slot = n;

				this->ItemByteConvert(info.ItemInfo, lpObj->Inventory[n]);

				memcpy(&send[size], &info, sizeof(info));

				size += sizeof(info);

				pMsg.count++;
			}
		}
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(aIndex, send, size);
}

void CItemManager::GCItemEquipmentSend(int aIndex)
{
	PMSG_ITEM_EQUIPMENT_SEND pMsg;

	pMsg.header.set(0xF3, 0x13, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	memcpy(pMsg.CharSet, gObj[aIndex].CharSet, sizeof(pMsg.CharSet));

	MsgSendV2(&gObj[aIndex], (BYTE*)&pMsg, pMsg.header.size);
}

void CItemManager::GCItemModifySend(int aIndex, BYTE slot)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Inventory[slot].IsItem() == 0)
	{
		return;
	}

	PMSG_ITEM_MODIFY_SEND pMsg;

	pMsg.header.set(0xF3, 0x14, sizeof(pMsg));

	pMsg.slot = slot;

	this->ItemByteConvert(pMsg.ItemInfo, lpObj->Inventory[slot]);

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CItemManager::GCItemDropSend(int aIndex, BYTE slot)
{
	PMSG_ITEM_DROP_SEND pMsg;

	pMsg.header.set(0x23, sizeof(pMsg));

	pMsg.result = 1;

	pMsg.slot = slot;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

	this->UpdateInventoryViewport(aIndex, slot);
}