#include "stdafx.h"
#include "GoldenArcherBingo.h"
#include "GameMain.h"
#include "MapManager.h"
#include "Message.h"
#include "Monster.h"
#include "Notice.h"
#include "ReadScript.h"
#include "ScheduleManager.h"
#include "ServerInfo.h"
#include "Util.h"

CGoldenArcherBingo gGoldenArcherBingo;

CGoldenArcherBingo::CGoldenArcherBingo()
{
	this->m_State = BINGO_STATE_BLANK;

	this->m_MinutesLeft = -1;

	this->m_RemainTime = 0;

	this->m_TargetTime = 0;

	this->m_TickCount = GetTickCount();

	this->m_GoldenArcherIndex = -1;

	this->m_GoldenArcherBingoStartTime.clear();

	this->m_WaitForDSResponse = false;

	this->m_GoldenArcherBingoWinners.clear();
}

CGoldenArcherBingo::~CGoldenArcherBingo()
{

}

void CGoldenArcherBingo::Init()
{
	if (gServerInfo.m_GoldenArcherBingoEvent == 0)
	{
		this->SetState(BINGO_STATE_BLANK);
	}
	else
	{
		this->SetState(BINGO_STATE_EMPTY);
	}
}

void CGoldenArcherBingo::Load(char* path)
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

	this->m_GoldenArcherBingoStartTime.clear();

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
					GOLDEN_ARCHER_BINGO_START_TIME info;

					info.Year = lpReadScript->GetNumber();

					info.Month = lpReadScript->GetAsNumber();

					info.Day = lpReadScript->GetAsNumber();

					info.DayOfWeek = lpReadScript->GetAsNumber();

					info.Hour = lpReadScript->GetAsNumber();

					info.Minute = lpReadScript->GetAsNumber();

					info.Second = lpReadScript->GetAsNumber();

					this->m_GoldenArcherBingoStartTime.push_back(info);
				}
				else if (section == 1)
				{
					this->m_GoldenArcherBingoInfo.WarningMsg = lpReadScript->GetNumber();

					this->m_GoldenArcherBingoInfo.WarningTime = lpReadScript->GetAsNumber();

					this->m_GoldenArcherBingoInfo.StartMessage = lpReadScript->GetAsNumber();

					this->m_GoldenArcherBingoInfo.EventTime = lpReadScript->GetAsNumber();

					this->m_GoldenArcherBingoInfo.FinalMessage = lpReadScript->GetAsNumber();

					strcpy_s(this->m_GoldenArcherBingoInfo.EventName, lpReadScript->GetAsString());
				}
				else if (section == 2)
				{
					this->m_GoldenArcherBingoMapInfo.Map = lpReadScript->GetNumber();

					this->m_GoldenArcherBingoMapInfo.PosX = lpReadScript->GetAsNumber();

					this->m_GoldenArcherBingoMapInfo.PosY = lpReadScript->GetAsNumber();

					this->m_GoldenArcherBingoMapInfo.Dir = lpReadScript->GetAsNumber();
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

void CGoldenArcherBingo::MainProc()
{
	DWORD elapsed = GetTickCount() - this->m_TickCount;

	if (elapsed < 1000)
	{
		return;
	}

	this->m_TickCount = GetTickCount();

	this->m_RemainTime = (int)difftime(this->m_TargetTime, time(0));

	switch (this->m_State)
	{
		case BINGO_STATE_BLANK:
		{
			this->ProcState_BLANK();

			break;
		}

		case BINGO_STATE_EMPTY:
		{
			this->ProcState_EMPTY();

			break;
		}

		case BINGO_STATE_STAND:
		{
			this->ProcState_STAND();

			break;
		}

		case BINGO_STATE_START:
		{
			this->ProcState_START();

			break;
		}

		case BINGO_STATE_CLEAN:
		{
			this->ProcState_CLEAN();

			break;
		}
	}
}

char* CGoldenArcherBingo::GetEventName()
{
	return this->m_GoldenArcherBingoInfo.EventName;
}

int CGoldenArcherBingo::GetState()
{
	return this->m_State;
}

int CGoldenArcherBingo::GetCurrentRemainTime()
{
	return this->m_RemainTime;
}

int CGoldenArcherBingo::GetGoldenArcherIndex()
{
	return this->m_GoldenArcherIndex;
}

bool CGoldenArcherBingo::CheckBingoWinner(int aIndex, const char* lpLuckyNumber)
{
	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	auto it = m_GoldenArcherBingoWinners.find(lpObj->Account);

	if (it == m_GoldenArcherBingoWinners.end())
	{
		return false;
	}

	auto& numbers = it->second;

	auto numIt = std::find(numbers.begin(), numbers.end(), lpLuckyNumber);

	if (numIt == numbers.end())
	{
		return false;
	}

	numbers.erase(numIt);

	if (numbers.empty())
	{
		this->m_GoldenArcherBingoWinners.erase(it);
	}

	return true;
}

void CGoldenArcherBingo::ProcState_BLANK()
{

}

void CGoldenArcherBingo::ProcState_EMPTY()
{
	if (this->m_RemainTime > 0 && this->m_RemainTime <= (this->m_GoldenArcherBingoInfo.WarningTime * 60))
	{
		int minutes = this->m_RemainTime / 60;

		if ((this->m_RemainTime % 60) == 0)
		{
			minutes--;
		}

		if (this->m_MinutesLeft != minutes)
		{
			this->m_MinutesLeft = minutes;

			gNotice.GCNoticeSendToAll(0, this->m_GoldenArcherBingoInfo.WarningMsg, this->m_GoldenArcherBingoInfo.EventName, (this->m_MinutesLeft + 1));
		}
	}

	if (this->m_RemainTime <= 0)
	{
		if (this->m_GoldenArcherBingoInfo.StartMessage != -1)
		{
			gNotice.GCNoticeSendToAll(0, this->m_GoldenArcherBingoInfo.StartMessage, this->m_GoldenArcherBingoInfo.EventName);
		}

		this->SetState(BINGO_STATE_STAND);
	}
}

void CGoldenArcherBingo::ProcState_STAND()
{
	if (this->m_WaitForDSResponse && this->m_RemainTime <= 0)
	{
		this->m_WaitForDSResponse = false;
	}

	if (!this->m_WaitForDSResponse)
	{
		if (this->m_GoldenArcherBingoWinners.empty())
		{
			LogAdd(LOG_EVENT, "[Bingo] There were no winners");

			gNotice.GCNoticeSendToAll(0, 31, this->m_GoldenArcherBingoInfo.EventName);

			this->SetState(BINGO_STATE_EMPTY);
		}
		else
		{
			this->SetState(BINGO_STATE_START);
		}
	}
}

void CGoldenArcherBingo::ProcState_START()
{
	if (this->m_RemainTime <= 0 || this->m_GoldenArcherBingoWinners.empty())
	{
		if (this->m_GoldenArcherBingoInfo.FinalMessage != -1)
		{
			gNotice.GCNoticeSendToAll(0, this->m_GoldenArcherBingoInfo.FinalMessage, this->m_GoldenArcherBingoInfo.EventName);
		}

		this->SetState(BINGO_STATE_CLEAN);
	}
}

void CGoldenArcherBingo::ProcState_CLEAN()
{
	if (this->m_WaitForDSResponse && this->m_RemainTime <= 0)
	{
		this->m_WaitForDSResponse = false;
	}

	if (!this->m_WaitForDSResponse)
	{
		this->SetState(BINGO_STATE_EMPTY);
	}
}

void CGoldenArcherBingo::SetState(int state)
{
	this->m_State = state;

	switch (this->m_State)
	{
		case BINGO_STATE_BLANK:
		{
			this->SetState_BLANK();

			break;
		}

		case BINGO_STATE_EMPTY:
		{
			this->SetState_EMPTY();

			break;
		}

		case BINGO_STATE_STAND:
		{
			this->SetState_STAND();

			break;
		}

		case BINGO_STATE_START:
		{
			this->SetState_START();

			break;
		}

		case BINGO_STATE_CLEAN:
		{
			this->SetState_CLEAN();

			break;
		}
	}
}

void CGoldenArcherBingo::SetState_BLANK()
{
	LogAdd(LOG_EVENT, "[Bingo] SetState BLANK");
}

void CGoldenArcherBingo::SetState_EMPTY()
{
	this->m_MinutesLeft = -1;

	this->m_WaitForDSResponse = false;

	this->m_GoldenArcherBingoWinners.clear();

	if (OBJECT_RANGE(this->m_GoldenArcherIndex) != 0)
	{
		gObjDel(this->m_GoldenArcherIndex);

		this->m_GoldenArcherIndex = -1;
	}

	this->CheckSync();

	LogAdd(LOG_EVENT, "[Bingo] SetState EMPTY");
}

void CGoldenArcherBingo::SetState_STAND()
{
	this->m_MinutesLeft = -1;

	this->m_WaitForDSResponse = true;

	this->m_GoldenArcherBingoWinners.clear();

	this->GDBingoGetWinnersSend();

	this->m_RemainTime = 1 * 60;

	this->m_TargetTime = (int)(time(0) + this->m_RemainTime);

	LogAdd(LOG_EVENT, "[Bingo] SetState STAND");
}

void CGoldenArcherBingo::SetState_START()
{
	this->m_MinutesLeft = -1;

	this->m_WaitForDSResponse = false;

	this->SetGoldenArcher();

	gNotice.GCNoticeSendToAll(
		0,
		32,
		this->m_GoldenArcherBingoInfo.EventName,
		gMapManager.GetMapName(this->m_GoldenArcherBingoMapInfo.Map),
		this->m_GoldenArcherBingoMapInfo.PosX,
		this->m_GoldenArcherBingoMapInfo.PosY,
		this->m_GoldenArcherBingoInfo.EventTime);

	this->m_RemainTime = this->m_GoldenArcherBingoInfo.EventTime * 60;

	this->m_TargetTime = (int)(time(0) + this->m_RemainTime);

	LogAdd(LOG_EVENT, "[Bingo] SetState START");
}

void CGoldenArcherBingo::SetState_CLEAN()
{
	this->m_MinutesLeft = -1;

	this->m_WaitForDSResponse = true;

	this->m_GoldenArcherBingoWinners.clear();

	if (gServerInfo.m_GoldenArcherDeleteRegistered != 0)
	{
		gGoldenArcher.ClearRegisteredLuckyNumbers();

		this->GDBingoClearRegisteredSend();
	}

	if (OBJECT_RANGE(this->m_GoldenArcherIndex) != 0)
	{
		gObjDel(this->m_GoldenArcherIndex);

		this->m_GoldenArcherIndex = -1;
	}

	this->m_RemainTime = 1 * 60;

	this->m_TargetTime = (int)(time(0) + this->m_RemainTime);

	LogAdd(LOG_EVENT, "[Bingo] SetState CLEAN");
}

void CGoldenArcherBingo::CheckSync()
{
	if (this->m_GoldenArcherBingoStartTime.empty())
	{
		this->SetState(BINGO_STATE_BLANK);

		return;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for (std::vector<GOLDEN_ARCHER_BINGO_START_TIME>::iterator it = this->m_GoldenArcherBingoStartTime.begin(); it != this->m_GoldenArcherBingoStartTime.end(); it++)
	{
		ScheduleManager.AddSchedule(it->Year, it->Month, it->Day, it->Hour, it->Minute, it->Second, it->DayOfWeek);
	}

	if (!ScheduleManager.GetSchedule(&ScheduleTime))
	{
		this->SetState(BINGO_STATE_BLANK);

		return;
	}

	this->m_RemainTime = (int)difftime(ScheduleTime.GetTime(), time(0));

	this->m_TargetTime = (int)ScheduleTime.GetTime();

	LogAdd(LOG_EVENT, "[Bingo] Sync Start Time. [%d] min remain", (this->m_RemainTime / 60));
}

void CGoldenArcherBingo::SetGoldenArcher()
{
	if (OBJECT_RANGE(this->m_GoldenArcherIndex) != 0)
	{
		return;
	}

	int index = gObjAddMonster(this->m_GoldenArcherBingoMapInfo.Map);

	if (OBJECT_RANGE(index) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[index];

	int px = this->m_GoldenArcherBingoMapInfo.PosX;
	int py = this->m_GoldenArcherBingoMapInfo.PosY;

	lpObj->PosNum = -1;
	lpObj->X = px;
	lpObj->Y = py;
	lpObj->TX = px;
	lpObj->TY = py;
	lpObj->OldX = px;
	lpObj->OldY = py;
	lpObj->StartX = px;
	lpObj->StartY = py;
	lpObj->Dir = this->m_GoldenArcherBingoMapInfo.Dir;
	lpObj->Map = this->m_GoldenArcherBingoMapInfo.Map;

	if (!gObjSetMonster(index, 236))
	{
		gObjDel(index);

		return;
	}

	this->m_GoldenArcherIndex = index;
}

void CGoldenArcherBingo::GDBingoGetWinnersSend()
{
	SDHP_BINGO_GET_WINNERS_SEND pMsg;

	pMsg.header.set(0x04, 0x04, sizeof(pMsg));

	pMsg.count = gServerInfo.m_GoldenArcherWinnersAmount;

	gDataServerConnection.DataSend((BYTE*)&pMsg, sizeof(pMsg));
}

void CGoldenArcherBingo::DGBingoGetWinnersRecv(SDHP_BINGO_GET_WINNERS_RECV* lpMsg)
{
	for (int i = 0; i < lpMsg->count; i++)
	{
		BINGO_WINNER* lpInfo = (BINGO_WINNER*)(((BYTE*)lpMsg) + sizeof(SDHP_BINGO_GET_WINNERS_RECV) + (sizeof(BINGO_WINNER) * i));

		this->m_GoldenArcherBingoWinners[lpInfo->Account].push_back(lpInfo->LuckyNumber);
	}

	this->m_WaitForDSResponse = false;
}

void CGoldenArcherBingo::GDBingoClearRegisteredSend()
{
	SDHP_BINGO_CLEAR_REGISTERED_SEND pMsg;

	pMsg.header.set(0x04, 0x05, sizeof(pMsg));

	gDataServerConnection.DataSend((BYTE*)&pMsg, sizeof(pMsg));
}

void CGoldenArcherBingo::DGBingoClearRegisteredRecv(SDHP_BINGO_CLEAR_REGISTERED_RECV* lpMsg)
{
	if (lpMsg->result == 0)
	{
		LogAdd(LOG_EVENT, "[Bingo] Cleared all registered Lucky Numbers");
	}
	else
	{
		LogAdd(LOG_EVENT, "[Bingo] Error while cleaning registered Lucky Numbers");
	}

	this->m_WaitForDSResponse = false;
}

void CGoldenArcherBingo::StartGoldenArcherBingo()
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

	GOLDEN_ARCHER_BINGO_START_TIME info;

	info.Year = -1;

	info.Month = -1;

	info.Day = -1;

	info.DayOfWeek = -1;

	info.Hour = hour;

	info.Minute = minute;

	info.Second = 0;

	this->m_GoldenArcherBingoStartTime.push_back(info);

	LogAdd(LOG_EVENT, "[Set %s Start] At %2d:%2d:00", this->m_GoldenArcherBingoInfo.EventName, hour, minute);

	this->Init();
}