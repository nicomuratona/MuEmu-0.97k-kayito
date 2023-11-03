#include "stdafx.h"
#include "Monster.h"
#include "Attack.h"
#include "BattleSoccerManager.h"
#include "BloodCastle.h"
#include "BonusManager.h"
#include "CriticalSection.h"
#include "DevilSquare.h"
#include "DSProtocol.h"
#include "EffectManager.h"
#include "ItemBagManager.h"
#include "ItemDrop.h"
#include "ItemOptionRate.h"
#include "Map.h"
#include "MapManager.h"
#include "MemoryAllocator.h"
#include "MonsterAIUtil.h"
#include "MonsterManager.h"
#include "MonsterSetBase.h"
#include "MonsterSkillManager.h"
#include "ObjectManager.h"
#include "Party.h"
#include "Protocol.h"
#include "Quest.h"
#include "QuestObjective.h"
#include "ServerInfo.h"
#include "ShopManager.h"
#include "SkillManager.h"
#include "Util.h"
#include "Viewport.h"

void gObjMonsterDieGiveItem(LPOBJ lpObj, LPOBJ lpTarget)
{
	int aIndex = gObjMonsterGetTopHitDamageUser(lpObj);

	if (OBJECT_RANGE(aIndex) != false)
	{
		lpTarget = &gObj[aIndex];
	}

	if (OBJECT_RANGE(lpObj->SummonIndex) != false)
	{
		return;
	}

	if (lpObj->Class == 132 || lpObj->Class == 133 || lpObj->Class == 134 || lpObj->Class == 247 || lpObj->Class == 249)
	{
		return;
	}

	if (gItemBagManager.DropItemByMonsterClass(lpObj->Class, lpTarget, lpObj->Map, lpObj->X, lpObj->Y) != 0)
	{
		return;
	}

	if (gQuestObjective.MonsterItemDrop(lpObj) != 0)
	{
		return;
	}

	if (gItemDrop.DropItem(lpObj, lpTarget) != 0)
	{
		return;
	}

	int DropIndex = -1;

	int DropLevel = lpObj->Level;

	int ExcItemDrop = 0;

	int ItemDropRate = gServerInfo.m_ItemDropRate[lpTarget->AccountLevel];

	ItemDropRate = (ItemDropRate * (lpTarget->ItemDropRate + lpTarget->EffectOption.AddItemDropRate)) / 100;

	ItemDropRate = (ItemDropRate * gMapManager.GetMapItemDropRate(lpObj->Map)) / 100;

	ItemDropRate = (ItemDropRate * gBonusManager.GetBonusValue(lpObj, BONUS_INDEX_ITEM_DROP_RATE, 100, -1, -1, lpObj->Class, lpObj->Level)) / 100;

	int ExcItemDropRate = gMapManager.GetMapExcItemDropRate(lpObj->Map);

	ExcItemDropRate = gBonusManager.GetBonusValue(lpTarget, BONUS_INDEX_EXC_ITEM_DROP_RATE, ExcItemDropRate, -1, -1, lpObj->Class, lpObj->Level);

	if (lpObj->Level >= 25 && (GetLargeRand() % 1000000) < ExcItemDropRate)
	{
		DropLevel -= 25;

		ExcItemDrop = 1;
	}

	if ((DropIndex = gMonsterManager.GetMonsterItem(DropLevel, ExcItemDrop)) >= GET_ITEM(0, 0) && (GetLargeRand() % ((lpObj->ItemRate == 0) ? 1 : lpObj->ItemRate)) < ItemDropRate)
	{
		WORD ItemIndex = DropIndex;

		BYTE ItemLevel = 0;

		BYTE ItemSkillOption = 0;

		BYTE ItemLuckOption = 0;

		BYTE ItemAddOption = 0;

		BYTE ItemExceOption = 0;

		gItemOptionRate.GetItemLevelOption(ExcItemDrop, &ItemLevel);

		gItemOptionRate.GetItemSkillOption(ExcItemDrop, &ItemSkillOption);

		gItemOptionRate.GetItemLuckOption(ExcItemDrop, &ItemLuckOption);

		gItemOptionRate.GetItemAddOption(ExcItemDrop, &ItemAddOption);

		gItemOptionRate.GetItemExceOption(ExcItemDrop, &ItemExceOption);

		ItemLevel = ((ItemIndex >= GET_ITEM(12, 0)) ? 0 : ItemLevel);

		ItemSkillOption = ((ItemIndex >= GET_ITEM(12, 0)) ? 0 : ItemSkillOption);

		ItemLuckOption = ((ItemIndex >= GET_ITEM(12, 0)) ? 0 : ItemLuckOption);

		ItemExceOption = ((ItemIndex >= GET_ITEM(12, 0)) ? 0 : ItemExceOption);

		gItemOptionRate.MakeExceOption(ItemIndex, ItemExceOption, &ItemExceOption);

		GDCreateItemSend(lpTarget->Index, lpObj->Map, (BYTE)lpObj->X, (BYTE)lpObj->Y, ItemIndex, ItemLevel, 0, ItemSkillOption, ItemLuckOption, ItemAddOption, lpTarget->Index, ItemExceOption);
	}
	else if (lpObj->Money > 0 && (GetLargeRand() % ((lpObj->MoneyRate == 0) ? 1 : lpObj->MoneyRate)) < 10)
	{
		int money = ((int)lpObj->Money * gServerInfo.m_MoneyAmountDropRate[lpTarget->AccountLevel]) / 100;

		money = (money * lpTarget->MoneyAmountDropRate) / 100;

		if (money > 0)
		{
			if ((money = (GetLargeRand() % money)) > 0)
			{
				gMap[lpObj->Map].MoneyItemDrop(money, lpObj->X, lpObj->Y);
			}
		}
	}
}

bool gObjSetPosMonster(int aIndex, int number)
{
	if (OBJECT_RANGE(aIndex) == false)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (number < 0 || number > MAX_MSB_MONSTER)
	{
		return 0;
	}

	MONSTER_SET_BASE_INFO* lpInfo = &gMonsterSetBase.m_MonsterSetBaseInfo[number];

	short x, y;

	if (gMonsterSetBase.GetPosition(number, lpInfo->Map, &x, &y) == 0)
	{
		return 0;
	}

	lpObj->PosNum = number;

	lpObj->X = x;

	lpObj->Y = y;

	lpObj->TX = lpObj->X;

	lpObj->TY = lpObj->Y;

	lpObj->OldX = lpObj->X;

	lpObj->OldY = lpObj->Y;

	lpObj->StartX = (BYTE)lpObj->X;

	lpObj->StartY = (BYTE)lpObj->Y;

	lpObj->Dir = lpInfo->Dir;

	lpObj->Map = lpInfo->Map;

	return 1;
}

