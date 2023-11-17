#include "stdafx.h"
#include "JSProtocol.h"
#include "CommandManager.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "Log.h"
#include "Message.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "ServerInfo.h"
#include "Util.h"

void JoinServerProtocolCore(BYTE head, BYTE* lpMsg, int size)
{
	switch (head)
	{
		case 0x01:
		{
			JGConnectAccountRecv((SDHP_CONNECT_ACCOUNT_RECV*)lpMsg);

			break;
		}

		case 0x02:
		{
			JGDisconnectAccountRecv((SDHP_DISCONNECT_ACCOUNT_RECV*)lpMsg);

			break;
		}

		case 0x03:
		{
			JGAccountLevelRecv((SDHP_ACCOUNT_LEVEL_RECV*)lpMsg);

			break;
		}

		case 0x05:
		{
			JGAccountAlreadyConnectedRecv((SDHP_ACCOUNT_ALREADY_CONNECTED_RECV*)lpMsg);

			break;
		}

		case 0x07:
		{
			gCommandManager.JGCommandOnlinesRecv((SDHP_COUNT_ONLINE_USER_RECV*)lpMsg);

			break;
		}
	}
}

void GJServerInfoSend()
{
	SDHP_JOIN_SERVER_INFO_SEND pMsg;

	pMsg.header.set(0x00, sizeof(pMsg));

	pMsg.type = 1;

	pMsg.ServerPort = (WORD)gServerInfo.m_ServerPort;

	strcpy_s(pMsg.ServerName, gServerInfo.m_ServerName);

	pMsg.ServerCode = (WORD)gServerInfo.m_ServerCode;

	gJoinServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void GJConnectAccountSend(int aIndex, char* account, char* password, char* IpAddress)
{
	SDHP_CONNECT_ACCOUNT_SEND pMsg;

	pMsg.header.set(0x01, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, account, sizeof(pMsg.account));

	memcpy(pMsg.password, password, sizeof(pMsg.password));

	memcpy(pMsg.IpAddress, IpAddress, sizeof(pMsg.IpAddress));

	gJoinServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void JGConnectAccountRecv(SDHP_CONNECT_ACCOUNT_RECV* lpMsg)
{
	if (OBJECT_RANGE(lpMsg->index) == 0)
	{
		return;
	}

	if ((gObj[lpMsg->index].LoginMessageSend--) == 0)
	{
		CloseClient(lpMsg->index);

		return;
	}

	if (gObj[lpMsg->index].Connected != OBJECT_CONNECTED)
	{
		GJDisconnectAccountSend(lpMsg->index, lpMsg->account, gObj[lpMsg->index].IpAddr);

		return;
	}

	if (lpMsg->result != 1)
	{
		GCConnectAccountSend(lpMsg->index, ((gObj[lpMsg->index].LoginMessageCount >= 3) ? 8 : lpMsg->result));

		return;
	}

	if (lpMsg->BlockCode != 0)
	{
		GJDisconnectAccountSend(lpMsg->index, lpMsg->account, gObj[lpMsg->index].IpAddr);

		GCConnectAccountSend(lpMsg->index, 5);

		return;
	}

	if (gServerInfo.m_ServerLock > lpMsg->AccountLevel)
	{
		GJDisconnectAccountSend(lpMsg->index, lpMsg->account, gObj[lpMsg->index].IpAddr);

		GCConnectAccountSend(lpMsg->index, 11);

		return;
	}

	gObj[lpMsg->index].Connected = OBJECT_LOGGED;

	memcpy(gObj[lpMsg->index].Account, lpMsg->account, sizeof(gObj[lpMsg->index].Account));

	memcpy(gObj[lpMsg->index].PersonalCode, lpMsg->PersonalCode, sizeof(gObj[lpMsg->index].PersonalCode));

	gObj[lpMsg->index].AccountLevel = lpMsg->AccountLevel;

	memcpy(gObj[lpMsg->index].AccountExpireDate, lpMsg->AccountExpireDate, sizeof(gObj[lpMsg->index].AccountExpireDate));

	GCConnectAccountSend(lpMsg->index, 1);

	//LogAdd(LOG_USER, "[ObjectManager][%d] AddAccountInfo (%s)(%s)", lpMsg->index, gObj[lpMsg->index].Account, gObj[lpMsg->index].HardwareID);

	gLog.Output(LOG_CONNECT, "[ObjectManager][%d] AddAccountInfo (%s)(%s)", lpMsg->index, gObj[lpMsg->index].Account, gObj[lpMsg->index].HardwareID);
}

void GJDisconnectAccountSend(int aIndex, char* account, char* IpAddress)
{
	SDHP_DISCONNECT_ACCOUNT_SEND pMsg;

	pMsg.header.set(0x02, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, account, sizeof(pMsg.account));

	memcpy(pMsg.IpAddress, IpAddress, sizeof(pMsg.IpAddress));

	gJoinServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void JGDisconnectAccountRecv(SDHP_DISCONNECT_ACCOUNT_RECV* lpMsg)
{
	if (gObjIsAccountValid(lpMsg->index, lpMsg->account) != 0)
	{
		CloseClient(lpMsg->index);
	}
}

void GJAccountLevelSend(int aIndex)
{
	SDHP_ACCOUNT_LEVEL_SEND pMsg;

	pMsg.header.set(0x03, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, gObj[aIndex].Account, sizeof(pMsg.account));

	gJoinServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void JGAccountLevelRecv(SDHP_ACCOUNT_LEVEL_RECV* lpMsg)
{
	if (gObjIsAccountValid(lpMsg->index, lpMsg->account) == 0)
	{
		LogAdd(LOG_RED, "[JGAccountLevelRecv] Invalid Account [%d](%s)", lpMsg->index, lpMsg->account);

		CloseClient(lpMsg->index);

		return;
	}

	if (gServerInfo.m_ServerLock > lpMsg->AccountLevel)
	{
		gObjUserKill(lpMsg->index);

		return;
	}

	if (gObj[lpMsg->index].AccountLevel == lpMsg->AccountLevel && strcmp(gObj[lpMsg->index].AccountExpireDate, lpMsg->AccountExpireDate) == 0)
	{
		gObj[lpMsg->index].AccountLevel = lpMsg->AccountLevel;

		memcpy(gObj[lpMsg->index].AccountExpireDate, lpMsg->AccountExpireDate, sizeof(gObj[lpMsg->index].AccountExpireDate));
	}
	else
	{
		gObj[lpMsg->index].AccountLevel = lpMsg->AccountLevel;

		memcpy(gObj[lpMsg->index].AccountExpireDate, lpMsg->AccountExpireDate, sizeof(gObj[lpMsg->index].AccountExpireDate));

		gNotice.GCNoticeSend(lpMsg->index, 1, gMessage.GetTextMessage((36 + gObj[lpMsg->index].AccountLevel), gObj[lpMsg->index].Lang), gObj[lpMsg->index].AccountExpireDate);
	}
}

void GJAccountLevelSaveSend(int aIndex, int AccountLevel, int AccountExpireTime)
{
	SDHP_ACCOUNT_LEVEL_SAVE_SEND pMsg;

	pMsg.header.set(0x04, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, gObj[aIndex].Account, sizeof(pMsg.account));

	pMsg.AccountLevel = AccountLevel;

	pMsg.AccountExpireTime = AccountExpireTime;

	gJoinServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void JGAccountAlreadyConnectedRecv(SDHP_ACCOUNT_ALREADY_CONNECTED_RECV* lpMsg)
{
	if (gObjIsAccountValid(lpMsg->index, lpMsg->account) == 0)
	{
		LogAdd(LOG_RED, "[JGAccountAlreadyConnectedRecv] Invalid Account [%d](%s)", lpMsg->index, lpMsg->account);

		CloseClient(lpMsg->index);

		return;
	}

	if (gServerInfo.m_DisconnectOnlineAccount != 0)
	{
		GCCloseClientSend(lpMsg->index, 0);
	}
	else
	{
		gNotice.GCNoticeSend(lpMsg->index, 1, gMessage.GetTextMessage((42), gObj[lpMsg->index].Lang));
	}
}

void GJServerUserInfoSend()
{
	SDHP_SERVER_USER_INFO_SEND pMsg;

	pMsg.header.set(0x06, sizeof(pMsg));

	pMsg.CurUserCount = (WORD)gObjTotalUser;

	pMsg.MaxUserCount = (WORD)gServerInfo.m_ServerMaxUserNumber;

	gJoinServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}