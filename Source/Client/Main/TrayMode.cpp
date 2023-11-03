#include "stdafx.h"
#include "resource.h"
#include "TrayMode.h"

CTrayMode gTrayMode;

CTrayMode::CTrayMode()
{

}

CTrayMode::~CTrayMode()
{

}

void CTrayMode::Init(HINSTANCE hins)
{
	this->m_TrayIcon = (HICON)LoadImage(hins, MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);

	this->m_MainWndProc = 0;
}

void CTrayMode::Toggle()
{
	if (this->GetMainWndProc() != 0)
	{
		if (IsWindowVisible(g_hWnd) == FALSE)
		{
			ShowWindow(g_hWnd, SW_SHOW);

			this->ShowNotify(false);
		}
		else
		{
			ShowWindow(g_hWnd, SW_HIDE);

			this->ShowNotify(true);

			this->ShowMessage(NIIF_INFO, "MU Online", "Ha sido minimizado.");
		}
	}
}

void CTrayMode::ShowMessage(DWORD Type, char* Title, char* Message)
{
	NOTIFYICONDATA Icon = { 0 };

	Icon.cbSize = sizeof(NOTIFYICONDATA);

	Icon.uID = WM_TRAY_MODE_ICON;

	Icon.hWnd = g_hWnd;

	Icon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_INFO;

	Icon.hIcon = this->m_TrayIcon;

	Icon.uCallbackMessage = WM_TRAY_MODE_MESSAGE;

	Icon.dwInfoFlags = Type;

	Icon.uTimeout = 5000;

	strcpy_s(Icon.szInfo, Message);

	strcpy_s(Icon.szInfoTitle, Title);

	Shell_NotifyIcon(NIM_MODIFY, &Icon);
}

LONG CTrayMode::GetMainWndProc()
{
	return ((this->m_MainWndProc == 0) ? ((LONG)(this->m_MainWndProc = (WNDPROC)SetWindowLong(g_hWnd, GWL_WNDPROC, (LONG)CTrayMode::TrayModeWndProc))) : ((LONG)this->m_MainWndProc));
}

void CTrayMode::ShowNotify(bool mode)
{
	NOTIFYICONDATA nid;

	memset(&nid, 0, sizeof(nid));

	nid.cbSize = sizeof(NOTIFYICONDATA);

	nid.hWnd = g_hWnd;

	nid.uID = WM_TRAY_MODE_ICON;

	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;

	nid.uCallbackMessage = WM_TRAY_MODE_MESSAGE;

	nid.hIcon = this->m_TrayIcon;

	wsprintf(nid.szTip, "MuOnline");

	Shell_NotifyIcon(((mode == false) ? NIM_DELETE : NIM_ADD), &nid);
}

LRESULT CALLBACK CTrayMode::TrayModeWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_TRAY_MODE_MESSAGE:
		{
			switch (lParam)
			{
				case WM_LBUTTONDOWN:
				{
					gTrayMode.Toggle();

					break;
				}
			}

			break;
		}
	}

	return CallWindowProc((WNDPROC)gTrayMode.GetMainWndProc(), hWnd, message, wParam, lParam);
}