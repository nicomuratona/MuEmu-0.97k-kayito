#include "stdafx.h"
#include "WeaponView.h"

CWeaponView gWeaponView;

BYTE SecondWeaponFixVal = 1;

CWeaponView::CWeaponView()
{

}

CWeaponView::~CWeaponView()
{

}

void CWeaponView::Init()
{
	SetCompleteHook(0xE9, 0x00458325, &this->WeaponViewCheck);

	SetCompleteHook(0xE9, 0x004584C0, &this->SecondWeaponViewCheck);

	SetCompleteHook(0xE9, 0x004556B9, &this->SecondWeaponViewFix);
}

_declspec(naked) void CWeaponView::WeaponViewCheck()
{
	static DWORD jmpBack = 0x004583BB;

	_asm
	{
		Mov Byte Ptr Ss : [Esp + 0x98] , 0;
		Cmp Dword Ptr Ss : [Esp + 0x18] , 0;
		Je RightHand;
		Mov Byte Ptr Ss : [Esp + 0x98] , 1;
	RightHand:
		Cmp Eax, 0;
		Jnz LeftHand;
		Cmp Word Ptr Ds : [Edi + 0x270] , GET_ITEM_MODEL(7, 0);
		Jge NoDisplay;
		Cmp Word Ptr Ds : [Edi + 0x270] , GET_ITEM_MODEL(4, 7);
		Je Display;
		Jmp End;
	LeftHand:
		Cmp Word Ptr Ds : [Edi + 0x288] , GET_ITEM_MODEL(7, 0);
		Jge NoDisplay;
		Cmp Word Ptr Ds : [Edi + 0x288] , GET_ITEM_MODEL(4, 15);
		Je Display;
		Jmp End;
	Display:
		Mov Byte Ptr Ss : [Esp + 0x98] , 1;
		Jmp End;
	NoDisplay:
		Mov Byte Ptr Ss : [Esp + 0x98] , 0;
	End:
		Jmp jmpBack;
	}
}

_declspec(naked) void CWeaponView::SecondWeaponViewCheck()
{
	static DWORD jmpBack = 0x004584C5;

	_asm
	{
		Cmp SecondWeaponFixVal, 0;
		Jnz End;
		Mov Eax, 1;
		Mov SecondWeaponFixVal, 1;
		Jmp CWeaponView::WeaponViewCheck;
	End:
		Mov SecondWeaponFixVal, 0;
		Mov Eax, Dword Ptr Ds : [0x0055A7AC] ;
		Jmp jmpBack;
	}
}

