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

			MAP_MANAGER_INFO info;

			info.MapNumber = lpMemScript->GetNumber();

			info.SwimableMap = lpMemScript->GetAsNumber();

			info.MapMovement = lpMemScript->GetAsNumber();

			strcpy_s(info.MapName, lpMemScript->GetAsString());

			strcpy_s(info.SongName, lpMemScript->GetAsString());

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		printf(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CMapManager::SetInfo(MAP_MANAGER_INFO info)
{
	if (info.MapNumber < 0 || info.MapNumber >= MAX_MAPS)
	{
		return;
	}

	this->m_MapManager[info.MapNumber] = info;
}