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
	WORD LevelMin;
	DWORD Money;
};

//**********************************************//
//**********************************************//
//**********************************************//

struct MAP_INFO
{
	BYTE MapNumber;
	char MapName[32];
	WORD LevelMin;
	DWORD Money;
	bool CanMove;
	bool Selected;
};

class CMoveList
{
public:

	CMoveList();

	virtual ~CMoveList();

	void Init();

	void Toggle();

	void CheckMoveListMouse();

	void GCMoveListRecv(PMSG_MOVE_LIST_RECV* lpMsg);

private:

	static void MainProc();

	void RenderMoveListBack();

	void RenderMoveListMaps();

	bool CheckClickOnMap();

	bool CheckClickOnClose();

private:

	bool m_MoveListSwitch;

	float MainWidth;

	float MainHeight;

	float MainBaseHeight;

	float MainPosX;

	float MainPosY;

	float SectionWidth;

	float MapNamePosX;

	float ReqLevelPosX;

	float ReqMoneyPosX;

	std::vector<MAP_INFO> m_MapList;
};

extern CMoveList gMoveList;