bool gObjSetMonster(int aIndex, int MonsterClass)
{
	if (OBJECT_RANGE(aIndex) == false)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	lpObj->ConnectTickCount = GetTickCount();

	lpObj->ShopNumber = (short)gShopManager.GetShopNumber(MonsterClass);

	lpObj->TargetNumber = -1;

	lpObj->SummonIndex = -1;

	lpObj->LastAttackerID = -1;

	lpObj->Connected = OBJECT_ONLINE;

	lpObj->Live = 1;

	lpObj->State = OBJECT_CREATE;

	lpObj->DieRegen = 0;

	lpObj->Type = OBJECT_MONSTER;

	lpObj->Class = MonsterClass;

	switch (MonsterClass)
	{
		case 200:
		{
			gBattleSoccer.gBSGround[0]->m_BallIndex = aIndex;

			break;
		}
	}

	if ((MonsterClass >= 230 && MonsterClass <= 255) || lpObj->ShopNumber != -1)
	{
		lpObj->Type = OBJECT_NPC;
	}

	MONSTER_INFO* lpInfo = gMonsterManager.GetInfo(MonsterClass);

	if (lpInfo == 0)
	{
		return 0;
	}

	memcpy(lpObj->Name, lpInfo->Name, sizeof(lpObj->Name));

	lpObj->Level = lpInfo->Level;
	lpObj->PhysiSpeed = lpInfo->AttackSpeed;
	lpObj->PhysiDamageMin = lpInfo->DamageMin;
	lpObj->PhysiDamageMax = lpInfo->DamageMax;
	lpObj->Defense = lpInfo->Defense;
	lpObj->MagicDefense = lpInfo->MagicDefense;
	lpObj->AttackSuccessRate = lpInfo->AttackRate;
	lpObj->DefenseSuccessRate = lpInfo->DefenseRate;
	lpObj->Life = (float)lpInfo->Life;
	lpObj->MaxLife = (float)lpInfo->Life;
	lpObj->Mana = (float)lpInfo->Mana;
	lpObj->MaxMana = (float)lpInfo->Mana;
	lpObj->MoveRange = lpInfo->MoveRange;
	lpObj->MoveSpeed = lpInfo->MoveSpeed;
	lpObj->MaxRegenTime = lpInfo->RegenTime * 1000;
	lpObj->AttackRange = lpInfo->AttackRange;
	lpObj->ViewRange = lpInfo->ViewRange;
	lpObj->Attribute = lpInfo->Attribute;
	lpObj->AttackType = lpInfo->AttackType;
	lpObj->ItemRate = lpInfo->ItemRate;
	lpObj->MoneyRate = lpInfo->MoneyRate;
	lpObj->Resistance[0] = ((lpInfo->Resistance[0] > 255) ? 255 : lpInfo->Resistance[0]);
	lpObj->Resistance[1] = ((lpInfo->Resistance[0] > 255) ? 255 : lpInfo->Resistance[1]);
	lpObj->Resistance[2] = ((lpInfo->Resistance[0] > 255) ? 255 : lpInfo->Resistance[2]);
	lpObj->Resistance[3] = ((lpInfo->Resistance[0] > 255) ? 255 : lpInfo->Resistance[3]);
	lpObj->ScriptMaxLife = (float)lpInfo->ScriptLife;
	lpObj->BasicAI = lpInfo->AINumber;
	lpObj->CurrentAI = lpInfo->AINumber;
	lpObj->CurrentAIState = 0;
	lpObj->LastAIRunTime = 0;
	lpObj->GroupNumber = 0;
	lpObj->SubGroupNumber = 0;
	lpObj->GroupMemberGuid = -1;
	lpObj->RegenType = 0;

	lpObj->Agro.ResetAll();

	gObjSetInventory1Pointer(lpObj);

	if (lpObj->AttackType != 0)
	{
		if (lpObj->AttackType == 150)
		{
			gSkillManager.AddSkill(lpObj, SKILL_MONSTER_AREA_ATTACK, 0);
		}
		else
		{
			gSkillManager.AddSkill(lpObj, lpObj->AttackType, 0);
		}

		if (MonsterClass == 66 || MonsterClass == 73) // Cursed King,Drakan
		{
			gSkillManager.AddSkill(lpObj, SKILL_ENERGY_BALL, 0);
		}

		if (MonsterClass == 77) // Phoenix of Darkness
		{
			gSkillManager.AddSkill(lpObj, SKILL_LIGHTNING, 0);
		}

		if (MonsterClass == 89 || MonsterClass == 95 || MonsterClass == 112 || MonsterClass == 118 || MonsterClass == 124 || MonsterClass == 130) // Spirit Sorcerer
		{
			gSkillManager.AddSkill(lpObj, SKILL_LIGHTNING, 0);
		}
	}

	gMap[lpObj->Map].SetStandAttr(lpObj->X, lpObj->Y);

	lpObj->OldX = lpObj->X;

	lpObj->OldY = lpObj->Y;

	return 1;
}

bool gObjBallRegen(LPOBJ lpObj)
{
	gObjViewportListProtocolDestroy(lpObj);

	lpObj->TargetNumber = -1;

	lpObj->LastAttackerID = -1;

	lpObj->NextActionTime = 5000;

	lpObj->ActionState.Rest = 0;

	lpObj->ActionState.Attack = 0;

	lpObj->ActionState.Move = 0;

	lpObj->ActionState.Escape = 0;

	lpObj->ActionState.Emotion = 0;

	lpObj->ActionState.EmotionCount = 0;

	if (lpObj->Attribute == 62)
	{
		int px = lpObj->X;

		int py = lpObj->Y;

		if (gObjGetRandomFreeLocation(lpObj->Map, &px, &py, 5, 5, 30) == 0)
		{
			lpObj->Live = 0;

			lpObj->State = OBJECT_DYING;

			lpObj->DieRegen = 1;

			lpObj->RegenTime = GetTickCount();

			return 0;
		}

		lpObj->X = px;

		lpObj->Y = py;
	}
	else
	{
		if (gMonsterSetBase.GetPosition(lpObj->PosNum, lpObj->Map, &lpObj->X, &lpObj->Y) == 0)
		{
			lpObj->Live = 0;

			lpObj->State = OBJECT_DYING;

			lpObj->DieRegen = 1;

			lpObj->RegenTime = GetTickCount();

			return 0;
		}
	}

	lpObj->MTX = lpObj->X;

	lpObj->MTY = lpObj->Y;

	lpObj->TX = lpObj->X;

	lpObj->TY = lpObj->Y;

	lpObj->StartX = (BYTE)lpObj->X;

	lpObj->StartY = (BYTE)lpObj->Y;

	lpObj->PathCur = 0;

	lpObj->PathCount = 0;

	lpObj->PathStartEnd = 0;

	gObjViewportListProtocolCreate(lpObj);

	gObjClearViewport(lpObj);

	gObjMonsterInitHitDamage(lpObj);

	return 1;
}

bool gObjMonsterRegen(LPOBJ lpObj)
{
	gObjClearViewport(lpObj);

	lpObj->TargetNumber = -1;

	lpObj->LastAttackerID = -1;

	lpObj->NextActionTime = 5000;

	lpObj->ActionState.Rest = 0;

	lpObj->ActionState.Attack = 0;

	lpObj->ActionState.Move = 0;

	lpObj->ActionState.Escape = 0;

	lpObj->ActionState.Emotion = 0;

	lpObj->ActionState.EmotionCount = 0;

	if (lpObj->Attribute == 62)
	{
		int px = lpObj->X;

		int py = lpObj->Y;

		if (gObjGetRandomFreeLocation(lpObj->Map, &px, &py, 5, 5, 30) == 0)
		{
			lpObj->Live = 0;

			lpObj->State = OBJECT_DYING;

			lpObj->DieRegen = 1;

			lpObj->RegenTime = GetTickCount();

			return 0;
		}

		lpObj->X = px;

		lpObj->Y = py;
	}
	else
	{
		if (gMonsterSetBase.GetPosition(lpObj->PosNum, lpObj->Map, &lpObj->X, &lpObj->Y) == 0)
		{
			lpObj->Live = 0;

			lpObj->State = OBJECT_DYING;

			lpObj->DieRegen = 1;

			lpObj->RegenTime = GetTickCount();

			return 0;
		}
	}

	lpObj->MTX = lpObj->X;

	lpObj->MTY = lpObj->Y;

	lpObj->TX = lpObj->X;

	lpObj->TY = lpObj->Y;

	lpObj->StartX = (BYTE)lpObj->X;

	lpObj->StartY = (BYTE)lpObj->Y;

	lpObj->PathCur = 0;

	lpObj->PathCount = 0;

	lpObj->PathStartEnd = 0;

	gObjMonsterInitHitDamage(lpObj);

	return 1;
}

bool gObjMonsterMoveCheck(LPOBJ lpObj, int tx, int ty)
{
	if (lpObj->ActionState.Emotion == 1)
	{
		return 1;
	}

	if (lpObj->PosNum == -1)
	{
		return 0;
	}

	if (gMonsterSetBase.m_MonsterSetBaseInfo[lpObj->PosNum].Dis == 0)
	{
		return 0;
	}

	if (sqrt((pow((float)(tx - lpObj->StartX), (float)2) + pow((float)(ty - lpObj->StartY), (float)2))) > gMonsterSetBase.m_MonsterSetBaseInfo[lpObj->PosNum].Dis)
	{
		return 0;
	}

	return 1;
}

void gObjMonsterInitHitDamage(LPOBJ lpObj)
{
	for (int n = 0; n < MAX_HIT_DAMAGE; n++)
	{
		lpObj->HitDamage[n].index = -1;

		lpObj->HitDamage[n].damage = 0;

		lpObj->HitDamage[n].time = 0;
	}

	lpObj->HitDamageCount = 0;
}

