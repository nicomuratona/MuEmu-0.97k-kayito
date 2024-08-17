#include "stdafx.h"
#include "ItemDrop.h"
#include "BonusManager.h"
#include "DSProtocol.h"
#include "ItemManager.h"
#include "ItemOptionRate.h"
#include "ReadScript.h"
#include "Monster.h"
#include "RandomManager.h"
#include "Util.h"

CItemDrop gItemDrop;

CItemDrop::CItemDrop()
{
	this->m_ItemDropInfo.clear();
}

CItemDrop::~CItemDrop()
{

}

void CItemDrop::Load(char* path)
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

	this->m_ItemDropInfo.clear();

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

			ITEM_DROP_INFO info;

			info.Index = lpReadScript->GetNumber();

			info.Index = SafeGetItem(GET_ITEM(info.Index, lpReadScript->GetAsNumber()));

			info.Level = lpReadScript->GetAsNumber();

			info.Grade = lpReadScript->GetAsNumber();

			info.LevelOptionRate = lpReadScript->GetAsNumber();

			info.SkillOptionRate = lpReadScript->GetAsNumber();

			info.LuckOptionRate = lpReadScript->GetAsNumber();

			info.AddOptionRate = lpReadScript->GetAsNumber();

			info.ExceOptionRate = lpReadScript->GetAsNumber();

			info.MapNumber = lpReadScript->GetAsNumber();

			info.MonsterClass = lpReadScript->GetAsNumber();

			info.MonsterLevelMin = lpReadScript->GetAsNumber();

			info.MonsterLevelMax = lpReadScript->GetAsNumber();

			info.DropRate = lpReadScript->GetAsNumber();

			this->m_ItemDropInfo.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript->GetError());
	}

	delete lpReadScript;
}

int CItemDrop::DropItem(LPOBJ lpObj, LPOBJ lpTarget)
{
	CRandomManager RandomManager;

	for (std::vector<ITEM_DROP_INFO>::iterator it = this->m_ItemDropInfo.begin(); it != this->m_ItemDropInfo.end(); it++)
	{
		int DropRate;

		ITEM_INFO ItemInfo;

		if (gItemManager.GetInfo(it->Index, &ItemInfo) == 0)
		{
			continue;
		}

		if (it->MapNumber != -1 && it->MapNumber != lpObj->Map)
		{
			continue;
		}

		if (it->MonsterClass != -1 && it->MonsterClass != lpObj->Class)
		{
			continue;
		}

		if (it->MonsterLevelMin != -1 && it->MonsterLevelMin > lpObj->Level)
		{
			continue;
		}

		if (it->MonsterLevelMax != -1 && it->MonsterLevelMax < lpObj->Level)
		{
			continue;
		}

		if ((DropRate = it->DropRate) == -1 || (GetLargeRand() % 1000000) < (DropRate = this->GetItemDropRate(lpObj, lpTarget, it->Index, it->Level, it->DropRate)))
		{
			int rate = (1000000 / ((DropRate == -1) ? 1000000 : DropRate));

			RandomManager.AddElement((int)(&(*it)), rate);
		}
	}

	ITEM_DROP_INFO* lpItemDropInfo;

	if (RandomManager.GetRandomElement((int*)&lpItemDropInfo) == 0)
	{
		return 0;
	}
	else
	{
		WORD ItemIndex = lpItemDropInfo->Index;

		BYTE ItemLevel = lpItemDropInfo->Level;

		BYTE ItemSkillOption = 0;

		BYTE ItemLuckOption = 0;

		BYTE ItemAddOption = 0;

		BYTE ItemExceOption = 0;

		gItemOptionRate.GetItemLevelOption(lpItemDropInfo->LevelOptionRate, &ItemLevel);

		gItemOptionRate.GetItemSkillOption(lpItemDropInfo->SkillOptionRate, &ItemSkillOption);

		gItemOptionRate.GetItemLuckOption(lpItemDropInfo->LuckOptionRate, &ItemLuckOption);

		gItemOptionRate.GetItemAddOption(lpItemDropInfo->AddOptionRate, &ItemAddOption);

		gItemOptionRate.GetItemExceOption(lpItemDropInfo->ExceOptionRate, &ItemExceOption);

		gItemOptionRate.MakeExceOption(ItemIndex, ItemExceOption, &ItemExceOption);

		GDCreateItemSend(lpTarget->Index, lpObj->Map, (BYTE)lpObj->X, (BYTE)lpObj->Y, ItemIndex, ItemLevel, 0, ItemSkillOption, ItemLuckOption, ItemAddOption, lpTarget->Index, ((ItemExceOption == 0) ? lpItemDropInfo->Grade : ItemExceOption));

		return 1;
	}
}

int CItemDrop::GetItemDropRate(LPOBJ lpObj, LPOBJ lpTarget, int ItemIndex, int ItemLevel, int DropRate)
{
	return gBonusManager.GetBonusValue(lpTarget, BONUS_INDEX_CMN_ITEM_DROP_RATE, DropRate, ItemIndex, ItemLevel, lpObj->Class, lpObj->Level);
}