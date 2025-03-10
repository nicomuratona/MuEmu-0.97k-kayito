#include "stdafx.h"
#include "User.h"
#include "Attack.h"
#include "BattleSoccerManager.h"
#include "BloodCastle.h"
#include "BonusManager.h"
#include "ChaosBox.h"
#include "CommandManager.h"
#include "CustomPkFree.h"
#include "DefaultClassInfo.h"
#include "DevilSquare.h"
#include "DSProtocol.h"
#include "EffectManager.h"
#include "EventTimeManager.h"
#include "FlyingDragons.h"
#include "GameMain.h"
#include "GameMaster.h"
#include "Gate.h"
#include "GoldenArcherBingo.h"
#include "Guild.h"
#include "HackPacketCheck.h"
#include "InvasionManager.h"
#include "IpManager.h"
#include "JSProtocol.h"
#include "Log.h"
#include "Map.h"
#include "MapManager.h"
#include "MemoryAllocator.h"
#include "Message.h"
#include "Monster.h"
#include "Move.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "Party.h"
#include "Quest.h"
#include "Reconnect.h"
#include "SerialCheck.h"
#include "ServerInfo.h"
#include "SkillManager.h"
#include "Trade.h"
#include "Util.h"
#include "Viewport.h"
#include "Warehouse.h"

int gObjCount;

int gObjMonCount;

int gObjCallMonCount;

int gObjTotalUser;

int gObjTotalMonster;

int gCloseMsg;

int gCloseMsgTime;

int gGameServerLogOut;

int gGameServerDisconnect;

OBJECTSTRUCT_HEADER gObj;

MESSAGE_STATE_MACHINE_HEADER gSMMsg;

MESSAGE_STATE_ATTACK_MACHINE_HEADER gSMAttackProcMsg;

DWORD gCheckSum[MAX_CHECKSUM_KEY];

DWORD gLevelExperience[MAX_CHARACTER_LEVEL + 1];

//**************************************************************************//
// OBJECT MAIN FUNCTIONS ***************************************************//
//**************************************************************************//

void gObjEventRunProc()
{
	gBloodCastle.MainProc();

	gBonusManager.MainProc();

	gDevilSquare.MainProc();

	gInvasionManager.MainProc();

	gFlyingDragons.FlyingDragonsDelete();

	gGoldenArcherBingo.MainProc();
}

void gObjViewportProc()
{
	for (int n = 0; n < MAX_OBJECT; n++)
	{
		gObjectManager.ObjectSetStateCreate(n);
	}

	for (int n = 0; n < MAX_OBJECT; n++)
	{
		gObjViewportListDestroy(n);
	}

	for (int n = 0; n < MAX_OBJECT; n++)
	{
		gObjViewportListCreate(n);
	}

	for (int n = 0; n < MAX_OBJECT; n++)
	{
		gObjViewportListProtocol(n);
	}

	gObjectManager.ObjectSetStateProc();
}

void gObjFirstProc()
{
	for (int n = 0; n < MAX_MAP; n++)
	{
		gMap[n].WeatherVariationProcess();
	}

	gObjSecondProc();

	gCommandManager.MainProc();

	gEffectManager.MainProc();

	gNotice.MainProc();

	gReconnect.MainProc();
}

void gObjCloseProc()
{
	if (gCloseMsg != 0)
	{
		if ((--gCloseMsgTime) <= 1)
		{
			if (gCloseMsgTime == 1)
			{
				gNotice.GCNoticeSendToAll(0, 48);

				LogAdd(LOG_RED, gMessage.GetTextMessage(48, 0));
			}
		}
		else
		{
			if ((gCloseMsgTime % 10) == 0)
			{
				gNotice.GCNoticeSendToAll(0, 49, gCloseMsgTime);

				LogAdd(LOG_RED, gMessage.GetTextMessage(49, 0), gCloseMsgTime);
			}
		}

		if (gCloseMsgTime < 0)
		{
			gCloseMsg = 0;

			gCloseMsgTime = 0;

			gObjAllLogOut();
		}
	}
}

void gObjAccountLevelProc()
{
	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnectedGP(n) != 0)
		{
			GJAccountLevelSend(n);
		}
	}
}

//**************************************************************************//
// OBJECT BASE FUNCTIONS ***************************************************//
//**************************************************************************//

void gObjInit()
{
	gObjTotalUser = 0;

	gObjTotalMonster = 0;

	gServerDisplayer.SetWindowName();

	gObjCount = OBJECT_START_USER;

	gObjMonCount = OBJECT_START_MONSTER;

	gObjCallMonCount = MAX_OBJECT_MONSTER;

	memset(gObj.CommonStruct, 0, sizeof(OBJECTSTRUCT));

	gMemoryAllocator.m_TempMemoryAllocatorInfo.Alloc();

	gMemoryAllocator.BindMemoryAllocatorInfo(OBJECT_START_USER, gMemoryAllocator.m_TempMemoryAllocatorInfo);
}

void gObjAllLogOut()
{
	gGameServerLogOut = 1;

	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObj[n].Connected != OBJECT_OFFLINE && gObj[n].Type == OBJECT_USER)
		{
			CloseClient(n);
		}
	}
}

void gObjAllDisconnect()
{
	gGameServerDisconnect = 1;

	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObj[n].Connected != OBJECT_OFFLINE && gObj[n].Type == OBJECT_USER)
		{
			CloseClient(n);
		}
	}
}

void gObjSetExperienceTable()
{
	memset(gLevelExperience, 0, sizeof(gLevelExperience));

	// Parámetros ajustables
	double maxExperience = DWORD_MAX * 0.95; // Aproximar al máximo seguro

	// Ajustar el multiplicador dinámicamente
	double scaleFactor = maxExperience / pow(gServerInfo.m_MaxCharacterLevel, 3);

	for (int level = 1; level <= gServerInfo.m_MaxCharacterLevel; level++)
	{
		gLevelExperience[level] = (DWORD)(scaleFactor * pow(level, 3));
	}
}

