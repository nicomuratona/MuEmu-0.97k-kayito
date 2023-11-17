#pragma once

#include "Effect.h"
#include "Item.h"
#include "MonsterAIAgro.h"
#include "MonsterSkillElementOption.h"
#include "Skill.h"
#include "GuildManager.h"

#define MAX_OBJECT 10000
#define MAX_OBJECT_MONSTER 8000
#define MAX_OBJECT_USER 1000

#define OBJECT_START_MONSTER 0
#define OBJECT_START_USER (MAX_OBJECT-MAX_OBJECT_USER)

#define MAX_SELF_DEFENSE 5
#define MAX_HIT_DAMAGE 40
#define MAX_VIEWPORT 75
#define MAX_MONEY 2000000000
#define MAX_CHECKSUM_KEY 1024
#define MAX_CHARACTER_LEVEL 400
#define MAX_ACCOUNT_LEVEL 4
#define MAX_DAMAGE_REDUCTION 6
#define MAX_MESSAGE_SIZE 255
#define MAX_CHAT_MESSAGE_SIZE 59
#define MAX_ROAD_PATH_TABLE 16
#define MAX_MONSTER_SEND_MSG 20
#define MAX_MONSTER_SEND_ATTACK_MSG 30

#define OBJECT_RANGE(x) (((x)<0)?0:((x)>=MAX_OBJECT)?0:1)
#define OBJECT_MONSTER_RANGE(x) (((x)<OBJECT_START_MONSTER)?0:((x)>=MAX_OBJECT_MONSTER)?0:1)
#define OBJECT_SUMMON_RANGE(x) (((x)<MAX_OBJECT_MONSTER)?0:((x)>=OBJECT_START_USER)?0:1)
#define OBJECT_USER_RANGE(x) (((x)<OBJECT_START_USER)?0:((x)>=MAX_OBJECT)?0:1)
#define GET_MAX_RESISTANCE(x,y,z) (((((x)>(y))?(x):(y))>(z))?(((x)>(y))?(x):(y)):(z))
#define GET_MAX_WORD_VALUE(x) (((x)>65000)?65000:((WORD)(x)))
#define CHECK_RANGE(x,y) (((x)<0)?0:((x)>=(y))?0:1)

enum eObjectConnectState
{
	OBJECT_OFFLINE = 0,
	OBJECT_CONNECTED = 1,
	OBJECT_LOGGED = 2,
	OBJECT_ONLINE = 3,
};

enum eObjectType
{
	OBJECT_NONE = 0,
	OBJECT_USER = 1,
	OBJECT_MONSTER = 2,
	OBJECT_NPC = 3,
	OBJECT_ITEM = 5,
};

enum eObjectState
{
	OBJECT_EMPTY = 0,
	OBJECT_CREATE = 1,
	OBJECT_PLAYING = 2,
	OBJECT_DYING = 4,
	OBJECT_DIECMD = 8,
	OBJECT_DIED = 16,
	OBJECT_DELCMD = 32,
};

enum eInterfaceType
{
	INTERFACE_NONE = 0,
	INTERFACE_TRADE = 1,
	INTERFACE_PARTY = 2,
	INTERFACE_SHOP = 3,
	INTERFACE_GUILD = 4,
	INTERFACE_GUILD_CREATE = 5,
	INTERFACE_WAREHOUSE = 6,
	INTERFACE_CHAOS_BOX = 7,
	INTERFACE_GOLDEN_ARCHER = 9,
	INTERFACE_QUEST = 10,
	INTERFACE_TRANSFER = 11,
	INTERFACE_GUILD_REQUEST = 14,
};

