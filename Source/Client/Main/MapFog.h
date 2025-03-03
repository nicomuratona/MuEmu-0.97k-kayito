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

	void Init(MAP_FOG_INFO* info);

private:

	void SetInfo(MAP_FOG_INFO info);

	void Init();

	MAP_FOG_INFO* GetInfoByIndex(int MapNumber);

	bool GetFogRGB(int MapNumber, float* DayFogRGB, float* NightFogRGB);

	static void glEnableFog();

	static void WINAPI glClearColorFog(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

	void ChangeFog();

private:

	MAP_FOG_INFO m_MapFog[MAX_MAPS];

	DWORD m_LastTickCount = 0;

	int m_LastWorld = -1;

	BYTE m_DefaultDayRGB[3] = { 127, 178, 255 };

	BYTE m_DefaultNightRGB[3] = { 51, 51, 51 };
};

extern CMapFog gMapFog;