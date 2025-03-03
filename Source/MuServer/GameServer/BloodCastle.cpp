#include "stdafx.h"
#include "BloodCastle.h"
#include "DSProtocol.h"
#include "EventSpawnMonster.h"
#include "ItemBagManager.h"
#include "Map.h"
#include "ReadScript.h"
#include "Message.h"
#include "Monster.h"
#include "MonsterSetBase.h"
#include "Notice.h"
#include "NpcTalk.h"
#include "ObjectManager.h"
#include "ScheduleManager.h"
#include "ServerInfo.h"
#include "Util.h"

CBloodCastle gBloodCastle;

CBloodCastle::CBloodCastle()
{
	for (int n = 0; n < MAX_BC_LEVEL; n++)
	{
		BLOOD_CASTLE_LEVEL* lpLevel = &this->m_BloodCastleLevel[n];

		lpLevel->Level = n;

		lpLevel->State = BC_STATE_BLANK;

		lpLevel->Map = (MAP_BLOOD_CASTLE1 + n);

		lpLevel->RemainTime = 0;

		lpLevel->TargetTime = 0;

		lpLevel->TickCount = GetTickCount();

		lpLevel->EnterEnabled = 0;

		lpLevel->MinutesLeft = -1;

		lpLevel->TimeCount = 0;

		lpLevel->Stage = 0;

		lpLevel->CurMonster = 0;

		lpLevel->MaxMonster = 0;

		lpLevel->BridgeOpenTime = 0;

		lpLevel->EventItemSerial = -1;

		lpLevel->EventItemNumber = -1;

		lpLevel->EventItemOwner = -1;

		lpLevel->EventItemLevel = -1;

		lpLevel->WinnerIndex = -1;

		this->CleanUser(lpLevel);

		lpLevel->GateIndex = -1;

		lpLevel->SaintStatueIndex = -1;

		this->CleanMonster(lpLevel);
	}

	memset(this->m_BloodCastleExperienceTable, 0, sizeof(this->m_BloodCastleExperienceTable));

	memset(this->m_BloodCastleMoneyTable, 0, sizeof(this->m_BloodCastleMoneyTable));

	memset(this->m_BloodCastleNpcLife, 0, sizeof(this->m_BloodCastleNpcLife));

	this->m_BloodCastleRequiredLevel[0][0] = 15;
	this->m_BloodCastleRequiredLevel[0][1] = 80;
	this->m_BloodCastleRequiredLevel[0][2] = 10;
	this->m_BloodCastleRequiredLevel[0][3] = 60;

	this->m_BloodCastleRequiredLevel[1][0] = 81;
	this->m_BloodCastleRequiredLevel[1][1] = 130;
	this->m_BloodCastleRequiredLevel[1][2] = 61;
	this->m_BloodCastleRequiredLevel[1][3] = 110;

	this->m_BloodCastleRequiredLevel[2][0] = 131;
	this->m_BloodCastleRequiredLevel[2][1] = 180;
	this->m_BloodCastleRequiredLevel[2][2] = 111;
	this->m_BloodCastleRequiredLevel[2][3] = 160;

	this->m_BloodCastleRequiredLevel[3][0] = 181;
	this->m_BloodCastleRequiredLevel[3][1] = 230;
	this->m_BloodCastleRequiredLevel[3][2] = 161;
	this->m_BloodCastleRequiredLevel[3][3] = 210;

	this->m_BloodCastleRequiredLevel[4][0] = 231;
	this->m_BloodCastleRequiredLevel[4][1] = 280;
	this->m_BloodCastleRequiredLevel[4][2] = 211;
	this->m_BloodCastleRequiredLevel[4][3] = 260;

	this->m_BloodCastleRequiredLevel[5][0] = 281;
	this->m_BloodCastleRequiredLevel[5][1] = 99999;
	this->m_BloodCastleRequiredLevel[5][2] = 261;
	this->m_BloodCastleRequiredLevel[5][3] = 99999;
}

CBloodCastle::~CBloodCastle()
{

}

void CBloodCastle::Init()
{
	for (int n = 0; n < MAX_BC_LEVEL; n++)
	{
		if (gServerInfo.m_BloodCastleEvent == 0)
		{
			this->SetState(&this->m_BloodCastleLevel[n], BC_STATE_BLANK);
		}
		else
		{
			this->SetState(&this->m_BloodCastleLevel[n], BC_STATE_EMPTY);
		}
	}
}

void CBloodCastle::Load(char* path)
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

	this->m_BloodCastleStartTime.clear();

	memset(this->m_BloodCastleExperienceTable, 0, sizeof(this->m_BloodCastleExperienceTable));

	memset(this->m_BloodCastleMoneyTable, 0, sizeof(this->m_BloodCastleMoneyTable));

	memset(this->m_BloodCastleNpcLife, 0, sizeof(this->m_BloodCastleNpcLife));

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

				if (section == 0)
				{
					this->m_WarningTime = lpReadScript->GetNumber();

					this->m_NotifyTime = lpReadScript->GetAsNumber();

					this->m_EventTime = lpReadScript->GetAsNumber();

					this->m_CloseTime = lpReadScript->GetAsNumber();
				}
				else if (section == 1)
				{
					BLOOD_CASTLE_START_TIME info;

					info.Year = lpReadScript->GetNumber();

					info.Month = lpReadScript->GetAsNumber();

					info.Day = lpReadScript->GetAsNumber();

					info.DayOfWeek = lpReadScript->GetAsNumber();

					info.Hour = lpReadScript->GetAsNumber();

					info.Minute = lpReadScript->GetAsNumber();

					info.Second = lpReadScript->GetAsNumber();

					this->m_BloodCastleStartTime.push_back(info);
				}
				else if (section == 2)
				{
					int level = lpReadScript->GetNumber();

					this->m_BloodCastleExperienceTable[level][0] = lpReadScript->GetAsNumber();

					this->m_BloodCastleExperienceTable[level][1] = lpReadScript->GetAsNumber();
				}
				else if (section == 3)
				{
					int level = lpReadScript->GetNumber();

					this->m_BloodCastleMoneyTable[level][0] = lpReadScript->GetAsNumber();

					this->m_BloodCastleMoneyTable[level][1] = lpReadScript->GetAsNumber();
				}
				else if (section == 4)
				{
					int level = lpReadScript->GetNumber();

					this->m_BloodCastleNpcLife[level][0] = lpReadScript->GetAsNumber();

					this->m_BloodCastleNpcLife[level][1] = lpReadScript->GetAsNumber();
				}
				else if (section == 5)
				{
					int level = lpReadScript->GetNumber();

					this->m_BloodCastleRequiredLevel[level][0] = lpReadScript->GetAsNumber();

					this->m_BloodCastleRequiredLevel[level][1] = lpReadScript->GetAsNumber();

					this->m_BloodCastleRequiredLevel[level][2] = lpReadScript->GetAsNumber();

					this->m_BloodCastleRequiredLevel[level][3] = lpReadScript->GetAsNumber();
				}
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript->GetError());
	}

	delete lpReadScript;
}

void CBloodCastle::MainProc()
{
	for (int n = 0; n < MAX_BC_LEVEL; n++)
	{
		BLOOD_CASTLE_LEVEL* lpLevel = &this->m_BloodCastleLevel[n];

		DWORD elapsed = GetTickCount() - lpLevel->TickCount;

		if (elapsed < 1000)
		{
			continue;
		}

		lpLevel->TickCount = GetTickCount();

		lpLevel->RemainTime = (int)difftime(lpLevel->TargetTime, time(0));

		switch (lpLevel->State)
		{
			case BC_STATE_BLANK:
			{
				this->ProcState_BLANK(lpLevel);

				break;
			}

			case BC_STATE_EMPTY:
			{
				this->ProcState_EMPTY(lpLevel);

				break;
			}

			case BC_STATE_STAND:
			{
				this->ProcState_STAND(lpLevel);

				break;
			}

			case BC_STATE_START:
			{
				this->ProcState_START(lpLevel);

				break;
			}

			case BC_STATE_CLEAN:
			{
				this->ProcState_CLEAN(lpLevel);

				break;
			}
		}

		this->CheckDelayScore(lpLevel);
	}
}

void CBloodCastle::ProcState_BLANK(BLOOD_CASTLE_LEVEL* lpLevel)
{

}

