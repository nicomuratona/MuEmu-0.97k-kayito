#include "stdafx.h"
#include "MonsterManager.h"
#include "BloodCastle.h"
#include "BonusManager.h"
#include "DevilSquare.h"
#include "GoldenArcherBingo.h"
#include "InvasionManager.h"
#include "ItemManager.h"
#include "Map.h"
#include "ReadScript.h"
#include "Monster.h"
#include "MonsterSetBase.h"
#include "ObjectManager.h"
#include "ServerInfo.h"
#include "User.h"
#include "Util.h"

CMonsterManager gMonsterManager;

CMonsterManager::CMonsterManager()
{
	this->Init();
}

CMonsterManager::~CMonsterManager()
{

}

void CMonsterManager::Init()
{
	for (int n = 0; n < MAX_MONSTER_INFO; n++)
	{
		this->m_MonsterInfo[n].Index = -1;
	}

	for (int n = 0; n < MAX_MONSTER_LEVEL; n++)
	{
		memset(this->m_MonsterItemInfo[n].IndexTable, -1, sizeof(this->m_MonsterItemInfo[n].IndexTable));

		this->m_MonsterItemInfo[n].IndexCount = 0;
	}
}

void CMonsterManager::Load(char* path)
{
	CReadScript* lpReadScript = new CReadScript;

	if (lpReadScript == NULL)
	{
		ErrorMessageBox(READ_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (!lpReadScript->Load(path))
	{
		ErrorMessageBox(READ_SCRIPT_FILE_ERROR, path);

		delete lpReadScript;

		return;
	}

	this->Init();

	try
	{
		eTokenResult token;

		while (true)
		{
			token = lpReadScript->GetToken();

			if (token == TOKEN_END || token == TOKEN_END_SECTION)
			{
				break;
			}

			MONSTER_INFO info;

			memset(&info, 0, sizeof(info));

			info.Index = lpReadScript->GetNumber();

			info.Type = lpReadScript->GetAsNumber();

			strcpy_s(info.Name, lpReadScript->GetAsString());

			info.Level = lpReadScript->GetAsNumber();

			info.Life = lpReadScript->GetAsNumber();

			info.Mana = lpReadScript->GetAsNumber();

			info.DamageMin = lpReadScript->GetAsNumber();

			info.DamageMax = lpReadScript->GetAsNumber();

			info.Defense = lpReadScript->GetAsNumber();

			info.MagicDefense = lpReadScript->GetAsNumber();

			info.AttackRate = lpReadScript->GetAsNumber();

			info.DefenseRate = lpReadScript->GetAsNumber();

			info.MoveRange = lpReadScript->GetAsNumber();

			info.AttackType = lpReadScript->GetAsNumber();

			info.AttackRange = lpReadScript->GetAsNumber();

			info.ViewRange = lpReadScript->GetAsNumber();

			info.MoveSpeed = lpReadScript->GetAsNumber();

			info.AttackSpeed = lpReadScript->GetAsNumber();

			info.RegenTime = lpReadScript->GetAsNumber();

			info.Attribute = lpReadScript->GetAsNumber();

			info.ItemRate = lpReadScript->GetAsNumber();

			info.MoneyRate = lpReadScript->GetAsNumber();

			info.MaxItemLevel = lpReadScript->GetAsNumber();

			info.MonsterSkill = lpReadScript->GetAsNumber();

			for (int n = 0; n < MAX_RESISTANCE_TYPE; n++)
			{
				info.Resistance[n] = lpReadScript->GetAsNumber();
			}

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript->GetError());
	}

	delete lpReadScript;

	this->InitMonsterItem();
}

void CMonsterManager::SetInfo(MONSTER_INFO info)
{
	if (info.Index < 0 || info.Index >= MAX_MONSTER_INFO)
	{
		return;
	}

	info.Life = ((__int64)info.Life * gServerInfo.m_MonsterLifeRate) / 100;

	info.ScriptLife = info.Life;

	this->m_MonsterInfo[info.Index] = info;
}

MONSTER_INFO* CMonsterManager::GetInfo(int index)
{
	if (index < 0 || index >= MAX_MONSTER_INFO)
	{
		return 0;
	}

	if (this->m_MonsterInfo[index].Index != index)
	{
		return 0;
	}

	return &this->m_MonsterInfo[index];
}

void CMonsterManager::InitMonsterItem()
{
	for (int n = 0; n < MAX_MONSTER_LEVEL; n++)
	{
		this->InsertMonsterItem(n);
	}
}

void CMonsterManager::InsertMonsterItem(int level)
{
	for (int n = 0; n < MAX_ITEM; n++)
	{
		ITEM_INFO ItemInfo;

		if (gItemManager.GetInfo(n, &ItemInfo) == 0)
		{
			continue;
		}

		if (ItemInfo.Level < 0 || ItemInfo.DropItem == 0)
		{
			continue;
		}

		if ((ItemInfo.Level + 4) >= level && (ItemInfo.Level - 2) <= level)
		{
			MONSTER_ITEM_INFO* lpItem = &this->m_MonsterItemInfo[level];

			if (lpItem->IndexCount >= 0 && lpItem->IndexCount < MAX_MONSTER_ITEM)
			{
				lpItem->IndexTable[lpItem->IndexCount++] = n;
			}
		}
	}
}

long CMonsterManager::GetMonsterItem(int level, int excellent)
{
	if (level < 0 || level >= MAX_MONSTER_LEVEL)
	{
		return -1;
	}

	if (this->m_MonsterItemInfo[level].IndexCount == 0)
	{
		return -1;
	}

	int IndexTable[MAX_MONSTER_ITEM];

	int IndexCount = 0;

	for (int n = 0; n < this->m_MonsterItemInfo[level].IndexCount; n++)
	{
		if (excellent != 0 && this->m_MonsterItemInfo[level].IndexTable[n] >= GET_ITEM(12, 0))
		{
			continue;
		}

		IndexTable[IndexCount++] = this->m_MonsterItemInfo[level].IndexTable[n];
	}

	return ((IndexCount == 0) ? -1 : IndexTable[GetLargeRand() % IndexCount]);
}

void CMonsterManager::SetMonsterData()
{
	for (int n = 0; n < gMonsterSetBase.m_count; n++)
	{
		MONSTER_SET_BASE_INFO* lpInfo = &gMonsterSetBase.m_MonsterSetBaseInfo[n];

		if (lpInfo->Type == 3 || lpInfo->Type == 4)
		{
			continue;
		}

		int index = gObjAddMonster(lpInfo->Map);

		if (OBJECT_RANGE(index) == false)
		{
			continue;
		}

		if (gObjSetPosMonster(index, n) == 0)
		{
			gObjDel(index);

			continue;
		}

		if (gObjSetMonster(index, lpInfo->MonsterClass) == 0)
		{
			gObjDel(index);

			continue;
		}
	}

	gBloodCastle.Init();

	gBonusManager.Init();

	gDevilSquare.Init();

	gInvasionManager.Init();

	gGoldenArcherBingo.Init();
}

char* CMonsterManager::GetMonsterName(int index)
{
	if (index < 0 || index >= MAX_MONSTER_INFO)
	{
		return 0;
	}

	if (this->m_MonsterInfo[index].Index != index)
	{
		return 0;
	}

	return this->m_MonsterInfo[index].Name;
}