void gObjMonsterSetHitDamage(LPOBJ lpObj, int aIndex, int damage)
{
	int HitDamageIndex = -1;

	for (int n = 0; n < MAX_HIT_DAMAGE; n++)
	{
		if (OBJECT_RANGE(lpObj->HitDamage[n].index) == false)
		{
			HitDamageIndex = n;

			continue;
		}

		if (lpObj->HitDamage[n].index == aIndex)
		{
			lpObj->HitDamage[n].damage = (((lpObj->HitDamage[n].damage + damage) > lpObj->MaxLife) ? (int)lpObj->MaxLife : (lpObj->HitDamage[n].damage + damage));

			lpObj->HitDamage[n].time = GetTickCount();

			return;
		}
	}

	if (CHECK_RANGE(HitDamageIndex, MAX_HIT_DAMAGE) != false || OBJECT_RANGE(HitDamageIndex = gObjMonsterDelHitDamageUser(lpObj)) != false)
	{
		lpObj->HitDamage[HitDamageIndex].index = aIndex;

		lpObj->HitDamage[HitDamageIndex].damage = ((damage > lpObj->MaxLife) ? (int)lpObj->MaxLife : damage);

		lpObj->HitDamage[HitDamageIndex].time = GetTickCount();

		lpObj->HitDamageCount++;
	}
}

int gObjMonsterDelHitDamageUser(LPOBJ lpObj)
{
	int HitDamageIndex = -1;

	for (int n = 0; n < MAX_HIT_DAMAGE; n++)
	{
		if (OBJECT_RANGE(lpObj->HitDamage[n].index) == false)
		{
			continue;
		}

		int result = 0;

		if (gObjIsConnected(lpObj->HitDamage[n].index) == 0)
		{
			result = 1;
		}
		else if ((GetTickCount() - lpObj->HitDamage[n].time) > 30000)
		{
			result = 1;
		}
		else if (lpObj->Map != gObj[lpObj->HitDamage[n].index].Map)
		{
			result = 1;
		}
		else if (gObjCalcDistance(lpObj, &gObj[lpObj->HitDamage[n].index]) > 20)
		{
			result = 1;
		}

		if (result != 0)
		{
			lpObj->HitDamage[n].index = -1;

			lpObj->HitDamageCount--;

			HitDamageIndex = n;
		}
	}

	return HitDamageIndex;
}

int gObjMonsterGetTopHitDamageUser(LPOBJ lpObj)
{
	int TopHitDamageUser = -1;

	int TopHitDamage = 0;

	int PartyTopHitDamageUser = -1;

	int PartyTopHitDamage = 0;

	for (int n = 0; n < MAX_HIT_DAMAGE; n++)
	{
		if (gObjIsConnected(lpObj->HitDamage[n].index) == 0)
		{
			continue;
		}

		if ((GetTickCount() - lpObj->HitDamage[n].time) > 30000)
		{
			continue;
		}

		if (lpObj->Map != gObj[lpObj->HitDamage[n].index].Map)
		{
			continue;
		}

		if (gObjCalcDistance(lpObj, &gObj[lpObj->HitDamage[n].index]) > 20)
		{
			continue;
		}

		if (OBJECT_RANGE(gObj[lpObj->HitDamage[n].index].PartyNumber) == false)
		{
			if (lpObj->HitDamage[n].damage > TopHitDamage)
			{
				TopHitDamageUser = lpObj->HitDamage[n].index;

				TopHitDamage = lpObj->HitDamage[n].damage;
			}
		}
		else
		{
			if ((PartyTopHitDamage = gObjMonsterGetTopHitDamageParty(lpObj, gObj[lpObj->HitDamage[n].index].PartyNumber, &PartyTopHitDamageUser)) > TopHitDamage)
			{
				TopHitDamageUser = PartyTopHitDamageUser;

				TopHitDamage = PartyTopHitDamage;
			}
		}
	}

	return TopHitDamageUser;
}

int gObjMonsterGetTopHitDamageParty(LPOBJ lpObj, int PartyNumber, int* TopHitDamageUser)
{
	int TopHitDamage = 0;

	int TotalHitDamage = 0;

	for (int n = 0; n < MAX_HIT_DAMAGE; n++)
	{
		if (gObjIsConnected(lpObj->HitDamage[n].index) == 0)
		{
			continue;
		}

		if ((GetTickCount() - lpObj->HitDamage[n].time) > 30000)
		{
			continue;
		}

		if (lpObj->Map != gObj[lpObj->HitDamage[n].index].Map)
		{
			continue;
		}

		if (gObjCalcDistance(lpObj, &gObj[lpObj->HitDamage[n].index]) > 20)
		{
			continue;
		}

		for (int i = 0; i < MAX_PARTY_USER; i++)
		{
			if (gParty.m_PartyInfo[PartyNumber].Index[i] == lpObj->HitDamage[n].index)
			{
				if (lpObj->HitDamage[n].damage > TopHitDamage)
				{
					(*TopHitDamageUser) = lpObj->HitDamage[n].index;

					TopHitDamage = lpObj->HitDamage[n].damage;

					TotalHitDamage += lpObj->HitDamage[n].damage;
				}
				else
				{
					(*TopHitDamageUser) = (*TopHitDamageUser);

					TopHitDamage = TopHitDamage;

					TotalHitDamage += lpObj->HitDamage[n].damage;
				}
			}
		}
	}

	return TotalHitDamage;
}

int gObjAddMonster(int map)
{
	static CCriticalSection critical;

	critical.lock();

	int index = -1;

	int count = gObjMonCount;

	if (gMemoryAllocator.GetMemoryAllocatorFree(&index, OBJECT_START_MONSTER, MAX_OBJECT_MONSTER, 10000) != 0)
	{
		gObjAllocData(index);

		gObjCharZeroSet(index);

		gObj[index].Index = index;

		gObj[index].Connected = OBJECT_CONNECTED;

		critical.unlock();

		return index;
	}

	for (int n = OBJECT_START_MONSTER; n < MAX_OBJECT_MONSTER; n++)
	{
		if (gObj[count].Connected == OBJECT_OFFLINE)
		{
			if (gObjAllocData(count) == 0)
			{
				break;
			}

			gObjCharZeroSet(count);

			gObj[count].Index = count;

			gObj[count].Connected = OBJECT_CONNECTED;

			critical.unlock();

			return count;
		}
		else
		{
			count = (((++count) >= MAX_OBJECT_MONSTER) ? OBJECT_START_MONSTER : count);
		}
	}

	critical.unlock();

	return -1;
}

int gObjAddSummon()
{
	static CCriticalSection critical;

	critical.lock();

	int index = -1;

	int count = gObjCallMonCount;

	if (gMemoryAllocator.GetMemoryAllocatorFree(&index, MAX_OBJECT_MONSTER, OBJECT_START_USER, 10000) != 0)
	{
		gObjAllocData(index);

		gObjCharZeroSet(index);

		gObj[index].Index = index;

		gObj[index].Connected = OBJECT_CONNECTED;

		critical.unlock();

		return index;
	}

	for (int n = MAX_OBJECT_MONSTER; n < OBJECT_START_USER; n++)
	{
		if (gObj[count].Connected == OBJECT_OFFLINE)
		{
			if (gObjAllocData(count) == 0)
			{
				break;
			}

			gObjCharZeroSet(count);

			gObj[count].Index = count;

			gObj[count].Connected = OBJECT_CONNECTED;

			critical.unlock();

			return count;
		}
		else
		{
			count = (((++count) >= OBJECT_START_USER) ? MAX_OBJECT_MONSTER : count);
		}
	}

	critical.unlock();

	return -1;
}

int gObjSummonSetEnemy(LPOBJ lpObj, int aIndex)
{
	if (lpObj->Type != OBJECT_USER)
	{
		return -1;
	}

	LPOBJ lpSummon = &gObj[lpObj->SummonIndex];

	if (OBJECT_RANGE(lpSummon->TargetNumber) != false)
	{
		return -1;
	}

	if (gObjIsConnected(aIndex) == 0 || aIndex == lpObj->Index || aIndex == lpSummon->Index)
	{
		return -1;
	}

	lpSummon->TargetNumber = aIndex;

	lpSummon->ActionState.Emotion = 1;

	lpSummon->ActionState.EmotionCount = 30;

	return aIndex;
}

