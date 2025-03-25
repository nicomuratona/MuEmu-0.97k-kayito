#include "stdafx.h"
#include "ServerInfo.h"
#include "Blacklist.h"
#include "BloodCastle.h"
#include "BonusManager.h"
#include "CommandManager.h"
#include "CustomNpcMove.h"
#include "CustomPkFree.h"
#include "CustomSafeZone.h"
#include "CustomWing.h"
#include "DefaultClassInfo.h"
#include "DevilSquare.h"
#include "EffectManager.h"
#include "EventSpawnMonster.h"
#include "ExperienceTable.h"
#include "Filter.h"
#include "GameMaster.h"
#include "Gate.h"
#include "GoldenArcher.h"
#include "GoldenArcherBingo.h"
#include "HackPacketCheck.h"
#include "InvasionManager.h"
#include "ItemBagManager.h"
#include "ItemDrop.h"
#include "ItemMove.h"
#include "ItemOption.h"
#include "ItemOptionRate.h"
#include "ItemStack.h"
#include "ItemValue.h"
#include "Map.h"
#include "MapManager.h"
#include "Message.h"
#include "MonsterManager.h"
#include "MonsterSetBase.h"
#include "Move.h"
#include "Notice.h"
#include "PacketManager.h"
#include "Path.h"
#include "Quest.h"
#include "QuestObjective.h"
#include "QuestReward.h"
#include "ResetTable.h"
#include "ShopManager.h"
#include "SkillHitBox.h"
#include "SkillManager.h"
#include "Util.h"

CServerInfo gServerInfo;

CServerInfo::CServerInfo()
{

}

CServerInfo::~CServerInfo()
{

}

void CServerInfo::ReadInit()
{
	gPath.SetMainPath(MAIN_PATH);

	this->ReadChaosMixInfo();

	this->ReadCharacterInfo();

	this->ReadCommandInfo();

	this->ReadCommonInfo();

	this->ReadCustomInfo();

	this->ReadEventInfo();

	this->ReadEventItemBagInfo();

	this->ReadHackInfo();

	this->ReadItemInfo();

	this->ReadMapInfo();

	this->ReadMonsterInfo();

	this->ReadMoveInfo();

	this->ReadQuestInfo();

	this->ReadShopInfo();

	this->ReadSkillInfo();

	this->ReadUtilInfo();
}

void CServerInfo::ReloadAll()
{
	this->ReadChaosMixInfo();

	this->ReadCharacterInfo();

	this->ReadCommandInfo();

	this->ReadCommonInfo();

	this->ReadCustomInfo();

	this->ReadEventInfo();

	this->ReadEventItemBagInfo();

	this->ReadHackInfo();

	this->ReadItemInfo();

	this->ReadMonsterInfo();

	this->ReadMoveInfo();

	this->ReadQuestInfo();

	this->ReadShopInfo();

	this->ReadSkillInfo();

	this->ReadUtilInfo();
}

void CServerInfo::ReadChaosMixInfo()
{
	this->ReadChaosMixInfo("GameServerInfo", ".\\Data\\GameServerInfo - ChaosMix.dat");

	LogAdd(LOG_BLUE, "[ServerInfo] ChaosMix loaded successfully");
}

void CServerInfo::ReadCharacterInfo()
{
	this->ReadCharacterInfo("GameServerInfo", ".\\Data\\GameServerInfo - Character.dat");

	gDefaultClassInfo.Load(gPath.GetFullPath("Character\\DefaultClassInfo.txt"));

	LogAdd(LOG_BLUE, "[ServerInfo] Character loaded successfully");
}

void CServerInfo::ReadCommandInfo()
{
	this->ReadCommandInfo("GameServerInfo", ".\\Data\\GameServerInfo - Command.dat");

	gCommandManager.Load(gPath.GetFullPath("Command.txt"));

	LogAdd(LOG_BLUE, "[ServerInfo] Command loaded successfully");
}

void CServerInfo::ReadCommonInfo()
{
	this->ReadCommonInfo("GameServerInfo", ".\\Data\\GameServerInfo - Common.dat");

	gEffectManager.Load(gPath.GetFullPath("Effect.txt"));

	gMapManager.Load(gPath.GetFullPath("MapManager.txt"));

	gMessage.Load(gPath.GetFullPath("Message_Eng.txt"), LANGUAGE_ENGLISH);
	gMessage.Load(gPath.GetFullPath("Message_Spn.txt"), LANGUAGE_SPANISH);
	gMessage.Load(gPath.GetFullPath("Message_Por.txt"), LANGUAGE_PORTUGUESE);

	gObjSetExperienceTable();

	LogAdd(LOG_BLUE, "[ServerInfo] Common loaded successfully");
}

void CServerInfo::ReadCustomInfo()
{
	gCustomNpcMove.Load(gPath.GetFullPath("Custom\\CustomNpcMove.txt"));

	gPKFree.Load(gPath.GetFullPath("Custom\\CustomPkFree.txt"));

	gSafeZone.Load(gPath.GetFullPath("Custom\\CustomSafeZone.txt"));

	LogAdd(LOG_BLUE, "[ServerInfo] Custom loaded successfully");
}

void CServerInfo::ReadEventInfo()
{
	this->ReadEventInfo("GameServerInfo", ".\\Data\\GameServerInfo - Event.dat");

	gGoldenArcher.ReadArcherInfo("GameServerInfo", ".\\Data\\GameServerInfo - Event.dat");

	gEventSpawnMonster.Load(gPath.GetFullPath("Event\\EventSpawnMonster.dat"));

	gBloodCastle.Load(gPath.GetFullPath("Event\\BloodCastle.dat"));

	gBonusManager.Load(gPath.GetFullPath("Event\\BonusManager.dat"));

	gDevilSquare.Load(gPath.GetFullPath("Event\\DevilSquare.dat"));

	gInvasionManager.Load(gPath.GetFullPath("Event\\InvasionManager.dat"));

	gGoldenArcherBingo.Load(gPath.GetFullPath("Event\\GoldenArcherBingo.dat"));

	LogAdd(LOG_BLUE, "[ServerInfo] Event loaded successfully");
}

void CServerInfo::ReadEventItemBagInfo()
{
	gItemBagManager.Load(gPath.GetFullPath("EventItemBagManager.txt"));

	gItemBagManager.LoadEventItemBag();

	LogAdd(LOG_BLUE, "[ServerInfo] EventItemBag loaded successfully");
}

void CServerInfo::ReadHackInfo()
{
	this->ReadHackInfo("GameServerInfo", ".\\Data\\GameServerInfo - Common.dat");

	gHackPacketCheck.Load(gPath.GetFullPath("Hack\\HackPacketCheck.txt"));

	gPacketManager.LoadEncryptionKey(gPath.GetFullPath("Hack\\Enc2.dat"));

	gPacketManager.LoadDecryptionKey(gPath.GetFullPath("Hack\\Dec1.dat"));

	LogAdd(LOG_BLUE, "[ServerInfo] Hack loaded successfully");
}

void CServerInfo::ReadItemInfo()
{
	gItemManager.Load(gPath.GetFullPath("Item\\Item.txt"));

	gItemDrop.Load(gPath.GetFullPath("Item\\ItemDrop.txt"));

	gItemMove.Load(gPath.GetFullPath("Item\\ItemMove.txt"));

	gItemOption.Load(gPath.GetFullPath("Item\\ItemOption.txt"));

	gItemOptionRate.Load(gPath.GetFullPath("Item\\ItemOptionRate.txt"));

	gItemStack.Load(gPath.GetFullPath("Item\\ItemStack.txt"));

	gItemValue.Load(gPath.GetFullPath("Item\\ItemValue.txt"));

	gCustomWing.Load(gPath.GetFullPath("Item\\CustomWing.txt"));

	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnectedGP(n) != 0)
		{
			gItemStack.GCItemStackListSend(gObj[n].Index);

			gItemValue.GCItemValueListSend(gObj[n].Index);
		}
	}

	LogAdd(LOG_BLUE, "[ServerInfo] Item loaded successfully");
}

void CServerInfo::ReadMapInfo()
{
	FILE* file;

	char path[256] = { 0 };

	char temp[256] = { 0 };

	for (int n = 0; n < MAX_MAP; n++)
	{
		wsprintf(temp, "Terrain\\terrain%d.att", (n + 1));

		if (fopen_s(&file, gPath.GetFullPath(temp), "r") == 0)
		{
			strcpy_s(path, temp);

			fclose(file);
		}

		gMap[n].Load(gPath.GetFullPath(path), n);
	}
}

void CServerInfo::ReadMonsterInfo()
{
	gMonsterManager.Load(gPath.GetFullPath("Monster\\Monster.txt"));

	gMonsterSetBase.Load(gPath.GetFullPath("Monster\\MonsterSetBase.txt"));

	LogAdd(LOG_BLUE, "[ServerInfo] Monster loaded successfully");
}

void CServerInfo::ReadMoveInfo()
{
	gGate.Load(gPath.GetFullPath("Move\\Gate.txt"));

	gMove.Load(gPath.GetFullPath("Move\\Move.txt"));

	gMove.ReloadMoveList();

	LogAdd(LOG_BLUE, "[ServerInfo] Move loaded successfully");
}

void CServerInfo::ReadQuestInfo()
{
	gQuest.Load(gPath.GetFullPath("Quest\\Quest.txt"));

	gQuestObjective.Load(gPath.GetFullPath("Quest\\QuestObjective.txt"));

	gQuestReward.Load(gPath.GetFullPath("Quest\\QuestReward.txt"));

	LogAdd(LOG_BLUE, "[ServerInfo] Quest loaded successfully");
}

void CServerInfo::ReadShopInfo()
{
	gShopManager.Load(gPath.GetFullPath("ShopManager.txt"));

	gShopManager.LoadShop();

	gShopManager.ReloadShopInterface();

	LogAdd(LOG_BLUE, "[ServerInfo] Shop loaded successfully");
}

void CServerInfo::ReadSkillInfo()
{
	this->ReadSkillInfo("GameServerInfo", ".\\Data\\GameServerInfo - Skill.dat");

	gSkillHitBox.Load(gPath.GetFullPath("Skill\\SkillElect.hit"));

	gSkillManager.Load(gPath.GetFullPath("Skill\\Skill.txt"));

	LogAdd(LOG_BLUE, "[ServerInfo] Skill loaded successfully");
}

void CServerInfo::ReadUtilInfo()
{
	gExperienceTable.Load(gPath.GetFullPath("Util\\ExperienceTable.txt"));

	gFilter.Load(gPath.GetFullPath("Util\\Filter.txt"));

	gGameMaster.Load(gPath.GetFullPath("Util\\GameMaster.txt"));

	gNotice.Load(gPath.GetFullPath("Util\\Notice.txt"));

	gResetTable.Load(gPath.GetFullPath("Util\\ResetTable.txt"));

	gBlackList.Load(gPath.GetFullPath("Util\\BlackList.dat"));

	LogAdd(LOG_BLUE, "[ServerInfo] Util loaded successfully");
}

void CServerInfo::ReloadMonsterInfo()
{
	for (int n = 0; n < MAX_OBJECT_MONSTER; n++)
	{
		if (gObj[n].Type != OBJECT_MONSTER && gObj[n].Type != OBJECT_NPC)
		{
			continue;
		}

		gObjDel(n);
	}

	gObjMonCount = 0;

	this->ReadMonsterInfo();

	gMonsterManager.SetMonsterData();
}