enum eActionType
{
	ACTION_ATTACK1 = 100,
	ACTION_ATTACK2 = 101,
	ACTION_STAND1 = 102,
	ACTION_STAND2 = 103,
	ACTION_MOVE1 = 104,
	ACTION_MOVE2 = 105,
	ACTION_DAMAGE1 = 106,
	ACTION_DIE1 = 107,
	ACTION_SIT1 = 108,
	ACTION_POSE1 = 109,
	ACTION_HEALING1 = 110,
	ACTION_GREETING1 = 111,
	ACTION_GOODBYE1 = 112,
	ACTION_CLAP1 = 113,
	ACTION_GESTURE1 = 114,
	ACTION_DIRECTION1 = 115,
	ACTION_UNKNOWN1 = 116,
	ACTION_CRY1 = 117,
	ACTION_CHEER1 = 118,
	ACTION_AWKWARD1 = 119,
	ACTION_SEE1 = 120,
	ACTION_WIN1 = 121,
	ACTION_SMILE1 = 122,
	ACTION_SLEEP1 = 123,
	ACTION_COLD1 = 124,
	ACTION_AGAIN1 = 125,
	ACTION_RESPECT1 = 126,
	ACTION_SALUTE1 = 127,
	ACTION_RUSH1 = 128,
	ACTION_SCISSORS = 129,
	ACTION_ROCK = 130,
	ACTION_PAPER = 131,
};

enum ePkLevel
{
	PKLVL_FULLHERO = 1,
	PKLVL_HERO = 2,
	PKLVL_COMMONER = 3,
	PKLVL_WARNING = 4,
	PKLVL_OUTLAW = 5,
	PKLVL_KILLER = 6
};

enum eDamageReductionType
{
	DAMAGE_REDUCTION_EXCELLENT_ITEM = 0,
};

enum eDamageType
{
	DAMAGE_TYPE_NORMAL = 0,
	DAMAGE_TYPE_IGNORE = 0x10,
	DAMAGE_TYPE_REFLECT = 0x20,
	DAMAGE_TYPE_EXCELLENT = 0x40,
	DAMAGE_TYPE_CRITICAL = 0x80,
};

enum eExperienceType
{
	EXPERIENCE_COMMON = 0,
	EXPERIENCE_PARTY = 1,
	EXPERIENCE_QUEST = 2,
	EXPERIENCE_DEVIL_SQUARE = 3,
	EXPERIENCE_BLOOD_CASTLE = 4,
};

enum eLanguages
{
	LANGUAGE_ENGLISH = 0,
	LANGUAGE_SPANISH = 1,
	LANGUAGE_PORTUGUESE = 2
};

struct MESSAGE_STATE_MACHINE
{
	MESSAGE_STATE_MACHINE()
	{
		this->Clear();
	}

	void Clear()
	{
		this->MsgCode = -1;
		this->SendUser = -1;
		this->MsgTime = 0;
		this->SubCode = 0;
	}

	int MsgCode;
	int SendUser;
	int MsgTime;
	int SubCode;
};

struct MESSAGE_STATE_MACHINE_COMMON
{
	MESSAGE_STATE_MACHINE_COMMON()
	{
		this->CommonStruct = new MESSAGE_STATE_MACHINE;

		for (int n = 0; n < MAX_MONSTER_SEND_MSG; n++)
		{
			this->ObjectStruct[n] = this->CommonStruct;
		}
	}

	MESSAGE_STATE_MACHINE& operator[](int index)
	{
		return (*this->ObjectStruct[index]);
	}

	MESSAGE_STATE_MACHINE* CommonStruct;
	MESSAGE_STATE_MACHINE* ObjectStruct[MAX_MONSTER_SEND_MSG];
};

struct MESSAGE_STATE_MACHINE_HEADER
{
	MESSAGE_STATE_MACHINE_HEADER()
	{
		this->CommonStruct = new MESSAGE_STATE_MACHINE_COMMON;

		for (int n = 0; n < MAX_OBJECT; n++)
		{
			this->ObjectStruct[n] = this->CommonStruct;
		}
	}

	MESSAGE_STATE_MACHINE_COMMON& operator[](int index)
	{
		return (*this->ObjectStruct[index]);
	}

	MESSAGE_STATE_MACHINE_COMMON* CommonStruct;
	MESSAGE_STATE_MACHINE_COMMON* ObjectStruct[MAX_OBJECT];
};

struct MESSAGE_STATE_ATTACK_MACHINE
{
	MESSAGE_STATE_ATTACK_MACHINE()
	{
		this->Clear();
	}

