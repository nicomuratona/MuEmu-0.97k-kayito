#include "stdafx.h"
#include "SkillManager.h"
#include "Math.h"
#include "Attack.h"
#include "EffectManager.h"
#include "Fruit.h"
#include "Guild.h"
#include "ItemManager.h"
#include "Map.h"
#include "ReadScript.h"
#include "Message.h"
#include "Monster.h"
#include "Move.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "Party.h"
#include "Quest.h"
#include "ServerInfo.h"
#include "SkillHitBox.h"
#include "Util.h"
#include "Viewport.h"

CSkillManager gSkillManager;

CSkillManager::CSkillManager()
{
	this->m_SkillInfo.clear();
}

CSkillManager::~CSkillManager()
{

}

void CSkillManager::Load(char* path)
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

	this->m_SkillInfo.clear();

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

			SKILL_INFO info;

			info.Index = lpReadScript->GetNumber();

			strcpy_s(info.Name, lpReadScript->GetAsString());

			info.Damage = lpReadScript->GetAsNumber();

			info.Mana = lpReadScript->GetAsNumber();

			info.BP = lpReadScript->GetAsNumber();

			info.Range = lpReadScript->GetAsNumber();

			info.Radio = lpReadScript->GetAsNumber();

			info.Delay = lpReadScript->GetAsNumber();

			info.Type = lpReadScript->GetAsNumber();

			info.Effect = lpReadScript->GetAsNumber();

			info.RequireLevel = lpReadScript->GetAsNumber();

			info.RequireEnergy = lpReadScript->GetAsNumber();

			for (int n = 0; n < MAX_CLASS; n++)
			{
				info.RequireClass[n] = lpReadScript->GetAsNumber();
			}

			this->m_SkillInfo.insert(std::pair<int, SKILL_INFO>(info.Index, info));
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript->GetError());
	}

	delete lpReadScript;
}

bool CSkillManager::GetInfo(int index, SKILL_INFO* lpInfo)
{
	std::map<int, SKILL_INFO>::iterator it = this->m_SkillInfo.find(index);

	if (it == this->m_SkillInfo.end())
	{
		return false;
	}
	else
	{
		(*lpInfo) = it->second;

		return true;
	}
}

int CSkillManager::GetSkillDamage(int index)
{
	std::map<int, SKILL_INFO>::iterator it = this->m_SkillInfo.find(index);

	if (it == this->m_SkillInfo.end())
	{
		return -1;
	}
	else
	{
		return it->second.Damage;
	}
}

int CSkillManager::GetSkillMana(int index)
{
	std::map<int, SKILL_INFO>::iterator it = this->m_SkillInfo.find(index);

	if (it == this->m_SkillInfo.end())
	{
		return -1;
	}
	else
	{
		return it->second.Mana;
	}
}

int CSkillManager::GetSkillBP(int index)
{
	std::map<int, SKILL_INFO>::iterator it = this->m_SkillInfo.find(index);

	if (it == this->m_SkillInfo.end())
	{
		return -1;
	}
	else
	{
		return it->second.BP;
	}
}

int CSkillManager::GetSkillType(int index)
{
	std::map<int, SKILL_INFO>::iterator it = this->m_SkillInfo.find(index);

	if (it == this->m_SkillInfo.end())
	{
		return -1;
	}
	else
	{
		return it->second.Type;
	}
}

int CSkillManager::GetSkillEffect(int index)
{
	std::map<int, SKILL_INFO>::iterator it = this->m_SkillInfo.find(index);

	if (it == this->m_SkillInfo.end())
	{
		return -1;
	}
	else
	{
		return it->second.Effect;
	}
}

int CSkillManager::GetSkillAngle(int x, int y, int tx, int ty)
{
	float dx = (float)(x - tx);

	float dy = (float)(y - ty);

	float rad = atan2(dy, dx);

	int angle = (int)(((rad * 180) / 3.141592741012573) + 90);

	return ((angle < 0) ? (angle + 360) : angle);
}

int CSkillManager::GetSkillFrustrum(int* SkillFrustrumX, int* SkillFrustrumY, BYTE angle, int x, int y, float sx, float sy)
{
	vec3_t p[4];

	Vector(-sx, sy, 0.0f, p[0]);

	Vector(sx, sy, 0.0f, p[1]);

	Vector(1, 0.0f, 0.0f, p[2]);

	Vector(-1, 0.0f, 0.0f, p[3]);

	vec3_t Angle;

	Vector(0.0f, 0.0f, (vec_t)((angle * 360) / 255), Angle);

	float Matrix[3][4];

	AngleMatrix(Angle, Matrix);

	vec3_t vFrustrum[4];

	for (int n = 0; n < 4; n++)
	{
		VectorRotate(p[n], Matrix, vFrustrum[n]);

		SkillFrustrumX[n] = (int)vFrustrum[n][0] + x;

		SkillFrustrumY[n] = (int)vFrustrum[n][1] + y;
	}

	return 1;
}

int CSkillManager::GetSkillNumber(int index, int level)
{
	if (index >= GET_ITEM(15, 0) && index <= GET_ITEM(15, 15))
	{
		return ((index % MAX_ITEM_TYPE) + 1);
	}

	switch (index)
	{
		case GET_ITEM(12, 7):
		{
			return SKILL_TWISTING_SLASH;
		}

		case GET_ITEM(12, 8):
		{
			return SKILL_HEAL;
		}

		case GET_ITEM(12, 9):
		{
			return SKILL_GREATER_DEFENSE;
		}

		case GET_ITEM(12, 10):
		{
			return SKILL_GREATER_DAMAGE;
		}

		case GET_ITEM(12, 11):
		{
			return (SKILL_SUMMON1 + level);
		}

		case GET_ITEM(12, 12):
		{
			return SKILL_RAGEFUL_BLOW;
		}

		case GET_ITEM(12, 13):
		{
			return SKILL_IMPALE;
		}

		case GET_ITEM(12, 14):
		{
			return SKILL_GREATER_LIFE;
		}

		case GET_ITEM(12, 16):
		{
			return SKILL_FIRE_SLASH;
		}

		case GET_ITEM(12, 17):
		{
			return SKILL_PENETRATION;
		}

		case GET_ITEM(12, 18):
		{
			return SKILL_ICE_ARROW;
		}

		case GET_ITEM(12, 19):
		{
			return SKILL_DEATH_STAB;
		}
	}

	return -1;
}

