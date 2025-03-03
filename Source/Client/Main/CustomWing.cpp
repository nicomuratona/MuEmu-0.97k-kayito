#include "stdafx.h"
#include "CustomWing.h"

CCustomWing gCustomWing;

CCustomWing::CCustomWing()
{
	for (int n = 0; n < MAX_CUSTOM_WING; n++)
	{
		this->m_CustomWingInfo[n].Index = -1;
	}

	this->Init();
}

CCustomWing::~CCustomWing()
{

}

void CCustomWing::Init(CUSTOM_WING_INFO* info)
{
	for (int n = 0; n < MAX_CUSTOM_WING; n++)
	{
		this->SetInfo(info[n]);
	}
}

void CCustomWing::SetInfo(CUSTOM_WING_INFO info)
{
	if (info.Index < 0 || info.Index >= MAX_CUSTOM_WING)
	{
		return;
	}

	this->m_CustomWingInfo[info.Index] = info;
}

CUSTOM_WING_INFO* CCustomWing::GetInfo(int Index)
{
	if (Index < 0 || Index >= MAX_CUSTOM_WING)
	{
		return NULL;
	}

	if (this->m_CustomWingInfo[Index].Index != Index)
	{
		return NULL;
	}

	return &this->m_CustomWingInfo[Index];
}

CUSTOM_WING_INFO* CCustomWing::GetInfoByIndex(int ItemIndex)
{
	for (int n = 0; n < MAX_CUSTOM_WING; n++)
	{
		CUSTOM_WING_INFO* lpInfo = this->GetInfo(n);

		if (lpInfo == NULL)
		{
			continue;
		}

		if (lpInfo->ItemIndex == ItemIndex)
		{
			return lpInfo;
		}
	}

	return NULL;
}

int CCustomWing::GetCustomWingDefense(int ItemIndex, int ItemLevel)
{
	CUSTOM_WING_INFO* lpInfo = this->GetInfoByIndex(ItemIndex);

	if (lpInfo == NULL)
	{
		return 0;
	}

	return (lpInfo->DefenseConstA * ItemLevel);
}

int CCustomWing::GetCustomWingIncDamage(int ItemIndex, int ItemLevel)
{
	CUSTOM_WING_INFO* lpInfo = this->GetInfoByIndex(ItemIndex);

	if (lpInfo == NULL)
	{
		return 100;
	}

	return (lpInfo->IncDamageConstA + (ItemLevel * lpInfo->IncDamageConstB));
}

int CCustomWing::GetCustomWingDecDamage(int ItemIndex, int ItemLevel)
{
	CUSTOM_WING_INFO* lpInfo = this->GetInfoByIndex(ItemIndex);

	if (lpInfo == NULL)
	{
		return 100;
	}

	return (lpInfo->DecDamageConstA + (ItemLevel * lpInfo->DecDamageConstB));
}

void CCustomWing::Init()
{
	SetCompleteHook(0xE9, 0x00504B88, &this->CustomWingRenderPartObjectEffect_SetLevel);
	SetCompleteHook(0xE9, 0x005057C3, &this->CustomWingRenderPartObjectEffect_SetGlow);

	SetCompleteHook(0xE9, 0x004C4778, &this->CustomWingRenderItemInfo_SetTextColor);
	SetCompleteHook(0xE9, 0x004C557F, &this->CustomWingRenderItemInfo_SetName);
	SetCompleteHook(0xE9, 0x004C63D2, &this->CustomWingRenderItemInfo_SetIncDecDamage);
	SetCompleteHook(0xE9, 0x004C6808, &this->CustomWingRenderItemInfo_SetDurability);

	SetCompleteHook(0xE9, 0x004CA6B3, &this->CustomWingRenderItemName_SetName);

	SetCompleteHook(0xE9, 0x004C8E9A, &this->CustomWingRenderRepairInfo_SetColor);
	SetCompleteHook(0xE9, 0x004C94E0, &this->CustomWingRenderRepairInfo_SetName);

	SetCompleteHook(0xE9, 0x004AC494, &this->CustomWingCheckGate_SetMessage);

	SetCompleteHook(0xE9, 0x004CC572, &this->CustomWing_InventoryCheck);

	SetCompleteHook(0xE9, 0x004CE65E, &this->CustomWing_EquipmentCheck);

	SetCompleteHook(0xE9, 0x004D1659, &this->CustomWingInventoryEquipment_IcarusWingCount);
	SetCompleteHook(0xE9, 0x004D1682, &this->CustomWingInventoryEquipment_PickCheck);

	SetCompleteHook(0xE9, 0x004DE955, &this->CustomWingInventoryProcess_AllowInsertJewels);

	SetCompleteHook(0xE9, 0x004E4211, &this->CustomWingChaosMix_AddToPlusItemMix);

	SetCompleteHook(0xE9, 0x004E02E3, &this->CustomWingIsHighValueItem_CheckWing);

	SetCompleteHook(0xE9, 0x0045C976, &this->CustomWingChangeCharacterExt);
}

