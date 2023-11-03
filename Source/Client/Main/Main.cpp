#include "stdafx.h"
#include "Buff.h"
#include "ChaosMix.h"
#include "Controller.h"
#include "GuildLogo.h"
#include "HackCheck.h"
#include "HealthBar.h"
#include "Interface.h"
#include "Item.h"
#include "ItemStack.h"
#include "ItemValue.h"
#include "Language.h"
#include "LoadModels.h"
#include "MiniMap.h"
#include "MoveList.h"
#include "OptionsMenu.h"
#include "PacketManager.h"
#include "Patchs.h"
#include "PrintPlayer.h"
#include "Protect.h"
#include "Protocol.h"
#include "Reconnect.h"
#include "Resolution.h"
#include "RightClickMove.h"
#include "SkyDome.h"
#include "Sound.h"
#include "TrayMode.h"
#include "WeaponView.h"
#include "Window.h"

HINSTANCE hins;

extern "C" _declspec(dllexport) void _cdecl EntryProc()
{
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

	gProtect.CheckClientFile();

	gPacketManager.LoadEncryptionKey("Data\\Enc1.dat");

	gPacketManager.LoadDecryptionKey("Data\\Dec2.dat");

	InitHackCheck();

	InitPatchs();

	gBuff.Init();

	InitProtocol();

	gLanguage.Init();

	InitPrintPlayer();

	InitReconnect();

	gSound.Init();

	gOptionsMenu.Init();

	gResolution.Init();

	gInterface.Init();

	gHealthBar.Init();

	gSkyDome.Init();

	gMiniMap.Init();

	gMoveList.Init();

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

			gTrayMode.Init(hins);

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