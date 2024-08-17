#include "stdafx.h"
#include "EffectManager.h"
#include "ReadScript.h"
#include "ObjectManager.h"
#include "Util.h"

CEffectManager gEffectManager;

CEffectManager::CEffectManager()
{
	this->Init();
}

CEffectManager::~CEffectManager()
{

}

void CEffectManager::Init()
{
	for (int n = 0; n < MAX_EFFECT; n++)
	{
		this->m_EffectInfo[n].Index = -1;
	}
}

void CEffectManager::Load(char* path)
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

			EFFECT_INFO info;

			info.Index = lpReadScript->GetNumber();

			info.Group = lpReadScript->GetAsNumber();

			info.ItemIndex = lpReadScript->GetAsNumber();

			strcpy_s(info.Name, lpReadScript->GetAsString());

			info.Save = lpReadScript->GetAsNumber();

			info.Type = lpReadScript->GetAsNumber();

			info.Flag = lpReadScript->GetAsNumber();

			info.Count = lpReadScript->GetAsNumber();

			info.Value[0] = lpReadScript->GetAsNumber();

			info.Value[1] = lpReadScript->GetAsNumber();

			info.Value[2] = lpReadScript->GetAsNumber();

			info.Value[3] = lpReadScript->GetAsNumber();

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript->GetError());
	}

	delete lpReadScript;
}

void CEffectManager::SetInfo(EFFECT_INFO info)
{
	if (EFFECT_RANGE(info.Index) == false)
	{
		return;
	}

	this->m_EffectInfo[info.Index] = info;
}

EFFECT_INFO* CEffectManager::GetInfo(int index)
{
	if (EFFECT_RANGE(index) == false)
	{
		return 0;
	}

	if (this->m_EffectInfo[index].Index != index)
	{
		return 0;
	}

	return &this->m_EffectInfo[index];
}

EFFECT_INFO* CEffectManager::GetInfoByItem(int ItemIndex)
{
	for (int n = 0; n < MAX_EFFECT; n++)
	{
		EFFECT_INFO* lpInfo = this->GetInfo(n);

		if (lpInfo == 0)
		{
			continue;
		}

		if (lpInfo->ItemIndex == ItemIndex)
		{
			return lpInfo;
		}
	}

	return 0;
}

void CEffectManager::MainProc()
{
	for (int n = 0; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnected(n) == false)
		{
			continue;
		}

		LPOBJ lpObj = &gObj[n];

		for (int i = 0; i < MAX_EFFECT_LIST; i++)
		{
			CEffect* lpEffect = &lpObj->Effect[i];

			if (lpEffect->IsEffect() == false)
			{
				continue;
			}

			if (((lpEffect->m_count == 0) ? 0 : (lpEffect->m_count--)) > 0)
			{
				this->PeriodicEffect(lpObj, lpEffect);

				if (lpEffect->m_count == 0)
				{
					this->RemoveEffect(lpObj, lpEffect);

					this->GCEffectStateSend(lpObj, 0, lpEffect->m_index);

					lpEffect->Clear();
				}
			}
		}
	}
}

