#include "stdafx.h"
#include "ExperienceTable.h"
#include "ReadScript.h"
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

	this->m_ExperienceTableInfo.clear();

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

			EXPERIENCE_TABLE_INFO info;

			info.MinLevel = lpReadScript->GetNumber();

			info.MaxLevel = lpReadScript->GetAsNumber();

			info.MinReset = lpReadScript->GetAsNumber();

			info.MaxReset = lpReadScript->GetAsNumber();

			info.MinGrandReset = lpReadScript->GetAsNumber();

			info.MaxGrandReset = lpReadScript->GetAsNumber();

			info.ExperienceRate = lpReadScript->GetAsNumber();

			this->m_ExperienceTableInfo.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript->GetError());
	}

	delete lpReadScript;
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