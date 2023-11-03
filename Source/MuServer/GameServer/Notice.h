#pragma once

#include "ProtocolDefines.h"

#define MAX_NOTICE 100

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_NOTICE_SEND
{
	PBMSG_HEAD header; // C1:0D
	BYTE type;
	char message[256];
};

//**********************************************//
//**********************************************//
//**********************************************//

struct NOTICE_INFO
{
	char Message[128];
	int Type;
	int RepeatTime;
};

class CNotice
{
public:

	CNotice();

	~CNotice();

	void Load(char* path);

	void SetInfo(NOTICE_INFO info);

	void MainProc();

	void GCNoticeSend(int aIndex, BYTE type, char* message, ...);

	void GCNoticeSendToAll(BYTE type, char* message, ...);

	void GCNoticeSendToAll(BYTE type, int message, ...);

private:

	NOTICE_INFO m_NoticeInfo[MAX_NOTICE];

	int m_count;

	int m_NoticeValue;

	DWORD m_NoticeTime;
};

extern CNotice gNotice;