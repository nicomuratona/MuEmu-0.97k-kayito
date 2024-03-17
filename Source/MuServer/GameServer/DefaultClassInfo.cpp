#include "stdafx.h"
#include "DefaultClassInfo.h"
#include "MemScript.h"
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

	this->Init();

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

			DEFAULT_CLASS_INFO info;

			info.Class = lpMemScript->GetNumber();

			info.Strength = lpMemScript->GetAsNumber();

			info.Dexterity = lpMemScript->GetAsNumber();

			info.Vitality = lpMemScript->GetAsNumber();

			info.Energy = lpMemScript->GetAsNumber();

			info.MaxLife = lpMemScript->GetAsFloatNumber();

			info.MaxMana = lpMemScript->GetAsFloatNumber();

			info.LevelLife = lpMemScript->GetAsFloatNumber();

			info.LevelMana = lpMemScript->GetAsFloatNumber();

			info.VitalityToLife = lpMemScript->GetAsFloatNumber();

			info.EnergyToMana = lpMemScript->GetAsFloatNumber();

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
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