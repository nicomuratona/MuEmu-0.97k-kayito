#include "stdafx.h"
#include "MapManager.h"

CMapManager gMapManager;

CMapManager::CMapManager()
{
	this->Init();
}

CMapManager::~CMapManager()
{

}

void CMapManager::Init()
{
	for (int i = 0; i < MAX_MAPS; i++)
	{
		this->m_MapManager[i].MapNumber = -1;
	}
}

void CMapManager::Load(char* path)
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

			MAP_MANAGER_INFO info;

			info.MapNumber = lpReadScript->GetNumber();

			info.SwimableMap = (lpReadScript->GetAsNumber() != 0);

			info.MapMovement = (lpReadScript->GetAsNumber() != 0);

			info.MiniMap = lpReadScript->GetAsNumber();

			strcpy_s(info.MapName, lpReadScript->GetAsString());

			strcpy_s(info.SongName, lpReadScript->GetAsString());

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		printf(lpReadScript->GetError());
	}

	delete lpReadScript;
}

void CMapManager::SetInfo(MAP_MANAGER_INFO info)
{
	if (info.MapNumber < 0 || info.MapNumber >= MAX_MAPS)
	{
		return;
	}

	this->m_MapManager[info.MapNumber] = info;
}