bool CSkillManager::CheckSkillMana(LPOBJ lpObj, int index)
{
	SKILL_INFO SkillInfo;

	if (!this->GetInfo(index, &SkillInfo))
	{
		return false;
	}

	if (lpObj->Mana < ((SkillInfo.Mana * lpObj->MPConsumptionRate) / 100))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CSkillManager::CheckSkillBP(LPOBJ lpObj, int index)
{
	SKILL_INFO SkillInfo;

	if (!this->GetInfo(index, &SkillInfo))
	{
		return false;
	}

	if (lpObj->BP < ((SkillInfo.BP * lpObj->BPConsumptionRate) / 100))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CSkillManager::CheckSkillRange(int index, int x, int y, int tx, int ty)
{
	SKILL_INFO SkillInfo;

	if (!this->GetInfo(index, &SkillInfo))
	{
		return false;
	}

	if (SkillInfo.Range == 0)
	{
		return false;
	}

	if ((int)sqrt(pow((float)(x - tx), 2) + pow((float)(y - ty), 2)) <= SkillInfo.Range)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CSkillManager::CheckSkillRadio(int index, int x, int y, int tx, int ty)
{
	SKILL_INFO SkillInfo;

	if (!this->GetInfo(index, &SkillInfo))
	{
		return false;
	}

	if (SkillInfo.Radio == 0)
	{
		return false;
	}

	if ((int)sqrt(pow((float)(x - tx), 2) + pow((float)(y - ty), 2)) <= SkillInfo.Radio)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CSkillManager::CheckSkillFrustrum(int* SkillFrustrumX, int* SkillFrustrumY, int x, int y)
{
	for (int n = 0, i = 3; n < 4; i = n, n++)
	{
		if ((((SkillFrustrumX[n] - x) * (SkillFrustrumY[i] - y)) - ((SkillFrustrumX[i] - x) * (SkillFrustrumY[n] - y))) < 0)
		{
			return false;
		}
	}

	return true;
}

bool CSkillManager::CheckSkillDelay(LPOBJ lpObj, int index)
{
	SKILL_INFO SkillInfo;

	if (!this->GetInfo(index, &SkillInfo))
	{
		return false;
	}

	if ((GetTickCount() - lpObj->SkillDelay[index]) < ((DWORD)SkillInfo.Delay))
	{
		return false;
	}

	lpObj->SkillDelay[index] = GetTickCount();

	return true;
}

bool CSkillManager::CheckSkillTarget(LPOBJ lpObj, int aIndex, int bIndex, int type)
{
	if (OBJECT_RANGE(aIndex) == 0)
	{
		return false;
	}

	if (type == OBJECT_NPC)
	{
		return false;
	}

	if (gObj[aIndex].Live == 0 || gObj[aIndex].State != OBJECT_PLAYING || gObj[aIndex].Teleport != 0)
	{
		return false;
	}

	if (lpObj->Type == OBJECT_MONSTER && type == OBJECT_USER)
	{
		return true;
	}

	if (aIndex == bIndex)
	{
		return true;
	}

	int SummonIndex = aIndex;

	if (type == OBJECT_MONSTER)
	{
		if ((gObj[aIndex].Class >= 100 && gObj[aIndex].Class <= 110)) // Trap
		{
			return false;
		}

		if (OBJECT_RANGE(gObj[aIndex].SummonIndex) == false)
		{
			return true;
		}
		else
		{
			SummonIndex = gObj[aIndex].SummonIndex;
		}
	}

	if (gGuild.GuildWarStateCheck(lpObj, &gObj[SummonIndex]) != false)
	{
		return true;
	}

	return false;
}

bool CSkillManager::CheckSkillRequireLevel(LPOBJ lpObj, int index)
{
	SKILL_INFO SkillInfo;

	if (!this->GetInfo(index, &SkillInfo))
	{
		return false;
	}

	if (lpObj->Level >= SkillInfo.RequireLevel)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CSkillManager::CheckSkillRequireEnergy(LPOBJ lpObj, int index)
{
	SKILL_INFO SkillInfo;

	if (!this->GetInfo(index, &SkillInfo))
	{
		return false;
	}

	if ((lpObj->Energy) >= SkillInfo.RequireEnergy)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CSkillManager::CheckSkillRequireClass(LPOBJ lpObj, int index)
{
	SKILL_INFO SkillInfo;

	if (!this->GetInfo(index, &SkillInfo))
	{
		return false;
	}

	if (CHECK_RANGE(lpObj->Class, MAX_CLASS) == 0)
	{
		return false;
	}

	if (SkillInfo.RequireClass[lpObj->Class] == 0)
	{
		return false;
	}

	if ((lpObj->ChangeUp + 1) >= SkillInfo.RequireClass[lpObj->Class])
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CSkillManager::CheckSkillRequireWeapon(LPOBJ lpObj, int index)
{
	if (index != SKILL_DEFENSE && index != SKILL_FALLING_SLASH && index != SKILL_LUNGE && index != SKILL_UPPERCUT && index != SKILL_CYCLONE && index != SKILL_SLASH && index != SKILL_TRIPLE_SHOT && index != SKILL_FIRE_BREATH && index != SKILL_POWER_SLASH)
	{
		return true;
	}

	for (int n = 0; n < INVENTORY_WEAR_SIZE; n++)
	{
		if (lpObj->Inventory[n].IsItem() != 0)
		{
			if (lpObj->Inventory[n].m_SkillOption != 0)
			{
				if (lpObj->Inventory[n].GetItemSkill() == index)
				{
					return true;
				}
			}
		}
	}

	return false;
}

int CSkillManager::AddSkillWeapon(LPOBJ lpObj, int index, int level)
{
	int slot = 0;

	if ((slot = this->AddSkill(lpObj, index, level)) >= 0)
	{
		this->GCSkillAddSend(lpObj->Index, slot, index, (BYTE)level);
		
		return 1;
	}
	else
	{
		return 0;
	}
}

int CSkillManager::DelSkillWeapon(LPOBJ lpObj, int index, int level)
{
	int slot = 0, count = 0;

	for (int n = 0; n < INVENTORY_WEAR_SIZE; n++)
	{
		if (lpObj->Inventory[n].IsItem() != 0)
		{
			if (lpObj->Inventory[n].m_SkillOption != 0)
			{
				if (lpObj->Inventory[n].GetItemSkill() == index)
				{
					count++;
				}
			}
		}
	}

	if (count == 1 && (slot = this->DelSkill(lpObj, index)) >= 0)
	{
		this->GCSkillDelSend(lpObj->Index, slot, index, (BYTE)level);
		
		return 1;
	}
	else
	{
		return 0;
	}
}

int CSkillManager::AddSkill(LPOBJ lpObj, int index, int level)
{
	if (lpObj->Type == OBJECT_USER && (!this->CheckSkillRequireLevel(lpObj, index) || !this->CheckSkillRequireEnergy(lpObj, index) || !this->CheckSkillRequireClass(lpObj, index)))
	{
		return -1;
	}

	if (this->GetSkill(lpObj, index) != 0)
	{
		return -1;
	}

	for (int n = 0; n < MAX_SKILL_LIST; n++)
	{
		if (lpObj->Skill[n].IsSkill() == 0)
		{
			return ((lpObj->Skill[n].Set(index, level) == 0) ? -1 : n);
		}
	}

	return -1;
}

int CSkillManager::DelSkill(LPOBJ lpObj, int index)
{
	for (int n = 0; n < MAX_SKILL_LIST; n++)
	{
		if (lpObj->Skill[n].IsSkill() != 0)
		{
			if (lpObj->Skill[n].m_index == index)
			{
				lpObj->Skill[n].Clear();

				return n;
			}
		}
	}

	return -1;
}

CSkill* CSkillManager::GetSkill(LPOBJ lpObj, int index)
{
	for (int n = 0; n < MAX_SKILL_LIST; n++)
	{
		if (lpObj->Skill[n].IsSkill() != 0)
		{
			if (lpObj->Skill[n].m_index == index || lpObj->Skill[n].m_skill == index)
			{
				return &lpObj->Skill[n];
			}
		}
	}

	return 0;
}

bool CSkillManager::ConvertSkillByte(CSkill* lpSkill, BYTE* lpMsg)
{
	BYTE index = (((lpMsg[0] == 0xFF && lpMsg[2] == 0x00) ? 0xFF : lpMsg[2]), lpMsg[0]);

	if (index == 0xFF)
	{
		return 0;
	}

	lpSkill->m_level = lpMsg[1];

	lpSkill->m_skill = index;

	lpSkill->m_index = index;

	return 1;
}

void CSkillManager::SkillByteConvert(BYTE* lpMsg, CSkill* lpSkill)
{
	lpMsg[0] = lpSkill->m_skill;

	lpMsg[1] = lpSkill->m_level << 3;

	lpMsg[2] = 0;
}

void CSkillManager::UseAttackSkill(int aIndex, int bIndex, CSkill* lpSkill)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Type == OBJECT_USER && !this->CheckSkillRequireWeapon(lpObj, lpSkill->m_skill))
	{
		return;
	}

	if (lpObj->Type == OBJECT_USER && lpSkill->m_skill == SKILL_TWISTING_SLASH && lpObj->Inventory[0].IsItem() == 0)
	{
		return;
	}

	if (lpObj->Type != OBJECT_USER || (this->CheckSkillMana(lpObj, lpSkill->m_index) && this->CheckSkillBP(lpObj, lpSkill->m_index)))
	{
		if (this->RunningSkill(aIndex, bIndex, lpSkill, (BYTE)lpObj->X, (BYTE)lpObj->Y, 0) && lpObj->Type == OBJECT_USER)
		{
			lpObj->Mana -= ((this->GetSkillMana(lpSkill->m_index) * lpObj->MPConsumptionRate) / 100);

			lpObj->BP -= ((this->GetSkillBP(lpSkill->m_index) * lpObj->BPConsumptionRate) / 100);

			GCManaSend(aIndex, 0xFF, (int)lpObj->Mana, lpObj->BP);
		}
	}
}

void CSkillManager::UseDurationSkillAttack(int aIndex, int bIndex, CSkill* lpSkill, BYTE x, BYTE y, BYTE dir, BYTE angle)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Type == OBJECT_USER && !this->CheckSkillRequireWeapon(lpObj, lpSkill->m_skill))
	{
		return;
	}

	if (lpObj->Type == OBJECT_USER && lpSkill->m_skill == SKILL_TWISTING_SLASH && lpObj->Inventory[0].IsItem() == 0)
	{
		return;
	}

	this->GCDurationSkillAttackSend(lpObj, lpSkill->m_index, x, y, dir);

	if (lpObj->Type != OBJECT_USER || (this->CheckSkillMana(lpObj, lpSkill->m_index) && this->CheckSkillBP(lpObj, lpSkill->m_index)))
	{
		if (this->RunningSkill(aIndex, bIndex, lpSkill, x, y, angle) && lpObj->Type == OBJECT_USER)
		{
			lpObj->Mana -= ((this->GetSkillMana(lpSkill->m_index) * lpObj->MPConsumptionRate) / 100);

			lpObj->BP -= ((this->GetSkillBP(lpSkill->m_index) * lpObj->BPConsumptionRate) / 100);

			GCManaSend(aIndex, 0xFF, (int)lpObj->Mana, lpObj->BP);
		}
	}
}

bool CSkillManager::RunningSkill(int aIndex, int bIndex, CSkill* lpSkill, BYTE x, BYTE y, BYTE angle)
{
	LPOBJ lpObj = &gObj[aIndex];

	lpObj->ShieldDamageReductionTime = 0;

	switch (lpSkill->m_skill)
	{
		case SKILL_FLAME:
		{
			return this->MultiSkillAttack(aIndex, bIndex, lpSkill);
		}

		case SKILL_TWISTER:
		{
			return this->MultiSkillAttack(aIndex, bIndex, lpSkill);
		}

		case SKILL_EVIL_SPIRIT:
		{
			return this->MultiSkillAttack(aIndex, bIndex, lpSkill);
		}

		case SKILL_HELL_FIRE:
		{
			return this->MultiSkillAttack(aIndex, bIndex, lpSkill);
		}

		case SKILL_AQUA_BEAM:
		{
			return this->MultiSkillAttack(aIndex, bIndex, lpSkill);
		}

		case SKILL_BLAST:
		{
			return this->MultiSkillAttack(aIndex, bIndex, lpSkill);
		}

		case SKILL_INFERNO:
		{
			return this->MultiSkillAttack(aIndex, bIndex, lpSkill);
		}

		case SKILL_MANA_SHIELD:
		{
			return this->SkillManaShield(aIndex, bIndex, lpSkill);
		}

		case SKILL_DEFENSE:
		{
			return this->SkillDefense(aIndex, bIndex, lpSkill);
		}

		case SKILL_TRIPLE_SHOT:
		{
			return this->SkillTripleShot(aIndex, bIndex, lpSkill, angle);
		}

		case SKILL_HEAL:
		{
			return this->SkillHeal(aIndex, bIndex, lpSkill);
		}

		case SKILL_GREATER_DEFENSE:
		{
			return this->SkillGreaterDefense(aIndex, bIndex, lpSkill);
		}

		case SKILL_GREATER_DAMAGE:
		{
			return this->SkillGreaterDamage(aIndex, bIndex, lpSkill);
		}

		case SKILL_SUMMON1:
		{
			return this->SkillSummon(aIndex, bIndex, lpSkill);
		}

		case SKILL_SUMMON2:
		{
			return this->SkillSummon(aIndex, bIndex, lpSkill);
		}

		case SKILL_SUMMON3:
		{
			return this->SkillSummon(aIndex, bIndex, lpSkill);
		}

		case SKILL_SUMMON4:
		{
			return this->SkillSummon(aIndex, bIndex, lpSkill);
		}

		case SKILL_SUMMON5:
		{
			return this->SkillSummon(aIndex, bIndex, lpSkill);
		}

		case SKILL_SUMMON6:
		{
			return this->SkillSummon(aIndex, bIndex, lpSkill);
		}

		case SKILL_SUMMON7:
		{
			return this->SkillSummon(aIndex, bIndex, lpSkill);
		}

		case SKILL_TWISTING_SLASH:
		{
			return this->SkillTwistingSlash(aIndex, bIndex, lpSkill);
		}

		case SKILL_RAGEFUL_BLOW:
		{
			return this->SkillRagefulBlow(aIndex, bIndex, lpSkill);
		}

		case SKILL_DEATH_STAB:
		{
			return this->SkillDeathStab(aIndex, bIndex, lpSkill);
		}

		case SKILL_IMPALE:
		{
			return this->MultiSkillAttack(aIndex, bIndex, lpSkill);
		}

		case SKILL_GREATER_LIFE:
		{
			return this->SkillGreaterLife(aIndex, bIndex, lpSkill);
		}

		case SKILL_MONSTER_AREA_ATTACK:
		{
			return this->SkillMonsterAreaAttack(aIndex, bIndex, lpSkill);
		}

		case SKILL_PENETRATION:
		{
			return this->MultiSkillAttack(aIndex, bIndex, lpSkill);
		}

		case SKILL_FIRE_SLASH:
		{
			return this->MultiSkillAttack(aIndex, bIndex, lpSkill);
			return this->SkillFireSlash(aIndex, bIndex, lpSkill);
		}

		case SKILL_POWER_SLASH:
		{
			return this->SkillPowerSlash(aIndex, bIndex, lpSkill, angle);
		}

		default:
		{
			return this->BasicSkillAttack(aIndex, bIndex, lpSkill);
		}
	}

	return false;
}

bool CSkillManager::BasicSkillAttack(int aIndex, int bIndex, CSkill* lpSkill)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (OBJECT_RANGE(bIndex) == 0)
	{
		return true;
	}

	if (lpObj->Type == OBJECT_USER && !this->CheckSkillRange(lpSkill->m_index, lpObj->X, lpObj->Y, gObj[bIndex].X, gObj[bIndex].Y))
	{
		return false;
	}

	gAttack.Attack(lpObj, &gObj[bIndex], lpSkill, true, 0, 0);

	return true;
}

bool CSkillManager::MultiSkillAttack(int aIndex, int bIndex, CSkill* lpSkill)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Type == OBJECT_USER)
	{
		lpObj->MultiSkillIndex = lpSkill->m_index;

		lpObj->MultiSkillCount = 0;

		switch (lpSkill->m_skill)
		{
			case SKILL_FLAME:
			{
				this->BasicSkillAttack(aIndex, bIndex, lpSkill);

				break;
			}

			case SKILL_BLAST:
			{
				this->BasicSkillAttack(aIndex, bIndex, lpSkill);

				break;
			}

			case SKILL_IMPALE:
			{
				this->BasicSkillAttack(aIndex, bIndex, lpSkill);

				break;
			}

			case SKILL_FIRE_SLASH:
			{
				this->BasicSkillAttack(aIndex, bIndex, lpSkill);

				break;
			}
		}
	}
	else
	{
		switch (lpSkill->m_skill)
		{
			case SKILL_EVIL_SPIRIT:
			{
				this->SkillEvilSpirit(aIndex, bIndex, lpSkill);

				break;
			}
		}
	}

	return true;
}