void gObjCharZeroSet(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	lpObj->CloseCount = -1;

	lpObj->CloseType = -1;

	lpObj->EnableDelCharacter = 1;

	lpObj->TimeCount = 0;

	lpObj->PKTickCount = 0;

	lpObj->CheckSumTableNum = -1;

	lpObj->CheckSumTime = 0;

	lpObj->Live = 0;

	memset(lpObj->Name, 0, sizeof(lpObj->Name));

	lpObj->Level = 0;

	lpObj->LevelUpPoint = 0;

	lpObj->FruitAddPoint = 0;

	lpObj->FruitSubPoint = 0;

	lpObj->Money = 0;

	lpObj->Strength = 0;

	lpObj->Dexterity = 0;

	lpObj->Vitality = 0;

	lpObj->Energy = 0;

	lpObj->Life = 0;

	lpObj->MaxLife = 0;

	lpObj->AddLife = 0;

	lpObj->ScriptMaxLife = 0;

	lpObj->Mana = 0;

	lpObj->MaxMana = 0;

	lpObj->AddMana = 0;

	lpObj->BP;

	lpObj->MaxBP = 0;

	lpObj->AddBP = 0;

	lpObj->ChatLimitTime = 0;

	lpObj->ChatLimitTimeSec = 0;

	lpObj->PKCount = 0;

	lpObj->PKLevel = 0;

	lpObj->PKTime = GetTickCount();

	lpObj->X = 0;

	lpObj->Y = 0;

	lpObj->TX = 0;

	lpObj->TY = 0;

	lpObj->MTX = 0;

	lpObj->MTY = 0;

	lpObj->OldX = 0;

	lpObj->OldY = 0;

	lpObj->StartX = 0;

	lpObj->StartY = 0;

	lpObj->Dir = 0;

	lpObj->Map = 0;

	lpObj->PathCount = 0;

	lpObj->PathCur = 0;

	lpObj->PathStartEnd = 0;

	lpObj->Authority = 0;

	lpObj->AuthorityCode = 0;

	lpObj->Penalty = 0;

	lpObj->State = OBJECT_EMPTY;

	lpObj->Rest = 0;

	lpObj->ViewState = 0;

	lpObj->ViewSkillState = 0;

	lpObj->DieRegen = 0;

	lpObj->RegenOk = 0;

	lpObj->RegenMapNumber = 0;

	lpObj->RegenMapX = 0;

	lpObj->RegenMapY = 0;

	lpObj->RegenTime = 0;

	lpObj->PosNum = -1;

	lpObj->DelayLevel = 0;

	lpObj->DrinkSpeed = 0;

	lpObj->DrinkLastTime = 0;

	lpObj->MonsterDeleteTime = 0;

	lpObj->AttackObj = 0;

	lpObj->AttackerKilled = 0;

	for (int n = 0; n < MAX_SELF_DEFENSE; n++)
	{
		lpObj->SelfDefense[n] = -1;
	}

	memset(lpObj->SelfDefenseTime, 0, sizeof(lpObj->SelfDefenseTime));

	memset(lpObj->GuildName, 0, sizeof(lpObj->GuildName));

	lpObj->MySelfDefenseTime = 0;

	lpObj->PartyNumber = -1;

	lpObj->PartyTargetUser = -1;

	lpObj->GuildNumber = 0;

	lpObj->Guild = 0;

	lpObj->GuildStatus = -1;

	lpObj->SummonIndex = -1;

	lpObj->Change = -1;

	lpObj->TargetNumber = -1;

	lpObj->TargetShopNumber = -1;

	lpObj->LastAttackerID = -1;

	lpObj->MagicDefense = 0;

	lpObj->Attribute = 0;

	lpObj->MultiSkillIndex = 0;

	lpObj->MultiSkillCount = 0;

	for (int n = 0; n < MAX_SKILL_LIST; n++)
	{
		lpObj->Skill[n].Clear();
	}

	memset(lpObj->CharSet, 0, sizeof(lpObj->CharSet));

	memset(lpObj->Resistance, 0, sizeof(lpObj->Resistance));

	memset(lpObj->AddResistance, 0, sizeof(lpObj->AddResistance));

	gObjClearViewport(lpObj);

	gObjMonsterInitHitDamage(lpObj);

	lpObj->Interface.use = 0;

	lpObj->Interface.type = INTERFACE_NONE;

	lpObj->Interface.state = 0;

	lpObj->InterfaceTime = 0;

	lpObj->Transaction = 0;

	gObjSetInventory1Pointer(lpObj);

	gObjClearPlayerOption(lpObj);

	lpObj->TradeMoney = 0;

	lpObj->TradeOk = 0;

	lpObj->WarehouseCount = 0;

	lpObj->WarehousePW = 0;

	lpObj->WarehouseLock = -1;

	lpObj->WarehouseMoney = 0;

	lpObj->WarehouseSave = 0;

	lpObj->ChaosMoney = 0;

	lpObj->ChaosSuccessRate = 0;

	lpObj->ChaosLock = 0;

	lpObj->Option = 3;

	lpObj->IsInBattleGround = 0;

	lpObj->LoadWarehouse = 0;

	memset(lpObj->Quest, 0xFF, sizeof(lpObj->Quest));

	lpObj->SendQuestInfo = 0;

	lpObj->CheckLifeTime = 0;

	lpObj->LastTeleportTime = 0;

	lpObj->ReqWarehouseOpen = 0;

	lpObj->IsChaosMixCompleted = 0;

	lpObj->CharSaveTime = 0;

	for (int n = 0; n < MAX_EFFECT_LIST; n++)
	{
		lpObj->Effect[n].Clear();
	}

	lpObj->WarehouseNumber = 0;

	lpObj->AutoAddPointCount = 0;

	lpObj->AutoResetEnable = 0;

	memset(lpObj->AutoPartyPassword, 0, sizeof(lpObj->AutoPartyPassword));

	memset(lpObj->AutoAddPointStats, 0, sizeof(lpObj->AutoAddPointStats));

	memset(lpObj->AutoResetStats, 0, sizeof(lpObj->AutoResetStats));

	lpObj->CommandManagerTransaction = 0;

	lpObj->PotionTime = 0;

	lpObj->HPAutoRecuperationTime = 0;

	lpObj->MPAutoRecuperationTime = 0;

	lpObj->BPAutoRecuperationTime = 0;

	lpObj->Reset = 0;

	lpObj->GrandReset = 0;

	memset(&lpObj->EffectOption, 0, sizeof(lpObj->EffectOption));

	gObjClearSpecialOption(lpObj);

	lpObj->HPRecoveryCount = 0;

	lpObj->MPRecoveryCount = 0;

	lpObj->BPRecoveryCount = 0;

	lpObj->GroupNumber = 0;

	lpObj->SubGroupNumber = 0;

	lpObj->GroupMemberGuid = -1;

	lpObj->RegenType = 0;

	lpObj->LastAutomataRuntime = 0;

	lpObj->LastAutomataDelay = 0;

	lpObj->LastCheckTick = 0;

	for (int n = 0; n < MAX_MONSTER_SEND_MSG; n++)
	{
		gSMMsg[aIndex][n].Clear();
	}

	for (int n = 0; n < MAX_MONSTER_SEND_ATTACK_MSG; n++)
	{
		gSMAttackProcMsg[aIndex][n].Clear();
	}

	for (int n = 0; n < 100; n++)
	{
		lpObj->CommandLastTick[n] = 0;
	}
}

void gObjClearPlayerOption(LPOBJ lpObj)
{
	if (lpObj->Type != OBJECT_USER)
	{
		return;
	}

	for (int n = 0; n < INVENTORY_SIZE; n++)
	{
		lpObj->Inventory[n].Clear();
	}

	for (int n = 0; n < INVENTORY_SIZE; n++)
	{
		lpObj->Inventory1[n].Clear();
	}

	for (int n = 0; n < INVENTORY_SIZE; n++)
	{
		lpObj->Inventory2[n].Clear();
	}

	for (int n = 0; n < WAREHOUSE_SIZE; n++)
	{
		lpObj->Warehouse[n].Clear();
	}

	gTrade.ClearTrade(lpObj);

	memset(lpObj->InventoryMap, 0xFF, INVENTORY_SIZE);

	memset(lpObj->InventoryMap1, 0xFF, INVENTORY_SIZE);

	memset(lpObj->InventoryMap2, 0xFF, INVENTORY_SIZE);

	memset(lpObj->WarehouseMap, 0xFF, WAREHOUSE_SIZE);

	for (int n = 0; n < MAX_SKILL; n++)
	{
		memset(&lpObj->SkillDelay[n], 0, sizeof(lpObj->SkillDelay[n]));
	}

	for (int n = 0; n < MAX_HACK_PACKET_INFO; n++)
	{
		memset(&lpObj->HackPacketDelay[n], 0, sizeof(lpObj->HackPacketDelay[n]));
	}

	for (int n = 0; n < MAX_HACK_PACKET_INFO; n++)
	{
		memset(&lpObj->HackPacketCount[n], 0, sizeof(lpObj->HackPacketCount[n]));
	}
}

void gObjClearSpecialOption(LPOBJ lpObj)
{
	lpObj->ArmorSetBonus = 0;

	lpObj->SkillDamageBonus = 0;

	lpObj->IgnoreDefenseRate = 0;

	lpObj->CriticalDamageRate = 0;

	lpObj->CriticalDamage = 0;

	lpObj->ExcellentDamageRate = 0;

	lpObj->ExcellentDamage = 0;

	lpObj->ResistIgnoreDefenseRate = 0;

	lpObj->ResistCriticalDamageRate = 0;

	lpObj->ResistExcellentDamageRate = 0;

	lpObj->ResistStunRate = 0;

	lpObj->ExperienceRate = 100;

	lpObj->ItemDropRate = 100;

	lpObj->MoneyAmountDropRate = 100;

	lpObj->HPRecovery = 0;

	lpObj->MPRecovery = 0;

	lpObj->BPRecovery = 2;

	lpObj->HPRecoveryRate = 0;

	lpObj->MPRecoveryRate = 0;

	lpObj->BPRecoveryRate = 0;

	lpObj->MPConsumptionRate = 100;

	lpObj->BPConsumptionRate = 100;

	lpObj->ShieldDamageReduction = gServerInfo.m_DefenseConstA;

	lpObj->ShieldDamageReductionTime = 0;

	memset(lpObj->DamageReduction, 0, sizeof(lpObj->DamageReduction));

	lpObj->DamageReflect = 0;

	lpObj->HuntHP = 0;

	lpObj->HuntMP = 0;

	lpObj->HuntBP = 0;

	lpObj->WeaponDurabilityRate = 100;

	lpObj->ArmorDurabilityRate = 100;

	lpObj->WingDurabilityRate = 100;

	lpObj->GuardianDurabilityRate = 100;

	lpObj->PendantDurabilityRate = 100;

	lpObj->RingDurabilityRate = 100;

	lpObj->DefensiveFullHPRestoreRate = 0;

	lpObj->DefensiveFullMPRestoreRate = 0;

	lpObj->DefensiveFullBPRestoreRate = 0;

	lpObj->OffensiveFullHPRestoreRate = 0;

	lpObj->OffensiveFullMPRestoreRate = 0;

	lpObj->OffensiveFullBPRestoreRate = 0;
}

void gObjCalcExperience(LPOBJ lpObj)
{
	lpObj->Experience = ((lpObj->Experience < gLevelExperience[(lpObj->Level - 1)]) ? gLevelExperience[(lpObj->Level - 1)] : lpObj->Experience);

	lpObj->NextExperience = gLevelExperience[((lpObj->Level >= MAX_CHARACTER_LEVEL) ? MAX_CHARACTER_LEVEL : lpObj->Level)];
}

bool gObjGetRandomFreeLocation(int map, int* ox, int* oy, int tx, int ty, int count)
{
	int x = (*ox);

	int y = (*oy);

	tx = ((tx < 1) ? 1 : tx);

	ty = ((ty < 1) ? 1 : ty);

	for (int n = 0; n < count; n++)
	{
		(*ox) = ((GetLargeRand() % (tx + 1)) * ((GetLargeRand() % 2 == 0) ? -1 : 1)) + x;

		(*oy) = ((GetLargeRand() % (ty + 1)) * ((GetLargeRand() % 2 == 0) ? -1 : 1)) + y;

		if (gMap[map].CheckAttr((*ox), (*oy), 255) == 0)
		{
			return 1;
		}
	}

	return 0;
}

