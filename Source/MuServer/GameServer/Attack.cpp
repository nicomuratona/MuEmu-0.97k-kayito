#include "stdafx.h"
#include "Attack.h"
#include "BattleSoccerManager.h"
#include "CustomSafeZone.h"
#include "CustomWing.h"
#include "EffectManager.h"
#include "Guild.h"
#include "Map.h"
#include "MapManager.h"
#include "Monster.h"
#include "ObjectManager.h"
#include "ServerInfo.h"
#include "SkillManager.h"
#include "Util.h"

CAttack gAttack;

CAttack::CAttack()
{

}

CAttack::~CAttack()
{

}

bool CAttack::Attack(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, bool send, int damage, int count)
{
#pragma region ATTACK_CHECK

	if (lpObj->Index == lpTarget->Index)
	{
		return false;
	}

	if (lpObj->Type == OBJECT_USER && gObjIsConnectedGP(lpObj->Index) == 0)
	{
		return false;
	}

	if (lpTarget->Type == OBJECT_USER && gObjIsConnectedGP(lpTarget->Index) == 0)
	{
		return false;
	}

	if (lpObj->Map != lpTarget->Map || lpObj->Teleport != 0)
	{
		return false;
	}

	if (gMap[lpObj->Map].CheckAttr(lpObj->X, lpObj->Y, 1) != 0 || gMap[lpTarget->Map].CheckAttr(lpTarget->X, lpTarget->Y, 1) != 0)
	{
		return false;
	}

	if (lpObj->GuildNumber > 0 && lpObj->Guild != 0 && lpObj->Guild->WarState != GUILD_WAR_STATE_NONE)
	{
		if (lpObj->Guild->WarType == GUILD_WAR_TYPE_SOCCER && gBattleSoccer.GetBattleSoccerGoalMove(0) == 0)
		{
			return true;
		}
	}

	if (lpTarget->Type == OBJECT_NPC || lpTarget->Live == 0 || lpTarget->State != OBJECT_PLAYING || lpTarget->Teleport != 0)
	{
		return false;
	}

	if (lpTarget->Type == OBJECT_MONSTER)
	{
		if (lpTarget->Class >= 100 && lpTarget->Class <= 110) // Trap
		{
			return false;
		}
	}

	if (lpObj->Type == OBJECT_USER && lpTarget->Type == OBJECT_USER)
	{
		if (gSafeZone.CheckSafeZone(lpTarget->Map, lpTarget->X, lpTarget->Y))
		{
			return false;
		}
	}

	if (lpObj->Type == OBJECT_USER && lpTarget->Type == OBJECT_MONSTER)
	{
		if (OBJECT_RANGE(lpObj->SummonIndex) != false)
		{
			if (lpObj->SummonIndex == lpTarget->Index)
			{
				return false;
			}
		}
	}

	int SummonIndex = lpObj->Index;

	if (lpObj->Type == OBJECT_MONSTER && OBJECT_RANGE(lpObj->SummonIndex) != false)
	{
		SummonIndex = lpObj->SummonIndex;
	}

	int SummonTargetIndex = lpTarget->Index;

	if (lpTarget->Type == OBJECT_MONSTER && OBJECT_RANGE(lpTarget->SummonIndex) != false)
	{
		SummonTargetIndex = lpTarget->SummonIndex;
	}

	if (this->CheckPlayerTarget(&gObj[SummonIndex], &gObj[SummonTargetIndex]) == false)
	{
		return false;
	}

#pragma endregion

#pragma region ATTACK_RETURN

	int skill = ((lpSkill == 0) ? SKILL_NONE : lpSkill->m_skill);

	if (damage == 0 && this->DecreaseArrow(lpObj) == false)
	{
		return false;
	}

	if (lpObj->Type == OBJECT_USER)
	{
		lpObj->HPAutoRecuperationTime = GetTickCount();

		lpObj->MPAutoRecuperationTime = GetTickCount();

		lpObj->BPAutoRecuperationTime = GetTickCount();
	}

	if (lpTarget->Type == OBJECT_USER)
	{
		lpTarget->HPAutoRecuperationTime = GetTickCount();

		lpTarget->MPAutoRecuperationTime = GetTickCount();

		lpTarget->BPAutoRecuperationTime = GetTickCount();
	}

	if (OBJECT_RANGE(lpObj->SummonIndex) != false)
	{
		gObjSummonSetEnemy(lpObj, lpTarget->Index);
	}

	if (lpTarget->Type == OBJECT_MONSTER)
	{
		if (lpTarget->Class == 200 && lpSkill == 0)
		{
			gObjMonsterStateProc(lpTarget, 6, lpObj->Index, 0);

			this->MissSend(lpObj, lpTarget, lpSkill, send, count);

			return true;
		}

		if (lpTarget->Class == 200 && lpSkill != 0)
		{
			gObjMonsterStateProc(lpTarget, 7, lpObj->Index, 0);

			this->MissSend(lpObj, lpTarget, lpSkill, send, count);

			return true;
		}

		if (gEffectManager.CheckEffect(lpTarget, EFFECT_MONSTER_PHYSI_DAMAGE_IMMUNITY) != 0 && lpSkill == 0)
		{
			return false;
		}

		if (gEffectManager.CheckEffect(lpTarget, EFFECT_MONSTER_MAGIC_DAMAGE_IMMUNITY) != 0 && lpSkill != 0)
		{
			return false;
		}
	}

#pragma endregion

#pragma region DAMAGE_CALC

	BYTE miss = 0;

	WORD effect = DAMAGE_TYPE_NORMAL;

	if (damage == 0)
	{
		if (this->MissCheck(lpObj, lpTarget, lpSkill, send, count, &miss) == false)
		{
			return true;
		}

		int defense = this->GetTargetDefense(lpObj, lpTarget, &effect);

		if ((lpObj->Class == CLASS_DW || lpObj->Class == CLASS_MG) && skill != SKILL_NONE && skill != SKILL_FALLING_SLASH && skill != SKILL_LUNGE && skill != SKILL_UPPERCUT && skill != SKILL_CYCLONE && skill != SKILL_SLASH && skill != SKILL_TWISTING_SLASH && skill != SKILL_IMPALE && skill != SKILL_FIRE_SLASH && skill != SKILL_POWER_SLASH)
		{
			damage = this->GetAttackDamageWizard(lpObj, lpTarget, lpSkill, &effect, defense);
		}
		else
		{
			damage = this->GetAttackDamage(lpObj, lpTarget, lpSkill, &effect, defense);
		}

		if (damage > 0)
		{
			this->WeaponDurabilityDown(lpObj, lpTarget);
		}

		if (miss != 0)
		{
			damage = (damage * 30) / 100;
		}

		for (int n = 0; n < MAX_DAMAGE_REDUCTION; n++)
		{
			damage -= (damage * lpTarget->DamageReduction[n]) / 100;
		}

		if (lpTarget->EffectOption.AddDamageReduction > 0)
		{
			damage -= (damage * lpTarget->EffectOption.AddDamageReduction) / 100;
		}

		if ((GetTickCount() - lpTarget->ShieldDamageReductionTime) < ((DWORD)(gServerInfo.m_DefenseTime * 1000)))
		{
			damage -= (damage * lpTarget->ShieldDamageReduction) / 100;
		}

		this->WingSprite(lpObj, lpTarget, &damage);

		this->HelperSprite(lpObj, lpTarget, &damage);

		int MinDamage = (lpObj->Level) / 10;

		MinDamage = ((MinDamage < 1) ? 1 : MinDamage);

		damage = ((damage < MinDamage) ? MinDamage : damage);

		this->DamageSprite(lpTarget, damage);

		if (skill == SKILL_FALLING_SLASH || skill == SKILL_LUNGE || skill == SKILL_UPPERCUT || skill == SKILL_CYCLONE || skill == SKILL_SLASH || skill == SKILL_TWISTING_SLASH || skill == SKILL_RAGEFUL_BLOW || skill == SKILL_DEATH_STAB || skill == SKILL_IMPALE || skill == SKILL_FIRE_BREATH || skill == SKILL_ICE_ARROW || skill == SKILL_PENETRATION || skill == SKILL_FIRE_SLASH || skill == SKILL_POWER_SLASH)
		{
			if (skill != SKILL_IMPALE || lpObj->Inventory[8].m_Index == GET_ITEM(13, 2) || lpObj->Inventory[8].m_Index == GET_ITEM(13, 3))
			{
				damage = (damage * lpObj->DamageMultiplierRate) / 100;
			}
		}
	}
	else
	{
		effect = DAMAGE_TYPE_REFLECT;

		damage = (damage * ((lpObj->Type == OBJECT_USER && lpTarget->Type == OBJECT_USER) ? gServerInfo.m_ReflectDamageRatePvP : gServerInfo.m_ReflectDamageRatePvM)) / 100;
	}

#pragma endregion

#pragma region DAMAGE_CONFIG

	if (lpObj->Type == OBJECT_USER)
	{
		if (lpTarget->Type == OBJECT_USER)
		{
			damage = (damage * gServerInfo.m_GeneralDamageRatePvP) / 100;

			damage = (damage * gServerInfo.m_DamageRatePvP[lpObj->Class]) / 100;

			damage = (damage * gServerInfo.m_DamageRateTo[lpObj->Class][lpTarget->Class]) / 100;
		}
		else
		{
			damage = (damage * gServerInfo.m_GeneralDamageRatePvM) / 100;

			damage = (damage * gServerInfo.m_DamageRatePvM[lpObj->Class]) / 100;
		}
	}

#pragma endregion

#pragma region DAMAGE_FINISH

	damage = ((damage < 0) ? 0 : damage);

#pragma endregion

#pragma region APPLY_EFFECT

	if (lpObj->Type == OBJECT_USER && effect != DAMAGE_TYPE_REFLECT)
	{
		if ((GetLargeRand() % 100) < lpObj->OffensiveFullHPRestoreRate)
		{
			lpObj->Life = lpObj->MaxLife + lpObj->AddLife;

			GCLifeSend(lpObj->Index, 0xFF, (int)lpObj->Life);
		}

		if ((GetLargeRand() % 100) < lpObj->OffensiveFullMPRestoreRate)
		{
			lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;

			GCManaSend(lpObj->Index, 0xFF, (int)lpObj->Mana, lpObj->BP);
		}

		if ((GetLargeRand() % 100) < lpObj->OffensiveFullBPRestoreRate)
		{
			lpObj->BP = lpObj->MaxBP + lpObj->AddBP;

			GCManaSend(lpObj->Index, 0xFF, (int)lpObj->Mana, lpObj->BP);
		}

		if (lpObj->Inventory[0].m_Index == GET_ITEM(2, 4) || lpObj->Inventory[1].m_Index == GET_ITEM(2, 4) // Crystal Morning Star
			|| lpObj->Inventory[0].m_Index == GET_ITEM(2, 5) || lpObj->Inventory[1].m_Index == GET_ITEM(2, 5)) // Crystal Sword
		{
			if (rand() % 20 == 0)
			{
				if (this->ApplySkillEffect(lpObj, lpTarget, SKILL_ICE, damage))
				{
					gSkillManager.GCSkillAttackSend(lpObj, SKILL_ICE, lpTarget->Index, 0);
				}
			}
		}
	}

	if (lpSkill != 0 && count <= 1)
	{
		if (this->ApplySkillEffect(lpObj, lpTarget, lpSkill->m_index, damage) == false)
		{
			if (send != false)
			{
				gSkillManager.GCSkillAttackSend(lpObj, lpSkill->m_index, lpTarget->Index, 0);
			}
		}
		else
		{
			if (send != false)
			{
				gSkillManager.GCSkillAttackSend(lpObj, lpSkill->m_index, lpTarget->Index, 1);
			}
		}
	}

#pragma endregion

#pragma region DAMAGE_APPLY

	if (lpTarget->Life < damage)
	{
		lpTarget->Life = 0;
	}
	else
	{
		lpTarget->Life -= damage;
	}

	if (lpTarget->Type == OBJECT_MONSTER)
	{
		lpTarget->LastAttackerID = lpObj->Index;

		gObjAddMsgSendDelay(lpTarget, 0, lpObj->Index, 100, 0);
	}

#pragma endregion

#pragma region CHECK_SELF_DEFENSE

	if (damage > 0)
	{
		if (lpObj->Type == OBJECT_USER && lpTarget->Type == OBJECT_USER && lpObj->Index != lpTarget->Index)
		{
			bool CheckSelfDefense = true;

			if (effect == DAMAGE_TYPE_REFLECT)
			{
				CheckSelfDefense = false;
			}

			if (gGuild.GuildWarStateCheck(lpObj, lpTarget) != false)
			{
				CheckSelfDefense = false;
			}

			if (gMapManager.GetMapNonOutlaw(lpObj->Map) != false)
			{
				CheckSelfDefense = false;
			}

			if (CheckSelfDefense != false)
			{
				gObjCheckSelfDefense(lpObj, lpTarget->Index);
			}
		}

		this->ArmorDurabilityDown(lpObj, lpTarget);
	}

	lpObj->Rest = 0;

#pragma endregion

#pragma region ATTACK_FINISH

	BYTE flag = 0;

	if (damage > 0)
	{
		if (lpTarget->Type == OBJECT_USER)
		{
			if (effect != DAMAGE_TYPE_REFLECT)
			{
				if (lpTarget->DamageReflect > 0)
				{
					gObjAddMsgSendDelay(lpTarget, 10, lpObj->Index, 10, ((damage * lpTarget->DamageReflect) / 100));
				}

				if ((GetLargeRand() % 100) < lpTarget->DefensiveFullHPRestoreRate)
				{
					lpTarget->Life = lpTarget->MaxLife + lpTarget->AddLife;

					GCLifeSend(lpTarget->Index, 0xFF, (int)lpTarget->Life);
				}

				if ((GetLargeRand() % 100) < lpTarget->DefensiveFullMPRestoreRate)
				{
					lpTarget->Mana = lpTarget->MaxMana + lpTarget->AddMana;

					GCManaSend(lpTarget->Index, 0xFF, (int)lpTarget->Mana, lpTarget->BP);
				}

				if ((GetLargeRand() % 100) < lpTarget->DefensiveFullBPRestoreRate)
				{
					lpTarget->BP = lpTarget->MaxBP + lpTarget->AddBP;

					GCManaSend(lpTarget->Index, 0xFF, (int)lpTarget->Mana, lpTarget->BP);
				}
			}

			if ((GetLargeRand() % 100) < gServerInfo.m_DamageStuckRate[lpTarget->Class])
			{
				flag = 1;

				if (lpTarget->Inventory[8].IsItem())
				{
					// Uniria
					if (lpTarget->Inventory[8].m_Index == GET_ITEM(13, 2) && gServerInfo.m_PetUniriaEnableStuck == 0)
					{
						flag = 0;
					}
					// Dinorant
					else if (lpTarget->Inventory[8].m_Index == GET_ITEM(13, 3) && gServerInfo.m_PetDinorantEnableStuck == 0)
					{
						flag = 0;
					}
				}
			}
		}

		gObjectManager.CharacterLifeCheck(lpObj, lpTarget, damage, 0, flag, effect, skill);
	}
	else
	{
		GCDamageSend(lpObj->Index, lpTarget->Index, 0, 0, effect);
	}

	if (lpObj->Type == OBJECT_USER && lpObj->Life <= 0 && lpObj->CheckLifeTime <= 0)
	{
		lpObj->AttackObj = lpTarget;

		lpObj->AttackerKilled = ((lpTarget->Type == OBJECT_USER) ? 1 : 0);

		lpObj->CheckLifeTime = 3;
	}

#pragma endregion

	return true;
}