bool CSkillManager::SkillChangeUse(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Type != OBJECT_USER)
	{
		return false;
	}

	int change = -1;

	for (int n = 0; n < 2; n++)
	{
		CItem* Ring = &lpObj->Inventory[10 + n];

		if (Ring->IsItem() == 0 || Ring->m_Durability == 0 || change != -1)
		{
			continue;
		}

		if (Ring->m_Index == GET_ITEM(13, 10)) // Transformation Ring
		{
			change = Ring->m_Level;
		}
	}

	switch (change)
	{
		case 0:
		{
			change = gServerInfo.m_TransformationRing1;

			break;
		}

		case 1:
		{
			change = gServerInfo.m_TransformationRing2;

			break;
		}

		case 2:
		{
			change = gServerInfo.m_TransformationRing3;

			break;
		}

		case 3:
		{
			change = gServerInfo.m_TransformationRing4;

			break;
		}

		case 4:
		{
			change = gServerInfo.m_TransformationRing5;

			break;
		}

		case 5:
		{
			change = gServerInfo.m_TransformationRing6;

			break;
		}
	}

	if (lpObj->Change == change)
	{
		return false;
	}
	else
	{
		lpObj->Change = change;

		return true;
	}
}

bool CSkillManager::SkillEvilSpirit(int aIndex, int bIndex, CSkill* lpSkill)
{
	LPOBJ lpObj = &gObj[aIndex];

	int count = 0;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (!this->CheckSkillTarget(lpObj, index, bIndex, lpObj->VpPlayer2[n].type))
		{
			continue;
		}

		if (!this->CheckSkillRadio(lpSkill->m_index, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y))
		{
			continue;
		}

		gAttack.Attack(lpObj, &gObj[index], lpSkill, false, 0, 0);

		if (CHECK_SKILL_ATTACK_COUNT(count) == false)
		{
			break;
		}
	}

	return true;
}