void gObjSummonKill(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (OBJECT_RANGE(lpObj->SummonIndex) == false)
	{
		return;
	}

	LPOBJ lpSummon = &gObj[lpObj->SummonIndex];

	if (lpSummon->Type != OBJECT_MONSTER || lpSummon->SummonIndex != aIndex)
	{
		return;
	}

	if (gObjIsConnected(lpSummon->Index) != 0)
	{
		lpSummon->SummonIndex = -1;
	}

	lpObj->SummonIndex = -1;

	gObjDel(lpSummon->Index);
}

void gObjMonsterMagicAttack(LPOBJ lpObj, int skill)
{
	CSkill* lpSkill = &lpObj->Skill[skill];

	if (lpSkill->IsSkill() == 0)
	{
		return;
	}

	gSkillManager.GCDurationSkillAttackSend(lpObj, lpSkill->m_index, (BYTE)lpObj->X, (BYTE)lpObj->Y, 0);

	BYTE send[256];

	PMSG_MULTI_SKILL_ATTACK_RECV pMsg;

	pMsg.header.set(PROTOCOL_CODE4, sizeof(pMsg));

	int size = sizeof(pMsg);

	pMsg.skill = 0;

	pMsg.x = (BYTE)lpObj->X;

	pMsg.y = (BYTE)lpObj->Y;

	pMsg.serial = 0;

	pMsg.count = 0;

	PMSG_MULTI_SKILL_ATTACK info;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE || OBJECT_RANGE(lpObj->VpPlayer2[n].index) == false || lpObj->VpPlayer2[n].type != OBJECT_USER)
		{
			continue;
		}

		if (gObjCalcDistance(lpObj, &gObj[lpObj->VpPlayer2[n].index]) >= 6)
		{
			continue;
		}

		info.index[0] = SET_NUMBERHB(lpObj->VpPlayer2[n].index);

		info.index[1] = SET_NUMBERLB(lpObj->VpPlayer2[n].index);

		memcpy(&send[size], &info, sizeof(info));

		size += sizeof(info);

		if (CHECK_SKILL_ATTACK_EXTENDED_COUNT(pMsg.count) == false)
		{
			break;
		}
	}

	pMsg.header.size = size;

	memcpy(send, &pMsg, sizeof(pMsg));

	gSkillManager.CGMultiSkillAttackRecv((PMSG_MULTI_SKILL_ATTACK_RECV*)send, lpObj->Index, 1);
}

void gObjMonsterBeattackRecv(BYTE* aRecv, int aIndex)
{
	PMSG_MULTI_SKILL_ATTACK_RECV* lpMsg = (PMSG_MULTI_SKILL_ATTACK_RECV*)aRecv;

	CSkill* lpSkill = &gObj[aIndex].Skill[lpMsg->skill];

	if (lpSkill->IsSkill() == 0)
	{
		return;
	}

	for (int n = 0; n < lpMsg->count; n++)
	{
		PMSG_MULTI_SKILL_ATTACK* lpInfo = (PMSG_MULTI_SKILL_ATTACK*)(((BYTE*)lpMsg) + sizeof(PMSG_MULTI_SKILL_ATTACK_RECV) + (sizeof(PMSG_MULTI_SKILL_ATTACK) * n));

		int bIndex = MAKE_NUMBERW(lpInfo->index[0], lpInfo->index[1]);

		if (OBJECT_RANGE(bIndex) == false)
		{
			continue;
		}

		gAttack.Attack(&gObj[aIndex], &gObj[bIndex], lpSkill, 1, 0, 0);
	}
}

void gObjMonsterAttack(LPOBJ lpObj, LPOBJ lpTarget)
{
	if (lpObj->Class == 66 || lpObj->Class == 73 || lpObj->Class == 77)
	{
		PMSG_SKILL_ATTACK_RECV pMsg;

		pMsg.header.set(0x19, sizeof(pMsg));

		pMsg.skill = 1;

		pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

		pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

		pMsg.dis = 0;

		gSkillManager.CGSkillAttackRecv(&pMsg, lpObj->Index);
	}
	else if (lpObj->Class == 89 || lpObj->Class == 95 || lpObj->Class == 112 || lpObj->Class == 118 || lpObj->Class == 124 || lpObj->Class == 130)
	{
		PMSG_SKILL_ATTACK_RECV pMsg;

		pMsg.header.set(0x19, sizeof(pMsg));

		pMsg.skill = 1;

		pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

		pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

		pMsg.dis = 0;

		gSkillManager.CGSkillAttackRecv(&pMsg, lpObj->Index);
	}
	else
	{
		if (lpObj->AttackType == 0 || lpObj->AttackType >= 100)
		{
			PMSG_ATTACK_RECV pMsg;

			pMsg.header.set(PROTOCOL_CODE2, sizeof(pMsg));

			pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

			pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

			pMsg.action = ACTION_ATTACK1;

			pMsg.dir = lpObj->Dir;

			gAttack.CGAttackRecv(&pMsg, lpObj->Index);
		}
		else
		{
			PMSG_SKILL_ATTACK_RECV pMsg;

			pMsg.header.set(0x19, sizeof(pMsg));

			pMsg.skill = 0;

			pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

			pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

			pMsg.dis = 0;

			gSkillManager.CGSkillAttackRecv(&pMsg, lpObj->Index);
		}
	}
}

void gObjMonsterDie(LPOBJ lpObj, LPOBJ lpTarget)
{
	gObjMonsterDieGiveItem(lpObj, lpTarget);

	lpObj->NextActionTime = 500;

	if (OBJECT_RANGE(lpObj->SummonIndex) != false)
	{
		gObjSummonKill(lpObj->SummonIndex);
	}
}

void gObjMonsterStateProc(LPOBJ lpObj, int code, int aIndex, int SubCode)
{
	switch (code)
	{
		case 0:
		{
			gObjMonsterStateProcCase0(lpObj, aIndex);

			break;
		}

		case 1:
		{
			gObjMonsterDie(lpObj, &gObj[aIndex]);

			break;
		}

		case 2:
		{
			if (gObj[aIndex].Live != 0)
			{
				gObjBackSpring(lpObj, &gObj[aIndex]);
			}

			break;
		}

		case 3:
		{
			lpObj->TargetNumber = -1;

			lpObj->LastAttackerID = -1;

			lpObj->NextActionTime = 1000;

			lpObj->ActionState.Attack = 0;

			lpObj->ActionState.Move = 0;

			lpObj->ActionState.Emotion = 0;

			break;
		}

		case 4:
		{
			lpObj->ActionState.Emotion = 3;

			lpObj->ActionState.EmotionCount = 1;

			break;
		}

		case 6:
		{
			if (gObj[aIndex].Live != 0)
			{
				gObjBackSpring2(lpObj, &gObj[aIndex], 2);
			}

			break;
		}

		case 7:
		{
			if (gObj[aIndex].Live != 0)
			{
				gObjBackSpring2(lpObj, &gObj[aIndex], 3);
			}

			break;
		}

		case 55:
		{
			if (gObj[aIndex].Live != 0)
			{
				gAttack.Attack(lpObj, &gObj[aIndex], 0, 0, 0, 0);
			}

			break;
		}

		case 56:
		{
			if (gObjCheckResistance(&gObj[aIndex], 1) == 0)
			{
				gEffectManager.AddEffect(&gObj[aIndex], 0, EFFECT_POISON, SubCode, lpObj->Index, 2, 3, 0);
			}

			break;
		}

		case 57:
		{
			if (gObj[aIndex].Live != 0)
			{
				gObjBackSpring2(&gObj[aIndex], lpObj, SubCode);
			}

			break;
		}
	}
}

//**************************************************************************//
// RAW FUNCTIONS ***********************************************************//
//**************************************************************************//

int gObjMonsterViewportIsCharacter(LPOBJ lpObj)
{
	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state != 0)
		{
			if (lpObj->VpPlayer2[n].index == lpObj->TargetNumber)
			{
				return n;
			}
		}
	}

	return -1;
}

