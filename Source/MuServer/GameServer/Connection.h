#pragma once

#include "CriticalSection.h"

#define MAX_BUFF_SIZE 524288

class CConnection
{
public:

	CConnection();

	virtual ~CConnection();

	void Init(HWND hwnd, char* name, void* function);

	bool Connect(char* IpAddress, WORD port);

	void Disconnect();

	bool CheckState();

	bool DataRecv();

	bool DataSend(BYTE* lpMsg, int size);

	bool DataSendEx();

private:

	bool CreateEventHandler();

	static DWORD WINAPI SocketEventsThread(CConnection* lpConnection);

private:

	std::string sConnectionName;

	HWND m_hwnd;

	SOCKET m_socket;

	HANDLE m_EventHandlerThread;

	WSAEVENT m_hEvent;

	BYTE m_RecvBuff[MAX_BUFF_SIZE];

	int m_RecvSize;

	BYTE m_SendBuff[MAX_BUFF_SIZE];

	int m_SendSize;

	void(*wsProtocolCore)(BYTE, BYTE*, int);

	CCriticalSection m_critical;
};