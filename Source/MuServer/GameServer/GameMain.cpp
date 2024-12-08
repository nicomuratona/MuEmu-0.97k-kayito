#include "stdafx.h"
#include "GameMain.h"
#include "BattleSoccerManager.h"
#include "DSProtocol.h"
#include "HackCheck.h"
#include "JSProtocol.h"
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

	gMonsterManager.SetMonsterData();

	gJoinServerConnection.Init(hwnd, "JoinServerConnection", JoinServerProtocolCore);

	gDataServerConnection.Init(hwnd, "DataServerConnection", DataServerProtocolCore);
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

bool JoinServerConnect()
{
	if (!gJoinServerConnection.Connect(gServerInfo.m_JoinServerAddress, (WORD)gServerInfo.m_JoinServerPort))
	{
		LogAdd(LOG_RED, "[JoinServerConnection] Connect Failed");

		return false;
	}

	LogAdd(LOG_GREEN, "[JoinServerConnection] Connected");

	GJServerInfoSend();

	return true;
}

bool DataServerConnect()
{
	if (!gDataServerConnection.Connect(gServerInfo.m_DataServerAddress, (WORD)gServerInfo.m_DataServerPort))
	{
		LogAdd(LOG_RED, "[DataServerConnection] Connect Failed");

		return false;
	}

	LogAdd(LOG_GREEN, "[DataServerConnection] Connected");

	GDServerInfoSend();

	return true;
}

bool JoinServerReconnect(HWND hwnd)
{
	if (!gJoinServerConnection.CheckState())
	{
		gJoinServerConnection.Init(hwnd, "JoinServerConnection", JoinServerProtocolCore);

		return JoinServerConnect();
	}

	return true;
}

bool DataServerReconnect(HWND hwnd)
{
	if (!gDataServerConnection.CheckState())
	{
		gDataServerConnection.Init(hwnd, "DataServerConnection", DataServerProtocolCore);

		return DataServerConnect();
	}

	return true;
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