bool CAttack::DecreaseArrow(LPOBJ lpObj)
{
	if (lpObj->Type != OBJECT_USER || lpObj->Class != CLASS_FE)
	{
		return true;
	}

	if (lpObj->Inventory[0].m_Index >= GET_ITEM(4, 0) && lpObj->Inventory[0].m_Index < GET_ITEM(5, 0) && lpObj->Inventory[0].m_Index != GET_ITEM(4, 15) && lpObj->Inventory[0].m_Slot == 0)
	{
		if (lpObj->Inventory[1].m_Index != GET_ITEM(4, 7) || lpObj->Inventory[1].m_Durability < 1)
		{
			return false;
		}
		else if (!gServerInfo.m_ArrowsInfiniteDurability)
		{
			gItemManager.DecreaseItemDur(lpObj, 1, 1);
		}
	}

	if (lpObj->Inventory[1].m_Index >= GET_ITEM(4, 0) && lpObj->Inventory[1].m_Index < GET_ITEM(5, 0) && lpObj->Inventory[1].m_Index != GET_ITEM(4, 7) && lpObj->Inventory[1].m_Slot == 1)
	{
		if (lpObj->Inventory[0].m_Index != GET_ITEM(4, 15) || lpObj->Inventory[0].m_Durability < 1)
		{
			return false;
		}
		else if (!gServerInfo.m_ArrowsInfiniteDurability)
		{
			gItemManager.DecreaseItemDur(lpObj, 0, 1);
		}
	}

	return true;
}