void CServerInfo::ReadStartupInfo(char* section, char* path)
{
	char GameServerInfo[100] = { 0 };

	GetPrivateProfileString(section, "CustomerName", "", this->m_CustomerName, sizeof(this->m_CustomerName), path);

	GetPrivateProfileString(section, "ServerName", "", this->m_ServerName, sizeof(this->m_ServerName), path);

	GetPrivateProfileString(section, "WindowName", "", this->m_WindowName, sizeof(this->m_WindowName), path);

	this->m_ServerCode = GetPrivateProfileInt(section, "ServerCode", 0, path);

	this->m_ServerLock = GetPrivateProfileInt(section, "ServerLock", 0, path);

	this->m_ServerPort = GetPrivateProfileInt(section, "ServerPort", 0, path);

	wsprintf(this->m_ServerMutex, "WZ_MU_GS_MUTEX_%d", this->m_ServerCode);

	GetPrivateProfileString(section, "ServerVersion", "", GameServerInfo, sizeof(GameServerInfo), path);

	gServerInfo.m_ServerVersion[0] = GameServerInfo[0];

	gServerInfo.m_ServerVersion[1] = GameServerInfo[2];

	gServerInfo.m_ServerVersion[2] = GameServerInfo[3];

	gServerInfo.m_ServerVersion[3] = GameServerInfo[5];

	gServerInfo.m_ServerVersion[4] = GameServerInfo[6];

	gServerInfo.m_ServerVersion[5] = 0;

	GetPrivateProfileString(section, "ServerSerial", "", this->m_ServerSerial, sizeof(this->m_ServerSerial), path);

	this->m_ServerMaxUserNumber = GetPrivateProfileInt(section, "ServerMaxUserNumber", 0, path);

	this->m_ServerMaxUserNumber = ((this->m_ServerMaxUserNumber > MAX_OBJECT_USER) ? MAX_OBJECT_USER : this->m_ServerMaxUserNumber);

	GetPrivateProfileString(section, "DataServerAddress", "", this->m_DataServerAddress, sizeof(this->m_DataServerAddress), path);

	this->m_DataServerPort = GetPrivateProfileInt(section, "DataServerPort", 0, path);

	GetPrivateProfileString(section, "JoinServerAddress", "", this->m_JoinServerAddress, sizeof(this->m_JoinServerAddress), path);

	this->m_JoinServerPort = GetPrivateProfileInt(section, "JoinServerPort", 0, path);

	GetPrivateProfileString(section, "ConnectServerAddress", "", this->m_ConnectServerAddress, sizeof(this->m_ConnectServerAddress), path);

	this->m_ConnectServerPort = GetPrivateProfileInt(section, "ConnectServerPort", 0, path);

	this->m_WriteGeneralLog = (GetPrivateProfileInt(section, "WriteGeneralLog", 0, path) != 0);

	this->m_WriteChatLog = (GetPrivateProfileInt(section, "WriteChatLog", 0, path) != 0);

	this->m_WriteCommandLog = (GetPrivateProfileInt(section, "WriteCommandLog", 0, path) != 0);

	this->m_WriteTradeLog = (GetPrivateProfileInt(section, "WriteTradeLog", 0, path) != 0);

	this->m_WriteConnectLog = (GetPrivateProfileInt(section, "WriteConnectLog", 0, path) != 0);

	this->m_WriteHackLog = (GetPrivateProfileInt(section, "WriteHackLog", 0, path) != 0);

	this->m_WriteChaosMixLog = (GetPrivateProfileInt(section, "WriteChaosMixLog", 0, path) != 0);
}

