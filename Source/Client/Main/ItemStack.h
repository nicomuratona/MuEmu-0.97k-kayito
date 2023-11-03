#pragma once

#include "ProtocolDefines.h"

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_ITEM_STACK_LIST_RECV
{
	PSWMSG_HEAD header; // C2:F3:E4
	BYTE count;
};

struct ITEM_STACK
{
	int ItemIndex;
	int Level;
	int MaxStack;
};

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_ITEM_UNSTACK_SEND
{
	PSBMSG_HEAD header; // C1:2B
	BYTE SourceSlot;
	BYTE Count;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CItemStack
{
	enum eStackClickType
	{
		STACK_CLICK_UNSTACK = 0,
		STACK_CLICK_STACK = 1,
	};

public:

	CItemStack();

	virtual ~CItemStack();

	int GetItemMaxStack(int index, int level);

	void GCItemStackListRecv(PMSG_ITEM_STACK_LIST_RECV* lpMsg);

	void Init();

	void CheckItemStackClicked(ITEM* pointedItem);

private:

	static void RenderItemNumber();

	static bool CanInsertItem(int MousePosX, int MousePosY, DWORD InterfaceOffset, int InterfaceWidth, int InterfaceHeight);

	void CGItemStackUnstackSend(BYTE Type, int Slot, int Count);

private:

	std::vector<ITEM_STACK> m_ItemStackInfo;
};

extern CItemStack gItemStack;