bool CEffectManager::AddEffect(LPOBJ lpObj, bool type, int index, int count, WORD value1, WORD value2, WORD value3, WORD value4)
{
	EFFECT_INFO* lpInfo = this->GetInfo(index);

	if (lpInfo == 0)
	{
		return false;
	}

	if (type == 0 && lpInfo->Count != -1)
	{
		count = ((lpInfo->Type == 2) ? ((int)time(0) + lpInfo->Count) : lpInfo->Count);
	}

	if (lpInfo->Value[0] != -1)
	{
		value1 = lpInfo->Value[0];
	}

	if (lpInfo->Value[1] != -1)
	{
		value2 = lpInfo->Value[1];
	}

	if (lpInfo->Value[2] != -1)
	{
		value3 = lpInfo->Value[2];
	}

	if (lpInfo->Value[3] != -1)
	{
		value4 = lpInfo->Value[3];
	}

	CEffect* lpEffect = this->GetEffectByGroup(lpObj, lpInfo->Group);

	if (lpEffect == 0)
	{
		for (int n = 0; n < MAX_EFFECT_LIST; n++)
		{
			lpEffect = &lpObj->Effect[n];

			if (lpEffect->Set(index, lpInfo->Group, lpInfo->Save, lpInfo->Type, lpInfo->Flag, 0, count, value1, value2, value3, value4) != false)
			{
				this->InsertEffect(lpObj, lpEffect);

				this->GCEffectStateSend(lpObj, 1, lpEffect->m_index);

				return true;
			}
		}
	}
	else
	{
		if (index != lpEffect->m_index || (value1 >= lpEffect->m_value[0] && value2 >= lpEffect->m_value[1] && value3 >= lpEffect->m_value[2] && value4 >= lpEffect->m_value[3]))
		{
			this->RemoveEffect(lpObj, lpEffect);

			this->GCEffectStateSend(lpObj, 0, lpEffect->m_index);

			lpEffect->Clear();

			if (lpEffect->Set(index, lpInfo->Group, lpInfo->Save, lpInfo->Type, lpInfo->Flag, 0, count, value1, value2, value3, value4) != false)
			{
				this->InsertEffect(lpObj, lpEffect);

				this->GCEffectStateSend(lpObj, 1, lpEffect->m_index);

				return true;
			}
		}
	}

	return false;
}

bool CEffectManager::DelEffect(LPOBJ lpObj, int index)
{
	for (int n = 0; n < MAX_EFFECT_LIST; n++)
	{
		CEffect* lpEffect = &lpObj->Effect[n];

		if (lpEffect->IsEffect() == false)
		{
			continue;
		}

		if (lpEffect->m_index == index)
		{
			this->RemoveEffect(lpObj, lpEffect);

			this->GCEffectStateSend(lpObj, 0, lpEffect->m_index);

			lpEffect->Clear();

			return true;
		}
	}

	return false;
}

bool CEffectManager::DelEffectByGroup(LPOBJ lpObj, int group)
{
	for (int n = 0; n < MAX_EFFECT_LIST; n++)
	{
		CEffect* lpEffect = &lpObj->Effect[n];

		if (lpEffect->IsEffect() == false)
		{
			continue;
		}

		if (lpEffect->m_group == group)
		{
			this->RemoveEffect(lpObj, lpEffect);

			this->GCEffectStateSend(lpObj, 0, lpEffect->m_index);

			lpEffect->Clear();

			return true;
		}
	}

	return false;
}

CEffect* CEffectManager::GetEffect(LPOBJ lpObj, int index)
{
	for (int n = 0; n < MAX_EFFECT_LIST; n++)
	{
		CEffect* lpEffect = &lpObj->Effect[n];

		if (lpEffect->IsEffect() == false)
		{
			continue;
		}

		if (lpEffect->m_index == index)
		{
			return lpEffect;
		}
	}

	return 0;
}

CEffect* CEffectManager::GetEffectByGroup(LPOBJ lpObj, int group)
{
	for (int n = 0; n < MAX_EFFECT_LIST; n++)
	{
		CEffect* lpEffect = &lpObj->Effect[n];

		if (lpEffect->IsEffect() == false)
		{
			continue;
		}

		if (lpEffect->m_group == group)
		{
			return lpEffect;
		}
	}

	return 0;
}

bool CEffectManager::CheckEffect(LPOBJ lpObj, int index)
{
	if (this->GetEffect(lpObj, index) != false)
	{
		return true;
	}

	return false;
}

bool CEffectManager::CheckEffectByGroup(LPOBJ lpObj, int group)
{
	if (this->GetEffectByGroup(lpObj, group) != false)
	{
		return true;
	}

	return false;
}

