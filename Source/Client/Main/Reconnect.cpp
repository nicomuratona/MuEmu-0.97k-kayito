#include "stdafx.h"
#include "Reconnect.h"
#include "HackCheck.h"
#include "Protect.h"
#include "Protocol.h"

char GameServerAddress[16];

WORD GameServerPort;

char ReconnectAccount[11];

char ReconnectPassword[11];

char ReconnectName[11];

DWORD ReconnectStatus = RECONNECT_STATUS_NONE;

DWORD ReconnectProgress = RECONNECT_PROGRESS_NONE;

DWORD ReconnectCurTime = 0;

DWORD ReconnectMaxTime = 0;

DWORD ReconnectCurWait = 0;

DWORD ReconnectMaxWait = 0;

DWORD ReconnectAuthSend = 0;

void InitReconnect()
{
	if (gProtect.m_MainInfo.ReconnectTime == 0)
	{
		return;
	}

	SetCompleteHook(0xE9, 0x00520428, &ReconnectGetAccountInfo);

	SetCompleteHook(0xE9, 0x00526A9E, &ReconnectCheckConnection);

	SetCompleteHook(0xE9, 0x0043DC93, &ReconnectCloseSocket);

	SetCompleteHook(0xE9, 0x005144DC, &ReconnectMenuExitGame);

	MemorySet(0x0042398E, 0x90, 0x0A); // Fix Reconnect Exit Message

	SetCompleteHook(0xE9, 0x0042395B, &ReconnectServerConnect);

	SetCompleteHook(0xE8, 0x00423FB5, &ReconnectCreateConnection);

	SetCompleteHook(0xE8, 0x00439BED, &ReconnectCreateConnection);

	SetCompleteHook(0xE8, 0x0051F953, &ReconnectCreateConnection);

	SetCompleteHook(0xE8, 0x00520212, &ReconnectCreateConnection);

	//SetCompleteHook(0xE8, 0x004BC0EE, &ReconnectMainProc); // Interface::Work()
}

__declspec(naked) void ReconnectGetAccountInfo()
{
	static DWORD ReconnectGetAccountInfoAddress1 = 0x00405540;
	static DWORD ReconnectGetAccountInfoAddress2 = 0x00520447;

	_asm
	{
		Push 0x07DB8710;
		Push 0x005619C0;
		Push 0x055C9BF0;
		Mov Dword Ptr Ds : [0x083A7AC8] , Eax;
		Mov Dword Ptr DS : [0x083A4320] , Ebx;
		Call[ReconnectGetAccountInfoAddress1];
		Push 0x0A;
		Lea Eax, Dword Ptr Ss : [0x07DB8710] ;
		Push Eax;
		Lea Ecx, ReconnectAccount;
		Push Ecx;
		Call[MemoryCpy];
		Add Esp, 0x0C;
		Push 0x0A;
		Lea Edx, Dword Ptr Ss : [0x07DB8810] ;
		Push Edx;
		Lea Eax, ReconnectPassword;
		Push Eax;
		Call[MemoryCpy];
		Add Esp, 0x0C;
		Jmp[ReconnectGetAccountInfoAddress2];
	}
}

__declspec(naked) void ReconnectCheckConnection()
{
	static DWORD ReconnectCheckConnectionAddress1 = 0x00526AA3;
	static DWORD ReconnectCheckConnectionAddress2 = 0x00526ADE;

	_asm
	{
		Cmp Eax, -1;
		Jnz EXIT;
		Mov Ecx, ReconnectStatus;
		Cmp Ecx, RECONNECT_STATUS_RECONNECT;
		Je EXIT;
		Jmp[ReconnectCheckConnectionAddress1];
	EXIT:
		Jmp[ReconnectCheckConnectionAddress2];
	}
}

__declspec(naked) void ReconnectCloseSocket()
{
	static DWORD ReconnectCloseSocketAddress1 = 0x0043DC9D;

	_asm
	{
		Mov Eax, Dword Ptr Ds : [MAIN_CONNECTION_STATUS] ;
		Cmp Eax, 0x00;
		Je EXIT;
		Call[CheckSocketPort];
		Test Eax, Eax;
		Je EXIT;
		Call[ReconnectOnCloseSocket];
	EXIT:
		Mov Dword Ptr Ds : [MAIN_CONNECTION_STATUS] , 0x00;
		Jmp[ReconnectCloseSocketAddress1];
	}
}

