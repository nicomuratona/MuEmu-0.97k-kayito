#include "stdafx.h"
#include "ItemBagEx.h"
#include "DSProtocol.h"
#include "ItemOptionRate.h"
#include "Map.h"
#include "MemScript.h"
#include "Party.h"
#include "RandomManager.h"
#include "Util.h"

CItemBagEx::CItemBagEx()
{
	this->m_ItemBagInfo.clear();

	this->m_ItemBagItemInfo.clear();
}

CItemBagEx::~CItemBagEx()
{

}

void CItemBagEx::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == NULL)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (!lpMemScript->SetBuffer(path))
	{
		ErrorMessageBox(lpMemScript->GetLastError());

		delete lpMemScript;

		return;
	}

	this->m_ItemBagInfo.clear();

	this->m_ItemBagItemInfo.clear();

	try
	{
		eTokenResult token;

		while (true)
		{
			token = lpMemScript->GetToken();

			if (token == TOKEN_END || token == TOKEN_END_SECTION)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while (true)
			{
				token = lpMemScript->GetToken();

				if (token == TOKEN_END || token == TOKEN_END_SECTION)
				{
					break;
				}

				if (section == 2)
				{
					ITEM_BAG_EX_INFO info;

					info.Index = lpMemScript->GetNumber();

					info.DropRate = lpMemScript->GetAsNumber();

					this->m_ItemBagInfo.insert(std::pair<int, ITEM_BAG_EX_INFO>(info.Index, info));
				}
				else if (section == 3)
				{
					ITEM_BAG_EX_DROP_INFO info;

					info.Index = lpMemScript->GetNumber();

					info.Section = lpMemScript->GetAsNumber();

					info.SectionRate = lpMemScript->GetAsNumber();

					info.MoneyAmount = lpMemScript->GetAsNumber();

					info.OptionValue = lpMemScript->GetAsNumber();

					for (int n = 0; n < MAX_CLASS; n++)
					{
						info.RequireClass[n] = lpMemScript->GetAsNumber();
					}

					std::map<int, ITEM_BAG_EX_INFO>::iterator it = this->m_ItemBagInfo.find(info.Index);

					if (it != this->m_ItemBagInfo.end())
					{
						it->second.DropInfo.push_back(info);
					}
				}
				else if (section >= 4)
				{
					ITEM_BAG_EX_ITEM_INFO info;

					info.Index = lpMemScript->GetNumber();

					info.Index = SafeGetItem(GET_ITEM(info.Index, lpMemScript->GetAsNumber()));

					info.Level = lpMemScript->GetAsNumber();

					info.Grade = lpMemScript->GetAsNumber();

					info.LevelOptionRate = lpMemScript->GetAsNumber();

					info.SkillOptionRate = lpMemScript->GetAsNumber();

					info.LuckOptionRate = lpMemScript->GetAsNumber();

					info.AddOptionRate = lpMemScript->GetAsNumber();

					info.ExceOptionRate = lpMemScript->GetAsNumber();

					std::map<int, std::vector<ITEM_BAG_EX_ITEM_INFO>>::iterator it = this->m_ItemBagItemInfo.find(section);

					if (it == this->m_ItemBagItemInfo.end())
					{
						this->m_ItemBagItemInfo.insert(std::pair<int, std::vector<ITEM_BAG_EX_ITEM_INFO>>(section, std::vector<ITEM_BAG_EX_ITEM_INFO>(1, info)));
					}
					else
					{
						it->second.push_back(info);
					}
				}
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CItemBagEx::GetItem(LPOBJ lpObj, CItem* lpItem)
{
	for (std::map<int, ITEM_BAG_EX_INFO>::iterator it = this->m_ItemBagInfo.begin(); it != this->m_ItemBagInfo.end(); it++)
	{
		if ((GetLargeRand() % 10000) < it->second.DropRate)
		{
			CRandomManager RandomManager;

			for (std::vector<ITEM_BAG_EX_DROP_INFO>::iterator DropInfo = it->second.DropInfo.begin(); DropInfo != it->second.DropInfo.end(); DropInfo++)
			{
				if (this->CheckDropClass(lpObj, (DropInfo->OptionValue & 2), DropInfo->RequireClass) != 0)
				{
					RandomManager.AddElement((int)(&(*DropInfo)), DropInfo->SectionRate);
				}
			}

			ITEM_BAG_EX_DROP_INFO* lpItemBagDropInfo = 0;

			if (RandomManager.GetRandomElement((int*)&lpItemBagDropInfo) == 0)
			{
				return 0;
			}

			std::map<int, std::vector<ITEM_BAG_EX_ITEM_INFO>>::iterator ItemInfo = this->m_ItemBagItemInfo.find(lpItemBagDropInfo->Section);

			if (ItemInfo == this->m_ItemBagItemInfo.end())
			{
				return 0;
			}

			if (ItemInfo->second.empty() != 0)
			{
				return 0;
			}

			ITEM_BAG_EX_ITEM_INFO* lpItemBagItemInfo = &ItemInfo->second[GetLargeRand() % ItemInfo->second.size()];

			WORD ItemIndex = lpItemBagItemInfo->Index;

			BYTE ItemLevel = lpItemBagItemInfo->Level;

			BYTE ItemSkillOption = 0;

			BYTE ItemLuckOption = 0;

			BYTE ItemAddOption = 0;

			BYTE ItemExceOption = 0;

			gItemOptionRate.GetItemLevelOption(lpItemBagItemInfo->LevelOptionRate, &ItemLevel);

			gItemOptionRate.GetItemSkillOption(lpItemBagItemInfo->SkillOptionRate, &ItemSkillOption);

			gItemOptionRate.GetItemLuckOption(lpItemBagItemInfo->LuckOptionRate, &ItemLuckOption);

			gItemOptionRate.GetItemAddOption(lpItemBagItemInfo->AddOptionRate, &ItemAddOption);

			gItemOptionRate.GetItemExceOption(lpItemBagItemInfo->ExceOptionRate, &ItemExceOption);

			gItemOptionRate.MakeExceOption(ItemIndex, ItemExceOption, &ItemExceOption);

			lpItem->m_Level = ItemLevel;

			lpItem->Convert(ItemIndex, ItemSkillOption, ItemLuckOption, ItemAddOption, ((ItemExceOption == 0) ? lpItemBagItemInfo->Grade : ItemExceOption));

			return 1;
		}
	}

	return 0;
}

bool CItemBagEx::DropItem(LPOBJ lpObj, int map, int x, int y)
{
	for (std::map<int, ITEM_BAG_EX_INFO>::iterator it = this->m_ItemBagInfo.begin(); it != this->m_ItemBagInfo.end(); it++)
	{
		if ((GetLargeRand() % 10000) < it->second.DropRate)
		{
			CRandomManager RandomManager;

			for (std::vector<ITEM_BAG_EX_DROP_INFO>::iterator DropInfo = it->second.DropInfo.begin(); DropInfo != it->second.DropInfo.end(); DropInfo++)
			{
				if (this->CheckDropClass(lpObj, (DropInfo->OptionValue & 2), DropInfo->RequireClass) != 0)
				{
					RandomManager.AddElement((int)(&(*DropInfo)), DropInfo->SectionRate);
				}
			}

			ITEM_BAG_EX_DROP_INFO* lpItemBagDropInfo = 0;

			if (RandomManager.GetRandomElement((int*)&lpItemBagDropInfo) == 0)
			{
				continue;
			}

			int px = x;

			int py = y;

			if (it != this->m_ItemBagInfo.begin() || (gMap[map].CheckAttr(px, py, 4) != 0 || gMap[map].CheckAttr(px, py, 8) != 0))
			{
				if (this->GetRandomItemDropLocation(map, &px, &py, 2, 2, 10) == 0)
				{
					px = lpObj->X;

					py = lpObj->Y;
				}
			}

			std::map<int, std::vector<ITEM_BAG_EX_ITEM_INFO>>::iterator ItemInfo = this->m_ItemBagItemInfo.find(lpItemBagDropInfo->Section);

			if (ItemInfo == this->m_ItemBagItemInfo.end())
			{
				gMap[map].MoneyItemDrop(lpItemBagDropInfo->MoneyAmount, px, py);

				continue;
			}

			if (ItemInfo->second.empty() != 0)
			{
				gMap[map].MoneyItemDrop(lpItemBagDropInfo->MoneyAmount, px, py);

				continue;
			}

			ITEM_BAG_EX_ITEM_INFO* lpItemBagItemInfo = &ItemInfo->second[GetLargeRand() % ItemInfo->second.size()];

			WORD ItemIndex = lpItemBagItemInfo->Index;

			BYTE ItemLevel = lpItemBagItemInfo->Level;

			BYTE ItemSkillOption = 0;

			BYTE ItemLuckOption = 0;

			BYTE ItemAddOption = 0;

			BYTE ItemExceOption = 0;

			gItemOptionRate.GetItemLevelOption(lpItemBagItemInfo->LevelOptionRate, &ItemLevel);

			gItemOptionRate.GetItemSkillOption(lpItemBagItemInfo->SkillOptionRate, &ItemSkillOption);

			gItemOptionRate.GetItemLuckOption(lpItemBagItemInfo->LuckOptionRate, &ItemLuckOption);

			gItemOptionRate.GetItemAddOption(lpItemBagItemInfo->AddOptionRate, &ItemAddOption);

			gItemOptionRate.GetItemExceOption(lpItemBagItemInfo->ExceOptionRate, &ItemExceOption);

			gItemOptionRate.MakeExceOption(ItemIndex, ItemExceOption, &ItemExceOption);

			GDCreateItemSend(lpObj->Index, map, px, py, ItemIndex, ItemLevel, 0, ItemSkillOption, ItemLuckOption, ItemAddOption, lpObj->Index, ((ItemExceOption == 0) ? lpItemBagItemInfo->Grade : ItemExceOption));

			if ((lpItemBagDropInfo->OptionValue & 1) != 0)
			{
				GCFireworksSend(lpObj, lpObj->X, lpObj->Y);
			}
		}
	}

	return 1;
}

bool CItemBagEx::CheckDropClass(LPOBJ lpObj, int type, int RequireClass[MAX_CLASS])
{
	int aIndex = -1;

	if (type == 0 || OBJECT_RANGE(lpObj->PartyNumber) == false)
	{
		if (RequireClass[lpObj->Class] != 0 && RequireClass[lpObj->Class] <= (lpObj->ChangeUp + 1))
		{
			return 1;
		}
	}
	else
	{
		for (int n = 0; n < MAX_PARTY_USER; n++)
		{
			if (OBJECT_RANGE((aIndex = gParty.GetMemberIndex(lpObj->PartyNumber, n))) != false)
			{
				if (RequireClass[gObj[aIndex].Class] != 0 && RequireClass[gObj[aIndex].Class] <= (gObj[aIndex].ChangeUp + 1))
				{
					return 1;
				}
			}
		}
	}

	return 0;
}

bool::CItemBagEx::GetRandomItemDropLocation(int map, int* ox, int* oy, int tx, int ty, int count)
{
	int x = (*ox);

	int y = (*oy);

	tx = ((tx < 1) ? 1 : tx);

	ty = ((ty < 1) ? 1 : ty);

	for (int n = 0; n < count; n++)
	{
		(*ox) = ((GetLargeRand() % (tx + 1)) * ((GetLargeRand() % 2 == 0) ? -1 : 1)) + x;

		(*oy) = ((GetLargeRand() % (ty + 1)) * ((GetLargeRand() % 2 == 0) ? -1 : 1)) + y;

		if (gMap[map].CheckAttr((*ox), (*oy), 4) == 0 && gMap[map].CheckAttr((*ox), (*oy), 8) == 0)
		{
			return 1;
		}
	}

	return 0;
}