void CBloodCastle::ProcState_EMPTY(BLOOD_CASTLE_LEVEL* lpLevel)
{
	if (lpLevel->RemainTime > 0 && lpLevel->RemainTime <= (this->m_WarningTime * 60))
	{
		this->CheckUser(lpLevel);

		int minutes = lpLevel->RemainTime / 60;

		if ((lpLevel->RemainTime % 60) == 0)
		{
			minutes--;
		}

		if (lpLevel->MinutesLeft != minutes)
		{
			lpLevel->MinutesLeft = minutes;

			if (lpLevel->Level == 0)
			{
				gNotice.GCNoticeSendToAll(0, 6, (lpLevel->MinutesLeft + 1));
			}
		}

		lpLevel->EnterEnabled = 1;
	}

	if (lpLevel->RemainTime > 0 && lpLevel->RemainTime <= 30 && lpLevel->TimeCount == 0)
	{
		PMSG_TIME_COUNT_SEND pMsg;

		pMsg.header.set(0x92, sizeof(pMsg));

		pMsg.type = 3;

		DataSendAll((BYTE*)&pMsg, pMsg.header.size);

		lpLevel->TimeCount = 1;
	}

	if (lpLevel->RemainTime <= 0)
	{
		if (lpLevel->Level == 0)
		{
			gNotice.GCNoticeSendToAll(0, 7);
		}

		this->NoticeSendToAll(lpLevel, 1, 8, (lpLevel->Level + 1), this->m_NotifyTime);

		this->SetState(lpLevel, BC_STATE_STAND);
	}
}

void CBloodCastle::ProcState_STAND(BLOOD_CASTLE_LEVEL* lpLevel)
{
	this->CheckUser(lpLevel);

	if (this->GetUserCount(lpLevel) == 0)
	{
		LogAdd(LOG_EVENT, "[Blood Castle] (%d) Not enough users", (lpLevel->Level + 1));

		this->SetState(lpLevel, BC_STATE_EMPTY);

		return;
	}

	if (lpLevel->RemainTime > 0 && lpLevel->RemainTime <= 30 && lpLevel->TimeCount == 0)
	{
		PMSG_TIME_COUNT_SEND pMsg;

		pMsg.header.set(0x92, sizeof(pMsg));

		pMsg.type = 4;

		this->DataSendToAll(lpLevel, (BYTE*)&pMsg, pMsg.header.size);

		lpLevel->TimeCount = 1;
	}

	if (lpLevel->RemainTime <= 0)
	{
		this->NoticeSendToAll(lpLevel, 0, 9);

		this->SetState(lpLevel, BC_STATE_START);
	}
}

void CBloodCastle::ProcState_START(BLOOD_CASTLE_LEVEL* lpLevel)
{
	this->CheckUser(lpLevel);

	if (this->GetUserCount(lpLevel) == 0)
	{
		LogAdd(LOG_EVENT, "[Blood Castle] (%d) Not enough users", (lpLevel->Level + 1));

		this->SetState(lpLevel, BC_STATE_EMPTY);

		return;
	}

	switch (lpLevel->Stage)
	{
		case 0:
		{
			this->SetStage0(lpLevel);

			break;
		}

		case 1:
		{
			this->SetStage1(lpLevel);

			break;
		}

		case 2:
		{
			this->SetStage2(lpLevel);

			break;
		}

		case 3:
		{
			this->SetStage3(lpLevel);

			break;
		}

		case 4:
		{
			this->SetStage4(lpLevel);

			break;
		}
	}

	if (lpLevel->Stage == 0 || lpLevel->Stage == 1 || lpLevel->Stage == 2)
	{
		this->GCBloodCastleStateSend(lpLevel, 1);
	}
	else
	{
		this->GCBloodCastleStateSend(lpLevel, 4);
	}

	if (lpLevel->Stage == 4 && OBJECT_RANGE(lpLevel->WinnerIndex) != 0)
	{
		this->SetState(lpLevel, BC_STATE_CLEAN);

		return;
	}

	if (lpLevel->RemainTime > 0 && lpLevel->RemainTime <= 30 && lpLevel->TimeCount == 0)
	{
		PMSG_TIME_COUNT_SEND pMsg;

		pMsg.header.set(0x92, sizeof(pMsg));

		pMsg.type = 5;

		this->DataSendToAll(lpLevel, (BYTE*)&pMsg, pMsg.header.size);

		lpLevel->TimeCount = 1;
	}

	if (lpLevel->RemainTime <= 0)
	{
		this->NoticeSendToAll(lpLevel, 0, 12);

		this->SetState(lpLevel, BC_STATE_CLEAN);
	}
}

void CBloodCastle::ProcState_CLEAN(BLOOD_CASTLE_LEVEL* lpLevel)
{
	this->CheckUser(lpLevel);

	if (lpLevel->RemainTime > 0 && lpLevel->RemainTime <= 30 && lpLevel->TimeCount == 0)
	{
		PMSG_TIME_COUNT_SEND pMsg;

		pMsg.header.set(0x92, sizeof(pMsg));

		pMsg.type = 6;

		this->DataSendToAll(lpLevel, (BYTE*)&pMsg, pMsg.header.size);

		lpLevel->TimeCount = 1;
	}

	if (lpLevel->RemainTime <= 0)
	{
		this->SetState(lpLevel, BC_STATE_EMPTY);
	}
}

void CBloodCastle::SetState(BLOOD_CASTLE_LEVEL* lpLevel, int state)
{
	lpLevel->State = state;

	switch (lpLevel->State)
	{
		case BC_STATE_BLANK:
		{
			this->SetState_BLANK(lpLevel);

			break;
		}

		case BC_STATE_EMPTY:
		{
			this->SetState_EMPTY(lpLevel);

			break;
		}

		case BC_STATE_STAND:
		{
			this->SetState_STAND(lpLevel);

			break;
		}

		case BC_STATE_START:
		{
			this->SetState_START(lpLevel);

			break;
		}

		case BC_STATE_CLEAN:
		{
			this->SetState_CLEAN(lpLevel);

			break;
		}
	}
}

void CBloodCastle::SetState_BLANK(BLOOD_CASTLE_LEVEL* lpLevel)
{
	LogAdd(LOG_EVENT, "[Blood Castle] (%d) SetState BLANK", (lpLevel->Level + 1));
}

void CBloodCastle::SetState_EMPTY(BLOOD_CASTLE_LEVEL* lpLevel)
{
	lpLevel->EnterEnabled = 0;

	lpLevel->MinutesLeft = -1;

	lpLevel->TimeCount = 0;

	lpLevel->Stage = 0;

	lpLevel->CurMonster = 0;

	lpLevel->MaxMonster = 0;

	lpLevel->BridgeOpenTime = 0;

	lpLevel->EventItemSerial = -1;

	lpLevel->EventItemNumber = -1;

	lpLevel->EventItemLevel = -1;

	lpLevel->EventItemOwner = -1;

	lpLevel->WinnerIndex = -1;

	this->GCBloodCastleSimpleStateSend(lpLevel, 2, -1);

	this->ClearUser(lpLevel);

	if (OBJECT_RANGE(lpLevel->GateIndex) != 0)
	{
		gObjDel(lpLevel->GateIndex);

		lpLevel->GateIndex = -1;
	}

	if (OBJECT_RANGE(lpLevel->SaintStatueIndex) != 0)
	{
		gObjDel(lpLevel->SaintStatueIndex);

		lpLevel->SaintStatueIndex = -1;
	}

	this->ClearMonster(lpLevel);

	this->SetEntranceZone(lpLevel);

	this->SetBridgeZone(lpLevel, 0);

	this->SetGateZone(lpLevel);

	this->CheckSync(lpLevel);

	LogAdd(LOG_EVENT, "[Blood Castle] (%d) SetState EMPTY", (lpLevel->Level + 1));
}

void CBloodCastle::SetState_STAND(BLOOD_CASTLE_LEVEL* lpLevel)
{
	lpLevel->EnterEnabled = 0;

	lpLevel->MinutesLeft = -1;

	lpLevel->TimeCount = 0;

	lpLevel->RemainTime = this->m_NotifyTime * 60;

	lpLevel->TargetTime = (int)(time(0) + lpLevel->RemainTime);

	LogAdd(LOG_EVENT, "[Blood Castle] (%d) SetState STAND", (lpLevel->Level + 1));
}