bool gObjAllocData(int aIndex)
{
	CMemoryAllocatorInfo MemoryAllocatorInfo;

	if (gMemoryAllocator.GetMemoryAllocatorInfo(&MemoryAllocatorInfo, aIndex) == 0)
	{
		gObj.ObjectStruct[aIndex] = new OBJECTSTRUCT;

		memset(gObj.ObjectStruct[aIndex], 0, sizeof(OBJECTSTRUCT));

		MemoryAllocatorInfo.m_Index = aIndex;

		MemoryAllocatorInfo.m_Active = 1;

		MemoryAllocatorInfo.m_ActiveTime = GetTickCount();

		MemoryAllocatorInfo.Alloc();

		gMemoryAllocator.InsertMemoryAllocatorInfo(MemoryAllocatorInfo);

		gMemoryAllocator.BindMemoryAllocatorInfo(aIndex, MemoryAllocatorInfo);

		if (OBJECT_MONSTER_RANGE(aIndex) != 0)
		{
			gObjMonCount = (((++gObjMonCount) >= MAX_OBJECT_MONSTER) ? OBJECT_START_MONSTER : gObjMonCount);
		}

		if (OBJECT_SUMMON_RANGE(aIndex) != 0)
		{
			gObjCallMonCount = (((++gObjCallMonCount) >= OBJECT_START_USER) ? MAX_OBJECT_MONSTER : gObjCallMonCount);
		}

		if (OBJECT_USER_RANGE(aIndex) != 0)
		{
			gObjCount = (((++gObjCount) >= MAX_OBJECT) ? OBJECT_START_USER : gObjCount);
		}
	}
	else
	{
		MemoryAllocatorInfo.m_Index = aIndex;

		MemoryAllocatorInfo.m_Active = 1;

		MemoryAllocatorInfo.m_ActiveTime = GetTickCount();

		gMemoryAllocator.InsertMemoryAllocatorInfo(MemoryAllocatorInfo);

		gMemoryAllocator.BindMemoryAllocatorInfo(aIndex, MemoryAllocatorInfo);
	}

	return 1;
}

void gObjFreeData(int aIndex)
{
	CMemoryAllocatorInfo MemoryAllocatorInfo;

	if (gMemoryAllocator.GetMemoryAllocatorInfo(&MemoryAllocatorInfo, aIndex) != 0)
	{
		MemoryAllocatorInfo.m_Index = aIndex;

		MemoryAllocatorInfo.m_Active = 0;

		MemoryAllocatorInfo.m_ActiveTime = GetTickCount();

		gMemoryAllocator.InsertMemoryAllocatorInfo(MemoryAllocatorInfo);
	}
}

short gObjAddSearch(SOCKET socket, char* IpAddress)
{
	int index = -1;

	int count = gObjCount;

	if (gGameServerLogOut != 0)
	{
		GCConnectAccountSend(0, 2, socket);

		return -1;
	}

	if (gGameServerDisconnect != 0)
	{
		GCConnectAccountSend(0, 2, socket);

		return -1;
	}

	if (gObjTotalUser >= gServerInfo.m_ServerMaxUserNumber)
	{
		GCConnectAccountSend(0, 4, socket);

		return -1;
	}

	if (gMemoryAllocator.GetMemoryAllocatorFree(&index, OBJECT_START_USER, MAX_OBJECT, 10000) != 0)
	{
		return index;
	}

	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObj[count].Connected == OBJECT_OFFLINE)
		{
			return count;
		}
		else
		{
			count = (((++count) >= MAX_OBJECT) ? OBJECT_START_USER : count);
		}
	}

	return -1;
}

short gObjAdd(SOCKET socket, char* IpAddress, int aIndex)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return -1;
	}

	if (gObj[aIndex].Connected != OBJECT_OFFLINE)
	{
		return -1;
	}

	if (gObjAllocData(aIndex) == 0)
	{
		return -1;
	}

	LPOBJ lpObj = &gObj[aIndex];

	gObjCharZeroSet(aIndex);

	lpObj->Lang = LANGUAGE_ENGLISH;

	lpObj->Index = aIndex;

	lpObj->Connected = OBJECT_CONNECTED;

	lpObj->LoginMessageSend = 0;

	lpObj->LoginMessageCount = 0;

	lpObj->Socket = socket;

	strcpy_s(lpObj->IpAddr, IpAddress);

	lpObj->AutoSaveTime = GetTickCount();

	lpObj->ConnectTickCount = GetTickCount();

	lpObj->Type = OBJECT_USER;

	gObjTotalUser++;

	gServerDisplayer.SetWindowName();

	memset(lpObj->Account, 0, sizeof(lpObj->Account));

	memset(lpObj->HardwareID, 0, sizeof(lpObj->HardwareID));

	gSerialCheck[aIndex].Init();

	gIpManager.InsertIpAddress(lpObj->IpAddr);

	//LogAdd(LOG_USER, "[ObjectManager][%d] AddClient (%s)", aIndex, lpObj->IpAddr);

	gLog.Output(LOG_CONNECT, "[ObjectManager][%d] AddClient (%s)", aIndex, lpObj->IpAddr);

	return aIndex;
}

short gObjDel(int aIndex)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return -1;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected == OBJECT_OFFLINE)
	{
		return -1;
	}

	if (MAP_RANGE(lpObj->Map) != 0)
	{
		gMap[lpObj->Map].DelStandAttr(lpObj->X, lpObj->Y);

		gMap[lpObj->Map].DelStandAttr(lpObj->OldX, lpObj->OldY);
	}

	if (lpObj->Type == OBJECT_USER)
	{
		gObjectManager.CharacterGameClose(aIndex);

		GJDisconnectAccountSend(aIndex, lpObj->Account, lpObj->IpAddr);

		if (lpObj->Account[0] != 0)
		{
			//LogAdd(LOG_USER, "[ObjectManager][%d] DelAccountInfo (%s)(%s)", aIndex, lpObj->Account, lpObj->HardwareID);

			gLog.Output(LOG_CONNECT, "[ObjectManager][%d] DelAccountInfo (%s)(%s)", aIndex, lpObj->Account, lpObj->HardwareID);
		}

		//LogAdd(LOG_USER, "[ObjectManager][%d] DelClient (%s)", aIndex, lpObj->IpAddr);

		gLog.Output(LOG_CONNECT, "[ObjectManager][%d] DelClient (%s)", aIndex, lpObj->IpAddr);

		memset(lpObj->Account, 0, sizeof(lpObj->Account));

		memset(lpObj->PersonalCode, 0, sizeof(lpObj->PersonalCode));

		gIpManager.RemoveIpAddress(lpObj->IpAddr);

		gObjTotalUser--;

		gServerDisplayer.SetWindowName();
	}

	if (lpObj->Type == OBJECT_MONSTER || lpObj->Type == OBJECT_NPC)
	{
		gObjTotalMonster--;

		gServerDisplayer.SetWindowName();
	}

	lpObj->Connected = OBJECT_OFFLINE;

	gObjFreeData(aIndex);

	return aIndex;
}

LPOBJ gObjFind(char* name)
{
	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnectedGP(n) != 0 && strcmp(gObj[n].Name, name) == 0)
		{
			return &gObj[n];
		}
	}

	return 0;
}

int gObjCalcDistance(LPOBJ lpObj, LPOBJ lpTarget)
{
	return (int)sqrt(pow(((float)lpObj->X - (float)lpTarget->X), 2) + pow(((float)lpObj->Y - (float)lpTarget->Y), 2));
}

//**************************************************************************//
// OBJECT CHECK FUNCTIONS **************************************************//
//**************************************************************************//

bool gObjIsConnected(int aIndex)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected != OBJECT_ONLINE)
	{
		return 0;
	}

	return 1;
}

bool gObjIsConnectedGP(int aIndex)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected != OBJECT_ONLINE)
	{
		return 0;
	}

	if (lpObj->Type != OBJECT_USER || lpObj->CloseCount > 0)
	{
		return 0;
	}

	return 1;
}

bool gObjIsConnectedGS(int aIndex)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected != OBJECT_ONLINE)
	{
		return 0;
	}

	if (lpObj->Type == OBJECT_USER && lpObj->CloseCount > 0)
	{
		return 0;
	}

	return 1;
}

bool gObjIsNameValid(int aIndex, char* name)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected == OBJECT_OFFLINE)
	{
		return 0;
	}

	if (strcmp(name, "") == 0)
	{
		return 0;
	}

	if (strcmp(lpObj->Name, name) != 0)
	{
		return 0;
	}

	return 1;
}

bool gObjIsAccountValid(int aIndex, char* account)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected == OBJECT_OFFLINE)
	{
		return 0;
	}

	if (strcmp(account, "") == 0)
	{
		return 0;
	}

	if (strcmp(lpObj->Account, account) != 0)
	{
		return 0;
	}

	return 1;
}

