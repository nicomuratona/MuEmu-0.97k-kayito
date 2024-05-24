#include "stdafx.h"
#include "DataServerProtocol.h"
#include "BadSyntax.h"
#include "CharacterManager.h"
#include "CommandManager.h"
#include "Guild.h"
#include "GuildManager.h"
#include "NpcTalk.h"
#include "QueryManager.h"
#include "ServerManager.h"
#include "SocketManager.h"
#include "Util.h"
#include "Warehouse.h"

void DataServerProtocolCore(int index, BYTE head, BYTE* lpMsg, int size)
{
	ConsoleProtocolLog(CON_PROTO_TCP_RECV, lpMsg, size);

	gServerManager[index].m_PacketTime = GetTickCount();

	switch (head)
	{
		case 0x00:
		{
			GDServerInfoRecv((SDHP_SERVER_INFO_RECV*)lpMsg, index);

			break;
		}

		case 0x01:
		{
			switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
			{
				case 0x00:
				{
					GDCharacterListRecv((SDHP_CHARACTER_LIST_RECV*)lpMsg, index);

					break;
				}

				case 0x01:
				{
					GDCharacterCreateRecv((SDHP_CHARACTER_CREATE_RECV*)lpMsg, index);

					break;
				}

				case 0x02:
				{
					GDCharacterDeleteRecv((SDHP_CHARACTER_DELETE_RECV*)lpMsg, index);

					break;
				}

				case 0x03:
				{
					GDCharacterInfoRecv((SDHP_CHARACTER_INFO_RECV*)lpMsg, index);

					break;
				}

				case 0x04:
				{
					GDCharacterInfoSaveRecv((SDHP_CHARACTER_INFO_SAVE_RECV*)lpMsg);

					break;
				}

				case 0x05:
				{
					GDConnectCharacterRecv((SDHP_CONNECT_CHARACTER_RECV*)lpMsg, index);

					break;
				}

				case 0x06:
				{
					GDDisconnectCharacterRecv((SDHP_CONNECT_CHARACTER_RECV*)lpMsg, index);

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
					GDCreateItemRecv((SDHP_CREATE_ITEM_RECV*)lpMsg, index);

					break;
				}

				case 0x01:
				{
					GDInventoryItemSaveRecv((SDHP_INVENTORY_ITEM_SAVE_RECV*)lpMsg);

					break;
				}

				case 0x02:
				{
					gWarehouse.GDWarehouseItemRecv((SDHP_WAREHOUSE_ITEM_RECV*)lpMsg, index);

					break;
				}

				case 0x04:
				{
					gWarehouse.GDWarehouseItemSaveRecv((SDHP_WAREHOUSE_ITEM_SAVE_RECV*)lpMsg);

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
					GDOptionDataRecv((SDHP_OPTION_DATA_RECV*)lpMsg, index);

					break;
				}

				case 0x01:
				{
					GDOptionDataSaveRecv((SDHP_OPTION_DATA_SAVE_RECV*)lpMsg);

					break;
				}

				case 0x02:
				{
					GDResetInfoSaveRecv((SDHP_RESET_INFO_SAVE_RECV*)lpMsg);

					break;
				}

				case 0x03:
				{
					GDGrandResetInfoSaveRecv((SDHP_GRAND_RESET_INFO_SAVE_RECV*)lpMsg);

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
					gNpcTalk.GDNpcGoldenArcherOpenRecv((SDHP_NPC_GOLDEN_ARCHER_OPEN_RECV*)lpMsg, index);

					break;
				}

				case 0x01:
				{
					gNpcTalk.GDNpcGoldenArcherRegisterCountRecv((SDHP_NPC_GOLDEN_ARCHER_REG_COUNT_RECV*)lpMsg, index);

					break;
				}

				case 0x02:
				{
					gNpcTalk.GDNpcGoldenArcherRegisterLuckyNumRecv((SDHP_NPC_GOLDEN_ARCHER_REG_LUCKYNUM_RECV*)lpMsg, index);

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
					GDGlobalNoticeRecv((SDHP_GLOBAL_NOTICE_RECV*)lpMsg, index);

					break;
				}

				case 0x01:
				{
					GDGlobalWhisperRecv((SDHP_GLOBAL_WHISPER_RECV*)lpMsg, index);

					break;
				}

				case 0x03:
				{
					gCommandManager.GDGlobalPostRecv((SDHP_GLOBAL_POST_RECV*)lpMsg, index);

					break;
				}

				case 0x04:
				{
					gCommandManager.GDCommandResetRecv((SDHP_COMMAND_RESET_RECV*)lpMsg, index);

					break;
				}

				case 0x05:
				{
					gCommandManager.GDCommandGrandResetRecv((SDHP_COMMAND_RESET_RECV*)lpMsg, index);

					break;
				}
			}

			break;
		}

		case 0x06:
		{
			switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
			{
				case 0x00:
				{
					GDRankingBloodCastleSaveRecv((SDHP_RANKING_SAVE_RECV*)lpMsg);

					break;
				}

				case 0x01:
				{
					GDRankingDevilSquareSaveRecv((SDHP_RANKING_SAVE_RECV*)lpMsg);

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
					gGuild.GDGuildCreateRecv((SDHP_GUILD_CREATE_RECV*)lpMsg, index);

					break;
				}

				case 0x01:
				{
					gGuild.GDGuildDeleteRecv((SDHP_GUILD_DELETE_RECV*)lpMsg, index);

					break;
				}

				case 0x02:
				{
					gGuild.GDGuildMemberDisconnectRecv((SDHP_GUILD_MEMBER_DISCONNECT_RECV*)lpMsg);

					break;
				}

				case 0x03:
				{
					gGuild.GDGuildMemberJoinRecv((SDHP_GUILD_MEMBER_JOIN_RECV*)lpMsg, index);

					break;
				}

				case 0x04:
				{
					gGuild.GDGuildMemberDeleteRecv((SDHP_GUILD_MEMBER_DELETE_RECV*)lpMsg, index);

					break;
				}

				case 0x05:
				{
					gGuild.GDGuildMemberGetInfoRecv((SDHP_GUILD_MEMBER_GET_INFO_RECV*)lpMsg, index);

					break;
				}

				case 0x06:
				{
					gGuild.GDGuildScoreUpdateRecv((SDHP_GUILD_SCORE_UPDATE_RECV*)lpMsg);

					break;
				}

				case 0x07:
				{
					gGuild.GDGuildGlobalChatRecv((SDHP_GUILD_GLOBAL_CHAT_RECV*)lpMsg);

					break;
				}

				case 0x08:
				{
					gGuild.GDGuildGlobalNoticeRecv((SDHP_GUILD_GLOBAL_NOTICE_RECV*)lpMsg);

					break;
				}
			}

			break;
		}
	}
}