void CEffectManager::InsertEffect(LPOBJ lpObj, CEffect* lpEffect)
{
	lpObj->ViewSkillState |= this->GetViewSkillState(lpEffect->m_index);

	switch (lpEffect->m_index)
	{
		case EFFECT_GREATER_DAMAGE:
		{
			lpObj->EffectOption.AddPhysiDamage += lpEffect->m_value[0];

			lpObj->EffectOption.AddMagicDamage += lpEffect->m_value[0];

			break;
		}

		case EFFECT_GREATER_DEFENSE:
		{
			lpObj->EffectOption.AddDefense += lpEffect->m_value[0];

			break;
		}

		case EFFECT_MANA_SHIELD:
		{
			lpObj->EffectOption.AddDamageReduction += lpEffect->m_value[0];

			break;
		}

		case EFFECT_BP_RECOVERY:
		{
			break;
		}

		case EFFECT_GREATER_LIFE:
		{
			lpObj->EffectOption.MulMaxHP += lpEffect->m_value[0];

			break;
		}

		case EFFECT_DISABLE_MAGIC:
		{
			break;
		}

		case EFFECT_POISON:
		{
			break;
		}

		case EFFECT_ICE:
		{
			lpObj->DelayLevel = 1;

			lpObj->DelayActionTime = 800;

			break;
		}

		case EFFECT_ICE_ARROW:
		{
			break;
		}

		case EFFECT_FIRE_SLASH:
		{
			lpObj->EffectOption.DivDefense += lpEffect->m_value[0];

			break;
		}

		case EFFECT_MONSTER_PHYSI_DAMAGE_IMMUNITY:
		{
			break;
		}

		case EFFECT_MONSTER_MAGIC_DAMAGE_IMMUNITY:
		{
			break;
		}
	}

	gObjectManager.CharacterCalcAttribute(lpObj->Index);
}

void CEffectManager::RemoveEffect(LPOBJ lpObj, CEffect* lpEffect)
{
	lpObj->ViewSkillState &= ~this->GetViewSkillState(lpEffect->m_index);

	switch (lpEffect->m_index)
	{
		case EFFECT_GREATER_DAMAGE:
		{
			lpObj->EffectOption.AddPhysiDamage -= lpEffect->m_value[0];

			lpObj->EffectOption.AddMagicDamage -= lpEffect->m_value[0];

			break;
		}

		case EFFECT_GREATER_DEFENSE:
		{
			lpObj->EffectOption.AddDefense -= lpEffect->m_value[0];

			break;
		}

		case EFFECT_MANA_SHIELD:
		{
			lpObj->EffectOption.AddDamageReduction -= lpEffect->m_value[0];

			break;
		}

		case EFFECT_BP_RECOVERY:
		{
			break;
		}

		case EFFECT_GREATER_LIFE:
		{
			lpObj->EffectOption.MulMaxHP -= lpEffect->m_value[0];

			break;
		}

		case EFFECT_DISABLE_MAGIC:
		{
			break;
		}

		case EFFECT_POISON:
		{
			break;
		}

		case EFFECT_ICE:
		{
			lpObj->DelayLevel = 0;

			lpObj->DelayActionTime = 0;

			break;
		}

		case EFFECT_ICE_ARROW:
		{
			break;
		}

		case EFFECT_FIRE_SLASH:
		{
			lpObj->EffectOption.DivDefense -= lpEffect->m_value[0];

			break;
		}

		case EFFECT_MONSTER_PHYSI_DAMAGE_IMMUNITY:
		{
			break;
		}

		case EFFECT_MONSTER_MAGIC_DAMAGE_IMMUNITY:
		{
			break;
		}
	}

	gObjectManager.CharacterCalcAttribute(lpObj->Index);
}

bool CEffectManager::ConvertEffectByte(CEffect* lpEffect, BYTE* lpMsg)
{
	if (lpMsg[0] == 0xFF)
	{
		return false;
	}

	lpEffect->m_index = lpMsg[0];

	lpEffect->m_count = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg[1], lpMsg[2]), MAKE_NUMBERW(lpMsg[3], lpMsg[4]));

	lpEffect->m_value[0] = MAKE_NUMBERW(lpMsg[5], lpMsg[6]);

	lpEffect->m_value[1] = MAKE_NUMBERW(lpMsg[7], lpMsg[8]);

	lpEffect->m_value[2] = MAKE_NUMBERW(lpMsg[9], lpMsg[10]);

	lpEffect->m_value[3] = MAKE_NUMBERW(lpMsg[11], lpMsg[12]);

	return true;
}

