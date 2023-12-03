#include "stdafx.h"
#include "Reconnect.h"
#include "HackCheck.h"
#include "Protect.h"
#include "Protocol.h"

CReconnect gReconnect;

CReconnect::CReconnect()
{
	memset(this->GameServerAddress, 0, sizeof(GameServerAddress));

	this->GameServerPort = 0;

	memset(this->ReconnectAccount, 0, sizeof(ReconnectAccount));

	memset(this->ReconnectPassword, 0, sizeof(ReconnectPassword));

	memset(this->ReconnectName, 0, sizeof(ReconnectName));

	this->ReconnectStatus = RECONNECT_STATUS_NONE;

	this->ReconnectProgress = RECONNECT_PROGRESS_NONE;

	this->ReconnectCurTime = 0;

	this->ReconnectMaxTime = 0;

	this->ReconnectCurWait = 0;

	this->ReconnectMaxWait = 0;

	this->ReconnectAuthSend = 0;
}

CReconnect::~CReconnect()
{

}

void CReconnect::Init()
{
	if (gProtect.m_MainInfo.ReconnectTime == 0)
	{
		return;
	}

	SetCompleteHook(0xE8, 0x00525995, &this->ReconnectMainProc);

	SetCompleteHook(0xE8, 0x00525CC1, &this->ReconnectDrawInterface);

	SetCompleteHook(0xE9, 0x00520428, &this->ReconnectGetAccountInfo);

	SetCompleteHook(0xE9, 0x00526A9E, &this->ReconnectCheckConnection);

	SetCompleteHook(0xE9, 0x0043DC90, &this->ReconnectCloseSocket);

	SetCompleteHook(0xE9, 0x005144DC, &this->ReconnectMenuExitGame);

	MemorySet(0x0042398E, 0x90, 0x0A); // Fix Reconnect Exit Message

	SetCompleteHook(0xE9, 0x0042395B, &this->ReconnectServerConnect);

	SetCompleteHook(0xE8, 0x00423FB5, &this->ReconnectCreateConnection);

	SetCompleteHook(0xE8, 0x00439BED, &this->ReconnectCreateConnection);

	SetCompleteHook(0xE8, 0x0051F953, &this->ReconnectCreateConnection);

	SetCompleteHook(0xE8, 0x00520212, &this->ReconnectCreateConnection);
}

void CReconnect::ReconnectDrawInterface()
{
	((void(_cdecl*)())0x0051AF50)();

	if (gReconnect.ReconnectStatus != RECONNECT_STATUS_RECONNECT)
	{
		return;
	}

	float MaxWidth = 150.0f;

	float MaxHeight = 18.0f;

	float progress = ((gReconnect.ReconnectMaxWait == 0) ? 0 : (((GetTickCount() - gReconnect.ReconnectMaxTime) * MaxWidth) / (float)gReconnect.ReconnectMaxWait));

	progress = ((progress > MaxWidth) ? MaxWidth : progress);

	float PosX = ImgCenterScreenPosX(MaxWidth);

	float PosY = ImgCenterScreenPosY(MaxHeight);

	EnableAlphaTest(true);

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

	RenderColor(PosX, PosY, MaxWidth + 10.0f, MaxHeight);

	glColor3f(1.0f, 0.0f, 0.0f);

	RenderColor(PosX + 5.0f, PosY + 5.0f, progress, MaxHeight - 10.0f);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);

	EnableAlphaTest(false);

	char buff[256];

	switch (gReconnect.ReconnectProgress)
	{
		case RECONNECT_PROGRESS_NONE:
		{
			wsprintf(buff, "Reconnecting...");

			break;
		}

		case RECONNECT_PROGRESS_CONNECTED:
		{
			wsprintf(buff, "Logging in...");

			break;
		}

		case RECONNECT_PROGRESS_JOINED:
		{
			wsprintf(buff, "Waiting character list...");

			break;
		}

		case RECONNECT_PROGRESS_CHAR_LIST:
		{
			wsprintf(buff, "Waiting character info...");

			break;
		}
	}

	EnableAlphaTest(true);

	SelectObject(m_hFontDC, g_hFont);

	SetBackgroundTextColor = Color4b(255, 255, 255, 0);

	SetTextColor = Color4b(255, 255, 255, 255);

	RenderText((int)PosX, (int)PosY + 5, buff, (int)MaxWidth * WindowWidth / 640, RT3_SORT_CENTER, NULL);

	DisableAlphaBlend();
}

