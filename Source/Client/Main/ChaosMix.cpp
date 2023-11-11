#include "stdafx.h"
#include "ChaosMix.h"
#include "Protocol.h"

CChaosMix gChaosMix;

CChaosMix::CChaosMix()
{
	this->Clear();
}

CChaosMix::~CChaosMix()
{

}

void CChaosMix::Init()
{
	SetCompleteHook(0xE9, 0x004DF476, &this->ChaosMixRateSend);

	SetCompleteHook(0xE9, 0x004DF483, &this->ChaosMixRateSend);

	SetCompleteHook(0xE8, 0x004F2D7A, &this->ChangeChaosRate);

	SetCompleteHook(0xE8, 0x004F2DB4, &this->ChangeChaosMoney);

	SetCompleteHook(0xE9, 0x004DF95E, &this->CGChaosMixSend);
}

void CChaosMix::Clear()
{
	this->m_ChaosMixRate = 0;

	wsprintf(this->m_ChaosMixMoney, "%d", 0);

	this->m_LastChaosValue = 0;
}

void CChaosMix::GCChaosMixRateRecv(PMSG_CHAOS_MIX_RATE_RECV* lpMsg)
{
	this->m_ChaosMixRate = lpMsg->rate;

	wsprintf(this->m_ChaosMixMoney, "%d", lpMsg->money);
}

void CChaosMix::ChangeChaosRate(char* Dest, char* Format)
{
	wsprintf(Dest, Format, gChaosMix.m_ChaosMixRate);
}

void CChaosMix::ChangeChaosMoney(char* Dest, char* Format)
{
	wsprintf(Dest, Format, gChaosMix.m_ChaosMixMoney);
}

_declspec(naked) void CChaosMix::ChaosMixRateSend()
{
	static DWORD jmpBack = 0x004DF48C;

	_asm
	{
		Mov Dword Ptr Ds : [0x07EAA16C] , Eax;
		Pushad;
	}

	if (gChaosMix.m_LastChaosValue != *(DWORD*)0x07EAA16C)
	{
		gChaosMix.m_ChaosMixRate = 0;

		wsprintf(gChaosMix.m_ChaosMixMoney, "%d", 0);

		gChaosMix.m_LastChaosValue = *(DWORD*)0x07EAA16C;

		if (gChaosMix.m_LastChaosValue == CHAOS_MIX_CHAOS_ITEM
		    || gChaosMix.m_LastChaosValue == CHAOS_MIX_DEVIL_SQUARE
		    || gChaosMix.m_LastChaosValue == CHAOS_MIX_PLUS_ITEM_LEVEL1
		    || gChaosMix.m_LastChaosValue == CHAOS_MIX_PLUS_ITEM_LEVEL2
		    || gChaosMix.m_LastChaosValue == CHAOS_MIX_DINORANT
		    || gChaosMix.m_LastChaosValue == CHAOS_MIX_FRUIT
		    || gChaosMix.m_LastChaosValue == CHAOS_MIX_WING1
		    || gChaosMix.m_LastChaosValue == CHAOS_MIX_BLOOD_CASTLE
		    || gChaosMix.m_LastChaosValue == CHAOS_MIX_WING2)
		{
			static PMSG_CHAOS_MIX_RATE_SEND pMsg;

			pMsg.header.set(0x88, sizeof(pMsg));

			pMsg.type = gChaosMix.m_LastChaosValue;

			gProtocol.DataSend((BYTE*)&pMsg, pMsg.header.size);
		}
	}

	_asm
	{
		Popad;
		Jmp jmpBack;
	}
}

_declspec(naked) void CChaosMix::CGChaosMixSend()
{
	static DWORD jmpBack = 0x004E137A;

	_asm
	{
		Pushad;
	}

	static PMSG_CHAOS_MIX_SEND pMsg;

	pMsg.header.set(0x86, sizeof(pMsg));

	pMsg.type = gChaosMix.m_LastChaosValue;

	gProtocol.DataSend((BYTE*)&pMsg, pMsg.header.size);

	_asm
	{
		Popad;
		Jmp jmpBack;
	}
}