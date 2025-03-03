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

struct PMSG_ANS_LUCKYNUMBER_SEND
{
	PBMSG_HEAD header; // C1:9D
	BYTE result;
	char luckyName[64];
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

	void NpcCharon(LPOBJ lpNpc, LPOBJ lpObj);

	void NpcChaosGoblin(LPOBJ lpNpc, LPOBJ lpObj);

	void NpcWarehouse(LPOBJ lpNpc, LPOBJ lpObj);

	void NpcGuildMaster(LPOBJ lpNpc, LPOBJ lpObj);

	void NpcGuard(LPOBJ lpNpc, LPOBJ lpObj);

	void CGNpcTalkRecv(PMSG_NPC_TALK_RECV* lpMsg, int aIndex);

	void CGNpcTalkCloseRecv(int aIndex);
};

extern CNpcTalk gNpcTalk;