bool gObjIsChangeSkin(int aIndex)
{
	if (gObj[aIndex].Change < 0)
	{
		return 0;
	}

	if (gObj[aIndex].Change == gServerInfo.m_TransformationRing1 || gObj[aIndex].Change == gServerInfo.m_TransformationRing2 || gObj[aIndex].Change == gServerInfo.m_TransformationRing3 || gObj[aIndex].Change == gServerInfo.m_TransformationRing4 || gObj[aIndex].Change == gServerInfo.m_TransformationRing5 || gObj[aIndex].Change == gServerInfo.m_TransformationRing6)
	{
		return 1;
	}

	return 1;
}

bool gObjCheckMaxMoney(int aIndex, DWORD AddMoney)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	if (((QWORD)gObj[aIndex].Money + (QWORD)AddMoney) > (QWORD)MAX_MONEY)
	{
		return 0;
	}

	return 1;
}

bool gObjCheckPersonalCode(int aIndex, char* PersonalCode)
{
	if (gServerInfo.m_PersonalCodeCheck == 0)
	{
		return 1;
	}

	if (strncmp(PersonalCode, &gObj[aIndex].PersonalCode[6], 7) == 0)
	{
		return 1;
	}

	return 0;
}

bool gObjCheckResistance(LPOBJ lpObj, int type)
{
	BYTE resist = lpObj->Resistance[type];

	if (resist == 0xFF)
	{
		return 1;
	}

	if ((lpObj->Authority & 32) != 0)
	{
		return 1;
	}

	if ((GetLargeRand() % (resist + 1)) == 0)
	{
		return 0;
	}

	return 1;
}

bool gObjCheckTeleportArea(int aIndex, int x, int y)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Live == 0 || lpObj->Teleport != 0)
	{
		return 0;
	}

	if (x < (lpObj->X - 8) || x >(lpObj->X + 8) || y < (lpObj->Y - 8) || y >(lpObj->Y + 8))
	{
		return 0;
	}

	if (gMap[lpObj->Map].CheckAttr(lpObj->X, lpObj->Y, 1) != 0 || gMap[lpObj->Map].CheckAttr(x, y, 1) != 0)
	{
		return 0;
	}

	return 1;
}

bool gObjCheckMapTile(LPOBJ lpObj, int type)
{
	if (lpObj->Type != OBJECT_USER)
	{
		return 0;
	}

	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			if (gMap[lpObj->Map].CheckAttr((lpObj->X + x), (lpObj->Y + y), 4) == 0 && gMap[lpObj->Map].CheckAttr((lpObj->X + x), (lpObj->Y + y), 8) == 0)
			{
				return 0;
			}
		}
	}

	switch (lpObj->Class)
	{
		case CLASS_DW:
		{
			gObjMoveGate(lpObj->Index, 17);

			break;
		}

		case CLASS_DK:
		{
			gObjMoveGate(lpObj->Index, 17);

			break;
		}

		case CLASS_FE:
		{
			gObjMoveGate(lpObj->Index, 27);

			break;
		}

		case CLASS_MG:
		{
			gObjMoveGate(lpObj->Index, 17);

			break;
		}
	}

	return 1;
}

//**************************************************************************//
// ITEM TRANSACTION FUNCTIONS **********************************************//
//**************************************************************************//

bool gObjFixInventoryPointer(int aIndex)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Inventory == lpObj->Inventory1)
	{
		return 1;
	}

	if (lpObj->Inventory == lpObj->Inventory2)
	{
		if (lpObj->Transaction == 1)
		{
			return 0;
		}
		else
		{
			for (int n = 0; n < INVENTORY_SIZE; n++)
			{
				lpObj->Inventory2[n].Clear();
			}
		}
	}

	gObjSetInventory1Pointer(lpObj);

	return 0;
}

void gObjSetInventory1Pointer(LPOBJ lpObj)
{
	lpObj->Inventory = lpObj->Inventory1;

	lpObj->InventoryMap = lpObj->InventoryMap1;
}

void gObjSetInventory2Pointer(LPOBJ lpObj)
{
	lpObj->Inventory = lpObj->Inventory2;

	lpObj->InventoryMap = lpObj->InventoryMap2;
}

bool gObjInventoryTransaction(int aIndex)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Transaction == 1)
	{
		return 0;
	}

	for (int n = 0; n < MAX_SKILL_LIST; n++)
	{
		lpObj->SkillBackup[n] = lpObj->Skill[n];
	}

	for (int n = 0; n < INVENTORY_SIZE; n++)
	{
		lpObj->Inventory2[n] = lpObj->Inventory1[n];
	}

	memcpy(lpObj->InventoryMap2, lpObj->InventoryMap1, INVENTORY_SIZE);

	gObjSetInventory2Pointer(lpObj);

	lpObj->Transaction = 1;

	return 1;
}

bool gObjInventoryCommit(int aIndex)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Transaction != 1)
	{
		return 0;
	}

	for (int n = 0; n < INVENTORY_SIZE; n++)
	{
		lpObj->Inventory1[n] = lpObj->Inventory2[n];
	}

	memcpy(lpObj->InventoryMap1, lpObj->InventoryMap2, INVENTORY_SIZE);

	gObjSetInventory1Pointer(lpObj);

	lpObj->Transaction = 2;

	return 1;
}

bool gObjInventoryRollback(int aIndex)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Transaction != 1)
	{
		return 0;
	}

	for (int n = 0; n < MAX_SKILL_LIST; n++)
	{
		lpObj->Skill[n] = lpObj->SkillBackup[n];
	}

	for (int n = 0; n < INVENTORY_SIZE; n++)
	{
		lpObj->Inventory2[n].Clear();
	}

	gObjSetInventory1Pointer(lpObj);

	lpObj->Transaction = 3;

	return 1;
}

//**************************************************************************//
// VIEWPORT FUNCTIONS ******************************************************//
//**************************************************************************//

void gObjSetViewport(int aIndex, int state)
{
	LPOBJ lpObj = &gObj[aIndex];

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer[n].state == state)
		{
			switch (state)
			{
				case VIEWPORT_SEND:
				{
					lpObj->VpPlayer[n].state = VIEWPORT_WAIT;

					break;
				}

				case VIEWPORT_DESTROY:
				{
					lpObj->VpPlayer[n].state = VIEWPORT_NONE;

					lpObj->VpPlayer[n].index = -1;

					lpObj->VPCount--;

					break;
				}
			}
		}

		if (lpObj->VpPlayerItem[n].state == state)
		{
			switch (state)
			{
				case VIEWPORT_SEND:
				{
					lpObj->VpPlayerItem[n].state = VIEWPORT_WAIT;

					break;
				}

				case VIEWPORT_DESTROY:
				{
					lpObj->VpPlayerItem[n].state = VIEWPORT_NONE;

					lpObj->VpPlayerItem[n].index = -1;

					lpObj->VPCountItem--;

					break;
				}
			}
		}
	}
}

void gObjClearViewport(LPOBJ lpObj)
{
	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		lpObj->VpPlayer[n].state = VIEWPORT_NONE;

		lpObj->VpPlayer[n].index = -1;

		lpObj->VpPlayer2[n].state = VIEWPORT_NONE;

		lpObj->VpPlayer2[n].index = -1;

		lpObj->VpPlayerItem[n].state = VIEWPORT_NONE;

		lpObj->VpPlayerItem[n].index = -1;
	}

	lpObj->VPCount = 0;

	lpObj->VPCount2 = 0;

	lpObj->VPCountItem = 0;
}

void gObjViewportListProtocolDestroy(LPOBJ lpObj)
{
	gViewport.GCViewportSimpleDestroySend(lpObj);
}

void gObjViewportListProtocolCreate(LPOBJ lpObj)
{
	if (lpObj->Type == OBJECT_USER)
	{
		gViewport.GCViewportSimplePlayerSend(lpObj);

		gViewport.GCViewportSimpleChangeSend(lpObj);

		gViewport.GCViewportSimpleGuildInfoSend(lpObj);

		gViewport.GCViewportSimpleGuildMemberSend(lpObj);
	}
	else
	{
		gViewport.GCViewportSimpleMonsterSend(lpObj);

		gViewport.GCViewportSimpleSummonSend(lpObj);
	}
}

void gObjViewportListProtocol(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected != OBJECT_ONLINE)
	{
		return;
	}

	if (lpObj->Type == OBJECT_USER)
	{
		gViewport.GCViewportDestroySend(aIndex);

		gViewport.GCViewportDestroyItemSend(aIndex);
	}

	gObjSetViewport(aIndex, VIEWPORT_DESTROY);

	if (lpObj->Type == OBJECT_USER)
	{
		gViewport.GCViewportPlayerSend(aIndex);

		gViewport.GCViewportMonsterSend(aIndex);

		gViewport.GCViewportSummonSend(aIndex);

		gViewport.GCViewportItemSend(aIndex);

		gViewport.GCViewportChangeSend(aIndex);

		gViewport.GCViewportGuildInfoSend(aIndex);

		gViewport.GCViewportGuildMemberSend(aIndex);
	}

	gObjSetViewport(aIndex, VIEWPORT_SEND);
}

