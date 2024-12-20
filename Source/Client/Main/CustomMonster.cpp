#include "stdafx.h"
#include "CustomMonster.h"
#include "LoadModels.h"

CCustomMonster gCustomMonster;

CCustomMonster::CCustomMonster()
{
	for (int n = 0; n < MAX_MONSTER; n++)
	{
		this->m_CustomMonsterInfo[n].Index = -1;
	}

	this->Init();
}

CCustomMonster::~CCustomMonster()
{

}

void CCustomMonster::Load(CUSTOM_MONSTER_INFO* info)
{
	for (int n = 0; n < MAX_MONSTER; n++)
	{
		this->SetInfo(info[n]);
	}
}

void CCustomMonster::SetInfo(CUSTOM_MONSTER_INFO info)
{
	if (info.Index < 0 || info.Index >= MAX_MONSTER)
	{
		return;
	}

	this->m_CustomMonsterInfo[info.Index] = info;
}

CUSTOM_MONSTER_INFO* CCustomMonster::GetInfo(int index)
{
	if (index < 0 || index >= MAX_MONSTER)
	{
		return NULL;
	}

	if (this->m_CustomMonsterInfo[index].Index != index)
	{
		return NULL;
	}

	return &this->m_CustomMonsterInfo[index];
}

CUSTOM_MONSTER_INFO* CCustomMonster::GetInfoByIndex(int index)
{
	for (int n = 0; n < MAX_MONSTER; n++)
	{
		CUSTOM_MONSTER_INFO* lpInfo = this->GetInfo(n);

		if (lpInfo == NULL)
		{
			continue;
		}

		if (lpInfo->MonsterIndex == index)
		{
			return lpInfo;
		}
	}

	return NULL;
}

void CCustomMonster::Init()
{
	SetCompleteHook(0xE8, 0x00429CF6, &this->CreateCustomMonster);

	SetCompleteHook(0xE8, 0x0042A288, &this->CreateCustomMonster);

	SetCompleteHook(0xE8, 0x0042A592, &this->CreateCustomMonster);

	SetCompleteHook(0xE8, 0x0047D0F0, &this->CreateCustomMonster);

	SetCompleteHook(0xE8, 0x004C07C2, &this->CreateCustomMonster);

	SetCompleteHook(0xE9, 0x0044ADFC, &this->CustomMonsterDie);

	SetCompleteHook(0xE9, 0x00456AEB, &this->CustomMonsterGolden);

	SetCompleteHook(0xE9, 0x0048FBFA, &this->CustomNpcTalk);
}

void OpenMonsterModel(CUSTOM_MONSTER_INFO* lpInfo)
{
	int Index = MAX_MODELS + lpInfo->Index;

	DWORD b = gLoadModels.GetModels() + (188 * Index);

	if (*(BYTE*)0x0055A7C4 && (*(short*)(b + 0x26) > 0 || *(short*)(b + 0x24) > 0)) // if (b->NumActions > 0 && b->NumMeshs > 0)
	{
		return;
	}

	char path[MAX_PATH] = { 0 };

	wsprintf(path, "Data\\%s", lpInfo->FolderName);

	gLoadModels.MyAccessModel(Index, path, lpInfo->ModelName);

	if (*(short*)(b + 0x24) == 0) // if (b->NumMeshs == 0)
	{
		return;
	}

	gLoadModels.MyOpenTexture(Index, lpInfo->FolderName);

	*(float*)(*(DWORD*)(b + 0x30) + 4) = 0.25f; // b->Actions[MONSTER01_STOP1].PlaySpeed = 0.25f;

	*(float*)(*(DWORD*)(b + 0x30) + 20) = 0.2f; // b->Actions[MONSTER01_STOP2].PlaySpeed = 0.2f;

	*(float*)(*(DWORD*)(b + 0x30) + 36) = 0.34f; // b->Actions[MONSTER01_WALK].PlaySpeed = 0.34f;

	*(float*)(*(DWORD*)(b + 0x30) + 52) = 0.33f; // b->Actions[MONSTER01_ATTACK1].PlaySpeed = 0.33f;

	*(float*)(*(DWORD*)(b + 0x30) + 68) = 0.33f; // b->Actions[MONSTER01_ATTACK2].PlaySpeed = 0.33f;

	*(float*)(*(DWORD*)(b + 0x30) + 84) = 0.5f; // b->Actions[MONSTER01_SHOCK].PlaySpeed = 0.5f;

	*(float*)(*(DWORD*)(b + 0x30) + 100) = 0.55f; // b->Actions[MONSTER01_DIE].PlaySpeed = 0.55f;

	*(bool*)(*(DWORD*)(b + 0x30) + 96) = true; // b->Actions[MONSTER01_DIE].Loop = true;
}

