#include "stdafx.h"
#include "Skill.h"
#include "SkillManager.h"

CSkill::CSkill()
{
	this->Clear();
}

CSkill::~CSkill()
{

}

void CSkill::Clear()
{
	this->m_level = 0;

	this->m_skill = 0xFF;

	this->m_index = 0xFF;

	this->m_DamageMin = 0;

	this->m_DamageMax = 0;
}

bool CSkill::IsSkill()
{
	if (this->m_index == 0xFF)
	{
		return 0;
	}

	return 1;
}

bool CSkill::Set(int index, int level)
{
	if (this->IsSkill() != 0 && index < 300)
	{
		return 0;
	}

	this->m_level = level;

	this->m_skill = index;

	this->m_index = index;

	this->m_DamageMin = gSkillManager.GetSkillDamage(index);

	this->m_DamageMax = this->m_DamageMin + (this->m_DamageMin / 2);

	return 1;
}