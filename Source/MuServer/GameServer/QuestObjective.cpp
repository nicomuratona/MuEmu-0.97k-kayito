#include "stdafx.h"
#include "QuestObjective.h"
#include "DSProtocol.h"
#include "ItemManager.h"
#include "ReadScript.h"
#include "Monster.h"
#include "Notice.h"
#include "Party.h"
#include "Quest.h"
#include "Util.h"

CQuestObjective gQuestObjective;

CQuestObjective::CQuestObjective()
{
	this->m_count = 0;
}

CQuestObjective::~CQuestObjective()
{

}

void CQuestObjective::Load(char* path)
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

	this->m_count = 0;

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

			QUEST_OBJECTIVE_INFO info;

			memset(&info, 0, sizeof(info));

			info.Number = lpReadScript->GetNumber();

			info.Type = lpReadScript->GetAsNumber();

			info.Index = lpReadScript->GetAsNumber();

			info.Value = lpReadScript->GetAsNumber();

			info.Level = lpReadScript->GetAsNumber();

			info.SkillOption = lpReadScript->GetAsNumber();

			info.LuckOption = lpReadScript->GetAsNumber();

			info.AddOption = lpReadScript->GetAsNumber();

			info.ExceOption = lpReadScript->GetAsNumber();

			info.DropMinLevel = lpReadScript->GetAsNumber();

			info.DropMaxLevel = lpReadScript->GetAsNumber();

			info.ItemDropRate = lpReadScript->GetAsNumber();

			info.RequireIndex = lpReadScript->GetAsNumber();

			info.RequireState = lpReadScript->GetAsNumber();

			for (int n = 0; n < MAX_CLASS; n++)
			{
				info.RequireClass[n] = lpReadScript->GetAsNumber();
			}

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript->GetError());
	}

	delete lpReadScript;
}

void CQuestObjective::SetInfo(QUEST_OBJECTIVE_INFO info)
{
	if (this->m_count < 0 || this->m_count >= MAX_QUEST_OBJECTIVE)
	{
		return;
	}

	this->m_QuestObjectiveInfo[this->m_count++] = info;
}

QUEST_OBJECTIVE_INFO* CQuestObjective::GetInfo(int index)
{
	if (index < 0 || index >= this->m_count)
	{
		return 0;
	}

	return &this->m_QuestObjectiveInfo[index];
}

int CQuestObjective::GetQuestObjectiveCount(LPOBJ lpObj, QUEST_OBJECTIVE_INFO* lpInfo)
{
	if (lpInfo->Type == QUEST_OBJECTIVE_NONE)
	{
		return 0;
	}

	if (lpInfo->Type == QUEST_OBJECTIVE_ITEM)
	{
		return gItemManager.GetInventoryItemCount(lpObj, lpInfo->Index, lpInfo->Level);
	}

	if (lpInfo->Type == QUEST_OBJECTIVE_MONEY)
	{
		return lpObj->Money;
	}

	return 0;
}

bool CQuestObjective::CheckQuestObjectiveRequisite(LPOBJ lpObj, QUEST_OBJECTIVE_INFO* lpInfo)
{
	if (lpInfo->RequireIndex != -1 && gQuest.CheckQuestListState(lpObj, lpInfo->RequireIndex, lpInfo->RequireState) == 0)
	{
		return 0;
	}

	if (lpInfo->RequireClass[lpObj->Class] == 0 || lpInfo->RequireClass[lpObj->Class] > (lpObj->ChangeUp + 1))
	{
		return 0;
	}

	return 1;
}

bool CQuestObjective::CheckQuestObjective(LPOBJ lpObj, int QuestIndex)
{
	for (int n = 0; n < this->m_count; n++)
	{
		QUEST_OBJECTIVE_INFO* lpInfo = this->GetInfo(n);

		if (lpInfo == 0)
		{
			continue;
		}

		if (lpInfo->RequireIndex != QuestIndex)
		{
			continue;
		}

		if (this->CheckQuestObjectiveRequisite(lpObj, lpInfo) == 0)
		{
			continue;
		}

		if (this->GetQuestObjectiveCount(lpObj, lpInfo) < lpInfo->Value)
		{
			return 0;
		}
	}

	return 1;
}