bool CSkillManager::SkillManaShield(int aIndex, int bIndex, CSkill* lpSkill)
{
	LPOBJ lpTarget = &gObj[bIndex];

	if (lpTarget->Type != OBJECT_USER && OBJECT_RANGE(lpTarget->SummonIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->PartyNumber != lpTarget->PartyNumber)
	{
		return false;
	}

	if (!this->CheckSkillRange(lpSkill->m_index, lpObj->X, lpObj->Y, lpTarget->X, lpTarget->Y))
	{
		return false;
	}

	int value1 = gServerInfo.m_ManaShieldConstA + (lpObj->Dexterity / gServerInfo.m_ManaShieldConstB) + (lpObj->Energy / gServerInfo.m_ManaShieldConstC);

	value1 = ((value1 > gServerInfo.m_ManaShieldMaxRate) ? gServerInfo.m_ManaShieldMaxRate : value1);

	value1 = (value1 * ((lpTarget->Type == OBJECT_USER) ? gServerInfo.m_ManaShieldRate[lpTarget->Class] : 100)) / 100;

	int value2 = 0;

	int count = gServerInfo.m_ManaShieldTime;

	gEffectManager.AddEffect(lpTarget, 0, this->GetSkillEffect(lpSkill->m_index), count, value1, value2, 0, 0);

	this->GCSkillAttackSend(lpObj, lpSkill->m_index, bIndex, 1);

	return true;
}

bool CSkillManager::SkillDefense(int aIndex, int bIndex, CSkill* lpSkill)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Inventory[1].IsItem() != 0)
	{
		if (lpObj->Inventory[1].m_Index >= GET_ITEM(6, 0) && lpObj->Inventory[1].m_Index < GET_ITEM(7, 0))
		{
			if (lpObj->Inventory[1].m_SkillOption != 0)
			{
				lpObj->ShieldDamageReductionTime = GetTickCount();

				GCActionSend(lpObj, SKILL_DEFENSE, lpObj->Index, bIndex);

				return true;
			}
		}
	}

	return false;
}

