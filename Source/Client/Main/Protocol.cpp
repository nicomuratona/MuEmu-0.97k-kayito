#include "stdafx.h"
#include "Protocol.h"
#include "ChaosMix.h"
#include "EventTimer.h"
#include "GoldenArcher.h"
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
	SetCompleteHook(0xE9, 0x00439505, &this->HookProtocol);

	SetByte(0x004E6DA2, 0xEB); // Skip MG Level in Devil Square
}

_declspec(naked) void CProtocol::HookProtocol()
{
	static DWORD jmpCurrentPacket = 0x0043951F;
	static DWORD jmpNextPacket = 0x004389C3;

	static BYTE* lpMsg;
	static DWORD Head;

	_asm
	{
		Mov Edx, Dword Ptr Ds : [0x07E11DCC] ; // TotalPacketSize
		Add Edx, Eax;
		Mov Dword Ptr Ds : [0x07E11DCC] , Edx; // TotalPacketSize += Size;
		Pushad;
		Mov lpMsg, Ebp;
		Mov Head, Esi;
	}

	if (gProtocol.ProtocolCompiler(lpMsg))
	{
		goto NEXT_PACKET;
	}

	if (Head > 0xF4)
	{
		goto NEXT_PACKET;
	}

	_asm
	{
		Popad;
		Jmp[jmpCurrentPacket];
	}

NEXT_PACKET:

	_asm
	{
		Popad;
		Jmp[jmpNextPacket];
	}
}

bool CProtocol::ProtocolCompiler(BYTE* lpMsg)
{
	int size = 0;

	BYTE header, head;

	if (lpMsg[0] == 0xC1 || lpMsg[0] == 0xC3)
	{
		header = lpMsg[0];

		size = lpMsg[1];

		head = lpMsg[2];
	}
	else if (lpMsg[0] == 0xC2 || lpMsg[0] == 0xC4)
	{
		header = lpMsg[0];

		size = MAKEWORD(lpMsg[2], lpMsg[1]);

		head = lpMsg[3];
	}
	else
	{
		char lpszMessage[128];

		wsprintf(lpszMessage, "[Connection] Protocol header error (Header: %02X)\r\n", lpMsg[0]);

		MessageBox(NULL, lpszMessage, "Error", MB_OK);

		return false;
	}

	return this->TranslateProtocol(head, lpMsg, size);
}

bool CProtocol::TranslateProtocol(BYTE head, BYTE* lpMsg, int Size)
{
	//ConsoleProtocolLog(CON_PROTO_TCP_RECV, lpMsg, Size);

	switch (head)
	{
		case 0x0D:
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

		case 0x22:
		{
			this->CGItemGetRecv((PMSG_ITEM_GET_RECV*)lpMsg);

			return true;
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

		case 0x8E:
		{
			this->GCDevilSquareRequiredLevelsRecv((PMSG_DEVIL_SQUARE_REQ_LEVELS_RECV*)lpMsg);

			return true;
		}

		case 0x8F:
		{
			this->GCBloodCastleRequiredLevelsRecv((PMSG_BLOOD_CASTLE_REQ_LEVELS_RECV*)lpMsg);

			return true;
		}

		case 0x94:
		{
			gGoldenArcher.GCGoldenArcherRecv((PMSG_NPC_GOLDEN_ARCHER_RECV*)lpMsg);

			return true;
		}

		case 0x95:
		{
			gGoldenArcher.GCGoldenArcherRegisterRecv((PMSG_GOLDEN_ARCHER_REGISTER_RECV*)lpMsg);

			return true;
		}

		case 0x96:
		{
			gGoldenArcher.GCGoldenArcherLuckyNumberRecv((PMSG_GOLDEN_ARCHER_LUCKY_NUMBER_RECV*)lpMsg);

			return true;
		}

		case 0x97:
		{
			switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
			{
				case 0x00:
				{
					gGoldenArcher.GCGoldenArcherInfoRecv((PMSG_GOLDEN_ARCHER_INFO_RECV*)lpMsg);

					return true;
				}

				case 0x01:
				{
					gGoldenArcher.GCGoldenArcherListRecv((PMSG_GOLDEN_ARCHER_LIST_RECV*)lpMsg);

					return true;
				}

				case 0x03:
				{
					gGoldenArcher.GCGoldenArcherRegisterLuckyRecv((PMSG_GOLDEN_ARCHER_REGISTER_LUCKY_RECV*)lpMsg);

					return true;
				}

				case 0x04:
				{
					gGoldenArcher.GoldenArcherCloseProc();

					return true;
				}
			}

			break;
		}

		case 0x9C:
		{
			this->GCRewardExperienceRecv((PMSG_REWARD_EXPERIENCE_RECV*)lpMsg);

			break;
		}

		case 0x9D:
		{
			gGoldenArcher.GCGoldenArcherExchangeLuckyRecv((PMSG_GOLDEN_ARCHER_EXCHANGE_LUCKY_RECV*)lpMsg);

			return true;
		}

		case 0xA3:
		{
			this->GCQuestRewardRecv((PMSG_QUEST_REWARD_RECV*)lpMsg);

			break;
		}

		case 0xDD:
		{
			this->GCCharacterDeleteMaxLevelRecv((PMSG_CHARACTER_DELETE_LEVEL_RECV*)lpMsg);

			return true;
		}

		case 0xDE:
		{
			this->GCCharacterCreationEnableRecv((PMSG_CHARACTER_CREATION_ENABLE_RECV*)lpMsg);

			return true;
		}

		case 0xDF:
		{
			this->GCCharacterMaxLevelRecv((PMSG_CHARACTER_MAX_LEVEL_RECV*)lpMsg);

			return true;
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

					return true;
				}

				case 0xE1:
				{
					this->GCNewCharacterCalcRecv((PMSG_NEW_CHARACTER_CALC_RECV*)lpMsg);

					return true;
				}

				case 0xE2:
				{
					this->GCHealthBarRecv((PMSG_HEALTH_BAR_RECV*)lpMsg);

					return true;
				}

				case 0xE3:
				{
					gItemStack.GCItemStackListRecv((PMSG_ITEM_STACK_LIST_RECV*)lpMsg);

					return true;
				}

				case 0xE4:
				{
					gItemValue.GCItemValueListRecv((PMSG_ITEM_VALUE_LIST_RECV*)lpMsg);

					return true;
				}

				case 0xE5:
				{
					gMoveList.GCMoveListRecv((PMSG_MOVE_LIST_RECV*)lpMsg);

					return true;
				}

				case 0xE6:
				{
					gEventTimer.GCEventTimeRecv((PMSG_EVENT_TIME_RECV*)lpMsg);

					return true;
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

					return true;
				}
			}

			break;
		}
	}

	return false;
}

