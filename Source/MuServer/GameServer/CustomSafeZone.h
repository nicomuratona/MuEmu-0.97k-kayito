#pragma once

struct SAFE_ZONE_INFO
{
	int Map;
	int X;
	int Y;
	int TX;
	int TY;
};

class CSafeZone
{
public:

	CSafeZone();

	~CSafeZone();

	void Load(char* path);

	bool CheckSafeZone(int map, int x, int y);

private:

	std::vector<SAFE_ZONE_INFO> m_SafeZoneInfo;
};

extern CSafeZone gSafeZone;