void GDServerInfoRecv(SDHP_SERVER_INFO_RECV* lpMsg, int index)
{
#ifndef MYSQL
	if (gQueryManager.ExecQuery("SELECT ItemCount FROM GameServerInfo WHERE Number = 0") == false || gQueryManager.Fetch() == SQL_NO_DATA)
#else
	if (gQueryManager.ExecResultQuery("SELECT ItemCount FROM GameServerInfo WHERE Number=0") == false || gQueryManager.Fetch() == false)
#endif
	{
		gQueryManager.Close();

	#ifndef MYSQL

		gQueryManager.ExecQuery("INSERT INTO GameServerInfo (Number, ItemCount) VALUES (0, 0)");
	
	#else

		gQueryManager.ExecUpdateQuery("INSERT INTO GameServerInfo (Number, ItemCount) VALUES (0, 0)");
	
	#endif

		gQueryManager.Close();
	}
	else
	{
		gQueryManager.Close();
	}

	gServerManager[index].SetServerInfo(lpMsg->ServerName, lpMsg->ServerPort, lpMsg->ServerCode);
}

void GDCharacterListRecv(SDHP_CHARACTER_LIST_RECV* lpMsg, int index)
{
	BYTE send[2048];

	SDHP_CHARACTER_LIST_SEND pMsg;

	pMsg.header.set(0x01, 0x00, 0);

	int size = sizeof(pMsg);

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account, lpMsg->account, sizeof(pMsg.account));

#ifndef MYSQL
	if (gQueryManager.ExecQuery("SELECT Id FROM AccountCharacter WHERE Id='%s'", lpMsg->account) == false || gQueryManager.Fetch() == SQL_NO_DATA)
#else
	if (gQueryManager.ExecResultQuery("SELECT Id FROM AccountCharacter WHERE Id='%s'", lpMsg->account) == false || gQueryManager.Fetch() == false)
#endif
	{
		gQueryManager.Close();

	#ifndef MYSQL

		gQueryManager.ExecQuery("INSERT INTO AccountCharacter (Id) VALUES ('%s')", lpMsg->account);

	#else
	
		gQueryManager.ExecUpdateQuery("INSERT INTO AccountCharacter (Id) VALUES ('%s')", lpMsg->account);

	#endif

		gQueryManager.Close();
	}
	else
	{
		gQueryManager.Close();
	}

	char CharacterName[5][11];

	memset(CharacterName, 0, sizeof(CharacterName));

#ifndef MYSQL

	gQueryManager.ExecQuery("SELECT * FROM AccountCharacter WHERE Id='%s'", lpMsg->account);

#else

	gQueryManager.ExecResultQuery("SELECT * FROM AccountCharacter WHERE Id='%s'", lpMsg->account);

#endif

	gQueryManager.Fetch();

	gQueryManager.GetAsString("GameID1", CharacterName[0], sizeof(CharacterName[0]));

	gQueryManager.GetAsString("GameID2", CharacterName[1], sizeof(CharacterName[1]));

	gQueryManager.GetAsString("GameID3", CharacterName[2], sizeof(CharacterName[2]));

	gQueryManager.GetAsString("GameID4", CharacterName[3], sizeof(CharacterName[3]));

	gQueryManager.GetAsString("GameID5", CharacterName[4], sizeof(CharacterName[4]));

	gQueryManager.Close();

	pMsg.count = 0;

	SDHP_CHARACTER_LIST info;

	for (int n = 0; n < 5; n++)
	{
		if (CharacterName[n][0] == 0)
		{
			continue;
		}

	#ifndef MYSQL
		if (gQueryManager.ExecQuery("SELECT cLevel,Class,Inventory,CtlCode FROM Character WHERE AccountID='%s' AND Name='%s'", lpMsg->account, CharacterName[n]) == false || gQueryManager.Fetch() == SQL_NO_DATA)
	#else
		if (gQueryManager.ExecResultQuery("SELECT cLevel, Class, Inventory, CtlCode FROM `Character` WHERE AccountID='%s' AND Name='%s'", lpMsg->account, CharacterName[n]) == false || gQueryManager.Fetch() == false)
	#endif
		{
			gQueryManager.Close();
		}
		else
		{
			info.slot = n;

			memcpy(info.name, CharacterName[n], sizeof(info.name));

			info.level = (WORD)gQueryManager.GetAsInteger("cLevel");

			info.Class = (BYTE)gQueryManager.GetAsInteger("Class");

			BYTE Inventory[12][ITEM_BYTE_SIZE];

			gQueryManager.GetAsBinary("Inventory", Inventory[0], sizeof(Inventory));

			info.CtlCode = (BYTE)gQueryManager.GetAsInteger("CtlCode");

			gQueryManager.Close();

			memset(info.Inventory, 0xFF, sizeof(info.Inventory));

			for (int i = 0; i < 12; i++)
			{
				if (Inventory[i][0] == 0xFF && (Inventory[i][7] & 0x80) == 0x80 && (Inventory[i][9] & 0xF0) == 0xF0)
				{
					info.Inventory[0 + (i * 5)] = 0xFF;

					info.Inventory[1 + (i * 5)] = 0xFF;

					info.Inventory[2 + (i * 5)] = 0xFF;

					info.Inventory[3 + (i * 5)] = 0xFF;

					info.Inventory[4 + (i * 5)] = 0xFF;
				}
				else
				{
					info.Inventory[0 + (i * 5)] = Inventory[i][0];

					info.Inventory[1 + (i * 5)] = Inventory[i][1];

					info.Inventory[2 + (i * 5)] = Inventory[i][7];

					info.Inventory[3 + (i * 5)] = Inventory[i][8];

					info.Inventory[4 + (i * 5)] = Inventory[i][9];
				}
			}

			memcpy(&send[size], &info, sizeof(info));

			size += sizeof(info);

			pMsg.count++;
		}
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	gSocketManager.DataSend(index, send, size);
}