int gObjMonsterGetTargetPos(LPOBJ lpObj)
{
	if (lpObj->MoveRange == 0 && gEffectManager.CheckImmobilizeEffect(lpObj) != 0)
	{
		return 0;
	}

	if (OBJECT_RANGE(lpObj->TargetNumber) == false)
	{
		return 0;
	}

	LPOBJ lpTarget = &gObj[lpObj->TargetNumber];

	if (lpTarget->Teleport != 0)
	{
		return 0;
	}

	if (gObjMonsterViewportIsCharacter(lpObj) == -1)
	{
		return 0;
	}

	int dis = (lpObj->AttackType >= 100) ? (lpObj->AttackRange + 2) : lpObj->AttackRange;

	int tpx = (lpObj->X > lpTarget->X) ? (lpTarget->X + dis) : (lpTarget->X - dis);

	int tpy = (lpObj->Y > lpTarget->Y) ? (lpTarget->Y + dis) : (lpTarget->Y - dis);

	int mtx = lpTarget->X;

	int mty = lpTarget->Y;

	if (gMap[lpObj->Map].CheckStandAttr(tpx, tpy) == 0)
	{
		int index = GetPathPacketDirPos((mtx - tpx), (mty - tpy)) * 2;

		for (int i = 1; i < (MAX_ROAD_PATH_TABLE / 2); i++)
		{
			mtx = lpTarget->X + RoadPathTable[index];

			mty = lpTarget->Y + RoadPathTable[1 + index];

			BYTE attr = gMap[lpObj->Map].GetAttr(mtx, mty);

			if (gObjMonsterMoveCheck(lpObj, mtx, mty) != 0)
			{
				if ((lpObj->Class == 249 && (attr & 2) != 2) || ((attr & 1) != 1 && (attr & 2) != 2 && (attr & 4) != 4 && (attr & 8) != 8))
				{
					lpObj->MTX = mtx;

					lpObj->MTY = mty;

					return 1;
				}
			}

			index += 2;

			index = (index > MAX_ROAD_PATH_TABLE - 1) ? 0 : index;
		}

		return 0;
	}
	else
	{
		BYTE attr = gMap[lpObj->Map].GetAttr(tpx, tpy);

		if (gObjMonsterMoveCheck(lpObj, mtx, mty) != 0)
		{
			if ((lpObj->Class == 249 && (attr & 2) != 2) || ((attr & 1) != 1 && (attr & 2) != 2 && (attr & 4) != 4 && (attr & 8) != 8))
			{
				lpObj->MTX = tpx;

				lpObj->MTY = tpy;

				return 1;
			}
		}

		return 0;
	}
}

BOOL gObjGetTargetPos(LPOBJ lpObj, int sx, int sy, int& tx, int& ty)
{
	int tpx, tpy, mtx, mty;
	int searchp = 0, sn = 0;
	int searchcount = 7;
	BYTE attr;

	mtx = tpx = sx;
	mty = tpy = sy;

	int dis;

	if (lpObj->AttackType >= 100)
	{
		dis = lpObj->AttackRange + 2;
	}
	else
	{
		dis = lpObj->AttackRange;
	}

	if (lpObj->X < mtx)
	{
		tpx -= dis;
	}

	if (lpObj->X > mtx)
	{
		tpx += dis;
	}

	if (lpObj->Y < mty)
	{
		tpy -= dis;
	}

	if (lpObj->Y > mty)
	{
		tpy += dis;
	}

	searchp = GetPathPacketDirPos(sx - tpx, sy - tpy) * 2;

	if (gMap[lpObj->Map].CheckStandAttr(tpx, tpy) == 0)
	{
		while (searchcount--)
		{
			mtx = sx + RoadPathTable[searchp];
			mty = sy + RoadPathTable[1 + searchp];
			attr = gMap[lpObj->Map].GetAttr(mtx, mty);

			if ((attr & 1) != 1 && (attr & 2) != 2 && (attr & 4) != 4 && (attr & 8) != 8)
			{
				tx = mtx;
				ty = mty;
				return TRUE;
			}

			searchp += 2;

			if (searchp > MAX_ROAD_PATH_TABLE - 1)
			{
				searchp = 0;
			}
		}

		return FALSE;
	}

	attr = gMap[lpObj->Map].GetAttr(tpx, tpy);

	if (lpObj->Attribute >= 100)
	{
		if ((attr & 2) != 2 && (attr & 4) != 4 && (attr & 8) != 8)
		{
			tx = tpx;
			ty = tpy;
			return TRUE;
		}
	}
	else if ((attr & 1) != 1 && (attr & 2) != 2 && (attr & 4) != 4 && (attr & 8) != 8)
	{
		tx = tpx;
		ty = tpy;
		return TRUE;
	}

	return FALSE;
}

int gObjMonsterSearchEnemy(LPOBJ lpObj, BYTE objtype)
{
	int tx, ty;
	int dis;
	int mindis = lpObj->ViewRange;
	int searchtarget = -1;
	int tObjNum;
	int count = 3;
	int t1, t2;

	t1 = objtype;//&OBJTYPE_CHARACTER;
	t2 = objtype;//&OBJTYPE_MONSTER;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		tObjNum = lpObj->VpPlayer2[n].index;

		if (tObjNum >= 0)
		{
			if ((gObj[tObjNum].Type == t1 || gObj[tObjNum].Type == t2) && (gObj[tObjNum].Live != FALSE))
			{
				if (((gObj[tObjNum].Class >= 100 && gObj[tObjNum].Class < 110)) || (gObj[tObjNum].Type == OBJECT_MONSTER && gObj[tObjNum].SummonIndex >= 0)) //TRAPS
				{

				}
				else if ((gObj[tObjNum].Authority & 2) != 2 && (gObj[tObjNum].Authority & 32) != 32 && gObj[tObjNum].Teleport == 0)
				{
					tx = lpObj->X - gObj[tObjNum].X;
					ty = lpObj->Y - gObj[tObjNum].Y;
					dis = (int)sqrt((float)tx * (float)tx + (float)ty * (float)ty);

					if (dis < mindis)
					{
						searchtarget = tObjNum;
						mindis = dis;
					}
				}
			}
		}
	}

	return searchtarget;
}

int gObjGuardSearchEnemy(LPOBJ lpObj)
{
	int tx, ty;
	int dis;
	int mindis = lpObj->ViewRange;
	int searchtarget = -1;
	int tObjNum;
	BYTE attr;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		tObjNum = lpObj->VpPlayer2[n].index;

		if (tObjNum >= 0)
		{
			BOOL bEnableAttack = FALSE;

			if (gObj[tObjNum].PKLevel > PKLVL_WARNING)
			{
				bEnableAttack = TRUE;
			}

			if (gObj[tObjNum].Type == OBJECT_USER)
			{
				if (bEnableAttack == TRUE)
				{
					attr = gMap[gObj[tObjNum].Map].GetAttr(gObj[tObjNum].X, gObj[tObjNum].Y);

					if ((attr & 1) != 1)
					{
						tx = lpObj->X - gObj[tObjNum].X;

						ty = lpObj->Y - gObj[tObjNum].Y;

						dis = (int)sqrt(((tx * tx) + (ty * ty)) * 1.0f);

						if (dis < mindis)
						{
							searchtarget = tObjNum;

							mindis = dis;
						}
					}
				}
			}
		}
	}

	return searchtarget;
}