_declspec(naked) void CCustomWing::CustomWingRenderPartObjectEffect_SetLevel()
{
	static DWORD jmpOnOk = 0x00504BD8;
	static DWORD jmpOnNot = 0x00504B90;

	static int ItemIndex;

	_asm
	{
		Lea Edi, [Esi + Edx * 4];
		Pushad;
		Mov ItemIndex, Eax;
	}

	if (gCustomWing.GetInfoByIndex(ItemIndex - ITEM_BASE_MODEL) == NULL)
	{
		goto EXIT;
	}

	_asm
	{
		Popad;
		Jmp[jmpOnOk];
	}

EXIT:

	_asm
	{
		Popad;
		Cmp Eax, 0x363;
		Jmp[jmpOnNot];
	}
}

_declspec(naked) void CCustomWing::CustomWingRenderPartObjectEffect_SetGlow()
{
	static DWORD jmpOnOk = 0x00505885;
	static DWORD jmpOnNot = 0x005057CB;

	static int ItemIndex;

	_asm
	{
		Mov Ax, Word Ptr Ds : [Esi + 0x2] ;
		Pushad;
		Movsx Eax, Ax;
		Mov ItemIndex, Eax;
	}

	if (gCustomWing.GetInfoByIndex(ItemIndex - ITEM_BASE_MODEL) == NULL)
	{
		goto EXIT;
	}

	_asm
	{
		Popad;
		Jmp[jmpOnOk];
	}

EXIT:

	_asm
	{
		Popad;
		Cmp Ax, 0x310;
		Jmp[jmpOnNot];
	}
}

_declspec(naked) void CCustomWing::CustomWingRenderItemInfo_SetTextColor()
{
	static DWORD jmpOnOk = 0x004C4786;
	static DWORD jmpOnNot = 0x004C477D;

	static int ItemIndex;

	_asm
	{
		Pushad;
		Movsx Esi, Si;
		Mov ItemIndex, Esi;
	}

	if (gCustomWing.GetInfoByIndex(ItemIndex) == NULL)
	{
		goto EXIT;
	}

	_asm
	{
		Popad;
		Jmp[jmpOnOk];
	}

EXIT:

	_asm
	{
		Popad;
		Cmp Si, 0x183;
		Jmp[jmpOnNot];
	}
}

_declspec(naked) void CCustomWing::CustomWingRenderItemInfo_SetName()
{
	static DWORD jmpOnOk = 0x004C558D;
	static DWORD jmpOnNot = 0x004C5584;

	static int ItemIndex;

	_asm
	{
		Pushad;
		Movsx Ecx, Cx;
		Mov ItemIndex, Ecx;
	}

	if (gCustomWing.GetInfoByIndex(ItemIndex) == NULL)
	{
		goto EXIT;
	}

	_asm
	{
		Popad;
		Jmp[jmpOnOk];
	}

EXIT:

	_asm
	{
		Popad;
		Cmp Cx, 0x183;
		Jmp[jmpOnNot];
	}
}