void GDCharacterCreateRecv(SDHP_CHARACTER_CREATE_RECV* lpMsg, int index)
{
	SDHP_CHARACTER_CREATE_SEND pMsg;

	pMsg.header.set(0x01, 0x01, sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account, lpMsg->account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpMsg->name, sizeof(pMsg.name));

	if (CheckTextSyntax(lpMsg->name, sizeof(lpMsg->name)) == false || gBadSyntax.CheckSyntax(lpMsg->name) == false || CheckSpecialText(lpMsg->name) == false)
	{
		pMsg.result = 0;
	}
	else
	{
		pMsg.result = 1;
	}

	pMsg.slot = 0;

	memset(pMsg.equipment, 0xFF, sizeof(pMsg.equipment));

	char CharacterName[5][11] = { 0 };

#ifndef MYSQL
	if (pMsg.result == 0 || gQueryManager.ExecQuery("SELECT * FROM AccountCharacter WHERE Id='%s'", lpMsg->account) == false || gQueryManager.Fetch() == SQL_NO_DATA)
#else
	if (pMsg.result == 0 || gQueryManager.ExecResultQuery("SELECT * FROM AccountCharacter WHERE Id='%s'", lpMsg->account) == false || gQueryManager.Fetch() == false)
#endif
	{
		gQueryManager.Close();

		pMsg.result = 0;
	}
	else
	{
		gQueryManager.GetAsString("GameID1", CharacterName[0], sizeof(CharacterName[0]));

		gQueryManager.GetAsString("GameID2", CharacterName[1], sizeof(CharacterName[1]));

		gQueryManager.GetAsString("GameID3", CharacterName[2], sizeof(CharacterName[2]));

		gQueryManager.GetAsString("GameID4", CharacterName[3], sizeof(CharacterName[3]));

		gQueryManager.GetAsString("GameID5", CharacterName[4], sizeof(CharacterName[4]));

		gQueryManager.Close();

		if (GetCharacterSlot(CharacterName, "", &pMsg.slot) == false)
		{
			pMsg.result = 2;
		}
		else
		{
		#ifndef MYSQL
			if (gQueryManager.ExecQuery("EXEC WZ_CreateCharacter '%s','%s','%d'", lpMsg->account, lpMsg->name, lpMsg->Class) == false || gQueryManager.Fetch() == SQL_NO_DATA)
		#else
			if (gQueryManager.ExecResultQuery("CALL WZ_CreateCharacter('%s', '%s', '%d')", lpMsg->account, lpMsg->name, lpMsg->Class) == false || gQueryManager.Fetch() == false)
		#endif
			{
				gQueryManager.Close();

				pMsg.result = 0;
			}
			else
			{
			#ifndef MYSQL

				pMsg.result = gQueryManager.GetResult(0);

			#else

				pMsg.result = gQueryManager.GetAsInteger("Result");

			#endif

				gQueryManager.Close();
			}

			if (pMsg.result == 1)
			{
			#ifndef MYSQL

				gQueryManager.ExecQuery("UPDATE AccountCharacter SET GameID%d='%s' WHERE Id='%s'", (pMsg.slot + 1), lpMsg->name, lpMsg->account);

			#else

				gQueryManager.ExecUpdateQuery("UPDATE AccountCharacter SET GameID%d='%s' WHERE Id='%s'", (pMsg.slot + 1), lpMsg->name, lpMsg->account);

			#endif

				gQueryManager.Close();
			}
		}
	}

	gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);
}

