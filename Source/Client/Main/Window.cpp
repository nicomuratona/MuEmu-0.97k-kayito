#include "StdAfx.h"
#include "Window.h"
#include "Camera.h"
#include "Controller.h"
#include "Font.h"
#include "PrintPlayer.h"
#include "Protect.h"
#include "resource.h"

cWindow	gWindow;

cWindow::cWindow()
{
	this->iResolutionValues[R640x480] = std::make_pair<WORD, WORD>(640, 480);
	this->iResolutionValues[R800x600] = std::make_pair<WORD, WORD>(800, 600);
	this->iResolutionValues[R1024x768] = std::make_pair<WORD, WORD>(1024, 768);
	this->iResolutionValues[R1280x960] = std::make_pair<WORD, WORD>(1280, 960);
	this->iResolutionValues[R1280x720] = std::make_pair<WORD, WORD>(1280, 720);
	this->iResolutionValues[R1366x768] = std::make_pair<WORD, WORD>(1360, 768);
	this->iResolutionValues[R1600x900] = std::make_pair<WORD, WORD>(1600, 900);
	this->iResolutionValues[R1920x1080] = std::make_pair<WORD, WORD>(1920, 1080);

	this->iWindowValues[FULL_SCREEN] = "Full Screen";
	this->iWindowValues[WINDOW_MODE] = "Window Mode";
	this->iWindowValues[BORDERLESS] = "Borderless";

	this->m_WindowMode = WINDOW_MODE;

	m_Resolution = R1024x768;

	WindowWidth = this->iResolutionValues[R1024x768].first;

	WindowHeight = this->iResolutionValues[R1024x768].second;

	g_fScreenRate_x = (float)WindowWidth / 640.0f;

	g_fScreenRate_y = (float)WindowHeight / 480.0f;
}

cWindow::~cWindow()
{
	char Text[33] = { 0 };

	wsprintf(Text, "%d", this->m_WindowMode);

	WritePrivateProfileString("Window", "WindowMode", Text, ".\\Config.ini");

	wsprintf(Text, "%d", m_Resolution);

	WritePrivateProfileString("Window", "Resolution", Text, ".\\Config.ini");
}

void cWindow::WindowModeLoad(HINSTANCE hins)
{
	this->Instance = hins;

	this->m_TrayIcon = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);

	this->SetWindowMode(GetPrivateProfileInt("Window", "WindowMode", WINDOW_MODE, ".\\Config.ini"));

	this->SetResolution(GetPrivateProfileInt("Window", "Resolution", R1024x768, ".\\Config.ini"));

	SetCompleteHook(0xE9, 0x0041ED79, 0x0041EEC6);

	SetCompleteHook(0xE9, 0x0041DFF0, &this->StartWindow);

	SetCompleteHook(0xE9, 0x0041F617, 0x00421B0B);
}

HWND cWindow::StartWindow(HINSTANCE hCurrentInst, int nCmdShow)
{
	char* windowName = "MU ONLINE";

	WNDCLASS wndClass = { 0 };

	wndClass.style = CS_OWNDC | CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;

	wndClass.lpfnWndProc = gWindow.MyWndProc;

	wndClass.cbClsExtra = 0;

	wndClass.cbWndExtra = 0;

	wndClass.hInstance = gWindow.Instance;

	wndClass.hIcon = LoadIcon(gWindow.Instance, MAKEINTRESOURCE(IDI_ICON));

	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);

	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	wndClass.lpszMenuName = NULL;

	wndClass.lpszClassName = windowName;

	RegisterClass(&wndClass);

	HWND hWnd;

	if (gWindow.m_WindowMode)
	{
		RECT rc = { 0, 0, WindowWidth, WindowHeight };

		LONG STYLE = (gWindow.m_WindowMode == 2) // Borderless
			? WS_POPUP | WS_VISIBLE
			: WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;

		AdjustWindowRect(&rc, STYLE, NULL);

		hWnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, windowName, gProtect.m_MainInfo.WindowName, STYLE, (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2, (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2, rc.right, rc.bottom + 26, NULL, NULL, gWindow.Instance, NULL);
	}
	else
	{
		hWnd = CreateWindowEx(WS_EX_APPWINDOW, windowName, gProtect.m_MainInfo.WindowName, WS_POPUP | WS_VISIBLE, 0, 0, WindowWidth, WindowHeight, NULL, NULL, gWindow.Instance, NULL);

		gWindow.ChangeDisplaySettingsFunction();
	}

	return hWnd;
}

void cWindow::ChangeDisplaySettingsFunction()
{
	DEVMODE devMode = { 0 };

	devMode.dmSize = sizeof(DEVMODE);

	devMode.dmDriverExtra = 0;

	devMode.dmPelsWidth = WindowWidth;

	devMode.dmPelsHeight = WindowHeight;

	devMode.dmBitsPerPel = 32;

	devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

	ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
}

void cWindow::ShowTrayNotify(bool mode)
{
	NOTIFYICONDATA nid;

	memset(&nid, 0, sizeof(nid));

	nid.cbSize = sizeof(NOTIFYICONDATA);

	nid.hWnd = g_hWnd;

	nid.uID = WM_TRAY_MODE_ICON;

	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_INFO;

	nid.uCallbackMessage = WM_TRAY_MODE_MESSAGE;

	nid.hIcon = this->m_TrayIcon;

	nid.dwInfoFlags = NIIF_INFO;

	nid.uTimeout = 5000;

	strcpy_s(nid.szInfo, "I'm Here");

	strcpy_s(nid.szInfoTitle, gProtect.m_MainInfo.WindowName);

	strcpy_s(nid.szTip, gProtect.m_MainInfo.WindowName);

	Shell_NotifyIcon(((mode == false) ? NIM_DELETE : NIM_ADD), &nid);
}

