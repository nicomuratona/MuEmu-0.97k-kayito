#include "stdafx.h"
#include "DefaultClassInfo.h"
#include "ReadScript.h"
#include "Util.h"

CDefaultClassInfo gDefaultClassInfo;

CDefaultClassInfo::CDefaultClassInfo()
{
	this->Init();
}

CDefaultClassInfo::~CDefaultClassInfo()
{

}

void CDefaultClassInfo::Init()
{
	memset(this->m_DefaultClassInfo, 0, sizeof(this->m_DefaultClassInfo));
}

void CDefaultClassInfo::Load(char* path)
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

			DEFAULT_CLASS_INFO info;

			info.Class = lpReadScript->GetNumber();

			info.Strength = lpReadScript->GetAsNumber();

			info.Dexterity = lpReadScript->GetAsNumber();

			info.Vitality = lpReadScript->GetAsNumber();

			info.Energy = lpReadScript->GetAsNumber();

			info.MaxLife = lpReadScript->GetAsFloatNumber();

			info.MaxMana = lpReadScript->GetAsFloatNumber();

			info.LevelLife = lpReadScript->GetAsFloatNumber();

			info.LevelMana = lpReadScript->GetAsFloatNumber();

			info.VitalityToLife = lpReadScript->GetAsFloatNumber();

			info.EnergyToMana = lpReadScript->GetAsFloatNumber();

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript->GetError());
	}

	delete lpReadScript;
}

void CDefaultClassInfo::SetInfo(DEFAULT_CLASS_INFO info)
{
	if (CHECK_RANGE(info.Class, MAX_CLASS) == false)
	{
		return;
	}

	this->m_DefaultClassInfo[info.Class] = info;
}

int CDefaultClassInfo::GetCharacterDefaultStat(int Class, int stat)
{
	if (CHECK_RANGE(Class, MAX_CLASS) == false)
	{
		return 0;
	}

	switch (stat)
	{
		case 0:
		{
			return this->m_DefaultClassInfo[Class].Strength;
		}

		case 1:
		{
			return this->m_DefaultClassInfo[Class].Dexterity;
		}

		case 2:
		{
			return this->m_DefaultClassInfo[Class].Vitality;
		}

		case 3:
		{
			return this->m_DefaultClassInfo[Class].Energy;
		}
	}

	return 0;
}