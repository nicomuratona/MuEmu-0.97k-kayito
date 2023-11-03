#pragma once

#include "ProtocolDefines.h"

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_NPC_GOLDEN_ARCHER_OPEN_SEND
{
	PSBMSG_HEAD header; // C1:04:00
	BYTE type;
	WORD index;
	WORD count;
};

struct SDHP_NPC_GOLDEN_ARCHER_REG_COUNT_SEND
{
	PSBMSG_HEAD header; // C1:04:01
	BYTE type;
	BYTE result;
	WORD index;
	BYTE oldCount;
	BYTE count;
	char luckyNum[16];
};

struct SDHP_NPC_GOLDEN_ARCHER_REG_LUCKYNUM_SEND
{
	PSBMSG_HEAD header; // C1:04:02
	WORD index;
	BYTE result;
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_NPC_GOLDEN_ARCHER_OPEN_RECV
{
	PSBMSG_HEAD header; // C1:04:00
	BYTE type;
	WORD index;
	char account[11];
};

struct SDHP_NPC_GOLDEN_ARCHER_REG_COUNT_RECV
{
	PSBMSG_HEAD header; // C1:04:01
	BYTE type;
	WORD index;
	BYTE count;
	char account[11];
};

struct SDHP_NPC_GOLDEN_ARCHER_REG_LUCKYNUM_RECV
{
	PSBMSG_HEAD header; // C1:04:02
	WORD index;
	char account[11];
	char serial1[5];
	char serial2[5];
	char serial3[5];
};

//**********************************************//
//**********************************************//
//**********************************************//

class CNpcTalk
{
public:

	CNpcTalk();

	~CNpcTalk();

	void GDNpcGoldenArcherOpenRecv(SDHP_NPC_GOLDEN_ARCHER_OPEN_RECV* lpMsg, int index);

	void GDNpcGoldenArcherRegisterCountRecv(SDHP_NPC_GOLDEN_ARCHER_REG_COUNT_RECV* lpMsg, int index);

	void GDNpcGoldenArcherRegisterLuckyNumRecv(SDHP_NPC_GOLDEN_ARCHER_REG_LUCKYNUM_RECV* lpMsg, int index);
};

extern CNpcTalk gNpcTalk;