void gObjMonsterProcess(LPOBJ lpObj)
{
	gObjectManager.ObjectMsgProc(lpObj);

	if (lpObj->Live == FALSE)
	{
		return;
	}

	if (lpObj->State != OBJECT_PLAYING)
	{
		return;
	}

	if ((GetTickCount() - lpObj->CurActionTime) < (lpObj->NextActionTime + lpObj->DelayActionTime))
	{
		return;
	}

	lpObj->CurActionTime = GetTickCount();

	if (BC_MAP_RANGE(lpObj->Map) != false)
	{
		if (lpObj->Class == 131 || ((lpObj->Class - 132 < 0) ? FALSE : (lpObj->Class - 132 > 2) ? FALSE : TRUE) != FALSE)
		{
			return;
		}
	}

	if ((lpObj->Class >= 100 && lpObj->Class <= 110))
	{
		gObjMonsterTrapAct(lpObj);
	}
	else if (lpObj->Class == 200)
	{
		int ground;

		int team = gBattleSoccer.CheckGoal(lpObj->X, lpObj->Y, ground);

		if (team >= 0)
		{
			gObjBallRegen(lpObj);

			gBattleSoccer.BattleSoccerScoreUpdate(ground, team);

			return;
		}
	}
	else
	{
		gObjMonsterBaseAct(lpObj);
	}

	if (lpObj->Attribute == 100)
	{
		if (lpObj->SummonIndex >= 0 && lpObj->SummonIndex < MAX_OBJECT)
		{
			LPOBJ lpCallMonObj;

			BOOL Success = FALSE;

			lpCallMonObj = &gObj[lpObj->SummonIndex];

			if (lpObj->Map != lpCallMonObj->Map)
			{
				Success = TRUE;
			}

			if (gObjCalcDistance(lpCallMonObj, lpObj) > 14)
			{
				Success = TRUE;
			}

			if (Success == TRUE)
			{
				gObjTeleportMagicUse(lpObj->Index, (BYTE)lpCallMonObj->X + 1, lpCallMonObj->Y);

				lpObj->Map = lpCallMonObj->Map;

				return;
			}
		}
	}

	if (lpObj->ActionState.Move != 0)
	{
		if (PathFindMoveMsgSend(lpObj) == TRUE)
		{
			lpObj->ActionState.Move = (DWORD)0;

			return;
		}
		else
		{
			lpObj->ActionState.Move = (DWORD)1;

			return;
		}
	}

	if (lpObj->ActionState.Attack == 1)
	{
		if (CMonsterSkillManager::CheckMonsterSkill(lpObj->Class, 0))
		{
			BOOL bEnableAttack = TRUE;

			if (lpObj->TargetNumber < 0)
			{
				lpObj->TargetNumber = -1;

				lpObj->ActionState.Emotion = 0;

				lpObj->ActionState.Attack = 0;

				lpObj->ActionState.Move = 0;

				lpObj->NextActionTime = 1000;

				return;
			}

			if (gObj[lpObj->TargetNumber].Live == FALSE || gObj[lpObj->TargetNumber].Teleport != 0)
			{
				bEnableAttack = FALSE;
			}

			if (gObj[lpObj->TargetNumber].Connected <= OBJECT_LOGGED || gObj[lpObj->TargetNumber].CloseCount != -1)
			{
				bEnableAttack = FALSE;
			}

			if (bEnableAttack == FALSE)
			{
				lpObj->TargetNumber = -1;

				lpObj->ActionState.Emotion = 0;

				lpObj->ActionState.Attack = 0;

				lpObj->ActionState.Move = 0;

				lpObj->NextActionTime = 1000;

				return;
			}

			if (GetLargeRand() % 4 == 0)
			{
				GCActionSend(lpObj, ACTION_ATTACK1, lpObj->Index, lpObj->TargetNumber);

				gAttack.Attack(lpObj, &gObj[lpObj->TargetNumber], NULL, FALSE, 0, 0);
			}
			else
			{
				CMonsterSkillManager::UseMonsterSkill(lpObj->Index, lpObj->TargetNumber, 0, -1, NULL);
			}

			lpObj->ActionState.Attack = 0;
		}
		else
		{
			int AttackType = lpObj->AttackType;

			int AttackFlag = 0;

			if (AttackType >= 100)
			{
				if ((GetLargeRand() % 5) == 0)
				{
					AttackType -= 100;

					AttackFlag = TRUE;
				}
				else
				{
					AttackType = 0;
				}
			}

			if (AttackFlag != FALSE || lpObj->AttackType == 50)
			{
				if (lpObj->TargetNumber >= 0)
				{
					if (gObj[lpObj->TargetNumber].Connected > OBJECT_LOGGED && gObj[lpObj->TargetNumber].CloseCount == -1)
					{
						if (gObj[lpObj->TargetNumber].Live == FALSE)
						{
							lpObj->TargetNumber = -1;

							lpObj->ActionState.Emotion = 0;

							lpObj->ActionState.Attack = 0;

							lpObj->ActionState.Move = 0;

							lpObj->NextActionTime = 1000;
						}
						else if (gObj[lpObj->TargetNumber].Teleport == 0)
						{
							gObjMonsterMagicAttack(lpObj, 0);
						}
					}
					else
					{
						lpObj->TargetNumber = -1;

						lpObj->ActionState.Emotion = 0;

						lpObj->ActionState.Attack = 0;

						lpObj->ActionState.Move = 0;

						lpObj->NextActionTime = 1000;
					}
				}
			}
			else
			{
				if (lpObj->TargetNumber >= 0)
				{
					if (gObj[lpObj->TargetNumber].Connected > OBJECT_LOGGED && gObj[lpObj->TargetNumber].CloseCount == -1)
					{
						if (gObj[lpObj->TargetNumber].Live == FALSE)
						{
							lpObj->TargetNumber = -1;

							lpObj->ActionState.Emotion = 0;

							lpObj->ActionState.Attack = 0;

							lpObj->ActionState.Move = 0;

							lpObj->NextActionTime = 1000;
						}
						else if (gObj[lpObj->TargetNumber].Teleport == 0)
						{
							gObjMonsterAttack(lpObj, &gObj[lpObj->TargetNumber]);
						}

					}
					else
					{
						lpObj->TargetNumber = -1;

						lpObj->ActionState.Emotion = 0;

						lpObj->ActionState.Attack = 0;

						lpObj->ActionState.Move = 0;

						lpObj->NextActionTime = 1000;
					}
				}
			}

			lpObj->ActionState.Attack = 0;
		}
	}
}

BOOL PathFindMoveMsgSend(LPOBJ lpObj)
{
	if (gEffectManager.CheckImmobilizeEffect(lpObj) != 0)
	{
		return 0;
	}

	PMSG_MOVE_RECV pMove;

	pMove.header.type = 0xC1;

	pMove.header.head = PROTOCOL_CODE1;

	pMove.header.size = sizeof(pMove);

	pMove.x = (BYTE)lpObj->X;

	pMove.y = (BYTE)lpObj->Y;

	PATH_INFO path;

	BOOL bPathFound = gMap[lpObj->Map].PathFinding2(lpObj->X, lpObj->Y, lpObj->MTX, lpObj->MTY, &path);

	if (bPathFound)
	{
		BYTE pos = 0;

		BYTE bPath[8];

		memset(bPath, 0, sizeof(bPath));

		int tx;
		int ty;
		int sx = lpObj->X;
		int sy = lpObj->Y;

		for (int n = 1; n < path.PathNum; n++)
		{
			tx = path.PathX[n];
			ty = path.PathY[n];
			pos = GetPathPacketDirPos(tx - sx, ty - sy);
			sx = tx;
			sy = ty;

			if ((n % 2) == 1)
			{
				bPath[(n + 1) / 2] = (pos << 4);
			}
			else
			{
				bPath[(n + 1) / 2] |= pos;
			}
		}

		bPath[0] = (pos << 4) + ((path.PathNum - 1) & 0x0F);

		memcpy(pMove.path, bPath, sizeof(pMove.path));

		CGMoveRecv((PMSG_MOVE_RECV*)&pMove, lpObj->Index);

		return TRUE;
	}

	return FALSE;
}

