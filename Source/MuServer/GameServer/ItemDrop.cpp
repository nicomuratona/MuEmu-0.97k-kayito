#include "stdafx.h"
#include "ItemDrop.h"
#include "BonusManager.h"
#include "DSProtocol.h"
#include "ItemManager.h"
#include "ItemOptionRate.h"
#include "MemScript.h"
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
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (lpMemScript->SetBuffer(path) == false)
	{
		ErrorMessageBox(lpMemScript->GetLastError());

		delete lpMemScript;

		return;
	}

	this->m_ItemDropInfo.clear();

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if (strcmp("end", lpMemScript->GetString()) == 0)
			{
				break;
			}

			ITEM_DROP_INFO info;

			info.Index = SafeGetItem(GET_ITEM(lpMemScript->GetNumber(), lpMemScript->GetAsNumber()));

			info.Level = lpMemScript->GetAsNumber();

			info.Grade = lpMemScript->GetAsNumber();

			info.LevelOptionRate = lpMemScript->GetAsNumber();

			info.SkillOptionRate = lpMemScript->GetAsNumber();

			info.LuckOptionRate = lpMemScript->GetAsNumber();

			info.AddOptionRate = lpMemScript->GetAsNumber();

			info.ExceOptionRate = lpMemScript->GetAsNumber();

			info.MapNumber = lpMemScript->GetAsNumber();

			info.MonsterClass = lpMemScript->GetAsNumber();

			info.MonsterLevelMin = lpMemScript->GetAsNumber();

			info.MonsterLevelMax = lpMemScript->GetAsNumber();

			info.DropRate = lpMemScript->GetAsNumber();

			this->m_ItemDropInfo.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
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