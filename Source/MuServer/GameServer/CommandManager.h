#pragma once

#include "ProtocolDefines.h"
#include "User.h"

enum eCommandNumber
{
	COMMAND_MOVE = 0,
	COMMAND_POST = 1,
	COMMAND_ADD_STR = 2,
	COMMAND_ADD_DEX = 3,
	COMMAND_ADD_VIT = 4,
	COMMAND_ADD_ENE = 5,
	COMMAND_RESET = 6,
	COMMAND_GRAND_RESET = 7,
	COMMAND_PK_CLEAR = 8,
	COMMAND_ADD_MONEY = 9,
	COMMAND_SUB_MONEY = 10,
	COMMAND_CHANGE = 11,
	COMMAND_WARE = 12,
	COMMAND_ONLINES = 13,
	COMMAND_GUILD_WAR = 14,
	COMMAND_BATTLE_SOCCER = 15,
	COMMAND_REQUEST = 16,
	COMMAND_GM_GLOBAL = 17,
	COMMAND_GM_MOVE = 18,
	COMMAND_GM_MOVEALL = 19,
	COMMAND_GM_CHASE = 20,
	COMMAND_GM_BRING = 21,
	COMMAND_GM_DISCONNECT = 22,
	COMMAND_GM_FIREWORKS = 23,
	COMMAND_GM_DROP = 24,
	COMMAND_GM_MAKE = 25,
	COMMAND_GM_MAKESET = 26,
	COMMAND_GM_CLEARINV = 27,
	COMMAND_GM_SKIN = 28,
	COMMAND_GM_MAKEMOB = 29,
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_GLOBAL_POST_RECV
{
	PSBMSG_HEAD header; // C1:05:03
	BYTE type;
	char name[11];
	char message[60];
	char serverName[60];
};

struct SDHP_COMMAND_RESET_RECV
{
	PSBMSG_HEAD header; // C1:05:04 | C1:05:05
	WORD index;
	char account[11];
	char name[11];
	UINT ResetDay;
	UINT ResetWek;
	UINT ResetMon;
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_GLOBAL_POST_SEND
{
	PSBMSG_HEAD header; // C1:05:03
	BYTE type;
	char name[11];
	char message[60];
	char serverName[60];
};

struct SDHP_COMMAND_RESET_SEND
{
	PSBMSG_HEAD header; // C1:05:04 | C1:05:05
	WORD index;
	char account[11];
	char name[11];
};

//**********************************************//
//********** JoinServer -> GameServer **********//
//**********************************************//

struct SDHP_COUNT_ONLINE_USER_RECV
{
	PBMSG_HEAD header; // C1:07
	int index;
	int count;
};

//**********************************************//
//********** GameServer -> JoinServer **********//
//**********************************************//

struct SDHP_COUNT_ONLINE_USER_SEND
{
	PBMSG_HEAD header; // C1:07
	int index;
};

//**********************************************//
//**********************************************//
//**********************************************//

struct COMMAND_LIST
{
	int Index;
	char Command[128];
	int Enable[MAX_ACCOUNT_LEVEL];
	int Money[MAX_ACCOUNT_LEVEL];
	int MinLevel[MAX_ACCOUNT_LEVEL];
	int MaxLevel[MAX_ACCOUNT_LEVEL];
	int MinReset[MAX_ACCOUNT_LEVEL];
	int MaxReset[MAX_ACCOUNT_LEVEL];
	int Delay;
	int GameMaster;
};

class CCommandManager
{
public:

	CCommandManager();

	~CCommandManager();

	void Load(char* path);

	void MainProc();

	long GetNumber(char* arg, int pos);

	void GetString(char* arg, char* out, int size, int pos);

	bool GetInfoByName(char* label, COMMAND_LIST* lpInfo);

	bool ManagementCore(LPOBJ lpObj, char* message);

	//Commands

	void CommandMove(LPOBJ lpObj, char* arg);

	void CommandPost(LPOBJ lpObj, char* arg);

	void GDGlobalPostSend(BYTE type, char* name, char* message);

	void DGGlobalPostRecv(SDHP_GLOBAL_POST_RECV* lpMsg);

	void GCPostMessageGold(char* name, char* serverName, int message, char* text);

	void GCPostMessageBlue(char* name, char* serverName, int message, char* text);

	void GCPostMessageGreen(char* name, char* serverName, int message, char* text);

	void CommandAddPoint(LPOBJ lpObj, char* arg, int type);

	void CommandAddPointAuto(LPOBJ lpObj, char* arg, int type);

	void CommandAddPointAutoProc(LPOBJ lpObj);

	void CommandReset(LPOBJ lpObj, char* arg);

	void CommandResetAuto(LPOBJ lpObj, char* arg);

	void CommandResetAutoProc(LPOBJ lpObj);

	void DGCommandResetRecv(SDHP_COMMAND_RESET_RECV* lpMsg);

	void CommandGrandReset(LPOBJ lpObj, char* arg);

	void DGCommandGrandResetRecv(SDHP_COMMAND_RESET_RECV* lpMsg);

	void CommandPKClear(LPOBJ lpObj, char* arg);

	void CommandAddMoney(LPOBJ lpObj, char* arg);

	void CommandSubMoney(LPOBJ lpObj, char* arg);

	void CommandChange(LPOBJ lpObj, char* arg);

	void CommandWare(LPOBJ lpObj, char* arg);

	void CommandOnlines(LPOBJ lpObj, char* arg);

	void JGCommandOnlinesRecv(SDHP_COUNT_ONLINE_USER_RECV* lpMsg);

	void CommandGuildWar(LPOBJ lpObj, char* arg);

	void CommandBattleSoccer(LPOBJ lpObj, char* arg);

	void CommandRequest(LPOBJ lpObj, char* arg);

	void CommandGMGlobal(LPOBJ lpObj, char* arg);

	void CommandGMMove(LPOBJ lpObj, char* arg);

	void CommandGMMoveAll(LPOBJ lpObj, char* arg);

	void CommandGMChase(LPOBJ lpObj, char* arg);

	void CommandGMBring(LPOBJ lpObj, char* arg);

	void CommandGMDisconnect(LPOBJ lpObj, char* arg);

	void CommandGMFireworks(LPOBJ lpObj, char* arg);

	void CommandGMDrop(LPOBJ lpObj, char* arg);

	void CommandGMMake(LPOBJ lpObj, char* arg);

	void CommandGMMakeSet(LPOBJ lpObj, char* arg);

	void CommandGMClearInv(LPOBJ lpObj, char* arg);

	void CommandGMSkin(LPOBJ lpObj, char* arg);

	void CommandGMMakeMob(LPOBJ lpObj, char* arg);

private:

	std::map<int, COMMAND_LIST> m_CommandInfo;
};

extern CCommandManager gCommandManager;