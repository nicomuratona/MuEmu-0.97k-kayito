#pragma once

#include "ProtocolDefines.h"
#include "User.h"

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_ATTACK_RECV
{
	PBMSG_HEAD header; // C1:[PROTOCOL_CODE2]
	BYTE index[2];
	BYTE action;
	BYTE dir;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CAttack
{
public:

	CAttack();

	~CAttack();

	bool Attack(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, bool send, int damage, int count);

	bool DecreaseArrow(LPOBJ lpObj);

	void WingSprite(LPOBJ lpObj, LPOBJ lpTarget, int* damage);

	void HelperSprite(LPOBJ lpObj, LPOBJ lpTarget, int* damage);

	void DamageSprite(LPOBJ lpObj, int damage);

	void WeaponDurabilityDown(LPOBJ lpObj, LPOBJ lpTarget);

	void ArmorDurabilityDown(LPOBJ lpObj, LPOBJ lpTarget);

	bool CheckPlayerTarget(LPOBJ lpObj, LPOBJ lpTarget);

	void MissSend(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int send, int count);

	bool MissCheck(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int send, int count, BYTE* miss);

	bool ApplySkillEffect(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int damage);

	int GetTargetDefense(LPOBJ lpObj, LPOBJ lpTarget, WORD* effect);

	int GetAttackDamage(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, WORD* effect, int TargetDefense);

	int GetAttackDamageWizard(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, WORD* effect, int TargetDefense);

	void GetPreviewDefense(LPOBJ lpObj, DWORD* defense);

	void GetPreviewPhysiDamage(LPOBJ lpObj, DWORD* DamageMin, DWORD* DamageMax, DWORD* MulDamage, DWORD* DivDamage);

	void GetPreviewMagicDamage(LPOBJ lpObj, DWORD* DamageMin, DWORD* DamageMax, DWORD* MulDamage, DWORD* DivDamage, DWORD* DamageRate);

	void CGAttackRecv(PMSG_ATTACK_RECV* lpMsg, int aIndex);
};

extern CAttack gAttack;