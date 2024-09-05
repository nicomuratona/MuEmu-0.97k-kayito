#include "stdafx.h"
#include "CustomMonster.h"

CCustomMonster gCustomMonster;

CCustomMonster::CCustomMonster()
{
	this->Init();
}

CCustomMonster::~CCustomMonster()
{

}

void CCustomMonster::Init()
{
	for (int n = 0; n < MAX_MONSTER; n++)
	{
		this->m_CustomMonsterInfo[n].Index = -1;
	}
}

void CCustomMonster::Load(char* path)
{
	CReadScript* lpReadScript = new CReadScript;

	if (lpReadScript == NULL)
	{
		printf(READ_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (!lpReadScript->Load(path))
	{
		printf(READ_SCRIPT_FILE_ERROR, path);

		delete lpReadScript;

		return;
	}

	int Index = 0;

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

			CUSTOM_MONSTER_INFO info;

			info.Index = Index++;

			info.MonsterIndex = lpReadScript->GetNumber();

			info.MonsterType = lpReadScript->GetAsNumber();

			info.GoldenType = (lpReadScript->GetAsNumber() != 0);

			info.Scale = lpReadScript->GetAsFloatNumber();

			strcpy_s(info.FolderName, lpReadScript->GetAsString());

			strcpy_s(info.ModelName, lpReadScript->GetAsString());

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		printf(lpReadScript->GetError());
	}

	delete lpReadScript;
}

void CCustomMonster::SetInfo(CUSTOM_MONSTER_INFO info)
{
	if (info.Index < 0 || info.Index >= MAX_MONSTER)
	{
		return;
	}

	this->m_CustomMonsterInfo[info.Index] = info;
}