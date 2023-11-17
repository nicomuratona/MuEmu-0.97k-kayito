#include "stdafx.h"
#include "ServerDisplayer.h"
#include "GameMain.h"
#include "Log.h"
#include "ServerInfo.h"
#include "SocketManager.h"
#include "User.h"

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

	gLog.AddLog(gServerInfo.m_WriteChatLog, "CHAT_LOG");

	gLog.AddLog(gServerInfo.m_WriteCommandLog, "COMMAND_LOG");

	gLog.AddLog(gServerInfo.m_WriteTradeLog, "TRADE_LOG");

	gLog.AddLog(gServerInfo.m_WriteConnectLog, "CONNECT_LOG");

	gLog.AddLog(gServerInfo.m_WriteHackLog, "HACK_LOG");

	gLog.AddLog(gServerInfo.m_WriteChaosMixLog, "CHAOS_MIX_LOG");
}

void CServerDisplayer::Run()
{
	this->LogTextPaint();

	this->PaintAllInfo();
}

void CServerDisplayer::SetWindowName()
{
	char buff[256];

	wsprintf(buff, "[%s] MuEmu GameServer Ex097 (PlayerCount : %d/%d) (MonsterCount : %d/%d)", gServerInfo.m_WindowName, gObjTotalUser, gServerInfo.m_ServerMaxUserNumber, gObjTotalMonster, MAX_OBJECT_MONSTER);

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

	if (gJoinServerConnection.CheckState() == 0 || gDataServerConnection.CheckState() == 0)
	{
		SetTextColor(hdc, RGB(200, 200, 200));

		FillRect(hdc, &rect, this->m_brush[0]);

		DrawText(hdc, this->m_DisplayerText[0], strlen(this->m_DisplayerText[0]), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		gGameServerDisconnect = 0;

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

			case LOG_ALERT:
			{
				SetBkMode(hdc, BKMODE_LAST);

				SetBkColor(hdc, RGB(255, 0, 0));

				SetTextColor(hdc, RGB(0, 0, 0));

				break;
			}
			case LOG_USER:
			{
				SetBkMode(hdc, TRANSPARENT);

				SetTextColor(hdc, RGB(140, 30, 160));

				break;
			}

			case LOG_EVENT:
			{
				SetBkMode(hdc, TRANSPARENT);

				SetTextColor(hdc, RGB(64, 192, 192));

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

	gLog.Output(LOG_GENERAL, "%s", &text[9]);

	this->Run();
}