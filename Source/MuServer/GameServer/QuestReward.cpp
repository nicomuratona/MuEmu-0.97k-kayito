#include "stdafx.h"
#include "QuestReward.h"
#include "MemScript.h"
#include "ObjectManager.h"
#include "Quest.h"
#include "ServerInfo.h"
#include "SkillManager.h"
#include "Util.h"

CQuestReward gQuestReward;

CQuestReward::CQuestReward()
{
	this->m_count = 0;
}

CQuestReward::~CQuestReward()
{

}

void CQuestReward::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());

		delete lpMemScript;

		return;
	}

	this->m_count = 0;

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

			QUEST_REWARD_INFO info;

			memset(&info, 0, sizeof(info));

			info.Number = lpMemScript->GetNumber();

			info.Type = lpMemScript->GetAsNumber();

			info.Index = lpMemScript->GetAsNumber();

			info.Value = lpMemScript->GetAsNumber();

			info.Level = lpMemScript->GetAsNumber();

			info.Option1 = lpMemScript->GetAsNumber();

			info.Option2 = lpMemScript->GetAsNumber();

			info.Option3 = lpMemScript->GetAsNumber();

			info.NewOption = lpMemScript->GetAsNumber();

			info.RequireIndex = lpMemScript->GetAsNumber();

			info.RequireState = lpMemScript->GetAsNumber();

			for (int n = 0; n < MAX_CLASS; n++)
			{
				info.RequireClass[n] = lpMemScript->GetAsNumber();
			}

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CQuestReward::SetInfo(QUEST_REWARD_INFO info)
{
	if (this->m_count < 0 || this->m_count >= MAX_QUEST_REWARD)
	{
		return;
	}

	this->m_QuestObjectiveInfo[this->m_count++] = info;
}

QUEST_REWARD_INFO* CQuestReward::GetInfo(int index)
{
	if (index < 0 || index >= this->m_count)
	{
		return 0;
	}

	return &this->m_QuestObjectiveInfo[index];
}

bool CQuestReward::CheckQuestRewardRequisite(LPOBJ lpObj, QUEST_REWARD_INFO* lpInfo)
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

void CQuestReward::InsertQuestReward(LPOBJ lpObj, int QuestIndex)
{
	for (int n = 0; n < this->m_count; n++)
	{
		QUEST_REWARD_INFO* lpInfo = this->GetInfo(n);

		if (lpInfo == 0)
		{
			continue;
		}

		if (lpInfo->RequireIndex != QuestIndex)
		{
			continue;
		}

		if (this->CheckQuestRewardRequisite(lpObj, lpInfo) == 0)
		{
			continue;
		}

		if (lpInfo->Type == QUEST_REWARD_NONE)
		{
			continue;
		}

		if (lpInfo->Type == QUEST_REWARD_POINT)
		{
			lpObj->LevelUpPoint += lpInfo->Value;

			gQuest.GCQuestRewardSend(lpObj->Index, lpInfo->Index, lpInfo->Value);

			continue;
		}

		if (lpInfo->Type == QUEST_REWARD_CHANGE1)
		{
			if ((lpObj->DBClass % 16) < 1)
			{
				lpObj->DBClass = ((lpObj->DBClass / 16) * 16) + 1;

				lpObj->ChangeUp = lpObj->DBClass % 16;
			}

			gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);

			BYTE Class = (lpObj->ChangeUp * 16);

			Class -= (Class / 32);

			Class += (lpObj->Class * 32);

			gQuest.GCQuestRewardSend(lpObj->Index, lpInfo->Index, Class);

			continue;
		}

		if (lpInfo->Type == QUEST_REWARD_HERO)
		{
			int AddPoint = ((lpObj->Level > 220) ? (lpObj->Level - 220) : 0) * gServerInfo.m_PlusStatPoint;

			lpObj->LevelUpPoint += AddPoint;

			gQuest.GCQuestRewardSend(lpObj->Index, lpInfo->Index, AddPoint);

			continue;
		}

		if (lpInfo->Type == QUEST_REWARD_COMBO)
		{
			gQuest.GCQuestRewardSend(lpObj->Index, lpInfo->Index, lpInfo->Value);

			continue;
		}

		if (lpInfo->Type == QUEST_REWARD_CHANGE2)
		{
			if ((lpObj->DBClass % 16) < 2)
			{
				lpObj->DBClass = ((lpObj->DBClass / 16) * 16) + 2;

				lpObj->ChangeUp = lpObj->DBClass % 16;
			}

			gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);

			BYTE Class = (lpObj->ChangeUp * 16);

			Class -= (Class / 32);

			Class += (lpObj->Class * 32);

			gQuest.GCQuestRewardSend(lpObj->Index, lpInfo->Index, Class);

			continue;
		}
	}
}

long CQuestReward::GetQuestRewardPoint(LPOBJ lpObj, int QuestIndex)
{
	long point = 0;

	for (int n = 0; n < this->m_count; n++)
	{
		QUEST_REWARD_INFO* lpInfo = this->GetInfo(n);

		if (lpInfo == 0)
		{
			continue;
		}

		if (lpInfo->RequireIndex != QuestIndex)
		{
			continue;
		}

		if (lpInfo->Type == QUEST_REWARD_POINT)
		{
			point += lpInfo->Value;

			continue;
		}
	}

	return point;
}