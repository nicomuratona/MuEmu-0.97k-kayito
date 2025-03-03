#pragma once

#include "ProtocolDefines.h"

enum eGoldenArcherTypes
{
	GOLDEN_ARCHER_RENA_COLLECTOR = 0,
	GOLDEN_ARCHER_STONE_COLLECTOR = 1,
	GOLDEN_ARCHER_LUCKY_NUMBER_EXCHANGE = 2,
};

enum eGoldenArcherCoinType
{
	GOLDEN_ARCHER_COIN_RENA = 0,
	GOLDEN_ARCHER_COIN_STONE = 1,
	GOLDEN_ARCHER_MAX_COINS
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_GOLDEN_ARCHER_GET_VALUES_SEND
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

struct SDHP_GOLDEN_ARCHER_SAVE_LUCKY_NUMBER_SEND
{
	PSBMSG_HEAD header; // C1:04:03
	WORD index;
	char account[11];
	char LuckyNumber[13];
	BYTE Result;
};

struct SDHP_BINGO_GET_WINNERS_SEND
{
	PSWMSG_HEAD header; // C2:04:04
	int count;
};

struct BINGO_WINNER
{
	char Account[11];
	char LuckyNumber[13];
};

struct SDHP_BINGO_CLEAR_REGISTERED_SEND
{
	PSWMSG_HEAD header; // C2:04:05
	BYTE result;
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_GOLDEN_ARCHER_GET_VALUES_RECV
{
	PSBMSG_HEAD header; // C1:04:00
	WORD index;
	char account[11];
};

struct SDHP_GOLDEN_ARCHER_SAVE_COIN_RECV
{
	PSBMSG_HEAD header; // C1:04:01
	WORD index;
	char account[11];
	int count[GOLDEN_ARCHER_MAX_COINS];
};

struct SDHP_GOLDEN_ARCHER_SAVE_LUCKY_NUMBER_RECV
{
	PSBMSG_HEAD header; // C1:04:02 | C1:04:03
	WORD index;
	char account[11];
	char LuckyNumber[13];
};

struct SDHP_BINGO_GET_WINNERS_RECV
{
	PSBMSG_HEAD header; // C1:04:04
	int count;
};

struct SDHP_BINGO_CLEAR_REGISTERED_RECV
{
	PSBMSG_HEAD header; // C1:04:05
};

//**********************************************//
//**********************************************//
//**********************************************//

class CGoldenArcher
{
public:

	CGoldenArcher();

	virtual ~CGoldenArcher();

	void GDGoldenArcherGetValuesRecv(SDHP_GOLDEN_ARCHER_GET_VALUES_RECV* lpMsg, int index);

	void GDGoldenArcherSaveCoinRecv(SDHP_GOLDEN_ARCHER_SAVE_COIN_RECV* lpMsg);

	void GDGoldenArcherSaveLuckyNumberRecv(SDHP_GOLDEN_ARCHER_SAVE_LUCKY_NUMBER_RECV* lpMsg);

	void GDGoldenArcherRegisterLuckyNumberRecv(SDHP_GOLDEN_ARCHER_SAVE_LUCKY_NUMBER_RECV* lpMsg, int index);

	void GDBingoGetWinnersRecv(SDHP_BINGO_GET_WINNERS_RECV* lpMsg, int index);

	void GDBingoClearRegisteredRecv(SDHP_BINGO_CLEAR_REGISTERED_RECV* lpMsg, int index);
};

extern CGoldenArcher gGoldenArcher;