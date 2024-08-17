#include "stdafx.h"
#include "BonusManager.h"
#include "DSProtocol.h"
#include "ItemDrop.h"
#include "ItemManager.h"
#include "ItemOptionRate.h"
#include "ReadScript.h"
#include "Message.h"
#include "Monster.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "ScheduleManager.h"
#include "ServerInfo.h"
#include "Util.h"

CBonusManager gBonusManager;

CBonusManager::CBonusManager()
{
	for (int n = 0; n < MAX_BONUS; n++)
	{
		BONUS_INFO* lpInfo = &this->m_BonusInfo[n];

		lpInfo->Index = n;

		lpInfo->State = BONUS_STATE_BLANK;

		lpInfo->RemainTime = 0;

		lpInfo->TargetTime = 0;

		lpInfo->TickCount = GetTickCount();

		lpInfo->StartTime.clear();

		lpInfo->ValueInfo.clear();
	}
}

CBonusManager::~CBonusManager()
{

}

void CBonusManager::Init()
{
	for (int n = 0; n < MAX_BONUS; n++)
	{
		if (gServerInfo.m_BonusManagerSwitch == 0)
		{
			this->SetState(&this->m_BonusInfo[n], BONUS_STATE_BLANK);
		}
		else
		{
			this->SetState(&this->m_BonusInfo[n], BONUS_STATE_EMPTY);
		}
	}
}

