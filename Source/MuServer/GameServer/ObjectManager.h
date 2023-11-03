#pragma once

#include "User.h"

class CObjectManager
{
public:

	CObjectManager();

	virtual ~CObjectManager();

	void ObjectMsgProc(LPOBJ lpObj);

	void ObjectSetStateCreate(int aIndex);

	void ObjectSetStateProc();

	void ObjectStateProc(LPOBJ lpObj, int MessageCode, int aIndex, int SubCode);

	void ObjectStateAttackProc(LPOBJ lpObj, int MessageCode, int aIndex, int SubCode1, int SubCode2);

	void ObjectMoveProc();

	void ObjectMonsterAndMsgProc();

	bool CharacterGameClose(int aIndex);

	void CharacterGameCloseSet(int aIndex, int type);

	void CharacterUpdateMapEffect(LPOBJ lpObj);

	bool CharacterGetRespawnLocation(LPOBJ lpObj);

	void CharacterCalcExperienceSplit(LPOBJ lpObj, LPOBJ lpMonster, int damage, int flag);

	void CharacterCalcExperienceAlone(LPOBJ lpObj, LPOBJ lpMonster, int damage, int flag, bool type, int AttackDamage);

	void CharacterCalcExperienceParty(LPOBJ lpObj, LPOBJ lpMonster, int damage, int flag);

	bool CharacterLevelUp(LPOBJ lpObj, DWORD AddExperience, int MaxLevelUp, int ExperienceType);

	bool CharacterLevelUpPointAdd(LPOBJ lpObj, int type, int amount);

	void CharacterMakePreviewCharSet(int aIndex);

	bool CharacterUseScroll(LPOBJ lpObj, CItem* lpItem);

	bool CharacterUsePotion(LPOBJ lpObj, CItem* lpItem);

	bool CharacterUsePortal(LPOBJ lpObj, CItem* lpItem);

	bool CharacterUseJewelOfBles(LPOBJ lpObj, int SourceSlot, int TargetSlot);

	bool CharacterUseJewelOfSoul(LPOBJ lpObj, int SourceSlot, int TargetSlot);

	bool CharacterUseJewelOfLife(LPOBJ lpObj, int SourceSlot, int TargetSlot);

	void CharacterMonsterDieHunt(LPOBJ lpObj, LPOBJ lpTarget);

	void CharacterAutoRecuperation(LPOBJ lpObj);

	void CharacterItemDurationDown(LPOBJ lpObj);

	void CharacterCalcBP(LPOBJ lpObj);

	void CharacterCalcAttribute(int aIndex);

	bool CharacterInfoSet(BYTE* aRecv, int aIndex);

	void CharacterLifeCheck(LPOBJ lpObj, LPOBJ lpTarget, int damage, int DamageType, int flag, int type, int skill);
};

extern CObjectManager gObjectManager;