	void Clear()
	{
		this->MsgCode = -1;
		this->SendUser = -1;
		this->MsgTime = 0;
		this->SubCode = 0;
		this->SubCode2 = 0;
	}

	int MsgCode;
	int SendUser;
	int MsgTime;
	int SubCode;
	int SubCode2;
};

struct MESSAGE_STATE_ATTACK_MACHINE_COMMON
{
	MESSAGE_STATE_ATTACK_MACHINE_COMMON()
	{
		this->CommonStruct = new MESSAGE_STATE_ATTACK_MACHINE;

		for (int n = 0; n < MAX_MONSTER_SEND_ATTACK_MSG; n++)
		{
			this->ObjectStruct[n] = this->CommonStruct;
		}
	}

	MESSAGE_STATE_ATTACK_MACHINE& operator[](int index)
	{
		return (*this->ObjectStruct[index]);
	}

	MESSAGE_STATE_ATTACK_MACHINE* CommonStruct;
	MESSAGE_STATE_ATTACK_MACHINE* ObjectStruct[MAX_MONSTER_SEND_ATTACK_MSG];
};

struct MESSAGE_STATE_ATTACK_MACHINE_HEADER
{
	MESSAGE_STATE_ATTACK_MACHINE_HEADER()
	{
		this->CommonStruct = new MESSAGE_STATE_ATTACK_MACHINE_COMMON;

		for (int n = 0; n < MAX_OBJECT; n++)
		{
			this->ObjectStruct[n] = this->CommonStruct;
		}
	}

	MESSAGE_STATE_ATTACK_MACHINE_COMMON& operator[](int index)
	{
		return (*this->ObjectStruct[index]);
	}

	MESSAGE_STATE_ATTACK_MACHINE_COMMON* CommonStruct;
	MESSAGE_STATE_ATTACK_MACHINE_COMMON* ObjectStruct[MAX_OBJECT];
};

struct EFFECT_OPTION
{
	int AddPhysiDamage;
	int AddMagicDamage;
	int AddCurseDamage;
	int AddMinPhysiDamage;
	int AddMaxPhysiDamage;
	int AddMinMagicDamage;
	int AddMaxMagicDamage;
	int AddMinCurseDamage;
	int AddMaxCurseDamage;
	int AddAttackSuccessRate;
	int AddPhysiSpeed;
	int AddMagicSpeed;
	int AddDefense;
	int AddDefenseSuccessRate;
	int AddMaxHP;
	int AddMaxMP;
	int AddMaxBP;
	int AddStrength;
	int AddDexterity;
	int AddVitality;
	int AddEnergy;
	int AddDamageReduction;
	int AddCriticalDamageRate;
	int AddCriticalDamage;
	int AddExcellentDamageRate;
	int AddExcellentDamage;
	int AddExperienceRate;
	int AddGrandExperienceRate;
	int AddPartyBonusExperienceRate;
	int AddIgnoreDefenseRate;
	int AddItemDropRate;
	int AddHPRecovery;
	int AddMPRecovery;
	int AddBPRecovery;
	int AddHPRecoveryRate;
	int AddMPRecoveryRate;
	int AddBPRecoveryRate;
	int AddSwordPowerDamageRate;
	int AddSwordPowerDefenseRate;
	int SubDefense;
	int MulPhysiDamage;
	int MulMagicDamage;
	int MulCurseDamage;
	int MulAttackSuccessRate;
	int MulPhysiSpeed;
	int MulMagicSpeed;
	int MulDefense;
	int MulDefenseSuccessRate;
	int MulMaxHP;
	int MulMaxMP;
	int MulMaxBP;
	int DivPhysiDamage;
	int DivMagicDamage;
	int DivCurseDamage;
	int DivAttackSuccessRate;
	int DivPhysiSpeed;
	int DivMagicSpeed;
	int DivDefense;
	int DivDefenseSuccessRate;
	int DivMaxHP;
	int DivMaxMP;
	int DivMaxBP;
};

