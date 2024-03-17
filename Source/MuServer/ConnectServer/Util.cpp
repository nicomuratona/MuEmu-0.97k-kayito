#include "stdafx.h"
#include "Util.h"
#include "ClientManager.h"
#include "SocketManager.h"

int gClientCount = 0;

void ErrorMessageBox(char* message, ...)
{
	char buff[256];

	memset(buff, 0, sizeof(buff));

	va_list arg;

	va_start(arg, message);

	vsprintf_s(buff, message, arg);

	va_end(arg);

	MessageBox(0, buff, "Error", MB_OK | MB_ICONERROR);

	ExitProcess(0);
}

void LogAdd(eLogColor color, char* text, ...)
{
	SYSTEMTIME now;

	GetLocalTime(&now);

	char time[10];

	wsprintf(time, "%02d:%02d:%02d", now.wHour, now.wMinute, now.wSecond);

	char temp[1024];

	va_list arg;

	va_start(arg, text);

	vsprintf_s(temp, text, arg);

	va_end(arg);

	char log[1024];

	wsprintf(log, "[%s] %s", time, temp);

	gServerDisplayer.LogAddText(color, log, strlen(log));
}

void ConsoleProtocolLog(int type, BYTE* lpMsg, int size)
{
	BYTE head, subhead;

	BYTE header = lpMsg[0];

	if (header == 0xC1 || header == 0xC3)
	{
		head = lpMsg[2];
	}
	else if (header == 0xC2 || header == 0xC4)
	{
		head = lpMsg[3];
	}

	subhead = ((header == 0xC1) ? lpMsg[3] : lpMsg[4]);

	gConsole.Output(type, "[%s] Header: 0x%02X, Head: 0x%02X, SubHead: 0x%02X, Size: %d", (type == CON_PROTO_TCP_RECV) ? "RECV" : "SEND", header, head, subhead, size);
}

void ConnectServerTimeoutProc()
{
	for (int n = 0; n < MAX_CLIENT; n++)
	{
		if (gClientManager[n].CheckState() != false)
		{
			if (gClientManager[n].CheckOnlineTime() == false)
			{
				gSocketManager.Disconnect(n);
			}
		}
	}
}

int GetFreeClientIndex()
{
	int index = -1;

	int count = gClientCount;

	if (SearchFreeClientIndex(&index, 0, MAX_CLIENT, 10000) != 0)
	{
		return index;
	}

	for (int n = 0; n < MAX_CLIENT; n++)
	{
		if (gClientManager[count].CheckState() == false)
		{
			return count;
		}
		else
		{
			count = (((++count) >= MAX_CLIENT) ? 0 : count);
		}
	}

	return -1;
}

int SearchFreeClientIndex(int* index, int MinIndex, int MaxIndex, DWORD MinTime)
{
	DWORD CurOnlineTime = 0;

	DWORD MaxOnlineTime = 0;

	for (int n = MinIndex; n < MaxIndex; n++)
	{
		if (gClientManager[n].CheckState() == false && gClientManager[n].CheckAlloc() != false)
		{
			if ((CurOnlineTime = (GetTickCount() - gClientManager[n].m_OnlineTime)) > MinTime && CurOnlineTime > MaxOnlineTime)
			{
				(*index) = n;

				MaxOnlineTime = CurOnlineTime;
			}
		}
	}

	return (((*index) == -1) ? 0 : 1);
}