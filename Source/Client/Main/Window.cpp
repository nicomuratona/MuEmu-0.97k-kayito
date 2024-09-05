#include "StdAfx.h"
#include "Window.h"
#include "Camera.h"
#include "Controller.h"
#include "Font.h"
#include "PrintPlayer.h"
#include "Protect.h"
#include "resource.h"

CWindow	gWindow;

CWindow::CWindow()
{
	this->iResolutionValues[R640x480] = std::make_pair<WORD, WORD>(640, 480);
	this->iResolutionValues[R800x600] = std::make_pair<WORD, WORD>(800, 600);
	this->iResolutionValues[R1024x768] = std::make_pair<WORD, WORD>(1024, 768);
	this->iResolutionValues[R1280x960] = std::make_pair<WORD, WORD>(1280, 960);
	this->iResolutionValues[R1280x720] = std::make_pair<WORD, WORD>(1280, 720);
	this->iResolutionValues[R1366x768] = std::make_pair<WORD, WORD>(1360, 768);
	this->iResolutionValues[R1600x900] = std::make_pair<WORD, WORD>(1600, 900);
	this->iResolutionValues[R1920x1080] = std::make_pair<WORD, WORD>(1920, 1080);

	this->m_WindowMode = WINDOW_MODE;

	this->m_Borderless = false;

	m_Resolution = R1024x768;

	WindowWidth = this->iResolutionValues[R1024x768].first;

	WindowHeight = this->iResolutionValues[R1024x768].second;

	g_fScreenRate_x = (float)WindowWidth / 640.0f;

	g_fScreenRate_y = (float)WindowHeight / 480.0f;
}

CWindow::~CWindow()
{
	char Text[33] = { 0 };

	wsprintf(Text, "%d", this->m_WindowMode);

	WritePrivateProfileString("Window", "WindowMode", Text, ".\\Config.ini");

	wsprintf(Text, "%d", this->m_Borderless);

	WritePrivateProfileString("Window", "Borderless", Text, ".\\Config.ini");

	wsprintf(Text, "%d", m_Resolution);

	WritePrivateProfileString("Window", "Resolution", Text, ".\\Config.ini");
}

void CWindow::WindowModeLoad(HINSTANCE hins)
{
	this->Instance = hins;

	this->m_WindowIcon = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);

	this->m_WindowMode = (GetPrivateProfileInt("Window", "WindowMode", WINDOW_MODE, ".\\Config.ini") != 0);

	this->m_Borderless = (GetPrivateProfileInt("Window", "Borderless", 0, ".\\Config.ini") != 0);

	this->SetResolution(GetPrivateProfileInt("Window", "Resolution", R1024x768, ".\\Config.ini"));

	SetCompleteHook(0xE8, 0x00412BC4, &this->FixDisplaySettingsOnClose);
	SetByte(0x00412BC4 + 5, 0x90);

	SetCompleteHook(0xE9, 0x0041ED79, 0x0041EEC6);

	SetCompleteHook(0xE9, 0x0041DFF0, &this->StartWindow);

	SetCompleteHook(0xE9, 0x0041DE30, &this->CreateOpenglWindow);

	SetCompleteHook(0xE9, 0x0041F617, 0x00421B0B);
}

LONG WINAPI CWindow::FixDisplaySettingsOnClose(DEVMODEA* lpDevMode, DWORD dwFlags)
{
	if (gWindow.m_WindowMode == FULL_SCREEN)
	{
		return ChangeDisplaySettings(NULL, 0);
	}

	return 0;
}

