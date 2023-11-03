#pragma once

#include "ProtocolDefines.h"

#define MAX_SERVER_CODE 65535

//**********************************************//
//********** ConnectServer -> Client ***********//
//**********************************************//

struct PMSG_CUSTOM_SERVER_LIST_RECV
{
	PSWMSG_HEAD header; // C1:F4:03
	BYTE count[2];
};

struct SERVER_LIST_INFO
{
	WORD ServerCode;
	char ServerName[32];
};

//**********************************************//
//**********************************************//
//**********************************************//

class CServerList
{
public:

	CServerList();

	virtual ~CServerList();

	void Init();

	static void MultiServerNameChange(char* Text, char* Format, char* ServerName, int ServerCode, char* TextLine);

	static void ServerNameChange(char* Text, char* Format, DWORD arg1, DWORD arg2);

	void GCCustomServerListRecv(PMSG_CUSTOM_SERVER_LIST_RECV* lpMsg);

	static void ServerBoxTexture(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale);

	static void ServerBoxText(int iPos_x, int iPos_y, char* pszText, int iBoxWidth, int iSort, SIZE* lpTextSize);

private:

	char m_ServerListInfo[MAX_SERVER_CODE][32];
};

extern CServerList gServerList;