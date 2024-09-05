#pragma once

#include "ProtocolDefines.h"

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_MOVE_LIST_RECV
{
	PSWMSG_HEAD header; // C2:F3:E5
	BYTE count;
};

struct MOVE_LIST_INFO
{
	BYTE MapNumber;
	char MapName[32];
	bool CanMove;
	WORD LevelMin;
	DWORD Money;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CMoveList
{
public:

	CMoveList();

	virtual ~CMoveList();

	bool GetMoveListState();

	void Toggle();

	void Render();

	void UpdateMouse();

	void GCMoveListRecv(PMSG_MOVE_LIST_RECV* lpMsg);

private:

	void RenderFrame();

	void RenderMapsList();

	bool CheckClickOnMap();

	bool CheckClickOnClose();

private:

	bool MoveListSwitch;

	float MainWidth;

	float MainHeight;

	float MainBaseHeight;

	float MainPosX;

	float MainPosY;

	float SectionWidth;

	float MapNamePosX;

	float ReqLevelPosX;

	float ReqMoneyPosX;

	std::vector<MOVE_LIST_INFO> m_MoveList;
};

extern CMoveList gMoveList;