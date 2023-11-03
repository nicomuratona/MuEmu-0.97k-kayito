#pragma once

#include "ProtocolDefines.h"

//**********************************************//
//********** JoinServer -> ConnectServer *******//
//**********************************************//

struct SDHP_JOIN_SERVER_LIVE_SEND
{
	PBMSG_HEAD header; // C1:02
	DWORD QueueSize;
};

//**********************************************//
//********** GameServer -> JoinServer **********//
//**********************************************//

struct SDHP_SERVER_INFO_RECV
{
	PBMSG_HEAD header; // C1:00
	BYTE type;
	WORD ServerPort;
	char ServerName[50];
	WORD ServerCode;
};

struct SDHP_CONNECT_ACCOUNT_RECV
{
	PBMSG_HEAD header; // C1:01
	WORD index;
	char account[11];
	char password[11];
	char IpAddress[16];
};

struct SDHP_DISCONNECT_ACCOUNT_RECV
{
	PBMSG_HEAD header; // C1:02
	WORD index;
	char account[11];
	char IpAddress[16];
};

struct SDHP_ACCOUNT_LEVEL_RECV
{
	PBMSG_HEAD header; // C1:03
	WORD index;
	char account[11];
};

struct SDHP_ACCOUNT_LEVEL_SAVE_RECV
{
	PBMSG_HEAD header; // C1:04
	WORD index;
	char account[11];
	WORD AccountLevel;
	DWORD AccountExpireTime;
};

struct SDHP_SERVER_USER_INFO_RECV
{
	PBMSG_HEAD header; // C1:06
	WORD CurUserCount;
	WORD MaxUserCount;
};

struct SDHP_COUNT_ONLINE_USER_RECV
{
	PBMSG_HEAD header; // C1:07
	int index;
};

//**********************************************//
//********** JoinServer -> GameServer **********//
//**********************************************//

struct SDHP_CONNECT_ACCOUNT_SEND
{
	PBMSG_HEAD header; // C1:01
	WORD index;
	char account[11];
	char PersonalCode[14];
	BYTE result;
	BYTE BlockCode;
	WORD AccountLevel;
	char AccountExpireDate[20];
};

struct SDHP_DISCONNECT_ACCOUNT_SEND
{
	PBMSG_HEAD header; // C1:02
	WORD index;
	char account[11];
	BYTE result;
};

struct SDHP_ACCOUNT_LEVEL_SEND
{
	PBMSG_HEAD header; // C1:03
	WORD index;
	char account[11];
	WORD AccountLevel;
	char AccountExpireDate[20];
};

struct SDHP_ACCOUNT_ALREADY_CONNECTED_SEND
{
	PBMSG_HEAD header; // C1:05
	WORD index;
	char account[11];
};

struct SDHP_COUNT_ONLINE_USER_SEND
{
	PBMSG_HEAD header; // C1:07
	int index;
	int count;
};

//**********************************************//
//**********************************************//
//**********************************************//

void JoinServerProtocolCore(int index, BYTE head, BYTE* lpMsg, int size);

void JoinServerLiveProc();

void GJServerInfoRecv(SDHP_SERVER_INFO_RECV* lpMsg, int index);

void GJConnectAccountRecv(SDHP_CONNECT_ACCOUNT_RECV* lpMsg, int index);

void GJDisconnectAccountRecv(SDHP_DISCONNECT_ACCOUNT_RECV* lpMsg, int index);

void GJAccountLevelRecv(SDHP_ACCOUNT_LEVEL_RECV* lpMsg, int index);

void GJAccountLevelSaveRecv(SDHP_ACCOUNT_LEVEL_SAVE_RECV* lpMsg, int index);

void JGAccountAlreadyConnectedSend(int GameServerCode, int UserIndex, char* account);

void GJAccountCountRecv(SDHP_COUNT_ONLINE_USER_RECV* lpMsg, int index);

void GJServerUserInfoRecv(SDHP_SERVER_USER_INFO_RECV* lpMsg, int index);