void CBloodCastle::SetState_START(BLOOD_CASTLE_LEVEL* lpLevel)
{
	lpLevel->EnterEnabled = 0;

	lpLevel->MinutesLeft = -1;

	lpLevel->TimeCount = 0;

	lpLevel->CurMonster = 0;

	lpLevel->MaxMonster = this->GetUserCount(lpLevel) * 40;

	this->DelEntranceZone(lpLevel);

	this->SetGate(lpLevel);

	this->StageSpawn(lpLevel, 0);

	this->GCBloodCastleSimpleStateSend(lpLevel, 0, -1);

	lpLevel->RemainTime = this->m_EventTime * 60;

	lpLevel->TargetTime = (int)(time(0) + lpLevel->RemainTime);

	LogAdd(LOG_EVENT, "[Blood Castle] (%d) SetState START", (lpLevel->Level + 1));
}

void CBloodCastle::SetState_CLEAN(BLOOD_CASTLE_LEVEL* lpLevel)
{
	lpLevel->EnterEnabled = 0;

	lpLevel->MinutesLeft = -1;

	lpLevel->TimeCount = 0;

	if (OBJECT_RANGE(lpLevel->EventItemOwner) != 0)
	{
		this->SearchUserDeleteEventItem(lpLevel->EventItemOwner);
	}

	if (OBJECT_RANGE(lpLevel->GateIndex) != 0)
	{
		gObjDel(lpLevel->GateIndex);

		lpLevel->GateIndex = -1;
	}

	if (OBJECT_RANGE(lpLevel->SaintStatueIndex) != 0)
	{
		gObjDel(lpLevel->SaintStatueIndex);

		lpLevel->SaintStatueIndex = -1;
	}

	this->ClearMonster(lpLevel);

	for (int n = 0; n < MAX_BC_USER; n++)
	{
		if (OBJECT_RANGE(lpLevel->User[n].Index) != 0)
		{
			this->GiveUserRewardExperience(lpLevel, lpLevel->User[n].Index);

			this->GiveUserRewardMoney(lpLevel, lpLevel->User[n].Index);

			if (lpLevel->User[n].Index == lpLevel->WinnerIndex || (OBJECT_RANGE(lpLevel->WinnerIndex) != 0 && gParty.IsMember(gObj[lpLevel->WinnerIndex].PartyNumber, lpLevel->User[n].Index) != 0))
			{
				this->GiveUserRewardItem(lpLevel, lpLevel->User[n].Index);

				this->GCBloodCastleScoreSend(lpLevel, lpLevel->User[n].Index, 1);
			}
			else
			{
				this->GCBloodCastleScoreSend(lpLevel, lpLevel->User[n].Index, 0);
			}

			GDRankingBloodCastleSaveSend(lpLevel->User[n].Index, lpLevel->User[n].Score);
		}
	}

	this->GCBloodCastleSimpleStateSend(lpLevel, 2, -1);

	lpLevel->RemainTime = this->m_CloseTime * 60;

	lpLevel->TargetTime = (int)(time(0) + lpLevel->RemainTime);

	LogAdd(LOG_EVENT, "[Blood Castle] (%d) SetState CLEAN", (lpLevel->Level + 1));
}

void CBloodCastle::SetStage0(BLOOD_CASTLE_LEVEL* lpLevel)
{
	if (lpLevel->CurMonster < lpLevel->MaxMonster)
	{
		return;
	}

	lpLevel->Stage = 1;

	lpLevel->BridgeOpenTime = GetTickCount();

	this->GCBloodCastleSimpleStateSend(lpLevel, 3, -1);

	this->DelBridgeZone(lpLevel, 0);

	this->NoticeSendToAll(lpLevel, 0, 10);
}

void CBloodCastle::SetStage1(BLOOD_CASTLE_LEVEL* lpLevel)
{
	if ((GetTickCount() - lpLevel->BridgeOpenTime) < 3000)
	{
		return;
	}

	lpLevel->Stage = 2;

	lpLevel->BridgeOpenTime = 0;

	this->DelBridgeZone(lpLevel, 1);

	if (OBJECT_RANGE(lpLevel->GateIndex) != 0)
	{
		gEffectManager.DelEffect(&gObj[lpLevel->GateIndex], EFFECT_MONSTER_PHYSI_DAMAGE_IMMUNITY);

		gEffectManager.DelEffect(&gObj[lpLevel->GateIndex], EFFECT_MONSTER_MAGIC_DAMAGE_IMMUNITY);
	}
}

void CBloodCastle::SetStage2(BLOOD_CASTLE_LEVEL* lpLevel)
{
	if (OBJECT_RANGE(lpLevel->GateIndex) != 0)
	{
		return;
	}

	lpLevel->Stage = 3;

	lpLevel->CurMonster = 0;

	lpLevel->MaxMonster = this->GetUserCount(lpLevel) * 2;

	this->DelGateZone(lpLevel);

	this->StageSpawn(lpLevel, lpLevel->Stage);
}

void CBloodCastle::SetStage3(BLOOD_CASTLE_LEVEL* lpLevel)
{
	if (lpLevel->CurMonster < lpLevel->MaxMonster)
	{
		return;
	}

	lpLevel->Stage = 4;

	this->SetSaintStatue(lpLevel);

	this->NoticeSendToAll(lpLevel, 0, 11);

	if (OBJECT_RANGE(lpLevel->SaintStatueIndex) != 0)
	{
		gEffectManager.DelEffect(&gObj[lpLevel->SaintStatueIndex], EFFECT_MONSTER_PHYSI_DAMAGE_IMMUNITY);

		gEffectManager.DelEffect(&gObj[lpLevel->SaintStatueIndex], EFFECT_MONSTER_MAGIC_DAMAGE_IMMUNITY);
	}
}

void CBloodCastle::SetStage4(BLOOD_CASTLE_LEVEL* lpLevel)
{

}

void CBloodCastle::CheckSync(BLOOD_CASTLE_LEVEL* lpLevel)
{
	if (this->m_BloodCastleStartTime.empty() != 0)
	{
		this->SetState(lpLevel, BC_STATE_BLANK);
		return;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for (std::vector<BLOOD_CASTLE_START_TIME>::iterator it = this->m_BloodCastleStartTime.begin(); it != this->m_BloodCastleStartTime.end(); it++)
	{
		ScheduleManager.AddSchedule(it->Year, it->Month, it->Day, it->Hour, it->Minute, it->Second, it->DayOfWeek);
	}

	if (ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(lpLevel, BC_STATE_BLANK);

		return;
	}

	lpLevel->RemainTime = (int)difftime(ScheduleTime.GetTime(), time(0));

	lpLevel->TargetTime = (int)ScheduleTime.GetTime();

	LogAdd(LOG_EVENT, "[Blood Castle] (%d) Sync Start Time. [%d] min remain", (lpLevel->Level + 1), (lpLevel->RemainTime / 60));
}

int CBloodCastle::GetState(int level)
{
	if (BC_LEVEL_RANGE(level) == 0)
	{
		return BC_STATE_BLANK;
	}

	return this->m_BloodCastleLevel[level].State;
}

int CBloodCastle::GetCurrentRemainTime(int level)
{
	if (BC_LEVEL_RANGE(level) == 0)
	{
		return BC_STATE_BLANK;
	}

	BLOOD_CASTLE_LEVEL* lpLevel = &this->m_BloodCastleLevel[level];

	return lpLevel->RemainTime;
}

int CBloodCastle::GetRemainTime(int level)
{
	if (BC_LEVEL_RANGE(level) == 0)
	{
		return 0;
	}

	if (this->m_BloodCastleStartTime.empty() != 0)
	{
		return 0;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for (std::vector<BLOOD_CASTLE_START_TIME>::iterator it = this->m_BloodCastleStartTime.begin(); it != this->m_BloodCastleStartTime.end(); it++)
	{
		ScheduleManager.AddSchedule(it->Year, it->Month, it->Day, it->Hour, it->Minute, it->Second, it->DayOfWeek);
	}

	if (ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		return 0;
	}

	int RemainTime = (int)difftime(ScheduleTime.GetTime(), time(0));

	return (((RemainTime % 60) == 0) ? (RemainTime / 60) : ((RemainTime / 60) + 1));
}

int CBloodCastle::GetEnterEnabled(int level)
{
	if (BC_LEVEL_RANGE(level) == 0)
	{
		return 0;
	}

	return this->m_BloodCastleLevel[level].EnterEnabled;
}

int CBloodCastle::GetEnteredUserCount(int level)
{
	if (BC_LEVEL_RANGE(level) == 0)
	{
		return 0;
	}

	return this->GetUserCount(&this->m_BloodCastleLevel[level]);
}

bool CBloodCastle::AddUser(BLOOD_CASTLE_LEVEL* lpLevel, int aIndex)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	if (this->GetUser(lpLevel, aIndex) != 0)
	{
		return 0;
	}

	for (int n = 0; n < MAX_BC_USER; n++)
	{
		if (OBJECT_RANGE(lpLevel->User[n].Index) != 0)
		{
			continue;
		}

		lpLevel->User[n].Index = aIndex;

		lpLevel->User[n].Score = 0;

		lpLevel->User[n].RewardExperience = 0;

		lpLevel->User[n].RewardMoney = 0;

		return 1;
	}

	return 0;
}

bool CBloodCastle::DelUser(BLOOD_CASTLE_LEVEL* lpLevel, int aIndex)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	BLOOD_CASTLE_USER* lpUser = this->GetUser(lpLevel, aIndex);

	if (lpUser == 0)
	{
		return 0;
	}

	if (lpLevel->State == BC_STATE_START)
	{
		this->SearchUserDropEventItem(aIndex);

		this->GCBloodCastleSimpleStateSend(lpLevel, 2, aIndex);
	}
	else
	{
		this->SearchUserDeleteEventItem(aIndex);
	}

	lpUser->Index = -1;

	lpUser->Score = 0;

	lpUser->RewardExperience = 0;

	lpUser->RewardMoney = 0;

	return 1;
}

