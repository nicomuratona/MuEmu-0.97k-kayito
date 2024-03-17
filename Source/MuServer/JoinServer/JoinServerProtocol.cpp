#include "stdafx.h"
#include "JoinServerProtocol.h"
#include "MD5.h"
#include "AccountManager.h"
#include "Log.h"
#include "QueryManager.h"
#include "ServerManager.h"
#include "SocketManager.h"
#include "SocketManagerUdp.h"
#include "Util.h"

void JoinServerProtocolCore(int index, BYTE head, BYTE* lpMsg, int size)
{
	ConsoleProtocolLog(CON_PROTO_TCP_RECV, lpMsg, size);

	gServerManager[index].m_PacketTime = GetTickCount();

	switch (head)
	{
		case 0x00:
		{
			GJServerInfoRecv((SDHP_SERVER_INFO_RECV*)lpMsg, index);

			break;
		}

		case 0x01:
		{
			GJConnectAccountRecv((SDHP_CONNECT_ACCOUNT_RECV*)lpMsg, index);

			break;
		}

		case 0x02:
		{
			GJDisconnectAccountRecv((SDHP_DISCONNECT_ACCOUNT_RECV*)lpMsg, index);

			break;
		}

		case 0x03:
		{
			GJAccountLevelRecv((SDHP_ACCOUNT_LEVEL_RECV*)lpMsg, index);

			break;
		}

		case 0x04:
		{
			GJAccountLevelSaveRecv((SDHP_ACCOUNT_LEVEL_SAVE_RECV*)lpMsg, index);

			break;
		}

		case 0x06:
		{
			GJServerUserInfoRecv((SDHP_SERVER_USER_INFO_RECV*)lpMsg, index);

			break;
		}

		case 0x07:
		{
			GJAccountCountRecv((SDHP_COUNT_ONLINE_USER_RECV*)lpMsg, index);

			break;
		}
	}
}

