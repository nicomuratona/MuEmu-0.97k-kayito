#include "stdafx.h"
#include "Protocol.h"
#include "ChaosMix.h"
#include "EventTimer.h"
#include "HackCheck.h"
#include "HealthBar.h"
#include "HWID.h"
#include "ItemManager.h"
#include "ItemStack.h"
#include "ItemValue.h"
#include "Language.h"
#include "MoveList.h"
#include "PacketManager.h"
#include "PrintPlayer.h"
#include "Protect.h"
#include "Reconnect.h"
#include "ServerList.h"
#include "Window.h"

CProtocol gProtocol;

CProtocol::CProtocol()
{

}

CProtocol::~CProtocol()
{

}

void CProtocol::Init()
{
	SetCompleteHook(0xE9, 0x004389D7, &this->HookProtocol);

	SetDword(0x0041F188, 1000); // For Ping

	SetCompleteHook(0xE8, 0x0043A62C, &this->CGItemGetRecv);
}

_declspec(naked) void CProtocol::HookProtocol()
{
	static DWORD jmpBack = 0x004389DC;

	_asm
	{
		Pushad;
		Push Ebp;
		Mov Ecx, gProtocol;
		Call [CProtocol::ProtocolCompiler];
		Popad;
		Mov Al, [Ebp + 0];
		Xor Ebx, Ebx;
		Jmp[jmpBack];
	}
}

void CProtocol::ProtocolCompiler(BYTE* lpMsg)
{
	int count = 0, size = 0, DecSize = 0;

	BYTE header, head;

	BYTE DecBuff[MAX_MAIN_PACKET_SIZE] = { 0 };

	if (lpMsg[count] == 0xC1 || lpMsg[count] == 0xC3)
	{
		header = lpMsg[count];

		size = lpMsg[count + 1];

		head = lpMsg[count + 2];
	}
	else if (lpMsg[count] == 0xC2 || lpMsg[count] == 0xC4)
	{
		header = lpMsg[count];

		size = MAKEWORD(lpMsg[count + 2], lpMsg[count + 1]);

		head = lpMsg[count + 3];
	}
	else
	{
		char lpszMessage[128];

		wsprintf(lpszMessage, "[Connection] Protocol header error (Header: %02X)\r\n", lpMsg[count]);

		MessageBox(NULL, lpszMessage, "Error", MB_OK);

		return;
	}

	if (header == 0xC3 || header == 0xC4)
	{
		if (header == 0xC3)
		{
			DecSize = gPacketManager.Decrypt(&DecBuff[1], &lpMsg[count + 2], (size - 2)) + 1;

			header = 0xC1;

			head = DecBuff[2];

			DecBuff[0] = header;

			DecBuff[1] = DecSize;
		}
		else
		{
			DecSize = gPacketManager.Decrypt(&DecBuff[2], &lpMsg[count + 3], (size - 3)) + 2;

			header = 0xC2;

			head = DecBuff[3];

			DecBuff[0] = header;

			DecBuff[1] = HIBYTE(DecSize);

			DecBuff[2] = LOBYTE(DecSize);
		}
	}
	else
	{
		memcpy(DecBuff, lpMsg, size);

		DecSize = size;
	}

	this->TranslateProtocol(head, DecBuff, DecSize);
}

