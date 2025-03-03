#include "stdafx.h"
#include "GoldenArcher.h"
#include "GoldenArcherBingo.h"
#include "GameMain.h"
#include "ItemBagManager.h"
#include "JSProtocol.h"
#include "Map.h"
#include "Message.h"
#include "Notice.h"
#include "NpcTalk.h"
#include "Protocol.h"
#include "Util.h"

CGoldenArcher gGoldenArcher;

CGoldenArcher::CGoldenArcher()
{
	this->m_RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA1] = 10;

	this->m_RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA2] = 100;

	this->m_RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA3] = 500;

	this->m_RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA4] = 1000;

	this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE1] = 50;
	this->m_StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE1] = 24;

	this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE2] = 100;
	this->m_StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE2] = 24;

	this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE3] = 150;
	this->m_StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE3] = 24;

	this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE4] = 30;
}

CGoldenArcher::~CGoldenArcher()
{

}

void CGoldenArcher::ReadArcherInfo(char* section, char* path)
{
	// Rena exchager
	this->m_RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA1] = GetPrivateProfileInt(section, "RenaExchangeRequired1", 10, path);

	this->m_RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA2] = GetPrivateProfileInt(section, "RenaExchangeRequired2", 100, path);

	this->m_RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA3] = GetPrivateProfileInt(section, "RenaExchangeRequired3", 500, path);

	this->m_RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA4] = GetPrivateProfileInt(section, "RenaExchangeRequired4", 1000, path);

	// Stone exchanger VIP
	this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE1] = GetPrivateProfileInt(section, "StoneExchangeRequiredVIP1", 50, path);
	this->m_StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE1] = GetPrivateProfileInt(section, "StoneExchangeVIPTime1", 24, path);

	this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE2] = GetPrivateProfileInt(section, "StoneExchangeRequiredVIP2", 100, path);
	this->m_StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE2] = GetPrivateProfileInt(section, "StoneExchangeVIPTime2", 24, path);

	this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE3] = GetPrivateProfileInt(section, "StoneExchangeRequiredVIP3", 150, path);
	this->m_StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE3] = GetPrivateProfileInt(section, "StoneExchangeVIPTime3", 24, path);

	// Stone exchanger Lucky Number
	this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE4] = GetPrivateProfileInt(section, "StoneExchangeRequiredLuckyNumber", 30, path);
}

void CGoldenArcher::GDGoldenArcherGetValuesSend(int aIndex)
{
	SDHP_GOLDEN_ARCHER_GET_VALUES_SEND pMsg;

	pMsg.header.set(0x04, 0x00, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, gObj[aIndex].Account, sizeof(pMsg.account));

	gDataServerConnection.DataSend((BYTE*)&pMsg, sizeof(pMsg));
}

