#include "stdafx.h"
#include "Protocol.h"
#include "ChaosMix.h"
#include "HackCheck.h"
#include "HealthBar.h"
#include "HWID.h"
#include "ItemStack.h"
#include "ItemValue.h"
#include "Language.h"
#include "MoveList.h"
#include "PacketManager.h"
#include "PrintPlayer.h"
#include "Protect.h"
#include "Reconnect.h"
#include "ServerList.h"

void InitProtocol()
{
	SetCompleteHook(0xE9, 0x004389D7, &HookProtocol);
}

_declspec(naked) void HookProtocol()
{
	static DWORD jmpBack = 0x004389DC;

	_asm
	{
		Pushad;
		Push Ebp;
		Call ProtocolCompiler;
		Popad;
		Mov Al, [Ebp + 0];
		Xor Ebx, Ebx;
		Jmp[jmpBack];
	}
}

void _stdcall ProtocolCompiler(BYTE* lpMsg)
{
	int count = 0, size = 0, DecSize = 0;

	BYTE header, head;

	BYTE DecBuff[MAX_MAIN_PACKET_SIZE];

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

	TranslateProtocol(head, DecBuff, DecSize);
}

void TranslateProtocol(BYTE head, BYTE* lpMsg, int Size)
{
	//gConsole.Write("RECV 0: %02X, 1: %02X, 2: %02X, 3: %02X, 4: %02X, 5: %02X", (Size > 0) ? lpMsg[0] : 0, (Size > 1) ? lpMsg[1] : 0, (Size > 2) ? lpMsg[2] : 0, (Size > 3) ? lpMsg[3] : 0, (Size > 4) ? lpMsg[4] : 0, (Size > 5) ? lpMsg[5] : 0);

	switch (head)
	{
		case 0xD:
		{
			GCNoticeRecv((PMSG_NOTICE_RECV*)lpMsg);

			break;
		}

		case 0x15:
		{
			GCDamageRecv((PMSG_DAMAGE_RECV*)lpMsg);

			break;
		}

		case 0x17:
		{
			GCUserDieRecv((PMSG_USER_DIE_RECV*)lpMsg);

			break;
		}

		case 0x24:
		{
			gChaosMix.Clear();

			break;
		}

		case 0x26:
		{
			GCLifeRecv((PMSG_LIFE_RECV*)lpMsg);

			break;
		}

		case 0x27:
		{
			GCManaRecv((PMSG_MANA_RECV*)lpMsg);

			break;
		}

		case 0x2C:
		{
			GCFruitResultRecv((PMSG_FRUIT_RESULT_RECV*)lpMsg);

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
			GCRewardExperienceRecv((PMSG_REWARD_EXPERIENCE_RECV*)lpMsg);

			break;
		}

		case 0xA3:
		{
			GCQuestRewardRecv((PMSG_QUEST_REWARD_RECV*)lpMsg);

			break;
		}

		case 0xDE:
		{
			GCCharacterCreationEnableRecv((PMSG_CHARACTER_CREATION_ENABLE_RECV*)lpMsg);

			break;
		}

		case 0xF1:
		{
			switch (((lpMsg[0] == 0xC1) ? lpMsg[3] : lpMsg[4]))
			{
				case 0x00:
				{
					GCConnectClientRecv((PMSG_CONNECT_CLIENT_RECV*)lpMsg);

					break;
				}

				case 0x01:
				{
					GCConnectAccountRecv((PMSG_CONNECT_ACCOUNT_RECV*)lpMsg);

					break;
				}

				case 0x02:
				{
					GCCloseClientRecv((PMSG_CLOSE_CLIENT_RECV*)lpMsg);

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
					GCCharacterListRecv((PMSG_CHARACTER_LIST_RECV*)lpMsg);

					break;
				}

				case 0x03:
				{
					GCCharacterInfoRecv((PMSG_CHARACTER_INFO_RECV*)lpMsg);

					break;
				}

				case 0x04:
				{
					GCCharacterRegenRecv((PMSG_CHARACTER_REGEN_RECV*)lpMsg);

					break;
				}

				case 0x05:
				{
					GCLevelUpRecv((PMSG_LEVEL_UP_RECV*)lpMsg);

					break;
				}

				case 0x06:
				{
					GCLevelUpPointRecv((PMSG_LEVEL_UP_POINT_RECV*)lpMsg);

					break;
				}

				case 0x07:
				{
					GCMonsterDamageRecv((PMSG_MONSTER_DAMAGE_RECV*)lpMsg);

					break;
				}

				case 0xE0:
				{
					GCNewCharacterInfoRecv((PMSG_NEW_CHARACTER_INFO_RECV*)lpMsg);

					break;
				}

				case 0xE1:
				{
					GCNewCharacterCalcRecv((PMSG_NEW_CHARACTER_CALC_RECV*)lpMsg);

					break;
				}

				case 0xE2:
				{
					GCHealthBarRecv((PMSG_HEALTH_BAR_RECV*)lpMsg);

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

void GCNoticeRecv(PMSG_NOTICE_RECV* lpMsg)
{
	if (lpMsg->type == 3)
	{
		char text[64] = { 0 };

		sprintf_s(text, GetTextLine(486), atoi(lpMsg->message));

		((void(_cdecl*)(char* strID, char* strText, int MsgType))0x00480620)((char*)0x005826D3C, text, 1);
	}
}

void GCDamageRecv(PMSG_DAMAGE_RECV* lpMsg)
{
	int aIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]) & 0x7FFF;

	if (ViewIndex == aIndex)
	{
		ViewCurHP = lpMsg->ViewCurHP;
	}

	ViewDamageHP = lpMsg->ViewDamageHP;
}

void GCUserDieRecv(PMSG_USER_DIE_RECV* lpMsg)
{
	int aIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]) & 0x7FFF;

	if (ViewIndex == aIndex)
	{
		ViewCurHP = 0;
	}
}

void GCLifeRecv(PMSG_LIFE_RECV* lpMsg)
{
	if (lpMsg->type == 0xFE)
	{
		ViewMaxHP = lpMsg->ViewHP;
	}

	if (lpMsg->type == 0xFF)
	{
		ViewCurHP = ((ViewCurHP == 0) ? ViewCurHP : lpMsg->ViewHP);
	}
}

void GCManaRecv(PMSG_MANA_RECV* lpMsg)
{
	if (lpMsg->type == 0xFE)
	{
		ViewMaxMP = lpMsg->ViewMP;

		ViewMaxBP = lpMsg->ViewBP;
	}

	if (lpMsg->type == 0xFF)
	{
		ViewCurMP = lpMsg->ViewMP;

		ViewCurBP = lpMsg->ViewBP;
	}
}

void GCFruitResultRecv(PMSG_FRUIT_RESULT_RECV* lpMsg)
{
	if ((lpMsg->result >> 6) == 0)
	{
		STRUCT_DECRYPT;

		*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x14) = GET_MAX_WORD_VALUE(lpMsg->ViewStrength);

		*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x16) = GET_MAX_WORD_VALUE(lpMsg->ViewDexterity);

		*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x18) = GET_MAX_WORD_VALUE(lpMsg->ViewVitality);

		*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x1A) = GET_MAX_WORD_VALUE(lpMsg->ViewEnergy);

		STRUCT_ENCRYPT;

		ViewValue = lpMsg->ViewValue;

		ViewPoint = lpMsg->ViewPoint;

		ViewStrength = lpMsg->ViewStrength;

		ViewDexterity = lpMsg->ViewDexterity;

		ViewVitality = lpMsg->ViewVitality;

		ViewEnergy = lpMsg->ViewEnergy;
	}
}