void CProtocol::TranslateProtocol(BYTE head, BYTE* lpMsg, int Size)
{
	//ConsoleProtocolLog(CON_PROTO_TCP_RECV, lpMsg, Size);

	switch (head)
	{
		case 0xE:
		{
			this->GCLiveClientRecv((PMSG_LIVE_CLIENT_RECV*)lpMsg);

			break;
		}

		case 0xD:
		{
			this->GCNoticeRecv((PMSG_NOTICE_RECV*)lpMsg);

			break;
		}

		case 0x15:
		{
			this->GCDamageRecv((PMSG_DAMAGE_RECV*)lpMsg);

			break;
		}

		case 0x17:
		{
			this->GCUserDieRecv((PMSG_USER_DIE_RECV*)lpMsg);

			break;
		}

		case 0x1E:
		{
			this->GCDurationSkillAttackRecv((PMSG_DURATION_SKILL_ATTACK_RECV*)lpMsg);

			break;
		}

		case 0x24:
		{
			gChaosMix.Clear();

			break;
		}

		case 0x26:
		{
			this->GCLifeRecv((PMSG_LIFE_RECV*)lpMsg);

			break;
		}

		case 0x27:
		{
			this->GCManaRecv((PMSG_MANA_RECV*)lpMsg);

			break;
		}

		case 0x2C:
		{
			this->GCFruitResultRecv((PMSG_FRUIT_RESULT_RECV*)lpMsg);

			break;
		}

		case 0x87:
		{
			gChaosMix.Clear();

			break;
		}

		case 0x88:
		{
			gChaosMix.GCChaosMixRateRecv((PMSG_CHAOS_MIX_RATE_RECV*)lpMsg);

			break;
		}

		case 0x9C:
		{
			this->GCRewardExperienceRecv((PMSG_REWARD_EXPERIENCE_RECV*)lpMsg);

			break;
		}

		case 0xA3:
		{
			this->GCQuestRewardRecv((PMSG_QUEST_REWARD_RECV*)lpMsg);

			break;
		}

		case 0xDE:
		{
			this->GCCharacterCreationEnableRecv((PMSG_CHARACTER_CREATION_ENABLE_RECV*)lpMsg);

			break;
		}

		case 0xF1:
		{
			switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
			{
				case 0x00:
				{
					this->GCConnectClientRecv((PMSG_CONNECT_CLIENT_RECV*)lpMsg);

					break;
				}

				case 0x01:
				{
					this->GCConnectAccountRecv((PMSG_CONNECT_ACCOUNT_RECV*)lpMsg);

					break;
				}

				case 0x02:
				{
					this->GCCloseClientRecv((PMSG_CLOSE_CLIENT_RECV*)lpMsg);

					break;
				}
			}

			break;
		}

		case 0xF3:
		{
			switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
			{
				case 0x00:
				{
					this->GCCharacterListRecv((PMSG_CHARACTER_LIST_RECV*)lpMsg);

					break;
				}

				case 0x03:
				{
					this->GCCharacterInfoRecv((PMSG_CHARACTER_INFO_RECV*)lpMsg);

					break;
				}

				case 0x04:
				{
					this->GCCharacterRegenRecv((PMSG_CHARACTER_REGEN_RECV*)lpMsg);

					break;
				}

				case 0x05:
				{
					this->GCLevelUpRecv((PMSG_LEVEL_UP_RECV*)lpMsg);

					break;
				}

				case 0x06:
				{
					this->GCLevelUpPointRecv((PMSG_LEVEL_UP_POINT_RECV*)lpMsg);

					break;
				}

				case 0x07:
				{
					this->GCMonsterDamageRecv((PMSG_MONSTER_DAMAGE_RECV*)lpMsg);

					break;
				}

				case 0xE0:
				{
					this->GCNewCharacterInfoRecv((PMSG_NEW_CHARACTER_INFO_RECV*)lpMsg);

					break;
				}

				case 0xE1:
				{
					this->GCNewCharacterCalcRecv((PMSG_NEW_CHARACTER_CALC_RECV*)lpMsg);

					break;
				}

				case 0xE2:
				{
					this->GCHealthBarRecv((PMSG_HEALTH_BAR_RECV*)lpMsg);

					break;
				}

				case 0xE3:
				{
					gItemStack.GCItemStackListRecv((PMSG_ITEM_STACK_LIST_RECV*)lpMsg);

					break;
				}

				case 0xE4:
				{
					gItemValue.GCItemValueListRecv((PMSG_ITEM_VALUE_LIST_RECV*)lpMsg);

					break;
				}

				case 0xE5:
				{
					gMoveList.GCMoveListRecv((PMSG_MOVE_LIST_RECV*)lpMsg);

					break;
				}

				case 0xE6:
				{
					gEventTimer.GCEventTimeRecv((PMSG_EVENT_TIME_RECV*)lpMsg);

					break;
				}
			}

			break;
		}

		case 0xF4:
		{
			switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
			{
				case 0x04:
				{
					gServerList.GCCustomServerListRecv((PMSG_CUSTOM_SERVER_LIST_RECV*)lpMsg);

					break;
				}
			}

			break;
		}
	}
}