struct ACTION_STATE
{
	DWORD Rest : 1;
	DWORD Attack : 1;
	DWORD Move : 1;
	DWORD Escape : 1;
	DWORD Emotion : 4;
	DWORD EmotionCount : 8;
};

struct VIEWPORT_STRUCT
{
#pragma pack(1)
	BYTE state;
	short index;
	BYTE type;
#pragma pack()
};

struct HIT_DAMAGE_STRUCT
{
	int index;
	int damage;
	DWORD time;
};

struct INTERFACE_STATE
{
	DWORD use : 2;
	DWORD state : 4;
	DWORD type : 10;
};

struct OBJECTSTRUCT
{
	int Index;
	int Connected;
	char LoginMessageSend;
	char LoginMessageCount;
	char CloseCount;
	char CloseType;
	BOOL EnableDelCharacter;
	struct PER_SOCKET_CONTEXT* PerSocketContext;
	SOCKET Socket;
	char IpAddr[16];
	char HardwareID[36];
	BYTE ClassCode;
	BYTE ClassFlag;
	DWORD AutoSaveTime;
	DWORD ConnectTickCount;
	DWORD ClientTickCount;
	DWORD ServerTickCount;
	BYTE CheckTickCount;
	DWORD PostTime;
	DWORD TimeCount;
	DWORD PKTickCount;
	short CheckSumTableNum;
	DWORD CheckSumTime;
	WORD Type;
	BYTE Live;
	char Account[11];
	char Name[11];
	char PersonalCode[14];
	int Lang;
	BYTE Class;
	BYTE DBClass;
	BYTE ChangeUp;
	short Level;
	int LevelUpPoint;
	WORD FruitAddPoint;
	WORD FruitSubPoint;
	DWORD Experience;
	DWORD NextExperience;
	DWORD Money;
	int Strength;
	int Dexterity;
	int Vitality;
	int Energy;
	float Life;
	float MaxLife;
	float ScriptMaxLife;
	float Mana;
	float MaxMana;
	WORD ChatLimitTime;
	BYTE ChatLimitTimeSec;
	int AddStrength;
	int AddDexterity;
	int AddVitality;
	int AddEnergy;
	int BP;
	int MaxBP;
	int AddBP;
	float VitalityToLife;
	float EnergyToMana;
	char PKCount;
	char PKLevel;
	int PKTime;
	short X;
	short Y;
	BYTE Dir;
	BYTE Map;
	int AddLife;
	int AddMana;
	BYTE StartX;
	BYTE StartY;
	short OldX;
	short OldY;
	short TX;
	short TY;
	short MTX;
	short MTY;
	int PathCount;
	int PathCur;
	char PathStartEnd;
	short PathOri[15];
	short PathX[15];
	short PathY[15];
	char PathDir[15];
	DWORD PathTime;
	DWORD Authority;
	DWORD AuthorityCode;
	DWORD Penalty;
	BYTE AccountItemBlock;
	ACTION_STATE ActionState;
	BYTE ActionNumber;
	DWORD State;
	BYTE Rest;
	char ViewState;
	WORD ViewSkillState;
	DWORD LastMoveTime;
	DWORD LastAttackTime;
	DWORD TeleportTime;
	char Teleport;
	char KillerType;
	char DieRegen;
	char RegenOk;
	BYTE RegenMapNumber;
	BYTE RegenMapX;
	BYTE RegenMapY;
	DWORD RegenTime;
	DWORD MaxRegenTime;
	short PosNum;
	DWORD CurActionTime;
	DWORD NextActionTime;
	DWORD DelayActionTime;
	char DelayLevel;
	int DrinkSpeed;
	DWORD DrinkLastTime;
	DWORD MonsterDeleteTime;
	OBJECTSTRUCT* AttackObj;
	bool AttackerKilled;
	int SelfDefense[MAX_SELF_DEFENSE];
	DWORD SelfDefenseTime[MAX_SELF_DEFENSE];
	DWORD MySelfDefenseTime;
	int PartyNumber;
	int PartyTargetUser;
	int GuildNumber;
	GUILD_INFO* Guild;
	char GuildName[11];
	int GuildStatus;
	int IsInBattleGround;
	int SummonIndex;
	int Change;
	short TargetNumber;
	short TargetShopNumber;
	short ShopNumber;
	short LastAttackerID;
	int PhysiDamageMin;
	int PhysiDamageMax;
	int MagicDamageMin;
	int MagicDamageMax;
	int CurseDamageMin;
	int CurseDamageMax;
	int PhysiDamageMaxLeft;
	int PhysiDamageMinLeft;
	int PhysiDamageMaxRight;
	int PhysiDamageMinRight;
	int DKDamageMultiplierRate;
	int AttackSuccessRate;
	int PhysiSpeed;
	int MagicSpeed;
	int Defense;
	int MagicDefense;
	int DefenseSuccessRate;
	short MoveSpeed;
	short MoveRange;
	short AttackRange;
	short AttackType;
	short ViewRange;
	short Attribute;
	short ItemRate;
	short MoneyRate;
	CSkill* SkillBackup;
	CSkill* Skill;
	WORD MultiSkillIndex;
	WORD MultiSkillCount;
	BYTE CharSet[11];
	BYTE Resistance[MAX_RESISTANCE_TYPE];
	BYTE AddResistance[MAX_RESISTANCE_TYPE];
	VIEWPORT_STRUCT* VpPlayer;
	VIEWPORT_STRUCT* VpPlayer2;
	VIEWPORT_STRUCT* VpPlayerItem;
	int VPCount;
	int VPCount2;
	int VPCountItem;
	HIT_DAMAGE_STRUCT* HitDamage;
	short HitDamageCount;
	INTERFACE_STATE Interface;
	DWORD InterfaceTime;
	char Transaction;
	CItem* Inventory;
	CItem* Inventory1;
	CItem* Inventory2;
	BYTE* InventoryMap;
	BYTE* InventoryMap1;
	BYTE* InventoryMap2;
	CItem* Trade;
	BYTE* TradeMap;
	int TradeMoney;
	bool TradeOk;
	CItem* Warehouse;
	BYTE* WarehouseMap;
	char WarehouseCount;
	short WarehousePW;
	BYTE WarehouseLock;
	int WarehouseMoney;
	int WarehouseSave;
	CItem* ChaosBox;
	BYTE* ChaosBoxMap;
	int ChaosMoney;
	int ChaosSuccessRate;
	int ChaosLock;
	DWORD Option;
	bool HaveWeaponInHand;
	int LoadWarehouse;
	BYTE Quest[50];
	bool SendQuestInfo;
	int CheckLifeTime;
	BYTE LastTeleportTime;
	BYTE ReqWarehouseOpen;
	bool IsChaosMixCompleted;
	DWORD CharSaveTime;
	CEffect* Effect;
	DWORD* SkillDelay;
	DWORD* HackPacketDelay;
	DWORD* HackPacketCount;
	int WarehouseNumber;
	int AccountLevel;
	char AccountExpireDate[20];
	char AutoPartyPassword[11];
	int AutoAddPointCount;
	int AutoAddPointStats[5];
	int AutoResetEnable;
	int AutoResetStats[5];
	int CommandManagerTransaction;
	DWORD CommandLastTick[100];
	DWORD TradeOkTime;
	DWORD PotionTime;
	DWORD HPAutoRecuperationTime;
	DWORD MPAutoRecuperationTime;
	DWORD BPAutoRecuperationTime;
	int Reset;
	int GrandReset;
	EFFECT_OPTION EffectOption;
	int ArmorSetBonus;
	int SkillDamageBonus;
	int IgnoreDefenseRate;
	int CriticalDamageRate;
	int CriticalDamage;
	int ExcellentDamageRate;
	int ExcellentDamage;
	int ResistIgnoreDefenseRate;
	int ResistCriticalDamageRate;
	int ResistExcellentDamageRate;
	int ResistStunRate;
	int ExperienceRate;
	int ItemDropRate;
	int MoneyAmountDropRate;
	int HPRecovery;
	int MPRecovery;
	int BPRecovery;
	int HPRecoveryRate;
	int MPRecoveryRate;
	int BPRecoveryRate;
	int HPRecoveryCount;
	int MPRecoveryCount;
	int BPRecoveryCount;
	int MPConsumptionRate;
	int BPConsumptionRate;
	int DamagePvP;
	int DefensePvP;
	int AttackSuccessRatePvP;
	int DefenseSuccessRatePvP;
	int ShieldDamageReduction;
	int ShieldDamageReductionTime;
	int DamageReduction[MAX_DAMAGE_REDUCTION];
	int DamageReflect;
	int HuntHP;
	int HuntMP;
	int HuntBP;
	int WeaponDurabilityRate;
	int ArmorDurabilityRate;
	int WingDurabilityRate;
	int GuardianDurabilityRate;
	int PendantDurabilityRate;
	int RingDurabilityRate;
	int DefensiveFullHPRestoreRate;
	int DefensiveFullMPRestoreRate;
	int DefensiveFullBPRestoreRate;
	int OffensiveFullHPRestoreRate;
	int OffensiveFullMPRestoreRate;
	int OffensiveFullBPRestoreRate;
	CMonsterSkillElementOption MonsterSkillElementOption;
	int BasicAI;
	int CurrentAI;
	int CurrentAIState;
	int LastAIRunTime;
	int GroupNumber;
	int SubGroupNumber;
	int GroupMemberGuid;
	int RegenType;
	CMonsterAIAgro Agro;
	int LastAutomataRuntime;
	int LastAutomataDelay;
	DWORD LastCheckTick;
};

