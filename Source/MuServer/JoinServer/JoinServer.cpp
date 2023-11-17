#include "stdafx.h"
#include "resource.h"
#include "JoinServer.h"
#include "AccountManager.h"
#include "AllowableIpList.h"
#include "JoinServerProtocol.h"
#include "MiniDump.h"
#include "QueryManager.h"
#include "ServerDisplayer.h"
#include "SocketManager.h"
#include "SocketManagerUdp.h"
#include "Util.h"

HINSTANCE hInst;

HWND hWnd;

TCHAR szTitle[MAX_LOADSTRING] = "MuEmu JoinServer Ex097";

TCHAR szWindowClass[MAX_LOADSTRING] = "JOINSERVER";

BOOL CaseSensitive;

BOOL MD5Encryption;

char GlobalPassword[11];

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
		char JoinServerODBC[32] = { 0 };

		char JoinServerUSER[32] = { 0 };

		char JoinServerPASS[32] = { 0 };

		GetPrivateProfileString("JoinServerInfo", "JoinServerODBC", "", JoinServerODBC, sizeof(JoinServerODBC), ".\\JoinServer.ini");

		GetPrivateProfileString("JoinServerInfo", "JoinServerUSER", "", JoinServerUSER, sizeof(JoinServerUSER), ".\\JoinServer.ini");

		GetPrivateProfileString("JoinServerInfo", "JoinServerPASS", "", JoinServerPASS, sizeof(JoinServerPASS), ".\\JoinServer.ini");

		GetPrivateProfileString("JoinServerInfo", "GlobalPassword", "XwefDastoD", GlobalPassword, sizeof(GlobalPassword), ".\\JoinServer.ini");

		WORD JoinServerPort = GetPrivateProfileInt("JoinServerInfo", "JoinServerPort", 55970, ".\\JoinServer.ini");

		char ConnectServerAddress[16] = { 0 };

		GetPrivateProfileString("JoinServerInfo", "ConnectServerAddress", "127.0.0.1", ConnectServerAddress, sizeof(ConnectServerAddress), ".\\JoinServer.ini");

		WORD ConnectServerPort = GetPrivateProfileInt("JoinServerInfo", "ConnectServerPort", 55557, ".\\JoinServer.ini");

		CaseSensitive = GetPrivateProfileInt("JoinServerInfo", "CaseSensitive", 0, ".\\JoinServer.ini");

		MD5Encryption = GetPrivateProfileInt("JoinServerInfo", "MD5Encryption", 0, ".\\JoinServer.ini");

		if (gQueryManager.Connect(JoinServerODBC, JoinServerUSER, JoinServerPASS) == false)
		{
			LogAdd(LOG_RED, "Could not connect to database");
		}
		else
		{
			if (gSocketManager.Start(JoinServerPort) == false)
			{
				gQueryManager.Disconnect();
			}
			else
			{
				if (gSocketManagerUdp.Connect(ConnectServerAddress, ConnectServerPort) == false)
				{
					gSocketManager.Clean();

					gQueryManager.Disconnect();
				}
				else
				{
					gAllowableIpList.Load("AllowableIpList.txt");

					SetTimer(hWnd, TIMER_1000, 1000, 0);
				}
			}
		}
	}
	else
	{
		LogAdd(LOG_RED, "WSAStartup() failed with error: %d", WSAGetLastError());
	}

	MSG msg;

	while (GetMessage(&msg, 0, 0, 0) != FALSE)
	{
		TranslateMessage(&msg);

		DispatchMessageA(&msg);
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
					if (MessageBox(hWnd, "Are you sure to terminate JoinServer?", "Ask terminate server", MB_YESNO | MB_ICONQUESTION) == IDYES)
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
			if (MessageBox(hWnd, "Close JoinServer?", "JoinServer", MB_OKCANCEL) == IDOK)
			{
				DestroyWindow(hWnd);
			}

			break;
		}

		case WM_TIMER:
		{
			switch (wParam)
			{
				case TIMER_1000:
				{
					JoinServerLiveProc();

					break;
				}
			}

			break;
		}

		case WM_SIZE:
		{
			gServerDisplayer.Run();

			break;
		}

		case WM_ACTIVATE:
		{
			gServerDisplayer.Run();

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