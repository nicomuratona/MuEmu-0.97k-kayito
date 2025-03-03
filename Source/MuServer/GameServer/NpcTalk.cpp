#include "stdafx.h"
#include "NpcTalk.h"
#include "BloodCastle.h"
#include "ChaosBox.h"
#include "CustomNpcMove.h"
#include "DevilSquare.h"
#include "DSProtocol.h"
#include "EffectManager.h"
#include "GameMain.h"
#include "GoldenArcher.h"
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
	gGoldenArcher.NpcGoldenArcher(lpNpc->Index, lpObj->Index);
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

	if (SHOP_RANGE(lpNpc->ShopNumber) != 0 && (gServerInfo.m_PKLimitFree == 0 && lpObj->PKLevel >= PKLVL_OUTLAW))
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