bool CSkillManager::SkillTripleShot(int aIndex, int bIndex, CSkill* lpSkill, BYTE angle)
{
	LPOBJ lpObj = &gObj[aIndex];

	SKILL_INFO SkillInfo;

	if (!this->GetInfo(lpSkill->m_index, &SkillInfo))
	{
		return false;
	}

	int SkillFrustrumX[4], SkillFrustrumY[4];

	this->GetSkillFrustrum(SkillFrustrumX, SkillFrustrumY, angle, lpObj->X, lpObj->Y, (float)SkillInfo.Radio, (float)SkillInfo.Range);

	int count = 0;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (!this->CheckSkillTarget(lpObj, index, bIndex, lpObj->VpPlayer2[n].type))
		{
			continue;
		}

		if (!this->CheckSkillRange(lpSkill->m_index, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y))
		{
			continue;
		}

		if (!this->CheckSkillFrustrum(SkillFrustrumX, SkillFrustrumY, gObj[index].X, gObj[index].Y))
		{
			continue;
		}

		gAttack.Attack(lpObj, &gObj[index], lpSkill, false, 0, 0);

		if (CHECK_SKILL_ATTACK_COUNT(count) == 0)
		{
			break;
		}
	}

	return true;
}

bool CSkillManager::SkillHeal(int aIndex, int bIndex, CSkill* lpSkill)
{
	LPOBJ lpTarget = &gObj[bIndex];

	if (lpTarget->Type != OBJECT_USER && OBJECT_RANGE(lpTarget->SummonIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (!this->CheckSkillRange(lpSkill->m_index, lpObj->X, lpObj->Y, lpTarget->X, lpTarget->Y))
	{
		return false;
	}

	int value = gServerInfo.m_HealConstA + (lpObj->Energy / gServerInfo.m_HealConstB);

	if ((lpTarget->Life + value) > (lpTarget->MaxLife + lpTarget->AddLife))
	{
		lpTarget->Life = lpTarget->MaxLife + lpTarget->AddLife;
	}
	else
	{
		lpTarget->Life += value;
	}

	if (lpTarget->Type == OBJECT_USER)
	{
		GCLifeSend(bIndex, 0xFF, (int)lpTarget->Life);
	}

	if (lpTarget->Type == OBJECT_MONSTER && OBJECT_RANGE(lpTarget->SummonIndex) != false)
	{
		GCSummonLifeSend(lpTarget->SummonIndex, (int)lpTarget->Life, (int)lpTarget->MaxLife);
	}

	this->GCSkillAttackSend(lpObj, lpSkill->m_index, bIndex, 1);

	return true;
}

bool CSkillManager::SkillGreaterDefense(int aIndex, int bIndex, CSkill* lpSkill)
{
	LPOBJ lpTarget = &gObj[bIndex];

	if (lpTarget->Type != OBJECT_USER && OBJECT_RANGE(lpTarget->SummonIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (!this->CheckSkillRange(lpSkill->m_index, lpObj->X, lpObj->Y, lpTarget->X, lpTarget->Y))
	{
		return false;
	}

	int value = gServerInfo.m_GreaterDefenseConstA + (lpObj->Energy / gServerInfo.m_GreaterDefenseConstB);

	value = (value * ((lpTarget->Type == OBJECT_USER) ? gServerInfo.m_GreaterDefenseRate[lpTarget->Class] : 100)) / 100;

	int count = gServerInfo.m_GreaterDefenseTime;

	gEffectManager.AddEffect(lpTarget, 0, this->GetSkillEffect(lpSkill->m_index), count, (value * 2), 0, 0, 0);

	this->GCSkillAttackSend(lpObj, lpSkill->m_index, bIndex, 1);

	return true;
}

bool CSkillManager::SkillGreaterDamage(int aIndex, int bIndex, CSkill* lpSkill)
{
	LPOBJ lpTarget = &gObj[bIndex];

	if (lpTarget->Type != OBJECT_USER && OBJECT_RANGE(lpTarget->SummonIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (!this->CheckSkillRange(lpSkill->m_index, lpObj->X, lpObj->Y, lpTarget->X, lpTarget->Y))
	{
		return false;
	}

	int value = gServerInfo.m_GreaterDamageConstA + (lpObj->Energy / gServerInfo.m_GreaterDamageConstB);

	value = (value * ((lpTarget->Type == OBJECT_USER) ? gServerInfo.m_GreaterDamageRate[lpTarget->Class] : 100)) / 100;

	int count = gServerInfo.m_GreaterDamageTime;

	gEffectManager.AddEffect(lpTarget, 0, this->GetSkillEffect(lpSkill->m_index), count, value, 0, 0, 0);

	this->GCSkillAttackSend(lpObj, lpSkill->m_index, bIndex, 1);

	return true;
}

bool CSkillManager::SkillSummon(int aIndex, int bIndex, CSkill* lpSkill)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Map == MAP_ICARUS)
	{
		return false;
	}

	int monster = 0;

	switch (lpSkill->m_index)
	{
		case SKILL_SUMMON1:
		{
			monster = gServerInfo.m_SummonMonster1;

			break;
		}

		case SKILL_SUMMON2:
		{
			monster = gServerInfo.m_SummonMonster2;

			break;
		}

		case SKILL_SUMMON3:
		{
			monster = gServerInfo.m_SummonMonster3;

			break;
		}

		case SKILL_SUMMON4:
		{
			monster = gServerInfo.m_SummonMonster4;

			break;
		}

		case SKILL_SUMMON5:
		{
			monster = gServerInfo.m_SummonMonster5;

			break;
		}

		case SKILL_SUMMON6:
		{
			monster = gServerInfo.m_SummonMonster6;

			break;
		}

		case SKILL_SUMMON7:
		{
			monster = gServerInfo.m_SummonMonster7;

			break;
		}

		default:
		{
			return false;
		}
	}

	if (OBJECT_RANGE(lpObj->SummonIndex) != 0)
	{
		gObjSummonKill(aIndex);

		GCSummonLifeSend(aIndex, 0, 1);

		return false;
	}

	int index = gObjAddSummon();

	if (OBJECT_RANGE(index) == 0)
	{
		return false;
	}

	lpObj->SummonIndex = index;

	LPOBJ lpSummon = &gObj[index];

	lpSummon->X = lpObj->X - 1;

	lpSummon->Y = lpObj->Y + 1;

	lpSummon->MTX = lpSummon->X;

	lpSummon->MTY = lpSummon->Y;

	lpSummon->Dir = 2;

	lpSummon->Map = lpObj->Map;

	gObjSetMonster(index, monster);

	lpSummon->SummonIndex = aIndex;

	lpSummon->Attribute = 100;

	lpSummon->TargetNumber = -1;

	lpSummon->ActionState.Attack = 0;

	lpSummon->ActionState.Emotion = 0;

	lpSummon->ActionState.EmotionCount = 0;

	lpSummon->PathCount = 0;

	lpSummon->MoveRange = 15;

	GCSummonLifeSend(lpSummon->SummonIndex, (int)lpSummon->Life, (int)lpSummon->MaxLife);

	return true;
}

bool CSkillManager::SkillTwistingSlash(int aIndex, int bIndex, CSkill* lpSkill)
{
	LPOBJ lpObj = &gObj[aIndex];

	int count = 0;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (!this->CheckSkillTarget(lpObj, index, bIndex, lpObj->VpPlayer2[n].type))
		{
			continue;
		}

		if (!this->CheckSkillRadio(lpSkill->m_index, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y))
		{
			continue;
		}

		gAttack.Attack(lpObj, &gObj[index], lpSkill, false, 0, 0);

		if (CHECK_SKILL_ATTACK_COUNT(count) == 0)
		{
			break;
		}
	}

	return true;
}

bool CSkillManager::SkillRagefulBlow(int aIndex, int bIndex, CSkill* lpSkill)
{
	LPOBJ lpObj = &gObj[aIndex];

	int count = 0;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (!this->CheckSkillTarget(lpObj, index, bIndex, lpObj->VpPlayer2[n].type))
		{
			continue;
		}

		if (!this->CheckSkillRadio(lpSkill->m_index, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y))
		{
			continue;
		}

		gObjAddAttackProcMsgSendDelay(lpObj, 50, index, 500, lpSkill->m_index, 0);

		if (CHECK_SKILL_ATTACK_COUNT(count) == 0)
		{
			break;
		}
	}

	return true;
}

bool CSkillManager::SkillDeathStab(int aIndex, int bIndex, CSkill* lpSkill)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (OBJECT_RANGE(bIndex) == 0)
	{
		return false;
	}

	LPOBJ lpTarget = &gObj[bIndex];

	if (!this->CheckSkillRange(lpSkill->m_index, lpObj->X, lpObj->Y, lpTarget->X, lpTarget->Y))
	{
		return false;
	}

	this->GCSkillAttackSend(lpObj, lpSkill->m_index, bIndex, 1);

	gAttack.Attack(lpObj, lpTarget, lpSkill, false, 0, 0);

	int angle = this->GetSkillAngle(lpObj->X, lpObj->Y, lpTarget->X, lpTarget->Y);

	int SkillFrustrumX[4], SkillFrustrumY[4];

	this->GetSkillFrustrum(SkillFrustrumX, SkillFrustrumY, angle, lpObj->X, lpObj->Y, 1.5f, 3.0f);

	int count = 0;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		if (lpObj->VpPlayer2[n].index == bIndex)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (!this->CheckSkillTarget(lpObj, index, bIndex, lpObj->VpPlayer2[n].type))
		{
			continue;
		}

		if (!this->CheckSkillRadio(lpSkill->m_index, lpTarget->X, lpTarget->Y, gObj[index].X, gObj[index].Y))
		{
			continue;
		}

		if (!this->CheckSkillFrustrum(SkillFrustrumX, SkillFrustrumY, gObj[index].X, gObj[index].Y))
		{
			continue;
		}

		gAttack.Attack(lpObj, &gObj[index], lpSkill, false, 0, 0);

		if (CHECK_SKILL_ATTACK_COUNT(count) == 0)
		{
			break;
		}
	}

	return true;
}

