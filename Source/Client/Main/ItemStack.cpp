#include "stdafx.h"
#include "ItemStack.h"
#include "ItemManager.h"
#include "Protocol.h"

CItemStack gItemStack;

CItemStack::CItemStack()
{

}

CItemStack::~CItemStack()
{

}

int CItemStack::GetItemMaxStack(int index, int level)
{
	for (std::vector<ITEM_STACK>::iterator it = this->m_ItemStackInfo.begin(); it != this->m_ItemStackInfo.end(); it++)
	{
		if (it->ItemIndex != index)
		{
			continue;
		}

		if (it->Level != -1 && it->Level != level)
		{
			continue;
		}

		return it->MaxStack;
	}

	return 0;
}

void CItemStack::GCItemStackListRecv(PMSG_ITEM_STACK_LIST_RECV* lpMsg)
{
	this->m_ItemStackInfo.clear();

	for (int n = 0; n < lpMsg->count; n++)
	{
		ITEM_STACK* lpInfo = (ITEM_STACK*)(((BYTE*)lpMsg) + sizeof(PMSG_ITEM_STACK_LIST_RECV) + (sizeof(ITEM_STACK) * n));

		ITEM_STACK info;

		info.ItemIndex = lpInfo->ItemIndex;

		info.Level = lpInfo->Level;

		info.MaxStack = lpInfo->MaxStack;

		this->m_ItemStackInfo.push_back(info);
	}
}

void CItemStack::Init()
{
	SetCompleteHook(0xE9, 0x004E398C, &this->RenderItemNumber);

	SetCompleteHook(0xE9, 0x004D5D70, &this->CanInsertItem);
}

_declspec(naked) void CItemStack::RenderItemNumber()
{
	static DWORD jmpOnOk = 0x004E3998;
	static DWORD jmpOnNot = 0x004E39E0;
	static DWORD item;

	_asm
	{
		Pushad;
		Mov item, Esi;
	}

	if ((((ITEM*)item)->Type >= GET_ITEM(14, 0) && ((ITEM*)item)->Type <= GET_ITEM(14, 8))) // Apple, Healing, Mana, Siege, Antidote
	{
		goto Exit;
	}

	if (gItemStack.GetItemMaxStack(((ITEM*)item)->Type, GET_ITEM_OPT_LEVEL(((ITEM*)item)->Level)))
	{
		goto Exit;
	}

	_asm
	{
		Popad;
		Jmp[jmpOnNot];
	}

Exit:

	_asm
	{
		Popad;
		Jmp[jmpOnOk];
	}
}

bool CItemStack::CanInsertItem(int MousePosX, int MousePosY, DWORD InterfaceOffset, int InterfaceWidth, int InterfaceHeight)
{
	if (gItemManager.CheckPickedItemOverlay(MousePosX, MousePosY, InterfaceOffset, InterfaceWidth, InterfaceHeight))
	{
		return true;
	}
	else if (InterfaceOffset == OffsetInventoryItems)
	{
		ITEM_ATTRIBUTE* ItemInfo = (ITEM_ATTRIBUTE*)(ItemAttribute + pPickedItem->Type * sizeof(ITEM_ATTRIBUTE));

		int SlotX = (int)((MouseX - MousePosX) * 0.05f - ItemInfo->Width * 0.5f + 0.5f);

		int SlotY = (int)((MouseY - MousePosY) * 0.05f - ItemInfo->Height * 0.5f + 0.5f);

		int Slot = SlotY * InterfaceWidth + SlotX;

		ITEM* TargetItem = GetInventoryItem(Slot);

		if (pPickedItem->Type == TargetItem->Type)
		{
			return (gItemStack.GetItemMaxStack(pPickedItem->Type, GET_ITEM_OPT_LEVEL(pPickedItem->Level)) != 0);
		}
	}

	return false;
}

void CItemStack::CheckItemStackClicked(ITEM* pointedItem)
{
	if (!gItemStack.GetItemMaxStack(pointedItem->Type, GET_ITEM_OPT_LEVEL(pointedItem->Level)))
	{
		return;
	}

	((unsigned int(__thiscall*)(DWORD a1, DWORD a2)) 0x0043D8A0)(0x055C9BC8, 0x07EAA11B);

	int trade_var = TradeOpened;

	((unsigned int(__thiscall*)(DWORD a1, DWORD a2)) 0x00404040)(0x055C9BC8, 0x07EAA11B);

	if (trade_var // Trade
	    || ChaosMixOpened && MixState != 1 // Chaos Mix
	    || WarehouseOpened) // Warehouse
	{
		UIChatLogWindow_AddText("", GlobalText[474], 2);

		return;
	}

	BYTE Slot = pointedItem->x + pointedItem->y * 8 + INVENTORY_WEAR_SIZE;

	gItemStack.CGItemStackUnstackSend(STACK_CLICK_UNSTACK, Slot, 1);
}

void CItemStack::CGItemStackUnstackSend(BYTE Type, int Slot, int Count)
{
	PMSG_ITEM_UNSTACK_SEND pMsg = { 0 };

	pMsg.header.set(0x2B, Type, sizeof(pMsg));

	pMsg.SourceSlot = Slot;

	pMsg.Count = Count;

	gProtocol.DataSend((BYTE*)&pMsg, pMsg.header.size);
}