void ReconnectOnCloseSocket()
{
	if (SceneFlag == 5 && ReconnectStatus != RECONNECT_STATUS_DISCONNECT)
	{
		ReconnectSetInfo(RECONNECT_STATUS_RECONNECT, RECONNECT_PROGRESS_NONE, 30000, gProtect.m_MainInfo.ReconnectTime);

		ReconnectAuthSend = 0;

		ReconnectViewportDestroy();

		PartyNumber = 0;

		memcpy(ReconnectName, (char*)(Hero + 0x1C1), sizeof(ReconnectName));
	}
}

void ReconnectViewportDestroy()
{
	DWORD ViewportAddress = 0;

	for (DWORD count = 0; count < MAX_MAIN_VIEWPORT; count++)
	{
		if ((ViewportAddress = (*(DWORD*)(0x07ABF5D0) + (count * 916))) == 0)
		{
			continue;
		}

		if (*(BYTE*)ViewportAddress == 0)
		{
			continue;
		}

		((int(__cdecl*)(int key)) 0x0045AC20)(*(WORD*)(ViewportAddress + 0x1DC));
	}
}

__declspec(naked) void ReconnectMenuExitGame()
{
	static DWORD ReconnectMenuExitGameAddress1 = 0x005144E1;

	_asm
	{
		Mov Eax, ReconnectStatus;
		Cmp Eax, RECONNECT_STATUS_RECONNECT;
		Jnz EXIT;
		Push 0;
		Call[ExitProcess];
	EXIT:
		Mov ReconnectStatus, RECONNECT_STATUS_DISCONNECT;
		Push 0x005617BC;
		Jmp[ReconnectMenuExitGameAddress1];
	}
}

__declspec(naked) void ReconnectServerConnect()
{
	static DWORD ReconnectServerConnectAddress1 = 0x00423962;

	_asm
	{
		Mov GameServerPort, Cx;
		Lea Ecx, GameServerAddress;
		Push 0x10;
		Push Edx;
		Push Ecx;
		Call[MemoryCpy];
		Add Esp, 0x0C;
		Movzx Ecx, GameServerPort;
		Lea Edx, GameServerAddress;
		Push 0x400;
		Push Ecx;
		Push Edx;
		Jmp[ReconnectServerConnectAddress1];
	}
}

BOOL ReconnectCreateConnection(char* address, WORD port)
{
	if (PORT_RANGE(port) != 0 && GameServerAddress != address)
	{
		wsprintf(GameServerAddress, "%s", address);

		GameServerPort = port;
	}

	return ((BOOL(*)(char*, WORD))0x00423920)(address, port);
}

void ReconnectMainProc()
{
	// ((void(*)())0x004BD2B0)(); // Interface::Work()

	if (SceneFlag != 5)
	{
		return;
	}

	if (ReconnectStatus != RECONNECT_STATUS_RECONNECT)
	{
		return;
	}

	ReconnectDrawInterface();

	if ((GetTickCount() - ReconnectMaxTime) > ReconnectMaxWait)
	{
		ReconnectSetInfo(RECONNECT_STATUS_DISCONNECT, RECONNECT_PROGRESS_NONE, 0, 0);

		((void(__thiscall*)(void*))0x0043DC90)((void*)0x055CA160);

		return;
	}

	if ((GetTickCount() - ReconnectCurTime) < ReconnectCurWait)
	{
		return;
	}

	switch (ReconnectProgress)
	{
		case RECONNECT_PROGRESS_NONE:
		{
			ReconnectToGameServer();

			break;
		}

		case RECONNECT_PROGRESS_CONNECTED:
		{
			ReconnectIntoAccount();

			break;
		}
	}

	ReconnectCurTime = GetTickCount();
}

void ReconnectDrawInterface()
{
	ErrorMessage = 0x1B;

	float MaxWidth = 150.0f;

	float MaxHeight = 18.0f;

	float progress = ((ReconnectMaxWait == 0) ? 0 : (((GetTickCount() - ReconnectMaxTime) * MaxWidth) / (float)ReconnectMaxWait));

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

	switch (ReconnectProgress)
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

	SetBackgroundTextColor = Color4f(255, 255, 255, 0);

	SetTextColor = Color4f(255, 255, 255, 255);

	RenderText((int)PosX, (int)PosY + 5, buff, (int)MaxWidth * WindowWidth / 640, 1, 0);

	DisableAlphaBlend();
}