void CProtocol::GCLiveClientRecv(PMSG_LIVE_CLIENT_RECV* lpMsg)
{
	gPrintPlayer.ViewPing = GetTickCount() - lpMsg->TickCount;
}

void CProtocol::GCNoticeRecv(PMSG_NOTICE_RECV* lpMsg)
{
	if (lpMsg->type == 3)
	{
		char text[64] = { 0 };

		sprintf_s(text, GetTextLine(486), atoi(lpMsg->message));

		((void(_cdecl*)(char* strID, char* strText, int MsgType))0x00480620)((char*)0x005826D3C, text, 1);
	}
}

void CProtocol::GCDamageRecv(PMSG_DAMAGE_RECV* lpMsg)
{
	int aIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]) & 0x7FFF;

	if (gPrintPlayer.ViewIndex == aIndex)
	{
		gPrintPlayer.ViewCurHP = lpMsg->ViewCurHP;
	}

	gPrintPlayer.ViewDamageHP = lpMsg->ViewDamageHP;
}

void CProtocol::GCUserDieRecv(PMSG_USER_DIE_RECV* lpMsg)
{
	int aIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]) & 0x7FFF;

	if (gPrintPlayer.ViewIndex == aIndex)
	{
		gPrintPlayer.ViewCurHP = 0;

		gWindow.ShowTrayMessage((char*)(Hero + 0x1C1), "You Died");
	}
}

void CProtocol::GCDurationSkillAttackRecv(PMSG_DURATION_SKILL_ATTACK_RECV* lpMsg)
{
	int aIndex = FindCharacterIndex(MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]));

	if (aIndex >= 0 && aIndex < MAX_MAIN_VIEWPORT)
	{
		DWORD c = CharactersClient + (aIndex * 916);

		*(float*)(c + 0x24) = (lpMsg->dir / 256.0f) * 360.0f;
	}
}

void CProtocol::CGItemGetRecv(PMSG_ITEM_GET_RECV* lpMsg)
{
	if (lpMsg->result != (BYTE)0xFF)
	{
		char szMessage[128];

		int Type = ConvertItemType(lpMsg->ItemInfo);

		if (lpMsg->result == (BYTE)0xFE) // Receive Money
		{
			STRUCT_DECRYPT;

			int backupGold = *(DWORD*)(CharacterMachine + 0x548);

			*(DWORD*)(CharacterMachine + 0x548) = (lpMsg->ItemInfo[0] << 24) + (lpMsg->ItemInfo[1] << 16) + (lpMsg->ItemInfo[2] << 8) + (lpMsg->ItemInfo[3]);

			int getGold = 0;

			if (backupGold != -1)
			{
				getGold = *(DWORD*)(CharacterMachine + 0x548) - backupGold;
			}

			STRUCT_ENCRYPT;

			if (getGold > 0)
			{
				char money[32];

				ConvertGold(getGold, money);

				wsprintf(szMessage, "%s %s %s", money, GlobalText[224], GlobalText[918]);

				UIChatLogWindow_AddText("", szMessage, 1);
			}
		}
		else // Receive Item
		{
			if (lpMsg->result != (BYTE)0xFD) // Not Stackeable
			{
				if (INVENTORY_MAX_RANGE(lpMsg->result) != 0)
				{
					InsertInventoryItem((ITEM*)OffsetInventoryItems, 8, 8, lpMsg->result, lpMsg->ItemInfo, false);
				}
			}

			char szItem[64] = { 0, };

			int Level = (lpMsg->ItemInfo[1] >> 3) & 0xF;

			gItemManager.GetItemName(Type, Level, szItem);

			wsprintf(szMessage, "%s %s", szItem, GlobalText[918]);

			UIChatLogWindow_AddText("", szMessage, 1);
		}

		if (Type == GET_ITEM(14, 13) // Jewel of Bless
		    || Type == GET_ITEM(14, 14) // Jewel of Soul
		    || Type == GET_ITEM(14, 16) // Jewel of Life
		    || Type == GET_ITEM(13, 15) // Jewel of Chaos
		    || Type == GET_ITEM(14, 22)) // Jewel of Creation
		{
			PlayBuffer(49, Hero, FALSE);
		}
		else
		{
			PlayBuffer(29, Hero, FALSE);
		}
	}

	SendGetItem = -1;
}