void gObjMonsterMoveAction(LPOBJ lpObj)
{
	if (gEffectManager.CheckImmobilizeEffect(lpObj) != 0)
	{
		return;
	}

	int maxmoverange = lpObj->MoveRange * 2 + 1;

	int searchc = 10;

	lpObj->NextActionTime = 1000;

	BYTE tpx;

	BYTE tpy;

	while (searchc-- != 0)
	{
		__try
		{
			tpx = (lpObj->X - lpObj->MoveRange) + (BYTE)(GetLargeRand() % maxmoverange);

			tpy = (lpObj->Y - lpObj->MoveRange) + (BYTE)(GetLargeRand() % maxmoverange);
		}
		__except (maxmoverange = 1, 1)
		{

		}

		int mchk = gObjMonsterMoveCheck(lpObj, tpx, tpy);

		BYTE attr = gMap[lpObj->Map].GetAttr(tpx, tpy);

		if ((lpObj->Class == 249 || lpObj->Class == 247) && mchk)
		{
			if ((attr & 2) != 2)
			{
				lpObj->MTX = tpx;

				lpObj->MTY = tpy;

				lpObj->ActionState.Move = 1;

				lpObj->NextActionTime = 3000;

				return;
			}
		}
		else
		{
			if ((attr & 1) != 1 && (attr & 2) != 2 && (attr & 4) != 4 && (attr & 8) != 8 && mchk)
			{
				lpObj->TargetNumber = -1;

				lpObj->ActionState.Attack = 0;

				lpObj->NextActionTime = 500;

				lpObj->ActionState.Emotion = 0;

				lpObj->MTX = tpx;

				lpObj->MTY = tpy;

				lpObj->ActionState.Move = 1;

				return;
			}
		}
	}
}

void gObjMonsterBaseAct(LPOBJ lpObj)
{
	LPOBJ lpTargetObj = NULL;

	if (lpObj->TargetNumber >= 0)
	{
		lpTargetObj = &gObj[lpObj->TargetNumber];
	}
	else
	{
		lpObj->ActionState.Emotion = 0;
	}

	if (lpObj->ActionState.Emotion == 0)
	{
		if (lpObj->Attribute)
		{
			if (lpObj->ActionState.Attack)
			{
				lpObj->ActionState.Attack = 0;

				lpObj->TargetNumber = -1;

				lpObj->NextActionTime = 500;
			}

			int actcode1 = GetLargeRand() % 2;

			if (lpObj->Attribute == 100)
			{
				actcode1 = 1;
			}

			if (actcode1 == 0)
			{
				lpObj->ActionState.Rest = 1;

				lpObj->NextActionTime = 500;
			}
			else if (lpObj->MoveRange > 0 && gEffectManager.CheckImmobilizeEffect(lpObj) == 0)
			{
				if (lpObj->Attribute != 100)
				{
					gObjMonsterMoveAction(lpObj);
				}
				else
				{
					int tx = 0;

					int ty = 0;

					if (lpObj->SummonIndex >= 0)
					{
						if (gObj[lpObj->SummonIndex].Connected > OBJECT_LOGGED)
						{
							LPOBJ lpRecallObj = &gObj[lpObj->SummonIndex];

							if (lpRecallObj->Rest == FALSE)
							{
								if (gObjGetTargetPos(lpObj, lpRecallObj->X, lpRecallObj->Y, tx, ty) == TRUE)
								{
									lpObj->MTX = tx;

									lpObj->MTY = ty;

									lpObj->ActionState.Move = 1;

									lpObj->NextActionTime = 1000;
								}
							}
						}
					}
				}
			}

			if (lpObj->Class == 249 || lpObj->Class == 247)
			{
				lpObj->TargetNumber = gObjGuardSearchEnemy(lpObj);

				if (lpObj->TargetNumber >= 0)
				{
					if (gObj[lpObj->TargetNumber].Class >= 248)
					{
						lpObj->TargetNumber = -1;
					}
				}
			}
			else
			{
				if (lpObj->Attribute == 100)
				{
					lpObj->TargetNumber = gObjMonsterSearchEnemy(lpObj, OBJECT_MONSTER);
				}
				else
				{
					lpObj->TargetNumber = gObjMonsterSearchEnemy(lpObj, OBJECT_USER);
				}
			}

			if (lpObj->TargetNumber >= 0)
			{
				lpObj->ActionState.EmotionCount = 30;

				lpObj->ActionState.Emotion = 1;
			}
		}
	}
	else if (lpObj->ActionState.Emotion == 1)
	{
		if (lpObj->ActionState.EmotionCount > 0)
		{
			lpObj->ActionState.EmotionCount--;
		}
		else
		{
			lpObj->ActionState.Emotion = 0;
		}

		if (lpObj->TargetNumber >= 0 && lpObj->PathStartEnd == 0)
		{
			if (BC_MAP_RANGE(lpObj->Map) != false)
			{
				int iRAND_CHANGE_TARGET = GetLargeRand() % 10;

				if (iRAND_CHANGE_TARGET == 3)
				{
					lpObj->LastAttackerID = -1;
				}

				if (iRAND_CHANGE_TARGET == 1)
				{
					if (lpObj->LastAttackerID != -1 && lpObj->LastAttackerID != lpObj->TargetNumber)
					{
						if (gObj[lpObj->LastAttackerID].Connected > OBJECT_LOGGED && lpObj->Map == gObj[lpObj->LastAttackerID].Map)
						{
							lpObj->TargetNumber = lpObj->LastAttackerID;

							lpTargetObj = &gObj[lpObj->LastAttackerID];
						}
					}
				}
			}

			int dis = gObjCalcDistance(lpObj, lpTargetObj);

			int attackrange;

			if (lpObj->AttackType >= 100)
			{
				attackrange = lpObj->AttackRange + 2;
			}
			else
			{
				attackrange = lpObj->AttackRange;
			}

			if (dis <= attackrange)
			{
				int tuser = lpObj->TargetNumber;

				int map = gObj[tuser].Map;

				BYTE attr;

				if (gMap[map].CheckWall(lpObj->X, lpObj->Y, gObj[tuser].X, gObj[tuser].Y) == TRUE)
				{
					attr = gMap[map].GetAttr(gObj[tuser].X, gObj[tuser].Y);

					if ((attr & 1) != 1)
					{
						lpObj->ActionState.Attack = 1;
					}
					else
					{
						lpObj->TargetNumber = -1;

						lpObj->ActionState.EmotionCount = 30;

						lpObj->ActionState.Emotion = 1;
					}

					lpObj->Dir = GetPathPacketDirPos(lpTargetObj->X - lpObj->X, lpTargetObj->Y - lpObj->Y);

					lpObj->NextActionTime = lpObj->PhysiSpeed;
				}
				else
				{
					lpObj->TargetNumber = -1;

					lpObj->ActionState.Attack = 0;

					lpObj->NextActionTime = 500;

					lpObj->ActionState.Emotion = 0;

					lpObj->ActionState.Move = 1;
				}
			}
			else
			{
				if (gObjMonsterGetTargetPos(lpObj) == TRUE)
				{
					if (gMap[lpObj->Map].CheckWall(lpObj->X, lpObj->Y, lpObj->MTX, lpObj->MTY) == TRUE)
					{
						lpObj->ActionState.Move = 1;

						lpObj->NextActionTime = 400;

						lpObj->Dir = GetPathPacketDirPos(lpTargetObj->X - lpObj->X, lpTargetObj->Y - lpObj->Y);
					}
					else
					{
						gObjMonsterMoveAction(lpObj);

						lpObj->ActionState.Emotion = 3;

						lpObj->ActionState.EmotionCount = 10;

					}
				}
				else
				{
					gObjMonsterMoveAction(lpObj);
				}
			}
		}
	}
	else if (lpObj->ActionState.Emotion == 2)
	{
		if (lpObj->ActionState.EmotionCount > 0)
		{
			lpObj->ActionState.EmotionCount--;
		}
		else
		{
			lpObj->ActionState.Emotion = 0;
		}

		lpObj->ActionState.Move = 1;

		lpObj->NextActionTime = 800;

		if (lpTargetObj != NULL)
		{
			int tdir = GetPathPacketDirPos(lpTargetObj->X - lpObj->X, lpTargetObj->Y - lpObj->Y) * 2;

			lpObj->MTX += RoadPathTable[tdir] * (-3);

			lpObj->MTY += RoadPathTable[tdir + 1] * (-3);
		}
	}
	else if (lpObj->ActionState.Emotion == 3)
	{
		if (lpObj->ActionState.EmotionCount > 0)
		{
			lpObj->ActionState.EmotionCount--;
		}
		else
		{
			lpObj->ActionState.Emotion = 0;
		}

		lpObj->ActionState.Move = 0;

		lpObj->ActionState.Attack = 0;

		lpObj->NextActionTime = 400;
	}
}

