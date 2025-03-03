#include "stdafx.h"
#include "CustomBow.h"

CCustomBow gCustomBow;

CCustomBow::CCustomBow()
{
	for (int n = 0; n < MAX_CUSTOM_BOW; n++)
	{
		this->m_CustomBowInfo[n].ItemIndex = -1;
	}

	this->Init();
}

CCustomBow::~CCustomBow()
{

}

void CCustomBow::Init(CUSTOM_BOW_INFO* info)
{
	for (int n = 0; n < MAX_CUSTOM_BOW; n++)
	{
		this->SetInfo(info[n]);
	}
}

void CCustomBow::SetInfo(CUSTOM_BOW_INFO info)
{
	if (info.ItemIndex < 0 || info.ItemIndex >= MAX_CUSTOM_BOW)
	{
		return;
	}

	this->m_CustomBowInfo[info.ItemIndex] = info;
}

CUSTOM_BOW_INFO* CCustomBow::GetInfoByIndex(int ItemIndex)
{
	if (ItemIndex < 0 || (ItemIndex / MAX_ITEM_TYPE) != 4)
	{
		return NULL;
	}

	ItemIndex %= MAX_ITEM_TYPE;

	if (this->m_CustomBowInfo[ItemIndex].ItemIndex != ItemIndex)
	{
		return NULL;
	}

	return &this->m_CustomBowInfo[ItemIndex];
}

int CCustomBow::GetBowHandByItem(int ItemIndex)
{
	CUSTOM_BOW_INFO* info = this->GetInfoByIndex(ItemIndex);

	if (info != NULL)
	{
		if (info->ItemType == TYPE_BOW)
		{
			return EQUIPMENT_WEAPON_LEFT;
		}
		
		if (info->ItemType == TYPE_CROSSBOW)
		{
			return EQUIPMENT_WEAPON_RIGHT;
		}
	}

	return -1;
}

bool CCustomBow::IsCustomBow(int ItemIndex)
{
	CUSTOM_BOW_INFO* info = this->GetInfoByIndex(ItemIndex);

	if (info != NULL)
	{
		return (info->ItemType == TYPE_BOW);
	}

	return false;
}

bool CCustomBow::IsCustomCrossbow(int ItemIndex)
{
	CUSTOM_BOW_INFO* info = this->GetInfoByIndex(ItemIndex);

	if (info != NULL)
	{
		return (info->ItemType == TYPE_CROSSBOW);
	}

	return false;
}

int CCustomBow::GetSkillEffect(int ItemIndex)
{
	CUSTOM_BOW_INFO* lpInfo = this->GetInfoByIndex(ItemIndex);

	if (lpInfo == NULL)
	{
		return -1;
	}

	if (lpInfo->SkillShot >= 0 && lpInfo->SkillShot < 11)
	{
		int SkillEffects[11] = { 216, 217, 218, 219, 220, 221, 222, 223, 224, 243, 242 };

		return SkillEffects[lpInfo->SkillShot];
	}
	else
	{
		return -1;
	}
}