void OpenNpcModel(CUSTOM_MONSTER_INFO* lpInfo)
{
	int Index = MAX_MODELS + lpInfo->Index;

	DWORD b = gLoadModels.GetModels() + (188 * Index);

	if (*(BYTE*)0x0055A7C4 && *(short*)(b + 0x26) > 0) // if (b->NumActions > 0)
	{
		return;
	}

	char path[MAX_PATH] = { 0 };

	wsprintf(path, "Data\\%s", lpInfo->FolderName);

	gLoadModels.MyAccessModel(Index, path, lpInfo->ModelName);

	for (int i = 0; i < *(short*)(b + 0x26); i++) // for (int i = 0; i < b->NumActions; i++)
	{
		*(float*)(*(DWORD*)(b + 0x30) + 4 + 16 * i) = 0.25f; // b->Actions[i].PlaySpeed = 0.25f;
	}

	if (*(short*)(b + 0x24) > 0) // if (b->NumMeshs > 0)
	{
		gLoadModels.MyOpenTexture(Index, lpInfo->FolderName);
	}
}

DWORD CCustomMonster::CreateCustomMonster(int Type, int PositionX, int PositionY, int Key)
{
	CUSTOM_MONSTER_INFO* lpInfo = gCustomMonster.GetInfoByIndex(Type);

	if (lpInfo != NULL) // Is Custom Monster
	{
		int Index = MAX_MODELS + lpInfo->Index;

		if (lpInfo->MonsterType != 0) // Is Monster
		{
			OpenMonsterModel(lpInfo);
		}
		else
		{
			OpenNpcModel(lpInfo);
		}

		DWORD c = CreateCharacter(Key, Index, PositionX, PositionY, 0.0f);

		*(float*)(c + 0xC) = lpInfo->Scale; // c->Object.Scale

		memcpy((char*)(c + 0x1C1), getMonsterName(Type), 24); // c->ID -> Max 24 characters

		*(BYTE*)(c + 0x2EB) = Type; // c->MonsterIndex

		*(short*)(c + 8) = 0; // c->Object.ExtState

		*(short*)(c + 0x310) = HeroIndex; // c->TargetCharacter

		*(BYTE*)(c + 0x84) = (lpInfo->MonsterType == 0) ? 4 : 2; // o->Kind -> 1 = Player / 2 = Monster / 4 = NPC / 8 = TRAP

		return c;
	}

	return CreateMonster(Type, PositionX, PositionY, Key);
}

_declspec(naked) void CCustomMonster::CustomMonsterDie()
{
	static DWORD jmpOnOk = 0x0044AE22;
	static DWORD jmpOnNot = 0x0044AE03;

	static int Index;

	_asm
	{
		Mov Ecx, Dword Ptr Ds : [Ebp + 0xC] ;
		Movsx Edx, Word Ptr Ds : [Ecx + 0x2] ;
		Pushad;
		Mov Ecx, Dword Ptr Ds : [Ebp + 0x8] ;
		Movzx Edx, Byte Ptr Ds : [Ecx + 0x2EB] ;
		Mov Index, Edx;
	}

	if (gCustomMonster.GetInfoByIndex(Index) == NULL)
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

_declspec(naked) void CCustomMonster::CustomMonsterGolden()
{
	static DWORD jmpOnOk = 0x00457277;
	static DWORD jmpOnNot = 0x00456AF1;

	static BYTE Index;
	static CUSTOM_MONSTER_INFO* lpInfo;

	_asm
	{
		Xor Eax, Eax;
		Mov Al, Byte Ptr Ds : [Ebx + 0x2EB] ;
		Mov Index, Al;
		Pushad;
	}

	lpInfo = gCustomMonster.GetInfoByIndex(Index);

	if (lpInfo && lpInfo->GoldenType)
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
		Mov Dword Ptr Ds : [Esp + 0x94] , 0x3F800000;
		Mov Esi, Dword Ptr Ds : [Esp + 0x94] ;
		Jmp[jmpOnOk];
	}
}

_declspec(naked) void CCustomMonster::CustomNpcTalk()
{
	static DWORD jmpOnOk = 0x0048FC0A;
	static DWORD jmpOnNot = 0x0048FC01;

	static BYTE Index;
	static CUSTOM_MONSTER_INFO* lpInfo;

	_asm
	{
		Mov Cl, Byte Ptr Ds : [Edx + Ecx * 4 + 0x2EB] ;
		Mov Index, Cl;
		Pushad;
	}

	lpInfo = gCustomMonster.GetInfoByIndex(Index);

	if (lpInfo && lpInfo->MonsterType == 0)
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