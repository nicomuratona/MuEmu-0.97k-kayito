#include "stdafx.h"
#include "GameMaster.h"
#include "ReadScript.h"
#include "Util.h"

CGameMaster gGameMaster;

CGameMaster::CGameMaster()
{
	this->m_count = 0;
}

CGameMaster::~CGameMaster()
{

}

void CGameMaster::Load(char* path)
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

	this->m_count = 0;

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

			GAME_MASTER_INFO info;

			memset(&info, 0, sizeof(info));

			strcpy_s(info.Account, lpReadScript->GetString());

			strcpy_s(info.Name, lpReadScript->GetAsString());

			info.Level = lpReadScript->GetAsNumber();

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript->GetError());
	}

	delete lpReadScript;
}

void CGameMaster::SetInfo(GAME_MASTER_INFO info)
{
	if (this->m_count < 0 || this->m_count >= MAX_GAME_MASTER)
	{
		return;
	}

	this->m_GameMasterInfo[this->m_count++] = info;
}

void CGameMaster::SetGameMasterLevel(LPOBJ lpObj, int level)
{
	for (int n = 0; n < this->m_count; n++)
	{
		if (strcmp(this->m_GameMasterInfo[n].Account, lpObj->Account) == 0 && strcmp(this->m_GameMasterInfo[n].Name, lpObj->Name) == 0)
		{
			return;
		}
	}

	GAME_MASTER_INFO info;

	memcpy(info.Account, lpObj->Account, sizeof(info.Account));

	memcpy(info.Name, lpObj->Name, sizeof(info.Name));

	info.Level = level;

	this->SetInfo(info);
}

bool CGameMaster::CheckGameMasterLevel(LPOBJ lpObj, int level)
{
	for (int n = 0; n < this->m_count; n++)
	{
		if (strcmp(this->m_GameMasterInfo[n].Account, lpObj->Account) == 0 && strcmp(this->m_GameMasterInfo[n].Name, lpObj->Name) == 0 && this->m_GameMasterInfo[n].Level >= level)
		{
			return true;
		}
	}

	return false;
}