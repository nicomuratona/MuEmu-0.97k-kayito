#include "stdafx.h"
#include "Quest.h"
#include "GameMain.h"
#include "ReadScript.h"
#include "Monster.h"
#include "QuestObjective.h"
#include "QuestReward.h"
#include "SkillManager.h"
#include "Util.h"

CQuest gQuest;

CQuest::CQuest()
{
	this->m_count = 0;
}

CQuest::~CQuest()
{

}

void CQuest::Load(char* path)
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

			QUEST_INFO info;

			memset(&info, 0, sizeof(info));

			info.Index = lpReadScript->GetNumber();

			info.MonsterClass = lpReadScript->GetAsNumber();

			info.CurrentState = lpReadScript->GetAsNumber();

			info.RequireIndex = lpReadScript->GetAsNumber();

			info.RequireState = lpReadScript->GetAsNumber();

			info.RequireMinLevel = lpReadScript->GetAsNumber();

			info.RequireMaxLevel = lpReadScript->GetAsNumber();

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

void CQuest::SetInfo(QUEST_INFO info)
{
	if (this->m_count < 0 || this->m_count >= MAX_QUEST)
	{
		return;
	}

	this->m_QuestInfo[this->m_count++] = info;
}

QUEST_INFO* CQuest::GetInfo(int index)
{
	if (index < 0 || index >= this->m_count)
	{
		return 0;
	}

	return &this->m_QuestInfo[index];
}

QUEST_INFO* CQuest::GetInfoByIndex(LPOBJ lpObj, int QuestIndex)
{
	for (int n = 0; n < this->m_count; n++)
	{
		QUEST_INFO* lpInfo = this->GetInfo(n);

		if (lpInfo == 0)
		{
			continue;
		}

		if (lpInfo->Index != QuestIndex)
		{
			continue;
		}

		if (this->CheckQuestListState(lpObj, lpInfo->Index, lpInfo->CurrentState) == 0)
		{
			continue;
		}

		return lpInfo;
	}

	return 0;
}

bool CQuest::AddQuestList(LPOBJ lpObj, int QuestIndex, int QuestState)
{
	if (QuestIndex < 0 || QuestIndex >= MAX_QUEST_LIST)
	{
		return 0;
	}

	lpObj->Quest[QuestIndex / 4] = (lpObj->Quest[QuestIndex / 4] & gQuestBitMask[(QuestIndex % 4) * 2]) | ((QuestState & 3) << ((QuestIndex % 4) * 2));

	return 1;
}

BYTE CQuest::GetQuestList(LPOBJ lpObj, int QuestIndex)
{
	if (QuestIndex < 0 || QuestIndex >= MAX_QUEST_LIST)
	{
		return 0;
	}

	return lpObj->Quest[QuestIndex / 4];
}

bool CQuest::CheckQuestListState(LPOBJ lpObj, int QuestIndex, int QuestState)
{
	if (QuestIndex < 0 || QuestIndex >= MAX_QUEST_LIST)
	{
		return 0;
	}

	if (((lpObj->Quest[QuestIndex / 4] >> ((QuestIndex % 4) * 2)) & 3) == QuestState)
	{
		return 1;
	}

	return 0;
}

long CQuest::GetQuestRewardLevelUpPoint(LPOBJ lpObj)
{
	int point = 0;

	for (int n = 0; n < MAX_QUEST_LIST; n++)
	{
		if (this->CheckQuestListState(lpObj, n, QUEST_FINISH) != 0)
		{
			point += gQuestReward.GetQuestRewardPoint(lpObj, n);
		}
	}

	return point;
}

bool CQuest::NpcTalk(LPOBJ lpNpc, LPOBJ lpObj)
{
	for (int n = 0; n < this->m_count; n++)
	{
		QUEST_INFO* lpInfo = this->GetInfo(n);

		if (lpInfo == 0)
		{
			continue;
		}

		if (lpInfo->MonsterClass != lpNpc->Class)
		{
			continue;
		}

		if (this->CheckQuestListState(lpObj, lpInfo->Index, lpInfo->CurrentState) == 0)
		{
			continue;
		}

		lpObj->Interface.use = 1;
		lpObj->Interface.type = INTERFACE_QUEST;
		lpObj->Interface.state = 0;

		this->GCQuestStateSend(lpObj->Index, lpInfo->Index);

		return 1;
	}

	return 0;
}