void GDCharacterDeleteRecv(SDHP_CHARACTER_DELETE_RECV* lpMsg, int index)
{
	SDHP_CHARACTER_DELETE_SEND pMsg;

	pMsg.header.set(0x01, 0x02, sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account, lpMsg->account, sizeof(pMsg.account));

	if (CheckTextSyntax(lpMsg->name, sizeof(lpMsg->name)) == false)
	{
		pMsg.result = 0;
	}
	else
	{
		pMsg.result = 1;
	}

#ifndef MYSQL
	if (pMsg.result == 0 || gQueryManager.ExecQuery("EXEC WZ_DeleteCharacter '%s','%s'", lpMsg->account, lpMsg->name) == false || gQueryManager.Fetch() == SQL_NO_DATA)
#else
	if (pMsg.result == 0 || gQueryManager.ExecResultQuery("CALL WZ_DeleteCharacter('%s', '%s')", lpMsg->account, lpMsg->name) == false || gQueryManager.Fetch() == false)
#endif
	{
		gQueryManager.Close();

		pMsg.result = 0;
	}
	else
	{
	#ifndef MYSQL

		pMsg.result = gQueryManager.GetResult(0);

	#else

		pMsg.result = gQueryManager.GetAsInteger("Result");

	#endif

		gQueryManager.Close();

		if (pMsg.result == 1)
		{
			char CharacterName[5][11] = { 0 };

		#ifndef MYSQL
			if (gQueryManager.ExecQuery("SELECT * FROM AccountCharacter WHERE Id='%s'", lpMsg->account) == false || gQueryManager.Fetch() == SQL_NO_DATA)
		#else
			if (gQueryManager.ExecResultQuery("SELECT * FROM AccountCharacter WHERE Id='%s'", lpMsg->account) == false || gQueryManager.Fetch() == false)
		#endif
			{
				gQueryManager.Close();

				pMsg.result = 1;
			}
			else
			{
				gQueryManager.GetAsString("GameID1", CharacterName[0], sizeof(CharacterName[0]));

				gQueryManager.GetAsString("GameID2", CharacterName[1], sizeof(CharacterName[1]));

				gQueryManager.GetAsString("GameID3", CharacterName[2], sizeof(CharacterName[2]));

				gQueryManager.GetAsString("GameID4", CharacterName[3], sizeof(CharacterName[3]));

				gQueryManager.GetAsString("GameID5", CharacterName[4], sizeof(CharacterName[4]));

				gQueryManager.Close();

				BYTE slot;

				if (GetCharacterSlot(CharacterName, lpMsg->name, &slot) != false)
				{
				#ifndef MYSQL

					gQueryManager.ExecQuery("UPDATE AccountCharacter SET GameID%d=NULL WHERE Id='%s'", (slot + 1), lpMsg->account);

				#else

					gQueryManager.ExecUpdateQuery("UPDATE AccountCharacter SET GameID%d=NULL WHERE Id='%s'", (slot + 1), lpMsg->account);

				#endif

					gQueryManager.Close();
				}
			}

			if (lpMsg->guild == 1)
			{
				SDHP_GUILD_DELETE_RECV pMsg;

				pMsg.index = lpMsg->index;

				memcpy(pMsg.Name, lpMsg->name, sizeof(pMsg.Name));

				memcpy(pMsg.GuildName, lpMsg->GuildName, sizeof(pMsg.GuildName));

				gGuild.GDGuildDeleteRecv(&pMsg, index);
			}
			else if (lpMsg->guild == 2)
			{
				SDHP_GUILD_MEMBER_DELETE_RECV pMsg;

				pMsg.index = lpMsg->index;

				memcpy(pMsg.Name, lpMsg->name, sizeof(pMsg.Name));

				memcpy(pMsg.GuildName, lpMsg->GuildName, sizeof(pMsg.GuildName));

				gGuild.GDGuildMemberDeleteRecv(&pMsg, index);
			}
		}
	}

	gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);
}

