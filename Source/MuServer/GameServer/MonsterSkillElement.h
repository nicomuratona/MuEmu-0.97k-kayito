#pragma once

#include "MonsterSkillElementInfo.h"

#define MAX_MONSTER_SKILL_ELEMENT_INFO 100

class CMonsterSkillElement
{
public:

	CMonsterSkillElement();

	~CMonsterSkillElement();

	static void LoadData(char* path);

	static void DelAllSkillElement();

	static CMonsterSkillElementInfo* FindSkillElementInfo(int ElementNumber);

public:

	static BOOL m_DataLoad;

	static CMonsterSkillElementInfo m_MonsterSkillElementInfoArray[MAX_MONSTER_SKILL_ELEMENT_INFO];
};