#include "stdafx.h"
#include "ConnectServerProtocol.h"
#include "ClientManager.h"
#include "ServerList.h"
#include "SocketManager.h"
#include "Util.h"

void ConnectServerProtocolCore(int index, BYTE head, BYTE* lpMsg, int size)
{
	gConsole.Output(CON_PROTO_TCP_RECV, "RECV 0: %02X, 1: %02X, 2: %02X, 3: %02X, 4: %02X, 5: %02X", (size > 0) ? lpMsg[0] : 0, (size > 1) ? lpMsg[1] : 0, (size > 2) ? lpMsg[2] : 0, (size > 3) ? lpMsg[3] : 0, (size > 4) ? lpMsg[4] : 0, (size > 5) ? lpMsg[5] : 0);

	gClientManager[index].m_PacketTime = GetTickCount();

	switch (head)
	{
		case 0xF4:
		{
			switch (lpMsg[3])
			{
				case 0x02:
				{
					CCCustomServerListSend(index);

					CCServerListRecv((PMSG_SERVER_LIST_RECV*)lpMsg, index);

					break;
				}

				case 0x03:
				{
					CCServerInfoRecv((PMSG_SERVER_INFO_RECV*)lpMsg, index);

					break;
				}
			}

			break;
		}
	}
}

void CCServerInitSend(int index, int result)
{
	PMSG_SERVER_INIT_SEND pMsg;

	pMsg.header.set(0x00, sizeof(pMsg));

	pMsg.result = result;

	gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);
}

void CCCustomServerListSend(int index)
{
	BYTE send[2048];

	PMSG_CUSTOM_SERVER_LIST_SEND pMsg;

	pMsg.header.set(0xF4, 0x04, 0);

	int size = sizeof(pMsg);

	int count = gServerList.GenerateCustomServerList(send, &size);

	pMsg.count[0] = SET_NUMBERHB(count);

	pMsg.count[1] = SET_NUMBERLB(count);

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	gSocketManager.DataSend(index, send, size);
}

void CCServerListRecv(PMSG_SERVER_LIST_RECV* lpMsg, int index)
{
	BYTE send[2048];

	PMSG_SERVER_LIST_SEND pMsg;

	pMsg.header.set(0xF4, 0x02, 0);

	int size = sizeof(pMsg);

	int count = gServerList.GenerateServerList(send, &size);

	pMsg.count = count;

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	gSocketManager.DataSend(index, send, size);
}

void CCServerInfoRecv(PMSG_SERVER_INFO_RECV* lpMsg, int index)
{
	if (gServerList.CheckJoinServerState() == 0)
	{
		return;
	}

	SERVER_LIST_INFO* lpServerListInfo = gServerList.GetServerListInfo(lpMsg->ServerCode);

	if (lpServerListInfo == 0)
	{
		return;
	}

	if (lpServerListInfo->ServerShow == 0 || lpServerListInfo->ServerState == 0)
	{
		return;
	}

	PMSG_SERVER_INFO_SEND pMsg;

	pMsg.header.set(0xF4, 0x03, sizeof(pMsg));

	memcpy(pMsg.ServerAddress, lpServerListInfo->ServerAddress, sizeof(pMsg.ServerAddress));

	pMsg.ServerPort = lpServerListInfo->ServerPort;

	gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);
}