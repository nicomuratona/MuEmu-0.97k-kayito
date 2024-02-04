#pragma once

class cWindow
{
public:

	cWindow();

	virtual ~cWindow();

	void WindowModeLoad(HINSTANCE hins);

	void ChangeWindowText();

	void SetWindowMode(int mode);

	void SetResolution(int res);

	void ChangeWindowState();

	void ToggleTrayMode();

	void ShowTrayMessage(char* Title, char* Message);

private:

	static HWND StartWindow(HINSTANCE hCurrentInst, int nCmdShow);

	void ChangeDisplaySettingsFunction();

	void ShowTrayNotify(bool mode);

	static LONG WINAPI MyWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:

	HINSTANCE Instance;

	HICON m_TrayIcon;

public:

	int m_WindowMode;

	std::pair<WORD, WORD> iResolutionValues[MAX_RESOLUTION_VALUE];

	std::string iWindowValues[MAX_WINDOW_MODE];
};

extern cWindow gWindow;