void gObjViewportListDestroy(int aIndex)
{
	if (gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	gViewport.DestroyViewportPlayer1(aIndex);

	gViewport.DestroyViewportPlayer2(aIndex);

	gViewport.DestroyViewportMonster1(aIndex);

	gViewport.DestroyViewportMonster2(aIndex);

	gViewport.DestroyViewportItem(aIndex);
}

void gObjViewportListCreate(int aIndex)
{
	if (gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	if (gObj[aIndex].RegenOk > 0)
	{
		return;
	}

	gViewport.CreateViewportPlayer(aIndex);

	gViewport.CreateViewportMonster(aIndex);

	gViewport.CreateViewportItem(aIndex);
}

//**************************************************************************//
// USER FUNCTIONS **********************************************************//
//**************************************************************************//

void gObjTeleportMagicUse(int aIndex, int x, int y)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Teleport != 0)
	{
		return;
	}

	lpObj->TeleportTime = GetTickCount();

	lpObj->PathCount = 0;

	lpObj->Teleport = 1;

	lpObj->ViewState = 1;

	lpObj->X = x;

	lpObj->Y = y;

	lpObj->TX = x;

	lpObj->TY = y;

	gMap[lpObj->Map].DelStandAttr(lpObj->OldX, lpObj->OldY);

	gMap[lpObj->Map].SetStandAttr(lpObj->TX, lpObj->TY);

	lpObj->OldX = lpObj->TX;

	lpObj->OldY = lpObj->TY;

	gObjViewportListProtocolDestroy(lpObj);
}

void gObjInterfaceCheckTime(LPOBJ lpObj)
{
	if (lpObj->Interface.use == 0)
	{
		return;
	}

	if ((GetTickCount() - lpObj->InterfaceTime) < 5000)
	{
		return;
	}

	if (lpObj->Interface.type == INTERFACE_TRADE)
	{
		if (lpObj->Interface.state == 0)
		{
			if (OBJECT_RANGE(lpObj->TargetNumber) != 0)
			{
				gObj[lpObj->TargetNumber].Interface.use = 0;

				gObj[lpObj->TargetNumber].Interface.type = INTERFACE_NONE;

				gObj[lpObj->TargetNumber].Interface.state = 0;

				gObj[lpObj->TargetNumber].TargetNumber = -1;

				gTrade.GCTradeResultSend(lpObj->TargetNumber, 3);
			}

			lpObj->Interface.use = 0;

			lpObj->Interface.type = INTERFACE_NONE;

			lpObj->Interface.state = 0;

			lpObj->TargetNumber = -1;

			gTrade.GCTradeResultSend(lpObj->Index, 3);
		}
	}

	if (lpObj->Interface.type == INTERFACE_PARTY)
	{
		if (lpObj->Interface.state == 0)
		{
			if (OBJECT_RANGE(lpObj->TargetNumber) != 0)
			{
				gObj[lpObj->TargetNumber].Interface.use = 0;

				gObj[lpObj->TargetNumber].Interface.type = INTERFACE_NONE;

				gObj[lpObj->TargetNumber].Interface.state = 0;

				gObj[lpObj->TargetNumber].TargetNumber = -1;

				gObj[lpObj->TargetNumber].PartyTargetUser = -1;

				gParty.GCPartyResultSend(lpObj->TargetNumber, 0);
			}

			lpObj->Interface.use = 0;

			lpObj->Interface.type = INTERFACE_NONE;

			lpObj->Interface.state = 0;

			lpObj->TargetNumber = -1;

			lpObj->PartyTargetUser = -1;

			gParty.GCPartyResultSend(lpObj->Index, 0);
		}
	}

	lpObj->InterfaceTime = GetTickCount();
}

void gObjPKDownCheckTime(LPOBJ lpObj, int TargetLevel)
{
	if (lpObj->PKLevel == PKLVL_COMMONER)
	{
		return;
	}

	lpObj->PKTime += TargetLevel;

	if (lpObj->PKLevel < PKLVL_COMMONER)
	{
		if (lpObj->PKTime > gServerInfo.m_PKDownTime1)
		{
			if (lpObj->PKCount < 100)
			{
				lpObj->PKCount++;
			}

			lpObj->PKLevel++;

			lpObj->PKTime = 0;

			GCPKLevelSend(lpObj->Index, lpObj->PKLevel);
		}
	}
	else
	{
		if (lpObj->PKTime > gServerInfo.m_PKDownTime2)
		{
			if (lpObj->PKCount > 0)
			{
				lpObj->PKCount--;
			}

			if (lpObj->PKCount == 0)
			{
				lpObj->PKLevel = PKLVL_COMMONER;
			}
			else if (lpObj->PKCount == 1)
			{
				lpObj->PKLevel = PKLVL_WARNING;
			}
			else if (lpObj->PKCount == 2)
			{
				lpObj->PKLevel = PKLVL_OUTLAW;
			}

			lpObj->PKTime = 0;

			GCPKLevelSend(lpObj->Index, lpObj->PKLevel);
		}
	}
}

void gObjUserDie(LPOBJ lpObj, LPOBJ lpTarget)
{
	if (lpObj->Type != OBJECT_USER)
	{
		return;
	}

	if (DS_MAP_RANGE(lpObj->Map) != 0)
	{
		gDevilSquare.UserDieProc(lpObj, lpTarget);

		return;
	}
	else if (BC_MAP_RANGE(lpObj->Map) != 0)
	{
		gBloodCastle.UserDieProc(lpObj, lpTarget);

		return;
	}

	if (gGuild.GuildWarCheck(lpTarget, lpObj) != false)
	{
		return;
	}
}

void gObjPlayerKiller(LPOBJ lpObj, LPOBJ lpTarget)
{
	if (lpObj->Type != OBJECT_USER || lpTarget->Type != OBJECT_USER)
	{
		return;
	}

	if (lpObj->Authority == 32 || lpTarget->Authority == 32)
	{
		return;
	}

	if (gGuild.GuildWarStateCheck(lpObj, lpTarget) != false)
	{
		return;
	}

	if (gMapManager.GetMapNonOutlaw(lpObj->Map) != 0)
	{
		return;
	}

	for (int n = 0; n < MAX_SELF_DEFENSE; n++)
	{
		if (OBJECT_RANGE(lpTarget->SelfDefense[n]) != 0)
		{
			if (lpObj->Index == lpTarget->SelfDefense[n])
			{
				return;
			}
		}
	}

	if (gPKFree.CheckPKFree(lpObj->Map, lpObj->X, lpObj->Y) != 0)
	{
		return;
	}

	if (lpObj->PKLevel > PKLVL_COMMONER)
	{
		if (lpTarget->PKLevel > PKLVL_WARNING)
		{
			return;
		}

		if (lpObj->PKCount < 100)
		{
			lpObj->PKCount++;
		}
	}
	else
	{
		if (lpTarget->PKLevel < PKLVL_OUTLAW)
		{
			lpObj->PKCount = 1;
		}
		else if (lpObj->PKCount > -3)
		{
			lpObj->PKCount--;
		}
	}

	if (lpObj->PKCount <= -3)
	{
		lpObj->PKLevel = PKLVL_FULLHERO;
	}
	else if (lpObj->PKCount == 0)
	{
		lpObj->PKLevel = PKLVL_COMMONER;
	}
	else if (lpObj->PKCount == 1)
	{
		lpObj->PKLevel = PKLVL_WARNING;
	}
	else if (lpObj->PKCount == 2)
	{
		lpObj->PKLevel = PKLVL_OUTLAW;
	}
	else if (lpObj->PKCount >= 3)
	{
		lpObj->PKLevel = PKLVL_KILLER;
	}

	lpObj->PKTime = 0;

	GCPKLevelSend(lpObj->Index, lpObj->PKLevel);
}

BOOL gObjMoveGate(int aIndex, int gate)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (BC_MAP_RANGE(lpObj->Map) != 0)
	{
		if (gBloodCastle.GetState(GET_BC_LEVEL(lpObj->Map)) == BC_STATE_START)
		{
			gBloodCastle.SearchUserDropEventItem(aIndex);
		}
		else
		{
			gBloodCastle.SearchUserDeleteEventItem(aIndex);
		}
	}

	if (lpObj->RegenOk != 0 || gGate.IsGate(gate) == 0)
	{
		goto ERROR_JUMP;
	}

	int TargetGate, map, x, y, dir, level;

	if (gGate.GetGate(gate, &TargetGate, &map, &x, &y, &dir, &level) == 0)
	{
		goto ERROR_JUMP;
	}

	if (map == MAP_ATLANS && (lpObj->Inventory[8].m_Index == GET_ITEM(13, 2) || lpObj->Inventory[8].m_Index == GET_ITEM(13, 3))) // Uniria, Dinorant
	{
		gNotice.GCNoticeSend(aIndex, 1, gMessage.GetTextMessage(67, gObj[aIndex].Lang));

		goto ERROR_JUMP;
	}

	if (map == MAP_ICARUS && (lpObj->Inventory[7].IsItem() == 0 && lpObj->Inventory[8].m_Index != GET_ITEM(13, 3))) // Wings
	{
		gNotice.GCNoticeSend(aIndex, 1, gMessage.GetTextMessage(68, gObj[aIndex].Lang));

		goto ERROR_JUMP;
	}

	if (map == MAP_ICARUS && lpObj->Inventory[8].m_Index == GET_ITEM(13, 2)) // Uniria
	{
		gNotice.GCNoticeSend(aIndex, 1, gMessage.GetTextMessage(67, gObj[aIndex].Lang));

		goto ERROR_JUMP;
	}

	if (lpObj->Interface.use != 0 || lpObj->DieRegen != 0)
	{
		lpObj->State = OBJECT_DELCMD;

		lpObj->RegenOk = 1;

		gMove.GCTeleportSend(aIndex, gate, lpObj->Map, (BYTE)lpObj->X, (BYTE)lpObj->Y, lpObj->Dir);

		return 0;
	}

	lpObj->State = OBJECT_DELCMD;

	lpObj->X = x;

	lpObj->Y = y;

	lpObj->TX = x;

	lpObj->TY = y;

	lpObj->Map = map;

	lpObj->Dir = dir;

	lpObj->PathCount = 0;

	lpObj->Teleport = 0;

	lpObj->ViewState = 0;

	gObjViewportListProtocolDestroy(lpObj);

	gMove.GCTeleportSend(aIndex, gate, lpObj->Map, (BYTE)lpObj->X, (BYTE)lpObj->Y, lpObj->Dir);

	gObjViewportListProtocolCreate(lpObj);

	gObjClearViewport(lpObj);

	gObjectManager.CharacterUpdateMapEffect(lpObj);

	gFlyingDragons.FlyingDragonsCheck(lpObj->Map, lpObj->Index);

	lpObj->RegenMapNumber = lpObj->Map;

	lpObj->RegenMapX = (BYTE)lpObj->X;

	lpObj->RegenMapY = (BYTE)lpObj->Y;

	lpObj->RegenOk = 1;

	if (lpObj->Type == OBJECT_USER)
	{
		lpObj->LastTeleportTime = 10;
	}

	return 1;

