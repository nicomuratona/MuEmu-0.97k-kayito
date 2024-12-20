#pragma once

class CWindow
{
public:

	CWindow();

	virtual ~CWindow();

	void WindowModeLoad(HINSTANCE hins);

	void ToggleTrayMode();

	void ShowTrayMessage(char* Title, char* Message);

	void ChangeWindowText();

	void SetWindowMode(bool windowMode, bool borderless);

	void SetResolution(int res);

	void ChangeWindowState(bool windowMode, bool borderless, int resolution);

private:

	static LONG WINAPI FixDisplaySettingsOnClose(DEVMODEA* lpDevMode, DWORD dwFlags);

	static LONG WINAPI MyWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static HWND StartWindow(HINSTANCE hCurrentInst, int nCmdShow);

	void ChangeDisplaySettingsFunction();

	static bool CreateOpenglWindow();

	void ShowTrayNotify(bool mode);

private:

	HINSTANCE Instance;

	HICON m_WindowIcon;

	char m_WindowName[256];

public:

	bool m_WindowMode;

	bool m_Borderless;

	std::pair<WORD, WORD> iResolutionValues[MAX_RESOLUTION_VALUE];
};

extern CWindow gWindow;