_declspec(naked) void CCustomWing::CustomWingRenderItemInfo_SetIncDecDamage()
{
	static DWORD jmpOnOk = 0x004C67E2;
	static DWORD jmpOnNot = 0x004C63DA;

	static DWORD Item;

	_asm
	{
		Mov Ax, Word Ptr Ds : [Ebp + 0] ;
		Pushad;
		Mov Item, Ebp;
	}

	if (gCustomWing.GetInfoByIndex(((ITEM*)Item)->Type) == NULL)
	{
		goto EXIT;
	}

	sprintf_s(TextList[TextNum], GlobalText[577], gCustomWing.GetCustomWingIncDamage(((ITEM*)Item)->Type, GET_ITEM_OPT_LEVEL(((ITEM*)Item)->Level))); // Damage increases by a few percent.

	TextListColor[TextNum] = 0;

	TextBold[TextNum] = false;

	TextNum += 1;

	sprintf_s(TextList[TextNum], GlobalText[578], gCustomWing.GetCustomWingDecDamage(((ITEM*)Item)->Type, GET_ITEM_OPT_LEVEL(((ITEM*)Item)->Level))); // Absorbs a certain percentage of damage.

	TextListColor[TextNum] = 0;

	TextBold[TextNum] = false;

	TextNum += 1;

	sprintf_s(TextList[TextNum], GlobalText[579]); // Increased movement speed.

	TextListColor[TextNum] = 0;

	TextBold[TextNum] = false;

	TextNum += 1;

	_asm
	{
		Popad;
		Jmp[jmpOnOk];
	}

EXIT:

	_asm
	{
		Popad;
		Cmp Ax, 0x180;
		Jmp[jmpOnNot];
	}
}

_declspec(naked) void CCustomWing::CustomWingRenderItemInfo_SetDurability()
{
	static DWORD jmpOnOk = 0x004C6818;
	static DWORD jmpOnNot = 0x004C6810;

	static int ItemIndex;

	_asm
	{
		Mov Ax, Word Ptr Ds : [Ebp + 0] ;
		Pushad;
		Movsx Eax, Ax;
		Mov ItemIndex, Eax;
	}

	if (gCustomWing.GetInfoByIndex(ItemIndex) == NULL)
	{
		goto EXIT;
	}

	_asm
	{
		Popad;
		Jmp[jmpOnOk];
	}

EXIT:

	_asm
	{
		Popad;
		Cmp Ax, 0x180;
		Jmp[jmpOnNot];
	}
}

_declspec(naked) void CCustomWing::CustomWingRenderItemName_SetName()
{
	static DWORD jmpOnOk = 0x004CA6CA;
	static DWORD jmpOnNot = 0x004CA6B9;

	static int ItemIndex;

	_asm
	{
		Pushad;
		Movsx Eax, Ax;
		Mov ItemIndex, Eax;
	}

	if (gCustomWing.GetInfoByIndex(ItemIndex) != NULL)
	{
		goto EXIT;
	}

	// Original
	if (ItemIndex <= GET_ITEM_MODEL(12, 6))
	{
		goto EXIT;
	}

	_asm
	{
		Popad;
		Jmp[jmpOnNot];
	}

EXIT:

	_asm
	{
		Popad;
		Jmp[jmpOnOk];
	}
}

_declspec(naked) void CCustomWing::CustomWingRenderRepairInfo_SetColor()
{
	static DWORD jmpOnOk = 0x004C8EAC;
	static DWORD jmpOnNot = 0x004C8EA3;

	static int ItemIndex;

	_asm
	{
		Mov[Esp + 0x14], Ebp;
		Pushad;
		Movsx Ecx, Cx;
		Mov ItemIndex, Ecx;
	}

	if (gCustomWing.GetInfoByIndex(ItemIndex) == NULL)
	{
		goto EXIT;
	}

	_asm
	{
		Popad;
		Jmp[jmpOnOk];
	}

EXIT:

	_asm
	{
		Popad;
		Cmp Cx, 0x183;
		Jmp[jmpOnNot];
	}
}