void GCRewardExperienceRecv(PMSG_REWARD_EXPERIENCE_RECV* lpMsg)
{
	ViewDamageHP = lpMsg->ViewDamageHP;

	ViewExperience = lpMsg->ViewExperience;

	ViewNextExperience = lpMsg->ViewNextExperience;
}

void GCQuestRewardRecv(PMSG_QUEST_REWARD_RECV* lpMsg)
{
	int aIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]) & 0x7FFF;

	if (ViewIndex == aIndex)
	{
		ViewPoint = lpMsg->ViewPoint;
	}
}

void GCCharacterCreationEnableRecv(PMSG_CHARACTER_CREATION_ENABLE_RECV* lpMsg)
{
	SetDword(0x00522929 + 1, lpMsg->result); // Allow Create MG

	SetDword(0x0052428E + 1, lpMsg->result); // Allow Create MG
}

void GCConnectClientRecv(PMSG_CONNECT_CLIENT_RECV* lpMsg)
{
	ViewIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]);

	gLanguage.SendLanguage();

	gHwid.SendHwid();
}

void GCConnectAccountRecv(PMSG_CONNECT_ACCOUNT_RECV* lpMsg)
{
	ReconnectOnConnectAccount(lpMsg->result);
}

void GCCloseClientRecv(PMSG_CLOSE_CLIENT_RECV* lpMsg)
{
	ReconnectOnCloseClient(lpMsg->result);
}

