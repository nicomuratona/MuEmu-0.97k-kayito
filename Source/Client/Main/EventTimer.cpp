#include "stdafx.h"
#include "EventTimer.h"
#include "MiniMap.h"
#include "MoveList.h"

CEventTimer gEventTimer;

CEventTimer::CEventTimer()
{
	this->EventTimerSwitch = false;

	this->MainWidth = 180.0f;

	this->MainBaseHeight = 60.0f;

	this->MainPosX = 5.0f;

	this->MainPosY = 5.0f;

	this->MainEndX = (this->MainPosX + 5.0f) + (this->MainWidth - 10.0f);

	this->m_EventTimeInfo.clear();
}

CEventTimer::~CEventTimer()
{

}

bool CEventTimer::GetEventTimerState()
{
	return this->EventTimerSwitch;
}

void CEventTimer::Toggle()
{
	if (CheckInputInterfaces())
	{
		return;
	}

	if (CheckRightInterfaces())
	{
		this->EventTimerSwitch = false;

		return;
	}

	if (gMoveList.GetMoveListState())
	{
		gMoveList.Toggle();
	}

	if (gMiniMap.GetMiniMapState())
	{
		gMiniMap.Toggle();
	}

	this->EventTimerSwitch ^= 1;

	PlayBuffer(25, 0, 0);
}

void CEventTimer::Render()
{
	if (!this->EventTimerSwitch)
	{
		return;
	}

	this->RenderFrame();

	this->RenderEventsTime();
}

void CEventTimer::UpdateMouse()
{
	if (!this->EventTimerSwitch)
	{
		return;
	}

	if (CheckRightInterfaces())
	{
		this->EventTimerSwitch = false;

		return;
	}

	if (IsWorkZone((int)this->MainPosX, (int)this->MainPosY, (int)this->MainWidth, (int)this->MainHeight))
	{
		MouseOnWindow = true;

		if (this->CheckClickOnClose())
		{
			return;
		}

		if (MouseLButton && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;

			MouseUpdateTimeMax = 6;
		}
	}
}

void CEventTimer::RenderFrame()
{
	this->MainHeight = this->MainBaseHeight + (this->m_EventTimeInfo.size() * 12.0f);

	this->MainHeight = (this->MainHeight > 430.0f) ? 430.0f : this->MainHeight;

	EnableAlphaTest(true);

	glColor4f(0.0f, 0.0f, 0.0f, 0.8f);

	RenderColor(this->MainPosX, this->MainPosY, this->MainWidth, this->MainHeight);

	glColor3f(1.0f, 1.0f, 1.0f);

	EnableAlphaTest(true);

	DWORD backupBgTextColor = SetBackgroundTextColor;

	SetBackgroundTextColor = Color4b(255, 255, 255, 0);

	DWORD backupTextColor = SetTextColor;

	SetTextColor = Color4b(255, 204, 26, 255);

	SelectObject(m_hFontDC, g_hFontBold);

	RenderText((int)this->MainPosX + 5, (int)this->MainPosY + 5, "Event Timer", (int)(this->MainWidth - 10.0f) * WindowWidth / 640, RT3_SORT_CENTER, NULL);

	SetBackgroundTextColor = Color4b(255, 255, 255, 0);

	SetTextColor = Color4b(127, 178, 255, 255);

	SelectObject(m_hFontDC, g_hFont);

	RenderText((int)this->MainPosX + 5, (int)this->MainPosY + 20, "EVENT", 0, RT3_SORT_LEFT, NULL);

	int PosX = (int)this->MainEndX - GetTextWidth("TIME");

	RenderText(PosX, (int)this->MainPosY + 20, "TIME", 0, RT3_SORT_LEFT, NULL);

	SetBackgroundTextColor = Color4b(255, 0, 0, 255);

	SetTextColor = Color4b(255, 255, 255, 255);

	SelectObject(m_hFontDC, g_hFont);

	RenderText((int)this->MainPosX + 5, (int)this->MainPosY + (int)this->MainHeight - 15, "Close", (int)(this->MainWidth - 10.0f) * WindowWidth / 640, RT3_SORT_CENTER, NULL);

	SetBackgroundTextColor = backupBgTextColor;

	SetTextColor = backupTextColor;
}

