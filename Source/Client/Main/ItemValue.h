#pragma once

#include "ProtocolDefines.h"

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_ITEM_VALUE_LIST_RECV
{
	PSWMSG_HEAD header; // C2:F3:E5
	BYTE count;
};

struct ITEM_VALUE
{
	WORD Index;
	BYTE Level;
	BYTE Grade;
	DWORD Value;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CItemValue
{
public:

	CItemValue();

	virtual ~CItemValue();

	bool GetItemValue(ITEM* lpItem, int* value);

	void GCItemValueListRecv(PMSG_ITEM_VALUE_LIST_RECV* lpMsg);

private:

	std::vector<ITEM_VALUE> m_ItemValueInfo;
};

extern CItemValue gItemValue;