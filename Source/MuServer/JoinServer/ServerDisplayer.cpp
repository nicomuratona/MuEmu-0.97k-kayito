#include "stdafx.h"
#include "ServerDisplayer.h"
#include "AccountManager.h"
#include "JoinServerProtocol.h"
#include "Log.h"
#include "ServerManager.h"
#include "SocketManager.h"

CServerDisplayer gServerDisplayer;

CServerDisplayer::CServerDisplayer()
{
	for (int n = 0; n < MAX_LOG_TEXT_LINE; n++)
	{
		memset(&this->m_log[n], 0, sizeof(this->m_log[n]));
	}

	this->m_font = CreateFont(70, 0, 0, 0, FW_THIN, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Tahoma");

	this->m_brush[0] = CreateSolidBrush(RGB(105, 105, 105)); //Offline - Standby

	strcpy_s(this->m_DisplayerText[0], "STANDBY MODE");

	this->m_brush[1] = CreateSolidBrush(RGB(140, 30, 160)); //Active mode

	strcpy_s(this->m_DisplayerText[1], "ACTIVE MODE");
}

CServerDisplayer::~CServerDisplayer()
{
	DeleteObject(this->m_font);

	DeleteObject(this->m_brush[0]);

	DeleteObject(this->m_brush[1]);
}

void CServerDisplayer::Init(HWND hWnd)
{
	this->m_hwnd = hWnd;

	gLog.AddLog(true, "LOG");

	gLog.AddLog(true, "LOG_ACCOUNT");
}

void CServerDisplayer::Run()
{
	this->SetWindowName();

	this->LogTextPaint();

	this->PaintAllInfo();
}

void CServerDisplayer::SetWindowName()
{
	char buff[256];

	wsprintf(buff, "MuEmu JoinServer Ex097 (QueueSize : %d) (AccountCount : %d/%d)", gSocketManager.GetQueueSize(), gAccountManager.GetAccountCount(), MAX_ACCOUNT);

	SetWindowText(this->m_hwnd, buff);
}

void CServerDisplayer::PaintAllInfo()
{
	RECT rect;

	GetClientRect(this->m_hwnd, &rect);

	rect.top = 0;

	rect.bottom = 80;

	HDC hdc = GetDC(this->m_hwnd);

	int OldBkMode = SetBkMode(hdc, TRANSPARENT);

	HFONT OldFont = (HFONT)SelectObject(hdc, this->m_font);

	bool state = false;

	for (int n = 0; n < MAX_SERVER; n++)
	{
		if (gServerManager[n].CheckState() == false)
		{
			continue;
		}

		if ((GetTickCount() - gServerManager[n].m_PacketTime) <= 60000)
		{
			state = true;

			break;
		}
	}

	if (state == false)
	{
		SetTextColor(hdc, RGB(200, 200, 200));

		FillRect(hdc, &rect, this->m_brush[0]);

		DrawText(hdc, this->m_DisplayerText[0], strlen(this->m_DisplayerText[0]), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		SetTextColor(hdc, RGB(250, 250, 250));

		FillRect(hdc, &rect, this->m_brush[1]);

		DrawText(hdc, this->m_DisplayerText[1], strlen(this->m_DisplayerText[1]), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	SelectObject(hdc, OldFont);

	SetBkMode(hdc, OldBkMode);

	ReleaseDC(this->m_hwnd, hdc);
}

void CServerDisplayer::LogTextPaint()
{
	RECT rect;

	GetClientRect(this->m_hwnd, &rect);

	rect.top = 80;

	HDC hdc = GetDC(this->m_hwnd);

	FillRect(hdc, &rect, (HBRUSH)COLOR_CAPTIONTEXT);

	int line = MAX_LOG_TEXT_LINE;

	int count = (((this->m_count - 1) >= 0) ? (this->m_count - 1) : (MAX_LOG_TEXT_LINE - 1));

	for (int n = 0; n < MAX_LOG_TEXT_LINE; n++)
	{
		switch (this->m_log[count].color)
		{
			case LOG_BLACK:
			{
				SetBkMode(hdc, TRANSPARENT);

				SetTextColor(hdc, RGB(255, 255, 255));

				break;
			}

			case LOG_RED:
			{
				SetBkMode(hdc, TRANSPARENT);

				SetTextColor(hdc, RGB(255, 0, 0));

				break;
			}

			case LOG_GREEN:
			{
				SetBkMode(hdc, TRANSPARENT);

				SetTextColor(hdc, RGB(0, 255, 0));

				break;
			}

			case LOG_BLUE:
			{
				SetBkMode(hdc, TRANSPARENT);

				SetTextColor(hdc, RGB(0, 0, 255));

				break;
			}
		}

		int size = strlen(this->m_log[count].text);

		if (size > 1)
		{
			TextOut(hdc, 10, (rect.bottom - 475 + (line * 15)), this->m_log[count].text, size);

			line--;
		}

		count = (((--count) >= 0) ? count : (MAX_LOG_TEXT_LINE - 1));
	}

	ReleaseDC(this->m_hwnd, hdc);
}

void CServerDisplayer::LogAddText(eLogColor color, char* text, int size)
{
	size = ((size >= MAX_LOG_TEXT_SIZE) ? (MAX_LOG_TEXT_SIZE - 1) : size);

	memset(&this->m_log[this->m_count].text, 0, sizeof(this->m_log[this->m_count].text));

	memcpy(&this->m_log[this->m_count].text, text, size);

	this->m_log[this->m_count].color = color;

	this->m_count = (((++this->m_count) >= MAX_LOG_TEXT_LINE) ? 0 : this->m_count);

	gLog.Output(LOG_GENERAL, "%s", &text[11]);
}