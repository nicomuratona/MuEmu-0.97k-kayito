#pragma once

#include "ProtocolDefines.h"
#include "User.h"

#define MAX_MOVE 100

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_TELEPORT_RECV
{
	PBMSG_HEAD header; // C1:1C
	BYTE gate;
	BYTE x;
	BYTE y;
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_TELEPORT_SEND
{
	PBMSG_HEAD header; // C3:1C
	BYTE gate;
	BYTE map;
	BYTE x;
	BYTE y;
	BYTE dir;
};

struct PMSG_MOVE_LIST_SEND
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

struct MOVE_INFO
{
	int Index;
	char Name[32];
	int Money;
	int MinLevel;
	int MaxLevel;
	int MinReset;
	int MaxReset;
	int AccountLevel;
	int Gate;
};

class CMove
{
public:

	CMove();

	~CMove();

	void Load(char* path);

	bool GetInfo(int index, MOVE_INFO* lpInfo);

	bool GetInfoByName(char* name, MOVE_INFO* lpInfo);

	void Move(LPOBJ lpObj, int index);

	void CGTeleportRecv(PMSG_TELEPORT_RECV* lpMsg, int aIndex);

	void GCTeleportSend(int aIndex, int gate, BYTE map, BYTE x, BYTE y, BYTE dir);

	void GCMoveListSend(LPOBJ lpObj);

	void ReloadMoveList();

private:

	std::map<int, MOVE_INFO> m_MoveInfo;
};

extern CMove gMove;