_declspec(naked) void CWeaponView::SecondWeaponViewFix()
{
	static DWORD jmpBack = 0x004556BE;
	static DWORD Function[2] = { 0x004F9DB0, 0x004F9F70 };
	static BYTE IsShield = 0;
	static DWORD backEax;
	static DWORD backEcx;
	static DWORD backEdx;

	_asm
	{
		Mov backEax, Eax;
		Mov backEcx, Ecx;
		Mov backEdx, Edx;
		Mov IsShield, 0;
		Cmp Ebx, GET_ITEM_MODEL(6, 0);
		Jl FlipCheck;
		Cmp Ebx, GET_ITEM_MODEL(7, 0);
		Jge FlipCheck;
		Mov IsShield, 1;
		Cmp Ebx, GET_ITEM_MODEL(6, 16);
		Jnz CheckShield1;
		Mov Dword Ptr Ss : [Esp + 0x20] , 0x41F00000;
		Mov Dword Ptr Ss : [Esp + 0x24] , 0;
		Mov Dword Ptr Ss : [Esp + 0x28] , 0x42B40000;
		Lea Edx, Dword Ptr Ss : [Esp + 0x58] ;
		Lea Eax, Dword Ptr Ss : [Esp + 0x28] ;
		Push Edx;
		Push Eax;
		Call Function[0];
		Add Esp, 0x8;
		Mov Dword Ptr Ss : [Esp + 0x64] , 0xC1A00000;
		Mov Dword Ptr Ss : [Esp + 0x74] , 0;
		Mov Dword Ptr Ss : [Esp + 0x84] , 0xC1A00000;
		Jmp FlipCheck;
	CheckShield1:
		Cmp Ebx, GET_ITEM_MODEL(6, 15);
		Je IsShield1;
		Cmp Ebx, GET_ITEM_MODEL(6, 14);
		Jnz CheckShield2;
	IsShield1:
		Mov Dword Ptr Ss : [Esp + 0x20] , 0x42480000;
		Mov Dword Ptr Ss : [Esp + 0x24] , 0;
		Mov Dword Ptr Ss : [Esp + 0x28] , 0x42B40000;
		Lea Edx, Dword Ptr Ss : [Esp + 0x58] ;
		Lea Eax, Dword Ptr Ss : [Esp + 0x28] ;
		Push Edx;
		Push Eax;
		Call Function[0];
		Add Esp, 0x8;
		Mov Dword Ptr Ss : [Esp + 0x64] , 0xC1E00000;
		Mov Dword Ptr Ss : [Esp + 0x74] , 0;
		Mov Dword Ptr Ss : [Esp + 0x84] , 0xC1C80000;
		Jmp FlipCheck;
	CheckShield2:
		Cmp Ebx, GET_ITEM_MODEL(6, 6);
		Jnz IsShield2;
		Mov Dword Ptr Ss : [Esp + 0x20] , 0x41F00000;
		Mov Dword Ptr Ss : [Esp + 0x24] , 0;
		Mov Dword Ptr Ss : [Esp + 0x28] , 0x42B40000;
		Lea Edx, Dword Ptr Ss : [Esp + 0x58] ;
		Lea Eax, Dword Ptr Ss : [Esp + 0x28] ;
		Push Edx;
		Push Eax;
		Call Function[0];
		Add Esp, 0x8;
		Mov Dword Ptr Ss : [Esp + 0x64] , 0xC1700000;
		Mov Dword Ptr Ss : [Esp + 0x74] , 0;
		Mov Dword Ptr Ss : [Esp + 0x84] , 0xC1C80000;
		Jmp FlipCheck;
	IsShield2:
		Mov Dword Ptr Ss : [Esp + 0x64] , 0xC1200000;
		Mov Dword Ptr Ss : [Esp + 0x74] , 0;
		Mov Dword Ptr Ss : [Esp + 0x84] , 0;
	FlipCheck:
		Cmp IsShield, 1;
		Je End;
		Cmp SecondWeaponFixVal, 0;
		Je End;
		Sub Esp, 0x3C;
		Mov Dword Ptr Ss : [Esp + 0x30] , 0x43110000;
		Mov Dword Ptr Ss : [Esp + 0x34] , 0;
		Mov Dword Ptr Ss : [Esp + 0x38] , 0x43898000;
		Lea Eax, Dword Ptr Ss : [Esp] ;
		Lea Edx, Dword Ptr Ss : [Esp + 0x30] ;
		Push Eax;
		Push Edx;
		Call Function[0];
		Add Esp, 0x8;
		Mov Dword Ptr Ss : [Esp + 0x0C] , 0;
		Mov Dword Ptr Ss : [Esp + 0x1C] , 0x41200000;
		Mov Dword Ptr Ss : [Esp + 0x2C] , 0xC1F00000;
		Lea Edx, Dword Ptr Ss : [Esp + 0x94] ;
		Lea Eax, Dword Ptr Ss : [Esp] ;
		Push Edx;
		Push Eax;
		Push Edx;
		Call Function[4];
		Add Esp, 0x48;
	End:
		Mov Eax, backEax;
		Mov Ecx, backEcx;
		Mov Edx, backEdx;
		Push 0x06989C9C;
		Jmp jmpBack;
	}
}