#include "stdafx.h"
#include "ServerList.h"
#include "MemScript.h"
#include "Util.h"

CServerList gServerList;

CServerList::CServerList()
{
	this->m_JoinServerState = false;

	this->m_JoinServerStateTime = 0;

	this->m_JoinServerQueueSize = 0;

	this->m_ServerListInfo.clear();
}

CServerList::~CServerList()
{

}

void CServerList::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == NULL)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (!lpMemScript->SetBuffer(path))
	{
		ErrorMessageBox(lpMemScript->GetLastError());

		delete lpMemScript;

		return;
	}

	this->m_ServerListInfo.clear();

	try
	{
		eTokenResult token;

		while (true)
		{
			token = lpMemScript->GetToken();

			if (token == TOKEN_END || token == TOKEN_END_SECTION)
			{
				break;
			}

			SERVER_LIST_INFO info;

			info.ServerCode = lpMemScript->GetNumber();

			strcpy_s(info.ServerName, lpMemScript->GetAsString());

			strcpy_s(info.ServerAddress, lpMemScript->GetAsString());

			info.ServerPort = lpMemScript->GetAsNumber();

			info.ServerShow = ((strcmp(lpMemScript->GetAsString(), "SHOW") == 0) ? true : false);

			info.ServerState = false;

			info.ServerStateTime = 0;

			info.UserTotal = 0;

			info.UserCount = 0;

			info.AccountCount = 0;

			info.MaxUserCount = 0;

			this->m_ServerListInfo.insert(std::pair<int, SERVER_LIST_INFO>(info.ServerCode, info));
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	LogAdd(LOG_BLUE, "[ServerList] ServerList loaded successfully");

	delete lpMemScript;
}

void CServerList::MainProc()
{
	if (this->m_JoinServerState != false && (GetTickCount() - this->m_JoinServerStateTime) > 10000)
	{
		this->m_JoinServerState = false;

		this->m_JoinServerStateTime = 0;

		LogAdd(LOG_RED, "[ServerList] JoinServer offline");
	}

	for (std::map<int, SERVER_LIST_INFO>::iterator it = this->m_ServerListInfo.begin(); it != this->m_ServerListInfo.end(); it++)
	{
		if (it->second.ServerState != false && (GetTickCount() - it->second.ServerStateTime) > 10000)
		{
			it->second.ServerState = false;

			it->second.ServerStateTime = 0;

			LogAdd(LOG_BLACK, "[ServerList] GameServer offline (%s) (%d)", it->second.ServerName, it->second.ServerCode);
		}
	}
}

bool CServerList::CheckJoinServerState()
{
	if (this->m_JoinServerState == false)
	{
		return false;
	}

	if (this->m_JoinServerQueueSize > MAX_JOIN_SERVER_QUEUE_SIZE)
	{
		return false;
	}

	return true;
}

long CServerList::GenerateCustomServerList(BYTE* lpMsg, int* size)
{
	int count = 0;

	PMSG_CUSTOM_SERVER_LIST info;

	if (this->CheckJoinServerState() != 0)
	{
		for (std::map<int, SERVER_LIST_INFO>::iterator it = this->m_ServerListInfo.begin(); it != this->m_ServerListInfo.end(); it++)
		{
			if (it->second.ServerShow != 0 && it->second.ServerState != 0)
			{
				info.ServerCode = it->second.ServerCode;

				strcpy_s(info.ServerName, it->second.ServerName);

				memcpy(&lpMsg[(*size)], &info, sizeof(info));

				(*size) += sizeof(info);

				count++;
			}
		}
	}

	return count;
}

long CServerList::GenerateServerList(BYTE* lpMsg, int* size)
{
	int count = 0;

	PMSG_SERVER_LIST info;

	if (this->CheckJoinServerState() != false)
	{
		for (std::map<int, SERVER_LIST_INFO>::iterator it = this->m_ServerListInfo.begin(); it != this->m_ServerListInfo.end(); it++)
		{
			if (it->second.ServerShow != false && it->second.ServerState != false)
			{
				info.ServerCode = it->second.ServerCode;

				info.UserTotal = it->second.UserTotal;

				memcpy(&lpMsg[(*size)], &info, sizeof(info));

				(*size) += sizeof(info);

				count++;
			}
		}
	}

	return count;
}

SERVER_LIST_INFO* CServerList::GetServerListInfo(int ServerCode)
{
	std::map<int, SERVER_LIST_INFO>::iterator it = this->m_ServerListInfo.find(ServerCode);

	if (it == this->m_ServerListInfo.end())
	{
		return 0;
	}
	else
	{
		return &it->second;
	}
}

void CServerList::ServerProtocolCore(BYTE head, BYTE* lpMsg, int size)
{
	switch (head)
	{
		case 0x01:
		{
			this->GCGameServerLiveRecv((SDHP_GAME_SERVER_LIVE_RECV*)lpMsg);

			break;
		}

		case 0x02:
		{
			this->JCJoinServerLiveRecv((SDHP_JOIN_SERVER_LIVE_RECV*)lpMsg);

			break;
		}
	}
}

void CServerList::GCGameServerLiveRecv(SDHP_GAME_SERVER_LIVE_RECV* lpMsg)
{
	SERVER_LIST_INFO* lpServerListInfo = this->GetServerListInfo(lpMsg->ServerCode);

	if (lpServerListInfo == 0)
	{
		return;
	}

	if (lpServerListInfo->ServerState == false)
	{
		LogAdd(LOG_BLACK, "[ServerList] GameServer online (%s) (%d)", lpServerListInfo->ServerName, lpServerListInfo->ServerCode);
	}

	lpServerListInfo->ServerState = true;

	lpServerListInfo->ServerStateTime = GetTickCount();

	lpServerListInfo->UserTotal = lpMsg->UserTotal;

	lpServerListInfo->UserCount = lpMsg->UserCount;

	lpServerListInfo->AccountCount = lpMsg->AccountCount;

	lpServerListInfo->MaxUserCount = lpMsg->MaxUserCount;
}

void CServerList::JCJoinServerLiveRecv(SDHP_JOIN_SERVER_LIVE_RECV* lpMsg)
{
	if (this->m_JoinServerState == false)
	{
		LogAdd(LOG_GREEN, "[ServerList] JoinServer online");
	}

	this->m_JoinServerState = true;

	this->m_JoinServerStateTime = GetTickCount();

	this->m_JoinServerQueueSize = lpMsg->QueueSize;
}