void cWindow::ShowTrayMessage(char* Title, char* Message)
{
	NOTIFYICONDATA Icon = { 0 };

	Icon.cbSize = sizeof(NOTIFYICONDATA);

	Icon.uID = WM_TRAY_MODE_ICON;

	Icon.hWnd = g_hWnd;

	Icon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_INFO;

	Icon.hIcon = this->m_TrayIcon;

	Icon.uCallbackMessage = WM_TRAY_MODE_MESSAGE;

	Icon.dwInfoFlags = NIIF_INFO;

	Icon.uTimeout = 5000;

	strcpy_s(Icon.szInfo, Message);

	strcpy_s(Icon.szInfoTitle, Title);

	Shell_NotifyIcon(NIM_MODIFY, &Icon);
}

LRESULT WINAPI cWindow::MyWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_NPROTECT_EXIT_TWO: // Fix disconnect when minimize
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
					gController.AutoClickState ^= 1;

					MouseRButtonPush = gController.AutoClickState;

					MouseRButton = gController.AutoClickState;

					return 0;
				}

				case WINDOWMINIMIZED_TIMER:
				{
					return 0;
				}
			}

			break;
		}

		case WM_TRAY_MODE_MESSAGE:
		{
			switch (lParam)
			{
				case WM_LBUTTONDOWN:
				{
					gWindow.ToggleTrayMode();

					break;
				}
			}

			break;
		}
	}

	return CallWindowProc(WndProc, hwnd, msg, wParam, lParam);
}

void cWindow::ChangeWindowText()
{
	if (SceneFlag != 5)
	{
		return;
	}

	static char text[256];

	STRUCT_DECRYPT;

	sprintf_s(text, sizeof(text), "%s || Resets: %d || GrandResets: %d || Level: %d", (char*)(*(DWORD*)(CharacterAttribute)+0x00), gPrintPlayer.ViewReset, gPrintPlayer.ViewGrandReset, *(WORD*)(*(DWORD*)(CharacterAttribute)+0x0E));

	STRUCT_ENCRYPT;

	SetWindowText(g_hWnd, text);
}

void cWindow::SetWindowMode(int mode)
{
	if (mode < FULL_SCREEN || mode > BORDERLESS)
	{
		mode = FULL_SCREEN;
	}

	this->m_WindowMode = mode;
}

void cWindow::SetResolution(int res)
{
	if (res >= R640x480 && res < MAX_RESOLUTION_VALUE)
	{
		m_Resolution = res;

		WindowWidth = this->iResolutionValues[res].first;

		WindowHeight = this->iResolutionValues[res].second;

		g_fScreenRate_x = (float)WindowWidth / 640.0f;

		g_fScreenRate_y = (float)WindowHeight / 480.0f;
	}
}

void cWindow::ChangeWindowState()
{
	RECT rc = { 0, 0, WindowWidth, WindowHeight };

	if (this->m_WindowMode)
	{
		ChangeDisplaySettings(NULL, 0);
	}
	else
	{
		this->ChangeDisplaySettingsFunction();
	}

	LONG PosX = ((GetSystemMetrics(SM_CXSCREEN)) / 2) - (WindowWidth / 2);

	LONG PosY = ((GetSystemMetrics(SM_CYSCREEN)) / 2) - (WindowHeight / 2);

	if (this->m_WindowMode)
	{
		LONG STYLE = (gWindow.m_WindowMode == 2) // Borderless
			? WS_POPUP | WS_VISIBLE
			: WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

		LONG EXSTYLE = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

		SetWindowLongPtr(g_hWnd, GWL_STYLE, STYLE); // Set the Window Style

		SetWindowLongPtr(g_hWnd, GWL_EXSTYLE, EXSTYLE); // Set the Window Extra Style

		AdjustWindowRect(&rc, STYLE, FALSE); // Adjust the rectangle inside
	}
	else
	{
		LONG STYLE = WS_POPUP | WS_VISIBLE;

		LONG EXSTYLE = WS_EX_APPWINDOW;

		SetWindowLongPtr(g_hWnd, GWL_STYLE, STYLE); // Set the Window Style

		SetWindowLongPtr(g_hWnd, GWL_EXSTYLE, EXSTYLE); // Set the Window Extra Style

		AdjustWindowRect(&rc, STYLE, FALSE); // Adjust the rectangle inside
	}

	SetWindowPos(g_hWnd, NULL, PosX, PosY, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW | SWP_NOZORDER | SWP_FRAMECHANGED);

	MoveWindow(g_hWnd, PosX, PosY, rc.right - rc.left, rc.bottom - rc.top, TRUE); // Change the size

	gCamera.SetCurrentValue(); // Fix the 3d camera position

	gFont.ReloadFont();
}

void cWindow::ToggleTrayMode()
{
	if (IsWindowVisible(g_hWnd) == FALSE)
	{
		ShowWindow(g_hWnd, SW_SHOW);

		this->ShowTrayNotify(false);
	}
	else
	{
		ShowWindow(g_hWnd, SW_HIDE);

		this->ShowTrayNotify(true);

		//this->ShowTrayMessage(gProtect.m_MainInfo.WindowName, "Ha sido minimizado.");
	}
}