void CAttack::WingSprite(LPOBJ lpObj, LPOBJ lpTarget, int* damage)
{
	if (lpObj != 0 && lpObj->Type == OBJECT_USER)
	{
		CItem* lpItem = &lpObj->Inventory[7];

		if (lpItem->IsItem() != false && lpItem->m_Durability > 0)
		{
			if (lpObj->Class == CLASS_DW || lpObj->Class == CLASS_FE)
			{
				lpObj->Life -= 1;
			}
			else
			{
				lpObj->Life -= 3;
			}

			GCLifeSend(lpObj->Index, 0xFF, (int)lpObj->Life);

			if ((lpItem->m_Index >= GET_ITEM(12, 0) && lpItem->m_Index <= GET_ITEM(12, 2))) // 1st wing
			{
				(*damage) = ((*damage) * (112 + (lpItem->m_Level * 2))) / 100;
			}
			else if ((lpItem->m_Index >= GET_ITEM(12, 3) && lpItem->m_Index <= GET_ITEM(12, 6))) // 2sd wing
			{
				(*damage) = ((*damage) * (132 + (lpItem->m_Level * 1))) / 100;
			}
			else if (gCustomWing.CheckCustomWingByItem(lpItem->m_Index) != 0)
			{
				(*damage) = ((*damage) * (100 + gCustomWing.GetCustomWingIncDamage(lpItem->m_Index, lpItem->m_Level))) / 100;
			}
		}
	}

	if (lpTarget != 0 && lpTarget->Type == OBJECT_USER)
	{
		CItem* lpItem = &lpTarget->Inventory[7];

		if (lpItem->IsItem() != false && lpItem->m_Durability > 0)
		{
			if ((lpItem->m_Index >= GET_ITEM(12, 0) && lpItem->m_Index <= GET_ITEM(12, 2))) // 1st wing
			{
				(*damage) = ((*damage) * (88 - (lpItem->m_Level * 2))) / 100;
			}
			else if ((lpItem->m_Index >= GET_ITEM(12, 3) && lpItem->m_Index <= GET_ITEM(12, 6))) // 2sd wing
			{
				(*damage) = ((*damage) * (75 - (lpItem->m_Level * 2))) / 100;
			}
			else if (gCustomWing.CheckCustomWingByItem(lpItem->m_Index) != 0)
			{
				(*damage) = ((*damage) * (100 - gCustomWing.GetCustomWingDecDamage(lpItem->m_Index, lpItem->m_Level))) / 100;
			}
		}
	}
}

