#include "stdafx.h"
#include "RightClickMove.h"
#include "ItemManager.h"
#include "ItemStack.h"

CRightClickMove gRightClickMove;

CRightClickMove::CRightClickMove()
{

}

CRightClickMove::~CRightClickMove()
{

}

void CRightClickMove::Init()
{
	SetCompleteHook(0xE9, 0x004D6020, &gItemManager.GetInterfaceEmptySlot);

	MemorySet(0x004D5C78, 0x90, 4); //from Inventory to Interface

	MemorySet(0x004D5CC5, 0x90, 4); //from Interface to Inventory

	SetCompleteHook(0xE8, 0x004D5C98, this->MoveItemToInterface);

	SetCompleteHook(0xE9, 0x004D5CA5, &this->CheckEquipItem);

	SetCompleteHook(0xE9, 0x004D1612, &this->CheckRighClickToPickItem);

	SetCompleteHook(0xE9, 0x004D16E9, &this->CheckUnequipItem);
}

bool CRightClickMove::MoveItemToInterface(int MousePosX, int MousePosY, DWORD InterfaceOffset, int InterfaceWidth, int InterfaceHeight)
{
	((unsigned int(__thiscall*)(DWORD a1, DWORD a2)) 0x0043D8A0)(0x055C9BC8, 0x07EAA11B);

	int trade_var = TradeOpened;

	((unsigned int(__thiscall*)(DWORD a1, DWORD a2)) 0x00404040)(0x055C9BC8, 0x07EAA11B);

	if (trade_var) // Trade
	{
		return gItemManager.GetInterfaceEmptySlot(*(DWORD*)0x07EA5290 + 0xF, *(DWORD*)0x07EA528C + 0x10E, OffsetTradeItems, 8, 4);
	}
	else if (ChaosMixOpened && MixState != 1) // Chaos Mix
	{
		return gItemManager.GetInterfaceEmptySlot(*(DWORD*)0x07EAA0C8 + 0xF, *(DWORD*)0x07EAA0CC + 0x6E, OffsetMixItems, 8, 4);
	}
	else if (WarehouseOpened) // Warehouse
	{
		return gItemManager.GetInterfaceEmptySlot(*(DWORD*)0x07EAA0C8 + 0xF, *(DWORD*)0x07EAA0CC + 0x32, OffsetWarehouseItems, 8, 15);
	}

	return false;
}

_declspec(naked) void CRightClickMove::CheckEquipItem()
{
	static DWORD ItemMovedToInterface = 0x004D5CB2;
	static DWORD ItemNotMoved = 0x004D5D56;
	static ITEM_ATTRIBUTE* ItemInfo;
	static BYTE ItemSlot;
	static int target;

	_asm
	{
		Mov Al, Byte Ptr Ds : [0x083A42EB] ;
		Test Al, Al;
		Jz Continue;
		Jmp[ItemMovedToInterface];
	Continue:
		Pushad;
	}

	if (pPickedItem)
	{
		if (pPickedItem->Part != 0xFF)
		{
			ItemSlot = pPickedItem->x + pPickedItem->y * 8 + INVENTORY_WEAR_SIZE;

			target = gItemManager.GetTargetSlotEquiped(pPickedItem, pPickedItem->Part);

			if (target != -1)
			{
				SetDword(0x07EA5B18, ItemSlot); // iSrcIndex

				((void(_cdecl*)(int SrcIndex, DWORD Interface, int MaxY))0x004CCE00)(ItemSlot, OffsetInventoryItems, 8); // DeleteItemSlot

				SetDword(0x07EAA160, 0);

				SendRequestEquipmentItem(0, ItemSlot, 0, target);

				PlayBuffer(29, NULL, FALSE);

				_asm
				{
					Popad;
					Jmp[ItemNotMoved];
				}
			}
		}
	}

	gItemStack.CheckItemStackClicked(pPickedItem);

	memset(pPickedItem, 0, sizeof(ITEM));

	_asm
	{
		Popad;
		Jmp[ItemNotMoved];
	}
}

_declspec(naked) void CRightClickMove::CheckRighClickToPickItem()
{
	static DWORD JmpOnOk = 0x004D1626;
	static DWORD JmpOnNot = 0x004D1D80;

	_asm
	{
		Pushad;
	}

	SetByte(0x07EAA164, 1);

	if (!MouseLButtonPush && MouseRButtonPush)
	{
		goto Exit;
	}

	if (MouseLButtonPush && !MouseRButtonPush)
	{
		goto Exit;
	}

	_asm
	{
		Popad;
		Jmp[JmpOnNot];
	}

Exit:

	_asm
	{
		Popad;
		Jmp[JmpOnOk];
	}
}

_declspec(naked) void CRightClickMove::CheckUnequipItem()
{
	static DWORD jmpOnOk = 0x004D16F2;
	static DWORD jmpOnNot = 0x004D1DAF;
	static DWORD itemSlot;
	static int target = -1;
	static ITEM_ATTRIBUTE* ItemInfo;

	_asm
	{
		Pushad;
		Mov itemSlot, Ebx;
	}

	if (pPickedItem && MouseRButtonPush)
	{
		MouseRButtonPush = false;

		ItemInfo = (ITEM_ATTRIBUTE*)(ItemAttribute + pPickedItem->Type * sizeof(ITEM_ATTRIBUTE));

		target = gItemManager.GetInventoryEmptySlot(ItemInfo->Width, ItemInfo->Height);

		SendRequestEquipmentItem(0, itemSlot, 0, target);
	}

	_asm
	{
		Popad;
		Cmp Ebx, 8;
		Jnz Exit;
		Jmp[jmpOnOk];
	Exit:
		Jmp[jmpOnNot];
	}
}