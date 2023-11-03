#include "stdafx.h"
#include "MonsterAIElement.h"
#include "MemScript.h"
#include "Path.h"
#include "Util.h"

BOOL CMonsterAIElement::m_DataLoad;

CMonsterAIElementInfo CMonsterAIElement::m_MonsterAIElementInfoArray[MAX_MONSTER_AI_ELEMENT_INFO];

CMonsterAIMovePath CMonsterAIElement::m_MonsterAIMovePath[MAX_MAP];

CMonsterAIElement::CMonsterAIElement()
{
	CMonsterAIElement::DelAllAIElement();
}

CMonsterAIElement::~CMonsterAIElement()
{

}

void CMonsterAIElement::LoadData(char* path)
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

	CMonsterAIElement::DelAllAIElement();

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

					CMonsterAIElementInfo info;

					info.m_ElementNumber = lpMemScript->GetNumber();

					strcpy_s(info.m_ElementName, lpMemScript->GetAsString());

					info.m_ElementClass = lpMemScript->GetAsNumber();

					info.m_ElementType = lpMemScript->GetAsNumber();

					info.m_SuccessRate = lpMemScript->GetAsNumber();

					info.m_DelayTime = lpMemScript->GetAsNumber();

					info.m_TargetType = lpMemScript->GetAsNumber();

					info.m_X = lpMemScript->GetAsNumber();

					info.m_Y = lpMemScript->GetAsNumber();

					if (info.m_ElementNumber >= 0 && info.m_ElementNumber < MAX_MONSTER_AI_ELEMENT_INFO)
					{
						CMonsterAIElement::m_MonsterAIElementInfoArray[info.m_ElementNumber] = info;
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

	CMonsterAIElement::m_DataLoad = 1;

	delete lpMemScript;
}

void CMonsterAIElement::DelAllAIElement()
{
	CMonsterAIElement::m_DataLoad = 0;

	for (int n = 0; n < MAX_MONSTER_AI_ELEMENT_INFO; n++)
	{
		CMonsterAIElement::m_MonsterAIElementInfoArray[n].Reset();
	}
}

CMonsterAIElementInfo* CMonsterAIElement::FindAIElementInfo(int ElementNumber)
{
	if (ElementNumber < 0 || ElementNumber >= MAX_MONSTER_AI_ELEMENT_INFO)
	{
		return 0;
	}

	if (CMonsterAIElement::m_MonsterAIElementInfoArray[ElementNumber].m_ElementNumber == ElementNumber)
	{
		return &CMonsterAIElement::m_MonsterAIElementInfoArray[ElementNumber];
	}

	return 0;
}