void CCustomBow::Init()
{
	SetCompleteHook(0xE9, 0x004431A6, &this->SetPlayerStop_Wings_Crossbow); // Flying/Swimming with Crossbow
	SetCompleteHook(0xE9, 0x004432B5, &this->SetPlayerStop_NoWings_Bow); // Standing with Bow
	SetCompleteHook(0xE9, 0x004432DC, &this->SetPlayerStop_NoWings_Crossbow); // Standing with Crossbow

	SetCompleteHook(0xE9, 0x00443B27, &this->SetPlayerWalk_Wings_Crossbow); // Flying with Crossbow
	SetCompleteHook(0xE9, 0x00443C41, &this->SetPlayerWalk_Walk_Bow); // Walking with Bow
	SetCompleteHook(0xE9, 0x00443C67, &this->SetPlayerWalk_Walk_Crossbow); // Walking with Crossbow
	SetCompleteHook(0xE9, 0x00443D3A, &this->SetPlayerWalk_Run_Bow); // Walking with Bow
	SetCompleteHook(0xE9, 0x00443D5D, &this->SetPlayerWalk_Run_Crossbow); // Walking with Crossbow

	SetCompleteHook(0xE9, 0x0042BC19, &this->SetPlayerBow_Bow); // Attack with Bow
	SetCompleteHook(0xE9, 0x0042BC65, &this->SetPlayerBow_Crossbow); // Attack with Crossbow

	SetCompleteHook(0xE9, 0x0044473D, &this->SetPlayetAttack_Bow); // Attack with Bow
	SetCompleteHook(0xE9, 0x00444785, &this->SetPlayetAttack_Crossbow); // Attack with Crossbow
	SetCompleteHook(0xE9, 0x004444A5, &this->SetPlayetAttack_Pet_Bow); // Attack with Bow and Pet
	SetCompleteHook(0xE9, 0x004444D4, &this->SetPlayetAttack_Pet_Crossbow); // Attack with Crossbow and Pet

	SetCompleteHook(0xE9, 0x00444934, &this->SetPlayetAttack_Sound_Bow); // Attack sound with Bow
	SetCompleteHook(0xE9, 0x00444964, &this->SetPlayetAttack_Sound_Crossbow); // Attack sound with Crossbow

	SetCompleteHook(0xE9, 0x0047D4A7, &this->CharacterStatsBow); // Render character stats with Bow
	SetCompleteHook(0xE9, 0x0047D4B5, &this->CharacterStatsCrossbow); // Render character stats with Crossbow

	SetCompleteHook(0xE9, 0x004826E1, &this->AssignArrowReloadBow); // Assign arrows to reload for Bow
	SetCompleteHook(0xE9, 0x004826FF, &this->AssignBoltReloadCrossbow); // Assign bolts to reload for Crossbow

	SetCompleteHook(0xE9, 0x0048B96E, &this->SendReloadArrows); // Send request to reload arrows for Bow
	SetCompleteHook(0xE9, 0x0048B9F7, &this->SendReloadBolts); // Send request to reload bolts for Crossbow

	SetCompleteHook(0xE9, 0x0048BCAA, &this->NoMoreArrowsMessage); // Show message when there are no more arrows to reload
	SetCompleteHook(0xE9, 0x0048BB67, &this->NoMoreBoltsMessage); // Show message when there are no more bolts to reload

	SetCompleteHook(0xE9, 0x0048D7B3, &this->SetBowAttackRange); // Set the attack range of the Bow
	SetCompleteHook(0xE9, 0x0048D793, &this->SetCrossbowAttackRange); // Set the attack range of the Crossbow

	SetCompleteHook(0xE9, 0x004AE74E, &this->UpdateAttackPositionBow); // Send current position when attacking with Bow
	SetCompleteHook(0xE9, 0x004AE72E, &this->UpdateAttackPositionCrossbow); // Send current position when attacking with Crossbow

	SetCompleteHook(0xE9, 0x004AEFDA, &this->AttackMovePositionBow); // Resolve the action of moving or attacking with Bow
	SetCompleteHook(0xE9, 0x004AEFBA, &this->AttackMovePositionCrossbow); // Resolve the action of moving or attacking with Crossbow

	SetCompleteHook(0xE9, 0x004E143D, &this->RenderPositionBow); // Position of rendering the Bow in the inventory
	SetCompleteHook(0xE9, 0x004E145E, &this->RenderPositionCrossbow); // Position of rendering Crossbow in the inventory

	SetCompleteHook(0xE9, 0x00458344, &this->CheckHandForBow); // Check the hand of the Bow
	SetCompleteHook(0xE9, 0x00458375, &this->CheckHandForCrossbow); // Check the hand of the Crossbow

	SetCompleteHook(0xE9, 0x00455683, &this->RenderBackPositionBow); // Position of rendering Bow on back
	SetCompleteHook(0xE9, 0x00455591, &this->RenderBackPositionCrossbow); // Position of rendering Crossbow on back

	SetCompleteHook(0xE9, 0x0049DA1D, &this->SkillPenetrationBow); // Enables the Penetration skill with Bow
	SetCompleteHook(0xE9, 0x0049DA2E, &this->SkillPenetrationCrossbow); // Enables the Penetration skill with Crossbow

	SetCompleteHook(0xE9, 0x0049F177, &this->SkillIceArrowBow); // Enables the Ice Arrow skill with Bow
	SetCompleteHook(0xE9, 0x0049F184, &this->SkillIceArrowCrossbow); // Enables the Ice Arrow skill with Crossbow

	SetCompleteHook(0xE9, 0x00474370, &this->CreateArrow); // Create Arrow function to add arrow effects
}

