#pragma once

#include "User.h"

class CMemoryAllocatorInfo
{
public:

	CMemoryAllocatorInfo();

	void Clear();

	void Alloc();

public:

	int m_Index;

	int m_Active;

	DWORD m_ActiveTime;

	CSkill* m_SkillBackup;

	CSkill* m_Skill;

	VIEWPORT_STRUCT* m_VpPlayer;

	VIEWPORT_STRUCT* m_VpPlayer2;

	VIEWPORT_STRUCT* m_VpPlayerItem;

	HIT_DAMAGE_STRUCT* m_HitDamage;

	CEffect* m_Effect;

	PER_SOCKET_CONTEXT* m_PerSocketContext;

	CItem* m_Inventory1;

	CItem* m_Inventory2;

	BYTE* m_InventoryMap1;

	BYTE* m_InventoryMap2;

	CItem* m_Trade;

	BYTE* m_TradeMap;

	CItem* m_Warehouse;

	BYTE* m_WarehouseMap;

	CItem* m_ChaosBox;

	BYTE* m_ChaosBoxMap;

	DWORD* m_SkillDelay;

	DWORD* m_HackPacketDelay;

	DWORD* m_HackPacketCount;
};