bool CSkillManager::SkillGreaterLife(int aIndex, int bIndex, CSkill* lpSkill)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Type != OBJECT_USER)
	{
		return false;
	}

	int value1 = gServerInfo.m_GreaterLifeConstA + (lpObj->Vitality / gServerInfo.m_GreaterLifeConstB) + (lpObj->Energy / gServerInfo.m_GreaterLifeConstC);

	value1 = ((value1 > gServerInfo.m_GreaterLifeMaxRate) ? gServerInfo.m_GreaterLifeMaxRate : value1);

	int value2 = 0;

	int value3 = 0;

	int count = gServerInfo.m_GreaterLifeTime;

	if (OBJECT_RANGE(lpObj->PartyNumber) == 0)
	{
		value1 = (value1 * ((lpObj->Type == OBJECT_USER) ? gServerInfo.m_GreaterLifeRate[lpObj->Class] : 100)) / 100;

		gEffectManager.AddEffect(lpObj, 0, this->GetSkillEffect(lpSkill->m_index), count, value1, value2, value3, 0);

		this->GCSkillAttackSend(lpObj, lpSkill->m_index, aIndex, 1);
	}
	else
	{
		PARTY_INFO* lpParty = &gParty.m_PartyInfo[lpObj->PartyNumber];

		for (int n = 0; n < MAX_PARTY_USER; n++)
		{
			int index = lpParty->Index[n];

			if (OBJECT_RANGE(index) == 0)
			{
				continue;
			}

			if (!this->CheckSkillRadio(lpSkill->m_index, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y))
			{
				continue;
			}

			value1 = (value1 * ((gObj[index].Type == OBJECT_USER) ? gServerInfo.m_GreaterLifeRate[gObj[index].Class] : 100)) / 100;

			gEffectManager.AddEffect(&gObj[index], 0, this->GetSkillEffect(lpSkill->m_index), count, value1, value2, value3, 0);

			this->GCSkillAttackSend(lpObj, lpSkill->m_index, index, 1);
		}
	}

	return true;
}

bool CSkillManager::SkillMonsterAreaAttack(int aIndex, int bIndex, CSkill* lpSkill)
{
	LPOBJ lpObj = &gObj[aIndex];

	int count = 0;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (!this->CheckSkillTarget(lpObj, index, bIndex, lpObj->VpPlayer2[n].type))
		{
			continue;
		}

		if (!this->CheckSkillRadio(lpSkill->m_index, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y))
		{
			continue;
		}

		gAttack.Attack(lpObj, &gObj[index], lpSkill, false, 0, 0);

		if (CHECK_SKILL_ATTACK_EXTENDED_COUNT(count) == 0)
		{
			break;
		}
	}

	return true;
}

bool CSkillManager::SkillFireSlash(int aIndex, int bIndex, CSkill* lpSkill)
{
	LPOBJ lpObj = &gObj[aIndex];

	int count = 0;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (!this->CheckSkillTarget(lpObj, index, bIndex, lpObj->VpPlayer2[n].type))
		{
			continue;
		}

		if (!this->CheckSkillRange(lpSkill->m_index, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y))
		{
			continue;
		}

		if (!this->CheckSkillRadio(lpSkill->m_index, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y))
		{
			continue;
		}

		gAttack.Attack(lpObj, &gObj[index], lpSkill, true, 0, 0);

		if (CHECK_SKILL_ATTACK_EXTENDED_COUNT(count) == 0)
		{
			break;
		}
	}

	return true;
}

bool CSkillManager::SkillPowerSlash(int aIndex, int bIndex, CSkill* lpSkill, BYTE angle)
{
	LPOBJ lpObj = &gObj[aIndex];

	SKILL_INFO SkillInfo;

	if (!this->GetInfo(lpSkill->m_index, &SkillInfo))
	{
		return false;
	}

	int SkillFrustrumX[4], SkillFrustrumY[4];

	this->GetSkillFrustrum(SkillFrustrumX, SkillFrustrumY, angle, lpObj->X, lpObj->Y, (float)SkillInfo.Radio, (float)SkillInfo.Range);

	int count = 0;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if (!this->CheckSkillTarget(lpObj, index, bIndex, lpObj->VpPlayer2[n].type))
		{
			continue;
		}

		if (!this->CheckSkillRange(lpSkill->m_index, lpObj->X, lpObj->Y, gObj[index].X, gObj[index].Y))
		{
			continue;
		}

		if (!this->CheckSkillFrustrum(SkillFrustrumX, SkillFrustrumY, gObj[index].X, gObj[index].Y))
		{
			continue;
		}

		gAttack.Attack(lpObj, &gObj[index], lpSkill, false, 0, 0);

		if (CHECK_SKILL_ATTACK_COUNT(count) == false)
		{
			break;
		}
	}

	return true;
}

