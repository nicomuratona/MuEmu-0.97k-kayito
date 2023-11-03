#pragma once

class cWindow
{
public:

	cWindow();

	virtual ~cWindow();

	void WindowModeLoad(HINSTANCE hins);

	void ChangeWindowText();

private:

	static LONG WINAPI ChangeDisplaySettingsHook(DEVMODEA* lpDevMode, DWORD dwFlags);

	void ChangeDisplaySettingsFunction();

	static HWND StartWindow(HINSTANCE hCurrentInst, int nCmdShow);

	static LONG WINAPI MyWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HINSTANCE Instance;

	int m_WindowMode;
};

extern cWindow gWindow;