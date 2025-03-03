#pragma once

struct MAP_FOG_INFO
{
	int MapNumber;
	BYTE DayRGB[3];
	BYTE NightRGB[3];
};

class CMapFog
{
public:

	CMapFog();

	virtual ~CMapFog();

	void Init();

	void Load(char* path);

	void SetInfo(MAP_FOG_INFO info);

public:

	MAP_FOG_INFO m_MapFog[MAX_MAPS];
};

extern CMapFog gMapFog;