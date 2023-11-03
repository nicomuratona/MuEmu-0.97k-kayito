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

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());

		delete lpMemScript;

		return;
	}

	this->m_EventSpawnMonsterInfo.clear();

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int Event = lpMemScript->GetNumber();

			while (true)
			{
				if (strcmp("end", lpMemScript->GetAsString()) == 0)
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