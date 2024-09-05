#pragma once

struct MAP_MANAGER_INFO
{
	int MapNumber;
	bool SwimableMap;
	bool MapMovement;
	BYTE MiniMap;
	bool SkyDome;
	char MapName[32];
	char SongName[64];
};

class CMapManager
{
public:

	CMapManager();

	virtual ~CMapManager();

	void Load(MAP_MANAGER_INFO* info);

	void SetInfo(MAP_MANAGER_INFO info);

	void Init();

	//Funciones

	BYTE GetMiniMap(int map);

	bool GetSkyDome(int map);

	static char* GetMapName(int MapNumber);

	static void GetPartyMapName();

	static void ApplyMapMovement();

	static void LoadMapMusic();

	static void SwimAnimationStart();

	static void SwimAnimationStop();

	static void SwimSoundPlay();

	static void SwimBackItemRender();

private:

	MAP_MANAGER_INFO m_MapManager[MAX_MAPS];
};

extern CMapManager gMapManager;