void CProtocol::GCLifeRecv(PMSG_LIFE_RECV* lpMsg)
{
	if (lpMsg->type == 0xFE)
	{
		gPrintPlayer.ViewMaxHP = lpMsg->ViewHP;
	}

	if (lpMsg->type == 0xFF)
	{
		gPrintPlayer.ViewCurHP = ((gPrintPlayer.ViewCurHP == 0) ? gPrintPlayer.ViewCurHP : lpMsg->ViewHP);
	}
}

void CProtocol::GCManaRecv(PMSG_MANA_RECV* lpMsg)
{
	if (lpMsg->type == 0xFE)
	{
		gPrintPlayer.ViewMaxMP = lpMsg->ViewMP;

		gPrintPlayer.ViewMaxBP = lpMsg->ViewBP;
	}

	if (lpMsg->type == 0xFF)
	{
		gPrintPlayer.ViewCurMP = lpMsg->ViewMP;

		gPrintPlayer.ViewCurBP = lpMsg->ViewBP;
	}
}

void CProtocol::GCFruitResultRecv(PMSG_FRUIT_RESULT_RECV* lpMsg)
{
	if ((lpMsg->result >> 6) == 0)
	{
		STRUCT_DECRYPT;

		*(WORD*)(CharacterAttribute + 0x14) = GET_MAX_WORD_VALUE(lpMsg->ViewStrength);

		*(WORD*)(CharacterAttribute + 0x16) = GET_MAX_WORD_VALUE(lpMsg->ViewDexterity);

		*(WORD*)(CharacterAttribute + 0x18) = GET_MAX_WORD_VALUE(lpMsg->ViewVitality);

		*(WORD*)(CharacterAttribute + 0x1A) = GET_MAX_WORD_VALUE(lpMsg->ViewEnergy);

		STRUCT_ENCRYPT;

		gPrintPlayer.ViewValue = lpMsg->ViewValue;

		gPrintPlayer.ViewPoint = lpMsg->ViewPoint;

		gPrintPlayer.ViewStrength = lpMsg->ViewStrength;

		gPrintPlayer.ViewDexterity = lpMsg->ViewDexterity;

		gPrintPlayer.ViewVitality = lpMsg->ViewVitality;

		gPrintPlayer.ViewEnergy = lpMsg->ViewEnergy;
	}
}

void CProtocol::GCRewardExperienceRecv(PMSG_REWARD_EXPERIENCE_RECV* lpMsg)
{
	gPrintPlayer.ViewDamageHP = lpMsg->ViewDamageHP;

	gPrintPlayer.ViewExperience = lpMsg->ViewExperience;

	gPrintPlayer.ViewNextExperience = lpMsg->ViewNextExperience;
}

void CProtocol::GCQuestRewardRecv(PMSG_QUEST_REWARD_RECV* lpMsg)
{
	int aIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]) & 0x7FFF;

	if (gPrintPlayer.ViewIndex == aIndex)
	{
		gPrintPlayer.ViewPoint = lpMsg->ViewPoint;
	}
}

void CProtocol::GCCharacterCreationEnableRecv(PMSG_CHARACTER_CREATION_ENABLE_RECV* lpMsg)
{
	SetDword(0x00522929 + 1, lpMsg->result); // Allow Create MG

	SetDword(0x0052428E + 1, lpMsg->result); // Allow Create MG
}

void CProtocol::GCConnectClientRecv(PMSG_CONNECT_CLIENT_RECV* lpMsg)
{
	gPrintPlayer.ViewIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]);

	gLanguage.SendLanguage();

	gHwid.SendHwid();
}

void CProtocol::GCConnectAccountRecv(PMSG_CONNECT_ACCOUNT_RECV* lpMsg)
{
	gReconnect.ReconnectOnConnectAccount(lpMsg->result);
}

void CProtocol::GCCloseClientRecv(PMSG_CLOSE_CLIENT_RECV* lpMsg)
{
	gReconnect.ReconnectOnCloseClient(lpMsg->result);
}

void CProtocol::GCCharacterListRecv(PMSG_CHARACTER_LIST_RECV* lpMsg)
{
	gReconnect.ReconnectOnCharacterList();
}