void CSkillManager::ApplyFireSlashEffect(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int damage)
{
	int value = gServerInfo.m_FireSlashConstA + (lpObj->Strength / gServerInfo.m_FireSlashConstB);

	value = ((value > gServerInfo.m_FireSlashMaxRate) ? gServerInfo.m_FireSlashMaxRate : value);

	int count = gServerInfo.m_FireSlashTime;

	gEffectManager.AddEffect(lpTarget, 0, this->GetSkillEffect(lpSkill->m_index), count, value, 0, 0, 0);
}

void CSkillManager::CGMultiSkillAttackRecv(PMSG_MULTI_SKILL_ATTACK_RECV* lpMsg, int aIndex, bool type)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGS(aIndex) == 0)
	{
		return;
	}

	if (lpObj->Live == 0 || lpObj->Teleport != 0)
	{
		return;
	}

	if (gMap[lpObj->Map].CheckAttr(lpObj->X, lpObj->Y, 1) != 0)
	{
		return;
	}

	int skill = lpMsg->skill;

	CSkill* lpSkill = 0;

	if (lpObj->Type == OBJECT_USER)
	{
		lpSkill = this->GetSkill(lpObj, skill);

		if (lpSkill == 0)
		{
			return;
		}

		if (!this->CheckSkillDelay(lpObj, lpSkill->m_index))
		{
			return;
		}

		if (!this->CheckSkillRequireClass(lpObj, lpSkill->m_index))
		{
			return;
		}
	}
	else
	{
		lpSkill = &lpObj->Skill[skill];

		if (lpSkill->IsSkill() == 0)
		{
			return;
		}
	}

	if (lpSkill->m_skill != SKILL_FLAME && lpSkill->m_skill != SKILL_TWISTER && lpSkill->m_skill != SKILL_EVIL_SPIRIT && lpSkill->m_skill != SKILL_HELL_FIRE && lpSkill->m_skill != SKILL_AQUA_BEAM && lpSkill->m_skill != SKILL_BLAST && lpSkill->m_skill != SKILL_INFERNO && lpSkill->m_skill != SKILL_TRIPLE_SHOT && lpSkill->m_skill != SKILL_TWISTING_SLASH && lpSkill->m_skill != SKILL_RAGEFUL_BLOW && lpSkill->m_skill != SKILL_DEATH_STAB && lpSkill->m_skill != SKILL_IMPALE && lpSkill->m_skill != SKILL_MONSTER_AREA_ATTACK && lpSkill->m_skill != SKILL_PENETRATION && lpSkill->m_skill != SKILL_FIRE_SLASH)
	{
		return;
	}

	if (lpObj->Type == OBJECT_USER && (lpObj->MultiSkillIndex == 0 || lpObj->MultiSkillIndex != lpSkill->m_index || (lpObj->MultiSkillCount++) >= 5))
	{
		return;
	}

	if (lpObj->Type == OBJECT_USER && lpSkill->m_skill == SKILL_TRIPLE_SHOT)
	{
		if (lpObj->Inventory[0].IsItem() == 0 || lpObj->Inventory[1].IsItem() == 0)
		{
			return;
		}

		if (lpObj->Inventory[0].m_Index < GET_ITEM(4, 0) || lpObj->Inventory[0].m_Index >= GET_ITEM(5, 0))
		{
			return;
		}

		if (lpObj->Inventory[1].m_Index < GET_ITEM(4, 0) || lpObj->Inventory[1].m_Index >= GET_ITEM(5, 0))
		{
			return;
		}
	}

	lpMsg->count = ((lpMsg->count > 5) ? 5 : lpMsg->count);

	for (int n = 0; n < lpMsg->count; n++)
	{
		PMSG_MULTI_SKILL_ATTACK* lpInfo = (PMSG_MULTI_SKILL_ATTACK*)(((BYTE*)lpMsg) + sizeof(PMSG_MULTI_SKILL_ATTACK_RECV) + (sizeof(PMSG_MULTI_SKILL_ATTACK) * n));

		int bIndex = MAKE_NUMBERW(lpInfo->index[0], lpInfo->index[1]);

		if (OBJECT_RANGE(bIndex) == 0)
		{
			continue;
		}

		LPOBJ lpTarget = &gObj[bIndex];

		if (lpObj->Type == OBJECT_USER && !this->CheckSkillRadio(lpSkill->m_index, lpObj->X, lpObj->Y, lpTarget->X, lpTarget->Y))
		{
			continue;
		}

		if (type != 0 || lpSkill->m_skill == SKILL_FIRE_SLASH)
		{
			gAttack.Attack(lpObj, lpTarget, lpSkill, true, 0, 0);
		}
		else
		{
			gAttack.Attack(lpObj, lpTarget, lpSkill, false, 0, 0);
		}
	}
}

void CSkillManager::CGSkillAttackRecv(PMSG_SKILL_ATTACK_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGS(aIndex) == 0)
	{
		return;
	}

	if (lpObj->Live == 0 || lpObj->Teleport != 0)
	{
		return;
	}

	int bIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]);

	if (OBJECT_RANGE(bIndex) == 0)
	{
		return;
	}

	LPOBJ lpTarget = &gObj[bIndex];

	if (gObjIsConnectedGS(bIndex) == 0)
	{
		return;
	}

	if (lpTarget->Live == 0)
	{
		return;
	}

	if (BC_MAP_RANGE(lpObj->Map) != 0 || DS_MAP_RANGE(lpObj->Map) != 0)
	{
		//Enable buff skills in event maps
	}
	else if (gMap[lpObj->Map].CheckAttr(lpObj->X, lpObj->Y, 1) != 0 || gMap[lpTarget->Map].CheckAttr(lpTarget->X, lpTarget->Y, 1) != 0)
	{
		return;
	}

	int skill = lpMsg->skill;

	CSkill* lpSkill = 0;

	if (lpObj->Type == OBJECT_USER)
	{
		lpSkill = this->GetSkill(lpObj, skill);

		if (lpSkill == 0)
		{
			return;
		}

		if (!this->CheckSkillDelay(lpObj, lpSkill->m_index))
		{
			return;
		}

		if (!this->CheckSkillRequireClass(lpObj, lpSkill->m_index))
		{
			return;
		}
	}
	else
	{
		lpSkill = &lpObj->Skill[skill];

		if (lpSkill->IsSkill() == 0)
		{
			return;
		}
	}

	lpObj->MultiSkillIndex = 0;

	lpObj->MultiSkillCount = 0;

	this->UseAttackSkill(aIndex, bIndex, lpSkill);
}

