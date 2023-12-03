#pragma once

#include "DefaultClassInfo.h"
#include "Party.h"
#include "User.h"

class CServerInfo
{
public:

	CServerInfo();

	~CServerInfo();

	void ReadInit();

	void ReloadAll();

	void ReadChaosMixInfo();

	void ReadCharacterInfo();

	void ReadCommandInfo();

	void ReadCommonInfo();

	void ReadCustomInfo();

	void ReadEventInfo();

	void ReadEventItemBagInfo();

	void ReadHackInfo();

	void ReadItemInfo();

	void ReadMapInfo();

	void ReadMonsterInfo();

	void ReadMoveInfo();

	void ReadQuestInfo();

	void ReadShopInfo();

	void ReadSkillInfo();

	void ReadUtilInfo();

	void ReloadMonsterInfo();

	void ReadStartupInfo(char* section, char* path);

	void ReadChaosMixInfo(char* section, char* path);

	void ReadCharacterInfo(char* section, char* path);

	void ReadCommandInfo(char* section, char* path);

	void ReadCommonInfo(char* section, char* path);

	void ReadCustomInfo(char* section, char* path);

	void ReadEventInfo(char* section, char* path);

	void ReadHackInfo(char* section, char* path);

	void ReadSkillInfo(char* section, char* path);

public:
	// Startup
	char m_CustomerName[32];
	char m_ServerName[32];
	char m_WindowName[32];
	long m_ServerCode;
	long m_ServerLock;
	long m_ServerPort;
	char m_ServerMutex[32];
	char m_ServerVersion[6];
	char m_ServerSerial[17];
	long m_ServerMaxUserNumber;
	char m_JoinServerAddress[16];
	long m_JoinServerPort;
	char m_DataServerAddress[16];
	long m_DataServerPort;
	char m_ConnectServerAddress[16];
	long m_ConnectServerPort;
	int m_WriteChatLog;
	int m_WriteCommandLog;
	int m_WriteTradeLog;
	int m_WriteConnectLog;
	int m_WriteHackLog;
	int m_WriteChaosMixLog;
	// Hack
	int m_CheckSpeedHack;
	int m_CheckSpeedHackTolerance;
	int m_CheckLatencyHack;
	int m_CheckLatencyHackTolerance;
	int m_CheckAutoPotionHack;
	int m_CheckAutoPotionHackTolerance;
	// Common
	int m_MainChecksum;
	int m_MaxItemOption;
	int m_MaxIpConnection;
	int m_DisconnectOnlineAccount;
	int m_PersonalCodeCheck;
	int m_MonsterLifeRate;
	int m_MonsterHealthBarSwitch;
	int m_NonPK;
	int m_PKLimitFree;
	int m_PKDownTime1;
	int m_PKDownTime2;
	int m_GuildCreateSwitch;
	int m_GuildDeleteSwitch;
	int m_GuildCreateMinLevel;
	int m_GuildCreateMinReset;
	int m_GuildInsertMaxUser[MAX_ACCOUNT_LEVEL];
	int m_GuildWarWinScore;
	int m_GuildWarSoccerWinScore;
	int m_GuildWarWinPointsGain_Max;
	int m_GuildWarWinPointsGain_Half;
	int m_GuildWarWinPointsGain_Normal;
	int m_GoldenArcherRewardSwitch;
	int m_GoldenArcherMinRenas;
	int m_GoldenArcherMinStones;
	int m_GoldenArcherRewardItemIndex;
	int m_AddExperienceRate[MAX_ACCOUNT_LEVEL];
	int m_AddEventExperienceRate[MAX_ACCOUNT_LEVEL];
	int m_ItemDropTime;
	int m_ItemDropRate[MAX_ACCOUNT_LEVEL];
	int m_MoneyDropTime;
	int m_MoneyAmountDropRate[MAX_ACCOUNT_LEVEL];
	int m_WeaponDurabilityRate;
	int m_ArmorDurabilityRate;
	int m_WingDurabilityRate;
	int m_GuardianDurabilityRate;
	int m_PendantDurabilityRate;
	int m_RingDurabilityRate;
	int m_TradeSwitch;
	int m_TradeItemBlock;
	int m_TradeItemBlockExc;
	int m_TradeItemBlockSell;
	int m_ShopItemBlockSellOnMaxMoney;
	int m_MaxLevelUp;
	int m_MaxLevelUpEvent;
	int m_MaxStatPoint[MAX_ACCOUNT_LEVEL];
	int m_LevelUpPoint[MAX_CLASS];
	int m_PlusStatPoint;
	int m_CharacterCreateSwitch;
	int m_MGCreateLevel[MAX_ACCOUNT_LEVEL];
	int m_PartyLevelDifference;
	int m_PartyReconnectTime;
	int m_PartyGeneralExperience[MAX_PARTY_USER];
	int m_PartySpecialExperience[MAX_PARTY_USER];
	int m_ApplePotionRate;
	int m_SmallLifePotionRate;
	int m_MidleLifePotionRate;
	int m_LargeLifePotionRate;
	int m_SmallManaPotionRate;
	int m_MidleManaPotionRate;
	int m_LargeManaPotionRate;
	int m_TransformationRing1;
	int m_TransformationRing2;
	int m_TransformationRing3;
	int m_TransformationRing4;
	int m_TransformationRing5;
	int m_TransformationRing6;
	int m_JewelOfSoulSuccessRate[MAX_ACCOUNT_LEVEL];
	int m_JewelOfLifeSuccessRate[MAX_ACCOUNT_LEVEL];
	int m_JewelOfSoulAddLuckRate[MAX_ACCOUNT_LEVEL];
	int m_FruitAddPointMin;
	int m_FruitAddPointMax;
	int m_FruitAddPointSuccessRate[MAX_ACCOUNT_LEVEL];
	// Event
	int m_BloodCastleEvent;
	int m_BloodCastleMaxUser;
	int m_BloodCastleAllowPK;
	int m_BonusManagerSwitch;
	int m_DevilSquareEvent;
	int m_DevilSquareMaxUser;
	int m_DevilSquareAllowPK;
	int m_InvasionManagerSwitch;
	int m_FlyingDragonsSwitch;
	int m_FlyingDragonsOnlyBossMapSpawn;
	int m_FlyingDragonsKillBossDisappear;
	// Chaos Mix
	int m_ChaosItemMixRate[MAX_ACCOUNT_LEVEL];
	int m_ChaosItemMixCost[MAX_ACCOUNT_LEVEL];
	int m_ChaosItemMixAddItemRate[MAX_ACCOUNT_LEVEL];
	int m_ChaosItemMixAddJewelRate[MAX_ACCOUNT_LEVEL];
	int m_DevilSquareMixRate1[MAX_ACCOUNT_LEVEL];
	int m_DevilSquareMixRate2[MAX_ACCOUNT_LEVEL];
	int m_DevilSquareMixRate3[MAX_ACCOUNT_LEVEL];
	int m_DevilSquareMixRate4[MAX_ACCOUNT_LEVEL];
	int m_DevilSquareMixCost1[MAX_ACCOUNT_LEVEL];
	int m_DevilSquareMixCost2[MAX_ACCOUNT_LEVEL];
	int m_DevilSquareMixCost3[MAX_ACCOUNT_LEVEL];
	int m_DevilSquareMixCost4[MAX_ACCOUNT_LEVEL];
	int m_PlusItemLevelMixRate1[MAX_ACCOUNT_LEVEL];
	int m_PlusItemLevelMixRate2[MAX_ACCOUNT_LEVEL];
	int m_PlusItemLevelMixCost1[MAX_ACCOUNT_LEVEL];
	int m_PlusItemLevelMixCost2[MAX_ACCOUNT_LEVEL];
	int m_PlusItemLevelAddLuckRate[MAX_ACCOUNT_LEVEL];
	int m_DinorantMixRate[MAX_ACCOUNT_LEVEL];
	int m_DinorantMixCost[MAX_ACCOUNT_LEVEL];
	int m_FruitMixRate[MAX_ACCOUNT_LEVEL];
	int m_FruitMixCost[MAX_ACCOUNT_LEVEL];
	int m_Wing1MixRate[MAX_ACCOUNT_LEVEL];
	int m_Wing1MixCost[MAX_ACCOUNT_LEVEL];
	int m_Wing1MixAddItemRate[MAX_ACCOUNT_LEVEL];
	int m_Wing1MixAddJewelRate[MAX_ACCOUNT_LEVEL];
	int m_BloodCastleMixRate1[MAX_ACCOUNT_LEVEL];
	int m_BloodCastleMixRate2[MAX_ACCOUNT_LEVEL];
	int m_BloodCastleMixRate3[MAX_ACCOUNT_LEVEL];
	int m_BloodCastleMixRate4[MAX_ACCOUNT_LEVEL];
	int m_BloodCastleMixRate5[MAX_ACCOUNT_LEVEL];
	int m_BloodCastleMixRate6[MAX_ACCOUNT_LEVEL];
	int m_BloodCastleMixCost1[MAX_ACCOUNT_LEVEL];
	int m_BloodCastleMixCost2[MAX_ACCOUNT_LEVEL];
	int m_BloodCastleMixCost3[MAX_ACCOUNT_LEVEL];
	int m_BloodCastleMixCost4[MAX_ACCOUNT_LEVEL];
	int m_BloodCastleMixCost5[MAX_ACCOUNT_LEVEL];
	int m_BloodCastleMixCost6[MAX_ACCOUNT_LEVEL];
	int m_Wing2MixRate[MAX_ACCOUNT_LEVEL];
	int m_Wing2MixCost[MAX_ACCOUNT_LEVEL];
	int m_Wing2MixAddItemRate[MAX_ACCOUNT_LEVEL];
	// Character
	int m_GeneralDamageRatePvP;
	int m_GeneralDamageRatePvM;
	int m_ReflectDamageRatePvP;
	int m_ReflectDamageRatePvM;
	int m_DamageRatePvP[MAX_CLASS];
	int m_DamageRatePvM[MAX_CLASS];
	int m_DamageRateTo[MAX_CLASS][MAX_CLASS];
	int m_PetUniriaEnableStuck;
	int m_PetDinorantEnableStuck;
	int m_DamageStuckRate[MAX_CLASS];
	int m_DKDamageMultiplierConstA;
	int m_DKDamageMultiplierMaxRate;
	int m_SatanIncDamageConstA;
	int m_DinorantIncDamageConstA;
	int m_AngelDecDamageConstA;
	int m_DinorantDecDamageConstA;
	int m_DinorantDecDamageConstB;
	int m_HPRecoveryRate[MAX_CLASS];
	int m_MPRecoveryRate[MAX_CLASS];
	int m_BPRecoveryRate[MAX_CLASS];
	int m_DWPhysiDamageMinConstA;
	int m_DWPhysiDamageMaxConstA;
	int m_DWMagicDamageMinConstA;
	int m_DWMagicDamageMaxConstA;
	int m_DKPhysiDamageMinConstA;
	int m_DKPhysiDamageMaxConstA;
	int m_DKMagicDamageMinConstA;
	int m_DKMagicDamageMaxConstA;
	int m_FEPhysiDamageMinConstA;
	int m_FEPhysiDamageMaxConstA;
	int m_FEPhysiDamageMinBowConstA;
	int m_FEPhysiDamageMinBowConstB;
	int m_FEPhysiDamageMaxBowConstA;
	int m_FEPhysiDamageMaxBowConstB;
	int m_FEMagicDamageMinConstA;
	int m_FEMagicDamageMaxConstA;
	int m_MGPhysiDamageMinConstA;
	int m_MGPhysiDamageMinConstB;
	int m_MGPhysiDamageMaxConstA;
	int m_MGPhysiDamageMaxConstB;
	int m_MGMagicDamageMinConstA;
	int m_MGMagicDamageMaxConstA;
	int m_DWAttackSuccessRateConstA;
	int m_DWAttackSuccessRateConstB;
	int m_DWAttackSuccessRateConstC;
	int m_DWAttackSuccessRateConstD;
	int m_DKAttackSuccessRateConstA;
	int m_DKAttackSuccessRateConstB;
	int m_DKAttackSuccessRateConstC;
	int m_DKAttackSuccessRateConstD;
	int m_FEAttackSuccessRateConstA;
	int m_FEAttackSuccessRateConstB;
	int m_FEAttackSuccessRateConstC;
	int m_FEAttackSuccessRateConstD;
	int m_MGAttackSuccessRateConstA;
	int m_MGAttackSuccessRateConstB;
	int m_MGAttackSuccessRateConstC;
	int m_MGAttackSuccessRateConstD;
	int m_DWAttackSuccessRatePvPConstA;
	int m_DWAttackSuccessRatePvPConstB;
	int m_DWAttackSuccessRatePvPConstC;
	int m_DWAttackSuccessRatePvPConstD;
	int m_DKAttackSuccessRatePvPConstA;
	int m_DKAttackSuccessRatePvPConstB;
	int m_DKAttackSuccessRatePvPConstC;
	int m_DKAttackSuccessRatePvPConstD;
	int m_FEAttackSuccessRatePvPConstA;
	int m_FEAttackSuccessRatePvPConstB;
	int m_FEAttackSuccessRatePvPConstC;
	int m_FEAttackSuccessRatePvPConstD;
	int m_MGAttackSuccessRatePvPConstA;
	int m_MGAttackSuccessRatePvPConstB;
	int m_MGAttackSuccessRatePvPConstC;
	int m_MGAttackSuccessRatePvPConstD;
	int m_DWPhysiSpeedConstA;
	int m_DWMagicSpeedConstA;
	int m_DKPhysiSpeedConstA;
	int m_DKMagicSpeedConstA;
	int m_FEPhysiSpeedConstA;
	int m_FEMagicSpeedConstA;
	int m_MGPhysiSpeedConstA;
	int m_MGMagicSpeedConstA;
	int m_DWDefenseSuccessRateConstA;
	int m_DKDefenseSuccessRateConstA;
	int m_FEDefenseSuccessRateConstA;
	int m_MGDefenseSuccessRateConstA;
	int m_DWDefenseSuccessRatePvPConstA;
	int m_DWDefenseSuccessRatePvPConstB;
	int m_DWDefenseSuccessRatePvPConstC;
	int m_DKDefenseSuccessRatePvPConstA;
	int m_DKDefenseSuccessRatePvPConstB;
	int m_DKDefenseSuccessRatePvPConstC;
	int m_FEDefenseSuccessRatePvPConstA;
	int m_FEDefenseSuccessRatePvPConstB;
	int m_FEDefenseSuccessRatePvPConstC;
	int m_MGDefenseSuccessRatePvPConstA;
	int m_MGDefenseSuccessRatePvPConstB;
	int m_MGDefenseSuccessRatePvPConstC;
	int m_DWDefenseConstA;
	int m_DKDefenseConstA;
	int m_FEDefenseConstA;
	int m_MGDefenseConstA;
	// Command
	int m_CommandPostType;
	int m_CommandAddPointAutoEnable[MAX_ACCOUNT_LEVEL];
	int m_CommandResetAutoEnable[MAX_ACCOUNT_LEVEL];
	int m_CommandResetLevel[MAX_ACCOUNT_LEVEL];
	int m_CommandResetStartLevel[MAX_ACCOUNT_LEVEL];
	int m_CommandResetMoney[MAX_ACCOUNT_LEVEL];
	int m_CommandResetPoint[MAX_ACCOUNT_LEVEL];
	int m_CommandResetPointRate[MAX_CLASS];
	int m_CommandResetCount[MAX_ACCOUNT_LEVEL];
	int m_CommandResetCheckItem[MAX_ACCOUNT_LEVEL];
	int m_CommandResetStats[MAX_ACCOUNT_LEVEL];
	int m_CommandResetQuest[MAX_ACCOUNT_LEVEL];
	int m_CommandResetSkill[MAX_ACCOUNT_LEVEL];
	int m_CommandResetMove[MAX_ACCOUNT_LEVEL];
	int m_CommandResetLimit[MAX_ACCOUNT_LEVEL];
	int m_CommandResetLimitDay[MAX_ACCOUNT_LEVEL];
	int m_CommandResetLimitWek[MAX_ACCOUNT_LEVEL];
	int m_CommandResetLimitMon[MAX_ACCOUNT_LEVEL];
	int m_CommandGrandResetLevel[MAX_ACCOUNT_LEVEL];
	int m_CommandGrandResetStartLevel[MAX_ACCOUNT_LEVEL];
	int m_CommandGrandResetReset[MAX_ACCOUNT_LEVEL];
	int m_CommandGrandResetStartReset[MAX_ACCOUNT_LEVEL];
	int m_CommandGrandResetMoney[MAX_ACCOUNT_LEVEL];
	int m_CommandGrandResetPoint[MAX_ACCOUNT_LEVEL];
	int m_CommandGrandResetPointRate[MAX_CLASS];
	int m_CommandGrandResetCount[MAX_ACCOUNT_LEVEL];
	int m_CommandGrandResetCheckItem[MAX_ACCOUNT_LEVEL];
	int m_CommandGrandResetStats[MAX_ACCOUNT_LEVEL];
	int m_CommandGrandResetQuest[MAX_ACCOUNT_LEVEL];
	int m_CommandGrandResetSkill[MAX_ACCOUNT_LEVEL];
	int m_CommandGrandResetMove[MAX_ACCOUNT_LEVEL];
	int m_CommandGrandResetLimit[MAX_ACCOUNT_LEVEL];
	int m_CommandGrandResetLimitDay[MAX_ACCOUNT_LEVEL];
	int m_CommandGrandResetLimitWek[MAX_ACCOUNT_LEVEL];
	int m_CommandGrandResetLimitMon[MAX_ACCOUNT_LEVEL];
	int m_CommandWareNumber[MAX_ACCOUNT_LEVEL];
	// Skill
	int m_ManaShieldConstA;
	int m_ManaShieldConstB;
	int m_ManaShieldConstC;
	int m_ManaShieldRate[MAX_CLASS];
	int m_ManaShieldTime;
	int m_ManaShieldMaxRate;
	int m_DefenseConstA;
	int m_DefenseTime;
	int m_HealConstA;
	int m_HealConstB;
	int m_GreaterDefenseConstA;
	int m_GreaterDefenseConstB;
	int m_GreaterDefenseRate[MAX_CLASS];
	int m_GreaterDefenseTime;
	int m_GreaterDamageConstA;
	int m_GreaterDamageConstB;
	int m_GreaterDamageRate[MAX_CLASS];
	int m_GreaterDamageTime;
	int m_SummonMonster1;
	int m_SummonMonster2;
	int m_SummonMonster3;
	int m_SummonMonster4;
	int m_SummonMonster5;
	int m_SummonMonster6;
	int m_SummonMonster7;
	int m_GreaterLifeConstA;
	int m_GreaterLifeConstB;
	int m_GreaterLifeConstC;
	int m_GreaterLifeRate[MAX_CLASS];
	int m_GreaterLifeTime;
	int m_GreaterLifeMaxRate;
	int m_FireSlashConstA;
	int m_FireSlashConstB;
	int m_FireSlashTime;
	int m_FireSlashMaxRate;
	// Custom
	long m_AnnounceChaosMix;
	char m_AnnounceChaosMixText[50];
};

extern CServerInfo gServerInfo;