void CAttack::HelperSprite(LPOBJ lpObj, LPOBJ lpTarget, int* damage)
{
	if (lpObj != 0 && lpObj->Type == OBJECT_USER)
	{
		CItem* lpItem = &lpObj->Inventory[8];

		if (lpItem->IsItem() != false && lpItem->m_Durability > 0)
		{
			if (lpItem->m_Index == GET_ITEM(13, 1)) // Satan
			{
				lpObj->Life -= 3;

				GCLifeSend(lpObj->Index, 0xFF, (int)lpObj->Life);

				(*damage) = ((*damage) * (100 + gServerInfo.m_SatanIncDamageConstA)) / 100;
			}
			else if (lpItem->m_Index == GET_ITEM(13, 3)) // Dinorant
			{
				lpObj->Life -= 1;

				GCLifeSend(lpObj->Index, 0xFF, (int)lpObj->Life);

				(*damage) = ((*damage) * (100 + gServerInfo.m_DinorantIncDamageConstA)) / 100;
			}
		}
	}

	if (lpTarget != 0 && lpTarget->Type == OBJECT_USER)
	{
		CItem* lpItem = &lpTarget->Inventory[8];

		if (lpItem->IsItem() != false && lpItem->m_Durability > 0)
		{
			if (lpItem->m_Index == GET_ITEM(13, 0)) // Angel
			{
				(*damage) = ((*damage) * (100 - gServerInfo.m_AngelDecDamageConstA)) / 100;
			}
			else if (lpItem->m_Index == GET_ITEM(13, 3)) // Dinorant
			{
				(*damage) = ((*damage) * (100 - gServerInfo.m_DinorantDecDamageConstA - (((lpItem->m_ExceOption & 1) == 0) ? 0 : gServerInfo.m_DinorantDecDamageConstB))) / 100;
			}
		}
	}
}

void CAttack::DamageSprite(LPOBJ lpObj, int damage)
{
	if (lpObj->Type != OBJECT_USER)
	{
		return;
	}

	CItem* lpItem = &lpObj->Inventory[8];

	if (lpItem->IsItem() == false)
	{
		return;
	}

	float DurabilityValue = (1.0f / gServerInfo.m_GuardianDurabilityRate) * 100;

	DurabilityValue = (DurabilityValue / lpObj->GuardianDurabilityRate) * 100;

	if (lpItem->m_Index == GET_ITEM(13, 0)) // Angel
	{
		lpItem->m_Durability -= (damage * (3.0f * DurabilityValue)) / 100;
	}
	else if (lpItem->m_Index == GET_ITEM(13, 1)) // Satan
	{
		lpItem->m_Durability -= (damage * (2.0f * DurabilityValue)) / 100;
	}
	else if (lpItem->m_Index == GET_ITEM(13, 2)) // Uniria
	{
		lpItem->m_Durability -= (damage * (1.0f * DurabilityValue)) / 100;
	}
	else if (lpItem->m_Index == GET_ITEM(13, 3)) // Dinorant
	{
		lpItem->m_Durability -= (damage * (1.0f * DurabilityValue)) / 100;
	}
	else
	{
		return;
	}

	gItemManager.GCItemDurSend(lpObj->Index, 8, (BYTE)lpItem->m_Durability, 0);

	if (lpItem->m_Durability < 1)
	{
		if (lpObj->Map == MAP_ICARUS && lpObj->Inventory[7].IsItem() == 0 && lpItem->m_Index == GET_ITEM(13, 3))
		{
			gObjMoveGate(lpObj->Index, 22);
		}

		gItemManager.InventoryDelItem(lpObj->Index, 8);

		gItemManager.GCItemDeleteSend(lpObj->Index, 8, 0);

		gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);

		gItemManager.GCItemChangeSend(lpObj->Index, 8);
	}
}

