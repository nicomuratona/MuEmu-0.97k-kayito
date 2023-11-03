#pragma once

struct MAP_MANAGER_INFO
{
	int Index;
	int NonPK;
	int ViewRange;
	int ExperienceRate;
	int ItemDropRate;
	int ExcItemDropRate;
	int DeadGate;
	int RespawnInPlace;
	int FlyingDragons;
	char MapName[32];
};

class CMapManager
{
public:

	CMapManager();

	~CMapManager();

	void Load(char* path);

	int GetMapNonPK(int index);

	int GetMapNonOutlaw(int index);

	int GetMapViewRange(int index);

	int GetMapExperienceRate(int index);

	int GetMapItemDropRate(int index);

	int GetMapExcItemDropRate(int index);

	int GetMapDeadGate(int index);

	int GetMapRespawnInPlace(int index);

	int GetMapFlyingDragons(int index);

	char* GetMapName(int index);

public:

	std::map<int, MAP_MANAGER_INFO> m_MapManagerInfo;
};

extern CMapManager gMapManager;