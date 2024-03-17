#include "stdafx.h"
#include "Connection.h"
#include "Log.h"
#include "Util.h"

CConnection::CConnection()
{
	this->m_hwnd = 0;

	this->m_socket = INVALID_SOCKET;

	this->m_EventHandlerThread = NULL;

	this->m_hEvent = NULL;
}

CConnection::~CConnection()
{
	this->Disconnect();
}

void CConnection::Init(HWND hwnd, char* name, void* function)
{
	this->m_hwnd = hwnd;

	this->sConnectionName = name;

	this->m_socket = socket(PF_INET, SOCK_STREAM, 0);

	this->wsProtocolCore = (void(*)(BYTE, BYTE*, int))function;
}

bool CConnection::Connect(char* IpAddress, WORD port)
{
	if (this->m_socket == INVALID_SOCKET)
	{
		return false;
	}

	SOCKADDR_IN target;

	target.sin_family = AF_INET;

	target.sin_port = htons(port);

	inet_pton(AF_INET, IpAddress, &target.sin_addr.s_addr);

	if (target.sin_addr.s_addr == INADDR_NONE)
	{
		char port_str[16] = {};
		sprintf_s(port_str, "%d", port);

		struct addrinfo hints = {}, * addrs;
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_TCP;

		int err = getaddrinfo(IpAddress, port_str, &hints, &addrs);

		if (err == 0)
		{
			memcpy(&target.sin_addr.s_addr, addrs->ai_addr, addrs->ai_addrlen);
		}
	}

	if (connect(this->m_socket, (SOCKADDR*)&target, sizeof(target)) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			gLog.Output(LOG_CONNECT, "[%s] connect() failed with error: %d", this->sConnectionName.c_str(), WSAGetLastError());

			this->Disconnect();

			return false;
		}
	}

	// Create event object and store the event handle and window message for later use
	this->m_hEvent = WSACreateEvent();

	if (this->m_hEvent == WSA_INVALID_EVENT)
	{
		gLog.Output(LOG_CONNECT, "[%s] WSACreateEvent() failed with error: %d", this->sConnectionName.c_str(), WSAGetLastError());

		this->Disconnect();

		return false;
	}

	// Associate the event object with the socket
	if (WSAEventSelect(this->m_socket, this->m_hEvent, FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
	{
		// Handle event association error
		WSACloseEvent(this->m_hEvent);

		gLog.Output(LOG_CONNECT, "[%s] WSAEventSelect() failed with error: %d", this->sConnectionName.c_str(), WSAGetLastError());

		this->Disconnect();

		return false;
	}

	if (this->CreateEventHandler() == false)
	{
		this->Disconnect();

		return false;
	}

	memset(this->m_RecvBuff, 0, sizeof(this->m_RecvBuff));

	this->m_RecvSize = 0;

	memset(this->m_SendBuff, 0, sizeof(this->m_SendBuff));

	this->m_SendSize = 0;

	return true;
}

bool CConnection::CreateEventHandler()
{
	if ((this->m_EventHandlerThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)this->SocketEventsThread, this, 0, 0)) == NULL)
	{
		LogAdd(LOG_RED, "[%s] CreateThread() failed with error: %d", this->sConnectionName.c_str(), GetLastError());

		return false;
	}

	if (SetThreadPriority(this->m_EventHandlerThread, THREAD_PRIORITY_HIGHEST) == FALSE)
	{
		LogAdd(LOG_RED, "[%s] SetThreadPriority() failed with error: %d", this->sConnectionName.c_str(), GetLastError());

		return false;
	}

	return true;
}

DWORD WINAPI CConnection::SocketEventsThread(CConnection* lpConnection)
{
	DWORD dwWait;

	while (true)
	{
		dwWait = WSAWaitForMultipleEvents(1, &lpConnection->m_hEvent, FALSE, WSA_INFINITE, FALSE);

		if (dwWait == WSA_WAIT_FAILED)
		{
			LogAdd(LOG_RED, "[%s] WSAWaitForMultipleEvents() failed with error: %d", lpConnection->sConnectionName.c_str(), GetLastError());

			break;
		}
		else if (dwWait == WSA_WAIT_EVENT_0)
		{
			// Event is signaled, check for socket events
			WSANETWORKEVENTS netEvents;

			WSAEnumNetworkEvents(lpConnection->m_socket, lpConnection->m_hEvent, &netEvents);

			if (netEvents.lNetworkEvents & FD_READ)
			{
				lpConnection->DataRecv();
			}

			if (netEvents.lNetworkEvents & FD_WRITE)
			{
				lpConnection->DataSendEx();
			}

			if (netEvents.lNetworkEvents & FD_CLOSE)
			{
				LogAdd(LOG_RED, "[%s] Disconnected", lpConnection->sConnectionName.c_str());

				if (lpConnection->m_socket != INVALID_SOCKET)
				{
					closesocket(lpConnection->m_socket);

					lpConnection->m_socket = INVALID_SOCKET;
				}

				gObjAllDisconnect();

				lpConnection->Disconnect();

				break;
			}
		}
	}

	return 0;
}

