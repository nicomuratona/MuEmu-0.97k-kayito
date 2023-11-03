#pragma once

#include "MapItem.h"
#include "MapPath.h"

#define MAX_MAP 32
#define MAX_MAP_ITEM 300
#define MAX_MAP_WIDTH 256
#define MAX_MAP_HEIGHT 256
#define TERRAIN_SIZE_MASK 255
#define TERRAIN_SIZE 256

#define MAP_ATTR_NONE		0x00 // Map attribute - none
#define MAP_ATTR_SAFTYZONE	0x01 // Map attribute - safe zone
#define MAP_ATTR_STAND		0x02 // Map attribute - pvp zone
#define MAP_ATTR_BLOCK		0x04 // Map attribute - wall
#define MAP_ATTR_HOLLOW		0x08 // Map attribute - ground crack
#define MAP_ATTR_WATER		0x10 // Map attribute - water

#define MAP_RANGE(x) (((x)<0)?0:((x)>=MAX_MAP)?0:1)
#define MAP_ITEM_RANGE(x) (((x)<0)?0:((x)>=MAX_MAP_ITEM)?0:1)

#define DS_MAP_RANGE(x) (((x)==MAP_DEVIL_SQUARE1)?1:0)
#define BC_MAP_RANGE(x) (((x)<MAP_BLOOD_CASTLE1)?0:((x)>MAP_BLOOD_CASTLE6)?0:1)

enum eMapNumber
{
	MAP_LORENCIA = 0,
	MAP_DUNGEON = 1,
	MAP_DEVIAS = 2,
	MAP_NORIA = 3,
	MAP_LOST_TOWER = 4,
	MAP_RESERVED1 = 5,
	MAP_ARENA = 6,
	MAP_ATLANS = 7,
	MAP_TARKAN = 8,
	MAP_DEVIL_SQUARE1 = 9,
	MAP_ICARUS = 10,
	MAP_BLOOD_CASTLE1 = 11,
	MAP_BLOOD_CASTLE2 = 12,
	MAP_BLOOD_CASTLE3 = 13,
	MAP_BLOOD_CASTLE4 = 14,
	MAP_BLOOD_CASTLE5 = 15,
	MAP_BLOOD_CASTLE6 = 16,
};

class CMap
{
public:

	CMap();

	~CMap();

	void Load(char* path, int map);

	BYTE GetAttr(int x, int y);

	bool CheckAttr(int x, int y, BYTE attr);

	void SetAttr(int x, int y, BYTE attr);

	void DelAttr(int x, int y, BYTE attr);

	bool CheckStandAttr(int x, int y);

	void SetStandAttr(int x, int y);

	void DelStandAttr(int x, int y);

	BYTE GetWeather();

	void SetWeather(BYTE weather, BYTE variation);

	void WeatherVariationProcess();

	void WeatherAllSend(BYTE weather);

	void GetMapPos(int map, short* ox, short* oy);

	void GetMapRandomPos(short* ox, short* oy, int size);

	bool MoneyItemDrop(int money, int x, int y);

	bool MonsterItemDrop(int index, int level, float dur, int x, int y, BYTE SkillOption, BYTE LuckOption, BYTE AddOption, BYTE ExceOption, int aIndex, DWORD serial);

	bool ItemDrop(int index, int level, float dur, int x, int y, BYTE SkillOption, BYTE LuckOption, BYTE AddOption, BYTE ExceOption, DWORD serial, int aIndex);

	bool CheckItemGive(int aIndex, int index);

	void ItemGive(int aIndex, int index);

	void StateSetDestroy();

	BOOL CheckWall(int sx, int sy, int tx, int ty);

	BYTE CheckWall2(int sx, int sy, int tx, int ty);

	bool PathFinding2(int sx, int sy, int tx, int ty, PATH_INFO* path);

	bool PathFinding3(int sx, int sy, int tx, int ty, PATH_INFO* path);

	bool PathFinding4(int sx, int sy, int tx, int ty, PATH_INFO* path);

public:

	BYTE m_Weather;

	BYTE m_WeatherVariation;

	DWORD m_WeatherTimer;

	DWORD m_NextWeatherTimer;

	BYTE* m_MapAttr;

	int m_width;

	int m_height;

	CMapPath* m_path;

	CMapItem m_Item[MAX_MAP_ITEM];

	RECT m_MapRespawn[MAX_MAP];

	int m_MapNumber;

	int m_ItemCount;
};

extern CMap gMap[MAX_MAP];