void CServerInfo::ReadChaosMixInfo(char* section, char* path)
{
	this->m_ChaosItemMixRate[0] = GetPrivateProfileInt(section, "ChaosItemMixRate_AL0", 0, path);

	this->m_ChaosItemMixRate[1] = GetPrivateProfileInt(section, "ChaosItemMixRate_AL1", 0, path);

	this->m_ChaosItemMixRate[2] = GetPrivateProfileInt(section, "ChaosItemMixRate_AL2", 0, path);

	this->m_ChaosItemMixRate[3] = GetPrivateProfileInt(section, "ChaosItemMixRate_AL3", 0, path);

	this->m_ChaosItemMixCost[0] = GetPrivateProfileInt(section, "ChaosItemMixCost_AL0", 0, path);

	this->m_ChaosItemMixCost[1] = GetPrivateProfileInt(section, "ChaosItemMixCost_AL1", 0, path);

	this->m_ChaosItemMixCost[2] = GetPrivateProfileInt(section, "ChaosItemMixCost_AL2", 0, path);

	this->m_ChaosItemMixCost[3] = GetPrivateProfileInt(section, "ChaosItemMixCost_AL3", 0, path);

	this->m_ChaosItemMixAddItemRate[0] = GetPrivateProfileInt(section, "ChaosItemMixAddItemRate_AL0", 0, path);

	this->m_ChaosItemMixAddItemRate[1] = GetPrivateProfileInt(section, "ChaosItemMixAddItemRate_AL1", 0, path);

	this->m_ChaosItemMixAddItemRate[2] = GetPrivateProfileInt(section, "ChaosItemMixAddItemRate_AL2", 0, path);

	this->m_ChaosItemMixAddItemRate[3] = GetPrivateProfileInt(section, "ChaosItemMixAddItemRate_AL3", 0, path);

	this->m_ChaosItemMixAddJewelRate[0] = GetPrivateProfileInt(section, "ChaosItemMixAddJewelRate_AL0", 0, path);

	this->m_ChaosItemMixAddJewelRate[1] = GetPrivateProfileInt(section, "ChaosItemMixAddJewelRate_AL1", 0, path);

	this->m_ChaosItemMixAddJewelRate[2] = GetPrivateProfileInt(section, "ChaosItemMixAddJewelRate_AL2", 0, path);

	this->m_ChaosItemMixAddJewelRate[3] = GetPrivateProfileInt(section, "ChaosItemMixAddJewelRate_AL3", 0, path);

	this->m_DevilSquareMixRate1[0] = GetPrivateProfileInt(section, "DevilSquareMixRate1_AL0", 0, path);

	this->m_DevilSquareMixRate1[1] = GetPrivateProfileInt(section, "DevilSquareMixRate1_AL1", 0, path);

	this->m_DevilSquareMixRate1[2] = GetPrivateProfileInt(section, "DevilSquareMixRate1_AL2", 0, path);

	this->m_DevilSquareMixRate1[3] = GetPrivateProfileInt(section, "DevilSquareMixRate1_AL3", 0, path);

	this->m_DevilSquareMixRate2[0] = GetPrivateProfileInt(section, "DevilSquareMixRate2_AL0", 0, path);

	this->m_DevilSquareMixRate2[1] = GetPrivateProfileInt(section, "DevilSquareMixRate2_AL1", 0, path);

	this->m_DevilSquareMixRate2[2] = GetPrivateProfileInt(section, "DevilSquareMixRate2_AL2", 0, path);

	this->m_DevilSquareMixRate2[3] = GetPrivateProfileInt(section, "DevilSquareMixRate2_AL3", 0, path);

	this->m_DevilSquareMixRate3[0] = GetPrivateProfileInt(section, "DevilSquareMixRate3_AL0", 0, path);

	this->m_DevilSquareMixRate3[1] = GetPrivateProfileInt(section, "DevilSquareMixRate3_AL1", 0, path);

	this->m_DevilSquareMixRate3[2] = GetPrivateProfileInt(section, "DevilSquareMixRate3_AL2", 0, path);

	this->m_DevilSquareMixRate3[3] = GetPrivateProfileInt(section, "DevilSquareMixRate3_AL3", 0, path);

	this->m_DevilSquareMixRate4[0] = GetPrivateProfileInt(section, "DevilSquareMixRate4_AL0", 0, path);

	this->m_DevilSquareMixRate4[1] = GetPrivateProfileInt(section, "DevilSquareMixRate4_AL1", 0, path);

	this->m_DevilSquareMixRate4[2] = GetPrivateProfileInt(section, "DevilSquareMixRate4_AL2", 0, path);

	this->m_DevilSquareMixRate4[3] = GetPrivateProfileInt(section, "DevilSquareMixRate4_AL3", 0, path);

	this->m_DevilSquareMixCost1[0] = GetPrivateProfileInt(section, "DevilSquareMixCost1_AL0", 0, path);

	this->m_DevilSquareMixCost1[1] = GetPrivateProfileInt(section, "DevilSquareMixCost1_AL1", 0, path);

	this->m_DevilSquareMixCost1[2] = GetPrivateProfileInt(section, "DevilSquareMixCost1_AL2", 0, path);

	this->m_DevilSquareMixCost1[3] = GetPrivateProfileInt(section, "DevilSquareMixCost1_AL3", 0, path);

	this->m_DevilSquareMixCost2[0] = GetPrivateProfileInt(section, "DevilSquareMixCost2_AL0", 0, path);

	this->m_DevilSquareMixCost2[1] = GetPrivateProfileInt(section, "DevilSquareMixCost2_AL1", 0, path);

	this->m_DevilSquareMixCost2[2] = GetPrivateProfileInt(section, "DevilSquareMixCost2_AL2", 0, path);

	this->m_DevilSquareMixCost2[3] = GetPrivateProfileInt(section, "DevilSquareMixCost2_AL3", 0, path);

	this->m_DevilSquareMixCost3[0] = GetPrivateProfileInt(section, "DevilSquareMixCost3_AL0", 0, path);

	this->m_DevilSquareMixCost3[1] = GetPrivateProfileInt(section, "DevilSquareMixCost3_AL1", 0, path);

	this->m_DevilSquareMixCost3[2] = GetPrivateProfileInt(section, "DevilSquareMixCost3_AL2", 0, path);

	this->m_DevilSquareMixCost3[3] = GetPrivateProfileInt(section, "DevilSquareMixCost3_AL3", 0, path);

	this->m_DevilSquareMixCost4[0] = GetPrivateProfileInt(section, "DevilSquareMixCost4_AL0", 0, path);

	this->m_DevilSquareMixCost4[1] = GetPrivateProfileInt(section, "DevilSquareMixCost4_AL1", 0, path);

	this->m_DevilSquareMixCost4[2] = GetPrivateProfileInt(section, "DevilSquareMixCost4_AL2", 0, path);

	this->m_DevilSquareMixCost4[3] = GetPrivateProfileInt(section, "DevilSquareMixCost4_AL3", 0, path);

	this->m_PlusItemLevelMixRate1[0] = GetPrivateProfileInt(section, "PlusItemLevelMixRate1_AL0", 0, path);

	this->m_PlusItemLevelMixRate1[1] = GetPrivateProfileInt(section, "PlusItemLevelMixRate1_AL1", 0, path);

	this->m_PlusItemLevelMixRate1[2] = GetPrivateProfileInt(section, "PlusItemLevelMixRate1_AL2", 0, path);

	this->m_PlusItemLevelMixRate1[3] = GetPrivateProfileInt(section, "PlusItemLevelMixRate1_AL3", 0, path);

	this->m_PlusItemLevelMixRate2[0] = GetPrivateProfileInt(section, "PlusItemLevelMixRate2_AL0", 0, path);

	this->m_PlusItemLevelMixRate2[1] = GetPrivateProfileInt(section, "PlusItemLevelMixRate2_AL1", 0, path);

	this->m_PlusItemLevelMixRate2[2] = GetPrivateProfileInt(section, "PlusItemLevelMixRate2_AL2", 0, path);

	this->m_PlusItemLevelMixRate2[3] = GetPrivateProfileInt(section, "PlusItemLevelMixRate2_AL3", 0, path);

	this->m_PlusItemLevelMixCost1[0] = GetPrivateProfileInt(section, "PlusItemLevelMixCost1_AL0", 0, path);

	this->m_PlusItemLevelMixCost1[1] = GetPrivateProfileInt(section, "PlusItemLevelMixCost1_AL1", 0, path);

	this->m_PlusItemLevelMixCost1[2] = GetPrivateProfileInt(section, "PlusItemLevelMixCost1_AL2", 0, path);

	this->m_PlusItemLevelMixCost1[3] = GetPrivateProfileInt(section, "PlusItemLevelMixCost1_AL3", 0, path);

	this->m_PlusItemLevelMixCost2[0] = GetPrivateProfileInt(section, "PlusItemLevelMixCost2_AL0", 0, path);

	this->m_PlusItemLevelMixCost2[1] = GetPrivateProfileInt(section, "PlusItemLevelMixCost2_AL1", 0, path);

	this->m_PlusItemLevelMixCost2[2] = GetPrivateProfileInt(section, "PlusItemLevelMixCost2_AL2", 0, path);

	this->m_PlusItemLevelMixCost2[3] = GetPrivateProfileInt(section, "PlusItemLevelMixCost2_AL3", 0, path);

	this->m_PlusItemLevelAddLuckRate[0] = GetPrivateProfileInt(section, "PlusItemLevelAddLuckRate_AL0", 0, path);

	this->m_PlusItemLevelAddLuckRate[1] = GetPrivateProfileInt(section, "PlusItemLevelAddLuckRate_AL1", 0, path);

	this->m_PlusItemLevelAddLuckRate[2] = GetPrivateProfileInt(section, "PlusItemLevelAddLuckRate_AL2", 0, path);

	this->m_PlusItemLevelAddLuckRate[3] = GetPrivateProfileInt(section, "PlusItemLevelAddLuckRate_AL3", 0, path);

	this->m_DinorantMixRate[0] = GetPrivateProfileInt(section, "DinorantMixRate_AL0", 0, path);

	this->m_DinorantMixRate[1] = GetPrivateProfileInt(section, "DinorantMixRate_AL1", 0, path);

	this->m_DinorantMixRate[2] = GetPrivateProfileInt(section, "DinorantMixRate_AL2", 0, path);

	this->m_DinorantMixRate[3] = GetPrivateProfileInt(section, "DinorantMixRate_AL3", 0, path);

	this->m_DinorantMixCost[0] = GetPrivateProfileInt(section, "DinorantMixCost_AL0", 0, path);

	this->m_DinorantMixCost[1] = GetPrivateProfileInt(section, "DinorantMixCost_AL1", 0, path);

	this->m_DinorantMixCost[2] = GetPrivateProfileInt(section, "DinorantMixCost_AL2", 0, path);

	this->m_DinorantMixCost[3] = GetPrivateProfileInt(section, "DinorantMixCost_AL3", 0, path);

	this->m_FruitMixRate[0] = GetPrivateProfileInt(section, "FruitMixRate_AL0", 0, path);

	this->m_FruitMixRate[1] = GetPrivateProfileInt(section, "FruitMixRate_AL1", 0, path);

	this->m_FruitMixRate[2] = GetPrivateProfileInt(section, "FruitMixRate_AL2", 0, path);

	this->m_FruitMixRate[3] = GetPrivateProfileInt(section, "FruitMixRate_AL3", 0, path);

	this->m_FruitMixCost[0] = GetPrivateProfileInt(section, "FruitMixCost_AL0", 0, path);

	this->m_FruitMixCost[1] = GetPrivateProfileInt(section, "FruitMixCost_AL1", 0, path);

	this->m_FruitMixCost[2] = GetPrivateProfileInt(section, "FruitMixCost_AL2", 0, path);

	this->m_FruitMixCost[3] = GetPrivateProfileInt(section, "FruitMixCost_AL3", 0, path);

	this->m_Wing1MixRate[0] = GetPrivateProfileInt(section, "Wing1MixRate_AL0", 0, path);

	this->m_Wing1MixRate[1] = GetPrivateProfileInt(section, "Wing1MixRate_AL1", 0, path);

	this->m_Wing1MixRate[2] = GetPrivateProfileInt(section, "Wing1MixRate_AL2", 0, path);

	this->m_Wing1MixRate[3] = GetPrivateProfileInt(section, "Wing1MixRate_AL3", 0, path);

	this->m_Wing1MixCost[0] = GetPrivateProfileInt(section, "Wing1MixCost_AL0", 0, path);

	this->m_Wing1MixCost[1] = GetPrivateProfileInt(section, "Wing1MixCost_AL1", 0, path);

	this->m_Wing1MixCost[2] = GetPrivateProfileInt(section, "Wing1MixCost_AL2", 0, path);

	this->m_Wing1MixCost[3] = GetPrivateProfileInt(section, "Wing1MixCost_AL3", 0, path);

	this->m_Wing1MixAddItemRate[0] = GetPrivateProfileInt(section, "Wing1MixAddItemRate_AL0", 0, path);

	this->m_Wing1MixAddItemRate[1] = GetPrivateProfileInt(section, "Wing1MixAddItemRate_AL1", 0, path);

	this->m_Wing1MixAddItemRate[2] = GetPrivateProfileInt(section, "Wing1MixAddItemRate_AL2", 0, path);

	this->m_Wing1MixAddItemRate[3] = GetPrivateProfileInt(section, "Wing1MixAddItemRate_AL3", 0, path);

	this->m_Wing1MixAddJewelRate[0] = GetPrivateProfileInt(section, "Wing1MixAddJewelRate_AL0", 0, path);

	this->m_Wing1MixAddJewelRate[1] = GetPrivateProfileInt(section, "Wing1MixAddJewelRate_AL1", 0, path);

	this->m_Wing1MixAddJewelRate[2] = GetPrivateProfileInt(section, "Wing1MixAddJewelRate_AL2", 0, path);

	this->m_Wing1MixAddJewelRate[3] = GetPrivateProfileInt(section, "Wing1MixAddJewelRate_AL3", 0, path);

	this->m_BloodCastleMixRate1[0] = GetPrivateProfileInt(section, "BloodCastleMixRate1_AL0", 0, path);

	this->m_BloodCastleMixRate1[1] = GetPrivateProfileInt(section, "BloodCastleMixRate1_AL1", 0, path);

	this->m_BloodCastleMixRate1[2] = GetPrivateProfileInt(section, "BloodCastleMixRate1_AL2", 0, path);

	this->m_BloodCastleMixRate1[3] = GetPrivateProfileInt(section, "BloodCastleMixRate1_AL3", 0, path);

	this->m_BloodCastleMixRate2[0] = GetPrivateProfileInt(section, "BloodCastleMixRate2_AL0", 0, path);

	this->m_BloodCastleMixRate2[1] = GetPrivateProfileInt(section, "BloodCastleMixRate2_AL1", 0, path);

	this->m_BloodCastleMixRate2[2] = GetPrivateProfileInt(section, "BloodCastleMixRate2_AL2", 0, path);

	this->m_BloodCastleMixRate2[3] = GetPrivateProfileInt(section, "BloodCastleMixRate2_AL3", 0, path);

	this->m_BloodCastleMixRate3[0] = GetPrivateProfileInt(section, "BloodCastleMixRate3_AL0", 0, path);

	this->m_BloodCastleMixRate3[1] = GetPrivateProfileInt(section, "BloodCastleMixRate3_AL1", 0, path);

	this->m_BloodCastleMixRate3[2] = GetPrivateProfileInt(section, "BloodCastleMixRate3_AL2", 0, path);

	this->m_BloodCastleMixRate3[3] = GetPrivateProfileInt(section, "BloodCastleMixRate3_AL3", 0, path);

	this->m_BloodCastleMixRate4[0] = GetPrivateProfileInt(section, "BloodCastleMixRate4_AL0", 0, path);

	this->m_BloodCastleMixRate4[1] = GetPrivateProfileInt(section, "BloodCastleMixRate4_AL1", 0, path);

	this->m_BloodCastleMixRate4[2] = GetPrivateProfileInt(section, "BloodCastleMixRate4_AL2", 0, path);

	this->m_BloodCastleMixRate4[3] = GetPrivateProfileInt(section, "BloodCastleMixRate4_AL3", 0, path);

	this->m_BloodCastleMixRate5[0] = GetPrivateProfileInt(section, "BloodCastleMixRate5_AL0", 0, path);

	this->m_BloodCastleMixRate5[1] = GetPrivateProfileInt(section, "BloodCastleMixRate5_AL1", 0, path);

	this->m_BloodCastleMixRate5[2] = GetPrivateProfileInt(section, "BloodCastleMixRate5_AL2", 0, path);

	this->m_BloodCastleMixRate5[3] = GetPrivateProfileInt(section, "BloodCastleMixRate5_AL3", 0, path);

	this->m_BloodCastleMixRate6[0] = GetPrivateProfileInt(section, "BloodCastleMixRate6_AL0", 0, path);

	this->m_BloodCastleMixRate6[1] = GetPrivateProfileInt(section, "BloodCastleMixRate6_AL1", 0, path);

	this->m_BloodCastleMixRate6[2] = GetPrivateProfileInt(section, "BloodCastleMixRate6_AL2", 0, path);

	this->m_BloodCastleMixRate6[3] = GetPrivateProfileInt(section, "BloodCastleMixRate6_AL3", 0, path);

	this->m_BloodCastleMixCost1[0] = GetPrivateProfileInt(section, "BloodCastleMixCost1_AL0", 0, path);

	this->m_BloodCastleMixCost1[1] = GetPrivateProfileInt(section, "BloodCastleMixCost1_AL1", 0, path);

	this->m_BloodCastleMixCost1[2] = GetPrivateProfileInt(section, "BloodCastleMixCost1_AL2", 0, path);

	this->m_BloodCastleMixCost1[3] = GetPrivateProfileInt(section, "BloodCastleMixCost1_AL3", 0, path);

	this->m_BloodCastleMixCost2[0] = GetPrivateProfileInt(section, "BloodCastleMixCost2_AL0", 0, path);

	this->m_BloodCastleMixCost2[1] = GetPrivateProfileInt(section, "BloodCastleMixCost2_AL1", 0, path);

	this->m_BloodCastleMixCost2[2] = GetPrivateProfileInt(section, "BloodCastleMixCost2_AL2", 0, path);

	this->m_BloodCastleMixCost2[3] = GetPrivateProfileInt(section, "BloodCastleMixCost2_AL3", 0, path);

	this->m_BloodCastleMixCost3[0] = GetPrivateProfileInt(section, "BloodCastleMixCost3_AL0", 0, path);

	this->m_BloodCastleMixCost3[1] = GetPrivateProfileInt(section, "BloodCastleMixCost3_AL1", 0, path);

	this->m_BloodCastleMixCost3[2] = GetPrivateProfileInt(section, "BloodCastleMixCost3_AL2", 0, path);

	this->m_BloodCastleMixCost3[3] = GetPrivateProfileInt(section, "BloodCastleMixCost3_AL3", 0, path);

	this->m_BloodCastleMixCost4[0] = GetPrivateProfileInt(section, "BloodCastleMixCost4_AL0", 0, path);

	this->m_BloodCastleMixCost4[1] = GetPrivateProfileInt(section, "BloodCastleMixCost4_AL1", 0, path);

	this->m_BloodCastleMixCost4[2] = GetPrivateProfileInt(section, "BloodCastleMixCost4_AL2", 0, path);

	this->m_BloodCastleMixCost4[3] = GetPrivateProfileInt(section, "BloodCastleMixCost4_AL3", 0, path);

	this->m_BloodCastleMixCost5[0] = GetPrivateProfileInt(section, "BloodCastleMixCost5_AL0", 0, path);

	this->m_BloodCastleMixCost5[1] = GetPrivateProfileInt(section, "BloodCastleMixCost5_AL1", 0, path);

	this->m_BloodCastleMixCost5[2] = GetPrivateProfileInt(section, "BloodCastleMixCost5_AL2", 0, path);

	this->m_BloodCastleMixCost5[3] = GetPrivateProfileInt(section, "BloodCastleMixCost5_AL3", 0, path);

	this->m_BloodCastleMixCost6[0] = GetPrivateProfileInt(section, "BloodCastleMixCost6_AL0", 0, path);

	this->m_BloodCastleMixCost6[1] = GetPrivateProfileInt(section, "BloodCastleMixCost6_AL1", 0, path);

	this->m_BloodCastleMixCost6[2] = GetPrivateProfileInt(section, "BloodCastleMixCost6_AL2", 0, path);

	this->m_BloodCastleMixCost6[3] = GetPrivateProfileInt(section, "BloodCastleMixCost6_AL3", 0, path);

	this->m_Wing2MixRate[0] = GetPrivateProfileInt(section, "Wing2MixRate_AL0", 0, path);

	this->m_Wing2MixRate[1] = GetPrivateProfileInt(section, "Wing2MixRate_AL1", 0, path);

	this->m_Wing2MixRate[2] = GetPrivateProfileInt(section, "Wing2MixRate_AL2", 0, path);

	this->m_Wing2MixRate[3] = GetPrivateProfileInt(section, "Wing2MixRate_AL3", 0, path);

	this->m_Wing2MixCost[0] = GetPrivateProfileInt(section, "Wing2MixCost_AL0", 0, path);

	this->m_Wing2MixCost[1] = GetPrivateProfileInt(section, "Wing2MixCost_AL1", 0, path);

	this->m_Wing2MixCost[2] = GetPrivateProfileInt(section, "Wing2MixCost_AL2", 0, path);

	this->m_Wing2MixCost[3] = GetPrivateProfileInt(section, "Wing2MixCost_AL3", 0, path);

	this->m_Wing2MixAddItemRate[0] = GetPrivateProfileInt(section, "Wing2MixAddItemRate_AL0", 0, path);

	this->m_Wing2MixAddItemRate[1] = GetPrivateProfileInt(section, "Wing2MixAddItemRate_AL1", 0, path);

	this->m_Wing2MixAddItemRate[2] = GetPrivateProfileInt(section, "Wing2MixAddItemRate_AL2", 0, path);

	this->m_Wing2MixAddItemRate[3] = GetPrivateProfileInt(section, "Wing2MixAddItemRate_AL3", 0, path);

	this->m_Wing2CustomMixRate[0] = GetPrivateProfileInt(section, "Wing2CustomMixRate_AL0", 0, path);

	this->m_Wing2CustomMixRate[1] = GetPrivateProfileInt(section, "Wing2CustomMixRate_AL1", 0, path);

	this->m_Wing2CustomMixRate[2] = GetPrivateProfileInt(section, "Wing2CustomMixRate_AL2", 0, path);

	this->m_Wing2CustomMixRate[3] = GetPrivateProfileInt(section, "Wing2CustomMixRate_AL3", 0, path);
}

