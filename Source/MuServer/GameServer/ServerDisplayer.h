#pragma once

#define MAX_LOG_TEXT_LINE 30
#define MAX_LOG_TEXT_SIZE 256

enum eLogColor
{
	LOG_BLACK = 0,
	LOG_RED = 1,
	LOG_GREEN = 2,
	LOG_BLUE = 3,
	LOG_USER = 4,
	LOG_EVENT = 5,
	LOG_ALERT = 6,
};

struct LOG_DISPLAY_INFO
{
	char text[MAX_LOG_TEXT_SIZE];
	eLogColor color;
};

class CServerDisplayer
{
public:

	CServerDisplayer();

	~CServerDisplayer();

	void Init(HWND hWnd);

	void Run();

	void SetWindowName();

	void PaintAllInfo();

	void LogTextPaint();

	void LogAddText(eLogColor color, char* text, int size);

private:

	HWND m_hwnd;

	HFONT m_font;

	HBRUSH m_brush[2];

	LOG_DISPLAY_INFO m_log[MAX_LOG_TEXT_LINE];

	int m_count;

	char m_DisplayerText[2][64];
};

extern CServerDisplayer gServerDisplayer;