ERROR_JUMP:

	gObjViewportListProtocolDestroy(lpObj);

	gMove.GCTeleportSend(aIndex, gate, lpObj->Map, (BYTE)lpObj->X, (BYTE)lpObj->Y, lpObj->Dir);

	gObjViewportListProtocolCreate(lpObj);

	gObjClearViewport(lpObj);

	gObjectManager.CharacterUpdateMapEffect(lpObj);

	lpObj->RegenMapNumber = lpObj->Map;

	lpObj->RegenMapX = (BYTE)lpObj->X;

	lpObj->RegenMapY = (BYTE)lpObj->Y;

	lpObj->RegenOk = 1;

	return 0;
}

void gObjTeleport(int aIndex, int map, int x, int y)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return;
	}

	if (MAP_RANGE(map) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	lpObj->State = OBJECT_DELCMD;

	if (lpObj->Interface.use != 0 && lpObj->Interface.type == INTERFACE_TRADE)
	{
		gTrade.CGTradeCancelButtonRecv(aIndex);
	}

	if (lpObj->Interface.use != 0 && lpObj->Interface.type == INTERFACE_WAREHOUSE)
	{
		gWarehouse.CGWarehouseClose(aIndex);
	}

	lpObj->X = x;

	lpObj->Y = y;

	lpObj->TX = x;

	lpObj->TY = y;

	lpObj->Map = map;

	lpObj->PathCount = 0;

	lpObj->Teleport = 0;

	lpObj->ViewState = 0;

	gObjClearViewport(lpObj);

	gMove.GCTeleportSend(aIndex, 1, lpObj->Map, (BYTE)lpObj->X, (BYTE)lpObj->Y, lpObj->Dir);

	gObjViewportListProtocolCreate(lpObj);

	gObjectManager.CharacterUpdateMapEffect(lpObj);

	lpObj->RegenMapNumber = lpObj->Map;

	lpObj->RegenMapX = (BYTE)lpObj->X;

	lpObj->RegenMapY = (BYTE)lpObj->Y;

	lpObj->RegenOk = 1;
}

void gObjSkillUseProc(LPOBJ lpObj)
{
	if (lpObj->Type == OBJECT_USER && lpObj->DrinkSpeed > 0)
	{
		if (GetTickCount() >= lpObj->DrinkLastTime)
		{
			lpObj->DrinkSpeed = 0;

			lpObj->DrinkLastTime = 0;

			gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
	}
}

void gObjUserKill(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->CloseCount <= 0)
	{
		lpObj->CloseType = 0;

		lpObj->CloseCount = 1;
	}
}

bool gObjInventorySearchSerialNumber(LPOBJ lpObj, DWORD serial)
{
	int count = 0;

	for (int n = 0; n < INVENTORY_SIZE; n++)
	{
		if (lpObj->Inventory[n].m_Serial != 0 && lpObj->Inventory[n].m_Serial == serial && (count++) > 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(54, lpObj->Lang));

			gObjUserKill(lpObj->Index);

			return 0;
		}
	}

	return 1;
}

bool gObjWarehouseSearchSerialNumber(LPOBJ lpObj, DWORD serial)
{
	int count = 0;

	for (int n = 0; n < WAREHOUSE_SIZE; n++)
	{
		if (lpObj->Warehouse[n].m_Serial != 0 && lpObj->Warehouse[n].m_Serial == serial && (count++) > 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(54, lpObj->Lang));

			gObjUserKill(lpObj->Index);

			return 0;
		}
	}

	return 1;
}

void gObjAddMsgSend(LPOBJ lpObj, int MsgCode, int SendUser, int SubCode)
{
	for (int n = 0; n < MAX_MONSTER_SEND_MSG; n++)
	{
		if (gSMMsg[lpObj->Index][n].MsgCode == -1)
		{
			if (gSMMsg.ObjectStruct[lpObj->Index] == gSMMsg.CommonStruct)
			{
				gSMMsg.ObjectStruct[lpObj->Index] = new MESSAGE_STATE_MACHINE_COMMON;
			}

			if (gSMMsg.ObjectStruct[lpObj->Index]->ObjectStruct[n] == gSMMsg.ObjectStruct[lpObj->Index]->CommonStruct)
			{
				gSMMsg.ObjectStruct[lpObj->Index]->ObjectStruct[n] = new MESSAGE_STATE_MACHINE;
			}

			gSMMsg[lpObj->Index][n].MsgCode = MsgCode;

			gSMMsg[lpObj->Index][n].MsgTime = GetTickCount();

			gSMMsg[lpObj->Index][n].SendUser = SendUser;

			gSMMsg[lpObj->Index][n].SubCode = SubCode;

			return;
		}
	}
}

void gObjAddMsgSendDelay(LPOBJ lpObj, int MsgCode, int SendUser, int MsgTimeDelay, int SubCode)
{
	for (int n = 0; n < MAX_MONSTER_SEND_MSG; n++)
	{
		if (gSMMsg[lpObj->Index][n].MsgCode == -1)
		{
			if (gSMMsg.ObjectStruct[lpObj->Index] == gSMMsg.CommonStruct)
			{
				gSMMsg.ObjectStruct[lpObj->Index] = new MESSAGE_STATE_MACHINE_COMMON;
			}

			if (gSMMsg.ObjectStruct[lpObj->Index]->ObjectStruct[n] == gSMMsg.ObjectStruct[lpObj->Index]->CommonStruct)
			{
				gSMMsg.ObjectStruct[lpObj->Index]->ObjectStruct[n] = new MESSAGE_STATE_MACHINE;
			}

			gSMMsg[lpObj->Index][n].MsgCode = MsgCode;

			gSMMsg[lpObj->Index][n].MsgTime = GetTickCount() + MsgTimeDelay;

			gSMMsg[lpObj->Index][n].SendUser = SendUser;

			gSMMsg[lpObj->Index][n].SubCode = SubCode;

			return;
		}
	}
}

void gObjAddAttackProcMsgSendDelay(LPOBJ lpObj, int MsgCode, int SendUser, int MsgTimeDelay, int SubCode, int SubCode2)
{
	for (int n = 0; n < MAX_MONSTER_SEND_ATTACK_MSG; n++)
	{
		if (gSMAttackProcMsg[lpObj->Index][n].MsgCode == -1)
		{
			if (gSMAttackProcMsg.ObjectStruct[lpObj->Index] == gSMAttackProcMsg.CommonStruct)
			{
				gSMAttackProcMsg.ObjectStruct[lpObj->Index] = new MESSAGE_STATE_ATTACK_MACHINE_COMMON;
			}

			if (gSMAttackProcMsg.ObjectStruct[lpObj->Index]->ObjectStruct[n] == gSMAttackProcMsg.ObjectStruct[lpObj->Index]->CommonStruct)
			{
				gSMAttackProcMsg.ObjectStruct[lpObj->Index]->ObjectStruct[n] = new MESSAGE_STATE_ATTACK_MACHINE;
			}

			gSMAttackProcMsg[lpObj->Index][n].MsgCode = MsgCode;

			gSMAttackProcMsg[lpObj->Index][n].MsgTime = GetTickCount() + MsgTimeDelay;

			gSMAttackProcMsg[lpObj->Index][n].SendUser = SendUser;

			gSMAttackProcMsg[lpObj->Index][n].SubCode = SubCode;

			gSMAttackProcMsg[lpObj->Index][n].SubCode2 = SubCode2;

			return;
		}
	}
}

//**************************************************************************//
// RAW FUNCTIONS ***********************************************************//
//**************************************************************************//

