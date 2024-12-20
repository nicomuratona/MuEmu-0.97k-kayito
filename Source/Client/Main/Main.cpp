#include "stdafx.h"
#include "Buff.h"
#include "ChaosMix.h"
#include "Controller.h"
#include "Font.h"
#include "GuildLogo.h"
#include "HackCheck.h"
#include "HealthBar.h"
#include "Interface.h"
#include "Item.h"
#include "ItemPosition.h"
#include "ItemStack.h"
#include "ItemValue.h"
#include "Language.h"
#include "LoadModels.h"
#include "MiniMap.h"
#include "MoveList.h"
#include "OptionsMenu.h"
#include "PacketManager.h"
#include "Patchs.h"
#include "PingSystem.h"
#include "PrintPlayer.h"
#include "Protect.h"
#include "Protocol.h"
#include "Reconnect.h"
#include "RightClickMove.h"
#include "SkyDome.h"
#include "Sound.h"
#include "WeaponView.h"
#include "Window.h"

HINSTANCE hins;

extern "C" _declspec(dllexport) void _cdecl EntryProc()
{
	setlocale(LC_ALL, "es_ES.1252");

	if (gProtect.ReadMainFile("Data\\Local\\ClientInfo.bmd") == false)
	{
		MessageBoxA(NULL, "Read file incorrect or not exists", "Error", MB_OK);

		ExitProcess(0);
	}

#if (_DEBUG)

	gConsole.StartConsole();

#endif

	gProtect.CheckLauncher();

	gProtect.CheckInstance();

	if (!gPacketManager.LoadEncryptionKey("Data\\Enc1.dat") || !gPacketManager.LoadDecryptionKey("Data\\Dec2.dat"))
	{
		MessageBoxA(NULL, "Failed reading encoder and decoder files.", "Error", MB_OK);

		ExitProcess(0);
	}

	gHackCheck.Init();

	gPatchs.Init();

	gBuff.Init();

	gProtocol.Init();

	gLanguage.Init();

	gFont.Init();

	gPrintPlayer.Init();

	gReconnect.Init();

	gPing.Init();

	gSound.Init();

	gOptionsMenu.Init();

	gInterface.Init();

	gHealthBar.Init();

	gSkyDome.Init();

	gMiniMap.Init();

	gWeaponView.Init();

	gGuildLogo.Init();

	gRightClickMove.Init();

	gChaosMix.Init();

	gItemStack.Init();

	gItemOption.Load(gProtect.m_MainInfo.ItemOptionInfo);

	gItem.Init();

	gMapManager.Load(gProtect.m_MainInfo.MapManager);

	gLoadModels.Init();

	gCustomGlow.Load(gProtect.m_MainInfo.CustomGlowInfo);

	gCustomItem.Load(gProtect.m_MainInfo.CustomItemInfo);

	gCustomBow.Load(gProtect.m_MainInfo.CustomBowInfo);

	gCustomWing.Load(gProtect.m_MainInfo.CustomWingInfo);

	gCustomMonster.Load(gProtect.m_MainInfo.CustomMonsterInfo);

	gItemPosition.Load(gProtect.m_MainInfo.ItemPositionInfo);

	gProtect.CheckClientFile();

	gProtect.CheckPluginFile();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			hins = (HINSTANCE)hModule;

			gWindow.WindowModeLoad(hins);

			gController.Load(hins);

			break;
		}

		case DLL_PROCESS_DETACH:
		{
			break;
		}

		case DLL_THREAD_ATTACH:
		{
			break;
		}

		case DLL_THREAD_DETACH:
		{
			break;
		}
	}

	return 1;
}