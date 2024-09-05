#pragma once

#include "MapManager.h"
#include "CustomGlow.h"
#include "CustomItem.h"
#include "ItemOption.h"
#include "CustomMonster.h"

struct MAIN_FILE_INFO
{
	char CustomerName[32];
	BYTE LauncherType;
	char LauncherName[32];
	char IpAddress[32];
	WORD IpAddressPort;
	char ClientSerial[17];
	char ClientVersion[8];
	BYTE MultiInstanceBlock;
	char WindowName[32];
	char ScreenShotPath[50];
	char ClientName[32];
	char PluginName[32];
	DWORD ClientCRC32;
	DWORD PluginCRC32;
	DWORD ReconnectTime;
	BYTE HealthBarType;
	BYTE EnableMoveList;
	BYTE EnableSpecialCharacters;
	MAP_MANAGER_INFO MapManager[MAX_MAPS];
	CUSTOM_GLOW_INFO CustomGlowInfo[MAX_ITEM];
	CUSTOM_ITEM_INFO CustomItemInfo[MAX_ITEM];
	ITEM_OPTION_INFO ItemOptionInfo[MAX_ITEM_OPTION_INFO];
	CUSTOM_MONSTER_INFO CustomMonsterInfo[MAX_MONSTER];
};

class CProtect
{
public:

	CProtect();

	virtual ~CProtect();

	bool ReadMainFile(char* name);

	void CheckLauncher();

	void CheckInstance();

	void CheckClientFile();

	void CheckPluginFile();

public:

	MAIN_FILE_INFO m_MainInfo;

	DWORD m_ClientFileCRC;
};

extern CProtect gProtect;