#pragma once

#define INVENTORY_SIZE 76
#define MAX_EFFECT_LIST 16
#define MAX_SKILL_LIST 60
#define ITEM_BYTE_SIZE 10

#include "ProtocolDefines.h"

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_SERVER_INFO_RECV
{
	PBMSG_HEAD header; // C1:00
	BYTE type;
	WORD ServerPort;
	char ServerName[50];
	WORD ServerCode;
};

struct SDHP_CHARACTER_LIST_RECV
{
	PSBMSG_HEAD header; // C1:01:00
	WORD index;
	char account[11];
};

struct SDHP_CHARACTER_CREATE_RECV
{
	PSBMSG_HEAD header; // C1:01:01
	WORD index;
	char account[11];
	char name[11];
	BYTE Class;
};

struct SDHP_CHARACTER_DELETE_RECV
{
	PSBMSG_HEAD header; // C1:01:02
	WORD index;
	char account[11];
	char name[11];
	BYTE guild;
	char GuildName[9];
};

struct SDHP_CHARACTER_INFO_RECV
{
	PSBMSG_HEAD header; // C1:01:03
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_CHARACTER_INFO_SAVE_RECV
{
	PSWMSG_HEAD header; // C2:01:04
	WORD index;
	char account[11];
	char name[11];
	WORD Level;
	BYTE Class;
	DWORD LevelUpPoint;
	DWORD Experience;
	DWORD Money;
	DWORD Strength;
	DWORD Dexterity;
	DWORD Vitality;
	DWORD Energy;
	DWORD Life;
	DWORD MaxLife;
	DWORD Mana;
	DWORD MaxMana;
	DWORD BP;
	DWORD MaxBP;
	BYTE Inventory[INVENTORY_SIZE][ITEM_BYTE_SIZE];
	BYTE Skill[MAX_SKILL_LIST][3];
	BYTE Map;
	BYTE X;
	BYTE Y;
	BYTE Dir;
	DWORD PKCount;
	DWORD PKLevel;
	DWORD PKTime;
	BYTE Quest[50];
	WORD FruitAddPoint;
	WORD FruitSubPoint;
	BYTE Effect[MAX_EFFECT_LIST][13];
};

struct SDHP_CONNECT_CHARACTER_RECV
{
	PSBMSG_HEAD header; // C1:01:05 | C1:01:06
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_CREATE_ITEM_RECV
{
	PSBMSG_HEAD header; // C1:02:00
	WORD index;
	char account[11];
	BYTE X;
	BYTE Y;
	BYTE Map;
	WORD ItemIndex;
	BYTE Level;
	BYTE Dur;
	BYTE SkillOption;
	BYTE LuckOption;
	BYTE AddOption;
	BYTE ExceOption;
	WORD LootIndex;
};

struct SDHP_INVENTORY_ITEM_SAVE_RECV
{
	PSWMSG_HEAD header; // C2:02:01
	WORD index;
	char account[11];
	char name[11];
	BYTE Inventory[INVENTORY_SIZE][ITEM_BYTE_SIZE];
};

struct SDHP_OPTION_DATA_RECV
{
	PSBMSG_HEAD header; // C1:03:00
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_OPTION_DATA_SAVE_RECV
{
	PSBMSG_HEAD header; // C1:03:01
	WORD index;
	char account[11];
	char name[11];
	BYTE SkillKey[10];
	BYTE GameOption;
	BYTE QKey;
	BYTE WKey;
	BYTE EKey;
	BYTE ChatWindow;
};

struct SDHP_RESET_INFO_SAVE_RECV
{
	PSBMSG_HEAD header; // C1:03:02
	WORD index;
	char account[11];
	char name[11];
	UINT Reset;
	UINT ResetDay;
	UINT ResetWek;
	UINT ResetMon;
};

struct SDHP_GRAND_RESET_INFO_SAVE_RECV
{
	PSBMSG_HEAD header; // C1:03:03
	WORD index;
	char account[11];
	char name[11];
	UINT Reset;
	UINT GrandReset;
	UINT GrandResetDay;
	UINT GrandResetWek;
	UINT GrandResetMon;
};

struct SDHP_GLOBAL_NOTICE_RECV
{
	PSBMSG_HEAD header; // C1:05:00
	BYTE type;
	char message[128];
};

struct SDHP_GLOBAL_WHISPER_RECV
{
	PSBMSG_HEAD header; // C1:05:01
	WORD index;
	char account[11];
	char name[11];
	char TargetName[11];
	char message[60];
};

struct SDHP_RANKING_SAVE_RECV
{
	PSBMSG_HEAD header; // C1:06:00 | C1:06:01
	WORD index;
	char account[11];
	char name[11];
	DWORD score;
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_CHARACTER_LIST_SEND
{
	PSWMSG_HEAD header; // C2:01:00
	WORD index;
	char account[11];
	BYTE count;
};

struct SDHP_CHARACTER_LIST
{
	BYTE slot;
	char name[11];
	WORD level;
	BYTE Class;
	BYTE CtlCode;
	BYTE Inventory[60];
};

struct SDHP_CHARACTER_CREATE_SEND
{
	PSBMSG_HEAD header; // C1:01:01
	WORD index;
	char account[11];
	char name[11];
	BYTE result;
	BYTE slot;
	BYTE equipment[24];
};

struct SDHP_CHARACTER_DELETE_SEND
{
	PSBMSG_HEAD header; // C1:01:02
	WORD index;
	char account[11];
	BYTE result;
};

struct SDHP_CHARACTER_INFO_SEND
{
	PSWMSG_HEAD header; // C2:01:03
	WORD index;
	char account[11];
	char name[11];
	BYTE result;
	BYTE Class;
	WORD Level;
	DWORD LevelUpPoint;
	DWORD Experience;
	DWORD Money;
	DWORD Strength;
	DWORD Dexterity;
	DWORD Vitality;
	DWORD Energy;
	DWORD Life;
	DWORD MaxLife;
	DWORD Mana;
	DWORD MaxMana;
	DWORD BP;
	DWORD MaxBP;
	BYTE Inventory[INVENTORY_SIZE][ITEM_BYTE_SIZE];
	BYTE Skill[MAX_SKILL_LIST][3];
	BYTE Map;
	BYTE X;
	BYTE Y;
	BYTE Dir;
	DWORD PKCount;
	DWORD PKLevel;
	DWORD PKTime;
	BYTE CtlCode;
	BYTE Quest[50];
	WORD FruitAddPoint;
	WORD FruitSubPoint;
	BYTE Effect[MAX_EFFECT_LIST][13];
	DWORD Reset;
	DWORD GrandReset;
};

struct SDHP_CREATE_ITEM_SEND
{
	PSBMSG_HEAD header; // C1:02:00
	WORD index;
	char account[11];
	BYTE X;
	BYTE Y;
	BYTE Map;
	DWORD Serial;
	WORD ItemIndex;
	BYTE Level;
	BYTE Dur;
	BYTE SkillOption;
	BYTE LuckOption;
	BYTE AddOption;
	BYTE ExceOption;
	WORD LootIndex;
};

struct SDHP_OPTION_DATA_SEND
{
	PSBMSG_HEAD header; // C1:03:00
	WORD index;
	char account[11];
	char name[11];
	BYTE SkillKey[10];
	BYTE GameOption;
	BYTE QKey;
	BYTE WKey;
	BYTE EKey;
	BYTE ChatWindow;
};

struct SDHP_GLOBAL_NOTICE_SEND
{
	PSBMSG_HEAD header; // C1:05:00
	BYTE type;
	char message[128];
};

struct SDHP_GLOBAL_WHISPER_SEND
{
	PSBMSG_HEAD header; // C1:05:01
	WORD index;
	char account[11];
	char name[11];
	BYTE result;
	char TargetName[11];
	char message[60];
};

struct SDHP_GLOBAL_WHISPER_ECHO_SEND
{
	PSBMSG_HEAD header; // C1:05:02
	WORD index;
	char account[11];
	char name[11];
	char SourceName[11];
	char message[60];
};

//**********************************************//
//**********************************************//
//**********************************************//

void DataServerProtocolCore(int index, BYTE head, BYTE* lpMsg, int size);

void GDServerInfoRecv(SDHP_SERVER_INFO_RECV* lpMsg, int index);

void GDCharacterListRecv(SDHP_CHARACTER_LIST_RECV* lpMsg, int index);

void GDCharacterCreateRecv(SDHP_CHARACTER_CREATE_RECV* lpMsg, int index);

void GDCharacterDeleteRecv(SDHP_CHARACTER_DELETE_RECV* lpMsg, int index);

void GDCharacterInfoRecv(SDHP_CHARACTER_INFO_RECV* lpMsg, int index);

void GDCharacterInfoSaveRecv(SDHP_CHARACTER_INFO_SAVE_RECV* lpMsg);

void GDConnectCharacterRecv(SDHP_CONNECT_CHARACTER_RECV* lpMsg, int index);

void GDDisconnectCharacterRecv(SDHP_CONNECT_CHARACTER_RECV* lpMsg, int index);

void GDCreateItemRecv(SDHP_CREATE_ITEM_RECV* lpMsg, int index);

void GDInventoryItemSaveRecv(SDHP_INVENTORY_ITEM_SAVE_RECV* lpMsg);

void GDOptionDataRecv(SDHP_OPTION_DATA_RECV* lpMsg, int index);

void GDOptionDataSaveRecv(SDHP_OPTION_DATA_SAVE_RECV* lpMsg);

void GDResetInfoSaveRecv(SDHP_RESET_INFO_SAVE_RECV* lpMsg);

void GDGrandResetInfoSaveRecv(SDHP_GRAND_RESET_INFO_SAVE_RECV* lpMsg);

void GDGlobalNoticeRecv(SDHP_GLOBAL_NOTICE_RECV* lpMsg, int index);

void GDGlobalWhisperRecv(SDHP_GLOBAL_WHISPER_RECV* lpMsg, int index);

void DGGlobalWhisperEchoSend(WORD ServerCode, WORD index, char* account, char* name, char* SourceName, char* message);

void GDRankingBloodCastleSaveRecv(SDHP_RANKING_SAVE_RECV* lpMsg);

void GDRankingDevilSquareSaveRecv(SDHP_RANKING_SAVE_RECV* lpMsg);