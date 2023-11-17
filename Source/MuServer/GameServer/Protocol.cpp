#include "stdafx.h"
#include "Protocol.h"
#include "Attack.h"
#include "Blacklist.h"
#include "BloodCastle.h"
#include "ChaosBox.h"
#include "CommandManager.h"
#include "DefaultClassInfo.h"
#include "DevilSquare.h"
#include "DSProtocol.h"
#include "EffectManager.h"
#include "Filter.h"
#include "Fruit.h"
#include "Guild.h"
#include "GuildManager.h"
#include "HackPacketCheck.h"
#include "ItemManager.h"
#include "JSProtocol.h"
#include "Log.h"
#include "Map.h"
#include "Message.h"
#include "Move.h"
#include "Notice.h"
#include "NpcTalk.h"
#include "ObjectManager.h"
#include "Party.h"
#include "Quest.h"
#include "ServerInfo.h"
#include "SkillManager.h"
#include "Trade.h"
#include "Util.h"
#include "Viewport.h"
#include "Warehouse.h"

void ProtocolCore(BYTE head, BYTE* lpMsg, int size, int aIndex, int encrypt, int serial)
{
	gConsole.Output(CON_PROTO_TCP_RECV, "RECV 0: %02X, 1: %02X, 2: %02X, 3: %02X, 4: %02X, 5: %02X", (size > 0) ? lpMsg[0] : 0, (size > 1) ? lpMsg[1] : 0, (size > 2) ? lpMsg[2] : 0, (size > 3) ? lpMsg[3] : 0, (size > 4) ? lpMsg[4] : 0, (size > 5) ? lpMsg[5] : 0);

	if (gObj[aIndex].Type == OBJECT_USER && gHackPacketCheck.CheckPacketHack(aIndex, head, ((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]), encrypt, serial) == 0)
	{
		return;
	}

	switch (head)
	{
		case 0x00:
		{
			CGChatRecv((PMSG_CHAT_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x02:
		{
			CGChatWhisperRecv((PMSG_CHAT_WHISPER_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x03:
		{
			CGMainCheckRecv((PMSG_MAIN_CHECK_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x0E:
		{
			CGLiveClientRecv((PMSG_LIVE_CLIENT_RECV*)lpMsg, aIndex);

			break;
		}

		case PROTOCOL_CODE1:
		{
			CGMoveRecv((PMSG_MOVE_RECV*)lpMsg, aIndex);

			break;
		}

		case PROTOCOL_CODE3:
		{
			CGPositionRecv((PMSG_POSITION_RECV*)lpMsg, aIndex);

			break;
		}

		case PROTOCOL_CODE2:
		{
			gAttack.CGAttackRecv((PMSG_ATTACK_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x18:
		{
			CGActionRecv((PMSG_ACTION_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x19:
		{
			gSkillManager.CGSkillAttackRecv((PMSG_SKILL_ATTACK_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x1B:
		{
			gSkillManager.CGSkillCancelRecv((PMSG_SKILL_CANCEL_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x1C:
		{
			gMove.CGTeleportRecv((PMSG_TELEPORT_RECV*)lpMsg, aIndex);

			break;
		}

		case PROTOCOL_CODE4:
		{
			gSkillManager.CGMultiSkillAttackRecv((PMSG_MULTI_SKILL_ATTACK_RECV*)lpMsg, aIndex, 0);

			break;
		}

		case 0x1E:
		{
			gSkillManager.CGDurationSkillAttackRecv((PMSG_DURATION_SKILL_ATTACK_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x22:
		{
			gItemManager.CGItemGetRecv((PMSG_ITEM_GET_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x23:
		{
			gItemManager.CGItemDropRecv((PMSG_ITEM_DROP_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x24:
		{
			gItemManager.CGItemMoveRecv((PMSG_ITEM_MOVE_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x26:
		{
			gItemManager.CGItemUseRecv((PMSG_ITEM_USE_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x2B:
		{
			switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
			{
				case 0x00:
				{
					gItemManager.CGItemStackUnstackRecv((PMSG_ITEM_UNSTACK_RECV*)lpMsg, aIndex);

					break;
				}

				case 0x01:
				{
					gItemManager.CGItemStackStackRecv((PMSG_ITEM_UNSTACK_RECV*)lpMsg, aIndex);

					break;
				}
			}

			break;
		}

		case 0x30:
		{
			gNpcTalk.CGNpcTalkRecv((PMSG_NPC_TALK_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x31:
		{
			gNpcTalk.CGNpcTalkCloseRecv(aIndex);

			break;
		}

		case 0x32:
		{
			gItemManager.CGItemBuyRecv((PMSG_ITEM_BUY_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x33:
		{
			gItemManager.CGItemSellRecv((PMSG_ITEM_SELL_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x34:
		{
			gItemManager.CGItemRepairRecv((PMSG_ITEM_REPAIR_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x36:
		{
			gTrade.CGTradeRequestRecv((PMSG_TRADE_REQUEST_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x37:
		{
			gTrade.CGTradeResponseRecv((PMSG_TRADE_RESPONSE_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x3A:
		{
			gTrade.CGTradeMoneyRecv((PMSG_TRADE_MONEY_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x3C:
		{
			gTrade.CGTradeOkButtonRecv((PMSG_TRADE_OK_BUTTON_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x3D:
		{
			gTrade.CGTradeCancelButtonRecv(aIndex);

			break;
		}

		case 0x40:
		{
			gParty.CGPartyRequestRecv((PMSG_PARTY_REQUEST_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x41:
		{
			gParty.CGPartyRequestResultRecv((PMSG_PARTY_REQUEST_RESULT_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x42:
		{
			gParty.CGPartyListRecv(aIndex);

			break;
		}

		case 0x43:
		{
			gParty.CGPartyDelMemberRecv((PMSG_PARTY_DEL_MEMBER_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x50:
		{
			gGuild.CGGuildRequestRecv((PMSG_GUILD_REQUEST_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x51:
		{
			gGuild.CGGuildResultRecv((PMSG_GUILD_RESULT_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x52:
		{
			gGuild.CGGuildListRecv(aIndex);

			break;
		}

		case 0x53:
		{
			gGuild.CGGuildDeleteRecv((PMSG_GUILD_DELETE_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x54:
		{
			gGuild.CGGuildMasterOpenRecv((PMSG_GUILD_MASTER_OPEN_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x55:
		{
			gGuild.CGGuildCreateRecv((PMSG_GUILD_CREATE_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x57:
		{
			gGuild.CGGuildMasterCancelRecv(aIndex);

			break;
		}

		case 0x61:
		{
			gGuild.CGGuildWarRequestResultRecv((PMSG_GUILD_WAR_REQUEST_RESULT_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x81:
		{
			gWarehouse.CGWarehouseMoneyRecv((PMSG_WAREHOUSE_MONEY_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x82:
		{
			gWarehouse.CGWarehouseClose(aIndex);

			break;
		}

		case 0x83:
		{
			gWarehouse.CGWarehousePasswordRecv((PMSG_WAREHOUSE_PASSWORD_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x86:
		{
			gChaosBox.CGChaosMixRecv((PMSG_CHAOS_MIX_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x87:
		{
			gChaosBox.CGChaosMixCloseRecv(aIndex);

			break;
		}

		case 0x88:
		{
			gChaosBox.CGChaosMixRateRecv((PMSG_CHAOS_MIX_RATE_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x90:
		{
			gDevilSquare.CGDevilSquareEnterRecv((PMSG_DEVIL_SQUARE_ENTER_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x91:
		{
			CGEventRemainTimeRecv((PMSG_EVENT_REMAIN_TIME_RECV*)lpMsg, aIndex);

			break;
		}

		case 0x95:
		{
			gNpcTalk.NpcGoldenArcherRegisterCount((PMSG_GOLDEN_ARCHER_REG_COUNT*)lpMsg, aIndex);

			break;
		}

		case 0x97:
		{
			gNpcTalk.CGNpcTalkCloseRecv(aIndex);

			break;
		}

		case 0x9D:
		{
			gNpcTalk.NpcGoldenArcherRegisterLuckyNumber((PMSG_GOLDEN_ARCHER_REG_LUCKYNUM*)lpMsg, aIndex);

			break;
		}

		case 0x9A:
		{
			gBloodCastle.CGBloodCastleEnterRecv((PMSG_BLOOD_CASTLE_ENTER_RECV*)lpMsg, aIndex);

			break;
		}

		case 0xA0:
		{
			gQuest.CGQuestInfoRecv(aIndex);

			break;
		}

		case 0xA2:
		{
			gQuest.CGQuestStateRecv((PMSG_QUEST_STATE_RECV*)lpMsg, aIndex);

			break;
		}

		case 0xB0:
		{
			gSkillManager.CGSkillTeleportAllyRecv((PMSG_SKILL_TELEPORT_ALLY_RECV*)lpMsg, aIndex);

			break;
		}

		case 0xF1:
		{
			switch (lpMsg[3])
			{
				case 0x01:
				{
					CGConnectAccountRecv((PMSG_CONNECT_ACCOUNT_RECV*)lpMsg, aIndex);

					break;
				}

				case 0x02:
				{
					CGCloseClientRecv((PMSG_CLOSE_CLIENT_RECV*)lpMsg, aIndex);

					break;
				}

				case 0x03:
				{
					CGSetLangRecv((PMSG_SET_LANG_RECV*)lpMsg, aIndex);

					break;
				}

				case 0x04:
				{
					CGSetHwidRecv((PMSG_SET_HWID_RECV*)lpMsg, aIndex);

					break;
				}
			}

			break;
		}

		case 0xF3:
		{
			switch (lpMsg[3])
			{
				case 0x00:
				{
					CGCharacterListRecv(aIndex);

					break;
				}

				case 0x01:
				{
					CGCharacterCreateRecv((PMSG_CHARACTER_CREATE_RECV*)lpMsg, aIndex);

					break;
				}

				case 0x02:
				{
					CGCharacterDeleteRecv((PMSG_CHARACTER_DELETE_RECV*)lpMsg, aIndex);

					break;
				}

				case 0x03:
				{
					CGCharacterInfoRecv((PMSG_CHARACTER_INFO_RECV*)lpMsg, aIndex);

					break;
				}

				case 0x06:
				{
					CGLevelUpPointRecv((PMSG_LEVEL_UP_POINT_RECV*)lpMsg, aIndex);

					break;
				}

				case 0x12:
				{
					CGCharacterMoveViewportEnableRecv(aIndex);

					break;
				}

				case 0x30:
				{
					CGOptionDataRecv((PMSG_OPTION_DATA_RECV*)lpMsg, aIndex);

					break;
				}
			}

			break;
		}
	}
}

void CGChatRecv(PMSG_CHAT_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	char name[11] = { 0 };

	memcpy(name, lpMsg->name, sizeof(lpMsg->name));

	if (strcmp(name, lpObj->Name) != 0)
	{
		return;
	}

	lpMsg->message[(sizeof(lpMsg->message) - 1)] = 0;

	gLog.Output(LOG_CHAT, "[General][%s][%s] - (Message: %s)", lpObj->Account, lpObj->Name, lpMsg->message);

	gFilter.CheckSyntax(lpMsg->message);

	if (lpMsg->message[0] == '/')
	{
		if (gCommandManager.ManagementCore(&gObj[aIndex], lpMsg->message) == 0)
		{
			return;
		}
		else
		{
			return;
		}
	}

	if (lpObj->ChatLimitTime > 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(63, lpObj->Lang), lpObj->ChatLimitTime);

		return;
	}

	if ((lpObj->Penalty & 2) != 0)
	{
		return;
	}

	PMSG_CHAT_SEND pMsg;

	pMsg.header.set(0x00, sizeof(pMsg));

	memcpy(pMsg.name, lpMsg->name, sizeof(pMsg.name));

	memcpy(pMsg.message, lpMsg->message, sizeof(pMsg.message));

	if (lpMsg->message[0] == '~')
	{
		if (OBJECT_RANGE(lpObj->PartyNumber) != 0)
		{
			for (int n = 0; n < MAX_PARTY_USER; n++)
			{
				if (OBJECT_RANGE(gParty.m_PartyInfo[lpObj->PartyNumber].Index[n]) != 0)
				{
					DataSend(gParty.m_PartyInfo[lpObj->PartyNumber].Index[n], (BYTE*)&pMsg, pMsg.header.size);
				}
			}
		}
	}
	else if (lpMsg->message[0] == '@')
	{
		if (lpObj->Guild != 0)
		{
			if (lpMsg->message[1] == '>')
			{
				if (strcmp(lpObj->Name, lpObj->Guild->Members[0].Name) == 0)
				{
					gGuild.GDGuildGlobalNoticeSend(lpObj->Guild->Name, &lpMsg->message[2]);
				}
			}
			else
			{
				if (lpObj->Guild->Number != 0)
				{
					gGuild.GDGuildGlobalChatSend(lpObj->Guild->Name, lpMsg->name, lpMsg->message);
				}
			}
		}
	}
	else
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		MsgSendV2(lpObj, (BYTE*)&pMsg, pMsg.header.size);
	}
}

void CGChatWhisperRecv(PMSG_CHAT_WHISPER_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	char name[11] = { 0 };

	memcpy(name, lpMsg->name, sizeof(lpMsg->name));

	if (lpObj->ChatLimitTime > 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(63, lpObj->Lang), lpObj->ChatLimitTime);

		return;
	}

	if ((lpObj->Penalty & 2) != 0)
	{
		return;
	}

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		GDGlobalWhisperSend(aIndex, name, lpMsg->message);

		return;
	}

	if (aIndex == lpTarget->Index)
	{
		return;
	}

	lpMsg->message[(sizeof(lpMsg->message) - 1)] = 0;

	gLog.Output(LOG_CHAT, "[Whisper][%s][%s] - (Name: %s, Message: %s)", lpObj->Account, lpObj->Name, name, lpMsg->message);

	gFilter.CheckSyntax(lpMsg->message);

	GCChatWhisperSend(lpTarget->Index, lpObj->Name, lpMsg->message);
}

void CGMainCheckRecv(PMSG_MAIN_CHECK_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gServerInfo.m_MainChecksum != 0)
	{
		if (lpObj->CheckSumTableNum < 0 || lpObj->CheckSumTableNum >= MAX_CHECKSUM_KEY)
		{
			gLog.Output(LOG_HACK, "[%s][%s] Main CheckSum error", lpObj->Account, lpObj->Name);

			GCCloseClientSend(aIndex, 0);

			return;
		}

		if (gCheckSum[lpObj->CheckSumTableNum] != lpMsg->key)
		{
			gLog.Output(LOG_HACK, "[%s][%s] Invalid Main CheckSum", lpObj->Account, lpObj->Name);

			GCCloseClientSend(aIndex, 0);

			return;
		}
	}

	lpObj->CheckSumTime = 0;
}

void CGLiveClientRecv(PMSG_LIVE_CLIENT_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected != OBJECT_LOGGED && lpObj->Connected != OBJECT_ONLINE)
	{
		return;
	}

	//LogAdd(LOG_RED, "[%d] Latency: %d", lpObj->Index, abs((int)((GetTickCount() - lpObj->ServerTickCount) - (lpMsg->TickCount - lpObj->ClientTickCount))));

	if (gServerInfo.m_CheckLatencyHack != 0 && abs((int)((GetTickCount() - lpObj->ServerTickCount) - (lpMsg->TickCount - lpObj->ClientTickCount))) > gServerInfo.m_CheckLatencyHackTolerance)
	{
		gLog.Output(LOG_HACK, "[%s][%s] Client Latency Hack Detected [%d][%d]", lpObj->Account, lpObj->Name, (GetTickCount() - lpObj->ServerTickCount), (lpMsg->TickCount - lpObj->ClientTickCount));

		GCCloseClientSend(aIndex, 0);

		return;
	}

	if (lpObj->Connected == OBJECT_ONLINE && gServerInfo.m_CheckSpeedHack != 0 && lpMsg->PhysiSpeed > lpObj->PhysiSpeed && abs((int)(lpObj->PhysiSpeed - lpMsg->PhysiSpeed)) > gServerInfo.m_CheckSpeedHackTolerance)
	{
		gLog.Output(LOG_HACK, "[%s][%s] Client Speed Hack Detected [%d][%d]", lpObj->Account, lpObj->Name, lpObj->PhysiSpeed, lpMsg->PhysiSpeed);

		GCCloseClientSend(aIndex, 0);

		return;
	}

	if (lpObj->Connected == OBJECT_ONLINE && gServerInfo.m_CheckSpeedHack != 0 && lpMsg->MagicSpeed > lpObj->MagicSpeed && abs((int)(lpObj->MagicSpeed - lpMsg->MagicSpeed)) > gServerInfo.m_CheckSpeedHackTolerance)
	{
		gLog.Output(LOG_HACK, "[%s][%s] Client Speed Hack Detected [%d][%d]", lpObj->Account, lpObj->Name, lpObj->MagicSpeed, lpMsg->MagicSpeed);

		GCCloseClientSend(aIndex, 0);

		return;
	}

	lpObj->ConnectTickCount = GetTickCount();
}

void CGPositionRecv(PMSG_POSITION_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGS(aIndex) == 0)
	{
		return;
	}

	if (lpObj->Live == 0 || lpObj->Teleport != 0)
	{
		return;
	}

	if (gObjCheckMapTile(lpObj, 2) != 0)
	{
		return;
	}

	if (lpMsg->x < (lpObj->X - 15) || lpMsg->x >(lpObj->X + 15) || lpMsg->y < (lpObj->Y - 15) || lpMsg->y >(lpObj->Y + 15))
	{
		return;
	}

	gMap[lpObj->Map].DelStandAttr(lpObj->OldX, lpObj->OldY);

	lpObj->X = lpMsg->x;

	lpObj->Y = lpMsg->y;

	lpObj->TX = lpMsg->x;

	lpObj->TY = lpMsg->y;

	lpObj->OldX = lpMsg->x;

	lpObj->OldY = lpMsg->y;

	gMap[lpObj->Map].SetStandAttr(lpObj->TX, lpObj->TY);

	PMSG_POSITION_SEND pMsg;

	pMsg.header.set(PROTOCOL_CODE3, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.x = (BYTE)lpObj->TX;

	pMsg.y = (BYTE)lpObj->TY;

	if (lpObj->Type == OBJECT_USER)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
	}

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].type == OBJECT_USER)
		{
			if (lpObj->VpPlayer2[n].state != OBJECT_EMPTY && lpObj->VpPlayer2[n].state != OBJECT_DIECMD && lpObj->VpPlayer2[n].state != OBJECT_DIED)
			{
				DataSend(lpObj->VpPlayer2[n].index, (BYTE*)&pMsg, pMsg.header.size);
			}
		}
	}
}

void CGActionRecv(PMSG_ACTION_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	lpObj->Dir = lpMsg->dir;

	lpObj->ActionNumber = lpMsg->action;

	if (lpObj->ActionNumber == ACTION_SIT1)
	{
		lpObj->ViewState = 2;

		lpObj->Rest = lpObj->ActionNumber;
	}
	else if (lpObj->ActionNumber == ACTION_POSE1)
	{
		lpObj->ViewState = 3;

		lpObj->Rest = lpObj->ActionNumber;
	}
	else if (lpObj->ActionNumber == ACTION_HEALING1)
	{
		lpObj->ViewState = 4;

		lpObj->Rest = lpObj->ActionNumber;
	}

	PMSG_ACTION_SEND pMsg;

	pMsg.header.set(0x18, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.dir = lpMsg->dir;

	pMsg.action = lpMsg->action;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].type == OBJECT_USER)
		{
			if (lpObj->VpPlayer2[n].state != OBJECT_EMPTY && lpObj->VpPlayer2[n].state != OBJECT_DIECMD && lpObj->VpPlayer2[n].state != OBJECT_DIED)
			{
				DataSend(lpObj->VpPlayer2[n].index, (BYTE*)&pMsg, pMsg.header.size);
			}
		}
	}
}

void CGEventRemainTimeRecv(PMSG_EVENT_REMAIN_TIME_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_EVENT_REMAIN_TIME_SEND pMsg;

	pMsg.header.set(0x91, sizeof(pMsg));

	pMsg.EventType = lpMsg->EventType;

	pMsg.RemainTime = 0;

	switch (lpMsg->EventType)
	{
		case 1:
		{
			if (DS_LEVEL_RANGE(lpMsg->ItemLevel - 1) != 0)
			{
				if (gDevilSquare.GetState(lpMsg->ItemLevel - 1) == DS_STATE_EMPTY)
				{
					if (gDevilSquare.GetEnterEnabled(lpMsg->ItemLevel - 1) == 0)
					{
						pMsg.RemainTime = gDevilSquare.GetRemainTime(lpMsg->ItemLevel - 1);
					}
				}
				else
				{
					pMsg.RemainTime = gDevilSquare.GetRemainTime(lpMsg->ItemLevel - 1);
				}
			}
			else
			{
				pMsg.EventType = 0;
			}

			break;
		}

		case 2:
		{
			if (BC_LEVEL_RANGE(lpMsg->ItemLevel - 1) != 0)
			{
				if (gBloodCastle.GetState(lpMsg->ItemLevel - 1) == BC_STATE_EMPTY)
				{
					if (gBloodCastle.GetEnterEnabled(lpMsg->ItemLevel - 1) == 0)
					{
						pMsg.RemainTime = gBloodCastle.GetRemainTime(lpMsg->ItemLevel - 1);
					}
				}
				else
				{
					pMsg.RemainTime = gBloodCastle.GetRemainTime(lpMsg->ItemLevel - 1);
				}
			}
			else
			{
				pMsg.EventType = 0;
			}

			break;
		}

		default:
		{
			pMsg.EventType = 0;

			break;
		}
	}

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CGMoveRecv(PMSG_MOVE_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGS(aIndex) == 0)
	{
		return;
	}

	if (lpObj->RegenOk > 0)
	{
		return;
	}

	if (lpObj->Live == 0 || lpObj->Teleport != 0)
	{
		return;
	}

	if (gObjCheckMapTile(lpObj, 1) != 0)
	{
		return;
	}

	if ((GetTickCount() - lpObj->LastMoveTime) < 100)
	{
		return;
	}

	if (gEffectManager.CheckImmobilizeEffect(lpObj) != 0)
	{
		return;
	}

	lpObj->Dir = lpMsg->path[0] >> 4;

	lpObj->Rest = 0;

	lpObj->PathCur = 0;

	lpObj->PathCount = lpMsg->path[0] & 0x0F;

	lpObj->LastMoveTime = GetTickCount();

	memset(lpObj->PathX, 0, sizeof(lpObj->PathX));

	memset(lpObj->PathY, 0, sizeof(lpObj->PathY));

	memset(lpObj->PathOri, 0, sizeof(lpObj->PathOri));

	lpObj->TX = lpMsg->x;

	lpObj->TY = lpMsg->y;

	lpObj->PathCur = ((lpObj->PathCount>0) ? 1 : 0);

	lpObj->PathCount = ((lpObj->PathCount>0) ? (lpObj->PathCount + 1) : lpObj->PathCount);

	lpObj->PathStartEnd = 1;

	lpObj->PathX[0] = lpMsg->x;

	lpObj->PathY[0] = lpMsg->y;

	lpObj->PathDir[0] = lpObj->Dir;

	for (int n = 1; n < lpObj->PathCount; n++)
	{
		if ((n % 2) == 0)
		{
			lpObj->TX = lpObj->PathX[n - 1] + RoadPathTable[((lpMsg->path[((n + 1) / 2)] & 0x0F) * 2) + 0];

			lpObj->TY = lpObj->PathY[n - 1] + RoadPathTable[((lpMsg->path[((n + 1) / 2)] & 0x0F) * 2) + 1];

			lpObj->PathX[n] = lpObj->PathX[n - 1] + RoadPathTable[((lpMsg->path[((n + 1) / 2)] & 0x0F) * 2) + 0];

			lpObj->PathY[n] = lpObj->PathY[n - 1] + RoadPathTable[((lpMsg->path[((n + 1) / 2)] & 0x0F) * 2) + 1];

			lpObj->PathOri[n - 1] = lpMsg->path[((n + 1) / 2)] & 0x0F;

			lpObj->PathDir[n + 0] = lpMsg->path[((n + 1) / 2)] & 0x0F;
		}
		else
		{
			lpObj->TX = lpObj->PathX[n - 1] + RoadPathTable[((lpMsg->path[((n + 1) / 2)] / 0x10) * 2) + 0];

			lpObj->TY = lpObj->PathY[n - 1] + RoadPathTable[((lpMsg->path[((n + 1) / 2)] / 0x10) * 2) + 1];

			lpObj->PathX[n] = lpObj->PathX[n - 1] + RoadPathTable[((lpMsg->path[((n + 1) / 2)] / 0x10) * 2) + 0];

			lpObj->PathY[n] = lpObj->PathY[n - 1] + RoadPathTable[((lpMsg->path[((n + 1) / 2)] / 0x10) * 2) + 1];

			lpObj->PathOri[n - 1] = lpMsg->path[((n + 1) / 2)] / 0x10;

			lpObj->PathDir[n + 0] = lpMsg->path[((n + 1) / 2)] / 0x10;
		}
	}

	if (lpObj->TX < (lpObj->X - 15) || lpObj->TX >(lpObj->X + 15) || lpObj->TY < (lpObj->Y - 15) || lpObj->TY >(lpObj->Y + 15))
	{
		lpObj->PathCur = 0;

		lpObj->PathCount = 0;

		lpObj->PathStartEnd = 0;

		memset(lpObj->PathX, 0, sizeof(lpObj->PathX));

		memset(lpObj->PathY, 0, sizeof(lpObj->PathY));

		memset(lpObj->PathOri, 0, sizeof(lpObj->PathOri));

		gObjSetPosition(lpObj->Index, lpObj->X, lpObj->Y);

		return;
	}

	if (lpObj->PathCount > 0 && (gMap[lpObj->Map].CheckAttr(lpObj->TX, lpObj->TY, 4) != 0 || gMap[lpObj->Map].CheckAttr(lpObj->TX, lpObj->TY, 8) != 0))
	{
		lpObj->PathCur = 0;

		lpObj->PathCount = 0;

		lpObj->PathStartEnd = 0;

		memset(lpObj->PathX, 0, sizeof(lpObj->PathX));

		memset(lpObj->PathY, 0, sizeof(lpObj->PathY));

		memset(lpObj->PathOri, 0, sizeof(lpObj->PathOri));

		gObjSetPosition(lpObj->Index, lpObj->X, lpObj->Y);

		return;
	}

	gMap[lpObj->Map].DelStandAttr(lpObj->OldX, lpObj->OldY);

	lpObj->X = lpMsg->x;

	lpObj->Y = lpMsg->y;

	lpObj->TX = lpObj->TX;

	lpObj->TY = lpObj->TY;

	lpObj->OldX = lpObj->TX;

	lpObj->OldY = lpObj->TY;

	lpObj->ViewState = 0;

	gMap[lpObj->Map].SetStandAttr(lpObj->TX, lpObj->TY);

	PMSG_MOVE_SEND pMsg;

	pMsg.header.set(PROTOCOL_CODE1, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.x = (BYTE)lpObj->TX;

	pMsg.y = (BYTE)lpObj->TY;

	pMsg.dir = lpObj->Dir << 4;

	if (lpObj->Type == OBJECT_USER)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
	}

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].type == OBJECT_USER)
		{
			if (lpObj->VpPlayer2[n].state != OBJECT_EMPTY && lpObj->VpPlayer2[n].state != OBJECT_DIECMD && lpObj->VpPlayer2[n].state != OBJECT_DIED)
			{
				DataSend(lpObj->VpPlayer2[n].index, (BYTE*)&pMsg, pMsg.header.size);
			}
		}
	}
}

void CGConnectAccountRecv(PMSG_CONNECT_ACCOUNT_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected != OBJECT_CONNECTED)
	{
		CloseClient(aIndex);

		return;
	}

	if (gBlackList.CheckHardwareID(lpObj->HardwareID) != 0)
	{
		GCConnectAccountSend(aIndex, 5);

		return;
	}

	if (gBlackList.CheckIpAddress(lpObj->IpAddr) != 0)
	{
		GCConnectAccountSend(aIndex, 5);

		return;
	}

	if (memcmp(gServerInfo.m_ServerVersion, lpMsg->ClientVersion, sizeof(lpMsg->ClientVersion)) != 0)
	{
		GCConnectAccountSend(aIndex, 6);

		return;
	}

	if (memcmp(gServerInfo.m_ServerSerial, lpMsg->ClientSerial, sizeof(lpMsg->ClientSerial)) != 0)
	{
		GCConnectAccountSend(aIndex, 6);

		return;
	}

	if (lpObj->LoginMessageSend == 0)
	{
		lpObj->LoginMessageSend++;

		lpObj->LoginMessageCount++;

		lpObj->ConnectTickCount = GetTickCount();

		lpObj->ClientTickCount = lpMsg->TickCount;

		lpObj->ServerTickCount = GetTickCount();

		char account[11] = { 0 };

		PacketArgumentDecrypt(account, lpMsg->account, (sizeof(account) - 1));

		char password[11] = { 0 };

		PacketArgumentDecrypt(password, lpMsg->password, (sizeof(password) - 1));

		GJConnectAccountSend(aIndex, account, password, lpObj->IpAddr);
	}
}

void CGCloseClientRecv(PMSG_CLOSE_CLIENT_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected != OBJECT_LOGGED && lpObj->Connected != OBJECT_ONLINE)
	{
		return;
	}

	gObjectManager.CharacterGameCloseSet(aIndex, lpMsg->type);
}

void CGSetLangRecv(PMSG_SET_LANG_RECV* lpMsg, int aIndex)
{
	if (gObj[aIndex].Connected != OBJECT_CONNECTED)
	{
		return;
	}

	gObj[aIndex].Lang = lpMsg->lang;
}

void CGSetHwidRecv(PMSG_SET_HWID_RECV* lpMsg, int aIndex)
{
	if (gObj[aIndex].Connected != OBJECT_CONNECTED)
	{
		return;
	}

	strcpy_s(gObj[aIndex].HardwareID, lpMsg->HardwareId);
}

void CGCharacterListRecv(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected != OBJECT_LOGGED)
	{
		return;
	}

	GDCharacterListSend(aIndex);
}

void CGCharacterCreateRecv(PMSG_CHARACTER_CREATE_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected != OBJECT_LOGGED)
	{
		return;
	}

	PMSG_CHARACTER_CREATE_SEND pMsg;

	pMsg.header.set(0xF3, 0x01, sizeof(pMsg));

	pMsg.result = 0;

	memcpy(pMsg.name, lpMsg->name, sizeof(pMsg.name));

	if (gServerInfo.m_CharacterCreateSwitch == 0)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (lpMsg->Class != DB_CLASS_DW && lpMsg->Class != DB_CLASS_DK && lpMsg->Class != DB_CLASS_FE && lpMsg->Class != DB_CLASS_MG)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	if (lpObj->Class == DB_CLASS_MG && lpObj->ClassCode < 3)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	char name[11] = { 0 };

	memcpy(name, lpMsg->name, sizeof(lpMsg->name));

	GDCharacterCreateSend(aIndex, name, lpMsg->Class);
}

void CGCharacterDeleteRecv(PMSG_CHARACTER_DELETE_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected != OBJECT_LOGGED)
	{
		return;
	}

	if (lpObj->EnableDelCharacter == 0)
	{
		return;
	}

	PMSG_CHARACTER_DELETE_SEND pMsg;

	pMsg.header.set(0xF3, 0x02, sizeof(pMsg));

	pMsg.result = 0;

	if (gObjCheckPersonalCode(aIndex, lpMsg->PersonalCode) == 0)
	{
		pMsg.result = 2;

		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

		return;
	}

	char name[11] = { 0 };

	memcpy(name, lpMsg->name, sizeof(lpMsg->name));

	GDCharacterDeleteSend(aIndex, name);
}

void CGCharacterInfoRecv(PMSG_CHARACTER_INFO_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected != OBJECT_LOGGED)
	{
		return;
	}

	lpObj->EnableDelCharacter = 0;

	char name[11] = { 0 };

	memcpy(name, lpMsg->name, sizeof(lpMsg->name));

	GDCharacterInfoSend(aIndex, name);
}

void CGLevelUpPointRecv(PMSG_LEVEL_UP_POINT_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_LEVEL_UP_POINT_SEND pMsg;

	pMsg.header.set(0xF3, 0x06, sizeof(pMsg));

	pMsg.result = 0;

	if (gObjectManager.CharacterLevelUpPointAdd(lpObj, lpMsg->type, 1) != 0)
	{
		pMsg.result = 16 + lpMsg->type;

		switch (lpMsg->type)
		{
			case 2: // Vitality
			{
				pMsg.MaxLifeAndMana = GET_MAX_WORD_VALUE((lpObj->MaxLife + lpObj->AddLife));

				break;
			}

			case 3: // Energy
			{
				pMsg.MaxLifeAndMana = GET_MAX_WORD_VALUE((lpObj->MaxMana + lpObj->AddMana));

				break;
			}
		}

		pMsg.MaxBP = GET_MAX_WORD_VALUE((lpObj->MaxBP + lpObj->AddBP));

#if(GAMESERVER_EXTRA==1)

		pMsg.ViewPoint = (DWORD)(lpObj->LevelUpPoint);

		pMsg.ViewMaxHP = (DWORD)(lpObj->MaxLife + lpObj->AddLife);

		pMsg.ViewMaxMP = (DWORD)(lpObj->MaxMana + lpObj->AddMana);

		pMsg.ViewMaxBP = (DWORD)(lpObj->MaxBP + lpObj->AddBP);

		pMsg.ViewStrength = lpObj->Strength;

		pMsg.ViewDexterity = lpObj->Dexterity;

		pMsg.ViewVitality = lpObj->Vitality;

		pMsg.ViewEnergy = lpObj->Energy;

#endif
	}

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CGCharacterMoveViewportEnableRecv(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	lpObj->RegenOk = ((lpObj->RegenOk == 1) ? 2 : lpObj->RegenOk);
}

void CGOptionDataRecv(PMSG_OPTION_DATA_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	GDOptionDataSaveSend(aIndex, lpMsg->SkillKey, lpMsg->GameOption, lpMsg->QKey, lpMsg->WKey, lpMsg->EKey, lpMsg->ChatWindow);
}

void GCChatTargetSend(LPOBJ lpObj, int aIndex, char* message)
{
	int size = strlen(message);

	size = ((size>MAX_CHAT_MESSAGE_SIZE) ? MAX_CHAT_MESSAGE_SIZE : size);

	PMSG_CHAT_TARGET_SEND pMsg;

	pMsg.header.set(0x01, (sizeof(pMsg) - (sizeof(pMsg.message) - (size + 1))));

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	memcpy(pMsg.message, message, size);

	pMsg.message[size] = 0;

	DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
}

void GCChatWhisperSend(int aIndex, char* name, char* message)
{
	int size = strlen(message);

	size = ((size>MAX_CHAT_MESSAGE_SIZE) ? MAX_CHAT_MESSAGE_SIZE : size);

	PMSG_CHAT_WHISPER_SEND pMsg;

	pMsg.header.set(0x02, (sizeof(pMsg) - (sizeof(pMsg.message) - (size + 1))));

	memcpy(pMsg.name, name, sizeof(pMsg.name));

	memcpy(pMsg.message, message, size);

	pMsg.message[size] = 0;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCMainCheckSend(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	lpObj->CheckSumTableNum = GetLargeRand() % MAX_CHECKSUM_KEY;

	lpObj->CheckSumTime = GetTickCount();

	PMSG_MAIN_CHECK_SEND pMsg;

	pMsg.header.set(0x03, sizeof(pMsg));

	pMsg.key = (((GetLargeRand() % 64) * 16) | ((lpObj->CheckSumTableNum & 0x3F0) * 64) | ((lpObj->CheckSumTableNum & 0x0F)) ^ 0xB479);

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCEventStateSend(int aIndex, BYTE state, BYTE event)
{
	PMSG_EVENT_STATE_SEND pMsg;

	pMsg.header.set(0x0B, sizeof(pMsg));

	pMsg.state = state;

	pMsg.event = event;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCEventStateSendToAll(BYTE state, BYTE event)
{
	PMSG_EVENT_STATE_SEND pMsg;

	pMsg.header.set(0x0B, sizeof(pMsg));

	pMsg.state = state;

	pMsg.event = event;

	DataSendAll((BYTE*)&pMsg, pMsg.header.size);
}

void GCEventStateSendToAll(int map, BYTE InvasionState, BYTE InvasionIndex)
{
	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnectedGP(n) == 0)
		{
			continue;
		}

		if (map == gObj[n].Map)
		{
			GCEventStateSend(n, InvasionState, InvasionIndex);
		}
	}
}

void GCServerMsgSend(int aIndex, BYTE msg)
{
	PMSG_SERVER_MSG_SEND pMsg;

	pMsg.header.set(0x0C, sizeof(pMsg));

	pMsg.MsgNumber = msg;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCWeatherSend(int aIndex, BYTE weather)
{
	PMSG_WEATHER_SEND pMsg;

	pMsg.header.set(0x0F, sizeof(pMsg));

	pMsg.weather = weather;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCDamageSend(int aIndex, int bIndex, BYTE flag, int damage, int type)
{
	PMSG_DAMAGE_SEND pMsg;

	pMsg.header.set(PROTOCOL_CODE2, sizeof(pMsg));

	pMsg.index[0] = (SET_NUMBERHB(bIndex) & 0x7F) | ((flag & 1) << 7);

	pMsg.index[1] = SET_NUMBERLB(bIndex);

	pMsg.damage[0] = (SET_NUMBERHB(GET_MAX_WORD_VALUE(damage)) & 0x0F) | (type & 0xF0);

	pMsg.damage[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(damage));

#if(GAMESERVER_EXTRA==1)

	pMsg.ViewCurHP = (DWORD)gObj[bIndex].Life;

	pMsg.ViewDamageHP = damage;

#endif

	if (gObj[aIndex].Type == OBJECT_USER)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
	}

	if (gObj[bIndex].Type == OBJECT_USER)
	{
		DataSend(bIndex, (BYTE*)&pMsg, pMsg.header.size);
	}
}

void GCUserDieSend(LPOBJ lpObj, int aIndex, int skill, int bIndex)
{
	PMSG_USER_DIE_SEND pMsg;

	pMsg.header.set(0x17, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.skill = skill;

	pMsg.killer[0] = SET_NUMBERHB(bIndex);

	pMsg.killer[1] = SET_NUMBERLB(bIndex);

	if (lpObj->Type == OBJECT_USER)
	{
		DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
	}

	MsgSendV2(lpObj, (BYTE*)&pMsg, pMsg.header.size);
}

void GCActionSend(LPOBJ lpObj, BYTE action, int aIndex, int bIndex)
{
	PMSG_ACTION_SEND pMsg;

	pMsg.header.set(0x18, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.dir = lpObj->Dir;

	pMsg.action = action;

	MsgSendV2(lpObj, (BYTE*)&pMsg, pMsg.header.size);
}

void GCMoneySend(int aIndex, DWORD money)
{
	PMSG_ITEM_GET_SEND pMsg;

	pMsg.header.setE(0x22, sizeof(pMsg));

	pMsg.result = 0xFE;

	memset(pMsg.ItemInfo, 0, sizeof(pMsg.ItemInfo));

	pMsg.ItemInfo[0] = SET_NUMBERHB(SET_NUMBERHW(money));

	pMsg.ItemInfo[1] = SET_NUMBERLB(SET_NUMBERHW(money));

	pMsg.ItemInfo[2] = SET_NUMBERHB(SET_NUMBERLW(money));

	pMsg.ItemInfo[3] = SET_NUMBERLB(SET_NUMBERLW(money));

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCLifeSend(int aIndex, BYTE type, int life)
{
	PMSG_LIFE_SEND pMsg;

	pMsg.header.set(0x26, sizeof(pMsg));

	pMsg.type = type;

	pMsg.life[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(life));

	pMsg.life[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(life));

	pMsg.flag = 0;

#if(GAMESERVER_EXTRA==1)

	pMsg.ViewHP = life;

#endif

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCManaSend(int aIndex, BYTE type, int mana, int bp)
{
	PMSG_MANA_SEND pMsg;

	pMsg.header.set(0x27, sizeof(pMsg));

	pMsg.type = type;

	pMsg.mana[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(mana));

	pMsg.mana[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(mana));

	pMsg.bp[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(bp));

	pMsg.bp[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(bp));

#if(GAMESERVER_EXTRA==1)

	pMsg.ViewMP = mana;

	pMsg.ViewBP = bp;

#endif

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCItemUseSpecialTimeSend(int aIndex, BYTE number, int time)
{
	PMSG_ITEM_SPECIAL_TIME_SEND pMsg;

	pMsg.header.setE(0x29, sizeof(pMsg));

	pMsg.number = number;

	pMsg.time = time;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCMapAttrSend(int aIndex, BYTE type, BYTE attr, BYTE flag, BYTE count, PMSG_MAP_ATTR* lpInfo)
{
	BYTE send[256];

	PMSG_MAP_ATTR_SEND pMsg;

	pMsg.header.set(0x46, 0);

	int size = sizeof(pMsg);

	pMsg.type = type;

	pMsg.attr = attr;

	pMsg.flag = flag;

	pMsg.count = count;

	PMSG_MAP_ATTR info;

	for (int n = 0; n < count; n++)
	{
		info.x = lpInfo[n].x;

		info.y = lpInfo[n].y;

		info.tx = lpInfo[n].tx;

		info.ty = lpInfo[n].ty;

		memcpy(&send[size], &info, sizeof(info));

		size += sizeof(info);
	}

	pMsg.header.size = size;

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(aIndex, send, size);
}

void GCGuildMasterQuestionSend(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Interface.use != 0)
	{
		return;
	}

	PBMSG_HEAD pMsg;

	pMsg.set(0x54, sizeof(pMsg));

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.size);
}

void GCMonsterDieSend(int aIndex, int bIndex, int experience, int damage, BYTE flag)
{
	PMSG_REWARD_EXPERIENCE_SEND pMsg;

	pMsg.header.setE(0x9C, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(bIndex) | (flag * 0x80);

	pMsg.index[1] = SET_NUMBERLB(bIndex);

	pMsg.experience[0] = SET_NUMBERHW(experience);

	pMsg.experience[1] = SET_NUMBERLW(experience);

	pMsg.damage[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(damage));

	pMsg.damage[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(damage));

#if(GAMESERVER_EXTRA==1)

	pMsg.ViewDamageHP = damage;

	pMsg.ViewExperience = gObj[aIndex].Experience;

	pMsg.ViewNextExperience = gObj[aIndex].NextExperience;

#endif

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCRewardExperienceSend(int aIndex, int experience)
{
	PMSG_REWARD_EXPERIENCE_SEND pMsg;

	pMsg.header.setE(0x9C, sizeof(pMsg));

	pMsg.index[0] = 0xFF;

	pMsg.index[1] = 0xFF;

	pMsg.experience[0] = SET_NUMBERHW(experience);

	pMsg.experience[1] = SET_NUMBERLW(experience);

	pMsg.damage[0] = 0;

	pMsg.damage[1] = 0;

#if(GAMESERVER_EXTRA==1)

	pMsg.ViewDamageHP = 0;

	pMsg.ViewExperience = gObj[aIndex].Experience;

	pMsg.ViewNextExperience = gObj[aIndex].NextExperience;

#endif

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCCharacterCreationEnableSend(int aIndex, BYTE result)
{
	PMSG_CHARACTER_CREATION_ENABLE_SEND pMsg;

	pMsg.header.set(0xDE, sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCConnectClientSend(int aIndex, BYTE result)
{
	PMSG_CONNECT_CLIENT_SEND pMsg;

	pMsg.header.set(0xF1, 0x00, sizeof(pMsg));

	pMsg.result = result;

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.ClientVersion[0] = gServerInfo.m_ServerVersion[0];

	pMsg.ClientVersion[1] = gServerInfo.m_ServerVersion[1];

	pMsg.ClientVersion[2] = gServerInfo.m_ServerVersion[2];

	pMsg.ClientVersion[3] = gServerInfo.m_ServerVersion[3];

	pMsg.ClientVersion[4] = gServerInfo.m_ServerVersion[4];

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

	gObj[aIndex].ConnectTickCount = GetTickCount();
}

void GCConnectAccountSend(int aIndex, BYTE result)
{
	PMSG_CONNECT_ACCOUNT_SEND pMsg;

	pMsg.header.set(0xF1, 0x01, sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCConnectAccountSend(int aIndex, BYTE result, SOCKET socket)
{
	PMSG_CONNECT_ACCOUNT_SEND pMsg;

	pMsg.header.set(0xF1, 0x01, sizeof(pMsg));

	pMsg.result = result;

	DataSendSocket(socket, (BYTE*)&pMsg, pMsg.header.size);
}

void GCCloseClientSend(int aIndex, BYTE result)
{
	PMSG_CLOSE_CLIENT_SEND pMsg;

	pMsg.header.setE(0xF1, 0x02, sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCCharacterRegenSend(LPOBJ lpObj)
{
	PMSG_CHARACTER_REGEN_SEND pMsg;

	pMsg.header.setE(0xF3, 0x04, sizeof(pMsg));

	pMsg.X = (BYTE)lpObj->X;

	pMsg.Y = (BYTE)lpObj->Y;

	pMsg.Map = lpObj->Map;

	pMsg.Dir = lpObj->Dir;

	pMsg.Life = GET_MAX_WORD_VALUE(lpObj->Life);

	pMsg.Mana = GET_MAX_WORD_VALUE(lpObj->Mana);

	pMsg.BP = GET_MAX_WORD_VALUE(lpObj->BP);

	pMsg.Experience = lpObj->Experience;

	pMsg.Money = lpObj->Money;

#if(GAMESERVER_EXTRA==1)

	pMsg.ViewCurHP = (DWORD)(lpObj->Life);

	pMsg.ViewCurMP = (DWORD)(lpObj->Mana);

	pMsg.ViewCurBP = (DWORD)(lpObj->BP);

#endif

	DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
}

void GCLevelUpSend(LPOBJ lpObj)
{
	PMSG_LEVEL_UP_SEND pMsg;

	pMsg.header.set(0xF3, 0x05, sizeof(pMsg));

	pMsg.Level = lpObj->Level;

	pMsg.LevelUpPoint = lpObj->LevelUpPoint;

	pMsg.MaxLife = GET_MAX_WORD_VALUE((lpObj->MaxLife + lpObj->AddLife));

	pMsg.MaxMana = GET_MAX_WORD_VALUE((lpObj->MaxMana + lpObj->AddMana));

	pMsg.MaxBP = GET_MAX_WORD_VALUE((lpObj->MaxBP + lpObj->AddBP));

	pMsg.FruitAddPoint = lpObj->FruitAddPoint;

	pMsg.MaxFruitAddPoint = gFruit.GetMaxFruitPoint(lpObj);

	pMsg.FruitSubPoint = lpObj->FruitSubPoint;

	pMsg.MaxFruitSubPoint = gFruit.GetMaxFruitPoint(lpObj);

#if(GAMESERVER_EXTRA==1)

	pMsg.ViewPoint = (DWORD)(lpObj->LevelUpPoint);

	pMsg.ViewMaxHP = (DWORD)(lpObj->MaxLife + lpObj->AddLife);

	pMsg.ViewMaxMP = (DWORD)(lpObj->MaxMana + lpObj->AddMana);

	pMsg.ViewMaxBP = (DWORD)(lpObj->MaxBP + lpObj->AddBP);

	pMsg.ViewExperience = lpObj->Experience;

	pMsg.ViewNextExperience = lpObj->NextExperience;

#endif

	DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
}

void GCMonsterDamageSend(int aIndex, int damage)
{
	PMSG_MONSTER_DAMAGE_SEND pMsg;

	pMsg.header.set(0xF3, 0x07, sizeof(pMsg));

	pMsg.damage[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(damage));

	pMsg.damage[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(damage));

#if(GAMESERVER_EXTRA==1)

	pMsg.ViewCurHP = (DWORD)gObj[aIndex].Life;

	pMsg.ViewDamageHP = damage;

#endif

	if (gObj[aIndex].Type == OBJECT_USER)
	{
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
	}
}

void GCPKLevelSend(int aIndex, int PKLevel)
{
	PMSG_PK_LEVEL_SEND pMsg;

	pMsg.header.set(0xF3, 0x08, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.PKLevel = PKLevel;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

	MsgSendV2(&gObj[aIndex], (BYTE*)&pMsg, pMsg.header.size);
}

void GCSummonLifeSend(int aIndex, int life, int MaxLife)
{
	if (MaxLife <= 0)
	{
		return;
	}

	PMSG_SUMMON_LIFE_SEND pMsg;

	pMsg.header.set(0xF3, 0x20, sizeof(pMsg));

	pMsg.life = (life * 100) / MaxLife;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCTimeViewSend(int aIndex, int time)
{
	PMSG_TIME_VIEW_SEND pMsg;

	pMsg.header.set(0xF3, 0x22, sizeof(pMsg));

	pMsg.time = time;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCFireworksSend(LPOBJ lpObj, int x, int y)
{
	PMSG_FIREWORKS_SEND pMsg;

	pMsg.header.set(0xF3, 0x40, sizeof(pMsg));

	pMsg.type = 0;

	pMsg.x = x;

	pMsg.y = y;

	DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);

	MsgSendV2(lpObj, (BYTE*)&pMsg, pMsg.header.size);
}

void GCServerCommandSend(int aIndex, BYTE type, BYTE cmd1, BYTE cmd2)
{
	PMSG_SERVER_COMMAND_SEND pMsg;

	pMsg.header.set(0xF3, 0x40, sizeof(pMsg));

	pMsg.type = type;

	pMsg.cmd1 = cmd1;

	pMsg.cmd2 = cmd2;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void GCNewCharacterInfoSend(LPOBJ lpObj)
{
#if(GAMESERVER_EXTRA==1)

	PMSG_NEW_CHARACTER_INFO_SEND pMsg;

	pMsg.header.set(0xF3, 0xE0, sizeof(pMsg));

	pMsg.Level = lpObj->Level;

	pMsg.LevelUpPoint = (DWORD)(lpObj->LevelUpPoint);

	pMsg.Experience = lpObj->Experience;

	pMsg.NextExperience = lpObj->NextExperience;

	pMsg.Strength = (DWORD)(lpObj->Strength);

	pMsg.Dexterity = (DWORD)(lpObj->Dexterity);

	pMsg.Vitality = (DWORD)(lpObj->Vitality);

	pMsg.Energy = (DWORD)(lpObj->Energy);

	pMsg.Life = (DWORD)(lpObj->Life);

	pMsg.MaxLife = (DWORD)(lpObj->MaxLife + lpObj->AddLife);

	pMsg.Mana = (DWORD)(lpObj->Mana);

	pMsg.MaxMana = (DWORD)((lpObj->MaxMana + lpObj->AddMana));

	pMsg.BP = (DWORD)(lpObj->BP);

	pMsg.MaxBP = (DWORD)(lpObj->MaxBP + lpObj->AddBP);

	pMsg.FruitAddPoint = lpObj->FruitAddPoint;

	pMsg.MaxFruitAddPoint = gFruit.GetMaxFruitPoint(lpObj);

	pMsg.ViewReset = (DWORD)(lpObj->Reset);

	pMsg.ViewGrandReset = (DWORD)(lpObj->GrandReset);

	DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);

#endif
}

void GCNewCharacterCalcSend(LPOBJ lpObj)
{
#if(GAMESERVER_EXTRA==1)

	PMSG_NEW_CHARACTER_CALC_SEND pMsg;

	pMsg.header.set(0xF3, 0xE1, sizeof(pMsg));

	pMsg.ViewCurHP = (DWORD)(lpObj->Life);

	pMsg.ViewMaxHP = (DWORD)(lpObj->MaxLife + lpObj->AddLife);

	pMsg.ViewCurMP = (DWORD)(lpObj->Mana);

	pMsg.ViewMaxMP = (DWORD)(lpObj->MaxMana + lpObj->AddMana);

	pMsg.ViewCurBP = (DWORD)(lpObj->BP);

	pMsg.ViewMaxBP = (DWORD)(lpObj->MaxBP + lpObj->AddBP);

	pMsg.ViewAddStrength = lpObj->AddStrength;

	pMsg.ViewAddDexterity = lpObj->AddDexterity;

	pMsg.ViewAddVitality = lpObj->AddVitality;

	pMsg.ViewAddEnergy = lpObj->AddEnergy;

	pMsg.ViewPhysiSpeed = lpObj->PhysiSpeed;

	pMsg.ViewMagicSpeed = lpObj->MagicSpeed;

	DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);

#endif
}

void GCHealthBarSend(LPOBJ lpObj)
{
	if (gServerInfo.m_MonsterHealthBarSwitch == 0)
	{
		return;
	}

	BYTE send[4096];

	PMSG_HEALTH_BAR_SEND pMsg;

	pMsg.header.set(0xF3, 0xE2, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_HEALTH_BAR info;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer[n].state != VIEWPORT_SEND && lpObj->VpPlayer[n].state != VIEWPORT_WAIT)
		{
			continue;
		}

		if (lpObj->VpPlayer[n].type != OBJECT_MONSTER)
		{
			continue;
		}

		if (OBJECT_RANGE(lpObj->VpPlayer[n].index) == 0)
		{
			continue;
		}

		LPOBJ lpTarget = &gObj[lpObj->VpPlayer[n].index];

		if (lpTarget->Live == 0 || lpTarget->State != OBJECT_PLAYING || OBJECT_RANGE(lpTarget->SummonIndex) != 0)
		{
			continue;
		}

		info.index = lpTarget->Index;

		info.type = (BYTE)lpTarget->Type;

		info.rateHP = (BYTE)((lpTarget->Life * 100) / (lpTarget->MaxLife + lpTarget->AddLife));

		memcpy(&send[size], &info, sizeof(info));

		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(lpObj->Index, send, size);
}