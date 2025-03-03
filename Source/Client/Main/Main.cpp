#include "stdafx.h"
#include "Buff.h"
#include "ChaosMix.h"
#include "Controller.h"
#include "MapFog.h"
#include "Font.h"
#include "GoldenArcher.h"
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

	gWindow.Init(hins);

	gController.Init(hins);

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

	gInterface.Init();

	gHealthBar.Init();

	gOptionsMenu.Init();

	gMapFog.Init(gProtect.m_MainInfo.MapFogInfo);

	gMiniMap.Init();

	gWeaponView.Init();

	gGuildLogo.Init();

	gRightClickMove.Init();

	gChaosMix.Init();

	gGoldenArcher.Init();

	gItemStack.Init();

	gItemOption.Init(gProtect.m_MainInfo.ItemOptionInfo);

	gItem.Init();

	gMapManager.Init(gProtect.m_MainInfo.MapManager);

	gLoadModels.Init();

	gCustomGlow.Init(gProtect.m_MainInfo.CustomGlowInfo);

	gCustomItem.Init(gProtect.m_MainInfo.CustomItemInfo);

	gCustomBow.Init(gProtect.m_MainInfo.CustomBowInfo);

	gCustomWing.Init(gProtect.m_MainInfo.CustomWingInfo);

	gCustomMonster.Init(gProtect.m_MainInfo.CustomMonsterInfo);

	gItemPosition.Init(gProtect.m_MainInfo.ItemPositionInfo);

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