BLOOD_CASTLE_USER* CBloodCastle::GetUser(BLOOD_CASTLE_LEVEL* lpLevel, int aIndex)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	for (int n = 0; n < MAX_BC_USER; n++)
	{
		if (lpLevel->User[n].Index == aIndex)
		{
			return &lpLevel->User[n];
		}
	}

	return 0;
}

void CBloodCastle::CleanUser(BLOOD_CASTLE_LEVEL* lpLevel)
{
	for (int n = 0; n < MAX_BC_USER; n++)
	{
		lpLevel->User[n].Index = -1;

		lpLevel->User[n].Score = 0;

		lpLevel->User[n].RewardExperience = 0;

		lpLevel->User[n].RewardMoney = 0;
	}
}

void CBloodCastle::ClearUser(BLOOD_CASTLE_LEVEL* lpLevel)
{
	for (int n = 0; n < MAX_BC_USER; n++)
	{
		if (OBJECT_RANGE(lpLevel->User[n].Index) == 0)
		{
			continue;
		}

		gObjMoveGate(lpLevel->User[n].Index, 22);

		lpLevel->User[n].Index = -1;

		lpLevel->User[n].Score = 0;

		lpLevel->User[n].RewardExperience = 0;

		lpLevel->User[n].RewardMoney = 0;
	}
}

void CBloodCastle::CheckUser(BLOOD_CASTLE_LEVEL* lpLevel)
{
	for (int n = 0; n < MAX_BC_USER; n++)
	{
		if (OBJECT_RANGE(lpLevel->User[n].Index) == 0)
		{
			continue;
		}

		if (gObjIsConnected(lpLevel->User[n].Index) == 0)
		{
			this->DelUser(lpLevel, lpLevel->User[n].Index);

			continue;
		}

		if (gObj[lpLevel->User[n].Index].Map != lpLevel->Map)
		{
			this->DelUser(lpLevel, lpLevel->User[n].Index);

			continue;
		}
	}
}

int CBloodCastle::GetUserCount(BLOOD_CASTLE_LEVEL* lpLevel)
{
	int count = 0;

	for (int n = 0; n < MAX_BC_USER; n++)
	{
		if (OBJECT_RANGE(lpLevel->User[n].Index) != 0)
		{
			count++;
		}
	}

	return count;
}

int CBloodCastle::GetUserAbleLevel(LPOBJ lpObj)
{
	int MinLevelIndex = (lpObj->Class == CLASS_MG) ? 2 : 0;
	int MaxLevelIndex = (lpObj->Class == CLASS_MG) ? 3 : 1;

	for (int Level = 0; Level < MAX_BC_LEVEL; Level++)
	{
		if (lpObj->Level >= this->m_BloodCastleRequiredLevel[Level][MinLevelIndex] && lpObj->Level <= this->m_BloodCastleRequiredLevel[Level][MaxLevelIndex])
		{
			return Level;
		}
	}

	return -1;
}

void CBloodCastle::GiveUserRewardExperience(BLOOD_CASTLE_LEVEL* lpLevel, int aIndex)
{
	BLOOD_CASTLE_USER* lpUser = this->GetUser(lpLevel, aIndex);

	if (lpUser == 0)
	{
		return;
	}

	if (lpUser->Index == lpLevel->WinnerIndex || (OBJECT_RANGE(lpLevel->WinnerIndex) != 0 && gParty.IsMember(gObj[lpLevel->WinnerIndex].PartyNumber, lpUser->Index) != 0))
	{
		lpUser->RewardExperience = this->m_BloodCastleExperienceTable[lpLevel->Level][0];
	}
	else
	{
		lpUser->RewardExperience = this->m_BloodCastleExperienceTable[lpLevel->Level][1];
	}

	lpUser->Score = lpUser->RewardExperience / 5;

	lpUser->RewardExperience = lpUser->RewardExperience * gServerInfo.m_AddEventExperienceRate[gObj[lpUser->Index].AccountLevel];

	if (gObjectManager.CharacterLevelUp(&gObj[lpUser->Index], lpUser->RewardExperience, gServerInfo.m_MaxLevelUpEvent, EXPERIENCE_BLOOD_CASTLE) != 0)
	{
		return;
	}

	GCRewardExperienceSend(lpUser->Index, lpUser->RewardExperience);
}

void CBloodCastle::GiveUserRewardMoney(BLOOD_CASTLE_LEVEL* lpLevel, int aIndex)
{
	BLOOD_CASTLE_USER* lpUser = this->GetUser(lpLevel, aIndex);

	if (lpUser == 0)
	{
		return;
	}

	if (lpUser->Index == lpLevel->WinnerIndex || (OBJECT_RANGE(lpLevel->WinnerIndex) != 0 && gParty.IsMember(gObj[lpLevel->WinnerIndex].PartyNumber, lpUser->Index) != 0))
	{
		lpUser->RewardMoney = this->m_BloodCastleMoneyTable[lpLevel->Level][0];
	}
	else
	{
		lpUser->RewardMoney = this->m_BloodCastleMoneyTable[lpLevel->Level][1];
	}

	if (gObjCheckMaxMoney(lpUser->Index, lpUser->RewardMoney) == 0)
	{
		gObj[lpUser->Index].Money = MAX_MONEY;
	}
	else
	{
		gObj[lpUser->Index].Money += lpUser->RewardMoney;
	}

	GCMoneySend(lpUser->Index, gObj[lpUser->Index].Money);
}

void CBloodCastle::GiveUserRewardItem(BLOOD_CASTLE_LEVEL* lpLevel, int aIndex)
{
	BLOOD_CASTLE_USER* lpUser = this->GetUser(lpLevel, aIndex);

	if (lpUser == 0)
	{
		return;
	}

	switch (lpLevel->Level)
	{
		case 0:
		{
			gItemBagManager.DropItemBySpecialValue(ITEM_BAG_BLOOD_CASTLE1, &gObj[lpUser->Index], gObj[lpUser->Index].Map, gObj[lpUser->Index].X, gObj[lpUser->Index].Y);

			break;
		}

		case 1:
		{
			gItemBagManager.DropItemBySpecialValue(ITEM_BAG_BLOOD_CASTLE2, &gObj[lpUser->Index], gObj[lpUser->Index].Map, gObj[lpUser->Index].X, gObj[lpUser->Index].Y);

			break;
		}

		case 2:
		{
			gItemBagManager.DropItemBySpecialValue(ITEM_BAG_BLOOD_CASTLE3, &gObj[lpUser->Index], gObj[lpUser->Index].Map, gObj[lpUser->Index].X, gObj[lpUser->Index].Y);

			break;
		}

		case 3:
		{
			gItemBagManager.DropItemBySpecialValue(ITEM_BAG_BLOOD_CASTLE4, &gObj[lpUser->Index], gObj[lpUser->Index].Map, gObj[lpUser->Index].X, gObj[lpUser->Index].Y);

			break;
		}

		case 4:
		{
			gItemBagManager.DropItemBySpecialValue(ITEM_BAG_BLOOD_CASTLE5, &gObj[lpUser->Index], gObj[lpUser->Index].Map, gObj[lpUser->Index].X, gObj[lpUser->Index].Y);

			break;
		}

		case 5:
		{
			gItemBagManager.DropItemBySpecialValue(ITEM_BAG_BLOOD_CASTLE6, &gObj[lpUser->Index], gObj[lpUser->Index].Map, gObj[lpUser->Index].X, gObj[lpUser->Index].Y);

			break;
		}
	}
}