struct OBJECTSTRUCT_HEADER
{
	OBJECTSTRUCT_HEADER()
	{
		this->CommonStruct = new OBJECTSTRUCT;

		for (int n = 0; n < MAX_OBJECT; n++)
		{
			this->ObjectStruct[n] = this->CommonStruct;
		}
	}

	OBJECTSTRUCT& operator[](int index)
	{
		return (*this->ObjectStruct[index]);
	}

	OBJECTSTRUCT* CommonStruct;
	OBJECTSTRUCT* ObjectStruct[MAX_OBJECT];
};

typedef OBJECTSTRUCT* LPOBJ;

extern OBJECTSTRUCT_HEADER gObj;

extern MESSAGE_STATE_MACHINE_HEADER gSMMsg;

extern MESSAGE_STATE_ATTACK_MACHINE_HEADER gSMAttackProcMsg;

extern DWORD gCheckSum[MAX_CHECKSUM_KEY];

extern DWORD gLevelExperience[MAX_CHARACTER_LEVEL + 1];

//**************************************************************************//
// OBJECT MAIN FUNCTIONS ***************************************************//
//**************************************************************************//

void gObjEventRunProc();

void gObjViewportProc();

void gObjFirstProc();

void gObjCloseProc();

void gObjAccountLevelProc();

