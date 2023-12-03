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

private:

	static HWND StartWindow(HINSTANCE hCurrentInst, int nCmdShow);

	void ChangeDisplaySettingsFunction();

	static LONG WINAPI MyWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:

	HINSTANCE Instance;

public:

	int m_WindowMode;

	std::pair<WORD, WORD> iResolutionValues[MAX_RESOLUTION_VALUE];

	std::string iWindowValues[MAX_WINDOW_MODE];
};

extern cWindow gWindow;