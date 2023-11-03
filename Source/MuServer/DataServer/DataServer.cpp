#include "stdafx.h"
#include "resource.h"
#include "DataServer.h"
#include "AllowableIpList.h"
#include "BadSyntax.h"
#include "GuildManager.h"
#include "MiniDump.h"
#include "QueryManager.h"
#include "ServerDisplayer.h"
#include "SocketManager.h"
#include "Util.h"

HINSTANCE hInst;

HWND hWnd;

TCHAR szTitle[MAX_LOADSTRING] = "MuEmu DataServer Ex097";

TCHAR szWindowClass[MAX_LOADSTRING] = "CONNECTSERVER";

int main()
{
	CMiniDump::Start();

	hInst = GetModuleHandle(NULL);

	if (MyRegisterClass(hInst) == 0)
	{
		MessageBox(NULL, "Failed to register window class.", "Register Class", MB_OK | MB_ICONERROR);

		return -1;
	}

	if (InitInstance(hInst) == false)
	{
		MessageBox(NULL, "Failed to initiaize window instance.", "Window Instance", MB_OK | MB_ICONERROR);

		return -1;
	}

	gServerDisplayer.Init(hWnd);

	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) == 0)
	{
		char DataServerODBC[32] = { 0 };

		char DataServerUSER[32] = { 0 };

		char DataServerPASS[32] = { 0 };

		GetPrivateProfileString("DataServerInfo", "DataServerODBC", "", DataServerODBC, sizeof(DataServerODBC), ".\\DataServer.ini");

		GetPrivateProfileString("DataServerInfo", "DataServerUSER", "", DataServerUSER, sizeof(DataServerUSER), ".\\DataServer.ini");

		GetPrivateProfileString("DataServerInfo", "DataServerPASS", "", DataServerPASS, sizeof(DataServerPASS), ".\\DataServer.ini");

		WORD DataServerPort = GetPrivateProfileInt("DataServerInfo", "DataServerPort", 55960, ".\\DataServer.ini");

		if (gQueryManager.Connect(DataServerODBC, DataServerUSER, DataServerPASS) == false)
		{
			LogAdd(LOG_RED, "Could not connect to database");
		}
		else
		{
			if (gSocketManager.Start(DataServerPort) == false)
			{
				gQueryManager.Disconnect();
			}
			else
			{
				gAllowableIpList.Load("AllowableIpList.txt");

				gBadSyntax.Load("BadSyntax.txt");

				SetTimer(hWnd, TIMER_1000, 1000, 0);

				gGuildManager.Init();
			}
		}
	}
	else
	{
		LogAdd(LOG_RED, "WSAStartup() failed with error: %d", WSAGetLastError());
	}

	gServerDisplayer.PaintAllInfo();

	SetTimer(hWnd, TIMER_2000, 2000, 0);

	MSG msg;

	while (GetMessage(&msg, 0, 0, 0) != FALSE)
	{
		TranslateMessage(&msg);

		DispatchMessageA(&msg);

		Sleep(1);
	}

	CMiniDump::Clean();

	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;

	wcex.lpfnWndProc = (WNDPROC)WndProc;

	wcex.cbClsExtra = 0;

	wcex.cbWndExtra = 0;

	wcex.hInstance = hInstance;

	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_ICON_NORMAL);

	wcex.hCursor = LoadCursor(0, IDC_ARROW);

	wcex.hbrBackground = (HBRUSH)COLOR_CAPTIONTEXT;

	wcex.lpszMenuName = (LPCSTR)IDR_MENU;

	wcex.lpszClassName = szWindowClass;

	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_ICON_SMALL);

	return RegisterClassEx(&wcex);
}

bool InitInstance(HINSTANCE hInstance)
{
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_THICKFRAME, CW_USEDEFAULT, 0, 600, 600, 0, 0, hInstance, 0);

	if (hWnd == 0)
	{
		return false;
	}

	ShowWindow(hWnd, SW_SHOW);

	ShowWindow(GetConsoleWindow(), (gConsole.HideConsole) ? SW_HIDE : SW_SHOW);

	UpdateWindow(hWnd);

	return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case ID_EXIT:
				{
					if (MessageBox(hWnd, "Are you sure to terminate DataServer?", "Ask terminate server", MB_YESNO | MB_ICONQUESTION) == IDYES)
					{
						DestroyWindow(hWnd);
					}

					break;
				}

				case ID_ABOUT:
				{
					DialogBox(hInst, (LPCTSTR)IDD_DIALOG_ABOUT, hWnd, (DLGPROC)About);

					break;
				}

				case ID_CONSOLE_SHOW:
				{
					gConsole.HideConsole ^= 1;

					ShowWindow(GetConsoleWindow(), (gConsole.HideConsole) ? SW_HIDE : SW_SHOW);

					break;
				}

				case ID_CONSOLE_CLEAR:
				{
					system("cls"); // clear console

					break;
				}

				case ID_CONSOLE_GENERAL:
				{
					gConsole.EnableOutput[CON_GENERAL] ^= 1;

					CheckMenuItem(GetMenu(hWnd), ID_CONSOLE_GENERAL, MF_BYCOMMAND | (gConsole.EnableOutput[CON_GENERAL]) ? MF_CHECKED : MF_UNCHECKED);

					break;
				}

				case ID_CONSOLE_TCPRECV:
				{
					gConsole.EnableOutput[CON_PROTO_TCP_RECV] ^= 1;

					CheckMenuItem(GetMenu(hWnd), ID_CONSOLE_TCPRECV, MF_BYCOMMAND | (gConsole.EnableOutput[CON_PROTO_TCP_RECV]) ? MF_CHECKED : MF_UNCHECKED);

					break;
				}

				case ID_CONSOLE_TCPSEND:
				{
					gConsole.EnableOutput[CON_PROTO_TCP_SEND] ^= 1;

					CheckMenuItem(GetMenu(hWnd), ID_CONSOLE_TCPSEND, MF_BYCOMMAND | (gConsole.EnableOutput[CON_PROTO_TCP_SEND]) ? MF_CHECKED : MF_UNCHECKED);

					break;
				}

				default:
				{
					return DefWindowProc(hWnd, message, wParam, lParam);
				}
			}

			break;
		}

		case WM_CLOSE:
		{
			if (MessageBox(hWnd, "Close DataServer?", "DataServer", MB_OKCANCEL) == IDOK)
			{
				DestroyWindow(hWnd);
			}

			break;
		}

		case WM_TIMER:
		{
			switch (LOWORD(wParam))
			{
				case TIMER_2000:
				{
					gServerDisplayer.Run();

					break;
				}
			}

			break;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);

			break;
		}

		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	return 0;
}

LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
		{
			return 1;
		}

		case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));

				return 1;
			}

			break;
		}
	}

	return 0;
}