void CServerInfo::ReadCharacterInfo(char* section, char* path)
{
	this->m_GeneralDamageRatePvP = GetPrivateProfileInt(section, "GeneralDamageRatePvP", 0, path);

	this->m_GeneralDamageRatePvM = GetPrivateProfileInt(section, "GeneralDamageRatePvM", 0, path);

	this->m_ReflectDamageRatePvP = GetPrivateProfileInt(section, "ReflectDamageRatePvP", 0, path);

	this->m_ReflectDamageRatePvM = GetPrivateProfileInt(section, "ReflectDamageRatePvM", 0, path);

	this->m_DamageRatePvP[0] = GetPrivateProfileInt(section, "DWDamageRatePvP", 0, path);

	this->m_DamageRatePvP[1] = GetPrivateProfileInt(section, "DKDamageRatePvP", 0, path);

	this->m_DamageRatePvP[2] = GetPrivateProfileInt(section, "FEDamageRatePvP", 0, path);

	this->m_DamageRatePvP[3] = GetPrivateProfileInt(section, "MGDamageRatePvP", 0, path);

	this->m_DamageRatePvM[0] = GetPrivateProfileInt(section, "DWDamageRatePvM", 0, path);

	this->m_DamageRatePvM[1] = GetPrivateProfileInt(section, "DKDamageRatePvM", 0, path);

	this->m_DamageRatePvM[2] = GetPrivateProfileInt(section, "FEDamageRatePvM", 0, path);

	this->m_DamageRatePvM[3] = GetPrivateProfileInt(section, "MGDamageRatePvM", 0, path);

	this->m_DamageRateTo[0][0] = GetPrivateProfileInt(section, "DWDamageRateToDW", 0, path);

	this->m_DamageRateTo[0][1] = GetPrivateProfileInt(section, "DWDamageRateToDK", 0, path);

	this->m_DamageRateTo[0][2] = GetPrivateProfileInt(section, "DWDamageRateToFE", 0, path);

	this->m_DamageRateTo[0][3] = GetPrivateProfileInt(section, "DWDamageRateToMG", 0, path);

	this->m_DamageRateTo[1][0] = GetPrivateProfileInt(section, "DKDamageRateToDW", 0, path);

	this->m_DamageRateTo[1][1] = GetPrivateProfileInt(section, "DKDamageRateToDK", 0, path);

	this->m_DamageRateTo[1][2] = GetPrivateProfileInt(section, "DKDamageRateToFE", 0, path);

	this->m_DamageRateTo[1][3] = GetPrivateProfileInt(section, "DKDamageRateToMG", 0, path);

	this->m_DamageRateTo[2][0] = GetPrivateProfileInt(section, "FEDamageRateToDW", 0, path);

	this->m_DamageRateTo[2][1] = GetPrivateProfileInt(section, "FEDamageRateToDK", 0, path);

	this->m_DamageRateTo[2][2] = GetPrivateProfileInt(section, "FEDamageRateToFE", 0, path);

	this->m_DamageRateTo[2][3] = GetPrivateProfileInt(section, "FEDamageRateToMG", 0, path);

	this->m_DamageRateTo[3][0] = GetPrivateProfileInt(section, "MGDamageRateToDW", 0, path);

	this->m_DamageRateTo[3][1] = GetPrivateProfileInt(section, "MGDamageRateToDK", 0, path);

	this->m_DamageRateTo[3][2] = GetPrivateProfileInt(section, "MGDamageRateToFE", 0, path);

	this->m_DamageRateTo[3][3] = GetPrivateProfileInt(section, "MGDamageRateToMG", 0, path);

	this->m_PetUniriaEnableStuck = GetPrivateProfileInt(section, "PetUniriaEnableStuck", 0, path);

	this->m_PetDinorantEnableStuck = GetPrivateProfileInt(section, "PetDinorantEnableStuck", 0, path);

	this->m_DamageStuckRate[0] = GetPrivateProfileInt(section, "DWDamageStuckRate", 0, path);

	this->m_DamageStuckRate[1] = GetPrivateProfileInt(section, "DKDamageStuckRate", 0, path);

	this->m_DamageStuckRate[2] = GetPrivateProfileInt(section, "FEDamageStuckRate", 0, path);

	this->m_DamageStuckRate[3] = GetPrivateProfileInt(section, "MGDamageStuckRate", 0, path);

	this->m_DamageMultiplierConstA[0] = GetPrivateProfileInt(section, "DWDamageMultiplierConstA", 0, path);

	this->m_DamageMultiplierConstA[1] = GetPrivateProfileInt(section, "DKDamageMultiplierConstA", 0, path);

	this->m_DamageMultiplierConstA[2] = GetPrivateProfileInt(section, "FEDamageMultiplierConstA", 0, path);

	this->m_DamageMultiplierConstA[3] = GetPrivateProfileInt(section, "MGDamageMultiplierConstA", 0, path);

	this->m_DamageMultiplierMaxRate[0] = GetPrivateProfileInt(section, "DWDamageMultiplierMaxRate", 0, path);

	this->m_DamageMultiplierMaxRate[1] = GetPrivateProfileInt(section, "DKDamageMultiplierMaxRate", 0, path);

	this->m_DamageMultiplierMaxRate[2] = GetPrivateProfileInt(section, "FEDamageMultiplierMaxRate", 0, path);

	this->m_DamageMultiplierMaxRate[3] = GetPrivateProfileInt(section, "MGDamageMultiplierMaxRate", 0, path);

	this->m_SatanIncDamageConstA = GetPrivateProfileInt(section, "SatanIncDamageConstA", 0, path);

	this->m_DinorantIncDamageConstA = GetPrivateProfileInt(section, "DinorantIncDamageConstA", 0, path);

	this->m_AngelDecDamageConstA = GetPrivateProfileInt(section, "AngelDecDamageConstA", 0, path);

	this->m_DinorantDecDamageConstA = GetPrivateProfileInt(section, "DinorantDecDamageConstA", 0, path);

	this->m_DinorantDecDamageConstB = GetPrivateProfileInt(section, "DinorantDecDamageConstB", 0, path);

	this->m_HPRecoveryRate[0] = GetPrivateProfileInt(section, "DWHPRecoveryRate", 0, path);

	this->m_HPRecoveryRate[1] = GetPrivateProfileInt(section, "DKHPRecoveryRate", 0, path);

	this->m_HPRecoveryRate[2] = GetPrivateProfileInt(section, "FEHPRecoveryRate", 0, path);

	this->m_HPRecoveryRate[3] = GetPrivateProfileInt(section, "MGHPRecoveryRate", 0, path);

	this->m_MPRecoveryRate[0] = GetPrivateProfileInt(section, "DWMPRecoveryRate", 0, path);

	this->m_MPRecoveryRate[1] = GetPrivateProfileInt(section, "DKMPRecoveryRate", 0, path);

	this->m_MPRecoveryRate[2] = GetPrivateProfileInt(section, "FEMPRecoveryRate", 0, path);

	this->m_MPRecoveryRate[3] = GetPrivateProfileInt(section, "MGMPRecoveryRate", 0, path);

	this->m_BPRecoveryRate[0] = GetPrivateProfileInt(section, "DWBPRecoveryRate", 0, path);

	this->m_BPRecoveryRate[1] = GetPrivateProfileInt(section, "DKBPRecoveryRate", 0, path);

	this->m_BPRecoveryRate[2] = GetPrivateProfileInt(section, "FEBPRecoveryRate", 0, path);

	this->m_BPRecoveryRate[3] = GetPrivateProfileInt(section, "MGBPRecoveryRate", 0, path);

	this->m_DWPhysiDamageMinConstA = GetPrivateProfileInt(section, "DWPhysiDamageMinConstA", 0, path);

	this->m_DWPhysiDamageMaxConstA = GetPrivateProfileInt(section, "DWPhysiDamageMaxConstA", 0, path);

	this->m_DWMagicDamageMinConstA = GetPrivateProfileInt(section, "DWMagicDamageMinConstA", 0, path);

	this->m_DWMagicDamageMaxConstA = GetPrivateProfileInt(section, "DWMagicDamageMaxConstA", 0, path);

	this->m_DKPhysiDamageMinConstA = GetPrivateProfileInt(section, "DKPhysiDamageMinConstA", 0, path);

	this->m_DKPhysiDamageMaxConstA = GetPrivateProfileInt(section, "DKPhysiDamageMaxConstA", 0, path);

	this->m_DKMagicDamageMinConstA = GetPrivateProfileInt(section, "DKMagicDamageMinConstA", 0, path);

	this->m_DKMagicDamageMaxConstA = GetPrivateProfileInt(section, "DKMagicDamageMaxConstA", 0, path);

	this->m_FEPhysiDamageMinConstA = GetPrivateProfileInt(section, "FEPhysiDamageMinConstA", 0, path);

	this->m_FEPhysiDamageMaxConstA = GetPrivateProfileInt(section, "FEPhysiDamageMaxConstA", 0, path);

	this->m_FEPhysiDamageMinBowConstA = GetPrivateProfileInt(section, "FEPhysiDamageMinBowConstA", 0, path);

	this->m_FEPhysiDamageMinBowConstB = GetPrivateProfileInt(section, "FEPhysiDamageMinBowConstB", 0, path);

	this->m_FEPhysiDamageMaxBowConstA = GetPrivateProfileInt(section, "FEPhysiDamageMaxBowConstA", 0, path);

	this->m_FEPhysiDamageMaxBowConstB = GetPrivateProfileInt(section, "FEPhysiDamageMaxBowConstB", 0, path);

	this->m_FEMagicDamageMinConstA = GetPrivateProfileInt(section, "FEMagicDamageMinConstA", 0, path);

	this->m_FEMagicDamageMaxConstA = GetPrivateProfileInt(section, "FEMagicDamageMaxConstA", 0, path);

	this->m_MGPhysiDamageMinConstA = GetPrivateProfileInt(section, "MGPhysiDamageMinConstA", 0, path);

	this->m_MGPhysiDamageMinConstB = GetPrivateProfileInt(section, "MGPhysiDamageMinConstB", 0, path);

	this->m_MGPhysiDamageMaxConstA = GetPrivateProfileInt(section, "MGPhysiDamageMaxConstA", 0, path);

	this->m_MGPhysiDamageMaxConstB = GetPrivateProfileInt(section, "MGPhysiDamageMaxConstB", 0, path);

	this->m_MGMagicDamageMinConstA = GetPrivateProfileInt(section, "MGMagicDamageMinConstA", 0, path);

	this->m_MGMagicDamageMaxConstA = GetPrivateProfileInt(section, "MGMagicDamageMaxConstA", 0, path);

	this->m_DWAttackSuccessRateConstA = GetPrivateProfileInt(section, "DWAttackSuccessRateConstA", 0, path);

	this->m_DWAttackSuccessRateConstB = GetPrivateProfileInt(section, "DWAttackSuccessRateConstB", 0, path);

	this->m_DWAttackSuccessRateConstC = GetPrivateProfileInt(section, "DWAttackSuccessRateConstC", 0, path);

	this->m_DWAttackSuccessRateConstD = GetPrivateProfileInt(section, "DWAttackSuccessRateConstD", 0, path);

	this->m_DKAttackSuccessRateConstA = GetPrivateProfileInt(section, "DKAttackSuccessRateConstA", 0, path);

	this->m_DKAttackSuccessRateConstB = GetPrivateProfileInt(section, "DKAttackSuccessRateConstB", 0, path);

	this->m_DKAttackSuccessRateConstC = GetPrivateProfileInt(section, "DKAttackSuccessRateConstC", 0, path);

	this->m_DKAttackSuccessRateConstD = GetPrivateProfileInt(section, "DKAttackSuccessRateConstD", 0, path);

	this->m_FEAttackSuccessRateConstA = GetPrivateProfileInt(section, "FEAttackSuccessRateConstA", 0, path);

	this->m_FEAttackSuccessRateConstB = GetPrivateProfileInt(section, "FEAttackSuccessRateConstB", 0, path);

	this->m_FEAttackSuccessRateConstC = GetPrivateProfileInt(section, "FEAttackSuccessRateConstC", 0, path);

	this->m_FEAttackSuccessRateConstD = GetPrivateProfileInt(section, "FEAttackSuccessRateConstD", 0, path);

	this->m_MGAttackSuccessRateConstA = GetPrivateProfileInt(section, "MGAttackSuccessRateConstA", 0, path);

	this->m_MGAttackSuccessRateConstB = GetPrivateProfileInt(section, "MGAttackSuccessRateConstB", 0, path);

	this->m_MGAttackSuccessRateConstC = GetPrivateProfileInt(section, "MGAttackSuccessRateConstC", 0, path);

	this->m_MGAttackSuccessRateConstD = GetPrivateProfileInt(section, "MGAttackSuccessRateConstD", 0, path);

	this->m_DWPhysiSpeedConstA = GetPrivateProfileInt(section, "DWPhysiSpeedConstA", 0, path);

	this->m_DWMagicSpeedConstA = GetPrivateProfileInt(section, "DWMagicSpeedConstA", 0, path);

	this->m_DKPhysiSpeedConstA = GetPrivateProfileInt(section, "DKPhysiSpeedConstA", 0, path);

	this->m_DKMagicSpeedConstA = GetPrivateProfileInt(section, "DKMagicSpeedConstA", 0, path);

	this->m_FEPhysiSpeedConstA = GetPrivateProfileInt(section, "FEPhysiSpeedConstA", 0, path);

	this->m_FEMagicSpeedConstA = GetPrivateProfileInt(section, "FEMagicSpeedConstA", 0, path);

	this->m_MGPhysiSpeedConstA = GetPrivateProfileInt(section, "MGPhysiSpeedConstA", 0, path);

	this->m_MGMagicSpeedConstA = GetPrivateProfileInt(section, "MGMagicSpeedConstA", 0, path);

	this->m_DWDefenseSuccessRateConstA = GetPrivateProfileInt(section, "DWDefenseSuccessRateConstA", 0, path);

	this->m_DKDefenseSuccessRateConstA = GetPrivateProfileInt(section, "DKDefenseSuccessRateConstA", 0, path);

	this->m_FEDefenseSuccessRateConstA = GetPrivateProfileInt(section, "FEDefenseSuccessRateConstA", 0, path);

	this->m_MGDefenseSuccessRateConstA = GetPrivateProfileInt(section, "MGDefenseSuccessRateConstA", 0, path);

	this->m_DWDefenseConstA = GetPrivateProfileInt(section, "DWDefenseConstA", 0, path);

	this->m_DKDefenseConstA = GetPrivateProfileInt(section, "DKDefenseConstA", 0, path);

	this->m_FEDefenseConstA = GetPrivateProfileInt(section, "FEDefenseConstA", 0, path);

	this->m_MGDefenseConstA = GetPrivateProfileInt(section, "MGDefenseConstA", 0, path);
}