void CBloodCastle::SearchUserDropEventItem(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	int level = GET_BC_LEVEL(lpObj->Map);

	if (BC_LEVEL_RANGE(level) == 0)
	{
		return;
	}

	BLOOD_CASTLE_LEVEL* lpLevel = &this->m_BloodCastleLevel[level];

	if (this->GetUser(lpLevel, aIndex) == 0)
	{
		return;
	}

	if (lpLevel->EventItemOwner != aIndex)
	{
		return;
	}

	int slot = gItemManager.GetInventoryItemSlot(lpObj, GET_ITEM(13, 19), lpLevel->EventItemLevel);

	if (slot == -1)
	{
		return;
	}

	if (lpObj->Inventory[slot].m_Serial != lpLevel->EventItemSerial)
	{
		return;
	}

	PMSG_ITEM_DROP_SEND pMsg;

	pMsg.header.set(0x23, sizeof(pMsg));

	pMsg.result = 0;

	pMsg.slot = slot;

	CItem* lpItem = &gObj[aIndex].Inventory[slot];

	if (gMap[lpObj->Map].ItemDrop(lpItem->m_Index, lpItem->m_Level, lpItem->m_Durability, lpObj->X, lpObj->Y, lpItem->m_SkillOption, lpItem->m_LuckOption, lpItem->m_AddOption, lpItem->m_ExceOption, lpItem->m_Serial, aIndex) != 0)
	{
		pMsg.result = 1;

		gItemManager.InventoryDelItem(aIndex, slot);

		gItemManager.UpdateInventoryViewport(aIndex, slot);
	}

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CBloodCastle::SearchUserDeleteEventItem(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	int level = GET_BC_LEVEL(lpObj->Map);

	if (BC_LEVEL_RANGE(level) == 0)
	{
		return;
	}

	BLOOD_CASTLE_LEVEL* lpLevel = &this->m_BloodCastleLevel[level];

	if (this->GetUser(lpLevel, aIndex) == 0)
	{
		return;
	}

	if (lpLevel->EventItemOwner != aIndex)
	{
		return;
	}

	int slot = gItemManager.GetInventoryItemSlot(lpObj, GET_ITEM(13, 19), lpLevel->EventItemLevel);

	if (slot == -1)
	{
		return;
	}

	if (lpObj->Inventory[slot].m_Serial != lpLevel->EventItemSerial)
	{
		return;
	}

	gItemManager.InventoryDelItem(lpObj->Index, slot);

	gItemManager.GCItemDeleteSend(lpObj->Index, slot, 1);

	lpLevel->EventItemSerial = -1;

	lpLevel->EventItemNumber = -1;

	lpLevel->EventItemOwner = -1;

	lpLevel->EventItemLevel = -1;
}

bool CBloodCastle::AddMonster(BLOOD_CASTLE_LEVEL* lpLevel, int aIndex)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	if (this->GetMonster(lpLevel, aIndex) != 0)
	{
		return 0;
	}

	for (int n = 0; n < MAX_BC_MONSTER; n++)
	{
		if (OBJECT_RANGE(lpLevel->MonsterIndex[n]) != 0)
		{
			continue;
		}

		lpLevel->MonsterIndex[n] = aIndex;

		return 1;
	}

	return 0;
}

bool CBloodCastle::DelMonster(BLOOD_CASTLE_LEVEL* lpLevel, int aIndex)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	int* index = this->GetMonster(lpLevel, aIndex);

	if (index == 0)
	{
		return 0;
	}

	(*index) = -1;

	return 1;
}

int* CBloodCastle::GetMonster(BLOOD_CASTLE_LEVEL* lpLevel, int aIndex)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	for (int n = 0; n < MAX_BC_MONSTER; n++)
	{
		if (lpLevel->MonsterIndex[n] == aIndex)
		{
			return &lpLevel->MonsterIndex[n];
		}
	}

	return 0;
}

void CBloodCastle::CleanMonster(BLOOD_CASTLE_LEVEL* lpLevel)
{
	for (int n = 0; n < MAX_BC_MONSTER; n++)
	{
		lpLevel->MonsterIndex[n] = -1;
	}
}

void CBloodCastle::ClearMonster(BLOOD_CASTLE_LEVEL* lpLevel)
{
	for (int n = 0; n < MAX_BC_MONSTER; n++)
	{
		if (OBJECT_RANGE(lpLevel->MonsterIndex[n]) != 0)
		{
			gObjDel(lpLevel->MonsterIndex[n]);

			lpLevel->MonsterIndex[n] = -1;
		}
	}
}

int CBloodCastle::GetMonsterCount(BLOOD_CASTLE_LEVEL* lpLevel)
{
	int count = 0;

	for (int n = 0; n < MAX_BC_MONSTER; n++)
	{
		if (OBJECT_RANGE(lpLevel->MonsterIndex[n]) != 0)
		{
			count++;
		}
	}

	return count;
}

