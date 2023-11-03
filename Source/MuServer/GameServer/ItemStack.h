#pragma once

#include "ProtocolDefines.h"

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_ITEM_STACK_LIST_SEND
{
	PSWMSG_HEAD header; // C2:F3:E3
	BYTE count;
};

struct ITEM_STACK
{
	int ItemIndex;
	int Level;
	int MaxStack;
};

//**********************************************//
//**********************************************//
//**********************************************//

struct ITEM_STACK_INFO
{
	int ItemIndex;
	int Level;
	int MaxStack;
	int CreateItemIndex;
};

class CItemStack
{
public:

	CItemStack();

	~CItemStack();

	void Load(char* path);

	int GetItemMaxStack(int index, int level);

	int GetCreateItemIndex(int index);

	void GCItemStackListSend(int aIndex);

private:

	std::vector<ITEM_STACK_INFO> m_ItemStackInfo;
};

extern CItemStack gItemStack;