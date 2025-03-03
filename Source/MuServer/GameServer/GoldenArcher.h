#pragma once

#include "ProtocolDefines.h"

enum eGoldenArcherTypes
{
	GOLDEN_ARCHER_RENA_COLLECTOR = 0,
	GOLDEN_ARCHER_STONE_COLLECTOR = 1,
	GOLDEN_ARCHER_LUCKY_NUMBER_EXCHANGE = 2,
	GOLDEN_ARCHER_BINGO_LIST = 3,
};

enum eGoldenArcherCoinType
{
	GOLDEN_ARCHER_COIN_RENA = 0,
	GOLDEN_ARCHER_COIN_STONE = 1,
	GOLDEN_ARCHER_MAX_COINS
};

enum eGoldenArcherRenaExchangeType
{
	GOLDEN_ARCHER_EXCHANGE_RENA1 = 0,
	GOLDEN_ARCHER_EXCHANGE_RENA2 = 1,
	GOLDEN_ARCHER_EXCHANGE_RENA3 = 2,
	GOLDEN_ARCHER_EXCHANGE_RENA4 = 3,
	GOLDEN_ARCHER_MAX_EXCHANGE_RENAS
};

enum eGoldenArcherStoneExchangeType
{
	GOLDEN_ARCHER_EXCHANGE_STONE1 = 0,
	GOLDEN_ARCHER_EXCHANGE_STONE2 = 1,
	GOLDEN_ARCHER_EXCHANGE_STONE3 = 2,
	GOLDEN_ARCHER_EXCHANGE_STONE4 = 3,
	GOLDEN_ARCHER_MAX_EXCHANGE_STONES
};

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_GOLDEN_ARCHER_REGISTER_RECV
{
	PBMSG_HEAD header; // C1:95
	BYTE Type;
};

struct PMSG_GOLDEN_ARCHER_EXCHANGE_RECV
{
	PSBMSG_HEAD header; // C1:97:02
	BYTE Type;
	short Count;
};

struct PMSG_GOLDEN_ARCHER_REGISTER_LUCKY_RECV
{
	PSBMSG_HEAD header; // C1:97:03
	char LuckyNumber[13];
};

struct PMSG_GOLDEN_ARCHER_EXCHANGE_LUCKY_RECV
{
	PBMSG_HEAD header; // C1:9D
	char LuckyNumber[13];
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_NPC_GOLDEN_ARCHER_SEND
{
	PBMSG_HEAD header; // C1:94
	BYTE Type;
	short Count;
	char LuckyNumber[13];
};

struct PMSG_GOLDEN_ARCHER_REGISTER_SEND
{
	PBMSG_HEAD header; // C1:95
	short Count;
};

struct PMSG_GOLDEN_ARCHER_LUCKY_NUMBER_SEND
{
	PBMSG_HEAD header; // C1:96
	char LuckyNumber[13];
};

struct PMSG_GOLDEN_ARCHER_INFO_SEND
{
	PSBMSG_HEAD header; // C1:97:00
	int RenaExchangeRequiredCount[GOLDEN_ARCHER_MAX_EXCHANGE_RENAS];
	int StoneExchangeRequiredCount[GOLDEN_ARCHER_MAX_EXCHANGE_STONES];
	DWORD StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE4];
};

struct PMSG_GOLDEN_ARCHER_LIST_SEND
{
	PSWMSG_HEAD header; // C1:97:01
	int count;
};

struct LUCKY_NUMBER_INFO
{
	char LuckyNumber[13];
};

struct PMSG_GOLDEN_ARCHER_REGISTER_LUCKY_SEND
{
	PSBMSG_HEAD header; // C1:97:03
	BYTE Result;
};

struct PMSG_GOLDEN_ARCHER_EXCHANGE_LUCKY_SEND
{
	PBMSG_HEAD header; // C1:9D
	BYTE Result;
};