void CBloodCastle::SetGate(BLOOD_CASTLE_LEVEL* lpLevel)
{
	if (OBJECT_RANGE(lpLevel->GateIndex) != 0)
	{
		return;
	}

	int index = gObjAddMonster(lpLevel->Map);

	if (OBJECT_RANGE(index) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[index];

	int px = 14;

	int py = 75;

	lpObj->PosNum = -1;

	lpObj->X = px;

	lpObj->Y = py;

	lpObj->TX = px;

	lpObj->TY = py;

	lpObj->OldX = px;

	lpObj->OldY = py;

	lpObj->StartX = px;

	lpObj->StartY = py;

	lpObj->Dir = 1;

	lpObj->Map = lpLevel->Map;

	if (gObjSetMonster(index, 131) == 0)
	{
		gObjDel(index);

		return;
	}

	lpObj->Life = (float)this->m_BloodCastleNpcLife[lpLevel->Level][0];

	lpObj->MaxLife = (float)this->m_BloodCastleNpcLife[lpLevel->Level][0];

	gEffectManager.AddEffect(lpObj, 1, EFFECT_MONSTER_PHYSI_DAMAGE_IMMUNITY, 0, 0, 0, 0, 0);

	gEffectManager.AddEffect(lpObj, 1, EFFECT_MONSTER_MAGIC_DAMAGE_IMMUNITY, 0, 0, 0, 0, 0);

	lpLevel->GateIndex = index;
}

void CBloodCastle::SetSaintStatue(BLOOD_CASTLE_LEVEL* lpLevel)
{
	if (OBJECT_RANGE(lpLevel->SaintStatueIndex) != 0)
	{
		return;
	}

	int index = gObjAddMonster(lpLevel->Map);

	if (OBJECT_RANGE(index) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[index];

	int px = 14;

	int py = 95;

	lpObj->PosNum = -1;

	lpObj->X = px;

	lpObj->Y = py;

	lpObj->TX = px;

	lpObj->TY = py;

	lpObj->OldX = px;

	lpObj->OldY = py;

	lpObj->StartX = px;

	lpObj->StartY = py;

	lpObj->Dir = 1;

	lpObj->Map = lpLevel->Map;

	if (gObjSetMonster(index, (132 + (GetLargeRand() % 3))) == 0)
	{
		gObjDel(index);

		return;
	}

	lpObj->Life = (float)this->m_BloodCastleNpcLife[lpLevel->Level][1];

	lpObj->MaxLife = (float)this->m_BloodCastleNpcLife[lpLevel->Level][1];

	gEffectManager.AddEffect(lpObj, 1, EFFECT_MONSTER_PHYSI_DAMAGE_IMMUNITY, 0, 0, 0, 0, 0);

	gEffectManager.AddEffect(lpObj, 1, EFFECT_MONSTER_MAGIC_DAMAGE_IMMUNITY, 0, 0, 0, 0, 0);

	lpLevel->SaintStatueIndex = index;
}

void CBloodCastle::StageSpawn(BLOOD_CASTLE_LEVEL* lpLevel, int Stage)
{
	for (std::vector<EVENT_SPAWN_MONSTER_INFO>::iterator it = gEventSpawnMonster.m_EventSpawnMonsterInfo.begin(); it != gEventSpawnMonster.m_EventSpawnMonsterInfo.end(); it++)
	{
		if (it->Event != EVENT_SPAWN_BLOODCASTLE || it->Level != lpLevel->Level || it->Stage != Stage)
		{
			continue;
		}

		this->SetMonster(lpLevel, it->Monster, it->MaxRegenTime);
	}
}

void CBloodCastle::SetMonster(BLOOD_CASTLE_LEVEL* lpLevel, int MonsterClass, int RegenTime)
{
	for (int n = 0; n < gMonsterSetBase.m_count; n++)
	{
		MONSTER_SET_BASE_INFO* lpInfo = &gMonsterSetBase.m_MonsterSetBaseInfo[n];

		if (lpInfo->Type != 4 || lpInfo->MonsterClass != MonsterClass || lpInfo->Map != lpLevel->Map)
		{
			continue;
		}

		int index = gObjAddMonster(lpLevel->Map);

		if (OBJECT_RANGE(index) == 0)
		{
			continue;
		}

		LPOBJ lpObj = &gObj[index];

		if (gObjSetPosMonster(index, n) == 0)
		{
			gObjDel(index);

			continue;
		}

		if (gObjSetMonster(index, lpInfo->MonsterClass) == 0)
		{
			gObjDel(index);

			continue;
		}

		lpObj->MaxRegenTime = RegenTime * 1000;

		if (this->AddMonster(lpLevel, index) == 0)
		{
			gObjDel(index);

			continue;
		}
	}
}

void CBloodCastle::NpcAngelKing(LPOBJ lpNpc, LPOBJ lpObj)
{
	if (gServerInfo.m_BloodCastleEvent == 0)
	{
		return;
	}

	int level = GET_BC_LEVEL(lpObj->Map);

	if (BC_LEVEL_RANGE(level) == 0)
	{
		return;
	}

	BLOOD_CASTLE_LEVEL* lpLevel = &this->m_BloodCastleLevel[level];

	if (OBJECT_RANGE(lpLevel->WinnerIndex) != 0)
	{
		GCServerCommandSend(lpObj->Index, 1, 46, 0);

		return;
	}

	if (this->GetState(level) != BC_STATE_START)
	{
		GCServerCommandSend(lpObj->Index, 1, 24, 0);

		return;
	}

	BLOOD_CASTLE_USER* lpUser = this->GetUser(lpLevel, lpObj->Index);

	if (lpUser == 0)
	{
		GCServerCommandSend(lpObj->Index, 1, 24, 0);

		return;
	}

	if (lpObj->Index != lpLevel->EventItemOwner)
	{
		GCServerCommandSend(lpObj->Index, 1, 24, 0);

		return;
	}

	int slot = gItemManager.GetInventoryItemSlot(lpObj, GET_ITEM(13, 19), lpLevel->EventItemLevel);

	if (slot == -1)
	{
		GCServerCommandSend(lpObj->Index, 1, 24, 0);

		return;
	}

	if (lpObj->Inventory[slot].m_Serial != lpLevel->EventItemSerial)
	{
		GCServerCommandSend(lpObj->Index, 1, 24, 0);

		return;
	}

	gItemManager.InventoryDelItem(lpObj->Index, slot);

	gItemManager.GCItemDeleteSend(lpObj->Index, slot, 1);

	lpLevel->EventItemSerial = -1;

	lpLevel->EventItemNumber = -1;

	lpLevel->EventItemOwner = -1;

	lpLevel->EventItemLevel = -1;

	lpLevel->WinnerIndex = lpObj->Index;

	GCServerCommandSend(lpObj->Index, 1, 23, 0);
}

void CBloodCastle::NpcAngelMessenger(LPOBJ lpNpc, LPOBJ lpObj)
{
	if (gServerInfo.m_BloodCastleEvent == 0)
	{
		return;
	}

	int level = this->GetUserAbleLevel(lpObj);

	if (BC_LEVEL_RANGE(level) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(13, lpObj->Lang));

		return;
	}

	if (gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(13, 18), -1) == 0)
	{
		GCServerCommandSend(lpObj->Index, 1, 21, 0);

		return;
	}

	if (this->GetEnterEnabled(level) == 0)
	{
		GCServerCommandSend(lpObj->Index, 1, 20, 0);

		return;
	}

	this->GCRequiredLevelsSend(lpObj->Index);

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30, sizeof(pMsg));

	pMsg.result = 6;

	DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
}

bool CBloodCastle::CheckEventItemSerial(int map, CMapItem* lpItem)
{
	int level = GET_BC_LEVEL(map);

	if (BC_LEVEL_RANGE(level) == 0)
	{
		return 0;
	}

	if (this->m_BloodCastleLevel[level].EventItemSerial == -1)
	{
		return 0;
	}

	if (lpItem->m_Index == GET_ITEM(13, 19) && lpItem->m_Serial == this->m_BloodCastleLevel[level].EventItemSerial)
	{
		return 1;
	}

	return 0;
}

void CBloodCastle::SetEventItemSerial(int map, int index, int serial)
{
	int level = GET_BC_LEVEL(map);

	if (BC_LEVEL_RANGE(level) == 0)
	{
		return;
	}

	if (this->m_BloodCastleLevel[level].EventItemSerial != -1)
	{
		return;
	}

	if (index == GET_ITEM(13, 19))
	{
		this->m_BloodCastleLevel[level].EventItemSerial = serial;
	}
}

void CBloodCastle::DropEventItem(int map, int number, int aIndex)
{
	int level = GET_BC_LEVEL(map);

	if (BC_LEVEL_RANGE(level) == 0)
	{
		return;
	}

	gMap[map].m_Item[number].m_Time = GetTickCount() + this->m_BloodCastleLevel[level].RemainTime;

	gMap[map].m_Item[number].m_LootTime = GetTickCount();

	this->m_BloodCastleLevel[level].EventItemNumber = number;

	this->m_BloodCastleLevel[level].EventItemOwner = -1;

	this->m_BloodCastleLevel[level].EventItemLevel = -1;

	this->NoticeSendToAll(&this->m_BloodCastleLevel[level], 0, 14, gObj[aIndex].Name);
}

void CBloodCastle::GetEventItem(int map, int aIndex, CMapItem* lpItem)
{
	int level = GET_BC_LEVEL(map);

	if (BC_LEVEL_RANGE(level) == 0)
	{
		return;
	}

	this->m_BloodCastleLevel[level].EventItemNumber = -1;

	this->m_BloodCastleLevel[level].EventItemOwner = aIndex;

	this->m_BloodCastleLevel[level].EventItemLevel = lpItem->m_Level;

	this->NoticeSendToAll(&this->m_BloodCastleLevel[level], 0, 15, gObj[aIndex].Name);
}

void CBloodCastle::SetEntranceZone(BLOOD_CASTLE_LEVEL* lpLevel)
{
	for (int x = gBloodCastleEntranceZone[0]; x <= gBloodCastleEntranceZone[2]; x++)
	{
		for (int y = gBloodCastleEntranceZone[1]; y <= gBloodCastleEntranceZone[3]; y++)
		{
			gMap[lpLevel->Map].SetAttr(x, y, 4);
		}
	}

	for (int n = 0; n < MAX_BC_USER; n++)
	{
		if (OBJECT_RANGE(lpLevel->User[n].Index) != 0)
		{
			GCMapAttrSend(lpLevel->User[n].Index, 0, 4, 0, 1, (PMSG_MAP_ATTR*)gBloodCastleEntranceZone);
		}
	}
}