void CAttack::WeaponDurabilityDown(LPOBJ lpObj, LPOBJ lpTarget)
{
	if (lpObj->Type != OBJECT_USER)
	{
		return;
	}

	for (int n = 0; n < 2; n++)
	{
		if (lpObj->Inventory[n].IsItem() != false)
		{
			bool result = false;

			switch ((lpObj->Inventory[n].m_Index / MAX_ITEM_TYPE))
			{
				case 0:
				{
					result = lpObj->Inventory[n].WeaponDurabilityDown(lpObj->Index, lpTarget->Defense, 0);

					break;
				}

				case 1:
				{
					result = lpObj->Inventory[n].WeaponDurabilityDown(lpObj->Index, lpTarget->Defense, 0);

					break;
				}

				case 2:
				{
					result = lpObj->Inventory[n].WeaponDurabilityDown(lpObj->Index, lpTarget->Defense, 0);

					break;
				}

				case 3:
				{
					result = lpObj->Inventory[n].WeaponDurabilityDown(lpObj->Index, lpTarget->Defense, 0);

					break;
				}

				case 4:
				{
					result = lpObj->Inventory[n].WeaponDurabilityDown(lpObj->Index, lpTarget->Defense, 1);

					break;
				}

				case 5:
				{
					result = lpObj->Inventory[n].WeaponDurabilityDown(lpObj->Index, lpTarget->Defense, ((lpObj->Inventory[n].m_Slot == 0) ? 2 : 3));

					break;
				}
			}

			if (result != false)
			{
				gItemManager.GCItemDurSend(lpObj->Index, n, (BYTE)lpObj->Inventory[n].m_Durability, 0);
			}
		}
	}
}

void CAttack::ArmorDurabilityDown(LPOBJ lpObj, LPOBJ lpTarget)
{
	if (lpTarget->Type != OBJECT_USER)
	{
		return;
	}

	int slot = 1 + (GetLargeRand() % 6);

	if (lpTarget->Inventory[slot].IsItem() != false)
	{
		if (slot != 1 || (lpTarget->Inventory[slot].m_Index >= GET_ITEM(6, 0) && lpTarget->Inventory[slot].m_Index < GET_ITEM(7, 0)))
		{
			if (lpTarget->Inventory[slot].ArmorDurabilityDown(lpTarget->Index, lpObj->PhysiDamageMin) != false)
			{
				gItemManager.GCItemDurSend(lpTarget->Index, slot, (BYTE)lpTarget->Inventory[slot].m_Durability, 0);
			}
		}
	}
}

bool CAttack::CheckPlayerTarget(LPOBJ lpObj, LPOBJ lpTarget)
{
	if (lpObj->Type != OBJECT_USER || lpTarget->Type != OBJECT_USER)
	{
		return true;
	}

	if (lpTarget->Authority == 32)
	{
		return false;
	}

	if (gParty.IsMember(lpObj->PartyNumber, lpTarget->Index) != false)
	{
		return false;
	}

	if (lpObj->Guild != 0 && lpObj->Guild->WarState != GUILD_WAR_STATE_NONE)
	{
		if (lpTarget->Guild == 0)
		{
			return false;
		}

		if (lpObj->Guild->Number == lpTarget->Guild->Number)
		{
			return false;
		}

		if (lpTarget->Guild->WarState == GUILD_WAR_STATE_NONE)
		{
			return false;
		}

		if (strcmp(lpObj->Guild->TargetGuildNode->Name, lpTarget->Guild->Name) != 0)
		{
			return false;
		}

		if (lpTarget->Guild->WarType == GUILD_WAR_TYPE_SOCCER && lpTarget->Map != MAP_ARENA)
		{
			return false;
		}
	}

	if (lpTarget->Guild != 0 && lpTarget->Guild->WarState != GUILD_WAR_STATE_NONE)
	{
		if (lpObj->Guild == 0)
		{
			return false;
		}

		if (lpTarget->Guild->Number == lpObj->Guild->Number)
		{
			return false;
		}

		if (lpObj->Guild->WarState == GUILD_WAR_STATE_NONE)
		{
			return false;
		}

		if (strcmp(lpTarget->Guild->TargetGuildNode->Name, lpObj->Guild->Name) != 0)
		{
			return false;
		}

		if (lpObj->Guild->WarType == GUILD_WAR_TYPE_SOCCER && lpObj->Map != MAP_ARENA)
		{
			return false;
		}
	}

	if (DS_MAP_RANGE(lpObj->Map) != 0 && DS_MAP_RANGE(lpTarget->Map) != 0)
	{
		return false;
	}

	if (BC_MAP_RANGE(lpObj->Map) != 0 && BC_MAP_RANGE(lpTarget->Map) != 0)
	{
		return false;
	}

	if (lpObj->Level <= 5 || lpTarget->Level <= 5)
	{
		return false;
	}

	if (gMapManager.GetMapNonPK(lpTarget->Map) != 0)
	{
		return false;
	}

	return true;
}

