#include "stdafx.h"
#include "PingSystem.h"
#include "Reconnect.h"

CPingSystem gPing;

CPingSystem::CPingSystem()
{
	this->m_Ping = 0;
}

CPingSystem::~CPingSystem()
{

}

void CPingSystem::Init()
{
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)this->MainThread, 0, 0, 0);
}

DWORD WINAPI CPingSystem::MainThread(CPingSystem* self)
{
	while (true)
	{
		if (g_bGameServerConnected)
		{
			gPing.SetPing();
		}

		Sleep(1000);
	}

	return 0;
}

/******************************************************************************/
/******************* This code belongs to the Ogocx source ********************/
/******************************************************************************/
void CPingSystem::SetPing()
{
	char SendData[32] = "Data Buffer";

	SOCKADDR_IN target;
	target.sin_family = AF_INET;
	target.sin_port = htons(gReconnect.GameServerPort);

	inet_pton(AF_INET, gReconnect.GameServerAddress, &target.sin_addr.s_addr);

	if (target.sin_addr.s_addr == INADDR_NONE)
	{
		return;
	}

	HANDLE hIcmpFile = IcmpCreateFile();
	if (hIcmpFile == INVALID_HANDLE_VALUE)
	{
		return;
	}

	DWORD ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);

	LPVOID ReplyBuffer = (VOID*)malloc(ReplySize);
	if (ReplyBuffer == NULL)
	{
		IcmpCloseHandle(hIcmpFile);
		return;
	}

	DWORD dwRetVal = IcmpSendEcho(hIcmpFile, target.sin_addr.s_addr, SendData, sizeof(SendData), NULL, ReplyBuffer, ReplySize, 1000);

	if (dwRetVal != 0)
	{
		PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;

		this->m_Ping = pEchoReply->RoundTripTime;
	}

	free(ReplyBuffer);
	IcmpCloseHandle(hIcmpFile);
}

/******************************************************************************/
/******************* This code belongs to the Louis source ********************/
/******************************************************************************/
void CPingSystem::SetPing2()
{
	SOCKET m_socket = socket(PF_INET, SOCK_STREAM, 0);

	if (m_socket == INVALID_SOCKET)
	{
		return;
	}

	SOCKADDR_IN target;
	memset(&target, 0, sizeof(target));
	target.sin_family = AF_INET;
	target.sin_port = htons(gReconnect.GameServerPort);

	if (inet_pton(AF_INET, gReconnect.GameServerAddress, &target.sin_addr.s_addr) <= 0)
	{
		char port_str[16];
		sprintf_s(port_str, sizeof(port_str), "%d", gReconnect.GameServerPort);

		struct addrinfo hints = {}, * addrs = nullptr;
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		int err = getaddrinfo(gReconnect.GameServerAddress, port_str, &hints, &addrs);
		if (err != 0 || addrs == nullptr)
		{
			closesocket(m_socket);
			return;
		}

		target = *(reinterpret_cast<SOCKADDR_IN*>(addrs->ai_addr));
		freeaddrinfo(addrs);
	}

	DWORD PingDelaySend = GetTickCount();

	if (connect(m_socket, reinterpret_cast<SOCKADDR*>(&target), sizeof(target)) != SOCKET_ERROR)
	{
		this->m_Ping = GetTickCount() - PingDelaySend;
	}

	closesocket(m_socket);
	m_socket = INVALID_SOCKET;
}