//**************************************************************************//
// OBJECT BASE FUNCTIONS ***************************************************//
//**************************************************************************//

void gObjInit();

void gObjAllLogOut();

void gObjAllDisconnect();

void gObjSetExperienceTable();

void gObjCharZeroSet(int aIndex);

void gObjClearPlayerOption(LPOBJ lpObj);

void gObjClearSpecialOption(LPOBJ lpObj);

void gObjCalcExperience(LPOBJ lpObj);

bool gObjGetRandomFreeLocation(int map, int* ox, int* oy, int tx, int ty, int count);

bool gObjAllocData(int aIndex);

void gObjFreeData(int aIndex);

short gObjAddSearch(SOCKET socket, char* IpAddress);

short gObjAdd(SOCKET socket, char* IpAddress, int aIndex);

short gObjDel(int aIndex);

LPOBJ gObjFind(char* name);

int gObjCalcDistance(LPOBJ lpObj, LPOBJ lpTarget);

//**************************************************************************//
// OBJECT CHECK FUNCTIONS **************************************************//
//**************************************************************************//

bool gObjIsConnected(int aIndex);

bool gObjIsConnectedGP(int aIndex);

bool gObjIsConnectedGS(int aIndex);

bool gObjIsNameValid(int aIndex, char* name);

bool gObjIsAccountValid(int aIndex, char* account);