void CAttack::MissSend(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int send, int count)
{
	GCDamageSend(lpObj->Index, lpTarget->Index, 0, 0, 0);

	if (send != 0 && lpSkill != 0)
	{
		gSkillManager.GCSkillAttackSend(lpObj, lpSkill->m_index, lpTarget->Index, 0);
	}
}

bool CAttack::MissCheck(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int send, int count, BYTE* miss)
{
	int AttackSuccessRate = lpObj->AttackSuccessRate;

	AttackSuccessRate = ((AttackSuccessRate < 0) ? 0 : AttackSuccessRate);

	int DefenseSuccessRate = lpTarget->DefenseSuccessRate;

	DefenseSuccessRate = ((DefenseSuccessRate < 0) ? 0 : DefenseSuccessRate);

	if (AttackSuccessRate < DefenseSuccessRate)
	{
		(*miss) = 1;

		if ((GetLargeRand() % 100) >= 5)
		{
			this->MissSend(lpObj, lpTarget, lpSkill, send, count);

			return false;
		}
	}
	else
	{
		(*miss) = 0;

		if ((GetLargeRand() % ((AttackSuccessRate == 0) ? 1 : AttackSuccessRate)) < DefenseSuccessRate)
		{
			this->MissSend(lpObj, lpTarget, lpSkill, send, count);

			return false;
		}
	}

	return true;
}

bool CAttack::ApplySkillEffect(LPOBJ lpObj, LPOBJ lpTarget, int skill, int damage)
{
	if (gSkillManager.GetSkillType(skill) != -1 && gObjCheckResistance(lpTarget, gSkillManager.GetSkillType(skill)) != false)
	{
		return false;
	}

	switch (skill)
	{
		case SKILL_POISON:
		{
			gEffectManager.AddEffect(lpTarget, 0, gSkillManager.GetSkillEffect(skill), 20, lpObj->Index, 2, 3, 0);

			break;
		}

		case SKILL_LIGHTNING:
		{
			gObjAddMsgSendDelay(lpTarget, 2, lpObj->Index, 150, 0);

			break;
		}

		case SKILL_ICE:
		{
			gEffectManager.AddEffect(lpTarget, 0, gSkillManager.GetSkillEffect(skill), 10, 0, 0, 0, 0);

			break;
		}

		case SKILL_FALLING_SLASH:
		{
			gObjAddMsgSendDelay(lpTarget, 2, lpObj->Index, 150, 0);

			break;
		}

		case SKILL_LUNGE:
		{
			gObjAddMsgSendDelay(lpTarget, 2, lpObj->Index, 150, 0);

			break;
		}

		case SKILL_UPPERCUT:
		{
			gObjAddMsgSendDelay(lpTarget, 2, lpObj->Index, 150, 0);

			break;
		}

		case SKILL_CYCLONE:
		{
			gObjAddMsgSendDelay(lpTarget, 2, lpObj->Index, 150, 0);

			break;
		}

		case SKILL_SLASH:
		{
			gObjAddMsgSendDelay(lpTarget, 2, lpObj->Index, 150, 0);

			break;
		}

		case SKILL_ICE_ARROW:
		{
			gEffectManager.AddEffect(lpTarget, 0, gSkillManager.GetSkillEffect(skill), 7, 0, 0, 0, 0);

			break;
		}

		case SKILL_FIRE_SLASH:
		{
			gSkillManager.ApplyFireSlashEffect(lpObj, lpTarget, skill, damage);

			break;
		}
	}

	return true;
}

int CAttack::GetTargetDefense(LPOBJ lpObj, LPOBJ lpTarget, WORD* effect)
{
	int defense = lpTarget->Defense;

	defense += lpTarget->EffectOption.AddDefense;

	if (lpTarget->EffectOption.DivDefense > 0)
	{
		defense -= (defense * lpTarget->EffectOption.DivDefense) / 100;
	}

	if (lpTarget->Type == OBJECT_USER)
	{
		defense = (defense * 50) / 100;
	}

	if ((GetLargeRand() % 100) < ((lpObj->IgnoreDefenseRate) - lpTarget->ResistIgnoreDefenseRate))
	{
		(*effect) = DAMAGE_TYPE_IGNORE;

		defense = 0;
	}

	defense = ((defense < 0) ? 0 : defense);

	return defense;
}

