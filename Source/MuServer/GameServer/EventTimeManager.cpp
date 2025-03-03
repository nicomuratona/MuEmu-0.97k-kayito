#include "stdafx.h"
#include "EventTimeManager.h"
#include "Protocol.h"
#include "ServerInfo.h"
#include "Util.h"

#include "DevilSquare.h"
#include "BloodCastle.h"
#include "InvasionManager.h"
#include "BonusManager.h"
#include "GoldenArcherBingo.h"

CEventTimeManager gEventTimeManager;

CEventTimeManager::CEventTimeManager()
{

}

CEventTimeManager::~CEventTimeManager()
{

}

void CEventTimeManager::GCEventTimeSend(int aIndex)
{
	BYTE send[4096];

	PMSG_EVENT_TIME_SEND pMsg;

	pMsg.header.set(0xF3, 0xE6, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_EVENT_TIME info;

	int state;

	int i;

	if (gServerInfo.m_DevilSquareEvent)
	{
		strcpy_s(info.name, "Devil Square");

		info.time = 0;

		state = gDevilSquare.GetState(0);

		if (state == DS_STATE_START)
		{
			info.status = EVENT_STATE_START;
		}
		else if (state == DS_STATE_EMPTY)
		{
			info.status = EVENT_STATE_STAND;

			info.time = gDevilSquare.GetCurrentRemainTime(0);
		}
		else if (state == DS_STATE_STAND)
		{
			info.status = EVENT_STATE_OPEN;
		}
		else
		{
			info.status = EVENT_STATE_BLANK;
		}

		memcpy(&send[size], &info, sizeof(info));

		size += sizeof(info);

		pMsg.count++;
	}

	if (gServerInfo.m_BloodCastleEvent)
	{
		strcpy_s(info.name, "Blood Castle");

		info.time = 0;

		state = gBloodCastle.GetState(0);

		if (state == BC_STATE_START)
		{
			info.status = EVENT_STATE_START;
		}
		else if (state == BC_STATE_EMPTY)
		{
			info.status = EVENT_STATE_STAND;

			info.time = gBloodCastle.GetCurrentRemainTime(0);
		}
		else if (state == BC_STATE_STAND)
		{
			info.status = EVENT_STATE_OPEN;
		}
		else
		{
			info.status = EVENT_STATE_BLANK;
		}

		memcpy(&send[size], &info, sizeof(info));

		size += sizeof(info);

		pMsg.count++;
	}

	if (gServerInfo.m_InvasionManagerSwitch)
	{
		for (i = 0; i < MAX_INVASION; i++)
		{
			state = gInvasionManager.GetState(i);

			if (state == INVASION_STATE_BLANK)
			{
				continue;
			}

			strcpy_s(info.name, gInvasionManager.GetInvasionName(i));

			if (state == INVASION_STATE_EMPTY)
			{
				info.status = EVENT_STATE_STAND;

				info.time = gInvasionManager.GetCurrentRemainTime(i);
			}
			else
			{
				info.status = EVENT_STATE_START;

				info.time = 0;
			}

			memcpy(&send[size], &info, sizeof(info));

			size += sizeof(info);

			pMsg.count++;
		}
	}

	if (gServerInfo.m_BonusManagerSwitch)
	{
		for (i = 0; i < MAX_BONUS; i++)
		{
			state = gBonusManager.GetState(i);

			if (state == BONUS_STATE_BLANK)
			{
				continue;
			}

			strcpy_s(info.name, gBonusManager.GetBonusName(i));

			if (state == BONUS_STATE_EMPTY)
			{
				info.status = EVENT_STATE_STAND;

				info.time = gBonusManager.GetCurrentRemainTime(i);
			}
			else
			{
				info.status = EVENT_STATE_START;

				info.time = 0;
			}

			memcpy(&send[size], &info, sizeof(info));

			size += sizeof(info);

			pMsg.count++;
		}
	}

	if (gServerInfo.m_GoldenArcherBingoEvent)
	{
		strcpy_s(info.name, gGoldenArcherBingo.GetEventName());

		info.time = 0;

		state = gGoldenArcherBingo.GetState();

		if (state == BINGO_STATE_START)
		{
			info.status = EVENT_STATE_START;
		}
		else if (state == BINGO_STATE_EMPTY)
		{
			info.status = EVENT_STATE_STAND;

			info.time = gGoldenArcherBingo.GetCurrentRemainTime();
		}
		else if (state == BINGO_STATE_STAND)
		{
			info.status = EVENT_STATE_OPEN;
		}
		else
		{
			info.status = EVENT_STATE_BLANK;
		}

		memcpy(&send[size], &info, sizeof(info));

		size += sizeof(info);

		pMsg.count++;
	}

	if (pMsg.count > 0)
	{
		pMsg.header.size[0] = SET_NUMBERHB(size);

		pMsg.header.size[1] = SET_NUMBERLB(size);

		memcpy(send, &pMsg, sizeof(pMsg));

		DataSend(aIndex, send, size);
	}
}