void gObjTrapAttackEnemySearchX(LPOBJ lpObj, int count)
{
	lpObj->TargetNumber = -1;
	int x = lpObj->X;
	int y = lpObj->Y;
	int tObjNum;

	for (int n = 0; n < count; n++)
	{
		x++;

		for (int i = 0; i < MAX_VIEWPORT; i++)
		{
			tObjNum = lpObj->VpPlayer2[i].index;

			if (tObjNum >= 0)
			{
				if (gObj[tObjNum].Type == OBJECT_USER)
				{
					if (gObj[tObjNum].Live)
					{
						if (y == gObj[tObjNum].Y)
						{
							if (x == gObj[tObjNum].X)
							{
								if ((gObj[tObjNum].Authority & 2) != 2)
								{
									lpObj->TargetNumber = tObjNum;

									return;
								}
							}
						}
					}
				}
			}
		}
	}
}

void gObjTrapAttackEnemySearchY(LPOBJ lpObj, int count)
{
	lpObj->TargetNumber = -1;
	int x = lpObj->X;
	int y = lpObj->Y;
	int tObjNum;

	for (int n = 0; n < count; n++)
	{
		y--;

		for (int i = 0; i < MAX_VIEWPORT; i++)
		{
			tObjNum = lpObj->VpPlayer2[i].index;

			if (tObjNum >= 0)
			{
				if (gObj[tObjNum].Type == 1)
				{
					if (gObj[tObjNum].Live)
					{
						if (y == gObj[tObjNum].Y)
						{
							if (x == gObj[tObjNum].X)
							{
								if ((gObj[tObjNum].Authority & 2) != 2)
								{
									lpObj->TargetNumber = tObjNum;

									return;
								}
							}
						}
					}
				}
			}
		}
	}
}

void gObjTrapAttackEnemySearch(LPOBJ lpObj)
{
	int tObjNum;

	lpObj->TargetNumber = -1;

	for (int i = 0; i < MAX_VIEWPORT; i++)
	{
		tObjNum = lpObj->VpPlayer2[i].index;

		if (tObjNum >= 0)
		{
			if (gObj[tObjNum].Type == 1)
			{
				if (gObj[tObjNum].Live)
				{
					if (lpObj->Y == gObj[tObjNum].Y)
					{
						if (lpObj->X == gObj[tObjNum].X)
						{
							if ((gObj[tObjNum].Authority & 2) != 2)
							{
								lpObj->TargetNumber = tObjNum;

								return;
							}
						}
					}
				}
			}
		}
	}
}

void gObjTrapAttackEnemySearchRange(LPOBJ lpObj, int iRange)
{
	int tObjNum = -1;
	int iTargetingRate = 0;
	int iSuccessRate = 0;

	lpObj->TargetNumber = -1;

	if (lpObj->VPCount <= 0)
	{
		return;
	}

	iTargetingRate = 100 / lpObj->VPCount;

	for (int i = 0; i < MAX_VIEWPORT; i++)
	{
		tObjNum = lpObj->VpPlayer2[i].index;

		if (tObjNum >= 0)
		{
			if (gObj[tObjNum].Type == 1)
			{
				if (gObj[tObjNum].Live)
				{
					if ((lpObj->Y - iRange) <= gObj[tObjNum].Y && (lpObj->Y + iRange) >= gObj[tObjNum].Y)
					{
						if ((lpObj->X - iRange) <= gObj[tObjNum].X && (lpObj->X + iRange) >= gObj[tObjNum].X)
						{
							if ((gObj[tObjNum].Authority & 2) != 2)
							{
								lpObj->TargetNumber = tObjNum;

								iSuccessRate = GetLargeRand() % 100;

								if (iSuccessRate < iTargetingRate)
								{
									break;
								}
							}
						}
					}
				}
			}
		}
	}
}

void gObjMonsterTrapAct(LPOBJ lpObj)
{
	if (lpObj->VPCount2 < 1)
	{
		return;
	}

	if (lpObj->AttackRange > 0)
	{
		if (lpObj->Dir == 3)
		{
			gObjTrapAttackEnemySearchX(lpObj, lpObj->AttackRange + 1);
		}
		else if (lpObj->Dir == 1)
		{
			gObjTrapAttackEnemySearchY(lpObj, lpObj->AttackRange + 1);
		}
		else if (lpObj->Dir == 8)
		{
			gObjTrapAttackEnemySearchRange(lpObj, lpObj->AttackRange);
		}
	}
	else
	{
		gObjTrapAttackEnemySearch(lpObj);
	}

	if (lpObj->TargetNumber >= 0)
	{
		lpObj->ActionState.Attack = 1;

		lpObj->NextActionTime = lpObj->PhysiSpeed;
	}
	else
	{
		lpObj->NextActionTime = lpObj->MoveSpeed;
	}
}

void gObjMonsterStateProcCase0(LPOBJ lpObj, int aIndex)
{
	if (lpObj->Attribute == 0)
	{
		return;
	}

	if (gObj[aIndex].Live == FALSE || gObj[aIndex].State != 2)
	{
		return;
	}

	if (lpObj->ActionState.Emotion == 0)
	{
		lpObj->ActionState.Emotion = 1;

		lpObj->ActionState.EmotionCount = 10;
	}
	else if (lpObj->ActionState.Emotion == 1)
	{
		lpObj->ActionState.EmotionCount = 10;
	}

	if (lpObj->ActionState.Attack == 0 && lpObj->PathStartEnd == 0)
	{
		if (OBJECT_RANGE(aIndex) != false)
		{
			int map = gObj[aIndex].Map;
			BYTE attr;
			int dis = gObjCalcDistance(lpObj, &gObj[aIndex]);
			int range;

			if (lpObj->AttackType >= 100)
			{
				range = lpObj->AttackRange + 2;
			}
			else
			{
				range = lpObj->AttackRange;
			}

			if (dis <= range)
			{
				if (gObj[aIndex].SummonIndex >= 0)
				{
					if (lpObj->SummonIndex >= 0)
					{
						if (gObj[aIndex].Type == OBJECT_MONSTER)
						{
							lpObj->TargetNumber = aIndex;
						}
					}
					else
					{
						lpObj->TargetNumber = aIndex;
					}
				}
				else if ((GetLargeRand() % 100) < 90)
				{
					if (lpObj->SummonIndex >= 0)
					{
						if (gObj[aIndex].Type == OBJECT_MONSTER)
						{
							lpObj->TargetNumber = aIndex;
						}
					}
					else
					{
						lpObj->TargetNumber = aIndex;
					}
				}
			}
			else
			{
				BYTE wall = 0;

				wall = gMap[map].CheckWall2(lpObj->X, lpObj->Y, gObj[aIndex].X, gObj[aIndex].Y);

				if (wall == 1)
				{
					attr = gMap[map].GetAttr(gObj[aIndex].X, gObj[aIndex].Y);

					if ((attr & 1) != 1)
					{
						if (lpObj->TargetNumber < 0)
						{
							lpObj->TargetNumber = aIndex;
						}
					}
				}
			}
		}
	}
	else
	{
		if ((GetLargeRand() % 2) == 1 && lpObj->PathStartEnd == 0)
		{
			int IndexEnemy = lpObj->TargetNumber;

			if (OBJECT_RANGE(IndexEnemy) == false)
			{
				return;
			}

			int EnemyMap = gObj[IndexEnemy].Map;

			int enemydis = gObjCalcDistance(lpObj, &gObj[aIndex]);

			int range;

			if (lpObj->AttackType >= 100)
			{
				range = lpObj->AttackRange + 2;
			}
			else
			{
				range = lpObj->AttackRange;
			}

			if (enemydis <= range)
			{
				lpObj->ActionState.Attack = 1;

				lpObj->TargetNumber = aIndex;
			}
			else
			{
				if (gMap[EnemyMap].CheckWall2(lpObj->X, lpObj->Y, gObj[IndexEnemy].X, gObj[IndexEnemy].Y) == 1)
				{
					lpObj->ActionState.Attack = 1;

					lpObj->TargetNumber = aIndex;
				}
			}
		}
		else
		{
			int MaxLife = (int)lpObj->MaxLife;

			MaxLife >>= 1;

			if (MaxLife > lpObj->Life)
			{
				if (lpObj->Attribute != 2)
				{
					lpObj->ActionState.Emotion = 2;

					lpObj->ActionState.EmotionCount = 2;
				}
			}
		}
	}
}