void CProtocol::GCNoticeRecv(PMSG_NOTICE_RECV* lpMsg)
{
	if (lpMsg->type == 3)
	{
		char text[64] = { 0 };

		sprintf_s(text, GlobalText[486], atoi(lpMsg->message));

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
		    || Type == GET_ITEM(12, 15) // Jewel of Chaos
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

void CProtocol::GCDevilSquareRequiredLevelsRecv(PMSG_DEVIL_SQUARE_REQ_LEVELS_RECV* lpMsg)
{
	for (int Level = 0; Level < 4; Level++)
	{
		// Normal Level
		m_iDevilSquareLimitLevel[Level][0] = lpMsg->m_DevilSquareRequiredLevel[Level][0];
		m_iDevilSquareLimitLevel[Level][1] = lpMsg->m_DevilSquareRequiredLevel[Level][1];
	}
}

void CProtocol::GCBloodCastleRequiredLevelsRecv(PMSG_BLOOD_CASTLE_REQ_LEVELS_RECV* lpMsg)
{
	for (int Level = 0; Level < 6; Level++)
	{
		// Normal Level
		m_iBloodCastleLimitLevel[Level][0] = lpMsg->m_BloodCastleRequiredLevel[Level][0];
		m_iBloodCastleLimitLevel[Level][1] = lpMsg->m_BloodCastleRequiredLevel[Level][1];
		
		// MG Level
		m_iBloodCastleLimitLevel[Level + 6][0] = lpMsg->m_BloodCastleRequiredLevel[Level][2];
		m_iBloodCastleLimitLevel[Level + 6][1] = lpMsg->m_BloodCastleRequiredLevel[Level][3];
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

void CProtocol::GCCharacterDeleteMaxLevelRecv(PMSG_CHARACTER_DELETE_LEVEL_RECV* lpMsg)
{
	gPrintPlayer.MaxCharacterDeleteLevel = lpMsg->Level;
}

void CProtocol::GCCharacterCreationEnableRecv(PMSG_CHARACTER_CREATION_ENABLE_RECV* lpMsg)
{
	SetDword(0x00522929 + 1, lpMsg->result); // Allow Create MG

	SetDword(0x0052428E + 1, lpMsg->result); // Allow Create MG
}

void CProtocol::GCCharacterMaxLevelRecv(PMSG_CHARACTER_MAX_LEVEL_RECV* lpMsg)
{
	gPrintPlayer.MaxCharacterLevel = lpMsg->MaxCharacterLevel;
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

	*(WORD*)(CharacterAttribute + 0x38) = GET_MAX_WORD_VALUE(lpMsg->ViewPhysiSpeed);

	*(WORD*)(CharacterAttribute + 0x44) = GET_MAX_WORD_VALUE(lpMsg->ViewMagicSpeed);

	*(WORD*)(CharacterAttribute + 0x3A) = GET_MAX_WORD_VALUE(lpMsg->ViewAttackSuccessRate);

	*(WORD*)(CharacterAttribute + 0x4E) = GET_MAX_WORD_VALUE(lpMsg->ViewDefense);

	*(WORD*)(CharacterAttribute + 0x4C) = GET_MAX_WORD_VALUE(lpMsg->ViewDefenseSuccessRate);

	*(WORD*)(CharacterAttribute + 0x46) = GET_MAX_WORD_VALUE(lpMsg->ViewMagicDamageMin);

	*(WORD*)(CharacterAttribute + 0x48) = GET_MAX_WORD_VALUE(lpMsg->ViewMagicDamageMax);

	STRUCT_ENCRYPT;

	gPrintPlayer.ViewCurHP = lpMsg->ViewCurHP;

	gPrintPlayer.ViewMaxHP = lpMsg->ViewMaxHP;

	gPrintPlayer.ViewCurMP = lpMsg->ViewCurMP;

	gPrintPlayer.ViewMaxMP = lpMsg->ViewMaxMP;

	gPrintPlayer.ViewCurBP = lpMsg->ViewCurBP;

	gPrintPlayer.ViewMaxBP = lpMsg->ViewMaxBP;

	gPrintPlayer.ViewPhysiSpeed = lpMsg->ViewPhysiSpeed;

	gPrintPlayer.ViewMagicSpeed = lpMsg->ViewMagicSpeed;

	gPrintPlayer.ViewPhysiDamageMin = lpMsg->ViewPhysiDamageMin;

	gPrintPlayer.ViewPhysiDamageMax = lpMsg->ViewPhysiDamageMax;

	gPrintPlayer.ViewMagicDamageMin = lpMsg->ViewMagicDamageMin;

	gPrintPlayer.ViewMagicDamageMax = lpMsg->ViewMagicDamageMax;

	gPrintPlayer.ViewMagicDamageRate = lpMsg->ViewMagicDamageRate;

	gPrintPlayer.ViewAttackSuccessRate = lpMsg->ViewAttackSuccessRate;

	gPrintPlayer.ViewDamageMultiplier = lpMsg->ViewDamageMultiplier;

	gPrintPlayer.ViewDefense = lpMsg->ViewDefense;

	gPrintPlayer.ViewDefenseSuccessRate = lpMsg->ViewDefenseSuccessRate;
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

				PACKET_DECRYPT(&g_byPacketSerialSend);

				EncBuff[1] = (g_byPacketSerialSend)++;

				PACKET_ENCRYPT(&g_byPacketSerialSend);

				size = gPacketManager.Encrypt(&send[2], &EncBuff[1], (size - 1)) + 2;

				EncBuff[1] = save;

				send[0] = 0xC3;

				send[1] = LOBYTE(size);
			}
			else
			{
				BYTE save = EncBuff[2];

				PACKET_DECRYPT(&g_byPacketSerialSend);

				EncBuff[2] = (g_byPacketSerialSend)++;

				PACKET_ENCRYPT(&g_byPacketSerialSend);

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

void CProtocol::CGLiveClientSend()
{
	PMSG_LIVE_CLIENT_SEND pMsg;

	pMsg.header.setE(0x0E, sizeof(pMsg));

	pMsg.TickCount = GetTickCount();

	STRUCT_DECRYPT;

	if (*(BYTE*)(CharacterAttribute + 0x28) & 1)
	{
		pMsg.PhysiSpeed = *(WORD*)(CharacterAttribute + 0x38) - 20;

		pMsg.MagicSpeed = *(WORD*)(CharacterAttribute + 0x44) - 20;
	}
	else if (*(BYTE*)(CharacterAttribute + 0x28) & 8)
	{
		pMsg.PhysiSpeed = *(WORD*)(CharacterAttribute + 0x38) - 20;

		pMsg.MagicSpeed = *(WORD*)(CharacterAttribute + 0x44) - 20;
	}
	else
	{
		pMsg.PhysiSpeed = *(WORD*)(CharacterAttribute + 0x38);

		pMsg.MagicSpeed = *(WORD*)(CharacterAttribute + 0x44);
	}

	STRUCT_ENCRYPT;

	this->DataSend((BYTE*)&pMsg, pMsg.header.size);
}