void CBloodCastle::DelEntranceZone(BLOOD_CASTLE_LEVEL* lpLevel)
{
	for (int x = gBloodCastleEntranceZone[0]; x <= gBloodCastleEntranceZone[2]; x++)
	{
		for (int y = gBloodCastleEntranceZone[1]; y <= gBloodCastleEntranceZone[3]; y++)
		{
			gMap[lpLevel->Map].DelAttr(x, y, 4);
		}
	}

	for (int n = 0; n < MAX_BC_USER; n++)
	{
		if (OBJECT_RANGE(lpLevel->User[n].Index) != 0)
		{
			GCMapAttrSend(lpLevel->User[n].Index, 0, 4, 1, 1, (PMSG_MAP_ATTR*)gBloodCastleEntranceZone);
		}
	}
}

void CBloodCastle::SetBridgeZone(BLOOD_CASTLE_LEVEL* lpLevel, bool send)
{
	if (send == 0)
	{
		for (int x = gBloocCastleBridgeZone[0]; x <= gBloocCastleBridgeZone[2]; x++)
		{
			for (int y = gBloocCastleBridgeZone[1]; y <= gBloocCastleBridgeZone[3]; y++)
			{
				gMap[lpLevel->Map].SetAttr(x, y, 8);
			}
		}
	}
	else
	{
		for (int n = 0; n < MAX_BC_USER; n++)
		{
			if (OBJECT_RANGE(lpLevel->User[n].Index) != 0)
			{
				GCMapAttrSend(lpLevel->User[n].Index, 0, 8, 0, 1, (PMSG_MAP_ATTR*)gBloocCastleBridgeZone);
			}
		}
	}
}

void CBloodCastle::DelBridgeZone(BLOOD_CASTLE_LEVEL* lpLevel, bool send)
{
	if (send == 0)
	{
		for (int x = gBloocCastleBridgeZone[0]; x <= gBloocCastleBridgeZone[2]; x++)
		{
			for (int y = gBloocCastleBridgeZone[1]; y <= gBloocCastleBridgeZone[3]; y++)
			{
				gMap[lpLevel->Map].DelAttr(x, y, 8);
			}
		}
	}
	else
	{
		for (int n = 0; n < MAX_BC_USER; n++)
		{
			if (OBJECT_RANGE(lpLevel->User[n].Index) != 0)
			{
				GCMapAttrSend(lpLevel->User[n].Index, 0, 8, 1, 1, (PMSG_MAP_ATTR*)gBloocCastleBridgeZone);
			}
		}
	}
}

void CBloodCastle::SetGateZone(BLOOD_CASTLE_LEVEL* lpLevel)
{
	for (int n = 0; n < 3; n++)
	{
		for (int x = gBloodCastleGateZone[n][0]; x <= gBloodCastleGateZone[n][2]; x++)
		{
			for (int y = gBloodCastleGateZone[n][1]; y <= gBloodCastleGateZone[n][3]; y++)
			{
				gMap[lpLevel->Map].SetAttr(x, y, 4);
			}
		}
	}

	for (int n = 0; n < MAX_BC_USER; n++)
	{
		if (OBJECT_RANGE(lpLevel->User[n].Index) != 0)
		{
			GCMapAttrSend(lpLevel->User[n].Index, 0, 4, 0, 3, (PMSG_MAP_ATTR*)gBloodCastleGateZone);
		}
	}
}

void CBloodCastle::DelGateZone(BLOOD_CASTLE_LEVEL* lpLevel)
{
	for (int n = 0; n < 3; n++)
	{
		for (int x = gBloodCastleGateZone[n][0]; x <= gBloodCastleGateZone[n][2]; x++)
		{
			for (int y = gBloodCastleGateZone[n][1]; y <= gBloodCastleGateZone[n][3]; y++)
			{
				gMap[lpLevel->Map].DelAttr(x, y, 4);
			}
		}
	}

	for (int n = 0; n < MAX_BC_USER; n++)
	{
		if (OBJECT_RANGE(lpLevel->User[n].Index) != 0)
		{
			GCMapAttrSend(lpLevel->User[n].Index, 0, 4, 1, 3, (PMSG_MAP_ATTR*)gBloodCastleGateZone);
		}
	}
}

void CBloodCastle::UserDieProc(LPOBJ lpObj, LPOBJ lpTarget)
{
	int level = GET_BC_LEVEL(lpObj->Map);

	if (BC_LEVEL_RANGE(level) == 0)
	{
		return;
	}

	if (this->GetState(level) != BC_STATE_START)
	{
		return;
	}

	if (this->GetUser(&this->m_BloodCastleLevel[level], lpObj->Index) == 0)
	{
		return;
	}

	this->GiveUserRewardExperience(&this->m_BloodCastleLevel[level], lpObj->Index);

	this->GiveUserRewardMoney(&this->m_BloodCastleLevel[level], lpObj->Index);

	this->GCBloodCastleDelayScoreSend(&this->m_BloodCastleLevel[level], lpObj->Index, 0);

	this->DelUser(&this->m_BloodCastleLevel[level], lpObj->Index);
}

void CBloodCastle::MonsterDieProc(LPOBJ lpObj, LPOBJ lpTarget)
{
	int aIndex = gObjMonsterGetTopHitDamageUser(lpObj);

	if (OBJECT_RANGE(aIndex) != 0)
	{
		lpTarget = &gObj[aIndex];
	}

	int level = GET_BC_LEVEL(lpObj->Map);

	if (BC_LEVEL_RANGE(level) == 0)
	{
		return;
	}

	if (this->GetState(level) != BC_STATE_START)
	{
		return;
	}

	if (lpObj->Class == 131 && lpObj->Index == this->m_BloodCastleLevel[level].GateIndex && this->m_BloodCastleLevel[level].Stage == 2)
	{
		this->m_BloodCastleLevel[level].GateIndex = -1;

		this->NoticeSendToAll(&this->m_BloodCastleLevel[level], 0, 16, lpTarget->Name);

		return;
	}

	if (lpObj->Class >= 132 && lpObj->Class <= 134 && lpObj->Index == this->m_BloodCastleLevel[level].SaintStatueIndex && this->m_BloodCastleLevel[level].Stage == 4)
	{
		GDCreateItemSend(lpTarget->Index, lpObj->Map, (BYTE)lpObj->X, (BYTE)lpObj->Y, GET_ITEM(13, 19), (lpObj->Class - 132), 0, 0, 0, 0, lpTarget->Index, 0);

		this->m_BloodCastleLevel[level].SaintStatueIndex = -1;

		this->NoticeSendToAll(&this->m_BloodCastleLevel[level], 0, 17, lpTarget->Name);

		return;
	}

	if (this->GetMonster(&this->m_BloodCastleLevel[level], lpObj->Index) == 0)
	{
		return;
	}

	if (lpTarget->Type != OBJECT_USER)
	{
		return;
	}

	BLOOD_CASTLE_USER* lpUser = this->GetUser(&this->m_BloodCastleLevel[level], lpTarget->Index);

	if (lpUser == 0)
	{
		return;
	}

	if (lpObj->Class == 89 || lpObj->Class == 95 || lpObj->Class == 112 || lpObj->Class == 118 || lpObj->Class == 124 || lpObj->Class == 130)
	{
		if (this->m_BloodCastleLevel[level].Stage == 3 && this->m_BloodCastleLevel[level].CurMonster < this->m_BloodCastleLevel[level].MaxMonster)
		{
			this->m_BloodCastleLevel[level].CurMonster++;
		}
	}
	else
	{
		if (this->m_BloodCastleLevel[level].Stage == 0 && this->m_BloodCastleLevel[level].CurMonster < this->m_BloodCastleLevel[level].MaxMonster)
		{
			this->m_BloodCastleLevel[level].CurMonster++;
		}
	}
}

void CBloodCastle::CheckDelayScore(BLOOD_CASTLE_LEVEL* lpLevel)
{
	for (std::vector<BLOOD_CASTLE_DELAY_SCORE>::iterator it = lpLevel->DelayScore.begin(); it != lpLevel->DelayScore.end();)
	{
		if ((GetTickCount() - it->Delay) < 8000)
		{
			it++;
			continue;
		}

		if (gObjIsConnected(it->Index) != 0)
		{
			DataSend(it->Index, (BYTE*)&it->Score, it->Score.header.size);
		}

		it = lpLevel->DelayScore.erase(it);
	}
}