void CReconnect::ReconnectMainProc()
{
	((void(_cdecl*)())0x004794A0)();

	if (gReconnect.ReconnectStatus != RECONNECT_STATUS_RECONNECT)
	{
		return;
	}

	ErrorMessage = 0x1B;

	if ((GetTickCount() - gReconnect.ReconnectMaxTime) > gReconnect.ReconnectMaxWait)
	{
		gReconnect.ReconnectSetInfo(RECONNECT_STATUS_DISCONNECT, RECONNECT_PROGRESS_NONE, 0, 0);

		gReconnect.ReconnectCloseSocket(SocketClient);

		return;
	}

	if ((GetTickCount() - gReconnect.ReconnectCurTime) < gReconnect.ReconnectCurWait)
	{
		return;
	}

	switch (gReconnect.ReconnectProgress)
	{
		case RECONNECT_PROGRESS_NONE:
		{
			gReconnect.ReconnectToGameServer();

			break;
		}

		case RECONNECT_PROGRESS_CONNECTED:
		{
			gReconnect.ReconnectIntoAccount();

			break;
		}
	}

	gReconnect.ReconnectCurTime = GetTickCount();
}

void CReconnect::ReconnectToGameServer()
{
	if (this->ReconnectCreateConnection(this->GameServerAddress, this->GameServerPort) != FALSE)
	{
		*(DWORD*)(g_bGameServerConnected) = TRUE;

		this->ReconnectSetInfo(RECONNECT_STATUS_RECONNECT, RECONNECT_PROGRESS_CONNECTED, 10000, 30000);
	}
}

