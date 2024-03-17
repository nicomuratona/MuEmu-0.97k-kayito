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

	void Load(MAP_MANAGER_INFO* info);

	void SetInfo(MAP_MANAGER_INFO info);

	void Init();

	//Funciones

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