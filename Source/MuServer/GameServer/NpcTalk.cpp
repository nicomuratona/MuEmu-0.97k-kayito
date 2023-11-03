#include "stdafx.h"
#include "NpcTalk.h"
#include "BloodCastle.h"
#include "ChaosBox.h"
#include "CustomNpcMove.h"
#include "DevilSquare.h"
#include "DSProtocol.h"
#include "EffectManager.h"
#include "GameMain.h"
#include "ItemBagManager.h"
#include "Log.h"
#include "Map.h"
#include "Message.h"
#include "Notice.h"
#include "Quest.h"
#include "ServerInfo.h"
#include "ShopManager.h"
#include "Trade.h"
#include "Util.h"
#include "Warehouse.h"

CNpcTalk gNpcTalk;

CNpcTalk::CNpcTalk()
{

}

CNpcTalk::~CNpcTalk()
{

}

bool CNpcTalk::NpcTalk(LPOBJ lpNpc, LPOBJ lpObj)
{
	if (lpObj->Interface.use != 0)
	{
		return 1;
	}

	if (gQuest.NpcTalk(lpNpc, lpObj) != 0)
	{
		return 1;
	}

	if (gCustomNpcMove.GetNpcMove(lpObj, lpNpc->Class, lpNpc->Map, lpNpc->X, lpNpc->Y) != 0)
	{
		return 1;
	}

	switch (lpNpc->Class)
	{
		case 232:
		{
			this->NpcAngelKing(lpNpc, lpObj);

			break;
		}

		case 233:
		{
			this->NpcAngelMessenger(lpNpc, lpObj);

			break;
		}

		case 234:
		{
			this->NpcServerTransfer(lpNpc, lpObj);

			break;
		}

		case 236:
		{
			this->NpcGoldenArcher(lpNpc, lpObj);

			break;
		}

		case 237:
		{
			this->NpcCharon(lpNpc, lpObj);

			break;
		}

		case 238:
		{
			this->NpcChaosGoblin(lpNpc, lpObj);

			break;
		}

		case 240:
		{
			this->NpcWarehouse(lpNpc, lpObj);

			break;
		}

		case 241:
		{
			this->NpcGuildMaster(lpNpc, lpObj);

			break;
		}

		case 247:
		case 249:
		{
			this->NpcGuard(lpNpc, lpObj);

			break;
		}

		default:
		{
			return 0;
		}
	}

	return 1;
}

void CNpcTalk::NpcAngelKing(LPOBJ lpNpc, LPOBJ lpObj)
{
	gBloodCastle.NpcAngelKing(lpNpc, lpObj);
}

void CNpcTalk::NpcAngelMessenger(LPOBJ lpNpc, LPOBJ lpObj)
{
	gBloodCastle.NpcAngelMessenger(lpNpc, lpObj);
}

void CNpcTalk::NpcServerTransfer(LPOBJ lpNpc, LPOBJ lpObj)
{
	if (lpObj->Guild != 0)
	{
		GCServerCommandSend(lpObj->Index, 6, 0, 0);

		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_TRANSFER;
	lpObj->Interface.state = 0;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30, sizeof(pMsg));

	pMsg.result = 5;

	DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
}

void CNpcTalk::NpcGoldenArcher(LPOBJ lpNpc, LPOBJ lpObj)
{
	if (lpObj->Map != 0 && lpObj->Map != 2 && lpObj->Map != 3)
	{
		GCChatTargetSend(lpObj, lpNpc->Index, gMessage.GetTextMessage(27, lpObj->Lang));

		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_GOLDEN_ARCHER;
	lpObj->Interface.state = 0;

	if (lpObj->Map == 0 || lpObj->Map == 3)
	{
		SDHP_NPC_GOLDEN_ARCHER_OPEN_SEND pMsg;

		pMsg.header.set(0x04, 0x00, sizeof(pMsg));

		pMsg.index = lpObj->Index;

		pMsg.type = ((lpObj->Map == 0) ? 0 : 1);

		memcpy(pMsg.account, lpObj->Account, sizeof(pMsg.account));

		gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
	}
	else
	{
		PMSG_NPC_GOLDEN_ARCHER_SEND pMsg;

		pMsg.header.set(0x94, sizeof(pMsg));

		pMsg.type = 2;

		DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
	}
}

void CNpcTalk::DGNpcGoldenArcherOpenRecv(SDHP_NPC_GOLDEN_ARCHER_OPEN_RECV* lpMsg)
{
	PMSG_NPC_GOLDEN_ARCHER_SEND pMsg;

	pMsg.header.set(0x94, sizeof(pMsg));

	pMsg.type = lpMsg->type;

	pMsg.count = lpMsg->count;

	memset(pMsg.num, 0, sizeof(pMsg.num));

	DataSend(lpMsg->index, (BYTE*)&pMsg, pMsg.header.size);
}

void CNpcTalk::NpcGoldenArcherRegisterCount(PMSG_GOLDEN_ARCHER_REG_COUNT* lpMsg, int aIndex)
{
	if (lpMsg->type != 0 && lpMsg->type != 1)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Interface.type == INTERFACE_TRADE)
	{
		LogAdd(LOG_RED, "[GoldenArcher] [%s][%s] Attempted ItemCopy using Trade Window", lpObj->Account, lpObj->Name);

		return;
	}

	if (lpObj->Interface.type == INTERFACE_CHAOS_BOX)
	{
		LogAdd(LOG_RED, "[GoldenArcher] [%s][%s] Attempted ItemCopy using ChaosBox Window", lpObj->Account, lpObj->Name);

		return;
	}

	int Quantity = 0;

	Quantity = gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(14, 21), lpMsg->type);

	if (Quantity == 0)
	{
		PMSG_NPC_GOLDEN_ARCHER_REGISTER_SEND pMsg;

		pMsg.header.set(0x95, sizeof(pMsg));

		pMsg.type = lpMsg->type;

		pMsg.count = -1;

		DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
	}
	else
	{
		SDHP_NPC_GOLDEN_ARCHER_REG_COUNT_SEND pMsg;

		pMsg.header.set(0x04, 0x01, sizeof(pMsg));

		pMsg.type = lpMsg->type;

		pMsg.index = aIndex;

		pMsg.count = Quantity;

		memcpy(pMsg.account, lpObj->Account, sizeof(pMsg.account));

		gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
	}
}