void CEventTimer::RenderEventsTime()
{
	DWORD backupBgTextColor = SetBackgroundTextColor;

	DWORD backupTextColor = SetTextColor;

	EnableAlphaTest(true);

	int PosY = (int)this->MainPosY + 35;

	if (this->m_EventTimeInfo.empty())
	{
		SetBackgroundTextColor = Color4b(255, 255, 255, 0);

		SetTextColor = Color4b(255, 255, 255, 255);

		SelectObject(m_hFontDC, g_hFontBig);

		RenderText((int)this->MainPosX + 5, PosY - 8, "NO EVENT INFO", (int)(this->MainWidth - 10.0f) * WindowWidth / 640, RT3_SORT_CENTER, NULL);
	}
	else
	{
		int PosX;

		char* buff;

		for each (PMSG_EVENT_TIME lpInfo in this->m_EventTimeInfo)
		{
			if (IsWorkZone((int)this->MainPosX + 5, PosY, (int)(this->MainWidth - 10), 10))
			{
				SetBackgroundTextColor = Color4b(204, 204, 25, 153);
			}
			else
			{
				SetBackgroundTextColor = Color4b(255, 255, 255, 0);
			}

			SetTextColor = Color4b(255, 255, 255, 255);

			SelectObject(m_hFontDC, g_hFontBold);

			RenderText((int)this->MainPosX + 5, PosY, lpInfo.name, (int)(this->MainWidth - 10.0f) * WindowWidth / 640, RT3_SORT_LEFT, NULL);

			SetBackgroundTextColor = Color4b(255, 255, 255, 0);

			this->SetEventRenderColor(lpInfo.status, lpInfo.time);

			SelectObject(m_hFontDC, g_hFont);

			buff = this->GetEventRemainTime(lpInfo.status, lpInfo.time);

			PosX = (int)this->MainEndX - GetTextWidth(buff);

			RenderText(PosX, PosY, buff, 0, RT3_SORT_LEFT, NULL);

			PosY += 12;
		}
	}

	SelectObject(m_hFontDC, g_hFont);

	SetBackgroundTextColor = backupBgTextColor;

	SetTextColor = backupTextColor;
}

bool CEventTimer::CheckClickOnClose()
{
	if (IsWorkZone((int)(this->MainPosX + 5.0f), (int)(this->MainPosY + this->MainHeight - 15.0f), (int)(this->MainWidth - 10.0f), (int)(10.0f)))
	{
		if (MouseLButton && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;

			MouseUpdateTimeMax = 6;

			this->Toggle();
		}

		return true;
	}

	return false;
}

void CEventTimer::SetEventRenderColor(int status, int time)
{
	if (status == EVENT_STATE_BLANK)
	{
		SetTextColor = Color4b(230, 5, 5, 255);
	}
	else if (status == EVENT_STATE_STAND)
	{
		if (time >= 0 && time <= 300)
		{
			SetTextColor = Color4b(230, 5, 5, 255);
		}
		else
		{
			SetTextColor = Color4b(223, 191, 103, 255);
		}
	}
	else if (status == EVENT_STATE_OPEN)
	{
		SetTextColor = Color4b(73, 183, 0, 255);
	}
	else if (status == EVENT_STATE_START)
	{
		SetTextColor = Color4b(100, 150, 255, 255);
	}
	else
	{
		SetTextColor = Color4b(223, 191, 103, 255);
	}
}

char* CEventTimer::GetEventRemainTime(int status, int time)
{
	static char buff[256];

	memset(buff, 0, sizeof(buff));

	if (status == EVENT_STATE_BLANK)
	{
		sprintf_s(buff, "Disabled");
	}
	else if (status == EVENT_STATE_STAND)
	{
		int day = time / 86400;
		time %= 86400;

		int hr = time / 3600;
		time %= 3600;

		int min = time / 60;
		time %= 60;

		if (day > 0)
		{
			sprintf_s(buff, "%02d day's", day);
		}
		else
		{
			sprintf_s(buff, "%02d:%02d:%02d", hr, min, time);
		}
	}
	else if (status == EVENT_STATE_OPEN)
	{
		sprintf_s(buff, "Open now");
	}
	else if (status == EVENT_STATE_START)
	{
		sprintf_s(buff, "Started");
	}

	return buff;
}

void CEventTimer::GCEventTimeRecv(PMSG_EVENT_TIME_RECV* lpMsg)
{
	this->m_EventTimeInfo.clear();

	for (int i = 0; i < lpMsg->count; i++)
	{
		PMSG_EVENT_TIME* lpInfo = (PMSG_EVENT_TIME*)(((BYTE*)lpMsg) + sizeof(PMSG_EVENT_TIME_RECV) + (sizeof(PMSG_EVENT_TIME) * i));

		PMSG_EVENT_TIME info;

		memcpy(info.name, lpInfo->name, sizeof(info.name));

		info.status = lpInfo->status;

		info.time = lpInfo->time;

		this->m_EventTimeInfo.push_back(info);
	}
}