void CBonusManager::Load(char* path)
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

	for (int n = 0; n < MAX_BONUS; n++)
	{
		this->m_BonusInfo[n].StartMessage = -1;

		this->m_BonusInfo[n].FinalMessage = -1;

		this->m_BonusInfo[n].BonusTime = 0;

		this->m_BonusInfo[n].AlarmTime = 0;

		this->m_BonusInfo[n].AlarmMsg = -1;

		this->m_BonusInfo[n].StartTime.clear();

		this->m_BonusInfo[n].ValueInfo.clear();
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
					BONUS_START_TIME info;

					int index = lpReadScript->GetNumber();

					info.Year = lpReadScript->GetAsNumber();

					info.Month = lpReadScript->GetAsNumber();

					info.Day = lpReadScript->GetAsNumber();

					info.DayOfWeek = lpReadScript->GetAsNumber();

					info.Hour = lpReadScript->GetAsNumber();

					info.Minute = lpReadScript->GetAsNumber();

					info.Second = lpReadScript->GetAsNumber();

					this->m_BonusInfo[index].StartTime.push_back(info);
				}
				else if (section == 1)
				{
					int index = lpReadScript->GetNumber();

					this->m_BonusInfo[index].StartMessage = lpReadScript->GetAsNumber();

					this->m_BonusInfo[index].FinalMessage = lpReadScript->GetAsNumber();

					this->m_BonusInfo[index].BonusTime = lpReadScript->GetAsNumber();

					this->m_BonusInfo[index].AlarmTime = lpReadScript->GetAsNumber();

					this->m_BonusInfo[index].AlarmMsg = lpReadScript->GetAsNumber();

					strcpy_s(this->m_BonusInfo[index].BonusName, lpReadScript->GetAsString());
				}
				else if (section == 2)
				{
					BONUS_VALUE_INFO info;

					int index = lpReadScript->GetNumber();

					info.BonusIndex = lpReadScript->GetAsNumber();

					info.BonusValue[0] = lpReadScript->GetAsNumber();

					info.BonusValue[1] = lpReadScript->GetAsNumber();

					info.BonusValue[2] = lpReadScript->GetAsNumber();

					info.BonusValue[3] = lpReadScript->GetAsNumber();

					info.ItemIndex = lpReadScript->GetAsNumber();

					if (info.ItemIndex != -1)
					{
						info.ItemIndex = SafeGetItem(GET_ITEM(info.ItemIndex, lpReadScript->GetAsNumber()));
					}

					info.ItemLevel = lpReadScript->GetAsNumber();

					info.MapNumber = lpReadScript->GetAsNumber();

					info.MonsterClass = lpReadScript->GetAsNumber();

					info.MonsterLevelMin = lpReadScript->GetAsNumber();

					info.MonsterLevelMax = lpReadScript->GetAsNumber();

					this->m_BonusInfo[index].ValueInfo.push_back(info);
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

void CBonusManager::MainProc()
{
	for (int n = 0; n < MAX_BONUS; n++)
	{
		BONUS_INFO* lpInfo = &this->m_BonusInfo[n];

		DWORD elapsed = GetTickCount() - lpInfo->TickCount;

		if (elapsed >= 1000)
		{
			lpInfo->TickCount = GetTickCount();

			lpInfo->RemainTime = (int)difftime(lpInfo->TargetTime, time(0));

			switch (lpInfo->State)
			{
				case BONUS_STATE_BLANK:
				{
					this->ProcState_BLANK(lpInfo);

					break;
				}

				case BONUS_STATE_EMPTY:
				{
					this->ProcState_EMPTY(lpInfo);

					break;
				}

				case BONUS_STATE_START:
				{
					this->ProcState_START(lpInfo);

					break;
				}
			}
		}
	}
}

void CBonusManager::ProcState_BLANK(BONUS_INFO* lpInfo)
{

}

void CBonusManager::ProcState_EMPTY(BONUS_INFO* lpInfo)
{
	if (lpInfo->RemainTime > 0 && lpInfo->RemainTime <= (lpInfo->AlarmTime * 60))
	{
		int minutes = lpInfo->RemainTime / 60;

		if ((lpInfo->RemainTime % 60) == 0)
		{
			minutes--;
		}

		if (lpInfo->AlarmMinLeft != minutes)
		{
			lpInfo->AlarmMinLeft = minutes;

			gNotice.GCNoticeSendToAll(0, lpInfo->AlarmMsg, lpInfo->BonusName, (lpInfo->AlarmMinLeft + 1));
		}
	}

	if (lpInfo->RemainTime <= 0)
	{
		if (lpInfo->StartMessage != -1)
		{
			gNotice.GCNoticeSendToAll(0, lpInfo->StartMessage, lpInfo->BonusName);
		}

		this->SetState(lpInfo, BONUS_STATE_START);
	}
}

void CBonusManager::ProcState_START(BONUS_INFO* lpInfo)
{
	if (lpInfo->RemainTime <= 0)
	{
		if (lpInfo->FinalMessage != -1)
		{
			gNotice.GCNoticeSendToAll(0, lpInfo->FinalMessage, lpInfo->BonusName);
		}

		this->SetState(lpInfo, BONUS_STATE_EMPTY);
	}
}

void CBonusManager::SetState(BONUS_INFO* lpInfo, int state)
{
	switch ((lpInfo->State = state))
	{
		case BONUS_STATE_BLANK:
		{
			this->SetState_BLANK(lpInfo);

			break;
		}

		case BONUS_STATE_EMPTY:
		{
			this->SetState_EMPTY(lpInfo);

			break;
		}

		case BONUS_STATE_START:
		{
			this->SetState_START(lpInfo);

			break;
		}
	}
}

void CBonusManager::SetState_BLANK(BONUS_INFO* lpInfo)
{

}

void CBonusManager::SetState_EMPTY(BONUS_INFO* lpInfo)
{
	this->CheckSync(lpInfo);
}

void CBonusManager::SetState_START(BONUS_INFO* lpInfo)
{
	lpInfo->RemainTime = lpInfo->BonusTime;

	lpInfo->TargetTime = (int)(time(0) + lpInfo->RemainTime);
}

void CBonusManager::CheckSync(BONUS_INFO* lpInfo)
{
	if (lpInfo->StartTime.empty() != 0)
	{
		this->SetState(lpInfo, BONUS_STATE_BLANK);

		return;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for (std::vector<BONUS_START_TIME>::iterator it = lpInfo->StartTime.begin(); it != lpInfo->StartTime.end(); it++)
	{
		ScheduleManager.AddSchedule(it->Year, it->Month, it->Day, it->Hour, it->Minute, it->Second, it->DayOfWeek);
	}

	if (ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(lpInfo, BONUS_STATE_BLANK);

		return;
	}

	lpInfo->RemainTime = (int)difftime(ScheduleTime.GetTime(), time(0));

	lpInfo->TargetTime = (int)ScheduleTime.GetTime();
}

int CBonusManager::GetState(int index)
{
	if (CHECK_RANGE(index, MAX_BONUS) == 0)
	{
		return BONUS_STATE_BLANK;
	}
	else
	{
		return this->m_BonusInfo[index].State;
	}
}

char* CBonusManager::GetBonusName(int index)
{
	if (CHECK_RANGE(index, MAX_BONUS) == 0)
	{
		return NULL;
	}

	return this->m_BonusInfo[index].BonusName;
}

int CBonusManager::GetCurrentRemainTime(int index)
{
	if (CHECK_RANGE(index, MAX_BONUS) == 0)
	{
		return 0;
	}

	return this->m_BonusInfo[index].RemainTime;
}

int CBonusManager::GetRemainTime(int index)
{
	if (CHECK_RANGE(index, MAX_BONUS) == 0)
	{
		return 0;
	}

	BONUS_INFO* lpInfo = &this->m_BonusInfo[index];

	if (lpInfo->StartTime.empty() != 0)
	{
		return 0;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for (std::vector<BONUS_START_TIME>::iterator it = lpInfo->StartTime.begin(); it != lpInfo->StartTime.end(); it++)
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

int CBonusManager::GetBonusValue(LPOBJ lpObj, int BonusIndex, int BonusValue, int ItemIndex, int ItemLevel, int MonsterClass, int MonsterLevel)
{
	for (int n = 0; n < MAX_BONUS; n++)
	{
		if (this->m_BonusInfo[n].State == BONUS_STATE_START)
		{
			for (std::vector<BONUS_VALUE_INFO>::iterator it = this->m_BonusInfo[n].ValueInfo.begin(); it != this->m_BonusInfo[n].ValueInfo.end(); it++)
			{
				if (it->BonusIndex == BonusIndex && (it->ItemIndex == -1 || it->ItemIndex == ItemIndex) && (it->ItemLevel == -1 || it->ItemLevel == ItemLevel) && (it->MapNumber == -1 || it->MapNumber == lpObj->Map) && (it->MonsterClass == -1 || it->MonsterClass == MonsterClass) && (it->MonsterLevelMin == -1 || it->MonsterLevelMin <= MonsterLevel) && (it->MonsterLevelMax == -1 || it->MonsterLevelMax >= MonsterLevel))
				{
					switch (it->BonusIndex)
					{
						case BONUS_INDEX_EXPERIENCE_RATE:
						{
							BonusValue += (BonusValue * it->BonusValue[lpObj->AccountLevel]) / 100;

							break;
						}

						case BONUS_INDEX_ITEM_DROP_RATE:
						{
							BonusValue += (BonusValue * it->BonusValue[lpObj->AccountLevel]) / 100;

							break;
						}

						case BONUS_INDEX_CMN_ITEM_DROP_RATE:
						{
							BonusValue += ((__int64)(1000000 - BonusValue) * it->BonusValue[lpObj->AccountLevel]) / 1000000;

							break;
						}

						case BONUS_INDEX_EXC_ITEM_DROP_RATE:
						{
							BonusValue += ((__int64)(1000000 - BonusValue) * it->BonusValue[lpObj->AccountLevel]) / 1000000;

							break;
						}

						case BONUS_INDEX_RESET_AMOUNT:
						{
							BonusValue += it->BonusValue[lpObj->AccountLevel];

							break;
						}
					}
				}
			}
		}
	}

	return BonusValue;
}