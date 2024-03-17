#pragma once

#include "ProtocolDefines.h"

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_EVENT_TIME_RECV
{
	PSWMSG_HEAD header; // C2:F3:E6
	BYTE count;
};

struct PMSG_EVENT_TIME
{
	char name[32];
	BYTE status;
	DWORD time;
};

//**********************************************//
//**********************************************//
//**********************************************//

enum eEventTimeState
{
	EVENT_STATE_BLANK = 0,
	EVENT_STATE_STAND = 1,
	EVENT_STATE_OPEN = 2,
	EVENT_STATE_START = 3,
};

class CEventTimer
{
public:

	CEventTimer();

	virtual ~CEventTimer();

	void Toggle();

	void Render();

	void UpdateMouse();

	void GCEventTimeRecv(PMSG_EVENT_TIME_RECV* lpMsg);

private:

	bool CheckInterfaces();

	void RenderFrame();

	void RenderEventsTime();

	bool CheckClickOnClose();

	void SetEventRenderColor(int status, int time);

	char* GetEventRemainTime(int status, int time);

private:

	bool EventTimerSwitch;

	float MainWidth;

	float MainHeight;

	float MainBaseHeight;

	float MainPosX;

	float MainPosY;

	float MainEndX;

	std::vector<PMSG_EVENT_TIME> m_EventTimeInfo;
};

extern CEventTimer gEventTimer;