void CServerInfo::ReadCommandInfo(char* section, char* path)
{
	this->m_CommandPostType = GetPrivateProfileInt(section, "CommandPostType", 0, path);

	this->m_CommandAddPointAutoEnable[0] = GetPrivateProfileInt(section, "CommandAddPointAutoEnable_AL0", 0, path);

	this->m_CommandAddPointAutoEnable[1] = GetPrivateProfileInt(section, "CommandAddPointAutoEnable_AL1", 0, path);

	this->m_CommandAddPointAutoEnable[2] = GetPrivateProfileInt(section, "CommandAddPointAutoEnable_AL2", 0, path);

	this->m_CommandAddPointAutoEnable[3] = GetPrivateProfileInt(section, "CommandAddPointAutoEnable_AL3", 0, path);

	this->m_CommandResetAutoEnable[0] = GetPrivateProfileInt(section, "CommandResetAutoEnable_AL0", 0, path);

	this->m_CommandResetAutoEnable[1] = GetPrivateProfileInt(section, "CommandResetAutoEnable_AL1", 0, path);

	this->m_CommandResetAutoEnable[2] = GetPrivateProfileInt(section, "CommandResetAutoEnable_AL2", 0, path);

	this->m_CommandResetAutoEnable[3] = GetPrivateProfileInt(section, "CommandResetAutoEnable_AL3", 0, path);

	this->m_CommandResetLevel[0] = GetPrivateProfileInt(section, "CommandResetLevel_AL0", 0, path);

	this->m_CommandResetLevel[1] = GetPrivateProfileInt(section, "CommandResetLevel_AL1", 0, path);

	this->m_CommandResetLevel[2] = GetPrivateProfileInt(section, "CommandResetLevel_AL2", 0, path);

	this->m_CommandResetLevel[3] = GetPrivateProfileInt(section, "CommandResetLevel_AL3", 0, path);

	this->m_CommandResetStartLevel[0] = GetPrivateProfileInt(section, "CommandResetStartLevel_AL0", 0, path);

	this->m_CommandResetStartLevel[1] = GetPrivateProfileInt(section, "CommandResetStartLevel_AL1", 0, path);

	this->m_CommandResetStartLevel[2] = GetPrivateProfileInt(section, "CommandResetStartLevel_AL2", 0, path);

	this->m_CommandResetStartLevel[3] = GetPrivateProfileInt(section, "CommandResetStartLevel_AL3", 0, path);

	this->m_CommandResetMoney[0] = GetPrivateProfileInt(section, "CommandResetMoney_AL0", 0, path);

	this->m_CommandResetMoney[1] = GetPrivateProfileInt(section, "CommandResetMoney_AL1", 0, path);

	this->m_CommandResetMoney[2] = GetPrivateProfileInt(section, "CommandResetMoney_AL2", 0, path);

	this->m_CommandResetMoney[3] = GetPrivateProfileInt(section, "CommandResetMoney_AL3", 0, path);

	this->m_CommandResetPoint[0] = GetPrivateProfileInt(section, "CommandResetPoint_AL0", 0, path);

	this->m_CommandResetPoint[1] = GetPrivateProfileInt(section, "CommandResetPoint_AL1", 0, path);

	this->m_CommandResetPoint[2] = GetPrivateProfileInt(section, "CommandResetPoint_AL2", 0, path);

	this->m_CommandResetPoint[3] = GetPrivateProfileInt(section, "CommandResetPoint_AL3", 0, path);

	this->m_CommandResetPointRate[0] = GetPrivateProfileInt(section, "CommandResetPointRateDW", 0, path);

	this->m_CommandResetPointRate[1] = GetPrivateProfileInt(section, "CommandResetPointRateDK", 0, path);

	this->m_CommandResetPointRate[2] = GetPrivateProfileInt(section, "CommandResetPointRateFE", 0, path);

	this->m_CommandResetPointRate[3] = GetPrivateProfileInt(section, "CommandResetPointRateMG", 0, path);

	this->m_CommandResetCount[0] = GetPrivateProfileInt(section, "CommandResetCount_AL0", 0, path);

	this->m_CommandResetCount[1] = GetPrivateProfileInt(section, "CommandResetCount_AL1", 0, path);

	this->m_CommandResetCount[2] = GetPrivateProfileInt(section, "CommandResetCount_AL2", 0, path);

	this->m_CommandResetCount[3] = GetPrivateProfileInt(section, "CommandResetCount_AL3", 0, path);

	this->m_CommandResetCheckItem[0] = GetPrivateProfileInt(section, "CommandResetCheckItem_AL0", 0, path);

	this->m_CommandResetCheckItem[1] = GetPrivateProfileInt(section, "CommandResetCheckItem_AL1", 0, path);

	this->m_CommandResetCheckItem[2] = GetPrivateProfileInt(section, "CommandResetCheckItem_AL2", 0, path);

	this->m_CommandResetCheckItem[3] = GetPrivateProfileInt(section, "CommandResetCheckItem_AL3", 0, path);

	this->m_CommandResetStats[0] = GetPrivateProfileInt(section, "CommandResetStat_AL0", 0, path);

	this->m_CommandResetStats[1] = GetPrivateProfileInt(section, "CommandResetStat_AL1", 0, path);

	this->m_CommandResetStats[2] = GetPrivateProfileInt(section, "CommandResetStat_AL2", 0, path);

	this->m_CommandResetStats[3] = GetPrivateProfileInt(section, "CommandResetStat_AL3", 0, path);

	this->m_CommandResetQuest[0] = GetPrivateProfileInt(section, "CommandResetQuest_AL0", 0, path);

	this->m_CommandResetQuest[1] = GetPrivateProfileInt(section, "CommandResetQuest_AL1", 0, path);

	this->m_CommandResetQuest[2] = GetPrivateProfileInt(section, "CommandResetQuest_AL2", 0, path);

	this->m_CommandResetQuest[3] = GetPrivateProfileInt(section, "CommandResetQuest_AL3", 0, path);

	this->m_CommandResetSkill[0] = GetPrivateProfileInt(section, "CommandResetSkill_AL0", 0, path);

	this->m_CommandResetSkill[1] = GetPrivateProfileInt(section, "CommandResetSkill_AL1", 0, path);

	this->m_CommandResetSkill[2] = GetPrivateProfileInt(section, "CommandResetSkill_AL2", 0, path);

	this->m_CommandResetSkill[3] = GetPrivateProfileInt(section, "CommandResetSkill_AL3", 0, path);

	this->m_CommandResetMove[0] = GetPrivateProfileInt(section, "CommandResetMove_AL0", 0, path);

	this->m_CommandResetMove[1] = GetPrivateProfileInt(section, "CommandResetMove_AL1", 0, path);

	this->m_CommandResetMove[2] = GetPrivateProfileInt(section, "CommandResetMove_AL2", 0, path);

	this->m_CommandResetMove[3] = GetPrivateProfileInt(section, "CommandResetMove_AL3", 0, path);

	this->m_CommandResetLimit[0] = GetPrivateProfileInt(section, "CommandResetLimit_AL0", 0, path);

	this->m_CommandResetLimit[1] = GetPrivateProfileInt(section, "CommandResetLimit_AL1", 0, path);

	this->m_CommandResetLimit[2] = GetPrivateProfileInt(section, "CommandResetLimit_AL2", 0, path);

	this->m_CommandResetLimit[3] = GetPrivateProfileInt(section, "CommandResetLimit_AL3", 0, path);

	this->m_CommandResetLimitDay[0] = GetPrivateProfileInt(section, "CommandResetLimitDay_AL0", 0, path);

	this->m_CommandResetLimitDay[1] = GetPrivateProfileInt(section, "CommandResetLimitDay_AL1", 0, path);

	this->m_CommandResetLimitDay[2] = GetPrivateProfileInt(section, "CommandResetLimitDay_AL2", 0, path);

	this->m_CommandResetLimitDay[3] = GetPrivateProfileInt(section, "CommandResetLimitDay_AL3", 0, path);

	this->m_CommandResetLimitWek[0] = GetPrivateProfileInt(section, "CommandResetLimitWek_AL0", 0, path);

	this->m_CommandResetLimitWek[1] = GetPrivateProfileInt(section, "CommandResetLimitWek_AL1", 0, path);

	this->m_CommandResetLimitWek[2] = GetPrivateProfileInt(section, "CommandResetLimitWek_AL2", 0, path);

	this->m_CommandResetLimitWek[3] = GetPrivateProfileInt(section, "CommandResetLimitWek_AL3", 0, path);

	this->m_CommandResetLimitMon[0] = GetPrivateProfileInt(section, "CommandResetLimitMon_AL0", 0, path);

	this->m_CommandResetLimitMon[1] = GetPrivateProfileInt(section, "CommandResetLimitMon_AL1", 0, path);

	this->m_CommandResetLimitMon[2] = GetPrivateProfileInt(section, "CommandResetLimitMon_AL2", 0, path);

	this->m_CommandResetLimitMon[3] = GetPrivateProfileInt(section, "CommandResetLimitMon_AL3", 0, path);

	this->m_CommandGrandResetLevel[0] = GetPrivateProfileInt(section, "CommandGrandResetLevel_AL0", 0, path);

	this->m_CommandGrandResetLevel[1] = GetPrivateProfileInt(section, "CommandGrandResetLevel_AL1", 0, path);

	this->m_CommandGrandResetLevel[2] = GetPrivateProfileInt(section, "CommandGrandResetLevel_AL2", 0, path);

	this->m_CommandGrandResetLevel[3] = GetPrivateProfileInt(section, "CommandGrandResetLevel_AL3", 0, path);

	this->m_CommandGrandResetStartLevel[0] = GetPrivateProfileInt(section, "CommandGrandResetStartLevel_AL0", 0, path);

	this->m_CommandGrandResetStartLevel[1] = GetPrivateProfileInt(section, "CommandGrandResetStartLevel_AL1", 0, path);

	this->m_CommandGrandResetStartLevel[2] = GetPrivateProfileInt(section, "CommandGrandResetStartLevel_AL2", 0, path);

	this->m_CommandGrandResetStartLevel[3] = GetPrivateProfileInt(section, "CommandGrandResetStartLevel_AL3", 0, path);

	this->m_CommandGrandResetReset[0] = GetPrivateProfileInt(section, "CommandGrandResetReset_AL0", 0, path);

	this->m_CommandGrandResetReset[1] = GetPrivateProfileInt(section, "CommandGrandResetReset_AL1", 0, path);

	this->m_CommandGrandResetReset[2] = GetPrivateProfileInt(section, "CommandGrandResetReset_AL2", 0, path);

	this->m_CommandGrandResetReset[3] = GetPrivateProfileInt(section, "CommandGrandResetReset_AL3", 0, path);

	this->m_CommandGrandResetStartReset[0] = GetPrivateProfileInt(section, "CommandGrandResetStartReset_AL0", 0, path);

	this->m_CommandGrandResetStartReset[1] = GetPrivateProfileInt(section, "CommandGrandResetStartReset_AL1", 0, path);

	this->m_CommandGrandResetStartReset[2] = GetPrivateProfileInt(section, "CommandGrandResetStartReset_AL2", 0, path);

	this->m_CommandGrandResetStartReset[3] = GetPrivateProfileInt(section, "CommandGrandResetStartReset_AL3", 0, path);

	this->m_CommandGrandResetMoney[0] = GetPrivateProfileInt(section, "CommandGrandResetMoney_AL0", 0, path);

	this->m_CommandGrandResetMoney[1] = GetPrivateProfileInt(section, "CommandGrandResetMoney_AL1", 0, path);

	this->m_CommandGrandResetMoney[2] = GetPrivateProfileInt(section, "CommandGrandResetMoney_AL2", 0, path);

	this->m_CommandGrandResetMoney[3] = GetPrivateProfileInt(section, "CommandGrandResetMoney_AL3", 0, path);

	this->m_CommandGrandResetPoint[0] = GetPrivateProfileInt(section, "CommandGrandResetPoint_AL0", 0, path);

	this->m_CommandGrandResetPoint[1] = GetPrivateProfileInt(section, "CommandGrandResetPoint_AL1", 0, path);

	this->m_CommandGrandResetPoint[2] = GetPrivateProfileInt(section, "CommandGrandResetPoint_AL2", 0, path);

	this->m_CommandGrandResetPoint[3] = GetPrivateProfileInt(section, "CommandGrandResetPoint_AL3", 0, path);

	this->m_CommandGrandResetPointRate[0] = GetPrivateProfileInt(section, "CommandGrandResetPointRateDW", 0, path);

	this->m_CommandGrandResetPointRate[1] = GetPrivateProfileInt(section, "CommandGrandResetPointRateDK", 0, path);

	this->m_CommandGrandResetPointRate[2] = GetPrivateProfileInt(section, "CommandGrandResetPointRateFE", 0, path);

	this->m_CommandGrandResetPointRate[3] = GetPrivateProfileInt(section, "CommandGrandResetPointRateMG", 0, path);

	this->m_CommandGrandResetCount[0] = GetPrivateProfileInt(section, "CommandGrandResetCount_AL0", 0, path);

	this->m_CommandGrandResetCount[1] = GetPrivateProfileInt(section, "CommandGrandResetCount_AL1", 0, path);

	this->m_CommandGrandResetCount[2] = GetPrivateProfileInt(section, "CommandGrandResetCount_AL2", 0, path);

	this->m_CommandGrandResetCount[3] = GetPrivateProfileInt(section, "CommandGrandResetCount_AL3", 0, path);

	this->m_CommandGrandResetCheckItem[0] = GetPrivateProfileInt(section, "CommandGrandResetCheckItem_AL0", 0, path);

	this->m_CommandGrandResetCheckItem[1] = GetPrivateProfileInt(section, "CommandGrandResetCheckItem_AL1", 0, path);

	this->m_CommandGrandResetCheckItem[2] = GetPrivateProfileInt(section, "CommandGrandResetCheckItem_AL2", 0, path);

	this->m_CommandGrandResetCheckItem[3] = GetPrivateProfileInt(section, "CommandGrandResetCheckItem_AL3", 0, path);

	this->m_CommandGrandResetStats[0] = GetPrivateProfileInt(section, "CommandGrandResetStat_AL0", 0, path);

	this->m_CommandGrandResetStats[1] = GetPrivateProfileInt(section, "CommandGrandResetStat_AL1", 0, path);

	this->m_CommandGrandResetStats[2] = GetPrivateProfileInt(section, "CommandGrandResetStat_AL2", 0, path);

	this->m_CommandGrandResetStats[3] = GetPrivateProfileInt(section, "CommandGrandResetStat_AL3", 0, path);

	this->m_CommandGrandResetQuest[0] = GetPrivateProfileInt(section, "CommandGrandResetQuest_AL0", 0, path);

	this->m_CommandGrandResetQuest[1] = GetPrivateProfileInt(section, "CommandGrandResetQuest_AL1", 0, path);

	this->m_CommandGrandResetQuest[2] = GetPrivateProfileInt(section, "CommandGrandResetQuest_AL2", 0, path);

	this->m_CommandGrandResetQuest[3] = GetPrivateProfileInt(section, "CommandGrandResetQuest_AL3", 0, path);

	this->m_CommandGrandResetSkill[0] = GetPrivateProfileInt(section, "CommandGrandResetSkill_AL0", 0, path);

	this->m_CommandGrandResetSkill[1] = GetPrivateProfileInt(section, "CommandGrandResetSkill_AL1", 0, path);

	this->m_CommandGrandResetSkill[2] = GetPrivateProfileInt(section, "CommandGrandResetSkill_AL2", 0, path);

	this->m_CommandGrandResetSkill[3] = GetPrivateProfileInt(section, "CommandGrandResetSkill_AL3", 0, path);

	this->m_CommandGrandResetMove[0] = GetPrivateProfileInt(section, "CommandGrandResetMove_AL0", 0, path);

	this->m_CommandGrandResetMove[1] = GetPrivateProfileInt(section, "CommandGrandResetMove_AL1", 0, path);

	this->m_CommandGrandResetMove[2] = GetPrivateProfileInt(section, "CommandGrandResetMove_AL2", 0, path);

	this->m_CommandGrandResetMove[3] = GetPrivateProfileInt(section, "CommandGrandResetMove_AL3", 0, path);

	this->m_CommandGrandResetLimit[0] = GetPrivateProfileInt(section, "CommandGrandResetLimit_AL0", 0, path);

	this->m_CommandGrandResetLimit[1] = GetPrivateProfileInt(section, "CommandGrandResetLimit_AL1", 0, path);

	this->m_CommandGrandResetLimit[2] = GetPrivateProfileInt(section, "CommandGrandResetLimit_AL2", 0, path);

	this->m_CommandGrandResetLimit[3] = GetPrivateProfileInt(section, "CommandGrandResetLimit_AL3", 0, path);

	this->m_CommandGrandResetLimitDay[0] = GetPrivateProfileInt(section, "CommandGrandResetLimitDay_AL0", 0, path);

	this->m_CommandGrandResetLimitDay[1] = GetPrivateProfileInt(section, "CommandGrandResetLimitDay_AL1", 0, path);

	this->m_CommandGrandResetLimitDay[2] = GetPrivateProfileInt(section, "CommandGrandResetLimitDay_AL2", 0, path);

	this->m_CommandGrandResetLimitDay[3] = GetPrivateProfileInt(section, "CommandGrandResetLimitDay_AL3", 0, path);

	this->m_CommandGrandResetLimitWek[0] = GetPrivateProfileInt(section, "CommandGrandResetLimitWek_AL0", 0, path);

	this->m_CommandGrandResetLimitWek[1] = GetPrivateProfileInt(section, "CommandGrandResetLimitWek_AL1", 0, path);

	this->m_CommandGrandResetLimitWek[2] = GetPrivateProfileInt(section, "CommandGrandResetLimitWek_AL2", 0, path);

	this->m_CommandGrandResetLimitWek[3] = GetPrivateProfileInt(section, "CommandGrandResetLimitWek_AL3", 0, path);

	this->m_CommandGrandResetLimitMon[0] = GetPrivateProfileInt(section, "CommandGrandResetLimitMon_AL0", 0, path);

	this->m_CommandGrandResetLimitMon[1] = GetPrivateProfileInt(section, "CommandGrandResetLimitMon_AL1", 0, path);

	this->m_CommandGrandResetLimitMon[2] = GetPrivateProfileInt(section, "CommandGrandResetLimitMon_AL2", 0, path);

	this->m_CommandGrandResetLimitMon[3] = GetPrivateProfileInt(section, "CommandGrandResetLimitMon_AL3", 0, path);

	this->m_CommandWareNumber[0] = GetPrivateProfileInt(section, "CommandWareNumber_AL0", 0, path);

	this->m_CommandWareNumber[1] = GetPrivateProfileInt(section, "CommandWareNumber_AL1", 0, path);

	this->m_CommandWareNumber[2] = GetPrivateProfileInt(section, "CommandWareNumber_AL2", 0, path);

	this->m_CommandWareNumber[3] = GetPrivateProfileInt(section, "CommandWareNumber_AL3", 0, path);
}