bool gObjIsChangeSkin(int aIndex);

bool gObjCheckMaxMoney(int aIndex, DWORD AddMoney);

bool gObjCheckPersonalCode(int aIndex, char* PersonalCode);

bool gObjCheckResistance(LPOBJ lpObj, int type);

bool gObjCheckTeleportArea(int aIndex, int x, int y);

bool gObjCheckMapTile(LPOBJ lpObj, int type);

//**************************************************************************//
// ITEM TRANSACTION FUNCTIONS **********************************************//
//**************************************************************************//

bool gObjFixInventoryPointer(int aIndex);

void gObjSetInventory1Pointer(LPOBJ lpObj);

void gObjSetInventory2Pointer(LPOBJ lpObj);

bool gObjInventoryTransaction(int aIndex);

bool gObjInventoryCommit(int aIndex);

bool gObjInventoryRollback(int aIndex);

//**************************************************************************//
// VIEWPORT FUNCTIONS ******************************************************//
//**************************************************************************//

void gObjSetViewport(int aIndex, int state);

void gObjClearViewport(LPOBJ lpObj);

void gObjViewportListProtocolDestroy(LPOBJ lpObj);

void gObjViewportListProtocolCreate(LPOBJ lpObj);

void gObjViewportListProtocol(int aIndex);

void gObjViewportListDestroy(int aIndex);

void gObjViewportListCreate(int aIndex);

//**************************************************************************//
// USER FUNCTIONS **********************************************************//
//**************************************************************************//

void gObjTeleportMagicUse(int aIndex, int x, int y);

void gObjInterfaceCheckTime(LPOBJ lpObj);

void gObjPKDownCheckTime(LPOBJ lpObj, int TargetLevel);

void gObjUserDie(LPOBJ lpObj, LPOBJ lpTarget);

void gObjPlayerKiller(LPOBJ lpObj, LPOBJ lpTarget);

BOOL gObjMoveGate(int aIndex, int gate);

void gObjTeleport(int aIndex, int map, int x, int y);

void gObjSkillUseProc(LPOBJ lpObj);

void gObjUserKill(int aIndex);

bool gObjInventorySearchSerialNumber(LPOBJ lpObj, DWORD serial);

bool gObjWarehouseSearchSerialNumber(LPOBJ lpObj, DWORD serial);

void gObjAddMsgSend(LPOBJ lpObj, int MsgCode, int SendUser, int SubCode);

void gObjAddMsgSendDelay(LPOBJ lpObj, int MsgCode, int SendUser, int MsgTimeDelay, int SubCode);

void gObjAddAttackProcMsgSendDelay(LPOBJ lpObj, int MsgCode, int SendUser, int MsgTimeDelay, int SubCode, int SubCode2);

//**************************************************************************//
// RAW FUNCTIONS ***********************************************************//
//**************************************************************************//

void gObjSecondProc();
void gObjDelayLifeCheck(int aIndex);
BOOL gObjBackSpring(LPOBJ lpObj, LPOBJ lpTarget);
BOOL gObjBackSpring2(LPOBJ lpObj, LPOBJ lpTarget, int count);
bool gObjIsSelfDefense(LPOBJ lpObj, int aTargetIndex);
void gObjCheckSelfDefense(LPOBJ lpObj, int aTargetIndex);
void gObjTimeCheckSelfDefense(LPOBJ lpObj);
void gObjSetPosition(int aIndex, int x, int y);
void gObjAuthorityCodeSet(LPOBJ lpObj);
void gObjUseDrink(LPOBJ lpObj, int level);
void gObjCustomLogPlusChaosMix(LPOBJ lpObj, int type, int index);
bool gObjCheckAutoParty(LPOBJ lpObj, LPOBJ lpTarget);

extern int gObjCount;

extern int gObjMonCount;

extern int gObjCallMonCount;

extern int gObjTotalUser;

extern int gObjTotalMonster;

extern int gCloseMsg;

extern int gCloseMsgTime;

extern int gGameServerLogOut;

extern int gGameServerDisconnect;