_declspec(naked) void CCustomWing::CustomWingRenderRepairInfo_SetName()
{
	static DWORD jmpOnOk = 0x004C94EE;
	static DWORD jmpOnNot = 0x004C94E5;

	static int ItemIndex;

	_asm
	{
		Pushad;
		Movsx Ecx, Cx;
		Mov ItemIndex, Ecx;
	}

	if (gCustomWing.GetInfoByIndex(ItemIndex) == NULL)
	{
		goto EXIT;
	}

	_asm
	{
		Popad;
		Jmp[jmpOnOk];
	}

EXIT:

	_asm
	{
		Popad;
		Cmp Cx, 0x183;
		Jmp[jmpOnNot];
	}
}

_declspec(naked) void CCustomWing::CustomWingCheckGate_SetMessage()
{
	static DWORD jmpOnOk = 0x004AC508;
	static DWORD jmpOnNot = 0x004AC49B;

	static int ItemIndex;

	_asm
	{
		Mov Ax, Word Ptr Ds : [Ecx + 0x3F4] ;
		Pushad;
		Movsx Eax, Ax;
		Mov ItemIndex, Eax;
	}

	if (gCustomWing.GetInfoByIndex(ItemIndex) == NULL)
	{
		goto EXIT;
	}

	_asm
	{
		Popad;
		Jmp[jmpOnOk];
	}

EXIT:

	_asm
	{
		Popad;
		Jmp[jmpOnNot];
	}
}

_declspec(naked) void CCustomWing::CustomWing_InventoryCheck()
{
	static DWORD jmpOnOk = 0x004CC5A3;
	static DWORD jmpOnNot = 0x004CC577;

	static int ItemIndex;

	_asm
	{
		Pushad;
		Movsx Ecx, Cx;
		Mov ItemIndex, Ecx;
	}

	if (gCustomWing.GetInfoByIndex(ItemIndex) == NULL)
	{
		goto EXIT;
	}

	_asm
	{
		Popad;
		Jmp[jmpOnOk];
	}

EXIT:

	_asm
	{
		Popad;
		Cmp Cx, 0x180;
		Jmp[jmpOnNot];
	}
}

_declspec(naked) void CCustomWing::CustomWing_EquipmentCheck()
{
	static DWORD jmpOnOk = 0x004CE6E0;
	static DWORD jmpOnNot = 0x004CE665;

	static int ItemIndex;

	_asm
	{
		Mov Dx, Word Ptr Ds : [0x07E91350] ; // pPickedItem
		Pushad;
		Movsx Edx, Dx;
		Mov ItemIndex, Edx;
	}

	if (gCustomWing.GetInfoByIndex(ItemIndex) == NULL)
	{
		goto EXIT;
	}

	_asm
	{
		Popad;
		Jmp[jmpOnOk];
	}

EXIT:

	_asm
	{
		Popad;
		Jmp[jmpOnNot];
	}
}

_declspec(naked) void CCustomWing::CustomWingInventoryEquipment_IcarusWingCount()
{
	static DWORD jmpOnOk = 0x004D166C;
	static DWORD jmpOnNot = 0x004D1660;

	static int ItemIndex;

	_asm
	{
		Mov Ax, Word Ptr Ds : [Edi + 0x3F4] ;
		Pushad;
		Movsx Eax, Ax;
		Mov ItemIndex, Eax;
	}

	if (gCustomWing.GetInfoByIndex(ItemIndex) == NULL)
	{
		goto EXIT;
	}

	_asm
	{
		Popad;
		Jmp[jmpOnOk];
	}

EXIT:

	_asm
	{
		Popad;
		Jmp[jmpOnNot];
	}
}