void CProtocol::GCCharacterInfoRecv(PMSG_CHARACTER_INFO_RECV* lpMsg)
{
	gReconnect.ReconnectOnCharacterInfo();
}

void CProtocol::GCCharacterRegenRecv(PMSG_CHARACTER_REGEN_RECV* lpMsg)
{
	gPrintPlayer.ViewCurHP = lpMsg->ViewCurHP;

	gPrintPlayer.ViewCurMP = lpMsg->ViewCurMP;

	gPrintPlayer.ViewCurBP = lpMsg->ViewCurBP;
}

void CProtocol::GCLevelUpRecv(PMSG_LEVEL_UP_RECV* lpMsg)
{
	gPrintPlayer.ViewPoint = lpMsg->ViewPoint;

	gPrintPlayer.ViewMaxHP = lpMsg->ViewMaxHP;

	gPrintPlayer.ViewMaxMP = lpMsg->ViewMaxMP;

	gPrintPlayer.ViewMaxBP = lpMsg->ViewMaxBP;

	gPrintPlayer.ViewExperience = lpMsg->ViewExperience;

	gPrintPlayer.ViewNextExperience = lpMsg->ViewNextExperience;
}

void CProtocol::GCLevelUpPointRecv(PMSG_LEVEL_UP_POINT_RECV* lpMsg)
{
	if (lpMsg->result >= 16 && lpMsg->result <= 20)
	{
		gPrintPlayer.ViewPoint = lpMsg->ViewPoint;

		gPrintPlayer.ViewMaxHP = lpMsg->ViewMaxHP;

		gPrintPlayer.ViewMaxMP = lpMsg->ViewMaxMP;

		gPrintPlayer.ViewMaxBP = lpMsg->ViewMaxBP;

		gPrintPlayer.ViewStrength = lpMsg->ViewStrength;

		gPrintPlayer.ViewDexterity = lpMsg->ViewDexterity;

		gPrintPlayer.ViewVitality = lpMsg->ViewVitality;

		gPrintPlayer.ViewEnergy = lpMsg->ViewEnergy;
	}
}

void CProtocol::GCMonsterDamageRecv(PMSG_MONSTER_DAMAGE_RECV* lpMsg)
{
	gPrintPlayer.ViewCurHP = lpMsg->ViewCurHP;

	gPrintPlayer.ViewDamageHP = lpMsg->ViewDamageHP;
}

void CProtocol::GCNewCharacterInfoRecv(PMSG_NEW_CHARACTER_INFO_RECV* lpMsg)
{
	STRUCT_DECRYPT;

	*(WORD*)(CharacterAttribute + 0x0E) = GET_MAX_WORD_VALUE(lpMsg->Level);

	*(WORD*)(CharacterAttribute + 0x54) = GET_MAX_WORD_VALUE(lpMsg->LevelUpPoint);

	*(WORD*)(CharacterAttribute + 0x14) = GET_MAX_WORD_VALUE(lpMsg->Strength);

	*(WORD*)(CharacterAttribute + 0x16) = GET_MAX_WORD_VALUE(lpMsg->Dexterity);

	*(WORD*)(CharacterAttribute + 0x18) = GET_MAX_WORD_VALUE(lpMsg->Vitality);

	*(WORD*)(CharacterAttribute + 0x1A) = GET_MAX_WORD_VALUE(lpMsg->Energy);

	STRUCT_ENCRYPT;

	gPrintPlayer.ViewPoint = lpMsg->LevelUpPoint;

	gPrintPlayer.ViewExperience = lpMsg->Experience;

	gPrintPlayer.ViewNextExperience = lpMsg->NextExperience;

	gPrintPlayer.ViewStrength = lpMsg->Strength;

	gPrintPlayer.ViewDexterity = lpMsg->Dexterity;

	gPrintPlayer.ViewVitality = lpMsg->Vitality;

	gPrintPlayer.ViewEnergy = lpMsg->Energy;

	gPrintPlayer.ViewCurHP = lpMsg->Life;

	gPrintPlayer.ViewMaxHP = lpMsg->MaxLife;

	gPrintPlayer.ViewCurMP = lpMsg->Mana;

	gPrintPlayer.ViewMaxMP = lpMsg->MaxMana;

	gPrintPlayer.ViewCurBP = lpMsg->BP;

	gPrintPlayer.ViewMaxBP = lpMsg->MaxBP;

	gPrintPlayer.ViewReset = lpMsg->ViewReset;

	gPrintPlayer.ViewGrandReset = lpMsg->ViewGrandReset;
}

