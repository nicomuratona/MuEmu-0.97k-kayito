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

struct ITEM_VALUE_INFO
{
	int Index;
	int Level;
	int BuyValue;
	int SellValue;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CItemValue
{
public:

	CItemValue();

	virtual ~CItemValue();

	bool GetItemBuyValue(ITEM* lpItem, ULONGLONG* value);

	bool GetItemSellValue(ITEM* lpItem, ULONGLONG* value);

	void GCItemValueListRecv(PMSG_ITEM_VALUE_LIST_RECV* lpMsg);

private:

	std::vector<ITEM_VALUE_INFO> m_ItemValueInfo;
};

extern CItemValue gItemValue;