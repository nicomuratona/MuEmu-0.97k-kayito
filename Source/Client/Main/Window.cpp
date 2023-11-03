#include "StdAfx.h"
#include "Window.h"
#include "Controller.h"
#include "PrintPlayer.h"
#include "Protect.h"
#include "resource.h"

cWindow	gWindow;

cWindow::cWindow()
{
	HKEY Key;

	if (RegOpenKey(HKEY_CURRENT_USER, "Software\\Webzen\\MU\\Config", &Key) == ERROR_SUCCESS)
	{
		DWORD Size = sizeof(int);

		if (RegQueryValueEx(Key, "WindowMode", nullptr, nullptr, (LPBYTE)(&this->m_WindowMode), &Size) != ERROR_SUCCESS)
		{
			this->m_WindowMode = 0;
		}

		RegCloseKey(Key);
	}
}

cWindow::~cWindow()
{

}

void cWindow::WindowModeLoad(HINSTANCE hins)
{
	this->Instance = hins;

	SetDword(0x0055237C, (DWORD)&this->ChangeDisplaySettingsHook);

	SetCompleteHook(0xE9, 0x0041ED79, 0x0041EEC6);

	SetCompleteHook(0xE9, 0x0041DFF0, &this->StartWindow);

	if (this->m_WindowMode != 0)
	{
		SetCompleteHook(0xE9, 0x0041F617, 0x00421B0B);
	}
}

void cWindow::ChangeWindowText()
{
	if (SceneFlag != 5)
	{
		return;
	}

	static char text[256];

	STRUCT_DECRYPT;

	sprintf_s(text, sizeof(text), "%s || Resets: %d || GrandResets: %d || Level: %d", (char*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x00), ViewReset, ViewGrandReset, *(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x0E));

	STRUCT_ENCRYPT;

	SetWindowText(g_hWnd, text);
}

LONG WINAPI cWindow::ChangeDisplaySettingsHook(DEVMODEA* lpDevMode, DWORD dwFlags)
{
	if (gWindow.m_WindowMode == 0)
	{
		return ChangeDisplaySettingsA(lpDevMode, dwFlags);
	}

	return DISP_CHANGE_SUCCESSFUL;
}

void cWindow::ChangeDisplaySettingsFunction()
{
	DEVMODE DevMode;

	int nModes = 0;

	while (EnumDisplaySettings(NULL, nModes, &DevMode))
	{
		nModes++;
	}

	DEVMODE* pDevmodes = new DEVMODE[nModes + 1];

	nModes = 0;

	while (EnumDisplaySettings(NULL, nModes, &pDevmodes[nModes]))
	{
		nModes++;
	}

	DWORD dwBitsPerPel = 32;

	for (int n1 = 0; n1 < nModes; n1++)
	{
		if (pDevmodes[n1].dmBitsPerPel == 24 || pDevmodes[n1].dmBitsPerPel == 32)
		{
			dwBitsPerPel = pDevmodes[n1].dmBitsPerPel;

			break;
		}
	}

	for (int n2 = 0; n2 < nModes; n2++)
	{
		if (pDevmodes[n2].dmPelsWidth == WindowWidth && pDevmodes[n2].dmPelsHeight == WindowHeight && pDevmodes[n2].dmBitsPerPel == dwBitsPerPel)
		{
			ChangeDisplaySettings(&pDevmodes[n2], 0);

			break;
		}
	}

	delete[] pDevmodes;
}

HWND cWindow::StartWindow(HINSTANCE hCurrentInst, int nCmdShow)
{
	char* windowName = "MU ONLINE";

	WNDCLASS wndClass;

	wndClass.style = CS_OWNDC | CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;

	wndClass.lpfnWndProc = gWindow.MyWndProc;

	wndClass.cbClsExtra = 0;

	wndClass.cbWndExtra = 0;

	wndClass.hInstance = hCurrentInst;

	wndClass.hIcon = LoadIcon(gWindow.Instance, MAKEINTRESOURCE(IDI_ICON));

	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);

	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	wndClass.lpszMenuName = NULL;

	wndClass.lpszClassName = windowName;

	RegisterClass(&wndClass);

	HWND hWnd;

	if (gWindow.m_WindowMode == 0)
	{
		hWnd = CreateWindowExA(WS_EX_APPWINDOW | WS_EX_TOPMOST, windowName, gProtect.m_MainInfo.WindowName, WS_POPUP, 0, 0, WindowWidth, WindowHeight, NULL, NULL, hCurrentInst, NULL);

		gWindow.ChangeDisplaySettingsFunction();
	}
	else
	{
		RECT rc = { 0, 0, WindowWidth, WindowHeight };

		AdjustWindowRect(&rc, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER | WS_CLIPCHILDREN, NULL);

		hWnd = CreateWindowA(windowName, gProtect.m_MainInfo.WindowName, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER | WS_CLIPCHILDREN, (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2, (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2, rc.right, rc.bottom + 26, NULL, NULL, hCurrentInst, NULL);
	}

	return hWnd;
}

LRESULT WINAPI cWindow::MyWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_NCACTIVATE:
		{
			return 0;
		}

		case 0x2B11u: // Fix disconnect when minimize
		{
			return 0;
		}

		case WM_SIZE: // Fix disconnect when minimize
		{
			return 0;
		}

		case WM_TIMER:
		{
			switch (wParam)
			{
				case WM_AUTOCLICKTIMER:
				{
					SendMessage(g_hWnd, (gController.AutoClickState) ? WM_RBUTTONUP : WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(MouseX, MouseY));

					gController.AutoClickState ^= 1;

					return NULL;
				}
			}

			break;
		}
	}

	return CallWindowProc(WndProc, hwnd, msg, wParam, lParam);
}