void CGoldenArcher::DGGoldenArcherGetValuesRecv(SDHP_GOLDEN_ARCHER_GET_VALUES_RECV* lpMsg)
{
	if (gObjIsAccountValid(lpMsg->index, lpMsg->account) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	lpObj->ArcherCoinCount[GOLDEN_ARCHER_COIN_RENA] = lpMsg->count[GOLDEN_ARCHER_COIN_RENA];

	lpObj->ArcherCoinCount[GOLDEN_ARCHER_COIN_STONE] = lpMsg->count[GOLDEN_ARCHER_COIN_STONE];

	memcpy(lpObj->ArcherLuckyNumber, lpMsg->LuckyNumber, sizeof(lpObj->ArcherLuckyNumber));

	lpObj->RegisteredLuckyNumbers.clear();

	for (int i = 0; i < lpMsg->registered; i++)
	{
		LUCKY_NUMBERS_REGISTERED* lpInfo = (LUCKY_NUMBERS_REGISTERED*)(((BYTE*)lpMsg) + sizeof(SDHP_GOLDEN_ARCHER_GET_VALUES_RECV) + (sizeof(LUCKY_NUMBERS_REGISTERED) * i));

		lpObj->RegisteredLuckyNumbers.push_back(lpInfo->LuckyNumber);
	}

	this->GCGoldenArcherInfoSend(lpMsg->index);
}

void CGoldenArcher::GCGoldenArcherInfoSend(int aIndex)
{
	PMSG_GOLDEN_ARCHER_INFO_SEND pMsg;

	pMsg.header.set(0x97, 0x00, sizeof(pMsg));

	pMsg.RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA1] = this->m_RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA1];

	pMsg.RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA2] = this->m_RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA2];

	pMsg.RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA3] = this->m_RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA3];

	pMsg.RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA4] = this->m_RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA4];

	pMsg.StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE1] = this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE1];
	pMsg.StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE1] = this->m_StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE1];

	pMsg.StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE2] = this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE2];
	pMsg.StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE2] = this->m_StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE2];

	pMsg.StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE3] = this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE3];
	pMsg.StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE3] = this->m_StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE3];

	pMsg.StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE4] = this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE4];

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CGoldenArcher::NpcGoldenArcher(int iNpcIndex, int iObjIndex)
{
	LPOBJ lpNpc = &gObj[iNpcIndex];

	LPOBJ lpObj = &gObj[iObjIndex];

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_GOLDEN_ARCHER;
	lpObj->Interface.state = 0;

	if (gGoldenArcherBingo.GetState() > BINGO_STATE_EMPTY)
	{
		if (iNpcIndex == gGoldenArcherBingo.GetGoldenArcherIndex())
		{
			this->NpcGoldenArcherOpenSend(iObjIndex, GOLDEN_ARCHER_LUCKY_NUMBER_EXCHANGE);
		}
		else
		{
			this->NpcGoldenArcherListSend(iObjIndex);
		}
	}
	else if (lpObj->Map == MAP_LORENCIA)
	{
		this->NpcGoldenArcherOpenSend(iObjIndex, GOLDEN_ARCHER_RENA_COLLECTOR);
	}
	else if (lpObj->Map == MAP_NORIA)
	{
		this->NpcGoldenArcherOpenSend(iObjIndex, GOLDEN_ARCHER_STONE_COLLECTOR);
	}
	else
	{
		this->NpcGoldenArcherListSend(iObjIndex);
	}
}

void CGoldenArcher::NpcGoldenArcherOpenSend(int aIndex, int Type)
{
	LPOBJ lpObj = &gObj[aIndex];

	PMSG_NPC_GOLDEN_ARCHER_SEND pMsg;

	pMsg.header.set(0x94, sizeof(pMsg));

	pMsg.Type = Type;

	if (Type == GOLDEN_ARCHER_RENA_COLLECTOR)
	{
		pMsg.Count = lpObj->ArcherCoinCount[GOLDEN_ARCHER_COIN_RENA];
	}
	else if (Type == GOLDEN_ARCHER_STONE_COLLECTOR)
	{
		pMsg.Count = lpObj->ArcherCoinCount[GOLDEN_ARCHER_COIN_STONE];
	}
	else
	{
		pMsg.Count = 0;
	}

	memcpy(pMsg.LuckyNumber, lpObj->ArcherLuckyNumber, sizeof(pMsg.LuckyNumber));

	DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
}