void CServerInfo::ReadCommonInfo(char* section, char* path)
{
	this->m_MainChecksum = GetPrivateProfileInt(section, "MainChecksum", 0, path);

	this->m_MaxItemOption = GetPrivateProfileInt(section, "MaxItemOption", 0, path);

	this->m_MaxIpConnection = GetPrivateProfileInt(section, "MaxIpConnection", 0, path);

	this->m_DisconnectOnlineAccount = GetPrivateProfileInt(section, "DisconnectOnlineAccount", 0, path);

	this->m_PersonalCodeCheck = GetPrivateProfileInt(section, "PersonalCodeCheck", 0, path);

	this->m_MonsterLifeRate = GetPrivateProfileInt(section, "MonsterLifeRate", 0, path);

	this->m_MonsterHealthBarSwitch = GetPrivateProfileInt(section, "MonsterHealthBarSwitch", 0, path);

	this->m_NonPK = GetPrivateProfileInt(section, "NonPK", 0, path);

	this->m_PKLimitFree = GetPrivateProfileInt(section, "PKLimitFree", 0, path);

	this->m_PKDownTime1 = GetPrivateProfileInt(section, "PKDownTime1", 0, path);

	this->m_PKDownTime2 = GetPrivateProfileInt(section, "PKDownTime2", 0, path);

	this->m_TradeSwitch = GetPrivateProfileInt(section, "TradeSwitch", 0, path);

	this->m_GuildCreateSwitch = GetPrivateProfileInt(section, "GuildCreateSwitch", 0, path);

	this->m_GuildDeleteSwitch = GetPrivateProfileInt(section, "GuildDeleteSwitch", 0, path);

	this->m_GuildCreateMinLevel = GetPrivateProfileInt(section, "GuildCreateMinLevel", 0, path);

	this->m_GuildCreateMinReset = GetPrivateProfileInt(section, "GuildCreateMinReset", 0, path);

	this->m_GuildInsertMaxUser[0] = GetPrivateProfileInt(section, "GuildInsertMaxUser_AL0", 0, path);

	this->m_GuildInsertMaxUser[1] = GetPrivateProfileInt(section, "GuildInsertMaxUser_AL1", 0, path);

	this->m_GuildInsertMaxUser[2] = GetPrivateProfileInt(section, "GuildInsertMaxUser_AL2", 0, path);

	this->m_GuildInsertMaxUser[3] = GetPrivateProfileInt(section, "GuildInsertMaxUser_AL3", 0, path);

	this->m_GuildWarWinScore = GetPrivateProfileInt(section, "GuildWarWinScore", 0, path);

	this->m_GuildWarSoccerWinScore = GetPrivateProfileInt(section, "GuildWarSoccerWinScore", 0, path);

	this->m_GuildWarWinPointsGain_Max = GetPrivateProfileInt(section, "GuildWarWinPointsGain_Max", 0, path);

	this->m_GuildWarWinPointsGain_Half = GetPrivateProfileInt(section, "GuildWarWinPointsGain_Half", 0, path);

	this->m_GuildWarWinPointsGain_Normal = GetPrivateProfileInt(section, "GuildWarWinPointsGain_Normal", 0, path);

	this->m_AddExperienceRate[0] = GetPrivateProfileInt(section, "AddExperienceRate_AL0", 0, path);

	this->m_AddExperienceRate[1] = GetPrivateProfileInt(section, "AddExperienceRate_AL1", 0, path);

	this->m_AddExperienceRate[2] = GetPrivateProfileInt(section, "AddExperienceRate_AL2", 0, path);

	this->m_AddExperienceRate[3] = GetPrivateProfileInt(section, "AddExperienceRate_AL3", 0, path);

	this->m_AddEventExperienceRate[0] = GetPrivateProfileInt(section, "AddEventExperienceRate_AL0", 0, path);

	this->m_AddEventExperienceRate[1] = GetPrivateProfileInt(section, "AddEventExperienceRate_AL1", 0, path);

	this->m_AddEventExperienceRate[2] = GetPrivateProfileInt(section, "AddEventExperienceRate_AL2", 0, path);

	this->m_AddEventExperienceRate[3] = GetPrivateProfileInt(section, "AddEventExperienceRate_AL3", 0, path);

	this->m_ItemDropTime = GetPrivateProfileInt(section, "ItemDropTime", 0, path);

	this->m_ItemDropRate[0] = GetPrivateProfileInt(section, "ItemDropRate_AL0", 0, path);

	this->m_ItemDropRate[1] = GetPrivateProfileInt(section, "ItemDropRate_AL1", 0, path);

	this->m_ItemDropRate[2] = GetPrivateProfileInt(section, "ItemDropRate_AL2", 0, path);

	this->m_ItemDropRate[3] = GetPrivateProfileInt(section, "ItemDropRate_AL3", 0, path);

	this->m_MoneyDropTime = GetPrivateProfileInt(section, "MoneyDropTime", 0, path);

	this->m_MoneyAmountDropRate[0] = GetPrivateProfileInt(section, "MoneyAmountDropRate_AL0", 0, path);

	this->m_MoneyAmountDropRate[1] = GetPrivateProfileInt(section, "MoneyAmountDropRate_AL1", 0, path);

	this->m_MoneyAmountDropRate[2] = GetPrivateProfileInt(section, "MoneyAmountDropRate_AL2", 0, path);

	this->m_MoneyAmountDropRate[3] = GetPrivateProfileInt(section, "MoneyAmountDropRate_AL3", 0, path);

	this->m_ArrowsInfiniteDurability = (GetPrivateProfileInt(section, "ArrowsInfiniteDurability", 0, path) != 0);

	this->m_WeaponDurabilityRate = GetPrivateProfileInt(section, "WeaponDurabilityRate", 0, path);

	this->m_ArmorDurabilityRate = GetPrivateProfileInt(section, "ArmorDurabilityRate", 0, path);

	this->m_WingDurabilityRate = GetPrivateProfileInt(section, "WingDurabilityRate", 0, path);

	this->m_GuardianDurabilityRate = GetPrivateProfileInt(section, "GuardianDurabilityRate", 0, path);

	this->m_PendantDurabilityRate = GetPrivateProfileInt(section, "PendantDurabilityRate", 0, path);

	this->m_RingDurabilityRate = GetPrivateProfileInt(section, "RingDurabilityRate", 0, path);

	this->m_TradeItemBlockExc = GetPrivateProfileInt(section, "TradeItemBlockExc", 0, path);

	this->m_ShopItemBlockSellOnMaxMoney = GetPrivateProfileInt(section, "ShopItemBlockSellOnMaxMoney", 0, path);

	this->m_ShopItemBlockExc = GetPrivateProfileInt(section, "ShopItemBlockExc", 0, path);

	this->m_MaxCharacterLevel = GetPrivateProfileInt(section, "MaxCharacterLevel", 400, path);

	this->m_MaxCharacterLevel = ((this->m_MaxCharacterLevel < 1) ? 1 : ((this->m_MaxCharacterLevel > MAX_CHARACTER_LEVEL) ? MAX_CHARACTER_LEVEL : this->m_MaxCharacterLevel));

	this->m_MaxLevelUp = GetPrivateProfileInt(section, "MaxLevelUp", 0, path);

	this->m_MaxLevelUpEvent = GetPrivateProfileInt(section, "MaxLevelUpEvent", 0, path);

	this->m_MaxStatPoint[0] = GetPrivateProfileInt(section, "MaxStatPoint_AL0", 0, path);

	this->m_MaxStatPoint[1] = GetPrivateProfileInt(section, "MaxStatPoint_AL1", 0, path);

	this->m_MaxStatPoint[2] = GetPrivateProfileInt(section, "MaxStatPoint_AL2", 0, path);

	this->m_MaxStatPoint[3] = GetPrivateProfileInt(section, "MaxStatPoint_AL3", 0, path);

	this->m_LevelUpPoint[0] = GetPrivateProfileInt(section, "DWLevelUpPoint", 0, path);

	this->m_LevelUpPoint[1] = GetPrivateProfileInt(section, "DKLevelUpPoint", 0, path);

	this->m_LevelUpPoint[2] = GetPrivateProfileInt(section, "FELevelUpPoint", 0, path);

	this->m_LevelUpPoint[3] = GetPrivateProfileInt(section, "MGLevelUpPoint", 0, path);

	this->m_PlusStatPoint = GetPrivateProfileInt(section, "PlusStatPoint", 0, path);

	this->m_CharacterCreateSwitch = GetPrivateProfileInt(section, "CharacterCreateSwitch", 0, path);

	this->m_MGCreateLevel[0] = GetPrivateProfileInt(section, "MGCreateLevel_AL0", 0, path);

	this->m_MGCreateLevel[1] = GetPrivateProfileInt(section, "MGCreateLevel_AL1", 0, path);

	this->m_MGCreateLevel[2] = GetPrivateProfileInt(section, "MGCreateLevel_AL2", 0, path);

	this->m_MGCreateLevel[3] = GetPrivateProfileInt(section, "MGCreateLevel_AL3", 0, path);

	this->m_CharacterDeleteSwitch = GetPrivateProfileInt(section, "CharacterDeleteSwitch", 0, path);

	this->m_CharacterDeleteMaxLevel[0] = GetPrivateProfileInt(section, "CharacterDeleteMaxLevel_AL0", 0, path);

	this->m_CharacterDeleteMaxLevel[1] = GetPrivateProfileInt(section, "CharacterDeleteMaxLevel_AL1", 0, path);

	this->m_CharacterDeleteMaxLevel[2] = GetPrivateProfileInt(section, "CharacterDeleteMaxLevel_AL2", 0, path);

	this->m_CharacterDeleteMaxLevel[3] = GetPrivateProfileInt(section, "CharacterDeleteMaxLevel_AL3", 0, path);

	this->m_PartyLevelDifference = GetPrivateProfileInt(section, "PartyLevelDifference", 0, path);

	this->m_PartyReconnectTime = GetPrivateProfileInt(section, "PartyReconnectTime", 0, path);

	this->m_PartyGeneralExperience[0] = GetPrivateProfileInt(section, "PartyGeneralExperience1", 0, path);

	this->m_PartyGeneralExperience[1] = GetPrivateProfileInt(section, "PartyGeneralExperience2", 0, path);

	this->m_PartyGeneralExperience[2] = GetPrivateProfileInt(section, "PartyGeneralExperience3", 0, path);

	this->m_PartyGeneralExperience[3] = GetPrivateProfileInt(section, "PartyGeneralExperience4", 0, path);

	this->m_PartyGeneralExperience[4] = GetPrivateProfileInt(section, "PartyGeneralExperience5", 0, path);

	this->m_PartySpecialExperience[0] = GetPrivateProfileInt(section, "PartySpecialExperience1", 0, path);

	this->m_PartySpecialExperience[1] = GetPrivateProfileInt(section, "PartySpecialExperience2", 0, path);

	this->m_PartySpecialExperience[2] = GetPrivateProfileInt(section, "PartySpecialExperience3", 0, path);

	this->m_PartySpecialExperience[3] = GetPrivateProfileInt(section, "PartySpecialExperience4", 0, path);

	this->m_PartySpecialExperience[4] = GetPrivateProfileInt(section, "PartySpecialExperience5", 0, path);

	this->m_ApplePotionRate = GetPrivateProfileInt(section, "ApplePotionRate", 0, path);

	this->m_SmallLifePotionRate = GetPrivateProfileInt(section, "SmallLifePotionRate", 0, path);

	this->m_MidleLifePotionRate = GetPrivateProfileInt(section, "MidleLifePotionRate", 0, path);

	this->m_LargeLifePotionRate = GetPrivateProfileInt(section, "LargeLifePotionRate", 0, path);

	this->m_SmallManaPotionRate = GetPrivateProfileInt(section, "SmallManaPotionRate", 0, path);

	this->m_MidleManaPotionRate = GetPrivateProfileInt(section, "MidleManaPotionRate", 0, path);

	this->m_LargeManaPotionRate = GetPrivateProfileInt(section, "LargeManaPotionRate", 0, path);

	this->m_TransformationRing1 = GetPrivateProfileInt(section, "TransformationRing1", 0, path);

	this->m_TransformationRing2 = GetPrivateProfileInt(section, "TransformationRing2", 0, path);

	this->m_TransformationRing3 = GetPrivateProfileInt(section, "TransformationRing3", 0, path);

	this->m_TransformationRing4 = GetPrivateProfileInt(section, "TransformationRing4", 0, path);

	this->m_TransformationRing5 = GetPrivateProfileInt(section, "TransformationRing5", 0, path);

	this->m_TransformationRing6 = GetPrivateProfileInt(section, "TransformationRing6", 0, path);

	this->m_JewelOfSoulSuccessRate[0] = GetPrivateProfileInt(section, "JewelOfSoulSuccessRate_AL0", 0, path);

	this->m_JewelOfSoulSuccessRate[1] = GetPrivateProfileInt(section, "JewelOfSoulSuccessRate_AL1", 0, path);

	this->m_JewelOfSoulSuccessRate[2] = GetPrivateProfileInt(section, "JewelOfSoulSuccessRate_AL2", 0, path);

	this->m_JewelOfSoulSuccessRate[3] = GetPrivateProfileInt(section, "JewelOfSoulSuccessRate_AL3", 0, path);

	this->m_JewelOfLifeSuccessRate[0] = GetPrivateProfileInt(section, "JewelOfLifeSuccessRate_AL0", 0, path);

	this->m_JewelOfLifeSuccessRate[1] = GetPrivateProfileInt(section, "JewelOfLifeSuccessRate_AL1", 0, path);

	this->m_JewelOfLifeSuccessRate[2] = GetPrivateProfileInt(section, "JewelOfLifeSuccessRate_AL2", 0, path);

	this->m_JewelOfLifeSuccessRate[3] = GetPrivateProfileInt(section, "JewelOfLifeSuccessRate_AL3", 0, path);

	this->m_JewelOfSoulAddLuckRate[0] = GetPrivateProfileInt(section, "JewelOfSoulAddLuckRate_AL0", 0, path);

	this->m_JewelOfSoulAddLuckRate[1] = GetPrivateProfileInt(section, "JewelOfSoulAddLuckRate_AL1", 0, path);

	this->m_JewelOfSoulAddLuckRate[2] = GetPrivateProfileInt(section, "JewelOfSoulAddLuckRate_AL2", 0, path);

	this->m_JewelOfSoulAddLuckRate[3] = GetPrivateProfileInt(section, "JewelOfSoulAddLuckRate_AL3", 0, path);

	this->m_FruitAddPointMin = GetPrivateProfileInt(section, "FruitAddPointMin", 0, path);

	this->m_FruitAddPointMax = GetPrivateProfileInt(section, "FruitAddPointMax", 0, path);

	this->m_FruitAddPointSuccessRate[0] = GetPrivateProfileInt(section, "FruitAddPointSuccessRate_AL0", 0, path);

	this->m_FruitAddPointSuccessRate[1] = GetPrivateProfileInt(section, "FruitAddPointSuccessRate_AL1", 0, path);

	this->m_FruitAddPointSuccessRate[2] = GetPrivateProfileInt(section, "FruitAddPointSuccessRate_AL2", 0, path);

	this->m_FruitAddPointSuccessRate[3] = GetPrivateProfileInt(section, "FruitAddPointSuccessRate_AL3", 0, path);
}