void GDCharacterInfoRecv(SDHP_CHARACTER_INFO_RECV* lpMsg, int index)
{
	SDHP_CHARACTER_INFO_SEND pMsg;

	pMsg.header.set(0x01, 0x03, sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account, lpMsg->account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpMsg->name, sizeof(pMsg.name));

	pMsg.result = ((CheckTextSyntax(lpMsg->name, sizeof(lpMsg->name)) == false) ? 0 : 1);

#ifndef MYSQL
	if (pMsg.result == 0 || gQueryManager.ExecQuery("SELECT * FROM Character WHERE AccountID='%s' AND Name='%s'", lpMsg->account, lpMsg->name) == false || gQueryManager.Fetch() == SQL_NO_DATA)
#else
	if (pMsg.result == 0 || gQueryManager.ExecResultQuery("SELECT * FROM `Character` WHERE AccountID='%s' AND Name='%s'", lpMsg->account, lpMsg->name) == false || gQueryManager.Fetch() == false)
#endif
	{
		gQueryManager.Close();

		pMsg.result = 0;
	}
	else
	{
		pMsg.Level = (WORD)gQueryManager.GetAsInteger("cLevel");

		pMsg.Class = (BYTE)gQueryManager.GetAsInteger("Class");

		pMsg.LevelUpPoint = gQueryManager.GetAsInteger("LevelUpPoint");

		pMsg.Experience = gQueryManager.GetAsInteger("Experience");

		pMsg.Strength = gQueryManager.GetAsInteger("Strength");

		pMsg.Dexterity = gQueryManager.GetAsInteger("Dexterity");

		pMsg.Vitality = gQueryManager.GetAsInteger("Vitality");

		pMsg.Energy = gQueryManager.GetAsInteger("Energy");

		gQueryManager.GetAsBinary("Inventory", pMsg.Inventory[0], sizeof(pMsg.Inventory));

		gQueryManager.GetAsBinary("MagicList", pMsg.Skill[0], sizeof(pMsg.Skill));

		pMsg.Money = gQueryManager.GetAsInteger("Money");

	#ifndef MYSQL

		pMsg.Life = (DWORD)gQueryManager.GetAsFloat("Life");

		pMsg.MaxLife = (DWORD)gQueryManager.GetAsFloat("MaxLife");

		pMsg.Mana = (DWORD)gQueryManager.GetAsFloat("Mana");

		pMsg.MaxMana = (DWORD)gQueryManager.GetAsFloat("MaxMana");

		pMsg.BP = (DWORD)gQueryManager.GetAsFloat("BP");

		pMsg.MaxBP = (DWORD)gQueryManager.GetAsFloat("MaxBP");

	#else

		pMsg.Life = gQueryManager.GetAsInteger("Life");

		pMsg.MaxLife = gQueryManager.GetAsInteger("MaxLife");

		pMsg.Mana = gQueryManager.GetAsInteger("Mana");

		pMsg.MaxMana = gQueryManager.GetAsInteger("MaxMana");

		pMsg.BP = gQueryManager.GetAsInteger("BP");

		pMsg.MaxBP = gQueryManager.GetAsInteger("MaxBP");

	#endif

		pMsg.Map = (BYTE)gQueryManager.GetAsInteger("MapNumber");

		pMsg.X = (BYTE)gQueryManager.GetAsInteger("MapPosX");

		pMsg.Y = (BYTE)gQueryManager.GetAsInteger("MapPosY");

		pMsg.Dir = (BYTE)gQueryManager.GetAsInteger("MapDir");

		pMsg.PKCount = gQueryManager.GetAsInteger("PkCount");

		pMsg.PKLevel = gQueryManager.GetAsInteger("PkLevel");

		pMsg.PKTime = gQueryManager.GetAsInteger("PkTime");

		pMsg.CtlCode = (BYTE)gQueryManager.GetAsInteger("CtlCode");

		gQueryManager.GetAsBinary("Quest", pMsg.Quest, sizeof(pMsg.Quest));

		gQueryManager.GetAsBinary("EffectList", pMsg.Effect[0], sizeof(pMsg.Effect));

		pMsg.FruitAddPoint = (WORD)gQueryManager.GetAsInteger("FruitAddPoint");

		pMsg.FruitSubPoint = (WORD)gQueryManager.GetAsInteger("FruitSubPoint");

		gQueryManager.Close();

	#ifndef MYSQL

		gQueryManager.ExecQuery("EXEC WZ_GetResetInfo '%s','%s'", lpMsg->account, lpMsg->name);

	#else

		gQueryManager.ExecResultQuery("CALL WZ_GetResetInfo('%s', '%s')", lpMsg->account, lpMsg->name);

	#endif

		gQueryManager.Fetch();

		pMsg.Reset = gQueryManager.GetAsInteger("Reset");

		gQueryManager.Close();

	#ifndef MYSQL

		gQueryManager.ExecQuery("EXEC WZ_GetGrandResetInfo '%s','%s'", lpMsg->account, lpMsg->name);

	#else

		gQueryManager.ExecResultQuery("CALL WZ_GetGrandResetInfo('%s', '%s')", lpMsg->account, lpMsg->name);

	#endif

		gQueryManager.Fetch();

		pMsg.GrandReset = gQueryManager.GetAsInteger("GrandReset");

		gQueryManager.Close();

	#ifndef MYSQL

		gQueryManager.ExecQuery("UPDATE AccountCharacter SET GameIDC='%s' WHERE Id='%s'", lpMsg->name, lpMsg->account);

	#else

		gQueryManager.ExecUpdateQuery("UPDATE AccountCharacter SET GameIDC='%s' WHERE Id='%s'", lpMsg->name, lpMsg->account);

	#endif

		gQueryManager.Close();
	}

	gSocketManager.DataSend(index, (BYTE*)&pMsg, sizeof(pMsg));
}

void GDCharacterInfoSaveRecv(SDHP_CHARACTER_INFO_SAVE_RECV* lpMsg)
{
#ifndef MYSQL

	gQueryManager.BindParameterAsBinary(1, lpMsg->Inventory[0], sizeof(lpMsg->Inventory));

	gQueryManager.BindParameterAsBinary(2, lpMsg->Skill[0], sizeof(lpMsg->Skill));

	gQueryManager.BindParameterAsBinary(3, lpMsg->Quest, sizeof(lpMsg->Quest));

	gQueryManager.BindParameterAsBinary(4, lpMsg->Effect[0], sizeof(lpMsg->Effect));

	gQueryManager.ExecQuery("UPDATE Character SET cLevel=%d,Class=%d,LevelUpPoint=%d,Experience=%d,Strength=%d,Dexterity=%d,Vitality=%d,Energy=%d,Inventory=?,MagicList=?,Money=%d,Life=%f,MaxLife=%f,Mana=%f,MaxMana=%f,BP=%f,MaxBP=%f,MapNumber=%d,MapPosX=%d,MapPosY=%d,MapDir=%d,PkCount=%d,PkLevel=%d,PkTime=%d,Quest=?,EffectList=?,FruitAddPoint=%d,FruitSubPoint=%d WHERE AccountID='%s' AND Name='%s'", lpMsg->Level, lpMsg->Class, lpMsg->LevelUpPoint, lpMsg->Experience, lpMsg->Strength, lpMsg->Dexterity, lpMsg->Vitality, lpMsg->Energy, lpMsg->Money, (float)lpMsg->Life, (float)lpMsg->MaxLife, (float)lpMsg->Mana, (float)lpMsg->MaxMana, (float)lpMsg->BP, (float)lpMsg->MaxBP, lpMsg->Map, lpMsg->X, lpMsg->Y, lpMsg->Dir, lpMsg->PKCount, lpMsg->PKLevel, lpMsg->PKTime, lpMsg->FruitAddPoint, lpMsg->FruitSubPoint, lpMsg->account, lpMsg->name);

#else

	gQueryManager.PrepareQuery
	(
		"UPDATE `Character` SET cLevel=%u, Class=%d, LevelUpPoint=%u, Experience=%u, Strength=%u, Dexterity=%u, Vitality=%u, Energy=%u, Inventory=?, MagicList=?, Money=%u, Life=%u, MaxLife=%u, Mana=%u, MaxMana=%u, BP=%u, MaxBP=%u, MapNumber=%d, MapPosX=%d, MapPosY=%d, MapDir=%d, PkCount=%u, PkLevel=%d, PkTime=%u, Quest=?, EffectList=?, FruitAddPoint=%d, FruitSubPoint=%d WHERE AccountID='%s' AND Name='%s'",
		lpMsg->Level, lpMsg->Class, lpMsg->LevelUpPoint, lpMsg->Experience, lpMsg->Strength, lpMsg->Dexterity, lpMsg->Vitality, lpMsg->Energy, lpMsg->Money, lpMsg->Life, lpMsg->MaxLife, lpMsg->Mana, lpMsg->MaxMana, lpMsg->BP, lpMsg->MaxBP, lpMsg->Map, lpMsg->X, lpMsg->Y, lpMsg->Dir, lpMsg->PKCount, lpMsg->PKLevel, lpMsg->PKTime, lpMsg->FruitAddPoint, lpMsg->FruitSubPoint, lpMsg->account, lpMsg->name
	);

	gQueryManager.SetAsBinary(1, lpMsg->Inventory[0], sizeof(lpMsg->Inventory));

	gQueryManager.SetAsBinary(2, lpMsg->Skill[0], sizeof(lpMsg->Skill));

	gQueryManager.SetAsBinary(3, lpMsg->Quest, sizeof(lpMsg->Quest));

	gQueryManager.SetAsBinary(4, lpMsg->Effect[0], sizeof(lpMsg->Effect));

	gQueryManager.ExecPreparedUpdateQuery();

#endif

	gQueryManager.Close();
}