void CGoldenArcher::NpcGoldenArcherListSend(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	BYTE send[4096];

	PMSG_GOLDEN_ARCHER_LIST_SEND pMsg;

	pMsg.header.set(0x97, 0x01, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	LUCKY_NUMBER_INFO info;

	for (std::string Number : lpObj->RegisteredLuckyNumbers)
	{
		memcpy(info.LuckyNumber, Number.c_str(), sizeof(info.LuckyNumber));

		memcpy(&send[size], &info, sizeof(info));

		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(aIndex, send, size);
}

void CGoldenArcher::CGGoldenArcherRegisterRecv(PMSG_GOLDEN_ARCHER_REGISTER_RECV* lpMsg, int aIndex)
{
	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	// Prevent npc working during event
	if (gGoldenArcherBingo.GetState() > BINGO_STATE_EMPTY)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Interface.use != 1 || lpObj->Interface.type != INTERFACE_GOLDEN_ARCHER)
	{
		return;
	}

	if (lpMsg->Type != GOLDEN_ARCHER_RENA_COLLECTOR && lpMsg->Type != GOLDEN_ARCHER_STONE_COLLECTOR)
	{
		return;
	}

	int Slot = gItemManager.GetInventoryItemSlot(lpObj, GET_ITEM(14, 21), lpMsg->Type);

	if (Slot == -1)
	{
		this->GCGoldenArcherRegisterSend(lpObj->Index, -1);

		return;
	}

	gItemManager.DecreaseItemDur(lpObj, Slot, 1);

	lpObj->ArcherCoinCount[lpMsg->Type]++;

	this->GCGoldenArcherRegisterSend(lpObj->Index, lpObj->ArcherCoinCount[lpMsg->Type]);

	this->GDGoldenArcherSaveCoinSend(lpObj->Index);
}

void CGoldenArcher::GCGoldenArcherRegisterSend(int aIndex, int Count)
{
	PMSG_GOLDEN_ARCHER_REGISTER_SEND pMsg;

	pMsg.header.set(0x95, sizeof(pMsg));

	pMsg.Count = Count;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CGoldenArcher::GDGoldenArcherSaveCoinSend(int aIndex)
{
	SDHP_GOLDEN_ARCHER_SAVE_COIN_SEND pMsg;

	pMsg.header.set(0x04, 0x01, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, gObj[aIndex].Account, sizeof(pMsg.account));

	pMsg.count[GOLDEN_ARCHER_COIN_RENA] = gObj[aIndex].ArcherCoinCount[GOLDEN_ARCHER_COIN_RENA];

	pMsg.count[GOLDEN_ARCHER_COIN_STONE] = gObj[aIndex].ArcherCoinCount[GOLDEN_ARCHER_COIN_STONE];

	gDataServerConnection.DataSend((BYTE*)&pMsg, sizeof(pMsg));
}

void CGoldenArcher::CGGoldenArcherExchangeRecv(PMSG_GOLDEN_ARCHER_EXCHANGE_RECV* lpMsg, int aIndex)
{
	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	// Prevent npc working during event
	if (gGoldenArcherBingo.GetState() > BINGO_STATE_EMPTY)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Interface.use != 1 || lpObj->Interface.type != INTERFACE_GOLDEN_ARCHER)
	{
		return;
	}

	if (lpMsg->Type == GOLDEN_ARCHER_RENA_COLLECTOR)
	{
		this->GoldenArcherExchangeRena(aIndex, lpMsg->Type, lpMsg->Count);
	}
	else if (lpMsg->Type == GOLDEN_ARCHER_STONE_COLLECTOR)
	{
		this->GoldenArcherExchangeStone(aIndex, lpMsg->Type, lpMsg->Count);
	}
}

void CGoldenArcher::GoldenArcherExchangeRena(int aIndex, int Type, int Count)
{
	if (Count < GOLDEN_ARCHER_EXCHANGE_RENA1 && Count >= GOLDEN_ARCHER_MAX_EXCHANGE_RENAS)
	{
		return;
	}

	if (this->m_RenaExchangeRequiredCount[Count] == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->ArcherCoinCount[Type] < this->m_RenaExchangeRequiredCount[Count])
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(28, lpObj->Lang), this->m_StoneExchangeRequiredCount[Count]);

		return;
	}

	lpObj->ArcherCoinCount[Type] -= this->m_RenaExchangeRequiredCount[Count];

	this->GCGoldenArcherRegisterSend(lpObj->Index, lpObj->ArcherCoinCount[Type]);

	this->GCGoldenArcherCloseSend(lpObj->Index);

	gNpcTalk.CGNpcTalkCloseRecv(lpObj->Index);

	gItemBagManager.DropItemBySpecialValue(ITEM_BAG_GOLDEN_ARCHER1 + Count, lpObj, lpObj->Map, lpObj->X, lpObj->Y);

	this->GDGoldenArcherSaveCoinSend(lpObj->Index);
}

void CGoldenArcher::GoldenArcherExchangeStone(int aIndex, int Type, int Count)
{
	if (Count < GOLDEN_ARCHER_EXCHANGE_STONE1 && Count >= GOLDEN_ARCHER_MAX_EXCHANGE_STONES)
	{
		return;
	}

	if (this->m_StoneExchangeRequiredCount[Count] == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->ArcherCoinCount[Type] < this->m_StoneExchangeRequiredCount[Count])
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(28, lpObj->Lang), this->m_StoneExchangeRequiredCount[Count]);

		return;
	}

	if (Count < GOLDEN_ARCHER_EXCHANGE_STONE4)
	{
		this->GoldenArcherExchangeStoneVIP(aIndex, Type, Count);
	}
	else if (Count == GOLDEN_ARCHER_EXCHANGE_STONE4)
	{
		this->GoldenArcherExchangeStoneLuckyNumber(aIndex, Type, Count);
	}
}