void GCCharacterListRecv(PMSG_CHARACTER_LIST_RECV* lpMsg)
{
	ReconnectOnCharacterList();
}

void GCCharacterInfoRecv(PMSG_CHARACTER_INFO_RECV* lpMsg)
{
	ReconnectOnCharacterInfo();

	ViewExperience = lpMsg->Experience;

	ViewNextExperience = lpMsg->NextExperience;

	ViewReset = lpMsg->ViewReset;

	ViewGrandReset = lpMsg->ViewGrandReset;

	ViewPoint = lpMsg->ViewPoint;

	ViewCurHP = lpMsg->ViewCurHP;

	ViewMaxHP = lpMsg->ViewMaxHP;

	ViewCurMP = lpMsg->ViewCurMP;

	ViewMaxMP = lpMsg->ViewMaxMP;

	ViewCurBP = lpMsg->ViewCurBP;

	ViewMaxBP = lpMsg->ViewMaxBP;

	ViewStrength = lpMsg->ViewStrength;

	ViewDexterity = lpMsg->ViewDexterity;

	ViewVitality = lpMsg->ViewVitality;

	ViewEnergy = lpMsg->ViewEnergy;

	ViewExperience = lpMsg->Experience;

	ViewNextExperience = lpMsg->NextExperience;

	*(WORD*)(Hero + 0x1DC) = 0;

	*(BYTE*)(Hero + 0x00) = 0;

	STRUCT_DECRYPT;

	switch (*(BYTE*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x0B) & 7)
	{
		case 0:
		{
			SetByte(0x0044895B, ((gProtect.m_MainInfo.DWMaxAttackSpeed >= 0xFFFF) ? 0x02 : 0x0F));

			break;
		}

		case 1:
		{
			SetByte(0x0044895B, ((gProtect.m_MainInfo.DKMaxAttackSpeed >= 0xFFFF) ? 0x0F : 0x0F));

			break;
		}

		case 2:
		{
			SetByte(0x0044895B, ((gProtect.m_MainInfo.FEMaxAttackSpeed >= 0xFFFF) ? 0x02 : 0x0F));

			break;
		}

		case 3:
		{
			SetByte(0x0044895B, ((gProtect.m_MainInfo.MGMaxAttackSpeed >= 0xFFFF) ? 0x02 : 0x0F));

			break;
		}
	}

	STRUCT_ENCRYPT;
}

void GCCharacterRegenRecv(PMSG_CHARACTER_REGEN_RECV* lpMsg)
{
	ViewCurHP = lpMsg->ViewCurHP;

	ViewCurMP = lpMsg->ViewCurMP;

	ViewCurBP = lpMsg->ViewCurBP;
}

void GCLevelUpRecv(PMSG_LEVEL_UP_RECV* lpMsg)
{
	ViewPoint = lpMsg->ViewPoint;

	ViewMaxHP = lpMsg->ViewMaxHP;

	ViewMaxMP = lpMsg->ViewMaxMP;

	ViewMaxBP = lpMsg->ViewMaxBP;

	ViewExperience = lpMsg->ViewExperience;

	ViewNextExperience = lpMsg->ViewNextExperience;
}

void GCLevelUpPointRecv(PMSG_LEVEL_UP_POINT_RECV* lpMsg)
{
	if (lpMsg->result >= 16 && lpMsg->result <= 20)
	{
		ViewPoint = lpMsg->ViewPoint;

		ViewMaxHP = lpMsg->ViewMaxHP;

		ViewMaxMP = lpMsg->ViewMaxMP;

		ViewMaxBP = lpMsg->ViewMaxBP;

		ViewStrength = lpMsg->ViewStrength;

		ViewDexterity = lpMsg->ViewDexterity;

		ViewVitality = lpMsg->ViewVitality;

		ViewEnergy = lpMsg->ViewEnergy;
	}
}

void GCMonsterDamageRecv(PMSG_MONSTER_DAMAGE_RECV* lpMsg)
{
	ViewCurHP = lpMsg->ViewCurHP;

	ViewDamageHP = lpMsg->ViewDamageHP;
}