void CEffectManager::EffectByteConvert(BYTE* lpMsg, CEffect* lpEffect)
{
	if (lpEffect->IsEffect() == false || lpEffect->m_save == 0)
	{
		memset(lpMsg, 0xFF, 13);

		return;
	}

	lpMsg[0] = lpEffect->m_index;

	if (lpEffect->m_type == 2)
	{
		lpMsg[1] = SET_NUMBERHB(SET_NUMBERHW(lpEffect->m_time));

		lpMsg[2] = SET_NUMBERLB(SET_NUMBERHW(lpEffect->m_time));

		lpMsg[3] = SET_NUMBERHB(SET_NUMBERLW(lpEffect->m_time));

		lpMsg[4] = SET_NUMBERLB(SET_NUMBERLW(lpEffect->m_time));
	}
	else
	{
		lpMsg[1] = SET_NUMBERHB(SET_NUMBERHW(lpEffect->m_count));

		lpMsg[2] = SET_NUMBERLB(SET_NUMBERHW(lpEffect->m_count));

		lpMsg[3] = SET_NUMBERHB(SET_NUMBERLW(lpEffect->m_count));

		lpMsg[4] = SET_NUMBERLB(SET_NUMBERLW(lpEffect->m_count));
	}

	lpMsg[5] = SET_NUMBERHB(lpEffect->m_value[0]);

	lpMsg[6] = SET_NUMBERLB(lpEffect->m_value[0]);

	lpMsg[7] = SET_NUMBERHB(lpEffect->m_value[1]);

	lpMsg[8] = SET_NUMBERLB(lpEffect->m_value[1]);

	lpMsg[9] = SET_NUMBERHB(lpEffect->m_value[2]);

	lpMsg[10] = SET_NUMBERLB(lpEffect->m_value[2]);

	lpMsg[11] = SET_NUMBERHB(lpEffect->m_value[3]);

	lpMsg[12] = SET_NUMBERLB(lpEffect->m_value[3]);
}

int CEffectManager::GeneratePartyEffectList(LPOBJ lpObj, BYTE* lpMsg, int* size)
{
	int count = 0;

	PMSG_PARTY_EFFECT_LIST info;

	for (int n = 0; n < MAX_EFFECT_LIST; n++)
	{
		if (lpObj->Effect[n].IsEffect() != false)
		{
			info.effect = lpObj->Effect[n].m_index;

			info.count = lpObj->Effect[n].m_count;

			memcpy(&lpMsg[(*size)], &info, sizeof(info));

			(*size) += sizeof(info);

			count++;
		}
	}

	return count;
}

