#include "stdafx.h"
#include "MonsterSkillElement.h"
#include "MemScript.h"
#include "Util.h"

BOOL CMonsterSkillElement::m_DataLoad;

CMonsterSkillElementInfo CMonsterSkillElement::m_MonsterSkillElementInfoArray[MAX_MONSTER_SKILL_ELEMENT_INFO];

CMonsterSkillElement::CMonsterSkillElement()
{
	CMonsterSkillElement::DelAllSkillElement();
}

CMonsterSkillElement::~CMonsterSkillElement()
{

}

void CMonsterSkillElement::LoadData(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (lpMemScript->SetBuffer(path) == false)
	{
		ErrorMessageBox(lpMemScript->GetLastError());

		delete lpMemScript;

		return;
	}

	CMonsterSkillElement::DelAllSkillElement();

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while (true)
			{
				if (section == 0)
				{
					if (strcmp("end", lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					CMonsterSkillElementInfo info;

					info.m_ElementNumber = lpMemScript->GetNumber();

					strcpy_s(info.m_ElementName, lpMemScript->GetAsString());

					info.m_ElementType = lpMemScript->GetAsNumber();

					info.m_SuccessRate = lpMemScript->GetAsNumber();

					info.m_ContinuanceTime = lpMemScript->GetAsNumber();

					info.m_IncAndDecType = lpMemScript->GetAsNumber();

					info.m_IncAndDecValue = lpMemScript->GetAsNumber();

					info.m_NullifiedSkill = lpMemScript->GetAsNumber();

					info.m_CharacterClass = lpMemScript->GetAsNumber();

					info.m_CharacterLevel = lpMemScript->GetAsNumber();

					if (info.m_ElementNumber >= 0 && info.m_ElementNumber < MAX_MONSTER_SKILL_ELEMENT_INFO)
					{
						CMonsterSkillElement::m_MonsterSkillElementInfoArray[info.m_ElementNumber] = info;
					}
				}
				else
				{
					break;
				}
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	CMonsterSkillElement::m_DataLoad = 1;

	delete lpMemScript;
}

void CMonsterSkillElement::DelAllSkillElement()
{
	CMonsterSkillElement::m_DataLoad = 0;

	for (int n = 0; n < MAX_MONSTER_SKILL_ELEMENT_INFO; n++)
	{
		CMonsterSkillElement::m_MonsterSkillElementInfoArray[n].Reset();
	}
}

CMonsterSkillElementInfo* CMonsterSkillElement::FindSkillElementInfo(int ElementNumber)
{
	if (ElementNumber < 0 || ElementNumber >= MAX_MONSTER_SKILL_ELEMENT_INFO)
	{
		return 0;
	}

	if (CMonsterSkillElement::m_MonsterSkillElementInfoArray[ElementNumber].m_ElementNumber == ElementNumber)
	{
		return &CMonsterSkillElement::m_MonsterSkillElementInfoArray[ElementNumber];
	}

	return 0;
}