void CBloodCastle::NoticeSendToAll(BLOOD_CASTLE_LEVEL* lpLevel, int type, int message, ...)
{
	for (int n = 0; n < MAX_BC_USER; n++)
	{
		if (OBJECT_RANGE(lpLevel->User[n].Index) != 0)
		{
			char buff[256];

			va_list arg;

			va_start(arg, message);

			vsprintf_s(buff, gMessage.GetTextMessage(message, gObj[lpLevel->User[n].Index].Lang), arg);

			va_end(arg);

			gNotice.GCNoticeSend(lpLevel->User[n].Index, type, buff);
		}
	}
}

void CBloodCastle::DataSendToAll(BLOOD_CASTLE_LEVEL* lpLevel, BYTE* lpMsg, int size)
{
	for (int n = 0; n < MAX_BC_USER; n++)
	{
		if (OBJECT_RANGE(lpLevel->User[n].Index) != 0)
		{
			DataSend(lpLevel->User[n].Index, lpMsg, size);
		}
	}
}

void CBloodCastle::CGBloodCastleEnterRecv(PMSG_BLOOD_CASTLE_ENTER_RECV* lpMsg, int aIndex)
{
	if (gServerInfo.m_BloodCastleEvent == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_BLOOD_CASTLE_ENTER_SEND pMsg;

	pMsg.header.set(0x9A, sizeof(pMsg));

	pMsg.result = 0;

	lpMsg->level = lpMsg->level - 1;

	if (BC_LEVEL_RANGE(lpMsg->level) == 0)
	{
		pMsg.result = 1;

		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	BLOOD_CASTLE_LEVEL* lpLevel = &this->m_BloodCastleLevel[lpMsg->level];

	if (INVENTORY_RANGE(lpMsg->slot) == 0)
	{
		pMsg.result = 1;

		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (lpObj->Inventory[lpMsg->slot].m_Index != GET_ITEM(13, 18) && lpObj->Inventory[lpMsg->slot].m_Index != GET_ITEM(13, 47))
	{
		pMsg.result = 1;

		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (lpObj->Inventory[lpMsg->slot].m_Index == GET_ITEM(13, 18) && lpObj->Inventory[lpMsg->slot].m_Level != (lpMsg->level + 1))
	{
		pMsg.result = 1;

		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (this->GetEnterEnabled(lpMsg->level) == 0)
	{
		pMsg.result = 2;

		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (this->GetUserAbleLevel(lpObj) > lpMsg->level)
	{
		pMsg.result = 3;

		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (this->GetUserAbleLevel(lpObj) < lpMsg->level)
	{
		pMsg.result = 4;

		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (this->GetUserCount(lpLevel) >= gServerInfo.m_BloodCastleMaxUser)
	{
		pMsg.result = 5;

		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (lpObj->PKLevel >= PKLVL_OUTLAW && gServerInfo.m_BloodCastleAllowPK == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(66, lpObj->Lang));

		pMsg.result = 6;

		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (this->AddUser(lpLevel, aIndex) == 0)
	{
		pMsg.result = 5;

		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	gItemManager.DecreaseItemDur(lpObj, lpMsg->slot, 1);

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

	lpObj->Interface.use = 0;

	lpObj->Interface.type = INTERFACE_NONE;

	lpObj->Interface.state = 0;

	switch (lpMsg->level)
	{
		case 0:
		{
			gObjMoveGate(aIndex, 66);

			break;
		}

		case 1:
		{
			gObjMoveGate(aIndex, 67);

			break;
		}

		case 2:
		{
			gObjMoveGate(aIndex, 68);

			break;
		}

		case 3:
		{
			gObjMoveGate(aIndex, 69);

			break;
		}

		case 4:
		{
			gObjMoveGate(aIndex, 70);

			break;
		}

		case 5:
		{
			gObjMoveGate(aIndex, 71);

			break;
		}
	}
}

void CBloodCastle::GCBloodCastleScoreSend(BLOOD_CASTLE_LEVEL* lpLevel, int aIndex, int type)
{
	BLOOD_CASTLE_USER* lpUser = this->GetUser(lpLevel, aIndex);

	if (lpUser == 0)
	{
		return;
	}

	PMSG_BLOOD_CASTLE_SCORE_SEND pMsg;

	pMsg.header.set(0x93, sizeof(pMsg));

	pMsg.type = type;

	pMsg.flag = 0xFF;

	memcpy(pMsg.name, gObj[aIndex].Name, sizeof(pMsg.name));

	pMsg.Score = lpUser->Score;

	pMsg.RewardExperience = lpUser->RewardExperience;

	pMsg.RewardMoney = lpUser->RewardMoney;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CBloodCastle::GCBloodCastleDelayScoreSend(BLOOD_CASTLE_LEVEL* lpLevel, int aIndex, int type)
{
	BLOOD_CASTLE_USER* lpUser = this->GetUser(lpLevel, aIndex);

	if (lpUser == 0)
	{
		return;
	}

	BLOOD_CASTLE_DELAY_SCORE DelayScore;

	DelayScore.Delay = GetTickCount();

	DelayScore.Index = lpUser->Index;

	DelayScore.Score.header.set(0x93, sizeof(DelayScore.Score));

	DelayScore.Score.type = type;

	DelayScore.Score.flag = 0xFF;

	memcpy(DelayScore.Score.name, gObj[aIndex].Name, sizeof(DelayScore.Score.name));

	DelayScore.Score.Score = lpUser->Score;

	DelayScore.Score.RewardExperience = lpUser->RewardExperience;

	DelayScore.Score.RewardMoney = lpUser->RewardMoney;

	lpLevel->DelayScore.push_back(DelayScore);
}

void CBloodCastle::GCBloodCastleStateSend(BLOOD_CASTLE_LEVEL* lpLevel, int state)
{
	PMSG_BLOOD_CASTLE_STATE_SEND pMsg;

	pMsg.header.set(0x9B, sizeof(pMsg));

	pMsg.state = state;

	pMsg.time = lpLevel->RemainTime;

	pMsg.MaxMonster = lpLevel->MaxMonster;

	pMsg.CurMonster = lpLevel->CurMonster;

	pMsg.EventItemOwner = lpLevel->EventItemOwner;

	pMsg.EventItemLevel = ((lpLevel->EventItemLevel == 0xFF) ? 0xFF : (lpLevel->EventItemLevel + 1));

	this->DataSendToAll(lpLevel, (BYTE*)&pMsg, pMsg.header.size);
}

void CBloodCastle::GCBloodCastleSimpleStateSend(BLOOD_CASTLE_LEVEL* lpLevel, int state, int aIndex)
{
	PMSG_BLOOD_CASTLE_STATE_SEND pMsg;

	pMsg.header.set(0x9B, sizeof(pMsg));

	pMsg.state = state;

	pMsg.time = 0;

	pMsg.MaxMonster = 0;

	pMsg.CurMonster = 0;

	pMsg.EventItemOwner = 0;

	pMsg.EventItemLevel = 0xFF;

	if (OBJECT_RANGE(aIndex) == 0)
	{
		this->DataSendToAll(lpLevel, (BYTE*)&pMsg, pMsg.header.size);
	}
	else
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
	}
}

void CBloodCastle::StartBC()
{
	time_t theTime = time(NULL);

	tm aTime;

	localtime_s(&aTime, &theTime);

	int hour = aTime.tm_hour;

	int minute = aTime.tm_min + 2;

	if (minute >= 60)
	{
		hour++;

		minute = minute - 60;
	}

	BLOOD_CASTLE_START_TIME info;

	info.Year = -1;

	info.Month = -1;

	info.Day = -1;

	info.DayOfWeek = -1;

	info.Hour = hour;

	info.Minute = minute;

	info.Second = 0;

	this->m_BloodCastleStartTime.push_back(info);

	LogAdd(LOG_EVENT, "[Set BC Start] At %2d:%2d:00", hour, minute);

	this->Init();
}

void CBloodCastle::GCRequiredLevelsSend(int aIndex)
{
	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_BLOOD_CASTLE_REQ_LEVELS_SEND pMsg;

	pMsg.header.set(0x8F, sizeof(pMsg));

	memcpy(pMsg.m_BloodCastleRequiredLevel, this->m_BloodCastleRequiredLevel, sizeof(pMsg.m_BloodCastleRequiredLevel));

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}