_declspec(naked) void CCustomWing::CustomWingInventoryEquipment_PickCheck()
{
	static DWORD jmpOnOk = 0x004D1DAF;
	static DWORD jmpOnNot = 0x004D1687;

	static int ItemIndex;

	_asm
	{
		Pushad;
		Movsx Ecx, Cx;
		Mov ItemIndex, Ecx;
	}

	if (gCustomWing.GetInfoByIndex(ItemIndex) == NULL)
	{
		goto EXIT;
	}

	_asm
	{
		Popad;
		Jmp[jmpOnOk];
	}

EXIT:

	_asm
	{
		Popad;
		Cmp Cx, 0x180;
		Jmp[jmpOnNot];
	}
}

_declspec(naked) void CCustomWing::CustomWingInventoryProcess_AllowInsertJewels()
{
	static DWORD jmpOnOk = 0x004DE973;
	static DWORD jmpOnNot = 0x004DE965;

	static int ItemIndex;

	_asm
	{
		Movsx Esi, Word Ptr Ds : [Ebx + Eax * 4] ;
		Lea Ebx, [Ebx + Eax * 4];
		Mov Eax, [Ebx + 4];
		Sar Eax, 3;
		And Eax, 0xF;
		Pushad;
		Mov ItemIndex, Esi;
	}

	if (gCustomWing.GetInfoByIndex(ItemIndex) == NULL)
	{
		goto EXIT;
	}

	_asm
	{
		Popad;
		Jmp[jmpOnOk];
	}

EXIT:

	_asm
	{
		Popad;
		Jmp[jmpOnNot];
	}
}

_declspec(naked) void CCustomWing::CustomWingChaosMix_AddToPlusItemMix()
{
	static DWORD jmpOnOk = 0x004E4218;
	static DWORD jmpOnNot = 0x004E4244;

	static int ItemIndex;

	_asm
	{
		Pushad;
		Movsx Edx, Dx;
		Mov ItemIndex, Edx;
	}

	if (gCustomWing.GetInfoByIndex(ItemIndex) != NULL)
	{
		goto EXIT;
	}

	if (ItemIndex <= GET_ITEM(12, 6))
	{
		goto EXIT;
	}

	_asm
	{
		Popad;
		Jmp[jmpOnNot];
	}

EXIT:

	_asm
	{
		Popad;
		Jmp[jmpOnOk];
	}
}

_declspec(naked) void CCustomWing::CustomWingIsHighValueItem_CheckWing()
{
	static DWORD jmpOnOk = 0x004E0847;
	static DWORD jmpOnNot = 0x004E02E9;

	static int ItemIndex;

	_asm
	{
		Mov Ax, Word Ptr Ds : [0x07E91350] ; // pPickedItem
		Pushad;
		Movsx Eax, Ax;
		Mov ItemIndex, Eax;
	}

	if (gCustomWing.GetInfoByIndex(ItemIndex) == NULL)
	{
		goto EXIT;
	}

	_asm
	{
		Popad;
		Jmp[jmpOnOk];
	}

EXIT:

	_asm
	{
		Popad;
		Jmp[jmpOnNot];
	}
}

_declspec(naked) void CCustomWing::CustomWingChangeCharacterExt()
{
	static DWORD jmpBack = 0x0045C988;

	static BYTE* Equipment;
	static DWORD ViewportAddress;
	static CUSTOM_WING_INFO* lpInfo;

	_asm
	{
		Pushad;
		Mov Equipment, Edi;
		Mov ViewportAddress, Esi;
	}

	if ((Equipment[8] & 7) != 0)
	{
		lpInfo = gCustomWing.GetInfo((Equipment[8] & 7) - 1);

		if (lpInfo != NULL)
		{
			*(WORD*)(ViewportAddress + 0x2A0) = lpInfo->ItemIndex + ITEM_BASE_MODEL;
		}
		else
		{
			*(WORD*)(ViewportAddress + 0x2A0) = -1;

			*(BYTE*)(ViewportAddress + 0x2A3) = 0;
		}
	}
	else
	{
		*(WORD*)(ViewportAddress + 0x2A0) = ((Equipment[4] >> 2) & 3) + GET_ITEM_MODEL(12, 0);
	}

	_asm
	{
		Popad;
		Jmp[jmpBack];
	}
}