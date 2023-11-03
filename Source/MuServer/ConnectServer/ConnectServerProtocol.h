#pragma once

#include "ProtocolDefines.h"

//**********************************************//
//********** Client -> ConnectServer ***********//
//**********************************************//

struct PMSG_SERVER_LIST_RECV
{
	PSBMSG_HEAD header; // C1:F4:02
};

struct PMSG_SERVER_INFO_RECV
{
	PSBMSG_HEAD header; // C1:F4:03
	BYTE ServerCode;
};

//**********************************************//
//********** ConnectServer -> Client ***********//
//**********************************************//

struct PMSG_SERVER_INIT_SEND
{
	PBMSG_HEAD header; // C1:00
	BYTE result;
};

struct PMSG_CUSTOM_SERVER_LIST_SEND
{
	PSWMSG_HEAD header; // C1:F4:01
	BYTE count[2];
};

struct PMSG_CUSTOM_SERVER_LIST
{
	WORD ServerCode;
	char ServerName[32];
};

struct PMSG_SERVER_LIST_SEND
{
	PSWMSG_HEAD header; // C1:F4:02
	BYTE count;
};

struct PMSG_SERVER_LIST
{
	WORD ServerCode;
	BYTE UserTotal;
};

struct PMSG_SERVER_INFO_SEND
{
	PSBMSG_HEAD header; // C1:F4:03
	char ServerAddress[16];
	WORD ServerPort;
};

//**********************************************//
//**********************************************//
//**********************************************//

void ConnectServerProtocolCore(int index, BYTE head, BYTE* lpMsg, int size);

void CCServerInfoRecv(PMSG_SERVER_INFO_RECV* lpMsg, int index);

void CCServerListRecv(PMSG_SERVER_LIST_RECV* lpMsg, int index);

void CCCustomServerListSend(int index);

void CCServerInitSend(int index, int result);