#pragma once

#include "ProtocolDefines.h"
#include "User.h"

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_NPC_TALK_RECV
{
	PBMSG_HEAD header; // C1:30
	BYTE index[2];
};

struct PMSG_GOLDEN_ARCHER_REG_COUNT
{
	PBMSG_HEAD header;
	BYTE type;
	BYTE pos;
};

struct PMSG_GOLDEN_ARCHER_REG_LUCKYNUM
{
	PBMSG_HEAD header; //C1:9D
	char serial1[5];
	char serial2[5];
	char serial3[5];
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_NPC_TALK_SEND
{
	PBMSG_HEAD header; // C3:30
	BYTE result;
	BYTE level[7];
};

struct PMSG_NPC_GOLDEN_ARCHER_SEND
{
	PBMSG_HEAD header; // C1:94
	BYTE type;
	WORD count;
	WORD num[3];
};

struct PMSG_NPC_GOLDEN_ARCHER_REGISTER_SEND
{
	PBMSG_HEAD header; // C1:94
	BYTE type;
	WORD count;
};

struct PMSG_ANS_LUCKYNUMBER_SEND
{
	PBMSG_HEAD header; // C1:9D
	BYTE result;
	char luckyName[64];
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_NPC_GOLDEN_ARCHER_OPEN_RECV
{
	PSBMSG_HEAD header; // C1:04:00
	BYTE type;
	WORD index;
	WORD count;
};

struct SDHP_NPC_GOLDEN_ARCHER_REG_COUNT_RECV
{
	PSBMSG_HEAD header; // C1:04:01
	BYTE type;
	BYTE result;
	WORD index;
	BYTE oldCount;
	BYTE count;
	char luckyNum[16];
};

struct SDHP_NPC_GOLDEN_ARCHER_REG_LUCKYNUM_RECV
{
	PSBMSG_HEAD header; // C1:04:02
	WORD index;
	BYTE result;
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_NPC_GOLDEN_ARCHER_OPEN_SEND
{
	PSBMSG_HEAD header; // C1:04:00
	BYTE type;
	WORD index;
	char account[11];
};

struct SDHP_NPC_GOLDEN_ARCHER_REG_COUNT_SEND
{
	PSBMSG_HEAD header; // C1:04:01
	BYTE type;
	WORD index;
	BYTE count;
	char account[11];
};

struct SDHP_NPC_GOLDEN_ARCHER_REG_LUCKYNUM_SEND
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

	bool NpcTalk(LPOBJ lpNpc, LPOBJ lpObj);

	void NpcAngelKing(LPOBJ lpNpc, LPOBJ lpObj);

	void NpcAngelMessenger(LPOBJ lpNpc, LPOBJ lpObj);

	void NpcServerTransfer(LPOBJ lpNpc, LPOBJ lpObj);

	void NpcGoldenArcher(LPOBJ lpNpc, LPOBJ lpObj);

	void DGNpcGoldenArcherOpenRecv(SDHP_NPC_GOLDEN_ARCHER_OPEN_RECV* lpMsg);

	void NpcGoldenArcherRegisterCount(PMSG_GOLDEN_ARCHER_REG_COUNT* lpMsg, int aIndex);

	void DGNpcGoldenArcherRegisterCountRecv(SDHP_NPC_GOLDEN_ARCHER_REG_COUNT_RECV* lpMsg);

	void NpcGoldenArcherRegisterLuckyNumber(PMSG_GOLDEN_ARCHER_REG_LUCKYNUM* lpMsg, int aIndex);

	void DGNpcGoldenArcherRegisterLuckyNumRecv(SDHP_NPC_GOLDEN_ARCHER_REG_LUCKYNUM_RECV* lpMsg);

	void NpcCharon(LPOBJ lpNpc, LPOBJ lpObj);

	void NpcChaosGoblin(LPOBJ lpNpc, LPOBJ lpObj);

	void NpcWarehouse(LPOBJ lpNpc, LPOBJ lpObj);

	void NpcGuildMaster(LPOBJ lpNpc, LPOBJ lpObj);

	void NpcGuard(LPOBJ lpNpc, LPOBJ lpObj);

	void CGNpcTalkRecv(PMSG_NPC_TALK_RECV* lpMsg, int aIndex);

	void CGNpcTalkCloseRecv(int aIndex);
};

extern CNpcTalk gNpcTalk;