void GDConnectCharacterRecv(SDHP_CONNECT_CHARACTER_RECV* lpMsg, int index)
{
	CHARACTER_INFO CharacterInfo;

	if (gCharacterManager.GetCharacterInfo(&CharacterInfo, lpMsg->name) != false)
	{
		return;
	}

	strcpy_s(CharacterInfo.Name, lpMsg->name);

	strcpy_s(CharacterInfo.Account, lpMsg->account);

	CharacterInfo.UserIndex = lpMsg->index;

	CharacterInfo.GameServerCode = gServerManager[index].m_ServerCode;

	gCharacterManager.InsertCharacterInfo(CharacterInfo);
}

void GDDisconnectCharacterRecv(SDHP_CONNECT_CHARACTER_RECV* lpMsg, int index)
{
	CHARACTER_INFO CharacterInfo;

	if (gCharacterManager.GetCharacterInfo(&CharacterInfo, lpMsg->name) == false)
	{
		return;
	}

	if (CharacterInfo.UserIndex != lpMsg->index)
	{
		return;
	}

	if (CharacterInfo.GameServerCode != gServerManager[index].m_ServerCode)
	{
		return;
	}

	gCharacterManager.RemoveCharacterInfo(CharacterInfo);
}

void GDCreateItemRecv(SDHP_CREATE_ITEM_RECV* lpMsg, int index)
{
	SDHP_CREATE_ITEM_SEND pMsg;

	pMsg.header.set(0x02, 0x00, sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account, lpMsg->account, sizeof(pMsg.account));

	pMsg.X = lpMsg->X;

	pMsg.Y = lpMsg->Y;

	pMsg.Map = lpMsg->Map;

#ifndef MYSQL
	if (gQueryManager.ExecQuery("EXEC WZ_GetItemSerial") == false || gQueryManager.Fetch() == SQL_NO_DATA)
#else
	if (gQueryManager.ExecResultQuery("CALL WZ_GetItemSerial()") == false || gQueryManager.Fetch() == false)
#endif
	{
		gQueryManager.Close();

		pMsg.Serial = 0;
	}
	else
	{
	#ifndef MYSQL

		pMsg.Serial = gQueryManager.GetResult(0);

	#else

		pMsg.Serial = gQueryManager.GetAsInteger("Result");

	#endif

		gQueryManager.Close();
	}

	pMsg.ItemIndex = lpMsg->ItemIndex;

	pMsg.Level = lpMsg->Level;

	pMsg.Dur = lpMsg->Dur;

	pMsg.SkillOption = lpMsg->SkillOption;

	pMsg.LuckOption = lpMsg->LuckOption;

	pMsg.AddOption = lpMsg->AddOption;

	pMsg.ExceOption = lpMsg->ExceOption;

	pMsg.LootIndex = lpMsg->LootIndex;

	gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);
}

void GDInventoryItemSaveRecv(SDHP_INVENTORY_ITEM_SAVE_RECV* lpMsg)
{
#ifndef MYSQL

	gQueryManager.BindParameterAsBinary(1, lpMsg->Inventory[0], sizeof(lpMsg->Inventory));

	gQueryManager.ExecQuery("UPDATE Character SET Inventory=? WHERE AccountID='%s' AND Name='%s'", lpMsg->account, lpMsg->name);

#else

	gQueryManager.PrepareQuery("UPDATE `Character` SET Inventory=? WHERE AccountID='%s' AND Name='%s'", lpMsg->account, lpMsg->name);

	gQueryManager.SetAsBinary(1, lpMsg->Inventory[0], sizeof(lpMsg->Inventory));

	gQueryManager.ExecPreparedUpdateQuery();

#endif

	gQueryManager.Close();
}