void CServerInfo::ReadEventInfo(char* section, char* path)
{
	this->m_BloodCastleEvent = GetPrivateProfileInt(section, "BloodCastleEvent", 0, path);

	this->m_BloodCastleMaxUser = GetPrivateProfileInt(section, "BloodCastleMaxUser", 0, path);

	this->m_BloodCastleAllowPK = GetPrivateProfileInt(section, "BloodCastleAllowPK", 1, path);

	this->m_BonusManagerSwitch = GetPrivateProfileInt(section, "BonusManagerSwitch", 0, path);

	this->m_DevilSquareEvent = GetPrivateProfileInt(section, "DevilSquareEvent", 0, path);

	this->m_DevilSquareMaxUser = GetPrivateProfileInt(section, "DevilSquareMaxUser", 0, path);

	this->m_DevilSquareAllowPK = GetPrivateProfileInt(section, "DevilSquareAllowPK", 1, path);

	this->m_InvasionManagerSwitch = GetPrivateProfileInt(section, "InvasionManagerSwitch", 0, path);

	this->m_FlyingDragonsSwitch = GetPrivateProfileInt(section, "FlyingDragonsSwitch", 0, path);

	this->m_FlyingDragonsOnlyBossMapSpawn = GetPrivateProfileInt(section, "FlyingDragonsOnlyBossMapSpawn", 0, path);

	this->m_FlyingDragonsKillBossDisappear = GetPrivateProfileInt(section, "FlyingDragonsKillBossDisappear", 0, path);

	this->m_GoldenArcherBingoEvent = GetPrivateProfileInt(section, "GoldenArcherBingoEvent", 0, path);

	this->m_GoldenArcherWinnersAmount = GetPrivateProfileInt(section, "GoldenArcherWinnersAmount", 1, path);

	this->m_GoldenArcherDeleteRegistered = GetPrivateProfileInt(section, "GoldenArcherDeleteRegistered", 1, path);
}