void CNpcTalk::DGNpcGoldenArcherRegisterCountRecv(SDHP_NPC_GOLDEN_ARCHER_REG_COUNT_RECV* lpMsg)
{
	LPOBJ lpObj = &gObj[lpMsg->index];

	PMSG_NPC_GOLDEN_ARCHER_REGISTER_SEND pMsg;

	pMsg.header.set(0x95, sizeof(pMsg));

	pMsg.type = lpMsg->type;

	if (lpMsg->result == 0)
	{
		pMsg.count = -1;

		gLog.Output(LOG_CHAOS_MIX, "[GoldenArcher] [%s][%s] RegEventServer Fail (RegEventchip)", lpObj->Account, lpObj->Name);
	}
	else
	{
		pMsg.count = lpMsg->count + lpMsg->oldCount;

		gItemManager.DeleteInventoryItemCount(lpObj, GET_ITEM(14, 21), lpMsg->type, lpMsg->count);

		gNotice.GCNoticeSend(lpObj->Index, 0, gMessage.GetTextMessage(((lpMsg->type == 0) ? 28 : 29), lpObj->Lang), lpMsg->count);

		if (pMsg.count >= ((pMsg.type == 0) ? gServerInfo.m_GoldenArcherMinRenas : gServerInfo.m_GoldenArcherMinStones))
		{
			gNotice.GCNoticeSend(lpObj->Index, 0, gMessage.GetTextMessage(32, lpObj->Lang), lpMsg->luckyNum);
		}
	}

	DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
}

void CNpcTalk::NpcGoldenArcherRegisterLuckyNumber(PMSG_GOLDEN_ARCHER_REG_LUCKYNUM* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Interface.type == INTERFACE_TRADE)
	{
		LogAdd(LOG_RED, "[GoldenArcher] [%s][%s] Attempted ItemCopy using Trade Window", lpObj->Account, lpObj->Name);

		return;
	}

	if (lpObj->Interface.type == INTERFACE_CHAOS_BOX)
	{
		LogAdd(LOG_RED, "[GoldenArcher] [%s][%s] Attempted ItemCopy using ChaosBox Window", lpObj->Account, lpObj->Name);

		return;
	}

	SDHP_NPC_GOLDEN_ARCHER_REG_LUCKYNUM_SEND pMsg;

	pMsg.header.set(0x04, 0x02, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, lpObj->Account, sizeof(pMsg.account));

	memcpy(pMsg.serial1, lpMsg->serial1, sizeof(pMsg.serial1));

	memcpy(pMsg.serial2, lpMsg->serial2, sizeof(pMsg.serial2));

	memcpy(pMsg.serial3, lpMsg->serial3, sizeof(pMsg.serial3));

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void CNpcTalk::DGNpcGoldenArcherRegisterLuckyNumRecv(SDHP_NPC_GOLDEN_ARCHER_REG_LUCKYNUM_RECV* lpMsg)
{
	//Results
	//0 = success
	//1 = number taken
	//2 = max quantity reached
	//3 = wrong serial number
	//4 = unkown error

	LPOBJ lpObj = &gObj[lpMsg->index];

	PMSG_ANS_LUCKYNUMBER_SEND pMsg;

	pMsg.header.set(0x9D, sizeof(pMsg));

	pMsg.result = lpMsg->result;

	memset(pMsg.luckyName, 0, sizeof(pMsg.luckyName));

	if (lpMsg->result == 4)
	{
		gLog.Output(LOG_CHAOS_MIX, "[GoldenArcher] [%s][%s] LuckyNumber Fail (Database Empty)", lpObj->Account, lpObj->Name);
	}
	else if (lpMsg->result == 3)
	{
		memcpy(pMsg.luckyName, gMessage.GetTextMessage(30, lpObj->Lang), sizeof(pMsg.luckyName));

		gLog.Output(LOG_CHAOS_MIX, "[GoldenArcher] [%s][%s] LuckyNumber Fail (Wrong Number)", lpObj->Account, lpObj->Name);
	}
	else
	{
		if (gServerInfo.m_GoldenArcherRewardSwitch != 0)
		{
			GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, gServerInfo.m_GoldenArcherRewardItemIndex, 0, 0, 0, 0, 0, -1, 0);
		}
		else
		{
			memcpy(pMsg.luckyName, gMessage.GetTextMessage(31, lpObj->Lang), sizeof(pMsg.luckyName));
		}
	}

	DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
}

