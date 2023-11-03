#pragma once

#include "Item.h"
#include "ProtocolDefines.h"

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_ITEM_VALUE_LIST_SEND
{
	PSWMSG_HEAD header; // C2:F3:E4
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

struct ITEM_VALUE_INFO
{
	int Index;
	int Level;
	int Grade;
	int Value;
};

class CItemValue
{
public:

	CItemValue();

	~CItemValue();

	void Load(char* path);

	bool GetItemValue(CItem* lpItem, int* value);

	void GCItemValueListSend(int aIndex);

private:

	std::vector<ITEM_VALUE_INFO> m_ItemValueInfo;
};

extern CItemValue gItemValue;