void CGoldenArcher::GoldenArcherExchangeStoneVIP(int aIndex, int Type, int Count)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->AccountLevel > 0 && lpObj->AccountLevel != (Count + 1))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(29, lpObj->Lang));

		return;
	}

	lpObj->ArcherCoinCount[Type] -= this->m_StoneExchangeRequiredCount[Count];

	this->GCGoldenArcherRegisterSend(lpObj->Index, lpObj->ArcherCoinCount[Type]);

	GJAccountLevelSaveSend(lpObj->Index, (Count + 1), this->m_StoneExchangeVipDuration[Count] * 3600);

	GJAccountLevelSend(lpObj->Index);

	GCFireworksSend(lpObj, lpObj->X, lpObj->Y);

	this->GDGoldenArcherSaveCoinSend(lpObj->Index);
}

void CGoldenArcher::GoldenArcherExchangeStoneLuckyNumber(int aIndex, int Type, int Count)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->ArcherLuckyNumber[0] != '\0')
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(30, lpObj->Lang));

		return;
	}

	char Randomize[37] = { "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" };

	for (int i = 0; i < 12; ++i)
	{
		lpObj->ArcherLuckyNumber[i] = Randomize[GetLargeRand() % (sizeof(Randomize) - 1)];
	}

	lpObj->ArcherLuckyNumber[12] = '\0';

	lpObj->ArcherCoinCount[Type] -= this->m_StoneExchangeRequiredCount[Count];

	this->GCGoldenArcherRegisterSend(lpObj->Index, lpObj->ArcherCoinCount[Type]);

	this->GDGoldenArcherSaveCoinSend(lpObj->Index);

	this->GCGoldenArcherLuckyNumberSend(lpObj->Index);

	GCFireworksSend(lpObj, lpObj->X, lpObj->Y);

	this->GDGoldenArcherSaveLuckyNumberSend(lpObj->Index);
}

void CGoldenArcher::GCGoldenArcherLuckyNumberSend(int aIndex)
{
	PMSG_GOLDEN_ARCHER_LUCKY_NUMBER_SEND pMsg;

	pMsg.header.set(0x96, sizeof(pMsg));

	memcpy(pMsg.LuckyNumber, gObj[aIndex].ArcherLuckyNumber, sizeof(pMsg.LuckyNumber));

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CGoldenArcher::GDGoldenArcherSaveLuckyNumberSend(int aIndex)
{
	SDHP_GOLDEN_ARCHER_SAVE_LUCKY_NUMBER_SEND pMsg;

	pMsg.header.set(0x04, 0x02, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, gObj[aIndex].Account, sizeof(pMsg.account));

	memcpy(pMsg.LuckyNumber, gObj[aIndex].ArcherLuckyNumber, sizeof(pMsg.LuckyNumber));

	gDataServerConnection.DataSend((BYTE*)&pMsg, sizeof(pMsg));
}

void CGoldenArcher::CGGoldenArcherRegisterLuckyRecv(PMSG_GOLDEN_ARCHER_REGISTER_LUCKY_RECV* lpMsg, int aIndex)
{
	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	// Prevent npc working during event
	if (gGoldenArcherBingo.GetState() > BINGO_STATE_EMPTY)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Interface.use != 1 || lpObj->Interface.type != INTERFACE_GOLDEN_ARCHER)
	{
		return;
	}

	if (lpObj->ArcherLuckyNumber[0] == '\0')
	{
		this->GCGoldenArcherRegisterLuckySend(aIndex, 2);

		return;
	}

	if (strcmp(lpMsg->LuckyNumber, lpObj->ArcherLuckyNumber) != 0)
	{
		this->GCGoldenArcherRegisterLuckySend(aIndex, 3);

		return;
	}

	this->GDGoldenArcherRegisterLuckyNumberSend(aIndex);
}

