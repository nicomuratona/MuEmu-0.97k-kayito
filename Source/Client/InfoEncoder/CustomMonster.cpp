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
		this->m_CustomMonsterInfo[n].MonsterIndex = -1;
	}
}

void CCustomMonster::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == NULL)
	{
		printf(MEM_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (!lpMemScript->SetBuffer(path))
	{
		printf(lpMemScript->GetLastError());

		delete lpMemScript;

		return;
	}

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

			CUSTOM_MONSTER_INFO info;

			info.MonsterIndex = lpMemScript->GetNumber();

			info.MonsterType = lpMemScript->GetAsNumber();

			info.GoldenType = lpMemScript->GetAsNumber();

			info.Scale = lpMemScript->GetAsFloatNumber();

			strcpy_s(info.FolderName, lpMemScript->GetAsString());

			strcpy_s(info.ModelName, lpMemScript->GetAsString());

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		printf(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CCustomMonster::SetInfo(CUSTOM_MONSTER_INFO info)
{
	if (info.MonsterIndex < 0 || info.MonsterIndex >= MAX_MONSTER)
	{
		return;
	}

	this->m_CustomMonsterInfo[info.MonsterIndex] = info;
}