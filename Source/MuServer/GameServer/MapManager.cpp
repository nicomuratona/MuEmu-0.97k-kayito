#include "stdafx.h"
#include "MapManager.h"
#include "MemScript.h"
#include "ServerInfo.h"
#include "Util.h"

CMapManager gMapManager;

CMapManager::CMapManager()
{
	this->m_MapManagerInfo.clear();
}

CMapManager::~CMapManager()
{

}

void CMapManager::Load(char* path)
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

	this->m_MapManagerInfo.clear();

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

			memset(&info, 0, sizeof(info));

			info.Index = lpMemScript->GetNumber();

			info.NonPK = lpMemScript->GetAsNumber();

			info.ViewRange = lpMemScript->GetAsNumber();

			info.ExperienceRate = lpMemScript->GetAsNumber();

			info.ItemDropRate = lpMemScript->GetAsNumber();

			info.ExcItemDropRate = lpMemScript->GetAsNumber();

			info.DeadGate = lpMemScript->GetAsNumber();

			info.RespawnInPlace = lpMemScript->GetAsNumber();

			info.FlyingDragons = lpMemScript->GetAsNumber();

			strcpy_s(info.MapName, lpMemScript->GetAsString());

			this->m_MapManagerInfo.insert(std::pair<int, MAP_MANAGER_INFO>(info.Index, info));
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

int CMapManager::GetMapNonPK(int index)
{
	std::map<int, MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if (it == this->m_MapManagerInfo.end())
	{
		return gServerInfo.m_NonPK;
	}
	else
	{
		return ((it->second.NonPK == -1) ? gServerInfo.m_NonPK : ((it->second.NonPK == 1) ? 1 : 0));
	}
}

int CMapManager::GetMapNonOutlaw(int index)
{
	std::map<int, MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if (it == this->m_MapManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return ((it->second.NonPK == -1) ? 0 : ((it->second.NonPK == 2) ? 1 : 0));
	}
}

int CMapManager::GetMapViewRange(int index)
{
	std::map<int, MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if (it == this->m_MapManagerInfo.end())
	{
		return 15;
	}
	else
	{
		return ((it->second.ViewRange == -1) ? 15 : it->second.ViewRange);
	}
}

int CMapManager::GetMapExperienceRate(int index)
{
	std::map<int, MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if (it == this->m_MapManagerInfo.end())
	{
		return 100;
	}
	else
	{
		return ((it->second.ExperienceRate == -1) ? 100 : it->second.ExperienceRate);
	}
}

int CMapManager::GetMapItemDropRate(int index)
{
	std::map<int, MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if (it == this->m_MapManagerInfo.end())
	{
		return 100;
	}
	else
	{
		return ((it->second.ItemDropRate == -1) ? 100 : it->second.ItemDropRate);
	}
}

int CMapManager::GetMapExcItemDropRate(int index)
{
	std::map<int, MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if (it == this->m_MapManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return ((it->second.ExcItemDropRate == -1) ? 0 : it->second.ExcItemDropRate);
	}
}

int CMapManager::GetMapDeadGate(int index)
{
	std::map<int, MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if (it == this->m_MapManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return it->second.DeadGate;
	}
}

int CMapManager::GetMapRespawnInPlace(int index)
{
	std::map<int, MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if (it == this->m_MapManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return it->second.RespawnInPlace;
	}
}

int CMapManager::GetMapFlyingDragons(int index)
{
	std::map<int, MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if (it == this->m_MapManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return it->second.FlyingDragons;
	}
}

char* CMapManager::GetMapName(int index)
{
	std::map<int, MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if (it == this->m_MapManagerInfo.end())
	{
		return "Not Found";
	}
	else
	{
		return it->second.MapName;
	}
}