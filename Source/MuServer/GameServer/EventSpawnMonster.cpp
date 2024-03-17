#include "stdafx.h"
#include "EventSpawnMonster.h"
#include "MemScript.h"
#include "Util.h"

CEventSpawnMonster gEventSpawnMonster;

CEventSpawnMonster::CEventSpawnMonster()
{
	this->m_EventSpawnMonsterInfo.clear();
}

CEventSpawnMonster::~CEventSpawnMonster()
{

}

void CEventSpawnMonster::Load(char* path)
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

	this->m_EventSpawnMonsterInfo.clear();

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

			int Event = lpMemScript->GetNumber();

			while (true)
			{
				token = lpMemScript->GetToken();

				if (token == TOKEN_END || token == TOKEN_END_SECTION)
				{
					break;
				}

				EVENT_SPAWN_MONSTER_INFO info;

				info.Event = Event;

				info.Level = lpMemScript->GetNumber();

				info.Stage = lpMemScript->GetAsNumber();

				info.Monster = lpMemScript->GetAsNumber();

				info.MaxRegenTime = lpMemScript->GetAsNumber();

				this->m_EventSpawnMonsterInfo.push_back(info);
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}