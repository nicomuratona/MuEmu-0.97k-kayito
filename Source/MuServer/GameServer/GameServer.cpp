#include "stdafx.h"
#include "Resource.h"
#include "GameServer.h"
#include "GameMain.h"
#include "BloodCastle.h"
#include "DevilSquare.h"
#include "JSProtocol.h"
#include "MapManager.h"
#include "Message.h"
#include "MiniDump.h"
#include "Notice.h"
#include "QueueTimer.h"
#include "ServerDisplayer.h"
#include "ServerInfo.h"
#include "SocketManager.h"
#include "SocketManagerUdp.h"
#include "Util.h"

HINSTANCE hInst;

TCHAR szTitle[MAX_LOADSTRING] = "MuEmu GameServer Ex097";

TCHAR szWindowClass[MAX_LOADSTRING] = "GAMESERVER";

HWND hWnd;

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

	SetLargeRand();

	gServerInfo.ReadStartupInfo("GameServerInfo", ".\\Data\\GameServerInfo - StartUp.dat");

	gServerDisplayer.Init(hWnd);

	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) == 0)
	{
		if (gSocketManager.Start((WORD)gServerInfo.m_ServerPort) == 0)
		{
			LogAdd(LOG_RED, "Could not start GameServer");
		}
		else
		{
			GameMainInit(hWnd);

			JoinServerConnect();

			DataServerConnect();

			gSocketManagerUdp.Connect(gServerInfo.m_ConnectServerAddress, (WORD)gServerInfo.m_ConnectServerPort);

			SetTimer(hWnd, TIMER_1000, 1000, 0);

			SetTimer(hWnd, TIMER_10000, 10000, 0);

			gQueueTimer.CreateTimer(QUEUE_TIMER_MONSTER, 100, &QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_MONSTER_MOVE, 100, &QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_EVENT, 100, &QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_VIEWPORT, 1000, &QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_FIRST, 1000, &QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_CLOSE, 1000, &QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_ACCOUNT_LEVEL, 60000, &QueueTimerCallback);
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
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_THICKFRAME, CW_USEDEFAULT, 0, 800, 600, 0, 0, hInstance, 0);

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
				case ID_FILE_ALLUSERLOGOUT:
				{
					gObjAllLogOut();

					break;
				}

				case ID_FILE_ALLUSERDISCONNECT:
				{
					gObjAllDisconnect();

					break;
				}

				case ID_FILE_1MINUTESERVERCLOSE:
				{
					if (gCloseMsg == 0)
					{
						gCloseMsg = 1;

						gCloseMsgTime = 60;

						gNotice.GCNoticeSendToAll(0, 50);
					}

					break;
				}

				case ID_FILE_3MINUTESERVERCLOSE:
				{
					if (gCloseMsg == 0)
					{
						gCloseMsg = 1;

						gCloseMsgTime = 180;

						gNotice.GCNoticeSendToAll(0, 51);
					}

					break;
				}

				case ID_FILE_5MINUTESERVERCLOSE:
				{
					if (gCloseMsg == 0)
					{
						gCloseMsg = 1;

						gCloseMsgTime = 300;

						gNotice.GCNoticeSendToAll(0, 52);
					}

					break;
				}

				case ID_EXIT:
				{
					if (MessageBox(0, "Are you sure to terminate GameServer?", "Ask terminate server", MB_YESNO | MB_ICONQUESTION) == IDYES)
					{
						DestroyWindow(hWnd);
					}

					break;
				}

				case ID_RELOAD_RELOADCHAOSMIX:
				{
					gServerInfo.ReadChaosMixInfo();

					break;
				}

				case ID_RELOAD_RELOADCHARACTER:
				{
					gServerInfo.ReadCharacterInfo();

					break;
				}

				case ID_RELOAD_RELOADCOMMAND:
				{
					gServerInfo.ReadCommandInfo();

					break;
				}

				case ID_RELOAD_RELOADCOMMON:
				{
					gServerInfo.ReadCommonInfo();

					break;
				}

				case ID_RELOAD_RELOADCUSTOM:
				{
					gServerInfo.ReadCustomInfo();

					break;
				}

				case ID_RELOAD_RELOADEVENT:
				{
					gServerInfo.ReadEventInfo();

					break;
				}

				case ID_RELOAD_RELOADEVENTITEMBAG:
				{
					gServerInfo.ReadEventItemBagInfo();

					break;
				}

				case ID_RELOAD_RELOADHACK:
				{
					gServerInfo.ReadHackInfo();

					break;
				}

				case ID_RELOAD_RELOADITEM:
				{
					gServerInfo.ReadItemInfo();

					break;
				}

				case ID_RELOAD_RELOADMONSTER:
				{
					gServerInfo.ReloadMonsterInfo();

					break;
				}

				case ID_RELOAD_RELOADMOVE:
				{
					gServerInfo.ReadMoveInfo();

					break;
				}

				case ID_RELOAD_RELOADQUEST:
				{
					gServerInfo.ReadQuestInfo();

					break;
				}

				case ID_RELOAD_RELOADSHOP:
				{
					gServerInfo.ReadShopInfo();

					break;
				}

				case ID_RELOAD_RELOADSKILL:
				{
					gServerInfo.ReadSkillInfo();

					break;
				}

				case ID_RELOAD_RELOADUTIL:
				{
					gServerInfo.ReadUtilInfo();

					break;
				}

				case ID_RELOAD_RELOADALL:
				{
					gServerInfo.ReloadAll();

					break;
				}

				case ID_STARTBC:
				{
					gBloodCastle.StartBC();

					break;
				}

				case ID_STARTDS:
				{
					gDevilSquare.StartDS();

					break;
				}

				case ID_ONLINEUSERS:
				{
					DialogBox(hInst, (LPCTSTR)IDD_DIALOG_ONLINEUSER, hWnd, (DLGPROC)UserOnline);

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
			if (MessageBox(0, "Close GameServer?", "GameServer", MB_OKCANCEL) == IDOK)
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
					GJServerUserInfoSend();

					ConnectServerInfoSend();

					break;
				}

				case TIMER_10000:
				{
					JoinServerReconnect(hWnd);

					DataServerReconnect(hWnd);

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

INT_PTR CALLBACK UserOnline(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
		{
			HWND hwndList = GetDlgItem(hDlg, IDC_LIST1);

			for (int i = OBJECT_START_USER; i < MAX_OBJECT; i++)
			{
				if (gObj[i].Connected >= OBJECT_LOGGED && gObj[i].Type == OBJECT_USER)
				{
					TCHAR buff[30];

					wsprintf(buff, "%s >>> %s", gObj[i].Account, gObj[i].Name);

					int pos = (int)SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)buff);

					SendMessage(hwndList, LB_SETITEMDATA, pos, (LPARAM)i);
				}
			}

			SetFocus(hwndList);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case IDOK:
				case IDCANCEL:
				{
					EndDialog(hDlg, LOWORD(wParam));

					return TRUE;
				}

				case IDC_LIST1:
				{
					switch (HIWORD(wParam))
					{
						case LBN_SELCHANGE:
						{
							HWND hwndList = GetDlgItem(hDlg, IDC_LIST1);

							int lbItem = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);

							int i = (int)SendMessage(hwndList, LB_GETITEMDATA, lbItem, 0);

							SetDlgItemText(hDlg, IDC_EDIT1, gObj[i].Account);

							SetDlgItemText(hDlg, IDC_EDIT2, gObj[i].IpAddr);

							SetDlgItemText(hDlg, IDC_EDIT3, gObj[i].HardwareID);

							if (gObj[i].Name[0] == 0)
							{
								SetDlgItemText(hDlg, IDC_EDIT4, "SELECT CHARACTER");

								SetDlgItemText(hDlg, IDC_EDIT5, "SELECT CHARACTER");

								SetDlgItemText(hDlg, IDC_EDIT6, "");

								SetDlgItemText(hDlg, IDC_EDIT7, "");
							}
							else
							{
								SetDlgItemText(hDlg, IDC_EDIT4, gObj[i].Name);

								SetDlgItemText(hDlg, IDC_EDIT5, gMapManager.GetMapName(gObj[i].Map));

								TCHAR buff[5];

								wsprintf(buff, "%d", gObj[i].X);

								SetDlgItemText(hDlg, IDC_EDIT6, buff);

								wsprintf(buff, "%d", gObj[i].Y);

								SetDlgItemText(hDlg, IDC_EDIT7, buff);
							}

							return TRUE;
						}
					}

					return FALSE;
				}

				case IDC_BUTTONDC:
				{
					HWND hwndList = GetDlgItem(hDlg, IDC_LIST1);

					int lbItem = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);

					int i = (int)SendMessage(hwndList, LB_GETITEMDATA, lbItem, 0);

					gObjUserKill(gObj[i].Index);

					SendMessage(hwndList, LB_DELETESTRING, lbItem, 0);

					SetDlgItemText(hDlg, IDC_EDIT1, "");

					SetDlgItemText(hDlg, IDC_EDIT2, "");

					SetDlgItemText(hDlg, IDC_EDIT3, "");

					SetDlgItemText(hDlg, IDC_EDIT4, "");

					SetDlgItemText(hDlg, IDC_EDIT5, "");

					SetDlgItemText(hDlg, IDC_EDIT6, "");

					SetDlgItemText(hDlg, IDC_EDIT7, "");

					MessageBox(hDlg, "Account disconnected", "Confirm", MB_OK);

					return TRUE;
				}

				case IDC_BUTTONRD:
				{
					HWND hwndList = GetDlgItem(hDlg, IDC_LIST1);

					SendMessage(hwndList, LB_RESETCONTENT, 0, 0);

					for (int i = OBJECT_START_USER; i < MAX_OBJECT; i++)
					{
						if (gObj[i].Connected >= OBJECT_LOGGED && gObj[i].Type == OBJECT_USER)
						{
							TCHAR buff[30];

							wsprintf(buff, "%s >>> %s", gObj[i].Account, gObj[i].Name);

							int pos = (int)SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)buff);

							SendMessage(hwndList, LB_SETITEMDATA, pos, (LPARAM)i);
						}
					}

					SetDlgItemText(hDlg, IDC_EDIT1, "");

					SetDlgItemText(hDlg, IDC_EDIT2, "");

					SetDlgItemText(hDlg, IDC_EDIT3, "");

					SetDlgItemText(hDlg, IDC_EDIT4, "");

					SetDlgItemText(hDlg, IDC_EDIT5, "");

					SetDlgItemText(hDlg, IDC_EDIT6, "");

					SetDlgItemText(hDlg, IDC_EDIT7, "");

					MessageBox(hDlg, "Reloaded users online", "Confirm", MB_OK);

					return TRUE;
				}
			}
		}
	}

	return FALSE;
}