void GCNewCharacterInfoRecv(PMSG_NEW_CHARACTER_INFO_RECV* lpMsg)
{
	STRUCT_DECRYPT;

	*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x54) = GET_MAX_WORD_VALUE(lpMsg->LevelUpPoint + 1);

	*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x14) = GET_MAX_WORD_VALUE(lpMsg->Strength);

	*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x16) = GET_MAX_WORD_VALUE(lpMsg->Dexterity);

	*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x18) = GET_MAX_WORD_VALUE(lpMsg->Vitality);

	*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x1A) = GET_MAX_WORD_VALUE(lpMsg->Energy);

	STRUCT_ENCRYPT;

	ViewPoint = lpMsg->LevelUpPoint;

	ViewExperience = lpMsg->Experience;

	ViewNextExperience = lpMsg->NextExperience;

	ViewStrength = lpMsg->Strength;

	ViewDexterity = lpMsg->Dexterity;

	ViewVitality = lpMsg->Vitality;

	ViewEnergy = lpMsg->Energy;

	ViewCurHP = lpMsg->Life;

	ViewMaxHP = lpMsg->MaxLife;

	ViewCurMP = lpMsg->Mana;

	ViewMaxMP = lpMsg->MaxMana;

	ViewCurBP = lpMsg->BP;

	ViewMaxBP = lpMsg->MaxBP;

	ViewReset = lpMsg->ViewReset;

	ViewGrandReset = lpMsg->ViewGrandReset;
}

void GCNewCharacterCalcRecv(PMSG_NEW_CHARACTER_CALC_RECV* lpMsg)
{
	STRUCT_DECRYPT;

	*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x1C) = GET_MAX_WORD_VALUE(lpMsg->ViewCurHP);

	*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x20) = GET_MAX_WORD_VALUE(lpMsg->ViewMaxHP);

	*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x1E) = GET_MAX_WORD_VALUE(lpMsg->ViewCurMP);

	*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x22) = GET_MAX_WORD_VALUE(lpMsg->ViewMaxMP);

	*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x24) = GET_MAX_WORD_VALUE(lpMsg->ViewCurBP);

	*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x26) = GET_MAX_WORD_VALUE(lpMsg->ViewMaxBP);

	STRUCT_ENCRYPT;

	ViewCurHP = lpMsg->ViewCurHP;

	ViewMaxHP = lpMsg->ViewMaxHP;

	ViewCurMP = lpMsg->ViewCurMP;

	ViewMaxMP = lpMsg->ViewMaxMP;

	ViewCurBP = lpMsg->ViewCurBP;

	ViewMaxBP = lpMsg->ViewMaxBP;

	ViewAddStrength = lpMsg->ViewAddStrength;

	ViewAddDexterity = lpMsg->ViewAddDexterity;

	ViewAddVitality = lpMsg->ViewAddVitality;

	ViewAddEnergy = lpMsg->ViewAddEnergy;

	ViewPhysiSpeed = lpMsg->ViewPhysiSpeed;

	ViewMagicSpeed = lpMsg->ViewMagicSpeed;
}

void GCHealthBarRecv(PMSG_HEALTH_BAR_RECV* lpMsg)
{
	gHealthBar.ClearHealthBar();

	for (int n = 0; n < lpMsg->count; n++)
	{
		MONSTER_HEALTH_BAR* lpInfo = (MONSTER_HEALTH_BAR*)(((BYTE*)lpMsg) + sizeof(PMSG_HEALTH_BAR_RECV) + (sizeof(MONSTER_HEALTH_BAR) * n));

		gHealthBar.InsertHealthBar(lpInfo->index, lpInfo->type, lpInfo->rateHP);
	}
}

void DataSend(BYTE* lpMsg, DWORD size)
{
	BYTE EncBuff[2048];

	if (gPacketManager.AddData(lpMsg, size) != false && gPacketManager.ExtractPacket(EncBuff) != false)
	{
		BYTE send[2048];

		memcpy(send, EncBuff, size);

		if (EncBuff[0] == 0xC3 || EncBuff[0] == 0xC4)
		{
			if (EncBuff[0] == 0xC3)
			{
				BYTE save = EncBuff[1];

				PACKET_DECRYPT;

				EncBuff[1] = (*(BYTE*)(MAIN_PACKET_SERIAL))++;

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

				EncBuff[2] = (*(BYTE*)(MAIN_PACKET_SERIAL))++;

				PACKET_ENCRYPT;

				size = gPacketManager.Encrypt(&send[3], &EncBuff[2], (size - 2)) + 3;

				EncBuff[2] = save;

				send[0] = 0xC4;

				send[1] = HIBYTE(size);

				send[2] = LOBYTE(size);
			}
		}

		MySend(pSocket, send, size, 0);
	}
}