#pragma once

#include "ProtocolDefines.h"

enum eChaosMixNumber
{
	CHAOS_MIX_CHAOS_ITEM = 1,
	CHAOS_MIX_DEVIL_SQUARE = 2,
	CHAOS_MIX_PLUS_ITEM_LEVEL1 = 3,
	CHAOS_MIX_PLUS_ITEM_LEVEL2 = 4,
	CHAOS_MIX_DINORANT = 5,
	CHAOS_MIX_FRUIT = 6,
	CHAOS_MIX_WING2 = 7,
	CHAOS_MIX_BLOOD_CASTLE = 8,
	CHAOS_MIX_WING1 = 11,
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_CHAOS_MIX_RATE_RECV
{
	PBMSG_HEAD header; // C1:88
	DWORD rate;
	DWORD money;
};

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_CHAOS_MIX_SEND
{
	PBMSG_HEAD header; // C1:86
	BYTE type;
};

struct PMSG_CHAOS_MIX_RATE_SEND
{
	PBMSG_HEAD header; // C1:88
	int type;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CChaosMix
{
public:

	CChaosMix();

	virtual ~CChaosMix();

	void Init();

	void Clear();

	void GCChaosMixRateRecv(PMSG_CHAOS_MIX_RATE_RECV* lpMsg);

private:

	static void ChangeChaosRate(char* Dest, char* Format);

	static void ChangeChaosMoney(char* Dest, char* Format);

	static void ChaosMixRateSend();

	static void CGChaosMixSend();

private:

	DWORD m_ChaosMixRate;

	char m_ChaosMixMoney[32];

	int m_LastChaosValue;
};

extern CChaosMix gChaosMix;