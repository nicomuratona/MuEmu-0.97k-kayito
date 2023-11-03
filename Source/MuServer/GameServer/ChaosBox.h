#pragma once

#include "ItemManager.h"
#include "ProtocolDefines.h"
#include "User.h"

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

struct PMSG_CHAOS_MIX_SEND
{
	PBMSG_HEAD header; // C1:86
	BYTE result;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

struct PMSG_CHAOS_MIX_RATE_SEND
{
	PBMSG_HEAD header; // C1:88
	DWORD rate;
	DWORD money;
};

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_CHAOS_MIX_RECV
{
	PBMSG_HEAD header; // C1:86
	BYTE type;
};

struct PMSG_CHAOS_MIX_RATE_RECV
{
	PBMSG_HEAD header; // C1:88
	int type;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CChaosBox
{
public:

	CChaosBox();

	~CChaosBox();

	void ChaosBoxInit(LPOBJ lpObj);

	void ChaosBoxItemDown(LPOBJ lpObj, int slot);

	void ChaosBoxItemKeep(LPOBJ lpObj, int slot);

	void GCChaosBoxSend(LPOBJ lpObj);

	void CGChaosMixCloseRecv(int aIndex);

	void CGChaosMixRecv(PMSG_CHAOS_MIX_RECV* lpMsg, int aIndex);

	void GCChaosMixSend(int aIndex, BYTE result, CItem* lpItem);

	void CGChaosMixRateRecv(PMSG_CHAOS_MIX_RATE_RECV* lpMsg, int aIndex);

	bool GetChaosMixRateAndMoney(int aIndex, int type, DWORD& rate, DWORD& money);

	void ChaosItemMix(LPOBJ lpObj);
	bool GetChaosItemMixRates(int aIndex, DWORD& rate, DWORD& money);

	void DevilSquareMix(LPOBJ lpObj);
	bool GetDevilSquareMixRates(int aIndex, DWORD& rate, DWORD& money, int& EyeLevel);

	void PlusItemLevelMix(LPOBJ lpObj, int type);
	bool GetPlusItemLevelMixRates(int aIndex, DWORD& rate, DWORD& money, int type, int& ItemSlot);

	void DinorantMix(LPOBJ lpObj);
	bool GetDinorantMixRates(int aIndex, DWORD& rate, DWORD& money);

	void FruitMix(LPOBJ lpObj);
	bool GetFruitMixRates(int aIndex, DWORD& rate, DWORD& money);

	void Wing1Mix(LPOBJ lpObj);
	bool GetWing1MixRates(int aIndex, DWORD& rate, DWORD& money);

	void BloodCastleMix(LPOBJ lpObj);
	bool GetBloodCastleMixRates(int aIndex, DWORD& rate, DWORD& money, int& ScrollLevel);

	void Wing2Mix(LPOBJ lpObj);
	bool GetWing2MixRates(int aIndex, DWORD& rate, DWORD& money);
};

extern CChaosBox gChaosBox;