bool CEffectManager::CheckImmobilizeEffect(LPOBJ lpObj)
{
	if (this->CheckEffect(lpObj, EFFECT_ICE_ARROW) != false)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CEffectManager::ClearAllEffect(LPOBJ lpObj)
{
	for (int n = 0; n < MAX_EFFECT_LIST; n++)
	{
		CEffect* lpEffect = &lpObj->Effect[n];

		if (lpEffect->IsEffect() == false || lpEffect->m_type != 0)
		{
			continue;
		}

		this->RemoveEffect(lpObj, lpEffect);

		this->GCEffectStateSend(lpObj, 0, lpEffect->m_index);

		lpEffect->Clear();
	}
}

void CEffectManager::ClearDebuffEffect(LPOBJ lpObj, int count)
{
	for (int n = 0; n < MAX_EFFECT_LIST; n++)
	{
		CEffect* lpEffect = &lpObj->Effect[n];

		if (lpEffect->IsEffect() == false || lpEffect->m_type != 0 || lpEffect->m_flag == 0)
		{
			continue;
		}

		if ((--count) >= 0)
		{
			this->RemoveEffect(lpObj, lpEffect);

			this->GCEffectStateSend(lpObj, 0, lpEffect->m_index);

			lpEffect->Clear();
		}
	}
}

WORD CEffectManager::GetViewSkillState(BYTE effect)
{
	switch (effect)
	{
		case EFFECT_GREATER_DAMAGE:
		{
			return STATE_ATTACK;
		}

		case EFFECT_GREATER_DEFENSE:
		{
			return STATE_DEFENSE;
		}

		case EFFECT_MANA_SHIELD:
		{
			return STATE_REDUCE_MAGICDEFENSE;
		}

		case EFFECT_GREATER_LIFE:
		{
			return STATE_ADD_LIFE;
		}

		case EFFECT_POISON:
		{
			return STATE_POISON;
		}

		case EFFECT_ICE:
		{
			return STATE_FREEZE;
		}

		case EFFECT_ICE_ARROW:
		{
			return STATE_HARDEN;
		}

		case EFFECT_FIRE_SLASH:
		{
			return STATE_REDUCE_DEFENSE;
		}

		case EFFECT_MONSTER_PHYSI_DAMAGE_IMMUNITY:
		{
			return STATE_REDUCE_ATTACKDAMAGE;
		}

		case EFFECT_MONSTER_MAGIC_DAMAGE_IMMUNITY:
		{
			return STATE_REDUCE_MAGICPOWER;
		}
	}

	return 0;
}

void CEffectManager::PeriodicEffect(LPOBJ lpObj, CEffect* lpEffect)
{
	if (lpEffect->m_index != EFFECT_POISON)
	{
		return;
	}

	if (lpEffect->m_value[1] == 0 || (lpEffect->m_count % lpEffect->m_value[1]) != 0)
	{
		return;
	}

	if (lpObj->Type == OBJECT_NPC || lpObj->Live == 0 || OBJECT_RANGE(lpEffect->m_value[0]) == false)
	{
		return;
	}

	LPOBJ lpTarget = &gObj[lpEffect->m_value[0]];

	int damage = 0;

	int absorb = 0;

	int type = 0;

	int effect = DAMAGE_TYPE_NORMAL;

	if (lpEffect->m_index == EFFECT_POISON)
	{
		damage = (((damage = (int)((lpObj->Life * lpEffect->m_value[2]) / 100)) > lpEffect->m_value[3]) ? ((lpEffect->m_value[3] == 0) ? damage : lpEffect->m_value[3]) : damage);

		absorb = 0;

		type = 2;

		effect = DAMAGE_TYPE_IGNORE;
	}

	if (lpObj->Life < damage)
	{
		lpObj->Life = 0;
	}
	else
	{
		lpObj->Life -= damage;
	}

	if ((lpTarget->Life + absorb) > (lpTarget->MaxLife + lpTarget->AddLife))
	{
		lpTarget->Life = lpTarget->MaxLife + lpTarget->AddLife;
	}
	else
	{
		lpTarget->Life += absorb;
	}

	if (damage > 0)
	{
		gObjectManager.CharacterLifeCheck(lpTarget, lpObj, damage, type, 0, effect, 0);
	}

	if (absorb > 0)
	{
		GCLifeSend(lpTarget->Index, 0xFF, (int)lpTarget->Life);
	}
}

void CEffectManager::GCEffectStateSend(LPOBJ lpObj, BYTE state, BYTE effect)
{
	PMSG_EFFECT_STATE_SEND pMsg;

	pMsg.header.set(0x07, sizeof(pMsg));

	pMsg.state = state;

	pMsg.effect = this->GetViewSkillState(effect);

	pMsg.index[0] = SET_NUMBERHB(lpObj->Index);

	pMsg.index[1] = SET_NUMBERLB(lpObj->Index);

	if (lpObj->Type == OBJECT_USER)
	{
		DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
	}

	MsgSendV2(lpObj, (BYTE*)&pMsg, pMsg.header.size);
}