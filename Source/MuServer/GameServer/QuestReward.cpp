#include "stdafx.h"
#include "QuestReward.h"
#include "ReadScript.h"
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

			QUEST_REWARD_INFO info;

			memset(&info, 0, sizeof(info));

			info.Number = lpReadScript->GetNumber();

			info.Type = lpReadScript->GetAsNumber();

			info.Index = lpReadScript->GetAsNumber();

			info.Value = lpReadScript->GetAsNumber();

			info.Level = lpReadScript->GetAsNumber();

			info.Option1 = lpReadScript->GetAsNumber();

			info.Option2 = lpReadScript->GetAsNumber();

			info.Option3 = lpReadScript->GetAsNumber();

			info.NewOption = lpReadScript->GetAsNumber();

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