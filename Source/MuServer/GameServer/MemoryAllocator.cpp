#include "stdafx.h"
#include "MemoryAllocator.h"

CMemoryAllocator gMemoryAllocator;

CMemoryAllocator::CMemoryAllocator()
{
	this->m_TempMemoryAllocatorInfo.m_Index = OBJECT_START_USER;
}

CMemoryAllocator::~CMemoryAllocator()
{

}

bool CMemoryAllocator::GetMemoryAllocatorFree(int* index, int MinIndex, int MaxIndex, DWORD MinTime)
{
	this->m_critical.lock();

	DWORD CurActiveTime = 0;

	DWORD MaxActiveTime = 0;

	for (int n = MinIndex; n < MaxIndex; n++)
	{
		if (OBJECT_RANGE(this->m_MemoryAllocatorInfo[n].m_Index) != 0)
		{
			if (this->m_MemoryAllocatorInfo[n].m_Active == 0 && (CurActiveTime = (GetTickCount() - this->m_MemoryAllocatorInfo[n].m_ActiveTime)) > MinTime && CurActiveTime > MaxActiveTime)
			{
				(*index) = this->m_MemoryAllocatorInfo[n].m_Index;

				MaxActiveTime = CurActiveTime;
			}
		}
	}

	this->m_critical.unlock();

	return (((*index) == -1) ? 0 : 1);
}

bool CMemoryAllocator::GetMemoryAllocatorInfo(CMemoryAllocatorInfo* lpMemoryAllocatorInfo, int index)
{
	this->m_critical.lock();

	if (OBJECT_RANGE(index) == 0)
	{
		this->m_critical.unlock();

		return 0;
	}

	if (OBJECT_RANGE(this->m_MemoryAllocatorInfo[index].m_Index) == 0)
	{
		this->m_critical.unlock();

		return 0;
	}

	(*lpMemoryAllocatorInfo) = this->m_MemoryAllocatorInfo[index];

	this->m_critical.unlock();

	return 1;
}

void CMemoryAllocator::InsertMemoryAllocatorInfo(CMemoryAllocatorInfo MemoryAllocatorInfo)
{
	this->m_critical.lock();

	if (OBJECT_RANGE(MemoryAllocatorInfo.m_Index) == 0)
	{
		this->m_critical.unlock();

		return;
	}

	this->m_MemoryAllocatorInfo[MemoryAllocatorInfo.m_Index] = MemoryAllocatorInfo;

	this->m_critical.unlock();
}

void CMemoryAllocator::RemoveMemoryAllocatorInfo(CMemoryAllocatorInfo MemoryAllocatorInfo)
{
	this->m_critical.lock();

	if (OBJECT_RANGE(MemoryAllocatorInfo.m_Index) == 0)
	{
		this->m_critical.unlock();

		return;
	}

	this->m_MemoryAllocatorInfo[MemoryAllocatorInfo.m_Index].Clear();

	this->m_critical.unlock();
}

void CMemoryAllocator::BindMemoryAllocatorInfo(int index, CMemoryAllocatorInfo MemoryAllocatorInfo)
{
	if (OBJECT_RANGE(index) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[index];

	lpObj->SkillBackup = MemoryAllocatorInfo.m_SkillBackup;

	lpObj->Skill = MemoryAllocatorInfo.m_Skill;

	lpObj->VpPlayer = MemoryAllocatorInfo.m_VpPlayer;

	lpObj->VpPlayer2 = MemoryAllocatorInfo.m_VpPlayer2;

	lpObj->VpPlayerItem = MemoryAllocatorInfo.m_VpPlayerItem;

	lpObj->HitDamage = MemoryAllocatorInfo.m_HitDamage;

	lpObj->Effect = MemoryAllocatorInfo.m_Effect;

	if (OBJECT_USER_RANGE(index) != 0)
	{
		lpObj->PerSocketContext = MemoryAllocatorInfo.m_PerSocketContext;

		lpObj->Inventory1 = MemoryAllocatorInfo.m_Inventory1;

		lpObj->Inventory2 = MemoryAllocatorInfo.m_Inventory2;

		lpObj->InventoryMap1 = MemoryAllocatorInfo.m_InventoryMap1;

		lpObj->InventoryMap2 = MemoryAllocatorInfo.m_InventoryMap2;

		lpObj->Trade = MemoryAllocatorInfo.m_Trade;

		lpObj->TradeMap = MemoryAllocatorInfo.m_TradeMap;

		lpObj->Warehouse = MemoryAllocatorInfo.m_Warehouse;

		lpObj->WarehouseMap = MemoryAllocatorInfo.m_WarehouseMap;

		lpObj->ChaosBox = MemoryAllocatorInfo.m_ChaosBox;

		lpObj->ChaosBoxMap = MemoryAllocatorInfo.m_ChaosBoxMap;

		lpObj->SkillDelay = MemoryAllocatorInfo.m_SkillDelay;

		lpObj->HackPacketDelay = MemoryAllocatorInfo.m_HackPacketDelay;

		lpObj->HackPacketCount = MemoryAllocatorInfo.m_HackPacketCount;
	}
	else
	{
		lpObj->Inventory1 = ((MemoryAllocatorInfo.m_Inventory1 == 0) ? this->m_TempMemoryAllocatorInfo.m_Inventory1 : MemoryAllocatorInfo.m_Inventory1);

		lpObj->Inventory2 = ((MemoryAllocatorInfo.m_Inventory2 == 0) ? this->m_TempMemoryAllocatorInfo.m_Inventory2 : MemoryAllocatorInfo.m_Inventory2);

		lpObj->InventoryMap1 = ((MemoryAllocatorInfo.m_InventoryMap1 == 0) ? this->m_TempMemoryAllocatorInfo.m_InventoryMap1 : MemoryAllocatorInfo.m_InventoryMap1);

		lpObj->InventoryMap2 = ((MemoryAllocatorInfo.m_InventoryMap2 == 0) ? this->m_TempMemoryAllocatorInfo.m_InventoryMap2 : MemoryAllocatorInfo.m_InventoryMap2);
	}
}