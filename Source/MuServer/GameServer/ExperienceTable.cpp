#include "stdafx.h"
#include "ExperienceTable.h"
#include "MemScript.h"
#include "Util.h"

CExperienceTable gExperienceTable;

CExperienceTable::CExperienceTable()
{
	this->m_ExperienceTableInfo.clear();
}

CExperienceTable::~CExperienceTable()
{

}

void CExperienceTable::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == NULL)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (!lpMemScript->SetBuffer(path))
	{
		ErrorMessageBox(lpMemScript->GetLastError());

		delete lpMemScript;

		return;
	}

	this->m_ExperienceTableInfo.clear();

	try
	{
		eTokenResult token;

		while (true)
		{
			token = lpMemScript->GetToken();

			if (token == TOKEN_END || token == TOKEN_END_SECTION)
			{
				break;
			}

			EXPERIENCE_TABLE_INFO info;

			info.MinLevel = lpMemScript->GetNumber();

			info.MaxLevel = lpMemScript->GetAsNumber();

			info.MinReset = lpMemScript->GetAsNumber();

			info.MaxReset = lpMemScript->GetAsNumber();

			info.MinGrandReset = lpMemScript->GetAsNumber();

			info.MaxGrandReset = lpMemScript->GetAsNumber();

			info.ExperienceRate = lpMemScript->GetAsNumber();

			this->m_ExperienceTableInfo.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

int CExperienceTable::GetExperienceRate(LPOBJ lpObj)
{
	for (std::vector<EXPERIENCE_TABLE_INFO>::iterator it = this->m_ExperienceTableInfo.begin(); it != this->m_ExperienceTableInfo.end(); it++)
	{
		if (it->MinLevel != -1 && it->MinLevel > lpObj->Level)
		{
			continue;
		}

		if (it->MaxLevel != -1 && it->MaxLevel < lpObj->Level)
		{
			continue;
		}

		if (it->MinReset != -1 && it->MinReset > lpObj->Reset)
		{
			continue;
		}

		if (it->MaxReset != -1 && it->MaxReset < lpObj->Reset)
		{
			continue;
		}

		if (it->MinGrandReset != -1 && it->MinGrandReset > lpObj->GrandReset)
		{
			continue;
		}

		if (it->MaxGrandReset != -1 && it->MaxGrandReset < lpObj->GrandReset)
		{
			continue;
		}

		return it->ExperienceRate;
	}

	return 100;
}