struct PMSG_GOLDEN_ARCHER_CLOSE_SEND
{
	PSBMSG_HEAD header; // C1:97:04
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_GOLDEN_ARCHER_GET_VALUES_RECV
{
	PSWMSG_HEAD header; // C1:04:00
	WORD index;
	char account[11];
	int count[GOLDEN_ARCHER_MAX_COINS];
	char LuckyNumber[13];
	int registered;
};

struct LUCKY_NUMBERS_REGISTERED
{
	char LuckyNumber[13];
};

struct SDHP_GOLDEN_ARCHER_SAVE_LUCKY_NUMBER_RECV
{
	PSBMSG_HEAD header; // C1:04:03
	WORD index;
	char account[11];
	char LuckyNumber[13];
	BYTE Result;
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_GOLDEN_ARCHER_GET_VALUES_SEND
{
	PSBMSG_HEAD header; // C1:04:00
	WORD index;
	char account[11];
};

struct SDHP_GOLDEN_ARCHER_SAVE_COIN_SEND
{
	PSBMSG_HEAD header; // C1:04:01
	WORD index;
	char account[11];
	int count[GOLDEN_ARCHER_MAX_COINS];
};

struct SDHP_GOLDEN_ARCHER_SAVE_LUCKY_NUMBER_SEND
{
	PSBMSG_HEAD header; // C1:04:02 | C1:04:03
	WORD index;
	char account[11];
	char LuckyNumber[13];
};

//**********************************************//
//**********************************************//
//**********************************************//

class CGoldenArcher
{
public:

	CGoldenArcher();

	virtual ~CGoldenArcher();

	void ReadArcherInfo(char* section, char* path);

	void GDGoldenArcherGetValuesSend(int aIndex); // 0x04,0x00 Send to DataServer
	void DGGoldenArcherGetValuesRecv(SDHP_GOLDEN_ARCHER_GET_VALUES_RECV* lpMsg); // 0x04,0x00 Response from DataServer
	void GCGoldenArcherInfoSend(int aIndex); // 0x97,0x00 Send to Client

	void NpcGoldenArcher(int iNpcIndex, int iObjIndex); // Open NPC from Client
	void NpcGoldenArcherOpenSend(int aIndex, int Type); // 0x94 Response to Client
	void NpcGoldenArcherListSend(int aIndex); // 0x97,0x01 Response to Client

	void CGGoldenArcherRegisterRecv(PMSG_GOLDEN_ARCHER_REGISTER_RECV* lpMsg, int aIndex); // 0x95 Send from Client
	void GCGoldenArcherRegisterSend(int aIndex, int Count); // 0x95 Response to Client
	void GDGoldenArcherSaveCoinSend(int aIndex); // 0x04,0x01 Send to DataServer (No answer expected)

	void CGGoldenArcherExchangeRecv(PMSG_GOLDEN_ARCHER_EXCHANGE_RECV* lpMsg, int aIndex); // 0x97,0x02 Send from Client
	// Exchange Renas
	void GoldenArcherExchangeRena(int aIndex, int Type, int Count); // Support for 0x97,0x02 Send from Client
	// Exchange Stones
	void GoldenArcherExchangeStone(int aIndex, int Type, int Count); // Support for 0x97,0x02 Send from Client
	void GoldenArcherExchangeStoneVIP(int aIndex, int Type, int Count); // Support
	void GoldenArcherExchangeStoneLuckyNumber(int aIndex, int Type, int Count); // Support
	void GCGoldenArcherLuckyNumberSend(int aIndex); // 0x96 Response to Client
	void GDGoldenArcherSaveLuckyNumberSend(int aIndex); // 0x04,0x02 Send to DataServer (No answer expected)

	void CGGoldenArcherRegisterLuckyRecv(PMSG_GOLDEN_ARCHER_REGISTER_LUCKY_RECV* lpMsg, int aIndex); // 0x97,0x03 Send from Client
	void GDGoldenArcherRegisterLuckyNumberSend(int aIndex); // 0x04,0x03 Send to DataServer
	void DGGoldenArcherRegisterLuckyRecv(SDHP_GOLDEN_ARCHER_SAVE_LUCKY_NUMBER_RECV* lpMsg); // 0x04,0x03 Response from DataServer
	void GCGoldenArcherRegisterLuckySend(int aIndex, int Result); // 0x97,0x03 Response to Client

	void CGGoldenArcherExchangeLuckyRecv(PMSG_GOLDEN_ARCHER_EXCHANGE_LUCKY_RECV* lpMsg, int aIndex); // 0x9D Send from Client
	void GCGoldenArcherExchangeLuckySend(int aIndex, int Result); // 0x9D Response to Client

	void GCGoldenArcherCloseSend(int aIndex); // 0x97,0x04 Send to Client

	void ClearRegisteredLuckyNumbers();

private:

	int m_RenaExchangeRequiredCount[GOLDEN_ARCHER_MAX_EXCHANGE_RENAS];

	int m_StoneExchangeRequiredCount[GOLDEN_ARCHER_MAX_EXCHANGE_STONES];
	DWORD m_StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE4];
};

extern CGoldenArcher gGoldenArcher;