void JoinServerLiveProc()
{
	SDHP_JOIN_SERVER_LIVE_SEND pMsg;

	pMsg.header.set(0x02, sizeof(pMsg));

	pMsg.QueueSize = gSocketManager.GetQueueSize();

	gSocketManagerUdp.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void GJServerInfoRecv(SDHP_SERVER_INFO_RECV* lpMsg, int index)
{
	gServerManager[index].SetServerInfo(lpMsg->ServerName, lpMsg->ServerPort, lpMsg->ServerCode);
}

void GJConnectAccountRecv(SDHP_CONNECT_ACCOUNT_RECV* lpMsg, int index)
{
	SDHP_CONNECT_ACCOUNT_SEND pMsg;

	pMsg.header.set(0x01, sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account, lpMsg->account, sizeof(pMsg.account));

	pMsg.result = 1;

	if (CheckTextSyntax(lpMsg->account, sizeof(lpMsg->account)) == false)
	{
		pMsg.result = 2;

		gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	ACCOUNT_INFO AccountInfo;

	if (gAccountManager.GetAccountInfo(&AccountInfo, lpMsg->account) != false)
	{
		pMsg.result = 3;

		gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);

		JGAccountAlreadyConnectedSend(AccountInfo.GameServerCode, AccountInfo.UserIndex, AccountInfo.Account);

		return;
	}

	if (gAccountManager.GetAccountCount() >= MAX_ACCOUNT)
	{
		pMsg.result = 4;

		gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (MD5Encryption == 0)
	{
		if (gQueryManager.ExecQuery("SELECT memb__pwd FROM MEMB_INFO WHERE memb___id='%s' COLLATE Latin1_General_BIN", lpMsg->account) == false || gQueryManager.Fetch() == SQL_NO_DATA)
		{
			gQueryManager.Close();

			pMsg.result = 2;

			gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);

			return;
		}

		char password[11] = { 0 };

		gQueryManager.GetAsString("memb__pwd", password, sizeof(password));

		if (strcmp(lpMsg->password, password) != 0 && strcmp(lpMsg->password, GlobalPassword) != 0)
		{
			gQueryManager.Close();

			pMsg.result = 0;

			gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);

			return;
		}

		gQueryManager.Close();

		if (strcmp(lpMsg->password, GlobalPassword) == 0)
		{
			LogAdd(LOG_RED, "IP [%s] has logged in '%s' account with global password!", lpMsg->IpAddress, lpMsg->account);
		}
	}
	else
	{
		if (gQueryManager.ExecQuery("SELECT memb__pwd FROM MEMB_INFO WHERE memb___id='%s' COLLATE Latin1_General_BIN", lpMsg->account) == false || gQueryManager.Fetch() == SQL_NO_DATA)
		{
			gQueryManager.Close();

			pMsg.result = 2;

			gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);

			return;
		}

		BYTE password[16] = { 0 };

		gQueryManager.GetAsBinary("memb__pwd", password, sizeof(password));

		MD5 MD5Hash;

		if (MD5Hash.MD5_CheckValue(lpMsg->password, (char*)password, MakeAccountKey(lpMsg->account)) == false && strcmp(lpMsg->password, GlobalPassword) != 0)
		{
			gQueryManager.Close();

			pMsg.result = 0;

			gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);

			return;
		}

		gQueryManager.Close();

		if (strcmp(lpMsg->password, GlobalPassword) == 0)
		{
			LogAdd(LOG_RED, "IP [%s] has logged in '%s' account with global password!", lpMsg->IpAddress, lpMsg->account);
		}
	}

	if (gQueryManager.ExecQuery("EXEC WZ_DesblocAccount '%s'", lpMsg->account) == false || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		pMsg.result = 2;

		gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (gQueryManager.ExecQuery("SELECT sno__numb, bloc_code FROM MEMB_INFO WHERE memb___id='%s'", lpMsg->account) == false || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		pMsg.result = 2;

		gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	gQueryManager.GetAsString("sno__numb", pMsg.PersonalCode, sizeof(pMsg.PersonalCode));

	pMsg.BlockCode = (BYTE)gQueryManager.GetAsInteger("bloc_code");

	gQueryManager.Close();

	if (gQueryManager.ExecQuery("EXEC WZ_GetAccountLevel '%s'", lpMsg->account) == false || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		pMsg.result = 2;

		gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	pMsg.AccountLevel = gQueryManager.GetAsInteger("AccountLevel");

	gQueryManager.GetAsString("AccountExpireDate", pMsg.AccountExpireDate, sizeof(pMsg.AccountExpireDate));

	gQueryManager.Close();

	gQueryManager.ExecQuery("EXEC WZ_CONNECT_MEMB '%s','%s','%s'", lpMsg->account, gServerManager[index].m_ServerName, lpMsg->IpAddress);

	gQueryManager.Close();

	gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);

	strcpy_s(AccountInfo.Account, lpMsg->account);

	strcpy_s(AccountInfo.IpAddress, lpMsg->IpAddress);

	AccountInfo.UserIndex = lpMsg->index;

	AccountInfo.GameServerCode = gServerManager[index].m_ServerCode;

	gAccountManager.InsertAccountInfo(AccountInfo);

	gLog.Output(LOG_ACCOUNT, "[AccountInfo] Account connected (Account: %s, IpAddress: %s, GameServerCode: %d)", AccountInfo.Account, AccountInfo.IpAddress, AccountInfo.GameServerCode);
}

void GJDisconnectAccountRecv(SDHP_DISCONNECT_ACCOUNT_RECV* lpMsg, int index)
{
	SDHP_DISCONNECT_ACCOUNT_SEND pMsg;

	pMsg.header.set(0x02, sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account, lpMsg->account, sizeof(pMsg.account));

	pMsg.result = 1;

	ACCOUNT_INFO AccountInfo;

	if (gAccountManager.GetAccountInfo(&AccountInfo, lpMsg->account) == false)
	{
		pMsg.result = 0;

		gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (AccountInfo.UserIndex != lpMsg->index)
	{
		pMsg.result = 0;

		gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (AccountInfo.GameServerCode != gServerManager[index].m_ServerCode)
	{
		pMsg.result = 0;

		gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	gQueryManager.ExecQuery("EXEC WZ_DISCONNECT_MEMB '%s'", lpMsg->account);

	gQueryManager.Close();

	gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);

	gAccountManager.RemoveAccountInfo(AccountInfo);

	gLog.Output(LOG_ACCOUNT, "[AccountInfo] Account disconnected (Account: %s, IpAddress: %s, GameServerCode: %d)", AccountInfo.Account, AccountInfo.IpAddress, AccountInfo.GameServerCode);
}

void GJAccountLevelRecv(SDHP_ACCOUNT_LEVEL_RECV* lpMsg, int index)
{
	ACCOUNT_INFO AccountInfo;

	if (gAccountManager.GetAccountInfo(&AccountInfo, lpMsg->account) == false)
	{
		return;
	}

	SDHP_ACCOUNT_LEVEL_SEND pMsg;

	pMsg.header.set(0x03, sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account, lpMsg->account, sizeof(pMsg.account));

	if (gQueryManager.ExecQuery("EXEC WZ_GetAccountLevel '%s'", lpMsg->account) == false || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		pMsg.AccountLevel = 0;
	}
	else
	{
		pMsg.AccountLevel = gQueryManager.GetAsInteger("AccountLevel");

		gQueryManager.GetAsString("AccountExpireDate", pMsg.AccountExpireDate, sizeof(pMsg.AccountExpireDate));

		gQueryManager.Close();
	}

	gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);
}

void GJAccountLevelSaveRecv(SDHP_ACCOUNT_LEVEL_SAVE_RECV* lpMsg, int index)
{
	gQueryManager.ExecQuery("EXEC WZ_SetAccountLevel '%s','%d','%d'", lpMsg->account, lpMsg->AccountLevel, lpMsg->AccountExpireTime);

	gQueryManager.Fetch();

	gQueryManager.Close();
}

void GJServerUserInfoRecv(SDHP_SERVER_USER_INFO_RECV* lpMsg, int index)
{
	gServerManager[index].m_CurUserCount = lpMsg->CurUserCount;

	gServerManager[index].m_MaxUserCount = lpMsg->MaxUserCount;
}

void JGAccountAlreadyConnectedSend(int GameServerCode, int UserIndex, char* account)
{
	CServerManager* lpServerManager = FindServerByCode(GameServerCode);

	if (lpServerManager == 0)
	{
		return;
	}

	SDHP_ACCOUNT_ALREADY_CONNECTED_SEND pMsg;

	pMsg.header.set(0x05, sizeof(pMsg));

	pMsg.index = UserIndex;

	memcpy(pMsg.account, account, sizeof(pMsg.account));

	gSocketManager.DataSend(lpServerManager->m_index, (BYTE*)&pMsg, pMsg.header.size);
}

void GJAccountCountRecv(SDHP_COUNT_ONLINE_USER_RECV* lpMsg, int index)
{
	SDHP_COUNT_ONLINE_USER_SEND pMsg;

	pMsg.header.set(0x07, sizeof(pMsg));

	pMsg.index = lpMsg->index;

	pMsg.count = (int)gAccountManager.GetAccountCount();

	gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);
}