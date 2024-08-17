#include "stdafx.h"
#include "EventSpawnMonster.h"
#include "ReadScript.h"
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

	this->m_EventSpawnMonsterInfo.clear();

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

			int Event = lpReadScript->GetNumber();

			while (true)
			{
				token = lpReadScript->GetToken();

				if (token == TOKEN_END || token == TOKEN_END_SECTION)
				{
					break;
				}

				EVENT_SPAWN_MONSTER_INFO info;

				info.Event = Event;

				info.Level = lpReadScript->GetNumber();

				info.Stage = lpReadScript->GetAsNumber();

				info.Monster = lpReadScript->GetAsNumber();

				info.MaxRegenTime = lpReadScript->GetAsNumber();

				this->m_EventSpawnMonsterInfo.push_back(info);
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript->GetError());
	}

	delete lpReadScript;
}