#include "stdafx.h"
#include "DSProtocol.h"
#include "BloodCastle.h"
#include "ChaosBox.h"
#include "CommandManager.h"
#include "Filter.h"
#include "FlyingDragons.h"
#include "Fruit.h"
#include "GameMain.h"
#include "Guild.h"
#include "ItemBagManager.h"
#include "ItemStack.h"
#include "ItemValue.h"
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
#include "Reconnect.h"
#include "ServerInfo.h"
#include "Util.h"
#include "Viewport.h"
#include "Warehouse.h"

void DataServerProtocolCore(BYTE head, BYTE* lpMsg, int size)
{
	switch (head)
	{
		case 0x01:
		{
			switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
			{
				case 0x00:
				{
					DGCharacterListRecv((SDHP_CHARACTER_LIST_RECV*)lpMsg);

					break;
				}

				case 0x01:
				{
					DGCharacterCreateRecv((SDHP_CHARACTER_CREATE_RECV*)lpMsg);

					break;
				}

				case 0x02:
				{
					DGCharacterDeleteRecv((SDHP_CHARACTER_DELETE_RECV*)lpMsg);

					break;
				}

				case 0x03:
				{
					DGCharacterInfoRecv((SDHP_CHARACTER_INFO_RECV*)lpMsg);

					break;
				}
			}

			break;
		}

		case 0x02:
		{
			switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
			{
				case 0x00:
				{
					DGCreateItemRecv((SDHP_CREATE_ITEM_RECV*)lpMsg);

					break;
				}

				case 0x02:
				{
					gWarehouse.DGWarehouseItemRecv((SDHP_WAREHOUSE_ITEM_RECV*)lpMsg);

					break;
				}

				case 0x03:
				{
					gWarehouse.DGWarehouseFreeRecv((SDHP_WAREHOUSE_FREE_RECV*)lpMsg);

					break;
				}
			}

			break;
		}

		case 0x03:
		{
			switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
			{
				case 0x00:
				{
					DGOptionDataRecv((SDHP_OPTION_DATA_RECV*)lpMsg);

					break;
				}
			}

			break;
		}

		case 0x04:
		{
			switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
			{
				case 0x00:
				{
					gNpcTalk.DGNpcGoldenArcherOpenRecv((SDHP_NPC_GOLDEN_ARCHER_OPEN_RECV*)lpMsg);

					break;
				}

				case 0x01:
				{
					gNpcTalk.DGNpcGoldenArcherRegisterCountRecv((SDHP_NPC_GOLDEN_ARCHER_REG_COUNT_RECV*)lpMsg);

					break;
				}

				case 0x02:
				{
					gNpcTalk.DGNpcGoldenArcherRegisterLuckyNumRecv((SDHP_NPC_GOLDEN_ARCHER_REG_LUCKYNUM_RECV*)lpMsg);

					break;
				}
			}

			break;
		}

		case 0x05:
		{
			switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
			{
				case 0x00:
				{
					DGGlobalNoticeRecv((SDHP_GLOBAL_NOTICE_RECV*)lpMsg);

					break;
				}

				case 0x01:
				{
					DGGlobalWhisperRecv((SDHP_GLOBAL_WHISPER_RECV*)lpMsg);

					break;
				}

				case 0x02:
				{
					DGGlobalWhisperEchoRecv((SDHP_GLOBAL_WHISPER_ECHO_RECV*)lpMsg);

					break;
				}

				case 0x03:
				{
					gCommandManager.DGGlobalPostRecv((SDHP_GLOBAL_POST_RECV*)lpMsg);

					break;
				}

				case 0x04:
				{
					gCommandManager.DGCommandResetRecv((SDHP_COMMAND_RESET_RECV*)lpMsg);

					break;
				}

				case 0x05:
				{
					gCommandManager.DGCommandGrandResetRecv((SDHP_COMMAND_RESET_RECV*)lpMsg);

					break;
				}
			}

			break;
		}

		case 0x07:
		{
			switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
			{
				case 0x00:
				{
					gGuild.DGGuildCreateRecv((SDHP_GUILD_CREATE_RECV*)lpMsg);

					break;
				}

				case 0x01:
				{
					gGuild.DGGuildDeleteRecv((SDHP_GUILD_DELETE_RECV*)lpMsg);

					break;
				}

				case 0x02:
				{
					gGuild.DGGuildMemberDisconnectRecv((SDHP_GUILD_MEMBER_DISCONNECT_RECV*)lpMsg);

					break;
				}

				case 0x03:
				{
					gGuild.DGGuildMemberJoinRecv((SDHP_GUILD_MEMBER_JOIN_RECV*)lpMsg);

					break;
				}

				case 0x04:
				{
					gGuild.DGGuildMemberDeleteRecv((SDHP_GUILD_MEMBER_DELETE_RECV*)lpMsg);

					break;
				}

				case 0x05:
				{
					gGuild.DGGuildMemberInfoRecv((SDHP_GUILD_MEMBER_INFO_RECV*)lpMsg);

					break;
				}

				case 0x06:
				{
					gGuild.DGGuildScoreUpdateRecv((SDHP_GUILD_SCORE_UPDATE_RECV*)lpMsg);

					break;
				}

				case 0x07:
				{
					gGuild.DGGuildGlobalChatRecv((SDHP_GUILD_GLOBAL_CHAT_RECV*)lpMsg);

					break;
				}

				case 0x08:
				{
					gGuild.DGGuildGlobalNoticeRecv((SDHP_GUILD_GLOBAL_NOTICE_RECV*)lpMsg);

					break;
				}

				case 0x09:
				{
					gGuild.DGGuildInfoRecv((SDHP_GUILD_INFO_RECV*)lpMsg);

					break;
				}
			}

			break;
		}
	}
}