void gObjSecondProc()
{
	LPOBJ lpObj;

	for (int n = 0; n < MAX_OBJECT; n++)
	{
		lpObj = &gObj[n];

		if (lpObj->Connected > OBJECT_LOGGED)
		{
			gObjSkillUseProc(lpObj);

			if (lpObj->Type == OBJECT_MONSTER)
			{
				if (lpObj->MonsterDeleteTime != 0 && GetTickCount() >= lpObj->MonsterDeleteTime)
				{
					gObjDel(lpObj->Index);

					continue;
				}
			}

			if (lpObj->Type == OBJECT_USER)
			{
				gObjCheckMapTile(lpObj, 3);

				GCHealthBarSend(lpObj->Index);

				gEventTimeManager.GCEventTimeSend(lpObj->Index);

				if (lpObj->ChatLimitTime > 0)
				{
					lpObj->ChatLimitTimeSec++;

					if (lpObj->ChatLimitTimeSec > 60)
					{
						lpObj->ChatLimitTimeSec = 0;

						lpObj->ChatLimitTime--;

						if (lpObj->ChatLimitTime < 1)
						{
							lpObj->ChatLimitTime = 0;
						}
					}
				}

				gObjectManager.CharacterAutoRecuperation(lpObj);

				if (lpObj->Type == OBJECT_USER && lpObj->LastTeleportTime > 0)
				{
					lpObj->LastTeleportTime--;
				}

				gObjDelayLifeCheck(n);

				gObjectManager.CharacterItemDurationDown(lpObj);

				if (lpObj->PartyNumber >= 0)
				{
					gParty.GCPartyLifeSend(lpObj->PartyNumber);
				}

				int BattleGround = 1;

				//int m_BattleTimer = gBattleSoccer.CheckBattleGroundTimer();

				if (gGameMaster.CheckGameMasterLevel(lpObj, 1) != 0)
				{
					BattleGround = 0;
				}

				if (lpObj->Map == MAP_ARENA && BattleGround == 1)
				{
					BattleGround = 1;

					if (lpObj->Guild != 0)
					{
						if (lpObj->Guild->WarState != GUILD_WAR_STATE_NONE)
						{
							int CheckBattleGround = gBattleSoccer.CheckBattleGround(lpObj);

							if (CheckBattleGround != lpObj->Guild->BattleGroundIndex)
							{
								if (lpObj->Guild->WarType == GUILD_WAR_TYPE_SOCCER)
								{
									BattleGround = 0;
								}
								else
								{
									BattleGround = 0;
								}
							}
							else
							{
								BattleGround = 0;
							}
						}
					}
					else
					{
						BattleGround = 1;
					}

					if (BattleGround != 0)
					{
						int CheckBattleGround = gBattleSoccer.CheckBattleGround(lpObj);

						if (CheckBattleGround >= 0)
						{
							gObjMoveGate(lpObj->Index, 17);
						}
					}
				}
			}
		}

		if (lpObj->Connected >= OBJECT_LOGGED && lpObj->Type == OBJECT_USER && lpObj->CloseCount > 0)
		{
			if (lpObj->CloseCount == 1)
			{
				if (lpObj->CloseType == 1)
				{
					if (gObjectManager.CharacterGameClose(lpObj->Index) == 1)
					{
						GCCloseClientSend(lpObj->Index, 1);
					}
				}
				else if (lpObj->CloseType == 0)
				{
					GCCloseClientSend(lpObj->Index, 0);

					CloseClient(lpObj->Index);
				}
				else if (lpObj->CloseType == 2)
				{
					GCCloseClientSend(lpObj->Index, 2);
				}
				else
				{
					GCCloseClientSend(lpObj->Index, 0);
				}
			}
			else
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(53, lpObj->Lang), (lpObj->CloseCount - 1));
			}

			lpObj->CloseCount--;
		}

		if (lpObj->Connected > OBJECT_LOGGED && lpObj->Type == OBJECT_USER)
		{
			if (GetTickCount() - lpObj->AutoSaveTime > 600000)
			{
				GDCharacterInfoSaveSend(lpObj->Index);

				lpObj->AutoSaveTime = GetTickCount();
			}

			if (lpObj->CheckSumTime > 0 && GetTickCount() - lpObj->CheckSumTime > 5000)
			{
				LogAdd(LOG_BLACK, "[%s][%s] CheckSumTime Error", lpObj->Account, lpObj->Name);

				GCCloseClientSend(n, 0);
			}

			gObjPKDownCheckTime(lpObj, 1);

			gObjInterfaceCheckTime(lpObj);

			gObjTimeCheckSelfDefense(lpObj);
		}

		if (lpObj->Connected == OBJECT_CONNECTED || lpObj->Connected == OBJECT_LOGGED || lpObj->Connected == OBJECT_ONLINE)
		{
			if (lpObj->Type == OBJECT_USER)
			{
				if (lpObj->Connected >= OBJECT_LOGGED)
				{
					if (GetTickCount() - lpObj->ConnectTickCount > 60000)
					{
						CloseClient(n);

						LogAdd(LOG_BLACK, "Game response error causes conclusion [%d][%s][%s][%s]", lpObj->Index, lpObj->Account, lpObj->Name, lpObj->IpAddr);
					}
				}
				else
				{
					if (GetTickCount() - lpObj->ConnectTickCount > 30000)
					{
						CloseClient(n);

						LogAdd(LOG_BLACK, "Response error after connection causes conclusion [%d][%s][%s][%s]", lpObj->Index, lpObj->Account, lpObj->Name, lpObj->IpAddr);
					}
				}
			}
		}
	}
}

void gObjDelayLifeCheck(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->CheckLifeTime > 0)
	{
		lpObj->CheckLifeTime--;

		if (lpObj->CheckLifeTime <= 0)
		{
			lpObj->CheckLifeTime = 0;

			if (lpObj->Life < 0)
			{
				lpObj->Life = 0;
			}

			if (lpObj->AttackObj != 0)
			{
				gObjectManager.CharacterLifeCheck(lpObj->AttackObj, lpObj, 0, 1, 0, DAMAGE_TYPE_NORMAL, 0);
			}
		}
	}
}

BOOL gObjBackSpring(LPOBJ lpObj, LPOBJ lpTarget)
{
	LPOBJ lpTargetObj = lpTarget;

	int tdir;

	if (MAP_RANGE(lpObj->Map) == FALSE)
	{
		return FALSE;
	}

	if (lpObj->Type == OBJECT_USER)
	{
		if (lpObj->Teleport != 0)
		{
			return FALSE;
		}
	}

	if (lpObj->Class >= 131 && lpObj->Class <= 134)
	{
		return FALSE;
	}

	if ((GetLargeRand() % 3) == 0)
	{
		if (lpTargetObj->Dir < 4)
		{
			tdir = lpTargetObj->Dir + 4;
		}
		else
		{
			tdir = lpTargetObj->Dir - 4;
		}

		tdir *= 2;
	}
	else
	{
		tdir = lpTargetObj->Dir * 2;
	}

	int x;

	int y;

	BYTE attr;

	x = lpObj->X;

	y = lpObj->Y;

	x += RoadPathTable[tdir];

	y += RoadPathTable[1 + tdir];

	attr = gMap[lpObj->Map].GetAttr(x, y);

	if ((attr & 1) == 1 || (attr & 2) == 2 || (attr & 4) == 4 || (attr & 8) == 8 || (attr & 16) == 16)
	{
		return FALSE;
	}

	PMSG_POSITION_RECV pMsg;

	pMsg.header.set(PROTOCOL_CODE3, sizeof(pMsg));

	pMsg.x = x;

	pMsg.y = y;

	lpObj->Rest = 0;

	gMap[lpObj->Map].DelStandAttr(lpObj->OldX, lpObj->OldY);

	gMap[lpObj->Map].SetStandAttr(x, y);

	lpObj->OldX = x;

	lpObj->OldY = y;

	CGPositionRecv(&pMsg, lpObj->Index);

	if (lpObj->Type == OBJECT_USER)
	{
		lpObj->PathCount = 0;
	}

	return TRUE;
}

BOOL BackSpringCheck(int& x, int& y, int& dir, BYTE map)
{
	BYTE attr;

	int tx = x;

	int ty = y;

	int tdir = dir / 2;

	tx = tx + RoadPathTable[dir];

	ty = ty + RoadPathTable[1 + dir];

	attr = gMap[map].GetAttr(tx, ty);

	if ((attr & 1) == 1 || (attr & 4) == 4 || (attr & 8) == 8 || (attr & 16) == 16)
	{
		tdir += 4;

		if (tdir > 7)
		{
			tdir -= 8;
		}

		dir = tdir * 2;

		return FALSE;
	}

	x = tx;

	y = ty;

	return TRUE;
}