bool CQuestObjective::CheckQuestObjectiveItemCount(LPOBJ lpObj, int ItemIndex, int ItemLevel)
{
	for (int n = 0; n < this->m_count; n++)
	{
		QUEST_OBJECTIVE_INFO* lpInfo = this->GetInfo(n);

		if (lpInfo == 0)
		{
			continue;
		}

		if (this->CheckQuestObjectiveRequisite(lpObj, lpInfo) == 0)
		{
			continue;
		}

		if (lpInfo->Type != QUEST_OBJECTIVE_ITEM)
		{
			continue;
		}

		if (lpInfo->Index == ItemIndex && lpInfo->Level == ItemLevel && lpInfo->Value <= this->GetQuestObjectiveCount(lpObj, lpInfo))
		{
			return 0;
		}
	}

	return 1;
}

bool CQuestObjective::CanGetItem(LPOBJ lpObj, int ItemIndex, int ItemLevel)
{
	for (int n = 0; n < this->m_count; n++)
	{
		QUEST_OBJECTIVE_INFO* lpInfo = this->GetInfo(n);

		if (lpInfo == 0)
		{
			continue;
		}

		if (lpInfo->Index == ItemIndex && lpInfo->Level == ItemLevel)
		{
			return this->CheckQuestObjectiveRequisite(lpObj, lpInfo);
		}
	}

	return 1;
}

void CQuestObjective::RemoveQuestObjective(LPOBJ lpObj, int QuestIndex)
{
	for (int n = 0; n < this->m_count; n++)
	{
		QUEST_OBJECTIVE_INFO* lpInfo = this->GetInfo(n);

		if (lpInfo == 0)
		{
			continue;
		}

		if (lpInfo->RequireIndex != QuestIndex)
		{
			continue;
		}

		if (this->CheckQuestObjectiveRequisite(lpObj, lpInfo) == 0)
		{
			continue;
		}

		if (lpInfo->Type == QUEST_OBJECTIVE_NONE)
		{
			continue;
		}

		if (lpInfo->Type == QUEST_OBJECTIVE_ITEM)
		{
			gItemManager.DeleteInventoryItemCount(lpObj, lpInfo->Index, lpInfo->Level, lpInfo->Value);

			continue;
		}

		if (lpInfo->Type == QUEST_OBJECTIVE_MONEY)
		{
			lpObj->Money = (((lpObj->Money - lpInfo->Value) < 0) ? 0 : (lpObj->Money - lpInfo->Value));

			GCMoneySend(lpObj->Index, lpObj->Money);

			continue;
		}
	}
}

bool CQuestObjective::MonsterItemDrop(LPOBJ lpMonster)
{
	int aIndex = gObjMonsterGetTopHitDamageUser(lpMonster);

	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	for (int n = 0; n < this->m_count; n++)
	{
		QUEST_OBJECTIVE_INFO* lpInfo = this->GetInfo(n);

		if (lpInfo == 0)
		{
			continue;
		}

		if (this->CheckQuestObjectiveRequisite(lpObj, lpInfo) == 0)
		{
			continue;
		}

		if (lpInfo->Type != QUEST_OBJECTIVE_ITEM)
		{
			continue;
		}

		if ((lpInfo->DropMinLevel == -1 && lpInfo->DropMaxLevel != lpMonster->Class) || (lpInfo->DropMinLevel != -1 && (lpInfo->DropMinLevel > lpMonster->Level || lpInfo->DropMaxLevel < lpMonster->Level)))
		{
			continue;
		}

		if (lpInfo->ItemDropRate > (GetLargeRand() % 10000) && lpInfo->Value > this->GetQuestObjectiveCount(lpObj, lpInfo))
		{
			GDCreateItemSend(aIndex, lpMonster->Map, (BYTE)lpMonster->X, (BYTE)lpMonster->Y, lpInfo->Index, lpInfo->Level, 0, lpInfo->SkillOption, lpInfo->LuckOption, lpInfo->AddOption, aIndex, lpInfo->ExceOption);

			return 1;
		}
	}

	return 0;
}