void CSkillManager::CGDurationSkillAttackRecv(PMSG_DURATION_SKILL_ATTACK_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGS(aIndex) == 0)
	{
		return;
	}

	if (lpObj->Live == 0 || lpObj->Teleport != 0)
	{
		return;
	}

	if (gMap[lpObj->Map].CheckAttr(lpObj->X, lpObj->Y, 1) != 0)
	{
		return;
	}

	int skill = lpMsg->skill;

	CSkill* lpSkill = 0;

	if (lpObj->Type == OBJECT_USER)
	{
		lpSkill = this->GetSkill(lpObj, skill);

		if (lpSkill == 0)
		{
			return;
		}

		if (!this->CheckSkillDelay(lpObj, lpSkill->m_index))
		{
			return;
		}

		if (!this->CheckSkillRequireClass(lpObj, lpSkill->m_index))
		{
			return;
		}
	}
	else
	{
		lpSkill = &lpObj->Skill[skill];

		if (lpSkill->IsSkill() == 0)
		{
			return;
		}
	}

	lpObj->MultiSkillIndex = 0;

	lpObj->MultiSkillCount = 0;

	short bIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]);

	this->UseDurationSkillAttack(lpObj->Index, bIndex, lpSkill, lpMsg->x, lpMsg->y, lpMsg->dir, lpMsg->angle);
}

void CSkillManager::CGSkillCancelRecv(PMSG_SKILL_CANCEL_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	gEffectManager.DelEffect(lpObj, this->GetSkillEffect(lpMsg->skill));
}

void CSkillManager::CGSkillTeleportAllyRecv(PMSG_SKILL_TELEPORT_ALLY_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if (OBJECT_RANGE(lpMsg->index) == false)
	{
		return;
	}

	if (gObj[lpMsg->index].Connected != OBJECT_ONLINE || gObj[lpMsg->index].CloseCount > 0)
	{
		lpMsg->index = aIndex;
	}

	LPOBJ lpTarget = &gObj[lpMsg->index];

	if (OBJECT_RANGE(lpObj->PartyNumber) == false || OBJECT_RANGE(lpTarget->PartyNumber) == false)
	{
		return;
	}

	if (lpObj->PartyNumber != lpTarget->PartyNumber)
	{
		return;
	}

	if (lpObj->Map != lpTarget->Map)
	{
		return;
	}

	if (gObjCheckTeleportArea(lpMsg->index, lpMsg->x, lpMsg->y) == 0)
	{
		return;
	}

	CSkill* lpSkill = gSkillManager.GetSkill(lpObj, SKILL_TELEPORT_ALLY);

	if (lpSkill != 0)
	{
		if (!gSkillManager.CheckSkillMana(lpObj, lpSkill->m_index) || !gSkillManager.CheckSkillBP(lpObj, lpSkill->m_index))
		{
			return;
		}

		gSkillManager.GCSkillAttackSend(lpObj, lpSkill->m_index, lpMsg->index, 1);

		gObjTeleportMagicUse(lpMsg->index, lpMsg->x, lpMsg->y);

		lpObj->Mana -= (gSkillManager.GetSkillMana(lpSkill->m_index) * lpObj->MPConsumptionRate) / 100;

		lpObj->BP -= (gSkillManager.GetSkillBP(lpSkill->m_index) * lpObj->BPConsumptionRate) / 100;

		GCManaSend(lpObj->Index, 0xFF, (int)lpObj->Mana, lpObj->BP);
	}
}

void CSkillManager::GCSkillAttackSend(LPOBJ lpObj, int skill, int aIndex, BYTE type)
{
	PMSG_SKILL_ATTACK_SEND pMsg;

	pMsg.header.setE(0x19, sizeof(pMsg));

	pMsg.skill = skill;

	pMsg.index[0] = SET_NUMBERHB(lpObj->Index);

	pMsg.index[1] = SET_NUMBERLB(lpObj->Index);

	pMsg.target[0] = SET_NUMBERHB(aIndex) | (type * 0x80);

	pMsg.target[1] = SET_NUMBERLB(aIndex);

	if (lpObj->Type == OBJECT_USER)
	{
		DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
	}

	MsgSendV2(lpObj, (BYTE*)&pMsg, pMsg.header.size);
}

void CSkillManager::GCSkillCancelSend(LPOBJ lpObj, int skill)
{
	PMSG_SKILL_CANCEL_SEND pMsg;

	pMsg.header.set(0x1B, sizeof(pMsg));

	pMsg.skill = skill;

	pMsg.index[0] = SET_NUMBERHB(lpObj->Index);

	pMsg.index[1] = SET_NUMBERLB(lpObj->Index);

	if (lpObj->Type == OBJECT_USER)
	{
		DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
	}

	MsgSendV2(lpObj, (BYTE*)&pMsg, pMsg.header.size);
}

void CSkillManager::GCDurationSkillAttackSend(LPOBJ lpObj, int skill, BYTE x, BYTE y, BYTE dir)
{
	PMSG_DURATION_SKILL_ATTACK_SEND pMsg;

	pMsg.header.setE(0x1E, sizeof(pMsg));

	pMsg.skill = skill;

	pMsg.index[0] = SET_NUMBERHB(lpObj->Index);

	pMsg.index[1] = SET_NUMBERLB(lpObj->Index);

	pMsg.x = x;

	pMsg.y = y;

	pMsg.dir = dir;

	if (lpObj->Type == OBJECT_USER)
	{
		DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
	}

	MsgSendV2(lpObj, (BYTE*)&pMsg, pMsg.header.size);
}

void CSkillManager::GCSkillAddSend(int aIndex, BYTE slot, int skill, BYTE level)
{
	BYTE send[256];

	PMSG_SKILL_LIST_SEND pMsg;

	pMsg.header.set(0xF3, 0x11, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0xFE;

	PMSG_SKILL_LIST info;

	info.slot = slot;

	info.skill = skill;

	info.level = (level << 3) | ((skill) & 7);

	memcpy(&send[size], &info, sizeof(info));

	size += sizeof(info);

	pMsg.header.size = size;

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(aIndex, send, size);
}

void CSkillManager::GCSkillDelSend(int aIndex, BYTE slot, int skill, BYTE level)
{
	BYTE send[256];

	PMSG_SKILL_LIST_SEND pMsg;

	pMsg.header.set(0xF3, 0x11, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0xFF;

	PMSG_SKILL_LIST info;

	info.slot = slot;

	info.skill = skill;

	info.level = (level << 3) | ((skill) & 7);

	memcpy(&send[size], &info, sizeof(info));

	size += sizeof(info);

	pMsg.header.size = size;

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(aIndex, send, size);
}

void CSkillManager::GCSkillListSend(LPOBJ lpObj)
{
	BYTE send[1024];

	PMSG_SKILL_LIST_SEND pMsg;

	pMsg.header.set(0xF3, 0x11, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_SKILL_LIST info;

	for (int n = 0; n < MAX_SKILL_LIST; n++)
	{
		if (lpObj->Skill[n].IsSkill() == 0)
		{
			continue;
		}

		info.slot = n;

		info.skill = (BYTE)lpObj->Skill[n].m_index;

		info.level = (lpObj->Skill[n].m_level << 3) | ((lpObj->Skill[n].m_index) & 7);

		memcpy(&send[size], &info, sizeof(info));

		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size = size;

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(lpObj->Index, send, size);
}