void GDServerInfoSend()
{
	SDHP_DATA_SERVER_INFO_SEND pMsg;

	pMsg.header.set(0x00, sizeof(pMsg));

	pMsg.type = 1;

	pMsg.ServerPort = (WORD)gServerInfo.m_ServerPort;

	strcpy_s(pMsg.ServerName, gServerInfo.m_ServerName);

	pMsg.ServerCode = (WORD)gServerInfo.m_ServerCode;

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void GDCharacterListSend(int aIndex)
{
	if (gObjIsAccountValid(aIndex, gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_CHARACTER_LIST_SEND pMsg;

	pMsg.header.set(0x01, 0x00, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, gObj[aIndex].Account, sizeof(pMsg.account));

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void DGCharacterListRecv(SDHP_CHARACTER_LIST_RECV* lpMsg)
{
	if (gObjIsAccountValid(lpMsg->index, lpMsg->account) == 0)
	{
		LogAdd(LOG_RED, "[DGCharacterListRecv] Invalid Account [%d](%s)", lpMsg->index, lpMsg->account);

		CloseClient(lpMsg->index);

		return;
	}

	BYTE send[256];

	PMSG_CHARACTER_LIST_SEND pMsg;

	pMsg.header.setE(0xF3, 0x00, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_CHARACTER_LIST info;

	WORD CharacterCreationLevel = 0;

	for (int n = 0; n < lpMsg->count; n++)
	{
		SDHP_CHARACTER_LIST* lpInfo = (SDHP_CHARACTER_LIST*)(((BYTE*)lpMsg) + sizeof(SDHP_CHARACTER_LIST_RECV) + (sizeof(SDHP_CHARACTER_LIST) * n));

		info.slot = lpInfo->slot;

		memcpy(info.Name, lpInfo->name, sizeof(info.Name));

		info.Level = lpInfo->level;

		CharacterCreationLevel = ((info.Level > CharacterCreationLevel) ? info.Level : CharacterCreationLevel);

		info.CtlCode = lpInfo->CtlCode;

	#pragma region CHAR_SET_CALC

		memset(info.CharSet, 0, sizeof(info.CharSet));

		info.CharSet[0] = (lpInfo->Class % 16) * 16;
		info.CharSet[0] -= (info.CharSet[0] / 32);
		info.CharSet[0] += (lpInfo->Class / 16) * 32;

		WORD TempInventory[12];

		for (int i = 0; i < 9; i++)
		{
			if (i == 0 || i == 1)
			{
				if (lpInfo->Inventory[0 + (i * 5)] == 0xFF && (lpInfo->Inventory[2 + (i * 5)] & 0x80) == 0x80 && (lpInfo->Inventory[4 + (i * 5)] & 0xF0) == 0xF0)
				{
					TempInventory[i] = 0xFFFF;
				}
				else
				{
					TempInventory[i] = (lpInfo->Inventory[0 + (i * 5)] + ((lpInfo->Inventory[2 + (i * 5)] & 0x80) * 2) + ((lpInfo->Inventory[4 + (i * 5)] & 0xF0) * 32));
				}
			}
			else
			{
				if (lpInfo->Inventory[0 + (i * 5)] == 0xFF && (lpInfo->Inventory[2 + (i * 5)] & 0x80) == 0x80 && (lpInfo->Inventory[4 + (i * 5)] & 0xF0) == 0xF0)
				{
					TempInventory[i] = (MAX_ITEM_TYPE - 1);
				}
				else
				{
					TempInventory[i] = (lpInfo->Inventory[0 + (i * 5)] + ((lpInfo->Inventory[2 + (i * 5)] & 0x80) * 2) + ((lpInfo->Inventory[4 + (i * 5)] & 0xF0) * 32)) % MAX_ITEM_TYPE;
				}
			}
		}

		info.CharSet[1] = TempInventory[0] % 256;

		info.CharSet[2] = TempInventory[1] % 256;

		info.CharSet[3] |= (TempInventory[2] & 0x0F) << 4;
		info.CharSet[9] |= (TempInventory[2] & 0x10) << 3;

		info.CharSet[3] |= (TempInventory[3] & 0x0F);
		info.CharSet[9] |= (TempInventory[3] & 0x10) << 2;

		info.CharSet[4] |= (TempInventory[4] & 0x0F) << 4;
		info.CharSet[9] |= (TempInventory[4] & 0x10) << 1;

		info.CharSet[4] |= (TempInventory[5] & 0x0F);
		info.CharSet[9] |= (TempInventory[5] & 0x10);

		info.CharSet[5] |= (TempInventory[6] & 0x0F) << 4;
		info.CharSet[9] |= (TempInventory[6] & 0x10) >> 1;

		int level = 0;

		BYTE table[7] = { 1, 0, 6, 5, 4, 3, 2 };

		for (int i = 0; i < 7; i++)
		{
			if (TempInventory[i] != (MAX_ITEM_TYPE - 1) && TempInventory[i] != 0xFFFF)
			{
				level |= LevelSmallConvert(((lpInfo->Inventory[1 + (i * 5)] / 8) & 0x0F)) << (i * 3);

				info.CharSet[10] |= ((lpInfo->Inventory[2 + (i * 5)] & 0x3F) ? 2 : 0) << table[i];
			}
		}

		info.CharSet[6] = level >> 16;
		info.CharSet[7] = level >> 8;
		info.CharSet[8] = level;

		if (TempInventory[7] == (MAX_ITEM_TYPE - 1))
		{
			info.CharSet[5] |= 12;
		}
		else if (TempInventory[7] >= 0 && TempInventory[7] <= 2)
		{
			info.CharSet[5] |= TempInventory[7] << 2;
		}
		else if (TempInventory[7] >= 3 && TempInventory[7] <= 6)
		{
			info.CharSet[5] |= 12;
			info.CharSet[9] |= TempInventory[7] - 2;
		}

		if (TempInventory[8] == (MAX_ITEM_TYPE - 1))
		{
			info.CharSet[5] |= 3;
		}
		else if (TempInventory[8] >= 0 && TempInventory[8] <= 2)
		{
			info.CharSet[5] |= TempInventory[8];
		}
		else if (TempInventory[8] == 3)
		{
			info.CharSet[5] |= 3;
			info.CharSet[10] |= 1;
		}

	#pragma endregion

		memcpy(&send[size], &info, sizeof(info));

		size += sizeof(info);

		pMsg.count++;
	}

	gObj[lpMsg->index].ClassCode = 2;

	if (CharacterCreationLevel >= gServerInfo.m_MGCreateLevel[gObj[lpMsg->index].AccountLevel])
	{
		gObj[lpMsg->index].ClassCode = 3;
	}

	pMsg.header.size = size;

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(lpMsg->index, send, size);

	GCCharacterCreationEnableSend(lpMsg->index, gObj[lpMsg->index].ClassCode);
}

void GDCharacterCreateSend(int aIndex, char* name, BYTE Class)
{
	if (gObjIsAccountValid(aIndex, gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_CHARACTER_CREATE_SEND pMsg;

	pMsg.header.set(0x01, 0x01, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, gObj[aIndex].Account, sizeof(pMsg.account));

	memcpy(pMsg.name, name, sizeof(pMsg.name));

	pMsg.Class = Class;

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void DGCharacterCreateRecv(SDHP_CHARACTER_CREATE_RECV* lpMsg)
{
	if (gObjIsAccountValid(lpMsg->index, lpMsg->account) == 0)
	{
		LogAdd(LOG_RED, "[DGCharacterCreateRecv] Invalid Account [%d](%s)", lpMsg->index, lpMsg->account);

		CloseClient(lpMsg->index);

		return;
	}

	PMSG_CHARACTER_CREATE_SEND pMsg;

	pMsg.header.set(0xF3, 0x01, sizeof(pMsg));

	pMsg.result = lpMsg->result;

	memcpy(pMsg.name, lpMsg->name, sizeof(pMsg.name));

	pMsg.slot = lpMsg->slot;

	memcpy(pMsg.equipment, lpMsg->equipment, sizeof(pMsg.equipment));

	DataSend(lpMsg->index, (BYTE*)&pMsg, pMsg.header.size);
}

void GDCharacterDeleteSend(int aIndex, char* name)
{
	if (gObjIsAccountValid(aIndex, gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_CHARACTER_DELETE_SEND pMsg;

	pMsg.header.set(0x01, 0x02, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, gObj[aIndex].Account, sizeof(pMsg.account));

	memcpy(pMsg.name, name, sizeof(pMsg.name));

	pMsg.guild = 0;

	if (gObj[aIndex].Guild != 0)
	{
		if (strcmp(gObj[aIndex].Name, gObj[aIndex].Guild->Members[0].Name) == 0)
		{
			pMsg.guild = 1; // Guild Master
		}
		else
		{
			pMsg.guild = 2; // Guild Member
		}

		memcpy(pMsg.GuildName, gObj[aIndex].Guild->Name, sizeof(pMsg.GuildName));
	}

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void DGCharacterDeleteRecv(SDHP_CHARACTER_DELETE_RECV* lpMsg)
{
	if (gObjIsAccountValid(lpMsg->index, lpMsg->account) == 0)
	{
		LogAdd(LOG_RED, "[DGCharacterDeleteRecv] Invalid Account [%d](%s)", lpMsg->index, lpMsg->account);

		CloseClient(lpMsg->index);

		return;
	}

	PMSG_CHARACTER_DELETE_SEND pMsg;

	pMsg.header.set(0xF3, 0x02, sizeof(pMsg));

	pMsg.result = lpMsg->result;

	DataSend(lpMsg->index, (BYTE*)&pMsg, pMsg.header.size);
}

void GDCharacterInfoSend(int aIndex, char* name)
{
	if (gObjIsAccountValid(aIndex, gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_CHARACTER_INFO_SEND pMsg;

	pMsg.header.set(0x01, 0x03, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, gObj[aIndex].Account, sizeof(pMsg.account));

	memcpy(pMsg.name, name, sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void DGCharacterInfoRecv(SDHP_CHARACTER_INFO_RECV* lpMsg)
{
	if (gObjIsAccountValid(lpMsg->index, lpMsg->account) == 0)
	{
		LogAdd(LOG_RED, "[DGCharacterInfoRecv] Invalid Account [%d](%s)", lpMsg->index, lpMsg->account);

		CloseClient(lpMsg->index);

		return;
	}

	if (lpMsg->result == 0)
	{
		CloseClient(lpMsg->index);

		return;
	}

	if ((lpMsg->CtlCode & 1) != 0)
	{
		CloseClient(lpMsg->index);

		return;
	}

	if (gObjectManager.CharacterInfoSet((BYTE*)lpMsg, lpMsg->index) == 0)
	{
		CloseClient(lpMsg->index);

		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	PMSG_CHARACTER_INFO_SEND pMsg;

	pMsg.header.setE(0xF3, 0x03, sizeof(pMsg));

	pMsg.X = (BYTE)lpObj->X;

	pMsg.Y = (BYTE)lpObj->Y;

	pMsg.Map = lpObj->Map;

	pMsg.Dir = lpObj->Dir;

	pMsg.Experience = lpObj->Experience;

	pMsg.NextExperience = lpObj->NextExperience;

	pMsg.LevelUpPoint = lpObj->LevelUpPoint;

	pMsg.Strength = GET_MAX_WORD_VALUE(lpObj->Strength);

	pMsg.Dexterity = GET_MAX_WORD_VALUE(lpObj->Dexterity);

	pMsg.Vitality = GET_MAX_WORD_VALUE(lpObj->Vitality);

	pMsg.Energy = GET_MAX_WORD_VALUE(lpObj->Energy);

	pMsg.Life = GET_MAX_WORD_VALUE(lpObj->Life);

	pMsg.MaxLife = GET_MAX_WORD_VALUE((lpObj->MaxLife + lpObj->AddLife));

	pMsg.Mana = GET_MAX_WORD_VALUE(lpObj->Mana);

	pMsg.MaxMana = GET_MAX_WORD_VALUE((lpObj->MaxMana + lpObj->AddMana));

	pMsg.BP = GET_MAX_WORD_VALUE(lpObj->BP);

	pMsg.MaxBP = GET_MAX_WORD_VALUE((lpObj->MaxBP + lpObj->AddBP));

	pMsg.Money = lpObj->Money;

	pMsg.PKLevel = lpObj->PKLevel;

	pMsg.CtlCode = lpMsg->CtlCode;

	pMsg.FruitAddPoint = lpObj->FruitAddPoint;

	pMsg.MaxFruitAddPoint = gFruit.GetMaxFruitPoint(lpObj);

	DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);

	GDConnectCharacterSend(lpObj->Index);

	GDOptionDataSend(lpObj->Index);

	GCNewCharacterInfoSend(lpObj);

	gMove.GCMoveListSend(lpObj);

	gItemStack.GCItemStackListSend(lpObj->Index);

	gItemValue.GCItemValueListSend(lpObj->Index);

	gGuild.GDGuildMemberGetInfoSend(lpObj->Index, lpObj->Name);

	gItemManager.GCItemListSend(lpObj->Index);

	gSkillManager.GCSkillListSend(lpObj, 0);

	gQuest.GCQuestInfoSend(lpObj->Index);

	gReconnect.GetReconnectInfo(lpObj);

	gSkillManager.SkillChangeUse(lpObj->Index);

	gObjViewportListProtocolCreate(lpObj);

	gObjectManager.CharacterUpdateMapEffect(lpObj);

	gFlyingDragons.FlyingDragonsCheck(lpObj->Map, lpObj->Index);

	gNotice.GCNoticeSend(lpObj->Index, 0, gMessage.GetTextMessage(41, lpObj->Lang), lpObj->Name);

	gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage((36 + lpObj->AccountLevel), lpObj->Lang), lpObj->AccountExpireDate);

	gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(40, lpObj->Lang), lpObj->Reset, lpObj->GrandReset);

	//LogAdd(LOG_USER, "[ObjectManager][%d] AddCharacterInfo (%s)", lpObj->Index, lpObj->Name);

	gLog.Output(LOG_CONNECT, "[ObjectManager][%d] AddCharacterInfo (%s)", lpObj->Index, lpObj->Name);
}

void GDCharacterInfoSaveSend(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_CHARACTER_INFO_SAVE_SEND pMsg;

	pMsg.header.set(0x01, 0x04, sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account, lpObj->Account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));

	pMsg.Level = lpObj->Level;

	pMsg.Class = lpObj->DBClass;

	pMsg.LevelUpPoint = lpObj->LevelUpPoint;

	pMsg.Experience = lpObj->Experience;

	pMsg.Money = lpObj->Money;

	pMsg.Strength = lpObj->Strength;

	pMsg.Dexterity = lpObj->Dexterity;

	pMsg.Vitality = lpObj->Vitality;

	pMsg.Energy = lpObj->Energy;

	pMsg.Life = (DWORD)lpObj->Life;

	pMsg.MaxLife = (DWORD)lpObj->MaxLife;

	pMsg.Mana = (DWORD)lpObj->Mana;

	pMsg.MaxMana = (DWORD)lpObj->MaxMana;

	pMsg.BP = (DWORD)lpObj->BP;

	pMsg.MaxBP = (DWORD)lpObj->MaxBP;

	for (int n = 0; n < INVENTORY_SIZE; n++)
	{
		gItemManager.DBItemByteConvert(pMsg.Inventory[n], &lpObj->Inventory[n]);
	}

	for (int n = 0; n < MAX_SKILL_LIST; n++)
	{
		gSkillManager.SkillByteConvert(pMsg.Skill[n], &lpObj->Skill[n]);
	}

	pMsg.Map = lpObj->Map;

	pMsg.X = (BYTE)lpObj->X;

	pMsg.Y = (BYTE)lpObj->Y;

	pMsg.Dir = lpObj->Dir;

	pMsg.PKCount = lpObj->PKCount;

	pMsg.PKLevel = lpObj->PKLevel;

	pMsg.PKTime = lpObj->PKTime;

	memcpy(pMsg.Quest, lpObj->Quest, sizeof(pMsg.Quest));

	pMsg.FruitAddPoint = lpObj->FruitAddPoint;

	pMsg.FruitSubPoint = lpObj->FruitSubPoint;

	for (int n = 0; n < MAX_EFFECT_LIST; n++)
	{
		gEffectManager.EffectByteConvert(pMsg.Effect[n], &lpObj->Effect[n]);
	}

	gDataServerConnection.DataSend((BYTE*)&pMsg, sizeof(pMsg));

	gWarehouse.GDWarehouseItemSaveSend(aIndex);
}

void GDConnectCharacterSend(int aIndex)
{
	SDHP_CONNECT_CHARACTER_SEND pMsg;

	pMsg.header.set(0x01, 0x05, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, gObj[aIndex].Account, sizeof(pMsg.account));

	memcpy(pMsg.name, gObj[aIndex].Name, sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg, sizeof(pMsg));
}

void GDDisconnectCharacterSend(int aIndex)
{
	SDHP_CONNECT_CHARACTER_SEND pMsg;

	pMsg.header.set(0x01, 0x06, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, gObj[aIndex].Account, sizeof(pMsg.account));

	memcpy(pMsg.name, gObj[aIndex].Name, sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg, sizeof(pMsg));
}

void GDCreateItemSend(int aIndex, BYTE map, BYTE x, BYTE y, int index, BYTE level, BYTE dur, BYTE SkillOption, BYTE LuckOption, BYTE AddOption, int LootIndex, BYTE ExceOption)
{
	if (gObjIsAccountValid(aIndex, gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_CREATE_ITEM_SEND pMsg;

	pMsg.header.set(0x02, 0x00, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, gObj[aIndex].Account, sizeof(pMsg.account));

	pMsg.X = x;

	pMsg.Y = y;

	pMsg.Map = map;

	pMsg.ItemIndex = index;

	pMsg.Level = level;

	pMsg.Dur = ((dur == 0) ? gItemManager.GetItemDurability(index, level, ExceOption) : dur);

	pMsg.SkillOption = SkillOption;

	pMsg.LuckOption = LuckOption;

	pMsg.AddOption = AddOption;

	pMsg.ExceOption = ExceOption;

	pMsg.LootIndex = LootIndex;

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void DGCreateItemRecv(SDHP_CREATE_ITEM_RECV* lpMsg)
{
	if (gObjIsAccountValid(lpMsg->index, lpMsg->account) == 0)
	{
		LogAdd(LOG_RED, "[DGCreateItemRecv] Invalid Account [%d](%s)", lpMsg->index, lpMsg->account);

		CloseClient(lpMsg->index);

		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	if (lpMsg->Map != 0xFF && lpMsg->Map != 0xFE && lpMsg->Map != 0xED && lpMsg->Map != 0xEC && lpMsg->Map != 0xEB && MAP_RANGE(lpMsg->Map) == 0)
	{
		return;
	}

	if (lpMsg->Map == 0xFE || lpMsg->Map == 0xFF)
	{
		if (lpMsg->Map == 0xFF && lpObj->Interface.type != INTERFACE_CHAOS_BOX)
		{
			return;
		}

		CItem item;

		item.m_Level = lpMsg->Level;

		item.m_Serial = lpMsg->Serial;

		if (lpMsg->ItemIndex == GET_ITEM(13, 18) || lpMsg->ItemIndex == GET_ITEM(14, 19))
		{
			item.m_Durability = (float)lpMsg->Dur;
		}
		else
		{
			item.m_Durability = (float)gItemManager.GetItemDurability(lpMsg->ItemIndex, lpMsg->Level, lpMsg->ExceOption);
		}

		item.Convert(lpMsg->ItemIndex, lpMsg->SkillOption, lpMsg->LuckOption, lpMsg->AddOption, lpMsg->ExceOption);

		gChaosBox.ChaosBoxInit(lpObj);

		gItemManager.ChaosBoxAddItem(lpObj->Index, item, 0);

		gChaosBox.GCChaosMixSend(lpObj->Index, ((lpMsg->Map == 0xFF) ? 1 : 100), &item);
	}
	else if (lpMsg->Map == 0xEB)
	{
		CItem item;

		item.m_Level = lpMsg->Level;

		item.m_Serial = lpMsg->Serial;

		item.m_Durability = lpMsg->Dur;

		item.Convert(lpMsg->ItemIndex, lpMsg->SkillOption, lpMsg->LuckOption, lpMsg->AddOption, lpMsg->ExceOption);

		BYTE slot = gItemManager.InventoryInsertItem(lpObj->Index, item);

		if (slot != 0xFF)
		{
			gItemManager.GCItemModifySend(lpObj->Index, slot);
		}
	}
	else
	{
		if (gMap[lpMsg->Map].MonsterItemDrop(lpMsg->ItemIndex, lpMsg->Level, lpMsg->Dur, lpMsg->X, lpMsg->Y, lpMsg->SkillOption, lpMsg->LuckOption, lpMsg->AddOption, lpMsg->ExceOption, lpMsg->LootIndex, lpMsg->Serial) != 0)
		{
			if (BC_MAP_RANGE(lpMsg->Map) != 0)
			{
				gBloodCastle.SetEventItemSerial(lpMsg->Map, lpMsg->ItemIndex, lpMsg->Serial);
			}
		}
	}
}

void GDInventoryItemSaveSend(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_INVENTORY_ITEM_SAVE_SEND pMsg;

	pMsg.header.set(0x02, 0x01, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, lpObj->Account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));

	for (int n = 0; n < INVENTORY_SIZE; n++)
	{
		gItemManager.DBItemByteConvert(pMsg.Inventory[n], &lpObj->Inventory[n]);
	}

	gDataServerConnection.DataSend((BYTE*)&pMsg, sizeof(pMsg));
}

void GDOptionDataSend(int aIndex)
{
	if (gObjIsAccountValid(aIndex, gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_OPTION_DATA_SEND pMsg;

	pMsg.header.set(0x03, 0x00, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, gObj[aIndex].Account, sizeof(pMsg.account));

	memcpy(pMsg.name, gObj[aIndex].Name, sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void DGOptionDataRecv(SDHP_OPTION_DATA_RECV* lpMsg)
{
	if (gObjIsAccountValid(lpMsg->index, lpMsg->account) == 0)
	{
		LogAdd(LOG_RED, "[DGOptionDataRecv] Invalid Account [%d](%s)", lpMsg->index, lpMsg->account);

		CloseClient(lpMsg->index);

		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	PMSG_OPTION_DATA_SEND pMsg;

	pMsg.header.set(0xF3, 0x30, sizeof(pMsg));

	memcpy(pMsg.SkillKey, lpMsg->SkillKey, sizeof(pMsg.SkillKey));

	pMsg.GameOption = lpMsg->GameOption;

	pMsg.QKey = lpMsg->QKey;

	pMsg.WKey = lpMsg->WKey;

	pMsg.EKey = lpMsg->EKey;

	pMsg.ChatWindow = lpMsg->ChatWindow;

	DataSend(lpMsg->index, (BYTE*)&pMsg, pMsg.header.size);
}

void GDOptionDataSaveSend(int aIndex, BYTE* SkillKey, BYTE GameOption, BYTE QKey, BYTE WKey, BYTE EKey, BYTE ChatWindow)
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_OPTION_DATA_SAVE_SEND pMsg;

	pMsg.header.set(0x03, 0x01, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, lpObj->Account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));

	memcpy(pMsg.SkillKey, SkillKey, sizeof(pMsg.SkillKey));

	pMsg.GameOption = GameOption;

	pMsg.QKey = QKey;

	pMsg.WKey = WKey;

	pMsg.EKey = EKey;

	pMsg.ChatWindow = ChatWindow;

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void GDResetInfoSaveSend(int aIndex, int ResetDay, int ResetWek, int ResetMon)
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_RESET_INFO_SAVE_SEND pMsg;

	pMsg.header.set(0x03, 0x02, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, lpObj->Account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));

	pMsg.Reset = lpObj->Reset;

	pMsg.ResetDay = ResetDay;

	pMsg.ResetWek = ResetWek;

	pMsg.ResetMon = ResetMon;

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void GDGrandResetInfoSaveSend(int aIndex, int GrandResetDay, int GrandResetWek, int GrandResetMon)
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_GRAND_RESET_INFO_SAVE_SEND pMsg;

	pMsg.header.set(0x03, 0x03, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, lpObj->Account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));

	pMsg.Reset = lpObj->Reset;

	pMsg.GrandReset = lpObj->GrandReset;

	pMsg.GrandResetDay = GrandResetDay;

	pMsg.GrandResetWek = GrandResetWek;

	pMsg.GrandResetMon = GrandResetMon;

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void GDGlobalNoticeSend(BYTE type, char* message)
{
	SDHP_GLOBAL_NOTICE_SEND pMsg;

	pMsg.header.set(0x05, 0x00, sizeof(pMsg));

	pMsg.type = type;

	memcpy(pMsg.message, message, sizeof(pMsg.message));

	gDataServerConnection.DataSend((BYTE*)&pMsg, sizeof(pMsg));
}

void DGGlobalNoticeRecv(SDHP_GLOBAL_NOTICE_RECV* lpMsg)
{
	gNotice.GCNoticeSendToAll(lpMsg->type, "%s", lpMsg->message);
}

void GDGlobalWhisperSend(int aIndex, char* TargetName, char* message)
{
	SDHP_GLOBAL_WHISPER_SEND pMsg;

	pMsg.header.set(0x05, 0x01, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, gObj[aIndex].Account, sizeof(pMsg.account));

	memcpy(pMsg.name, gObj[aIndex].Name, sizeof(pMsg.name));

	memcpy(pMsg.TargetName, TargetName, sizeof(pMsg.TargetName));

	memcpy(pMsg.message, message, sizeof(pMsg.message));

	gDataServerConnection.DataSend((BYTE*)&pMsg, sizeof(pMsg));
}

void DGGlobalWhisperRecv(SDHP_GLOBAL_WHISPER_RECV* lpMsg)
{
	if (gObjIsAccountValid(lpMsg->index, lpMsg->account) == 0)
	{
		LogAdd(LOG_RED, "[DGGlobalWhisperRecv] Invalid Account [%d](%s)", lpMsg->index, lpMsg->account);

		CloseClient(lpMsg->index);

		return;
	}

	if (lpMsg->result == 0)
	{
		GCServerMsgSend(lpMsg->index, 0);

		return;
	}

	lpMsg->message[(sizeof(lpMsg->message) - 1)] = ((strlen(lpMsg->message) > (sizeof(lpMsg->message) - 1)) ? 0 : lpMsg->message[(sizeof(lpMsg->message) - 1)]);

	gLog.Output(LOG_CHAT, "[Whisper][%s][%s] - (Name: %s, Message: %s)", lpMsg->account, lpMsg->name, lpMsg->TargetName, lpMsg->message);
}

void DGGlobalWhisperEchoRecv(SDHP_GLOBAL_WHISPER_ECHO_RECV* lpMsg)
{
	if (gObjIsAccountValid(lpMsg->index, lpMsg->account) == 0)
	{
		LogAdd(LOG_RED, "[DGGlobalWhisperEchoRecv] Invalid Account [%d](%s)", lpMsg->index, lpMsg->account);

		CloseClient(lpMsg->index);

		return;
	}

	lpMsg->message[(sizeof(lpMsg->message) - 1)] = ((strlen(lpMsg->message) > (sizeof(lpMsg->message) - 1)) ? 0 : lpMsg->message[(sizeof(lpMsg->message) - 1)]);

	gFilter.CheckSyntax(lpMsg->message);

	GCChatWhisperSend(lpMsg->index, lpMsg->SourceName, lpMsg->message);
}

void GDRankingBloodCastleSaveSend(int aIndex, DWORD Score)
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_RANKING_SAVE_SEND pMsg;

	pMsg.header.set(0x06, 0x00, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, lpObj->Account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));

	pMsg.score = Score;

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void GDRankingDevilSquareSaveSend(int aIndex, DWORD Score)
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_RANKING_SAVE_SEND pMsg;

	pMsg.header.set(0x06, 0x01, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, lpObj->Account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));

	pMsg.score = Score;

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}