int CAttack::GetAttackDamage(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, WORD* effect, int TargetDefense)
{
	CItem* Right = &lpObj->Inventory[0];

	CItem* Left = &lpObj->Inventory[1];

	int damage = 0;

	int DamageMin = 0;

	int DamageMax = 0;

	int SkillDamageMin = 0;

	int SkillDamageMax = 0;

	bool DualHandWeapon = false;

	if (lpObj->Type == OBJECT_MONSTER || lpObj->Type == OBJECT_NPC)
	{
		DamageMin = lpObj->PhysiDamageMin;

		DamageMax = lpObj->PhysiDamageMax;

		DamageMin += lpObj->EffectOption.AddPhysiDamage;

		DamageMax += lpObj->EffectOption.AddPhysiDamage;

		int range = (DamageMax - DamageMin);

		range = ((range < 1) ? 1 : range);

		damage = DamageMin + (GetLargeRand() % range);
	}
	else
	{
		if (lpObj->Class == CLASS_DK || lpObj->Class == CLASS_MG)
		{
			if (Right->m_Index >= GET_ITEM(0, 0) && Right->m_Index < GET_ITEM(4, 0) && Left->m_Index >= GET_ITEM(0, 0) && Left->m_Index < GET_ITEM(4, 0))
			{
				if (Right->m_IsValidItem != false && Left->m_IsValidItem != false)
				{
					DualHandWeapon = true;
				}
			}
		}

		if (lpSkill != 0)
		{
			SkillDamageMin = lpSkill->m_DamageMin;

			SkillDamageMax = lpSkill->m_DamageMax;

			SkillDamageMin += lpObj->SkillDamageBonus;

			SkillDamageMax += lpObj->SkillDamageBonus;

			int type = gSkillManager.GetSkillType(lpSkill->m_index);

			if (CHECK_RANGE(type, MAX_RESISTANCE_TYPE) != false)
			{
				SkillDamageMin += lpObj->AddResistance[type];

				SkillDamageMax += lpObj->AddResistance[type];
			}
		}

		if (DualHandWeapon != false)
		{
			DamageMin = lpObj->PhysiDamageMinRight + lpObj->PhysiDamageMinLeft + SkillDamageMin;

			DamageMax = lpObj->PhysiDamageMaxRight + lpObj->PhysiDamageMaxLeft + SkillDamageMax;
		}
		else if ((Right->m_Index >= GET_ITEM(0, 0) && Right->m_Index < GET_ITEM(4, 0)) || (Right->m_Index >= GET_ITEM(5, 0) && Right->m_Index < GET_ITEM(6, 0)))
		{
			DamageMin = lpObj->PhysiDamageMinRight + SkillDamageMin;

			DamageMax = lpObj->PhysiDamageMaxRight + SkillDamageMax;
		}
		else if (Right->m_Index >= GET_ITEM(4, 0) && Right->m_Index < GET_ITEM(5, 0) && Right->m_Index != GET_ITEM(4, 15) && Right->m_Slot == 0)
		{
			DamageMin = lpObj->PhysiDamageMinRight + SkillDamageMin;

			DamageMax = lpObj->PhysiDamageMaxRight + SkillDamageMax;
		}
		else if (Left->m_Index >= GET_ITEM(4, 0) && Left->m_Index < GET_ITEM(5, 0) && Left->m_Index != GET_ITEM(4, 7) && Left->m_Slot == 1)
		{
			DamageMin = lpObj->PhysiDamageMinLeft + SkillDamageMin;

			DamageMax = lpObj->PhysiDamageMaxLeft + SkillDamageMax;
		}
		else
		{
			DamageMin = lpObj->PhysiDamageMinLeft + SkillDamageMin;

			DamageMax = lpObj->PhysiDamageMaxLeft + SkillDamageMax;
		}

		DamageMin += lpObj->EffectOption.AddPhysiDamage;

		DamageMax += lpObj->EffectOption.AddPhysiDamage;

		int range = (DamageMax - DamageMin);

		range = ((range < 1) ? 1 : range);

		damage = DamageMin + (GetLargeRand() % range);

		if ((GetLargeRand() % 100) < ((lpObj->CriticalDamageRate) - lpTarget->ResistCriticalDamageRate))
		{
			(*effect) = DAMAGE_TYPE_CRITICAL;

			damage = DamageMax;

			damage += lpObj->CriticalDamage;

			damage += lpObj->EffectOption.AddCriticalDamage;
		}

		if ((GetLargeRand() % 100) < ((lpObj->ExcellentDamageRate) - lpTarget->ResistExcellentDamageRate))
		{
			(*effect) = DAMAGE_TYPE_EXCELLENT;

			damage = (DamageMax * 120) / 100;

			damage += lpObj->ExcellentDamage;
		}
	}

	damage -= TargetDefense;

	damage = ((damage < 0) ? 0 : damage);

	return damage;
}

int CAttack::GetAttackDamageWizard(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, WORD* effect, int TargetDefense)
{
	CItem* Right = &lpObj->Inventory[0];

	CItem* Left = &lpObj->Inventory[1];

	int DamageMin = lpObj->MagicDamageMin;

	int DamageMax = lpObj->MagicDamageMax;

	DamageMin += lpSkill->m_DamageMin;

	DamageMax += lpSkill->m_DamageMax;

	DamageMin += lpObj->SkillDamageBonus;

	DamageMax += lpObj->SkillDamageBonus;

	DamageMin += lpObj->EffectOption.AddMagicDamage;

	DamageMax += lpObj->EffectOption.AddMagicDamage;

	int type = gSkillManager.GetSkillType(lpSkill->m_index);

	if (CHECK_RANGE(type, MAX_RESISTANCE_TYPE) != false)
	{
		DamageMin += lpObj->AddResistance[type];

		DamageMax += lpObj->AddResistance[type];
	}

	if (Right->IsItem() != false && Right->m_IsValidItem != false && ((Right->m_Index >= GET_ITEM(0, 0) && Right->m_Index < GET_ITEM(1, 0)) || (Right->m_Index >= GET_ITEM(5, 0) && Right->m_Index < GET_ITEM(6, 0))))
	{
		int rise = (int)(((Right->m_MagicDamageRate / 2) + (Right->m_Level * 2)) * Right->m_CurrentDurabilityState);

		DamageMin += (DamageMin * rise) / 100;

		DamageMax += (DamageMax * rise) / 100;
	}

	int range = (DamageMax - DamageMin);

	range = ((range < 1) ? 1 : range);

	int damage = DamageMin + (GetLargeRand() % range);

	if ((GetLargeRand() % 100) < ((lpObj->CriticalDamageRate) - lpTarget->ResistCriticalDamageRate))
	{
		(*effect) = DAMAGE_TYPE_CRITICAL;

		damage = DamageMax;

		damage += lpObj->CriticalDamage;

		damage += lpObj->EffectOption.AddCriticalDamage;
	}

	if ((GetLargeRand() % 100) < ((lpObj->ExcellentDamageRate) - lpTarget->ResistExcellentDamageRate))
	{
		(*effect) = DAMAGE_TYPE_EXCELLENT;

		damage = (DamageMax * 120) / 100;

		damage += lpObj->ExcellentDamage;
	}

	damage -= TargetDefense;

	damage = ((damage < 0) ? 0 : damage);

	return damage;
}