void GDOptionDataRecv(SDHP_OPTION_DATA_RECV* lpMsg, int index)
{
	SDHP_OPTION_DATA_SEND pMsg;

	pMsg.header.set(0x03, 0x00, sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account, lpMsg->account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpMsg->name, sizeof(pMsg.name));

#ifndef MYSQL
	if (gQueryManager.ExecQuery("SELECT * FROM OptionData WHERE Name='%s'", lpMsg->name) == false || gQueryManager.Fetch() == SQL_NO_DATA)
#else
	if (gQueryManager.ExecResultQuery("SELECT * FROM OptionData WHERE Name='%s'", lpMsg->name) == false || gQueryManager.Fetch() == false)
#endif
	{
		gQueryManager.Close();

		memset(pMsg.SkillKey, 0xFF, sizeof(pMsg.SkillKey));

		pMsg.GameOption = 0xFF;

		pMsg.QKey = 0xFF;

		pMsg.WKey = 0xFF;

		pMsg.EKey = 0xFF;

		pMsg.ChatWindow = 0x0F;
	}
	else
	{
		gQueryManager.GetAsBinary("SkillKey", pMsg.SkillKey, sizeof(pMsg.SkillKey));

		pMsg.GameOption = (BYTE)gQueryManager.GetAsInteger("GameOption");

		pMsg.QKey = (BYTE)gQueryManager.GetAsInteger("Qkey");

		pMsg.WKey = (BYTE)gQueryManager.GetAsInteger("Wkey");

		pMsg.EKey = (BYTE)gQueryManager.GetAsInteger("Ekey");

		pMsg.ChatWindow = (BYTE)gQueryManager.GetAsInteger("ChatWindow");

		gQueryManager.Close();
	}

	gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);
}

void GDOptionDataSaveRecv(SDHP_OPTION_DATA_SAVE_RECV* lpMsg)
{
#ifndef MYSQL

	if (gQueryManager.ExecQuery("SELECT Name FROM OptionData WHERE Name='%s'", lpMsg->name) == false || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		gQueryManager.BindParameterAsBinary(1, lpMsg->SkillKey, sizeof(lpMsg->SkillKey));

		gQueryManager.ExecQuery("INSERT INTO OptionData (Name,SkillKey,GameOption,Qkey,Wkey,Ekey,ChatWindow) VALUES ('%s',?,%d,%d,%d,%d,%d)", lpMsg->name, lpMsg->GameOption, lpMsg->QKey, lpMsg->WKey, lpMsg->EKey, lpMsg->ChatWindow);

		gQueryManager.Close();
	}
	else
	{
		gQueryManager.Close();

		gQueryManager.BindParameterAsBinary(1, lpMsg->SkillKey, sizeof(lpMsg->SkillKey));

		gQueryManager.ExecQuery("UPDATE OptionData SET SkillKey=?,GameOption=%d,Qkey=%d,Wkey=%d,Ekey=%d,ChatWindow=%d WHERE Name='%s'", lpMsg->GameOption, lpMsg->QKey, lpMsg->WKey, lpMsg->EKey, lpMsg->ChatWindow, lpMsg->name);

		gQueryManager.Close();
	}

#else

	if (gQueryManager.ExecResultQuery("SELECT Name FROM OptionData WHERE Name='%s'", lpMsg->name) == false || gQueryManager.Fetch() == false)
	{
		gQueryManager.Close();

		gQueryManager.PrepareQuery("INSERT INTO OptionData (Name, SkillKey, GameOption, Qkey, Wkey, Ekey, ChatWindow) VALUES ('%s', ?, %d, %d, %d, %d, %d)", lpMsg->name, lpMsg->GameOption, lpMsg->QKey, lpMsg->WKey, lpMsg->EKey, lpMsg->ChatWindow);

		gQueryManager.SetAsBinary(1, lpMsg->SkillKey, sizeof(lpMsg->SkillKey));

		gQueryManager.ExecPreparedUpdateQuery();

		gQueryManager.Close();
}
	else
	{
		gQueryManager.Close();

		gQueryManager.PrepareQuery("UPDATE OptionData SET SkillKey=?, GameOption=%d, Qkey=%d, Wkey=%d, Ekey=%d, ChatWindow=%d WHERE Name='%s'", lpMsg->GameOption, lpMsg->QKey, lpMsg->WKey, lpMsg->EKey, lpMsg->ChatWindow, lpMsg->name);

		gQueryManager.SetAsBinary(1, lpMsg->SkillKey, sizeof(lpMsg->SkillKey));

		gQueryManager.ExecPreparedUpdateQuery();

		gQueryManager.Close();
	}

#endif
}

void GDResetInfoSaveRecv(SDHP_RESET_INFO_SAVE_RECV* lpMsg)
{
#ifndef MYSQL

	gQueryManager.ExecQuery("EXEC WZ_SetResetInfo '%s','%s','%d','%d','%d','%d'", lpMsg->account, lpMsg->name, lpMsg->Reset, lpMsg->ResetDay, lpMsg->ResetWek, lpMsg->ResetMon);

	gQueryManager.Fetch();

#else

	gQueryManager.ExecUpdateQuery("CALL WZ_SetResetInfo('%s', '%s', '%d', '%d', '%d', '%d')", lpMsg->account, lpMsg->name, lpMsg->Reset, lpMsg->ResetDay, lpMsg->ResetWek, lpMsg->ResetMon);

#endif

	gQueryManager.Close();
}

void GDGrandResetInfoSaveRecv(SDHP_GRAND_RESET_INFO_SAVE_RECV* lpMsg)
{
#ifndef MYSQL

	gQueryManager.ExecQuery("EXEC WZ_SetGrandResetInfo '%s','%s','%d','%d','%d','%d','%d'", lpMsg->account, lpMsg->name, lpMsg->Reset, lpMsg->GrandReset, lpMsg->GrandResetDay, lpMsg->GrandResetWek, lpMsg->GrandResetMon);

	gQueryManager.Fetch();

#else

	gQueryManager.ExecUpdateQuery("CALL WZ_SetGrandResetInfo('%s', '%s', '%d', '%d', '%d', '%d', '%d')", lpMsg->account, lpMsg->name, lpMsg->Reset, lpMsg->GrandReset, lpMsg->GrandResetDay, lpMsg->GrandResetWek, lpMsg->GrandResetMon);

#endif

	gQueryManager.Close();
}

