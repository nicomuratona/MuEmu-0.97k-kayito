#include "stdafx.h"
#include "GameMain.h"
#include "BattleSoccerManager.h"
#include "DSProtocol.h"
#include "HackCheck.h"
#include "JSProtocol.h"
#include "MonsterAI.h"
#include "MonsterManager.h"
#include "ObjectManager.h"
#include "QueueTimer.h"
#include "ServerInfo.h"
#include "SocketManagerUdp.h"
#include "User.h"
#include "Util.h"

CConnection gJoinServerConnection;

CConnection gDataServerConnection;

void GameMainInit(HWND hwnd)
{
	if (CreateMutex(0, 0, gServerInfo.m_ServerMutex) == 0 || GetLastError() == ERROR_ALREADY_EXISTS)
	{
		ErrorMessageBox("Could not open GameServer");

		return;
	}

	gObjInit();

	InitHackCheck();

	gBattleSoccer.InitBattleSoccer();

	gGameServerLogOut = 0;

	gGameServerDisconnect = 0;

	gServerInfo.ReadInit();

	gObjSetExperienceTable();

	gMonsterManager.SetMonsterData();

	gJoinServerConnection.Init(hwnd, JoinServerProtocolCore);

	gDataServerConnection.Init(hwnd, DataServerProtocolCore);
}

void ConnectServerInfoSend()
{
	SDHP_GAME_SERVER_LIVE_SEND pMsg;

	pMsg.header.set(0x01, sizeof(pMsg));

	pMsg.ServerCode = (WORD)gServerInfo.m_ServerCode;

	pMsg.UserTotal = (BYTE)((gObjTotalUser == 0) ? 0 : ((gObjTotalUser * 100) / gServerInfo.m_ServerMaxUserNumber));

	pMsg.UserCount = (WORD)gObjTotalUser;

	pMsg.AccountCount = 0;

	pMsg.MaxUserCount = (WORD)gServerInfo.m_ServerMaxUserNumber;

	gSocketManagerUdp.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

bool JoinServerConnect(DWORD wMsg)
{
	if (gJoinServerConnection.Connect(gServerInfo.m_JoinServerAddress, (WORD)gServerInfo.m_JoinServerPort, wMsg) == 0)
	{
		return 0;
	}

	GJServerInfoSend();

	return 1;
}

bool DataServerConnect(DWORD wMsg)
{
	if (gDataServerConnection.Connect(gServerInfo.m_DataServerAddress, (WORD)gServerInfo.m_DataServerPort, wMsg) == 0)
	{
		return 0;
	}

	GDServerInfoSend();

	return 1;
}

bool JoinServerReconnect(HWND hwnd, DWORD wMsg)
{
	if (gJoinServerConnection.CheckState() == 0)
	{
		gJoinServerConnection.Init(hwnd, JoinServerProtocolCore);

		return JoinServerConnect(wMsg);
	}

	return 1;
}

bool DataServerReconnect(HWND hwnd, DWORD wMsg)
{
	if (gDataServerConnection.CheckState() == 0)
	{
		gDataServerConnection.Init(hwnd, DataServerProtocolCore);

		return DataServerConnect(wMsg);
	}

	return 1;
}

void JoinServerMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(lParam))
	{
		case FD_READ:
		{
			gJoinServerConnection.DataRecv();

			break;
		}

		case FD_WRITE:
		{
			gJoinServerConnection.DataSendEx();

			break;
		}

		case FD_CLOSE:
		{
			gJoinServerConnection.Disconnect();

			gObjAllDisconnect();

			break;
		}
	}
}

void DataServerMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(lParam))
	{
		case FD_READ:
		{
			gDataServerConnection.DataRecv();

			break;
		}

		case FD_WRITE:
		{
			gDataServerConnection.DataSendEx();

			break;
		}

		case FD_CLOSE:
		{
			gDataServerConnection.Disconnect();

			gObjAllDisconnect();

			break;
		}
	}
}

void CALLBACK QueueTimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	static CCriticalSection critical;

	critical.lock();

	switch (((int)lpParameter))
	{
		case QUEUE_TIMER_MONSTER:
		{
			gObjectManager.ObjectMonsterAndMsgProc();

			break;
		}

		case QUEUE_TIMER_MONSTER_MOVE:
		{
			gObjectManager.ObjectMoveProc();

			break;
		}

		case QUEUE_TIMER_MONSTER_AI:
		{
			CMonsterAI::MonsterAIProc();

			break;
		}

		case QUEUE_TIMER_MONSTER_AI_MOVE:
		{
			CMonsterAI::MonsterMoveProc();

			break;
		}

		case QUEUE_TIMER_EVENT:
		{
			gObjEventRunProc();

			break;
		}

		case QUEUE_TIMER_VIEWPORT:
		{
			gObjViewportProc();

			break;
		}

		case QUEUE_TIMER_FIRST:
		{
			gObjFirstProc();

			break;
		}

		case QUEUE_TIMER_CLOSE:
		{
			gObjCloseProc();

			break;
		}

		case QUEUE_TIMER_ACCOUNT_LEVEL:
		{
			gObjAccountLevelProc();

			break;
		}
	}

	critical.unlock();
}