void CQuest::CGQuestInfoRecv(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	this->GCQuestInfoSend(aIndex);
}

void CQuest::CGQuestStateRecv(PMSG_QUEST_STATE_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	QUEST_INFO* lpInfo = this->GetInfoByIndex(lpObj, lpMsg->QuestIndex);

	if (lpInfo == 0)
	{
		return;
	}

	if (gQuestObjective.CheckQuestObjective(lpObj, lpInfo->Index) == 0)
	{
		this->GCQuestResultSend(aIndex, lpInfo->Index, 0xFF, this->GetQuestList(lpObj, lpInfo->Index));

		return;
	}

	if (lpInfo->CurrentState == QUEST_NORMAL)
	{
		gQuestObjective.RemoveQuestObjective(lpObj, lpInfo->Index);

		gQuestReward.InsertQuestReward(lpObj, lpInfo->Index);

		this->AddQuestList(lpObj, lpInfo->Index, QUEST_ACCEPT);

		this->GCQuestResultSend(aIndex, lpInfo->Index, 0x00, this->GetQuestList(lpObj, lpInfo->Index));

		return;
	}

	if (lpInfo->CurrentState == QUEST_ACCEPT)
	{
		gQuestObjective.RemoveQuestObjective(lpObj, lpInfo->Index);

		gQuestReward.InsertQuestReward(lpObj, lpInfo->Index);

		this->AddQuestList(lpObj, lpInfo->Index, QUEST_FINISH);

		this->GCQuestResultSend(aIndex, lpInfo->Index, 0x00, this->GetQuestList(lpObj, lpInfo->Index));

		return;
	}

	if (lpInfo->CurrentState == QUEST_FINISH)
	{
		gQuestObjective.RemoveQuestObjective(lpObj, lpInfo->Index);

		gQuestReward.InsertQuestReward(lpObj, lpInfo->Index);

		this->AddQuestList(lpObj, lpInfo->Index, QUEST_FINISH);

		this->GCQuestResultSend(aIndex, lpInfo->Index, 0xFF, this->GetQuestList(lpObj, lpInfo->Index));

		return;
	}

	if (lpInfo->CurrentState == QUEST_CANCEL)
	{
		gQuestObjective.RemoveQuestObjective(lpObj, lpInfo->Index);

		gQuestReward.InsertQuestReward(lpObj, lpInfo->Index);

		this->AddQuestList(lpObj, lpInfo->Index, QUEST_ACCEPT);

		this->GCQuestResultSend(aIndex, lpInfo->Index, 0x00, this->GetQuestList(lpObj, lpInfo->Index));

		return;
	}
}

void CQuest::GCQuestInfoSend(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->SendQuestInfo != 0)
	{
		return;
	}

	PMSG_QUEST_INFO_SEND pMsg;

	pMsg.header.set(0xA0, sizeof(pMsg));

	pMsg.count = this->m_count;

	memcpy(pMsg.QuestInfo, lpObj->Quest, sizeof(pMsg.QuestInfo));

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

	lpObj->SendQuestInfo = 1;
}

void CQuest::GCQuestStateSend(int aIndex, int QuestIndex)
{
	this->GCQuestInfoSend(aIndex);

	PMSG_QUEST_STATE_SEND pMsg;

	pMsg.header.set(0xA1, sizeof(pMsg));

	pMsg.QuestIndex = QuestIndex;

	pMsg.QuestState = this->GetQuestList(&gObj[aIndex], QuestIndex);

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CQuest::GCQuestResultSend(int aIndex, int QuestIndex, int QuestResult, int QuestState)
{
	PMSG_QUEST_RESULT_SEND pMsg;

	pMsg.header.set(0xA2, sizeof(pMsg));

	pMsg.QuestIndex = QuestIndex;

	pMsg.QuestResult = QuestResult;

	pMsg.QuestState = QuestState;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CQuest::GCQuestRewardSend(int aIndex, int QuestReward, int QuestAmount)
{
	this->GCQuestInfoSend(aIndex);

	PMSG_QUEST_REWARD_SEND pMsg;

	pMsg.header.set(0xA3, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.QuestReward = QuestReward;

	pMsg.QuestAmount = QuestAmount;

#if(GAMESERVER_EXTRA==1)

	pMsg.ViewPoint = gObj[aIndex].LevelUpPoint;

#endif

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

	MsgSendV2(&gObj[aIndex], (BYTE*)&pMsg, pMsg.header.size);
}