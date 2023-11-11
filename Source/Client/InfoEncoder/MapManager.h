#pragma once

struct MAP_MANAGER_INFO
{
	int MapNumber;
	int SwimableMap;
	int MapMovement;
	char MapName[32];
	char SongName[64];
};

class CMapManager
{
public:

	CMapManager();

	virtual ~CMapManager();

	void Init();

	void Load(char* path);

	void SetInfo(MAP_MANAGER_INFO info);

public:

	MAP_MANAGER_INFO m_MapManager[MAX_MAPS];
};

extern CMapManager gMapManager;