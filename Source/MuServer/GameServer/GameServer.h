#pragma once

#define MAX_LOADSTRING 100

#define TIMER_1000 100
#define TIMER_10000 101

#define WM_JOIN_SERVER_MSG_PROC (WM_USER+1)
#define WM_DATA_SERVER_MSG_PROC (WM_USER+2)

ATOM MyRegisterClass(HINSTANCE hInstance);

bool InitInstance(HINSTANCE hInstance);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

INT_PTR CALLBACK UserOnline(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);