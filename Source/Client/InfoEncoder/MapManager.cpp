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

	if (lpMemScript == 0)
	{
		printf(MEM_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		printf(lpMemScript->GetLastError());

		delete lpMemScript;

		return;
	}

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if (strcmp("end", lpMemScript->GetString()) == 0)
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