void CAttack::GetPreviewDefense(LPOBJ lpObj, DWORD* defense)
{
	(*defense) = lpObj->Defense;

	(*defense) += lpObj->EffectOption.AddDefense;

	(*defense) -= ((*defense) * lpObj->EffectOption.DivDefense) / 100;
}

void CAttack::GetPreviewPhysiDamage(LPOBJ lpObj, DWORD* DamageMin, DWORD* DamageMax)
{
	CItem* Right = &lpObj->Inventory[0];

	CItem* Left = &lpObj->Inventory[1];

	bool DualHandWeapon = false;

	if (lpObj->Class == CLASS_DK || lpObj->Class == CLASS_MG)
	{
		if (Right->m_Index >= GET_ITEM(0, 0) && Right->m_Index < GET_ITEM(4, 0) && Left->m_Index >= GET_ITEM(0, 0) && Left->m_Index < GET_ITEM(4, 0))
		{
			if (Right->m_IsValidItem != false && Left->m_IsValidItem != false)
			{
				DualHandWeapon = true;
			}
		}
	}

	if (DualHandWeapon != 0)
	{
		(*DamageMin) = lpObj->PhysiDamageMinRight + lpObj->PhysiDamageMinLeft;

		(*DamageMax) = lpObj->PhysiDamageMaxRight + lpObj->PhysiDamageMaxLeft;
	}
	else if ((Right->m_Index >= GET_ITEM(0, 0) && Right->m_Index < GET_ITEM(4, 0)) || (Right->m_Index >= GET_ITEM(5, 0) && Right->m_Index < GET_ITEM(6, 0)))
	{
		(*DamageMin) = lpObj->PhysiDamageMinRight;

		(*DamageMax) = lpObj->PhysiDamageMaxRight;
	}
	else if (Right->m_Index >= GET_ITEM(4, 0) && Right->m_Index < GET_ITEM(5, 0) && Right->m_Index != GET_ITEM(4, 15) && Right->m_Slot == 0)
	{
		(*DamageMin) = lpObj->PhysiDamageMinRight;

		(*DamageMax) = lpObj->PhysiDamageMaxRight;
	}
	else if (Left->m_Index >= GET_ITEM(4, 0) && Left->m_Index < GET_ITEM(5, 0) && Left->m_Index != GET_ITEM(4, 7) && Left->m_Slot == 1)
	{
		(*DamageMin) = lpObj->PhysiDamageMinLeft;

		(*DamageMax) = lpObj->PhysiDamageMaxLeft;
	}
	else
	{
		(*DamageMin) = lpObj->PhysiDamageMinLeft;

		(*DamageMax) = lpObj->PhysiDamageMaxLeft;
	}

	(*DamageMin) += lpObj->EffectOption.AddPhysiDamage;

	(*DamageMax) += lpObj->EffectOption.AddPhysiDamage;
}

void CAttack::GetPreviewMagicDamage(LPOBJ lpObj, DWORD* DamageMin, DWORD* DamageMax, DWORD* DamageRate)
{
	CItem* Right = &lpObj->Inventory[0];

	CItem* Left = &lpObj->Inventory[1];

	(*DamageMin) = lpObj->MagicDamageMin;

	(*DamageMax) = lpObj->MagicDamageMax;

	(*DamageMin) += lpObj->EffectOption.AddMagicDamage;

	(*DamageMax) += lpObj->EffectOption.AddMagicDamage;

	if (Right->IsItem() != false && Right->m_IsValidItem != false && ((Right->m_Index >= GET_ITEM(0, 0) && Right->m_Index < GET_ITEM(1, 0)) || (Right->m_Index >= GET_ITEM(5, 0) && Right->m_Index < GET_ITEM(6, 0))))
	{
		(*DamageRate) = (int)(((Right->m_MagicDamageRate / 2) + (Right->m_Level * 2)) * Right->m_CurrentDurabilityState);
	}
	else
	{
		(*DamageRate) = 0;
	}
}

void CAttack::CGAttackRecv(PMSG_ATTACK_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	int bIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]);

	if (OBJECT_RANGE(bIndex) == false)
	{
		return;
	}

	LPOBJ lpTarget = &gObj[bIndex];

	if (lpObj->Live == 0 || lpTarget->Live == 0)
	{
		return;
	}

	if (lpObj->Map != lpTarget->Map)
	{
		return;
	}

	if (gMap[lpObj->Map].CheckAttr(lpObj->X, lpObj->Y, 1) != false || gMap[lpTarget->Map].CheckAttr(lpTarget->X, lpTarget->Y, 1) != false)
	{
		return;
	}

	if (lpObj->Type == OBJECT_USER && sqrt(pow(((float)lpObj->X - (float)lpTarget->X), 2) + pow(((float)lpObj->Y - (float)lpTarget->Y), 2)) > ((lpObj->Class == CLASS_FE) ? 6 : 3))
	{
		return;
	}

	lpObj->Dir = lpMsg->dir;

	lpObj->MultiSkillIndex = 0;

	lpObj->MultiSkillCount = 0;

	GCActionSend(lpObj, lpMsg->action, aIndex, bIndex);

	this->Attack(lpObj, lpTarget, 0, 0, 0, 0);
}