BOOL gObjBackSpring2(LPOBJ lpObj, LPOBJ lpTargetObj, int count)
{
	int tdir;

	if (MAP_RANGE(lpObj->Map) == FALSE)
	{
		return FALSE;
	}

	if (lpObj->Type == OBJECT_USER)
	{
		if (lpObj->Teleport != 0)
		{
			return FALSE;
		}
	}

	if (lpObj->Class >= 131 && lpObj->Class <= 134)
	{
		return FALSE;
	}

	tdir = GetPathPacketDirPos(lpObj->X - lpTargetObj->X, lpObj->Y - lpTargetObj->Y) * 2;

	int x = lpObj->X;

	int y = lpObj->Y;

	for (int n = 0; n < count; n++)
	{
		if (n >= 2)
		{
			if (lpObj->Class == 4 || lpObj->Class == 3 || lpObj->Class == 1)
			{
				BackSpringCheck(x, y, tdir, lpObj->Map);
			}
		}
		else
		{
			BackSpringCheck(x, y, tdir, lpObj->Map);
		}
	}

	PMSG_POSITION_RECV pMsg;

	pMsg.header.set(PROTOCOL_CODE3, sizeof(pMsg));

	pMsg.x = x;

	pMsg.y = y;

	lpObj->Rest = 0;

	gMap[lpObj->Map].DelStandAttr(lpObj->OldX, lpObj->OldY);

	gMap[lpObj->Map].SetStandAttr(x, y);

	lpObj->OldX = x;

	lpObj->OldY = y;

	CGPositionRecv(&pMsg, lpObj->Index);

	return TRUE;
}

bool gObjIsSelfDefense(LPOBJ lpObj, int aTargetIndex)
{
	if (!OBJECT_RANGE(aTargetIndex))
	{
		return false;
	}

	for (int n = 0; n < MAX_SELF_DEFENSE; n++)
	{
		if (lpObj->SelfDefense[n] >= 0)
		{
			if (lpObj->SelfDefense[n] == aTargetIndex)
			{
				return true;
			}
		}
	}

	return false;
}

void gObjCheckSelfDefense(LPOBJ lpObj, int aTargetIndex)
{
	if (!OBJECT_RANGE(aTargetIndex))
	{
		return;
	}

	if (gObj[aTargetIndex].PKLevel > PKLVL_WARNING)
	{
		return;
	}

	int iPartyNumber = lpObj->PartyNumber;

	int n;

	for (n = 0; n < MAX_SELF_DEFENSE; n++)
	{
		if (gObj[aTargetIndex].SelfDefense[n] >= 0)
		{
			if (gObj[aTargetIndex].SelfDefense[n] == lpObj->Index)
			{
				return;
			}

			if (iPartyNumber >= 0)
			{
				if (gObj[gObj[aTargetIndex].SelfDefense[n]].PartyNumber == iPartyNumber)
				{
					return;
				}
			}
		}
	}

	int blank = -1;

	for (n = 0; n < MAX_SELF_DEFENSE; n++)
	{
		if (lpObj->SelfDefense[n] >= 0)
		{
			if (lpObj->SelfDefense[n] == aTargetIndex)
			{
				lpObj->SelfDefenseTime[n] = GetTickCount() + 60000;

				return;
			}
		}
		else
		{
			blank = n;
		}
	}

	if (blank < 0)
	{
		return;
	}

	lpObj->MySelfDefenseTime = GetTickCount();

	lpObj->SelfDefense[blank] = aTargetIndex;

	lpObj->SelfDefenseTime[blank] = GetTickCount() + 60000;

	char szTemp[64];

	iPartyNumber = gObj[aTargetIndex].PartyNumber;

	if (iPartyNumber >= 0)
	{
		int iPartyNumIndex = -1;

		for (n = 0; n < MAX_PARTY_USER; n++)
		{
			iPartyNumIndex = gParty.m_PartyInfo[iPartyNumber].Index[n];

			if (iPartyNumIndex >= 0)
			{
				wsprintf(szTemp, gMessage.GetTextMessage(55, gObj[iPartyNumIndex].Lang), lpObj->Name, gObj[aTargetIndex].Name);

				gNotice.GCNoticeSend(iPartyNumIndex, 1, szTemp);
			}
		}
	}
	else
	{
		wsprintf(szTemp, gMessage.GetTextMessage(55, gObj[aTargetIndex].Lang), lpObj->Name, gObj[aTargetIndex].Name);

		gNotice.GCNoticeSend(aTargetIndex, 1, szTemp);
	}

	wsprintf(szTemp, gMessage.GetTextMessage(55, lpObj->Lang), lpObj->Name, gObj[aTargetIndex].Name);

	gNotice.GCNoticeSend(lpObj->Index, 1, szTemp);

	LogAdd(LOG_BLACK, "[%s][%s] Set SelfDefence [%s][%s]", lpObj->Account, lpObj->Name, gObj[aTargetIndex].Account, gObj[aTargetIndex].Name);
}

void gObjTimeCheckSelfDefense(LPOBJ lpObj)
{
	char szTemp[64];

	for (int n = 0; n < MAX_SELF_DEFENSE; n++)
	{
		if (lpObj->SelfDefense[n] >= 0)
		{
			if (GetTickCount() > lpObj->SelfDefenseTime[n])
			{
				wsprintf(szTemp, gMessage.GetTextMessage(56, lpObj->Lang), lpObj->Name);

				gNotice.GCNoticeSend(lpObj->Index, 1, szTemp);

				gNotice.GCNoticeSend(lpObj->SelfDefense[n], 1, szTemp);

				LogAdd(LOG_BLACK, "[%s][%s] ReSet SelfDefence [%s][%s]", lpObj->Account, lpObj->Name, gObj[lpObj->SelfDefense[n]].Account, gObj[lpObj->SelfDefense[n]].Name);

				lpObj->SelfDefense[n] = -1;
			}
		}
	}
}

void gObjSetPosition(int aIndex, int x, int y)
{
	LPOBJ lpObj = &gObj[aIndex];

	PMSG_POSITION_RECV pMove;

	pMove.header.set(PROTOCOL_CODE3, sizeof(pMove));

	pMove.x = x;

	pMove.y = y;

	lpObj->Rest = 0;

	gMap[lpObj->Map].DelStandAttr(lpObj->OldX, lpObj->OldY);

	gMap[lpObj->Map].SetStandAttr(x, y);

	lpObj->OldX = x;

	lpObj->OldY = y;

	CGPositionRecv(&pMove, lpObj->Index);
}

void gObjAuthorityCodeSet(LPOBJ lpObj)
{
	if ((lpObj->Authority & 1) == 1)
	{
		return;
	}

	if ((lpObj->Authority & 2) == 2)
	{
		lpObj->AuthorityCode = -1;

		return;
	}

	if ((lpObj->Authority & 4) == 4)
	{
		return;
	}

	if ((lpObj->Authority & 8) == 8)
	{
		lpObj->AuthorityCode |= 1;

		lpObj->AuthorityCode |= 2;

		lpObj->AuthorityCode |= 4;

		lpObj->AuthorityCode |= 8;

		lpObj->AuthorityCode |= 16;

		lpObj->AuthorityCode |= 32;

		return;
	}

	if ((lpObj->Authority & 16) == 16)
	{
		return;
	}

	if ((lpObj->Authority & 32) == 32)
	{
		lpObj->AuthorityCode |= 1;

		lpObj->AuthorityCode |= 2;

		lpObj->AuthorityCode |= 4;

		lpObj->AuthorityCode |= 8;

		lpObj->AuthorityCode |= 16;

		lpObj->AuthorityCode |= 32;

		return;
	}

}

void gObjUseDrink(LPOBJ lpObj, int level)
{
	switch (level)
	{
		case 2: // Remedy of Love
		{
			GCItemUseSpecialTimeSend(lpObj->Index, 1, 90);

			break;
		}

		case 1: //ALE+1
		{
			GCItemUseSpecialTimeSend(lpObj->Index, 0, 180);

			lpObj->DrinkSpeed = 20;

			lpObj->DrinkLastTime = GetTickCount() + (180 * 1000);

			gObjectManager.CharacterCalcAttribute(lpObj->Index);

			break;
		}

		default: //ALE+0
		{
			GCItemUseSpecialTimeSend(lpObj->Index, 0, 80);

			lpObj->DrinkSpeed = 20;

			lpObj->DrinkLastTime = GetTickCount() + (80 * 1000);

			gObjectManager.CharacterCalcAttribute(lpObj->Index);

			break;
		}
	}
}

void gObjCustomLogPlusChaosMix(LPOBJ lpObj, int type, int index)
{
	if (type != 1)
	{
		return;
	}

	if (gServerInfo.m_AnnounceChaosMix != 0)
	{
		ITEM_INFO ItemInfo;

		if (gItemManager.GetInfo(index, &ItemInfo) != 0)
		{
			char buff[256];

			wsprintf(buff, gServerInfo.m_AnnounceChaosMixText, lpObj->Name, ItemInfo.Name);

			if (gServerInfo.m_AnnounceChaosMix == 1)
			{
				gNotice.GCNoticeSendToAll(0, buff);
			}
			else
			{
				GDGlobalNoticeSend(0, buff);
			}
		}
	}
}

bool gObjCheckAutoParty(LPOBJ lpObj, LPOBJ lpTarget)
{
	if ((lpTarget->Option & 4) == 0)
	{
		return 0;
	}

	if (lpTarget->AutoPartyPassword[0] != 0 && strcmp(lpObj->AutoPartyPassword, lpTarget->AutoPartyPassword) != 0)
	{
		return 0;
	}

	return 1;
}