void ReconnectSetInfo(DWORD status, DWORD progress, DWORD CurWait, DWORD MaxWait)
{
	ReconnectStatus = status;

	ReconnectProgress = progress;

	ReconnectCurTime = GetTickCount();

	ReconnectMaxTime = GetTickCount();

	ReconnectCurWait = CurWait;

	ReconnectMaxWait = MaxWait;

	ReconnectAuthSend = ((status == RECONNECT_STATUS_NONE) ? 0 : ReconnectAuthSend);
}

void ReconnectToGameServer()
{
	if (ReconnectCreateConnection(GameServerAddress, GameServerPort) != 0)
	{
		*(DWORD*)(MAIN_CONNECTION_STATUS) = 1;

		ReconnectSetInfo(RECONNECT_STATUS_RECONNECT, RECONNECT_PROGRESS_CONNECTED, 10000, 30000);
	}
}

void ReconnectIntoAccount()
{
	if (((ReconnectAuthSend == 0) ? (ReconnectAuthSend++) : ReconnectAuthSend) != 0)
	{
		return;
	}

	PMSG_CONNECT_ACCOUNT_SEND pMsg;

	pMsg.header.setE(0xF1, 0x01, sizeof(pMsg));

	PacketArgumentEncrypt((BYTE*)pMsg.account, (BYTE*)ReconnectAccount, (sizeof(ReconnectAccount) - 1));

	PacketArgumentEncrypt((BYTE*)pMsg.password, (BYTE*)ReconnectPassword, (sizeof(ReconnectPassword) - 1));

	pMsg.TickCount = GetTickCount();

	pMsg.ClientVersion[0] = (*(BYTE*)(0x0055961C)) - 1;

	pMsg.ClientVersion[1] = (*(BYTE*)(0x0055961D)) - 2;

	pMsg.ClientVersion[2] = (*(BYTE*)(0x0055961E)) - 3;

	pMsg.ClientVersion[3] = (*(BYTE*)(0x0055961F)) - 4;

	pMsg.ClientVersion[4] = (*(BYTE*)(0x00559620)) - 5;

	memcpy(pMsg.ClientSerial, (void*)0x00559624, sizeof(pMsg.ClientSerial));

	DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void ReconnectOnConnectAccount(BYTE result)
{
	if (ReconnectProgress == RECONNECT_PROGRESS_CONNECTED)
	{
		if (ReconnectAuthSend != 0)
		{
			if (result == 1)
			{
				PMSG_CHARACTER_LIST_SEND pMsg;

				pMsg.header.set(0xF3, 0x00, sizeof(pMsg));

				DataSend((BYTE*)&pMsg, pMsg.header.size);

				ReconnectSetInfo(RECONNECT_STATUS_RECONNECT, RECONNECT_PROGRESS_JOINED, 30000, 30000);
			}
			else
			{
				if (result == 3)
				{
					ReconnectSetInfo(RECONNECT_STATUS_RECONNECT, RECONNECT_PROGRESS_CONNECTED, 10000, 30000);

					ReconnectAuthSend = 0;
				}
				else
				{
					ReconnectSetInfo(RECONNECT_STATUS_DISCONNECT, RECONNECT_PROGRESS_NONE, 0, 0);

					((void(__thiscall*)(void*))0x0043DC90)((void*)0x055CA160);
				}
			}
		}
	}
}

void ReconnectOnCloseClient(BYTE result)
{
	if (ReconnectStatus != RECONNECT_STATUS_RECONNECT)
	{
		if (result == 0 || result == 2)
		{
			ReconnectSetInfo(RECONNECT_STATUS_DISCONNECT, RECONNECT_PROGRESS_NONE, 0, 0);
		}
	}
}

void ReconnectOnCharacterList()
{
	if (ReconnectProgress == RECONNECT_PROGRESS_JOINED)
	{
		PMSG_CHARACTER_INFO_SEND pMsg;

		pMsg.header.set(0xF3, 0x03, sizeof(pMsg));

		memcpy(pMsg.name, ReconnectName, sizeof(pMsg.name));

		DataSend((BYTE*)&pMsg, pMsg.header.size);

		ReconnectSetInfo(RECONNECT_STATUS_RECONNECT, RECONNECT_PROGRESS_CHAR_LIST, 30000, 30000);
	}
}

void ReconnectOnCharacterInfo()
{
	ReconnectSetInfo(RECONNECT_STATUS_NONE, RECONNECT_PROGRESS_NONE, 0, 0);

	ErrorMessage = 0;
}