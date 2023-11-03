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

	static void DrawMoveList(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale);

	void RenderMoveListBack();

	void RenderMoveListMaps();

	void CheckMoveListMouse();

	bool CheckClickOnMap();

	bool CheckClickOnClose();

	bool CheckClickOnPanel();

	void GCMoveListRecv(PMSG_MOVE_LIST_RECV* lpMsg);

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