void CServerInfo::ReadHackInfo(char* section, char* path)
{
	this->m_CheckSpeedHack = GetPrivateProfileInt(section, "CheckSpeedHack", 0, path);

	this->m_CheckSpeedHackTolerance = GetPrivateProfileInt(section, "CheckSpeedHackTolerance", 0, path);

	this->m_CheckLatencyHack = GetPrivateProfileInt(section, "CheckLatencyHack", 0, path);

	this->m_CheckLatencyHackTolerance = GetPrivateProfileInt(section, "CheckLatencyHackTolerance", 0, path);

	this->m_CheckAutoPotionHack = GetPrivateProfileInt(section, "CheckAutoPotionHack", 0, path);

	this->m_CheckAutoPotionHackTolerance = GetPrivateProfileInt(section, "CheckAutoPotionHackTolerance", 0, path);
}

void CServerInfo::ReadSkillInfo(char* section, char* path)
{
	this->m_ManaShieldConstA = GetPrivateProfileInt(section, "ManaShieldConstA", 0, path);

	this->m_ManaShieldConstB = GetPrivateProfileInt(section, "ManaShieldConstB", 0, path);

	this->m_ManaShieldConstC = GetPrivateProfileInt(section, "ManaShieldConstC", 0, path);

	this->m_ManaShieldRate[0] = GetPrivateProfileInt(section, "ManaShieldRateDW", 0, path);

	this->m_ManaShieldRate[1] = GetPrivateProfileInt(section, "ManaShieldRateDK", 0, path);

	this->m_ManaShieldRate[2] = GetPrivateProfileInt(section, "ManaShieldRateFE", 0, path);

	this->m_ManaShieldRate[3] = GetPrivateProfileInt(section, "ManaShieldRateMG", 0, path);

	this->m_ManaShieldTime = GetPrivateProfileInt(section, "ManaShieldTime", 0, path);

	this->m_ManaShieldMaxRate = GetPrivateProfileInt(section, "ManaShieldMaxRate", 0, path);

	this->m_DefenseConstA = GetPrivateProfileInt(section, "DefenseConstA", 0, path);

	this->m_DefenseTime = GetPrivateProfileInt(section, "DefenseTime", 0, path);

	this->m_HealConstA = GetPrivateProfileInt(section, "HealConstA", 0, path);

	this->m_HealConstB = GetPrivateProfileInt(section, "HealConstB", 0, path);

	this->m_GreaterDefenseConstA = GetPrivateProfileInt(section, "GreaterDefenseConstA", 0, path);

	this->m_GreaterDefenseConstB = GetPrivateProfileInt(section, "GreaterDefenseConstB", 0, path);

	this->m_GreaterDefenseRate[0] = GetPrivateProfileInt(section, "GreaterDefenseRateDW", 0, path);

	this->m_GreaterDefenseRate[1] = GetPrivateProfileInt(section, "GreaterDefenseRateDK", 0, path);

	this->m_GreaterDefenseRate[2] = GetPrivateProfileInt(section, "GreaterDefenseRateFE", 0, path);

	this->m_GreaterDefenseRate[3] = GetPrivateProfileInt(section, "GreaterDefenseRateMG", 0, path);

	this->m_GreaterDefenseTime = GetPrivateProfileInt(section, "GreaterDefenseTime", 0, path);

	this->m_GreaterDamageConstA = GetPrivateProfileInt(section, "GreaterDamageConstA", 0, path);

	this->m_GreaterDamageConstB = GetPrivateProfileInt(section, "GreaterDamageConstB", 0, path);

	this->m_GreaterDamageRate[0] = GetPrivateProfileInt(section, "GreaterDamageRateDW", 0, path);

	this->m_GreaterDamageRate[1] = GetPrivateProfileInt(section, "GreaterDamageRateDK", 0, path);

	this->m_GreaterDamageRate[2] = GetPrivateProfileInt(section, "GreaterDamageRateFE", 0, path);

	this->m_GreaterDamageRate[3] = GetPrivateProfileInt(section, "GreaterDamageRateMG", 0, path);

	this->m_GreaterDamageTime = GetPrivateProfileInt(section, "GreaterDamageTime", 0, path);

	this->m_SummonMonster1 = GetPrivateProfileInt(section, "SummonMonster1", 0, path);

	this->m_SummonMonster2 = GetPrivateProfileInt(section, "SummonMonster2", 0, path);

	this->m_SummonMonster3 = GetPrivateProfileInt(section, "SummonMonster3", 0, path);

	this->m_SummonMonster4 = GetPrivateProfileInt(section, "SummonMonster4", 0, path);

	this->m_SummonMonster5 = GetPrivateProfileInt(section, "SummonMonster5", 0, path);

	this->m_SummonMonster6 = GetPrivateProfileInt(section, "SummonMonster6", 0, path);

	this->m_SummonMonster7 = GetPrivateProfileInt(section, "SummonMonster7", 0, path);

	this->m_GreaterLifeConstA = GetPrivateProfileInt(section, "GreaterLifeConstA", 0, path);

	this->m_GreaterLifeConstB = GetPrivateProfileInt(section, "GreaterLifeConstB", 0, path);

	this->m_GreaterLifeConstC = GetPrivateProfileInt(section, "GreaterLifeConstC", 0, path);

	this->m_GreaterLifeRate[0] = GetPrivateProfileInt(section, "GreaterLifeRateDW", 0, path);

	this->m_GreaterLifeRate[1] = GetPrivateProfileInt(section, "GreaterLifeRateDK", 0, path);

	this->m_GreaterLifeRate[2] = GetPrivateProfileInt(section, "GreaterLifeRateFE", 0, path);

	this->m_GreaterLifeRate[3] = GetPrivateProfileInt(section, "GreaterLifeRateMG", 0, path);

	this->m_GreaterLifeTime = GetPrivateProfileInt(section, "GreaterLifeTime", 0, path);

	this->m_GreaterLifeMaxRate = GetPrivateProfileInt(section, "GreaterLifeMaxRate", 0, path);

	this->m_FireSlashConstA = GetPrivateProfileInt(section, "FireSlashConstA", 0, path);

	this->m_FireSlashConstB = GetPrivateProfileInt(section, "FireSlashConstB", 0, path);

	this->m_FireSlashTime = GetPrivateProfileInt(section, "FireSlashTime", 0, path);

	this->m_FireSlashMaxRate = GetPrivateProfileInt(section, "FireSlashMaxRate", 0, path);
}