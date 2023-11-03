#include "stdafx.h"
#include "MemoryAllocatorInfo.h"
#include "EffectManager.h"
#include "HackPacketCheck.h"
#include "Quest.h"
#include "SkillManager.h"
#include "SocketManager.h"

CMemoryAllocatorInfo::CMemoryAllocatorInfo()
{
	this->Clear();
}

void CMemoryAllocatorInfo::Clear()
{
	this->m_Index = -1;

	this->m_Active = 0;

	this->m_ActiveTime = 0;

	this->m_SkillBackup = 0;

	this->m_Skill = 0;

	this->m_VpPlayer = 0;

	this->m_VpPlayer2 = 0;

	this->m_VpPlayerItem = 0;

	this->m_HitDamage = 0;

	this->m_Effect = 0;

	this->m_Inventory1 = 0;

	this->m_Inventory2 = 0;

	this->m_InventoryMap1 = 0;

	this->m_InventoryMap2 = 0;

	this->m_Trade = 0;

	this->m_TradeMap = 0;

	this->m_Warehouse = 0;

	this->m_WarehouseMap = 0;

	this->m_ChaosBox = 0;

	this->m_ChaosBoxMap = 0;

	this->m_SkillDelay = 0;

	this->m_HackPacketDelay = 0;

	this->m_HackPacketCount = 0;
}

void CMemoryAllocatorInfo::Alloc()
{
	this->m_SkillBackup = new CSkill[MAX_SKILL_LIST];

	this->m_Skill = new CSkill[MAX_SKILL_LIST];

	this->m_VpPlayer = new VIEWPORT_STRUCT[MAX_VIEWPORT];

	this->m_VpPlayer2 = new VIEWPORT_STRUCT[MAX_VIEWPORT];

	this->m_VpPlayerItem = new VIEWPORT_STRUCT[MAX_VIEWPORT];

	this->m_HitDamage = new HIT_DAMAGE_STRUCT[MAX_HIT_DAMAGE];

	this->m_Effect = new CEffect[MAX_EFFECT_LIST];

	if (OBJECT_USER_RANGE(this->m_Index) != 0)
	{
		this->m_PerSocketContext = new PER_SOCKET_CONTEXT;

		this->m_Inventory1 = new CItem[INVENTORY_SIZE];

		this->m_Inventory2 = new CItem[INVENTORY_SIZE];

		this->m_InventoryMap1 = new BYTE[INVENTORY_SIZE];

		this->m_InventoryMap2 = new BYTE[INVENTORY_SIZE];

		this->m_Trade = new CItem[TRADE_SIZE];

		this->m_TradeMap = new BYTE[TRADE_SIZE];

		this->m_Warehouse = new CItem[WAREHOUSE_SIZE];

		this->m_WarehouseMap = new BYTE[WAREHOUSE_SIZE];

		this->m_ChaosBox = new CItem[CHAOS_BOX_SIZE];

		this->m_ChaosBoxMap = new BYTE[CHAOS_BOX_SIZE];

		this->m_SkillDelay = new DWORD[MAX_SKILL];

		this->m_HackPacketDelay = new DWORD[MAX_HACK_PACKET_INFO];

		this->m_HackPacketCount = new DWORD[MAX_HACK_PACKET_INFO];
	}
}