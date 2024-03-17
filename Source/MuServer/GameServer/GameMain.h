#pragma once

#include "Connection.h"
#include "Protocol.h"

struct SDHP_GAME_SERVER_LIVE_SEND
{
	PBMSG_HEAD header; // C1:01
	WORD ServerCode;
	BYTE UserTotal;
	WORD UserCount;
	WORD AccountCount;
	WORD MaxUserCount;
};

void GameMainInit(HWND hwnd);

void ConnectServerInfoSend();

bool JoinServerConnect();

bool DataServerConnect();

bool JoinServerReconnect(HWND hwnd);

bool DataServerReconnect(HWND hwnd);

void CALLBACK QueueTimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired);

extern CConnection gJoinServerConnection;

extern CConnection gDataServerConnection;