LRESULT WINAPI CWindow::MyWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_LBUTTONDOWN:
		{
			if (MouseRButton || MouseRButtonPush)
			{
				return 0;
			}

			MouseLButtonPop = false;

			if (!MouseLButton)
			{
				MouseLButtonPush = true;
			}

			MouseLButton = true;

			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

		case WM_RBUTTONDOWN:
		{
			if (MouseLButton || MouseLButtonPush)
			{
				return 0;
			}

			MouseRButtonPop = false;

			if (!MouseRButton)
			{
				MouseRButtonPush = true;
			}

			MouseRButton = true;

			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

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

HWND CWindow::StartWindow(HINSTANCE hCurrentInst, int nCmdShow)
{
	char* windowName = "MU ONLINE";

	WNDCLASS wndClass = { 0 };

	wndClass.style = CS_OWNDC | CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;

	wndClass.lpfnWndProc = gWindow.MyWndProc;

	wndClass.cbClsExtra = 0;

	wndClass.cbWndExtra = 0;

	wndClass.hInstance = gWindow.Instance;

	wndClass.hIcon = gWindow.m_WindowIcon;

	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);

	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	wndClass.lpszMenuName = NULL;

	wndClass.lpszClassName = windowName;

	RegisterClass(&wndClass);

	HWND hWnd;

	if (gWindow.m_WindowMode)
	{
		RECT rc = { 0, 0, WindowWidth, WindowHeight };

		LONG STYLE = (gWindow.m_Borderless)
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

void CWindow::ChangeDisplaySettingsFunction()
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

bool CWindow::CreateOpenglWindow()
{
	PIXELFORMATDESCRIPTOR pfd;

	memset(&pfd, 0, sizeof(pfd));

	pfd.nSize = sizeof(pfd);

	pfd.nVersion = 1;

	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;

	pfd.iPixelType = PFD_TYPE_RGBA;

	pfd.cColorBits = 32;

	pfd.cDepthBits = 32;

	if (!(g_hDC = GetDC(g_hWnd))) // Get the device context.
	{
		KillGLWindow(); // reset the display

		MessageBox(NULL, GlobalText[4], "OpenGL Get DC Error.", MB_OK | MB_ICONEXCLAMATION);

		return false; // failure
	}

	GLuint PixelFormat; // Find and remember the appropriate pixel format.

	if (!(PixelFormat = ChoosePixelFormat(g_hDC, &pfd))) // Select the pixel format closest to the one specified by pdf.
	{
		KillGLWindow(); // reset the display

		MessageBox(NULL, GlobalText[4], "OpenGL Choose Pixel Format Error.", MB_OK | MB_ICONEXCLAMATION);

		return false; // failure
	}

	if (!SetPixelFormat(g_hDC, PixelFormat, &pfd)) // Set the pixel format of the device context.
	{
		KillGLWindow(); // reset the display

		MessageBox(NULL, GlobalText[4], "OpenGL Set Pixel Format Error.", MB_OK | MB_ICONEXCLAMATION);

		return false; // failure
	}

	HGLRC hglrcDummy = wglCreateContext(g_hDC);

	if (!hglrcDummy) // Create an appropriate rendering context with the device context.
	{
		KillGLWindow(); // reset the display

		MessageBox(NULL, GlobalText[4], "OpenGL Create Context Error.", MB_OK | MB_ICONEXCLAMATION);

		return false; // failure
	}

	if (!wglMakeCurrent(g_hDC, hglrcDummy)) // Activate the rendering context and associate it with the device context.
	{
		KillGLWindow(); // reset the display

		MessageBox(NULL, GlobalText[4], "OpenGL Make Current Error.", MB_OK | MB_ICONEXCLAMATION);

		return false; // failure
	}

	if (glewInit() != GLEW_OK) // Initialize GLEW to load OpenGL extensions
	{
		KillGLWindow(); // reset the display

		MessageBox(NULL, GlobalText[4], "Failed to initialize GLEW.", MB_OK | MB_ICONEXCLAMATION);

		return false; // failure
	}

	// Now you can create an OpenGL 3.3 context
	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_PROFILE_MASK_ARB,
		WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		0
	};

	if (!(g_hRC = wglCreateContextAttribsARB(g_hDC, 0, attribs))) // Create the real OpenGL 3.3 context
	{
		KillGLWindow(); // reset the display

		MessageBox(NULL, GlobalText[4], "GLEW Create Context Error.", MB_OK | MB_ICONEXCLAMATION);

		return false; // failure
	}

	// Clean up the dummy context
	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(hglrcDummy);

	// Make the new context current
	wglMakeCurrent(g_hDC, g_hRC);

	ShowWindow(g_hWnd, SW_SHOW); // show the window

	SetForegroundWindow(g_hWnd); // bring the window to the top

	SetFocus(g_hWnd); // Give the window keyboard focus.

	return true;
}

void CWindow::ToggleTrayMode()
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

void CWindow::ShowTrayNotify(bool mode)
{
	NOTIFYICONDATA nid;

	memset(&nid, 0, sizeof(nid));

	nid.cbSize = sizeof(NOTIFYICONDATA);

	nid.hWnd = g_hWnd;

	nid.uID = WM_TRAY_MODE_ICON;

	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_INFO;

	nid.uCallbackMessage = WM_TRAY_MODE_MESSAGE;

	nid.hIcon = this->m_WindowIcon;

	nid.dwInfoFlags = NIIF_INFO;

	nid.uTimeout = 5000;

	strcpy_s(nid.szInfo, "I'm Here");

	strcpy_s(nid.szInfoTitle, gProtect.m_MainInfo.WindowName);

	strcpy_s(nid.szTip, gProtect.m_MainInfo.WindowName);

	Shell_NotifyIcon(((mode == false) ? NIM_DELETE : NIM_ADD), &nid);
}

void CWindow::ShowTrayMessage(char* Title, char* Message)
{
	NOTIFYICONDATA Icon = { 0 };

	Icon.cbSize = sizeof(NOTIFYICONDATA);

	Icon.uID = WM_TRAY_MODE_ICON;

	Icon.hWnd = g_hWnd;

	Icon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_INFO;

	Icon.hIcon = this->m_WindowIcon;

	Icon.uCallbackMessage = WM_TRAY_MODE_MESSAGE;

	Icon.dwInfoFlags = NIIF_INFO;

	Icon.uTimeout = 5000;

	strcpy_s(Icon.szInfo, Message);

	strcpy_s(Icon.szInfoTitle, Title);

	Shell_NotifyIcon(NIM_MODIFY, &Icon);
}

void CWindow::ChangeWindowText()
{
	if (SceneFlag != 5)
	{
		return;
	}

	char text[256];

	STRUCT_DECRYPT;

	sprintf_s(text, sizeof(text), "%s || Resets: %d || GrandResets: %d || Level: %d || PING: %u ms || FPS: %.0f", (char*)(CharacterAttribute + 0x00), gPrintPlayer.ViewReset, gPrintPlayer.ViewGrandReset, *(WORD*)(CharacterAttribute + 0x0E), gPrintPlayer.ViewPing, FPS);

	STRUCT_ENCRYPT;

	SetWindowText(g_hWnd, text);
}

void CWindow::SetWindowMode(bool windowMode, bool borderless)
{
	this->m_WindowMode = windowMode;

	this->m_Borderless = borderless;
}

void CWindow::SetResolution(int res)
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

void CWindow::ChangeWindowState(bool windowMode, bool borderless, int resolution)
{
	if (windowMode != this->m_WindowMode || borderless != this->m_Borderless)
	{
		if (!windowMode)
		{
			this->ChangeDisplaySettingsFunction();
		}
		else if (windowMode != this->m_WindowMode)
		{
			ChangeDisplaySettings(NULL, 0);
		}

		this->SetWindowMode(windowMode, borderless);
	}
	else if (resolution != m_Resolution)
	{
		this->SetResolution(resolution);

		if (!this->m_WindowMode)
		{
			this->ChangeDisplaySettingsFunction();
		}
	}

	if (!this->m_WindowMode || this->m_Borderless)
	{
		// Delete the window icon after changing styles in window mode
		SendMessage(g_hWnd, WM_SETICON, ICON_BIG, (LPARAM)NULL);

		SendMessage(g_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)NULL);
	}

	RECT rc = { 0, 0, WindowWidth, WindowHeight };

	LONG PosX = ((GetSystemMetrics(SM_CXSCREEN)) / 2) - (WindowWidth / 2);

	LONG PosY = ((GetSystemMetrics(SM_CYSCREEN)) / 2) - (WindowHeight / 2);

	if (this->m_WindowMode)
	{
		LONG STYLE = (this->m_Borderless)
			? WS_POPUP | WS_VISIBLE
			: WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;

		LONG EXSTYLE = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

		SetWindowLongPtr(g_hWnd, GWL_STYLE, STYLE); // Set the Window Style

		SetWindowLongPtr(g_hWnd, GWL_EXSTYLE, EXSTYLE); // Set the Window Extra Style

		AdjustWindowRect(&rc, STYLE, FALSE); // Adjust the rectangle inside

		if (!this->m_Borderless)
		{
			// Restore the window icon after changing styles in window mode
			SendMessage(g_hWnd, WM_SETICON, ICON_BIG, (LPARAM)this->m_WindowIcon);

			SendMessage(g_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)this->m_WindowIcon);
		}
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