void CNpcTalk::NpcCharon(LPOBJ lpNpc, LPOBJ lpObj)
{
	gDevilSquare.NpcCharon(lpNpc, lpObj);
}

void CNpcTalk::NpcChaosGoblin(LPOBJ lpNpc, LPOBJ lpObj)
{
	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_CHAOS_BOX;
	lpObj->Interface.state = 0;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30, sizeof(pMsg));

	pMsg.result = 3;

	DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);

	lpObj->IsChaosMixCompleted = 0;

	gObjInventoryTransaction(lpObj->Index);
}

void CNpcTalk::NpcWarehouse(LPOBJ lpNpc, LPOBJ lpObj)
{
	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_WAREHOUSE;
	lpObj->Interface.state = 0;

	gWarehouse.GDWarehouseItemSend(lpObj->Index, lpObj->Account);
}

void CNpcTalk::NpcGuildMaster(LPOBJ lpNpc, LPOBJ lpObj)
{
	if (lpObj->GuildNumber > 0)
	{
		GCChatTargetSend(lpObj, lpNpc->Index, gMessage.GetTextMessage(33, lpObj->Lang));

		return;
	}

	if (lpObj->Level < gServerInfo.m_GuildCreateMinLevel)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(34, lpObj->Lang), gServerInfo.m_GuildCreateMinLevel);

		return;
	}

	if (lpObj->Reset < gServerInfo.m_GuildCreateMinReset)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(35, lpObj->Lang), gServerInfo.m_GuildCreateMinReset);

		return;
	}

	GCGuildMasterQuestionSend(lpObj->Index);
}

void CNpcTalk::NpcGuard(LPOBJ lpNpc, LPOBJ lpObj)
{
	GCChatTargetSend(lpObj, lpNpc->Index, gMessage.GetTextMessage(27, lpObj->Lang));
}

void CNpcTalk::CGNpcTalkRecv(PMSG_NPC_TALK_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if (lpObj->Interface.use != 0)
	{
		return;
	}

	if (lpObj->Live == 0)
	{
		return;
	}

	int bIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]);

	if (OBJECT_RANGE(bIndex) == 0)
	{
		return;
	}

	LPOBJ lpNpc = &gObj[bIndex];

	if (lpNpc->Type != OBJECT_NPC || lpObj->Map != lpNpc->Map)
	{
		return;
	}

	if (lpObj->X < (lpNpc->X - 5) || lpObj->X >(lpNpc->X + 5) || lpObj->Y < (lpNpc->Y - 5) || lpObj->Y >(lpNpc->Y + 5))
	{
		return;
	}

	if (this->NpcTalk(lpNpc, lpObj) != 0)
	{
		return;
	}

	if (gShopManager.GetShopNumber(lpNpc->Class) == -1)
	{
		return;
	}

	if (SHOP_RANGE(lpNpc->ShopNumber) != 0 && lpObj->PKLevel >= PKLVL_OUTLAW)
	{
		GCChatTargetSend(lpObj, lpNpc->Index, gMessage.GetTextMessage(25, lpObj->Lang));

		return;
	}

	if (gShopManager.GetItemCountByIndex(lpNpc->ShopNumber) == 0)
	{
		GCChatTargetSend(lpObj, lpNpc->Index, gMessage.GetTextMessage(26, lpObj->Lang));

		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_SHOP;
	lpObj->Interface.state = 0;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30, sizeof(pMsg));

	pMsg.result = 0;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

	lpObj->TargetShopNumber = lpNpc->ShopNumber;

	gShopManager.GCShopItemListSendByIndex(lpNpc->ShopNumber, aIndex);
}

void CNpcTalk::CGNpcTalkCloseRecv(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if (lpObj->Interface.use == 0 || lpObj->Interface.type == INTERFACE_NONE)
	{
		return;
	}

	switch (lpObj->Interface.type)
	{
		case INTERFACE_TRADE:
		{
			gTrade.CGTradeCancelButtonRecv(aIndex);

			break;
		}

		case INTERFACE_SHOP:
		{
			lpObj->TargetShopNumber = -1;

			break;
		}

		case INTERFACE_WAREHOUSE:
		{
			gWarehouse.CGWarehouseClose(aIndex);

			break;
		}
	}

	lpObj->Interface.use = 0;
	lpObj->Interface.type = INTERFACE_NONE;
	lpObj->Interface.state = 0;
}