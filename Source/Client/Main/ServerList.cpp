#include "stdafx.h"
#include "ServerList.h"

CServerList gServerList;

CServerList::CServerList()
{
	memset(this->m_ServerListInfo, 0, sizeof(this->m_ServerListInfo));

	this->Init();
}

CServerList::~CServerList()
{

}

void CServerList::Init()
{
	SetByte(0x0051F053, 0xEB);

	SetCompleteHook(0xE9, 0x0051F0CF, 0x0051F138);

	SetByte(0x0051F13D, 0x50);

	SetCompleteHook(0xE8, 0x0051F1AE, &this->MultiServerNameChange);

	SetCompleteHook(0xE8, 0x004ED016, &this->ServerNameChange); //Ingame Character

	SetCompleteHook(0xE8, 0x0051F357, &this->ServerBoxTexture); // Server
	SetCompleteHook(0xE8, 0x0051F5FD, &this->ServerBoxTexture); // ServerList
	SetCompleteHook(0xE8, 0x0051F761, &this->ServerBoxTexture); // ServerListBar
	SetCompleteHook(0xE8, 0x0051F7F7, &this->ServerBoxTexture); // ServerListBar

	SetCompleteHook(0xE8, 0x0051F39E, &this->ServerBoxText); // Server
	SetCompleteHook(0xE8, 0x0051F688, &this->ServerBoxText); // ServerList

	SetDword(0x0051E8B7 + 1, 150 + 135); // ServerMouseX
	SetDword(0x0051E8A5 + 1, 375 + 135); // ServerMouseX
	SetDword(0x0051EC8F + 1, 230 + 135); // ServerListMouseX
	SetDword(0x0051EC9A + 1, 364 + 135); // ServerListMouseX
}

void CServerList::MultiServerNameChange(char* Text, char* Format, char* ServerName, int ServerCode, char* TextLine)
{
	wsprintf(Text, "%s", gServerList.m_ServerListInfo[ServerCode]);
}

void CServerList::ServerNameChange(char* Text, char* Format, DWORD arg1, DWORD arg2)
{
	wsprintf(Text, "%s", gServerList.m_ServerListInfo[(abs((int)((*(DWORD*)(0x00561694) - 23) * 20)) + (*(DWORD*)(0x0056169C) - 1))]);
}

void CServerList::GCCustomServerListRecv(PMSG_CUSTOM_SERVER_LIST_RECV* lpMsg)
{
	int count = MAKE_NUMBERW(lpMsg->count[0], lpMsg->count[1]);

	for (int n = 0; n < count; n++)
	{
		SERVER_LIST_INFO* lpInfo = (SERVER_LIST_INFO*)(((BYTE*)lpMsg) + sizeof(PMSG_CUSTOM_SERVER_LIST_RECV) + (sizeof(SERVER_LIST_INFO) * n));

		wsprintf(gServerList.m_ServerListInfo[lpInfo->ServerCode], "%s", lpInfo->ServerName);
	}
}

void CServerList::ServerBoxTexture(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale)
{
	RenderBitmap(Texture, x + 135.0f, y, Width, Height, u, v, uWidth, vHeight, Scale, StartScale);
}

void CServerList::ServerBoxText(int iPos_x, int iPos_y, char* pszText, int iBoxWidth, int iSort, SIZE* lpTextSize)
{
	RenderText(iPos_x + 135, iPos_y, pszText, iBoxWidth, iSort, lpTextSize);
}