void CReconnect::ReconnectIntoAccount()
{
	if (((this->ReconnectAuthSend == 0) ? (this->ReconnectAuthSend++) : this->ReconnectAuthSend) != 0)
	{
		return;
	}

	PMSG_CONNECT_ACCOUNT_SEND pMsg;

	pMsg.header.setE(0xF1, 0x01, sizeof(pMsg));

	PacketArgumentEncrypt((BYTE*)pMsg.account, (BYTE*)this->ReconnectAccount, (sizeof(this->ReconnectAccount) - 1));

	PacketArgumentEncrypt((BYTE*)pMsg.password, (BYTE*)this->ReconnectPassword, (sizeof(this->ReconnectPassword) - 1));

	pMsg.TickCount = GetTickCount();

	pMsg.ClientVersion[0] = (*(BYTE*)(0x0055961C)) - 1;

	pMsg.ClientVersion[1] = (*(BYTE*)(0x0055961D)) - 2;

	pMsg.ClientVersion[2] = (*(BYTE*)(0x0055961E)) - 3;

	pMsg.ClientVersion[3] = (*(BYTE*)(0x0055961F)) - 4;

	pMsg.ClientVersion[4] = (*(BYTE*)(0x00559620)) - 5;

	memcpy(pMsg.ClientSerial, (void*)0x00559624, sizeof(pMsg.ClientSerial));

	gProtocol.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

__declspec(naked) void CReconnect::ReconnectGetAccountInfo()
{
	static DWORD jmpBack = 0x00520442;

	_asm
	{
		Pushad;
	}

	memcpy(gReconnect.ReconnectAccount, (char*)0x07DB8710, sizeof(gReconnect.ReconnectAccount));

	memcpy(gReconnect.ReconnectPassword, (char*)0x07DB8810, sizeof(gReconnect.ReconnectPassword));

	_asm
	{
		Popad;
		Push 0x07DB8710;
		Push 0x005619C0;
		Push 0x055C9BF0;
		Mov Dword Ptr Ds : [0x083A7AC8] , Eax;
		Mov Dword Ptr DS : [0x083A4320] , Ebx;
		Jmp[jmpBack];
	}
}

__declspec(naked) void CReconnect::ReconnectCheckConnection()
{
	static DWORD jmpToSocketInvalid = 0x00526AA3;
	static DWORD jmpToSocketValid = 0x00526ADE;

	_asm
	{
		Cmp Eax, -1;
		Jnz EXIT;
		Mov Ecx, gReconnect.ReconnectStatus;
		Cmp Ecx, RECONNECT_STATUS_RECONNECT;
		Je EXIT;
		Jmp[jmpToSocketInvalid];
	EXIT:
		Jmp[jmpToSocketValid];
	}
}

BOOL CReconnect::ReconnectCloseSocket(DWORD This)
{
	if (*(BOOL*)g_bGameServerConnected != FALSE)
	{
		if (gHackCheck.CheckSocketPort(pSocket))
		{
			gReconnect.ReconnectOnCloseSocket();
		}

		*(BOOL*)g_bGameServerConnected = FALSE;
	}

	closesocket(pSocket);

	pSocket = -1;

	return TRUE;
}

void CReconnect::ReconnectOnCloseSocket()
{
	if (SceneFlag == 5 && this->ReconnectStatus != RECONNECT_STATUS_DISCONNECT)
	{
		this->ReconnectSetInfo(RECONNECT_STATUS_RECONNECT, RECONNECT_PROGRESS_NONE, 30000, gProtect.m_MainInfo.ReconnectTime);

		this->ReconnectAuthSend = 0;

		this->ReconnectViewportDestroy();

		PartyNumber = 0;

		memcpy(this->ReconnectName, (char*)(Hero + 0x1C1), sizeof(this->ReconnectName));
	}
}

void CReconnect::ReconnectSetInfo(eReconnectStatus status, eReconnectProgress progress, DWORD CurWait, DWORD MaxWait)
{
	this->ReconnectStatus = status;

	this->ReconnectProgress = progress;

	this->ReconnectCurTime = GetTickCount();

	this->ReconnectMaxTime = GetTickCount();

	this->ReconnectCurWait = CurWait;

	this->ReconnectMaxWait = MaxWait;

	this->ReconnectAuthSend = ((status == RECONNECT_STATUS_NONE) ? 0 : ReconnectAuthSend);
}

void CReconnect::ReconnectViewportDestroy()
{
	DWORD ViewportAddress = 0;

	for (DWORD count = 0; count < MAX_MAIN_VIEWPORT; count++)
	{
		if ((ViewportAddress = (CharactersClient + (count * 916))) == 0)
		{
			continue;
		}

		if (*(BYTE*)ViewportAddress == 0)
		{
			continue;
		}

		DeleteCharacter(*(WORD*)(ViewportAddress + 0x1DC));
	}
}

__declspec(naked) void CReconnect::ReconnectMenuExitGame()
{
	static DWORD jmpBack = 0x005144E1;

	_asm
	{
		Mov Eax, gReconnect.ReconnectStatus;
		Cmp Eax, RECONNECT_STATUS_RECONNECT;
		Jnz EXIT;
		Push 0;
		Call[ExitProcess];
	EXIT:
		Mov gReconnect.ReconnectStatus, RECONNECT_STATUS_DISCONNECT;
		Push 0x005617BC;
		Jmp[jmpBack];
	}
}

__declspec(naked) void CReconnect::ReconnectServerConnect()
{
	static DWORD jmpBack = 0x00423962;

	_asm
	{
		Mov gReconnect.GameServerPort, Cx;
		Lea Ecx, gReconnect.GameServerAddress;
		Push 0x10;
		Push Edx;
		Push Ecx;
		Call[MemoryCpy];
		Add Esp, 0x0C;
		Movzx Ecx, gReconnect.GameServerPort;
		Lea Edx, gReconnect.GameServerAddress;
		Push 0x400;
		Push Ecx;
		Push Edx;
		Jmp[jmpBack];
	}
}

BOOL CReconnect::ReconnectCreateConnection(char* IpAddr, unsigned short Port)
{
	if (PORT_RANGE(Port) != 0 && gReconnect.GameServerAddress != IpAddr)
	{
		wsprintf(gReconnect.GameServerAddress, "%s", IpAddr);

		gReconnect.GameServerPort = Port;
	}

	return CreateSocket(IpAddr, Port);
}

void CReconnect::ReconnectOnConnectAccount(BYTE result)
{
	if (this->ReconnectProgress == RECONNECT_PROGRESS_CONNECTED)
	{
		if (this->ReconnectAuthSend != 0)
		{
			if (result == 1)
			{
				PMSG_CHARACTER_LIST_SEND pMsg;

				pMsg.header.set(0xF3, 0x00, sizeof(pMsg));

				gProtocol.DataSend((BYTE*)&pMsg, pMsg.header.size);

				this->ReconnectSetInfo(RECONNECT_STATUS_RECONNECT, RECONNECT_PROGRESS_JOINED, 30000, 30000);
			}
			else
			{
				if (result == 3)
				{
					this->ReconnectSetInfo(RECONNECT_STATUS_RECONNECT, RECONNECT_PROGRESS_CONNECTED, 10000, 30000);

					this->ReconnectAuthSend = 0;
				}
				else
				{
					this->ReconnectSetInfo(RECONNECT_STATUS_DISCONNECT, RECONNECT_PROGRESS_NONE, 0, 0);

					this->ReconnectCloseSocket(SocketClient);
				}
			}
		}
	}
}

void CReconnect::ReconnectOnCloseClient(BYTE result)
{
	if (this->ReconnectStatus != RECONNECT_STATUS_RECONNECT)
	{
		if (result == 0 || result == 2)
		{
			this->ReconnectSetInfo(RECONNECT_STATUS_DISCONNECT, RECONNECT_PROGRESS_NONE, 0, 0);
		}
	}
}

void CReconnect::ReconnectOnCharacterList()
{
	if (this->ReconnectProgress == RECONNECT_PROGRESS_JOINED)
	{
		PMSG_CHARACTER_INFO_SEND pMsg;

		pMsg.header.set(0xF3, 0x03, sizeof(pMsg));

		memcpy(pMsg.name, this->ReconnectName, sizeof(pMsg.name));

		gProtocol.DataSend((BYTE*)&pMsg, pMsg.header.size);

		this->ReconnectSetInfo(RECONNECT_STATUS_RECONNECT, RECONNECT_PROGRESS_CHAR_LIST, 30000, 30000);
	}
}

void CReconnect::ReconnectOnCharacterInfo()
{
	this->ReconnectSetInfo(RECONNECT_STATUS_NONE, RECONNECT_PROGRESS_NONE, 0, 0);

	ErrorMessage = 0;
}