void CGoldenArcher::GDGoldenArcherRegisterLuckyNumberSend(int aIndex)
{
	SDHP_GOLDEN_ARCHER_SAVE_LUCKY_NUMBER_SEND pMsg;

	pMsg.header.set(0x04, 0x03, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, gObj[aIndex].Account, sizeof(pMsg.account));

	memcpy(pMsg.LuckyNumber, gObj[aIndex].ArcherLuckyNumber, sizeof(pMsg.LuckyNumber));

	gDataServerConnection.DataSend((BYTE*)&pMsg, sizeof(pMsg));
}

void CGoldenArcher::DGGoldenArcherRegisterLuckyRecv(SDHP_GOLDEN_ARCHER_SAVE_LUCKY_NUMBER_RECV* lpMsg)
{
	if (gObjIsAccountValid(lpMsg->index, lpMsg->account) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	if (lpObj->Interface.use != 1 || lpObj->Interface.type != INTERFACE_GOLDEN_ARCHER)
	{
		return;
	}

	if (lpMsg->Result == 0)
	{
		lpObj->RegisteredLuckyNumbers.push_back(lpObj->ArcherLuckyNumber);

		memset(lpObj->ArcherLuckyNumber, 0, sizeof(lpObj->ArcherLuckyNumber));

		this->GCGoldenArcherLuckyNumberSend(lpObj->Index);

		this->GDGoldenArcherSaveLuckyNumberSend(lpObj->Index);
	}

	this->GCGoldenArcherRegisterLuckySend(lpObj->Index, lpMsg->Result);
}

void CGoldenArcher::GCGoldenArcherRegisterLuckySend(int aIndex, int Result)
{
	/* Results:
	* 0 = Lucky Number registered successfully
	* 1 = There was an error registering the Lucky Number
	* 2 = You have to have a Lucky Number first
	* 3 = Wrong Lucky Number
	*/
	PMSG_GOLDEN_ARCHER_REGISTER_LUCKY_SEND pMsg;

	pMsg.header.set(0x97, 0x03, sizeof(pMsg));

	pMsg.Result = Result;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CGoldenArcher::CGGoldenArcherExchangeLuckyRecv(PMSG_GOLDEN_ARCHER_EXCHANGE_LUCKY_RECV* lpMsg, int aIndex)
{
	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	// Prevent npc working after event finished
	if (gGoldenArcherBingo.GetState() != BINGO_STATE_START)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Interface.use != 1 || lpObj->Interface.type != INTERFACE_GOLDEN_ARCHER)
	{
		return;
	}

	if (lpObj->RegisteredLuckyNumbers.size() == 0)
	{
		this->GCGoldenArcherExchangeLuckySend(aIndex, 2);

		return;
	}

	if (std::find(lpObj->RegisteredLuckyNumbers.begin(), lpObj->RegisteredLuckyNumbers.end(), lpMsg->LuckyNumber) == lpObj->RegisteredLuckyNumbers.end())
	{
		this->GCGoldenArcherExchangeLuckySend(aIndex, 3);

		return;
	}

	if (!gGoldenArcherBingo.CheckBingoWinner(aIndex, lpMsg->LuckyNumber))
	{
		this->GCGoldenArcherExchangeLuckySend(aIndex, 1);

		return;
	}

	gItemBagManager.DropItemBySpecialValue(ITEM_BAG_GOLDEN_ARCHER_BINGO, lpObj, lpObj->Map, lpObj->X, lpObj->Y);

	this->GCGoldenArcherExchangeLuckySend(aIndex, 0);
}

void CGoldenArcher::GCGoldenArcherExchangeLuckySend(int aIndex, int Result)
{
	/* Results:
	* 0 = This Lucky Number contains a reward!
	* 1 = Better luck next time!
	* 2 = You are not participanting in this event!
	* 3 = Use your own Lucky Numbers!
	*/

	PMSG_GOLDEN_ARCHER_EXCHANGE_LUCKY_SEND pMsg;

	pMsg.header.set(0x9D, sizeof(pMsg));

	pMsg.Result = Result;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CGoldenArcher::GCGoldenArcherCloseSend(int aIndex)
{
	PMSG_GOLDEN_ARCHER_CLOSE_SEND pMsg;

	pMsg.header.set(0x97, 0x04, sizeof(pMsg));

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CGoldenArcher::ClearRegisteredLuckyNumbers()
{
	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnectedGP(n) == false)
		{
			continue;
		}

		gObj[n].RegisteredLuckyNumbers.clear();
	}
}