void CConnection::Disconnect()
{
	if (this->m_socket != INVALID_SOCKET)
	{
		closesocket(this->m_socket);

		this->m_socket = INVALID_SOCKET;
	}

	if (this->m_hEvent != NULL)
	{
		WSACloseEvent(this->m_hEvent);

		this->m_hEvent = NULL;
	}

	if (this->m_EventHandlerThread != NULL)
	{
		TerminateThread(this->m_EventHandlerThread, 0);

		CloseHandle(this->m_EventHandlerThread);

		this->m_EventHandlerThread = NULL;
	}
}

bool CConnection::CheckState()
{
	return ((this->m_socket == INVALID_SOCKET) ? false : true);
}

bool CConnection::DataRecv()
{
	int count = 0, size = 0, result = 0;

	if ((result = recv(this->m_socket, (char*)&this->m_RecvBuff[this->m_RecvSize], (MAX_BUFF_SIZE - this->m_RecvSize), 0)) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			return true;
		}
		else
		{
			gLog.Output(LOG_CONNECT, "[%s] recv() failed with error: %d", this->sConnectionName.c_str(), WSAGetLastError());

			this->Disconnect();

			return false;
		}
	}

	this->m_RecvSize += result;

	if (this->m_RecvSize < 3)
	{
		return true;
	}

	BYTE header, head;

	while (true)
	{
		if (this->m_RecvBuff[count] == 0xC1)
		{
			header = this->m_RecvBuff[count];

			size = this->m_RecvBuff[count + 1];

			head = this->m_RecvBuff[count + 2];
		}
		else if (this->m_RecvBuff[count] == 0xC2)
		{
			header = this->m_RecvBuff[count];

			size = MAKEWORD(this->m_RecvBuff[count + 2], this->m_RecvBuff[count + 1]);

			head = this->m_RecvBuff[count + 3];
		}
		else
		{
			gLog.Output(LOG_CONNECT, "[%s] Protocol header error (Header: %x)", this->sConnectionName.c_str(), this->m_RecvBuff[count]);

			this->Disconnect();

			return false;
		}

		if (size < 3 || size > MAX_BUFF_SIZE)
		{
			gLog.Output(LOG_CONNECT, "[%s] Protocol size error (Header: %x, Size: %d, Head: %x)", this->sConnectionName.c_str(), header, size, head);

			this->Disconnect();

			return false;
		}

		if (size <= this->m_RecvSize)
		{
			this->wsProtocolCore(head, &this->m_RecvBuff[count], size);

			count += size;

			this->m_RecvSize -= size;

			if (this->m_RecvSize <= 0)
			{
				break;
			}
		}
		else
		{
			if (count > 0 && this->m_RecvSize > 0 && this->m_RecvSize <= (MAX_BUFF_SIZE - count))
			{
				memmove(this->m_RecvBuff, &this->m_RecvBuff[count], this->m_RecvSize);
			}

			break;
		}
	}

	return true;
}

bool CConnection::DataSend(BYTE* lpMsg, int size)
{
	this->m_critical.lock();

	if (this->m_socket == INVALID_SOCKET)
	{
		this->m_critical.unlock();

		return false;
	}

	if (this->m_SendSize > 0)
	{
		if ((this->m_SendSize + size) > MAX_BUFF_SIZE)
		{
			gLog.Output(LOG_CONNECT, "[%s] Max msg size (Type: 1, Size: %d)", this->sConnectionName.c_str(), (this->m_SendSize + size));

			this->Disconnect();

			this->m_critical.unlock();

			return false;
		}
		else
		{
			memcpy(&this->m_SendBuff[this->m_SendSize], lpMsg, size);

			this->m_SendSize += size;

			this->m_critical.unlock();

			return true;
		}
	}

	int count = 0, result = 0;

	while (size > 0)
	{
		if ((result = send(this->m_socket, (char*)&lpMsg[count], size, 0)) == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				if ((this->m_SendSize + size) > MAX_BUFF_SIZE)
				{
					gLog.Output(LOG_CONNECT, "[%s] Max msg size (Type: 2, Size: %d)", this->sConnectionName.c_str(), (this->m_SendSize + size));

					this->Disconnect();

					this->m_critical.unlock();

					return false;
				}
				else
				{
					memcpy(&this->m_SendBuff[this->m_SendSize], &lpMsg[count], size);

					this->m_SendSize += size;

					this->m_critical.unlock();

					return true;
				}
			}
			else
			{
				gLog.Output(LOG_CONNECT, "[%s] send() failed with error: %d", this->sConnectionName.c_str(), WSAGetLastError());

				this->Disconnect();

				this->m_critical.unlock();

				return false;
			}
		}
		else
		{
			count += result;

			size -= result;
		}
	}

	this->m_critical.unlock();

	return true;
}

bool CConnection::DataSendEx()
{
	this->m_critical.lock();

	int count = 0, result = 0;

	while (this->m_SendSize > 0)
	{
		if ((result = send(this->m_socket, (char*)&this->m_SendBuff[count], this->m_SendSize, 0)) == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				memmove(this->m_SendBuff, &this->m_SendBuff[count], this->m_SendSize);

				this->m_critical.unlock();

				return true;
			}
			else
			{
				gLog.Output(LOG_CONNECT, "[%s] send() failed with error: %d", this->sConnectionName.c_str(), WSAGetLastError());

				this->Disconnect();

				this->m_critical.unlock();

				return false;
			}
		}
		else
		{
			count += result;

			this->m_SendSize -= result;
		}
	}

	this->m_critical.unlock();

	return true;
}