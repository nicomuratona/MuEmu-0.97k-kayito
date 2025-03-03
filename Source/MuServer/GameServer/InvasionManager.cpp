#include "stdafx.h"
#include "InvasionManager.h"
#include "FlyingDragons.h"
#include "ReadScript.h"
#include "Monster.h"
#include "MonsterSetBase.h"
#include "Notice.h"
#include "resource.h"
#include "ScheduleManager.h"
#include "ServerInfo.h"
#include "Util.h"

CInvasionManager gInvasionManager;

CInvasionManager::CInvasionManager()
{
	for (int n = 0; n < MAX_INVASION; n++)
	{
		INVASION_INFO* lpInfo = &this->m_InvasionInfo[n];

		lpInfo->Index = n;

		lpInfo->State = INVASION_STATE_BLANK;

		lpInfo->RemainTime = 0;

		lpInfo->TargetTime = 0;

		lpInfo->TickCount = GetTickCount();

		this->CleanMonster(lpInfo);
	}
}

CInvasionManager::~CInvasionManager()
{

}

void CInvasionManager::Init()
{
	for (int n = 0; n < MAX_INVASION; n++)
	{
		if (gServerInfo.m_InvasionManagerSwitch == 0)
		{
			this->SetState(&this->m_InvasionInfo[n], INVASION_STATE_BLANK);
		}
		else
		{
			this->SetState(&this->m_InvasionInfo[n], INVASION_STATE_EMPTY);
		}
	}

	gFlyingDragons.Init();
}