_declspec(naked) void CCustomBow::SetPlayerStop_Wings_Crossbow()
{
	static DWORD jmpOnOk = 0x004431AC;
	static DWORD jmpOnNot = 0x004431B4;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Ax;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(4, 18) // Original
	    || gCustomBow.IsCustomCrossbow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::SetPlayerStop_NoWings_Bow()
{
	static DWORD jmpOnOk = 0x004432BC;
	static DWORD jmpOnNot = 0x004432C4;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Cx;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(4, 17) // Original
	    || gCustomBow.IsCustomBow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::SetPlayerStop_NoWings_Crossbow()
{
	static DWORD jmpOnOk = 0x004432E2;
	static DWORD jmpOnNot = 0x004432EA;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Ax;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(4, 18) // Original
	    || gCustomBow.IsCustomCrossbow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::SetPlayerWalk_Wings_Crossbow()
{
	static DWORD jmpOnOk = 0x00443B2D;
	static DWORD jmpOnNot = 0x00443B34;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Ax;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(4, 18) || //Original
	    gCustomBow.IsCustomCrossbow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::SetPlayerWalk_Walk_Bow()
{
	static DWORD jmpOnOk = 0x00443C48;
	static DWORD jmpOnNot = 0x00443C4F;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Cx;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(4, 17) // Original
	    || gCustomBow.IsCustomBow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::SetPlayerWalk_Walk_Crossbow()
{
	static DWORD jmpOnOk = 0x00443C6D;
	static DWORD jmpOnNot = 0x00443C74;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Ax;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(4, 18) // Original
	    || gCustomBow.IsCustomCrossbow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::SetPlayerWalk_Run_Bow()
{
	static DWORD jmpOnOk = 0x00443D41;
	static DWORD jmpOnNot = 0x00443D45;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Cx;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(4, 17) // Original
	    || gCustomBow.IsCustomBow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::SetPlayerWalk_Run_Crossbow()
{
	static DWORD jmpOnOk = 0x00443D67;
	static DWORD jmpOnNot = 0x00443B91;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Ax;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(4, 18) // Original
	    || gCustomBow.IsCustomCrossbow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::SetPlayerBow_Bow()
{
	static DWORD jmpOnOk = 0x0042BC20;
	static DWORD jmpOnNot = 0x0042BC50;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Cx;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(4, 17) // Original
	    || gCustomBow.IsCustomBow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::SetPlayerBow_Crossbow()
{
	static DWORD jmpOnOk = 0x0042BC6C;
	static DWORD jmpOnNot = 0x0042BC9B;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Cx;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(4, 16) // Original
	    || ItemID == GET_ITEM_MODEL(4, 18) // Original
	    || gCustomBow.IsCustomCrossbow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::SetPlayetAttack_Bow()
{
	static DWORD jmpOnOk = 0x00444744;
	static DWORD jmpOnNot = 0x0044476D;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Cx;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(4, 17) // Original
	    || gCustomBow.IsCustomBow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::SetPlayetAttack_Crossbow()
{
	static DWORD jmpOnOk = 0x0044478F;
	static DWORD jmpOnNot = 0x00444536;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Ax;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(4, 18) // Original
	    || gCustomBow.IsCustomCrossbow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::SetPlayetAttack_Pet_Bow()
{
	static DWORD jmpOnOk = 0x004444AC;
	static DWORD jmpOnNot = 0x004444BC;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Cx;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(4, 17) // Original
	    || gCustomBow.IsCustomBow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::SetPlayetAttack_Pet_Crossbow()
{
	static DWORD jmpOnOk = 0x004444DA;
	static DWORD jmpOnNot = 0x004444EA;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Ax;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(4, 18) // Original
	    || gCustomBow.IsCustomCrossbow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::SetPlayetAttack_Sound_Bow()
{
	static DWORD jmpOnOk = 0x0044493B;
	static DWORD jmpOnNot = 0x00444945;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Cx;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(4, 17) // Original
	    || gCustomBow.IsCustomBow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::SetPlayetAttack_Sound_Crossbow()
{
	static DWORD jmpOnOk = 0x0044496A;
	static DWORD jmpOnNot = 0x00444974;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Ax;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(4, 18) // Original
	    || gCustomBow.IsCustomCrossbow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::CharacterStatsBow()
{
	static DWORD jmpOnOk = 0x0047D4AE;
	static DWORD jmpOnNot = 0x0047D4B5;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Dx;
		Pushad;
	}

	if (ItemID == GET_ITEM(4, 17) // Original
	    || gCustomBow.IsCustomBow(ItemID))
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

_declspec(naked) void CCustomBow::CharacterStatsCrossbow()
{
	static DWORD jmpOnOk = 0x0047D4C1;
	static DWORD jmpOnNot = 0x0047D50C;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Ax;
		Pushad;
	}

	if (ItemID == GET_ITEM(4, 16) // Original
	    || ItemID == GET_ITEM(4, 18) // Original
	    || gCustomBow.IsCustomCrossbow(ItemID))
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

_declspec(naked) void CCustomBow::AssignArrowReloadBow()
{
	static DWORD jmpOnOk = 0x004826E8;
	static DWORD jmpOnNot = 0x004826EF;
	static DWORD ItemID;

	_asm
	{
		Mov ItemID, Eax;
		Pushad;
	}

	if (ItemID == GET_ITEM(4, 17) // Original
	    || gCustomBow.IsCustomBow(ItemID))
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

_declspec(naked) void CCustomBow::AssignBoltReloadCrossbow()
{
	static DWORD jmpOnOk = 0x0048270F;
	static DWORD jmpOnNot = 0x00482716;
	static DWORD ItemID;

	_asm
	{
		Mov ItemID, Esi;
		Pushad;
	}

	if (ItemID == GET_ITEM(4, 16) // Original
	    || ItemID == GET_ITEM(4, 18) // Original
	    || gCustomBow.IsCustomCrossbow(ItemID))
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

_declspec(naked) void CCustomBow::SendReloadArrows()
{
	static DWORD jmpOnOk = 0x0048B975;
	static DWORD jmpOnNot = 0x0048B9E4;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Cx;
		Pushad;
	}

	if (ItemID == GET_ITEM(4, 17) // Original
	    || gCustomBow.IsCustomBow(ItemID))
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

_declspec(naked) void CCustomBow::SendReloadBolts()
{
	static DWORD jmpOnOk = 0x0048BA03;
	static DWORD jmpOnNot = 0x0048BA68;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Ax;
		Pushad;
	}

	if (ItemID == GET_ITEM(4, 16) // Original
	    || ItemID == GET_ITEM(4, 18) // Original
	    || gCustomBow.IsCustomCrossbow(ItemID))
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

_declspec(naked) void CCustomBow::NoMoreArrowsMessage()
{
	static DWORD jmpOnOk = 0x0048BCB2;
	static DWORD jmpOnNot = 0x0048BC38;
	static DWORD ItemID;

	_asm
	{
		Mov ItemID, Ebp;
		Pushad;
	}

	if (ItemID == GET_ITEM(4, 17) // Original
	    || gCustomBow.IsCustomBow(ItemID))
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

_declspec(naked) void CCustomBow::NoMoreBoltsMessage()
{
	static DWORD jmpOnOk = 0x0048BB73;
	static DWORD jmpOnNot = 0x0048BC9A;
	static DWORD ItemID;

	_asm
	{
		Mov ItemID, Edi;
		Pushad;
	}

	if (ItemID == GET_ITEM(4, 18) // Original
	    || gCustomBow.IsCustomCrossbow(ItemID))
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

_declspec(naked) void CCustomBow::SetBowAttackRange()
{
	static DWORD jmpOnOk = 0x0048D7BB;
	static DWORD jmpOnNot = 0x0048D7C3;
	static DWORD ItemID;

	_asm
	{
		Mov ItemID, Ebp;
		Pushad;
	}

	if (ItemID == GET_ITEM(4, 17) // Original
	    || gCustomBow.IsCustomBow(ItemID))
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

_declspec(naked) void CCustomBow::SetCrossbowAttackRange()
{
	static DWORD jmpOnOk = 0x0048D79B;
	static DWORD jmpOnNot = 0x0048D7A3;
	static DWORD ItemID;

	_asm
	{
		Mov ItemID, Edi;
		Pushad;
	}

	if (ItemID == GET_ITEM(4, 18) // Original
	    || gCustomBow.IsCustomCrossbow(ItemID))
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

_declspec(naked) void CCustomBow::UpdateAttackPositionBow()
{
	static DWORD jmpOnOk = 0x004AE75A;
	static DWORD jmpOnNot = 0x004AEB7A;
	static DWORD ItemID;

	_asm
	{
		Mov ItemID, Ebp;
		Pushad;
	}

	if (ItemID == GET_ITEM(4, 17) // Original
	    || gCustomBow.IsCustomBow(ItemID))
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

_declspec(naked) void CCustomBow::UpdateAttackPositionCrossbow()
{
	static DWORD jmpOnOk = 0x004AE75A;
	static DWORD jmpOnNot = 0x004AE73E;
	static DWORD ItemID;

	_asm
	{
		Mov ItemID, Edi;
		Pushad;
	}

	if (ItemID == GET_ITEM(4, 16) // Original
	    || ItemID == GET_ITEM(4, 18) // Original
	    || gCustomBow.IsCustomCrossbow(ItemID))
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

_declspec(naked) void CCustomBow::AttackMovePositionBow()
{
	static DWORD jmpOnOk = 0x004AEFFD;
	static DWORD jmpOnNot = 0x004AEFE1;
	static DWORD ItemID;

	_asm
	{
		Mov ItemID, Eax;
		Pushad;
	}

	if (ItemID == GET_ITEM(4, 17) // Original
	    || gCustomBow.IsCustomBow(ItemID))
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

_declspec(naked) void CCustomBow::AttackMovePositionCrossbow()
{
	static DWORD jmpOnOk = 0x004AEFFD;
	static DWORD jmpOnNot = 0x004AEFC8;
	static DWORD ItemID;

	_asm
	{
		Mov ItemID, Eax;
		Pushad;
	}

	if (ItemID == GET_ITEM(4, 16) // Original
	    || ItemID == GET_ITEM(4, 18) // Original
	    || gCustomBow.IsCustomCrossbow(ItemID))
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

_declspec(naked) void CCustomBow::RenderPositionBow()
{
	static DWORD jmpOnOk = 0x004E1445;
	static DWORD jmpOnNot = 0x004E145E;
	static DWORD ItemID;

	_asm
	{
		Mov ItemID, Esi;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(4, 17) // Original
	    || gCustomBow.IsCustomBow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::RenderPositionCrossbow()
{
	static DWORD jmpOnOk = 0x004E146E;
	static DWORD jmpOnNot = 0x004E1491;
	static DWORD ItemID;

	_asm
	{
		Mov ItemID, Esi;
		Pushad;
	}

	if ((ItemID >= GET_ITEM_MODEL(4, 8) && ItemID < GET_ITEM_MODEL(4, 15)) // Original
	    || ItemID == GET_ITEM_MODEL(4, 16) // Original
	    || ItemID == GET_ITEM_MODEL(4, 18) // Original
	    || gCustomBow.IsCustomCrossbow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::CheckHandForBow()
{
	static DWORD jmpOnOk = 0x0045834B;
	static DWORD jmpOnNot = 0x00458352;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Bx;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(4, 17) // Original
	    || gCustomBow.IsCustomBow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::CheckHandForCrossbow()
{
	static DWORD jmpOnOk = 0x0045837C;
	static DWORD jmpOnNot = 0x00458380;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Cx;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(4, 18) // Original
	    || gCustomBow.IsCustomCrossbow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::RenderBackPositionBow()
{
	static DWORD jmpOnOk = 0x00455666;
	static DWORD jmpOnNot = 0x0045568B;
	static DWORD ItemID;

	_asm
	{
		Mov ItemID, Ebx;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(5, 8) // Original
	    || gCustomBow.IsCustomBow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::RenderBackPositionCrossbow()
{
	static DWORD jmpOnOk = 0x00455599;
	static DWORD jmpOnNot = 0x004555DF;
	static DWORD ItemID;

	_asm
	{
		Mov ItemID, Ebx;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(4, 18) // Original
	    || gCustomBow.IsCustomCrossbow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::SkillPenetrationBow()
{
	static DWORD jmpOnOk = 0x0049DA39;
	static DWORD jmpOnNot = 0x0049DA23;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Ax;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(4, 17) // Original
	    || gCustomBow.IsCustomBow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::SkillPenetrationCrossbow()
{
	static DWORD jmpOnOk = 0x0049DA39;
	static DWORD jmpOnNot = 0x0049E6FE;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Cx;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(4, 16) // Original
	    || ItemID == GET_ITEM_MODEL(4, 18) // Original
	    || gCustomBow.IsCustomCrossbow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::SkillIceArrowBow()
{
	static DWORD jmpOnOk = 0x0049F18B;
	static DWORD jmpOnNot = 0x0049F17D;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Ax;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(4, 17) // Original
	    || gCustomBow.IsCustomBow(ItemID - ITEM_BASE_MODEL))
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

_declspec(naked) void CCustomBow::SkillIceArrowCrossbow()
{
	static DWORD jmpOnOk = 0x0049F18B;
	static DWORD jmpOnNot = 0x0049F195;
	static WORD ItemID;

	_asm
	{
		Mov ItemID, Cx;
		Pushad;
	}

	if (ItemID == GET_ITEM_MODEL(4, 16) // Original
	    || ItemID == GET_ITEM_MODEL(4, 18) // Original
	    || gCustomBow.IsCustomCrossbow(ItemID - ITEM_BASE_MODEL))
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

void CCustomBow::CreateArrow(DWORD c, DWORD o, DWORD to, BYTE SkillIndex, BYTE Skill, BYTE SKKey)
{
	int SubType = 0;

	int Right = *(short*)(c + 624);

	if (Right != -1)
	{
		Right -= ITEM_BASE_MODEL;
	}

	int Left = *(short*)(c + 648);

	if (Left != -1)
	{
		Left -= ITEM_BASE_MODEL;
	}

	if (c == Hero)
	{
		STRUCT_DECRYPT;

		Right = *(short*)(CharacterMachine + 536);

		Left = *(short*)(CharacterMachine + 604);

		STRUCT_ENCRYPT;
	}

	if (SKKey == 52)
	{
		SubType = 2;

		PlayBuffer(65, o, 0);
	}

	*(int*)0x05826D10 = SKKey;

	if (SKKey == 51)
	{
		PlayBuffer(101, o, 0);
	}

	if (gCustomBow.IsCustomBow(Left))
	{
		int Effect = gCustomBow.GetSkillEffect(Left);

		if (Effect != -1)
		{
			CreateEffect(Effect, (float*)(o + 16), (float*)(o + 28), (float*)(o + 232), SubType, o, *(short*)(o + 134), SkillIndex, Skill);
		}
	}
	else if (gCustomBow.IsCustomCrossbow(Right))
	{
		int Effect = gCustomBow.GetSkillEffect(Right);

		if (Effect != -1)
		{
			CreateEffect(Effect, (float*)(o + 16), (float*)(o + 28), (float*)(o + 232), SubType, o, *(short*)(o + 134), SkillIndex, Skill);
		}
	}
	else
	{
		switch (Right)
		{
			case GET_ITEM(4, 8):
			case GET_ITEM(4, 9):
			{
				CreateEffect(217, (float*)(o + 16), (float*)(o + 28), (float*)(o + 232), SubType, o, *(short*)(o + 134), SkillIndex, Skill);

				return;
			}

			case GET_ITEM(4, 10):
			{
				CreateEffect(221, (float*)(o + 16), (float*)(o + 28), (float*)(o + 232), SubType, o, *(short*)(o + 134), SkillIndex, Skill);

				return;
			}

			case GET_ITEM(4, 11):
			{
				CreateEffect(219, (float*)(o + 16), (float*)(o + 28), (float*)(o + 232), SubType, o, *(short*)(o + 134), SkillIndex, Skill);

				return;
			}

			case GET_ITEM(4, 12):
			{
				CreateEffect(218, (float*)(o + 16), (float*)(o + 28), (float*)(o + 232), SubType, o, *(short*)(o + 134), SkillIndex, Skill);

				return;
			}

			case GET_ITEM(4, 13):
			{
				CreateEffect(224, (float*)(o + 16), (float*)(o + 28), (float*)(o + 232), SubType, o, *(short*)(o + 134), SkillIndex, Skill);

				return;
			}

			case GET_ITEM(4, 14):
			{
				CreateEffect(223, (float*)(o + 16), (float*)(o + 28), (float*)(o + 232), SubType, o, *(short*)(o + 134), SkillIndex, Skill);

				return;
			}

			case GET_ITEM(4, 16):
			{
				CreateEffect(242, (float*)(o + 16), (float*)(o + 28), (float*)(o + 232), SubType, o, *(short*)(o + 134), SkillIndex, Skill);

				return;
			}

			case GET_ITEM(4, 18):
			{
				CreateEffect(242, (float*)(o + 16), (float*)(o + 28), (float*)(o + 232), SubType, o, *(short*)(o + 134), SkillIndex, Skill);

				return;
			}
		}

		switch (Left)
		{
			case GET_ITEM(4, 0):
			case GET_ITEM(4, 1):
			case GET_ITEM(4, 3):
			case GET_ITEM(4, 4):
			case GET_ITEM(4, 5):
			{
				CreateEffect(216, (float*)(o + 16), (float*)(o + 28), (float*)(o + 232), SubType, o, *(short*)(o + 134), SkillIndex, Skill);

				return;
			}

			case GET_ITEM(4, 2):
			{
				CreateEffect(220, (float*)(o + 16), (float*)(o + 28), (float*)(o + 232), SubType, o, *(short*)(o + 134), SkillIndex, Skill);

				return;
			}

			case GET_ITEM(4, 6):
			{
				CreateEffect(222, (float*)(o + 16), (float*)(o + 28), (float*)(o + 232), SubType, o, *(short*)(o + 134), SkillIndex, Skill);

				return;
			}

			case GET_ITEM(4, 17):
			{
				CreateEffect(243, (float*)(o + 16), (float*)(o + 28), (float*)(o + 232), SubType, o, *(short*)(o + 134), SkillIndex, Skill);

				return;
			}
		}
	}
}