void CProtocol::GCNewCharacterCalcRecv(PMSG_NEW_CHARACTER_CALC_RECV* lpMsg)
{
	STRUCT_DECRYPT;

	*(WORD*)(CharacterAttribute + 0x1C) = GET_MAX_WORD_VALUE(lpMsg->ViewCurHP);

	*(WORD*)(CharacterAttribute + 0x20) = GET_MAX_WORD_VALUE(lpMsg->ViewMaxHP);

	*(WORD*)(CharacterAttribute + 0x1E) = GET_MAX_WORD_VALUE(lpMsg->ViewCurMP);

	*(WORD*)(CharacterAttribute + 0x22) = GET_MAX_WORD_VALUE(lpMsg->ViewMaxMP);

	*(WORD*)(CharacterAttribute + 0x24) = GET_MAX_WORD_VALUE(lpMsg->ViewCurBP);

	*(WORD*)(CharacterAttribute + 0x26) = GET_MAX_WORD_VALUE(lpMsg->ViewMaxBP);

	STRUCT_ENCRYPT;

	gPrintPlayer.ViewCurHP = lpMsg->ViewCurHP;

	gPrintPlayer.ViewMaxHP = lpMsg->ViewMaxHP;

	gPrintPlayer.ViewCurMP = lpMsg->ViewCurMP;

	gPrintPlayer.ViewMaxMP = lpMsg->ViewMaxMP;

	gPrintPlayer.ViewCurBP = lpMsg->ViewCurBP;

	gPrintPlayer.ViewMaxBP = lpMsg->ViewMaxBP;

	gPrintPlayer.ViewPhysiSpeed = lpMsg->ViewPhysiSpeed;

	gPrintPlayer.ViewMagicSpeed = lpMsg->ViewMagicSpeed;
}

void CProtocol::GCHealthBarRecv(PMSG_HEALTH_BAR_RECV* lpMsg)
{
	gHealthBar.ClearHealthBar();

	for (int n = 0; n < lpMsg->count; n++)
	{
		MONSTER_HEALTH_BAR* lpInfo = (MONSTER_HEALTH_BAR*)(((BYTE*)lpMsg) + sizeof(PMSG_HEALTH_BAR_RECV) + (sizeof(MONSTER_HEALTH_BAR) * n));

		gHealthBar.InsertHealthBar(lpInfo->index, lpInfo->type, lpInfo->rateHP);
	}
}

void CProtocol::DataSend(BYTE* lpMsg, DWORD size)
{
	//ConsoleProtocolLog(CON_PROTO_TCP_SEND, lpMsg, size);

	BYTE EncBuff[2048];

	if (gPacketManager.AddData(lpMsg, size) && gPacketManager.ExtractPacket(EncBuff))
	{
		BYTE send[2048];

		memcpy(send, EncBuff, size);

		if (EncBuff[0] == 0xC3 || EncBuff[0] == 0xC4)
		{
			if (EncBuff[0] == 0xC3)
			{
				BYTE save = EncBuff[1];

				PACKET_DECRYPT;

				EncBuff[1] = (*(BYTE*)(g_byPacketSerialSend))++;

				PACKET_ENCRYPT;

				size = gPacketManager.Encrypt(&send[2], &EncBuff[1], (size - 1)) + 2;

				EncBuff[1] = save;

				send[0] = 0xC3;

				send[1] = LOBYTE(size);
			}
			else
			{
				BYTE save = EncBuff[2];

				PACKET_DECRYPT;

				EncBuff[2] = (*(BYTE*)(g_byPacketSerialSend))++;

				PACKET_ENCRYPT;

				size = gPacketManager.Encrypt(&send[3], &EncBuff[2], (size - 2)) + 3;

				EncBuff[2] = save;

				send[0] = 0xC4;

				send[1] = HIBYTE(size);

				send[2] = LOBYTE(size);
			}
		}

		gHackCheck.MySend(pSocket, send, size, 0);
	}
}