void CInvasionManager::Load(char* path)
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

	for (int n = 0; n < MAX_INVASION; n++)
	{
		this->m_InvasionInfo[n].RespawnMessage = -1;

		this->m_InvasionInfo[n].DespawnMessage = -1;

		this->m_InvasionInfo[n].BossIndex = -1;

		this->m_InvasionInfo[n].BossMessage = -1;

		this->m_InvasionInfo[n].InvasionTime = 0;

		this->m_InvasionInfo[n].WarningTime = 0;

		this->m_InvasionInfo[n].WarningMsg = -1;

		this->m_InvasionInfo[n].StartTime.clear();

		this->m_InvasionInfo[n].RespawnInfo[0].clear();

		this->m_InvasionInfo[n].RespawnInfo[1].clear();

		this->m_InvasionInfo[n].RespawnInfo[2].clear();

		this->m_InvasionInfo[n].RespawnInfo[3].clear();

		this->m_InvasionInfo[n].RespawnInfo[4].clear();

		this->m_InvasionInfo[n].RespawnInfo[5].clear();

		this->m_InvasionInfo[n].RespawnInfo[6].clear();

		this->m_InvasionInfo[n].RespawnInfo[7].clear();

		this->m_InvasionInfo[n].RespawnInfo[8].clear();

		this->m_InvasionInfo[n].RespawnInfo[9].clear();

		this->m_InvasionInfo[n].RespawnInfo[10].clear();

		this->m_InvasionInfo[n].RespawnInfo[11].clear();

		this->m_InvasionInfo[n].RespawnInfo[12].clear();

		this->m_InvasionInfo[n].RespawnInfo[13].clear();

		this->m_InvasionInfo[n].RespawnInfo[14].clear();

		this->m_InvasionInfo[n].RespawnInfo[15].clear();

		this->m_InvasionInfo[n].RespawnInfo[16].clear();

		this->m_InvasionInfo[n].RespawnInfo[17].clear();

		this->m_InvasionInfo[n].RespawnInfo[18].clear();

		this->m_InvasionInfo[n].RespawnInfo[19].clear();

		this->m_InvasionInfo[n].MonsterInfo.clear();
	}

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
					INVASION_START_TIME info;

					int index = lpReadScript->GetNumber();

					info.Year = lpReadScript->GetAsNumber();

					info.Month = lpReadScript->GetAsNumber();

					info.Day = lpReadScript->GetAsNumber();

					info.DayOfWeek = lpReadScript->GetAsNumber();

					info.Hour = lpReadScript->GetAsNumber();

					info.Minute = lpReadScript->GetAsNumber();

					info.Second = lpReadScript->GetAsNumber();

					this->m_InvasionInfo[index].StartTime.push_back(info);
				}
				else if (section == 1)
				{
					int index = lpReadScript->GetNumber();

					this->m_InvasionInfo[index].RespawnMessage = lpReadScript->GetAsNumber();

					this->m_InvasionInfo[index].DespawnMessage = lpReadScript->GetAsNumber();

					this->m_InvasionInfo[index].BossIndex = lpReadScript->GetAsNumber();

					this->m_InvasionInfo[index].BossMessage = lpReadScript->GetAsNumber();

					this->m_InvasionInfo[index].InvasionTime = lpReadScript->GetAsNumber();

					this->m_InvasionInfo[index].WarningTime = lpReadScript->GetAsNumber();

					this->m_InvasionInfo[index].WarningMsg = lpReadScript->GetAsNumber();

					this->m_InvasionInfo[index].InvasionEffect = lpReadScript->GetAsNumber();

					strcpy_s(this->m_InvasionInfo[index].InvasionName, lpReadScript->GetAsString());

					CreateSubMenuItem(ID_STARTINVASION, index, this->m_InvasionInfo[index].InvasionName);
				}
				else if (section == 2)
				{
					INVASION_RESPWAN_INFO info;

					int index = lpReadScript->GetNumber();

					info.Group = lpReadScript->GetAsNumber();

					info.Map = lpReadScript->GetAsNumber();

					info.Value = lpReadScript->GetAsNumber();

					this->m_InvasionInfo[index].RespawnInfo[info.Group].push_back(info);
				}
				else if (section == 3)
				{
					INVASION_MONSTER_INFO info;

					int index = lpReadScript->GetNumber();

					info.Group = lpReadScript->GetAsNumber();

					info.MonsterClass = lpReadScript->GetAsNumber();

					info.RegenType = lpReadScript->GetAsNumber();

					info.RegenTime = lpReadScript->GetAsNumber();

					this->m_InvasionInfo[index].MonsterInfo.push_back(info);
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

void CInvasionManager::MainProc()
{
	for (int n = 0; n < MAX_INVASION; n++)
	{
		INVASION_INFO* lpInfo = &this->m_InvasionInfo[n];

		DWORD elapsed = GetTickCount() - lpInfo->TickCount;

		if (elapsed < 1000)
		{
			continue;
		}

		lpInfo->TickCount = GetTickCount();

		lpInfo->RemainTime = (int)difftime(lpInfo->TargetTime, time(0));

		switch (lpInfo->State)
		{
			case INVASION_STATE_BLANK:
			{
				this->ProcState_BLANK(lpInfo);

				break;
			}

			case INVASION_STATE_EMPTY:
			{
				this->ProcState_EMPTY(lpInfo);

				break;
			}

			case INVASION_STATE_START:
			{
				this->ProcState_START(lpInfo);

				break;
			}
		}
	}
}

void CInvasionManager::ProcState_BLANK(INVASION_INFO* lpInfo)
{

}

void CInvasionManager::ProcState_EMPTY(INVASION_INFO* lpInfo)
{
	if (lpInfo->RemainTime > 0 && lpInfo->RemainTime <= (lpInfo->WarningTime * 60))
	{
		int minutes = lpInfo->RemainTime / 60;

		if ((lpInfo->RemainTime % 60) == 0)
		{
			minutes--;
		}

		if (lpInfo->MinutesLeft != minutes)
		{
			lpInfo->MinutesLeft = minutes;

			gNotice.GCNoticeSendToAll(0, lpInfo->WarningMsg, lpInfo->InvasionName, (lpInfo->MinutesLeft + 1));
		}
	}

	if (lpInfo->RemainTime <= 0)
	{
		if (lpInfo->RespawnMessage != -1)
		{
			gNotice.GCNoticeSendToAll(0, lpInfo->RespawnMessage, lpInfo->InvasionName);
		}

		this->SetState(lpInfo, INVASION_STATE_START);
	}
}

void CInvasionManager::ProcState_START(INVASION_INFO* lpInfo)
{
	if (lpInfo->RemainTime <= 0)
	{
		if (lpInfo->DespawnMessage != -1)
		{
			gNotice.GCNoticeSendToAll(0, lpInfo->DespawnMessage, lpInfo->InvasionName);
		}

		this->SetState(lpInfo, INVASION_STATE_EMPTY);
	}
}

void CInvasionManager::SetState(INVASION_INFO* lpInfo, int state)
{
	lpInfo->State = state;

	switch (lpInfo->State)
	{
		case INVASION_STATE_BLANK:
		{
			this->SetState_BLANK(lpInfo);

			break;
		}

		case INVASION_STATE_EMPTY:
		{
			this->SetState_EMPTY(lpInfo);

			break;
		}

		case INVASION_STATE_START:
		{
			this->SetState_START(lpInfo);

			break;
		}
	}
}

void CInvasionManager::SetState_BLANK(INVASION_INFO* lpInfo)
{

}

void CInvasionManager::SetState_EMPTY(INVASION_INFO* lpInfo)
{
	this->ClearMonster(lpInfo);

	this->CheckSync(lpInfo);
}

void CInvasionManager::SetState_START(INVASION_INFO* lpInfo)
{
	for (int n = 0; n < MAX_INVASION_RESPAWN_GROUP; n++)
	{
		if (lpInfo->RespawnInfo[n].empty() == 0)
		{
			INVASION_RESPWAN_INFO* lpRespawnInfo = &lpInfo->RespawnInfo[n][(GetLargeRand() % lpInfo->RespawnInfo[n].size())];

			for (std::vector<INVASION_MONSTER_INFO>::iterator it = lpInfo->MonsterInfo.begin(); it != lpInfo->MonsterInfo.end(); it++)
			{
				if (it->Group == lpRespawnInfo->Group)
				{
					this->SetMonster(lpInfo, lpRespawnInfo, &(*it));
				}
			}
		}
	}

	lpInfo->RemainTime = lpInfo->InvasionTime * 60;

	lpInfo->TargetTime = (int)(time(0) + lpInfo->RemainTime);
}

void CInvasionManager::CheckSync(INVASION_INFO* lpInfo)
{
	if (lpInfo->StartTime.empty() != 0)
	{
		this->SetState(lpInfo, INVASION_STATE_BLANK);

		return;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for (std::vector<INVASION_START_TIME>::iterator it = lpInfo->StartTime.begin(); it != lpInfo->StartTime.end(); it++)
	{
		ScheduleManager.AddSchedule(it->Year, it->Month, it->Day, it->Hour, it->Minute, it->Second, it->DayOfWeek);
	}

	if (ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(lpInfo, INVASION_STATE_BLANK);

		return;
	}

	lpInfo->RemainTime = (int)difftime(ScheduleTime.GetTime(), time(0));

	lpInfo->TargetTime = (int)ScheduleTime.GetTime();
}

int CInvasionManager::GetState(int index)
{
	if (index < 0 || index >= MAX_INVASION)
	{
		return INVASION_STATE_BLANK;
	}

	return this->m_InvasionInfo[index].State;
}

char* CInvasionManager::GetInvasionName(int index)
{
	if (index < 0 || index >= MAX_INVASION)
	{
		return NULL;
	}

	return this->m_InvasionInfo[index].InvasionName;
}

int CInvasionManager::GetCurrentRemainTime(int index)
{
	if (index < 0 || index >= MAX_INVASION)
	{
		return 0;
	}

	return this->m_InvasionInfo[index].RemainTime;
}

int CInvasionManager::GetRemainTime(int index)
{
	if (index < 0 || index >= MAX_INVASION)
	{
		return 0;
	}

	INVASION_INFO* lpInfo = &this->m_InvasionInfo[index];

	if (lpInfo->StartTime.empty() != 0)
	{
		return 0;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for (std::vector<INVASION_START_TIME>::iterator it = lpInfo->StartTime.begin(); it != lpInfo->StartTime.end(); it++)
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

bool CInvasionManager::AddMonster(INVASION_INFO* lpInfo, int aIndex)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	if (this->GetMonster(lpInfo, aIndex) != 0)
	{
		return 0;
	}

	for (int n = 0; n < MAX_INVASION_MONSTER; n++)
	{
		if (OBJECT_RANGE(lpInfo->MonsterIndex[n]) == 0)
		{
			lpInfo->MonsterIndex[n] = aIndex;

			return 1;
		}
	}

	return 0;
}

bool CInvasionManager::DelMonster(INVASION_INFO* lpInfo, int aIndex)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	int* index = this->GetMonster(lpInfo, aIndex);

	if (index == 0)
	{
		return 0;
	}

	(*index) = -1;

	return 1;
}

int* CInvasionManager::GetMonster(INVASION_INFO* lpInfo, int aIndex)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	for (int n = 0; n < MAX_INVASION_MONSTER; n++)
	{
		if (lpInfo->MonsterIndex[n] == aIndex)
		{
			return &lpInfo->MonsterIndex[n];
		}
	}

	return 0;
}

void CInvasionManager::CleanMonster(INVASION_INFO* lpInfo)
{
	for (int n = 0; n < MAX_INVASION_MONSTER; n++)
	{
		lpInfo->MonsterIndex[n] = -1;
	}
}

void CInvasionManager::ClearMonster(INVASION_INFO* lpInfo)
{
	for (int n = 0; n < MAX_INVASION_MONSTER; n++)
	{
		if (OBJECT_RANGE(lpInfo->MonsterIndex[n]) != 0)
		{
			gObjDel(lpInfo->MonsterIndex[n]);

			lpInfo->MonsterIndex[n] = -1;
		}
	}
}

int CInvasionManager::GetMonsterCount(INVASION_INFO* lpInfo)
{
	int count = 0;

	for (int n = 0; n < MAX_INVASION_MONSTER; n++)
	{
		if (OBJECT_RANGE(lpInfo->MonsterIndex[n]) != 0)
		{
			count++;
		}
	}

	return count;
}

void CInvasionManager::SetMonster(INVASION_INFO* lpInfo, INVASION_RESPWAN_INFO* lpRespawnInfo, INVASION_MONSTER_INFO* lpMonsterInfo)
{
	for (int n = 0; n < gMonsterSetBase.m_count; n++)
	{
		MONSTER_SET_BASE_INFO* lpMsbInfo = &gMonsterSetBase.m_MonsterSetBaseInfo[n];

		if (lpMsbInfo->Type != 3 || lpMsbInfo->MonsterClass != lpMonsterInfo->MonsterClass || lpMsbInfo->Map != lpRespawnInfo->Map || lpMsbInfo->Value != lpRespawnInfo->Value)
		{
			continue;
		}

		int index = gObjAddMonster(lpMsbInfo->Map);

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

		if (gObjSetMonster(index, lpMsbInfo->MonsterClass) == 0)
		{
			gObjDel(index);

			continue;
		}

		lpObj->MaxRegenTime = ((lpMonsterInfo->RegenType == 0) ? ((lpInfo->InvasionTime * 60) * 1000) : lpMonsterInfo->RegenTime * 1000);

		if (this->AddMonster(lpInfo, index) == 0)
		{
			gObjDel(index);

			continue;
		}

		if (lpObj->Class == lpInfo->BossIndex)
		{
			LogAdd(LOG_EVENT, "[Invasion Manager] (%s) Boss Position (Map: %d, X: %d, Y: %d", lpInfo->InvasionName, lpObj->Map, lpObj->X, lpObj->Y);
		}

		if (gServerInfo.m_FlyingDragonsOnlyBossMapSpawn != 0)
		{
			if (lpObj->Class == lpInfo->BossIndex)
			{
				gFlyingDragons.FlyingDragonsAdd(lpObj->Map, lpInfo->InvasionTime * 60, lpInfo->InvasionEffect);
			}
		}
		else
		{
			gFlyingDragons.FlyingDragonsAdd(lpObj->Map, lpInfo->InvasionTime * 60, lpInfo->InvasionEffect);
		}
	}
}

void CInvasionManager::MonsterDieProc(LPOBJ lpObj, LPOBJ lpTarget)
{
	int aIndex = gObjMonsterGetTopHitDamageUser(lpObj);

	if (OBJECT_RANGE(aIndex) != 0)
	{
		lpTarget = &gObj[aIndex];
	}

	for (int n = 0; n < MAX_INVASION; n++)
	{
		INVASION_INFO* lpInfo = &this->m_InvasionInfo[n];

		if (this->GetState(lpInfo->Index) != INVASION_STATE_START)
		{
			continue;
		}

		if (this->GetMonster(lpInfo, lpObj->Index) == 0)
		{
			continue;
		}

		if (lpObj->Class == lpInfo->BossIndex)
		{
			if (gServerInfo.m_FlyingDragonsKillBossDisappear != 0)
			{
				gFlyingDragons.FlyingDragonsBossDieProc(lpObj->Map);
			}

			if (lpInfo->BossMessage != -1)
			{
				gNotice.GCNoticeSendToAll(0, lpInfo->BossMessage, lpTarget->Name, lpObj->Name);
			}
		}
	}
}

void CInvasionManager::StartInvasion(int InvasionIndex)
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

	INVASION_START_TIME info;

	info.Year = -1;

	info.Month = -1;

	info.Day = -1;

	info.DayOfWeek = -1;

	info.Hour = hour;

	info.Minute = minute;

	info.Second = 0;

	this->m_InvasionInfo[InvasionIndex].StartTime.push_back(info);

	LogAdd(LOG_EVENT, "[Set Invasion Start][%s] At %2d:%2d:00", this->m_InvasionInfo[InvasionIndex].InvasionName, hour, minute);

	this->Init();
}