void GDGlobalNoticeRecv(SDHP_GLOBAL_NOTICE_RECV* lpMsg, int index)
{
	SDHP_GLOBAL_NOTICE_SEND pMsg;

	pMsg.header.set(0x05, 0x00, sizeof(pMsg));

	pMsg.type = lpMsg->type;

	memcpy(pMsg.message, lpMsg->message, sizeof(pMsg.message));

	for (int n = 0; n < MAX_SERVER; n++)
	{
		if (gServerManager[n].CheckState() != false)
		{
			gSocketManager.DataSend(n, (BYTE*)&pMsg, pMsg.header.size);
		}
	}
}

void GDGlobalWhisperRecv(SDHP_GLOBAL_WHISPER_RECV* lpMsg, int index)
{
	SDHP_GLOBAL_WHISPER_SEND pMsg;

	pMsg.header.set(0x05, 0x01, sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account, lpMsg->account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpMsg->name, sizeof(pMsg.name));

	CHARACTER_INFO CharacterInfo;

	if (gCharacterManager.GetCharacterInfo(&CharacterInfo, lpMsg->TargetName) == false)
	{
		pMsg.result = 0;
	}
	else
	{
		pMsg.result = 1;

		DGGlobalWhisperEchoSend(CharacterInfo.GameServerCode, CharacterInfo.UserIndex, CharacterInfo.Account, CharacterInfo.Name, lpMsg->name, lpMsg->message);
	}

	memcpy(pMsg.TargetName, lpMsg->TargetName, sizeof(pMsg.TargetName));

	memcpy(pMsg.message, lpMsg->message, sizeof(pMsg.message));

	gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);
}

void DGGlobalWhisperEchoSend(WORD ServerCode, WORD index, char* account, char* name, char* SourceName, char* message)
{
	SDHP_GLOBAL_WHISPER_ECHO_SEND pMsg;

	pMsg.header.set(0x05, 0x02, sizeof(pMsg));

	pMsg.index = index;

	memcpy(pMsg.account, account, sizeof(pMsg.account));

	memcpy(pMsg.name, name, sizeof(pMsg.name));

	memcpy(pMsg.SourceName, SourceName, sizeof(pMsg.SourceName));

	memcpy(pMsg.message, message, sizeof(pMsg.message));

	CServerManager* lpServerManager = FindServerByCode(ServerCode);

	if (lpServerManager != 0)
	{
		gSocketManager.DataSend(lpServerManager->m_index, (BYTE*)&pMsg, pMsg.header.size);
	}
}

void GDRankingBloodCastleSaveRecv(SDHP_RANKING_SAVE_RECV* lpMsg)
{
#ifndef MYSQL

	if (gQueryManager.ExecQuery("SELECT Name FROM RankingBloodCastle WHERE Name='%s'", lpMsg->name) == false || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		gQueryManager.ExecQuery("INSERT INTO RankingBloodCastle (Name,Score) VALUES ('%s',%d)", lpMsg->name, lpMsg->score);

		gQueryManager.Close();
	}
	else
	{
		gQueryManager.Close();

		gQueryManager.ExecQuery("UPDATE RankingBloodCastle SET Score=Score+%d WHERE Name='%s'", lpMsg->score, lpMsg->name);

		gQueryManager.Close();
	}

#else

	if (gQueryManager.ExecResultQuery("SELECT Name FROM RankingBloodCastle WHERE Name='%s'", lpMsg->name) == false || gQueryManager.Fetch() == false)
	{
		gQueryManager.Close();

		gQueryManager.ExecUpdateQuery("INSERT INTO RankingBloodCastle (Name, Score) VALUES ('%s', %d)", lpMsg->name, lpMsg->score);

		gQueryManager.Close();
}
	else
	{
		gQueryManager.Close();

		gQueryManager.ExecUpdateQuery("UPDATE RankingBloodCastle SET Score=Score+%d WHERE Name='%s'", lpMsg->score, lpMsg->name);

		gQueryManager.Close();
	}

#endif
}

void GDRankingDevilSquareSaveRecv(SDHP_RANKING_SAVE_RECV* lpMsg)
{
#ifndef MYSQL

	if (gQueryManager.ExecQuery("SELECT Name FROM RankingDevilSquare WHERE Name='%s'", lpMsg->name) == false || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		gQueryManager.ExecQuery("INSERT INTO RankingDevilSquare (Name,Score) VALUES ('%s',%d)", lpMsg->name, lpMsg->score);

		gQueryManager.Close();
	}
	else
	{
		gQueryManager.Close();

		gQueryManager.ExecQuery("UPDATE RankingDevilSquare SET Score=Score+%d WHERE Name='%s'", lpMsg->score, lpMsg->name);

		gQueryManager.Close();
	}

#else

	if (gQueryManager.ExecResultQuery("SELECT Name FROM RankingDevilSquare WHERE Name='%s'", lpMsg->name) == false || gQueryManager.Fetch() == false)
	{
		gQueryManager.Close();

		gQueryManager.ExecUpdateQuery("INSERT INTO RankingDevilSquare (Name, Score) VALUES ('%s', %d)", lpMsg->name, lpMsg->score);

		gQueryManager.Close();
}
	else
	{
		gQueryManager.Close();

		gQueryManager.ExecUpdateQuery("UPDATE RankingDevilSquare SET Score=Score+%d WHERE Name='%s'", lpMsg->score, lpMsg->name);

		gQueryManager.Close();
	}

#endif
}