#pragma once

#include "MapManager.h"
#include "CustomGlow.h"
#include "CustomItem.h"
#include "ItemOption.h"
#include "CustomMonster.h"
#include "CustomBow.h"
#include "CustomWing.h"
#include "ItemPosition.h"
#include "MapFog.h"

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
	char WindowName[128];
	char ScreenShotPath[256];
	BYTE DisableResets;
	BYTE DisableGrandResets;
	char ClientName[128];
	char PluginName[128];
	DWORD ClientCRC32;
	DWORD PluginCRC32;
	DWORD ReconnectTime;
	BYTE HealthBarType;
	BYTE EnableMoveList;
	BYTE EnableSpecialCharacters;
	MAP_MANAGER_INFO MapManager[MAX_MAPS];
	CUSTOM_GLOW_INFO CustomGlowInfo[MAX_ITEM];
	CUSTOM_ITEM_INFO CustomItemInfo[MAX_ITEM];
	CUSTOM_BOW_INFO CustomBowInfo[MAX_CUSTOM_BOW];
	CUSTOM_WING_INFO CustomWingInfo[MAX_CUSTOM_WING];
	ITEM_OPTION_INFO ItemOptionInfo[MAX_ITEM_OPTION_INFO];
	CUSTOM_MONSTER_INFO CustomMonsterInfo[MAX_MONSTER];
	ITEM_POSITION_INFO ItemPositionInfo[MAX_ITEM];
	MAP_FOG_INFO MapFogInfo[MAX_MAPS];

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