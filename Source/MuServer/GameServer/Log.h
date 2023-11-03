#pragma once

#define MAX_LOG 7

enum eLogType
{
	LOG_GENERAL = 0,
	LOG_CHAT = 1,
	LOG_COMMAND = 2,
	LOG_TRADE = 3,
	LOG_CONNECT = 4,
	LOG_HACK = 5,
	LOG_CHAOS_MIX = 6,
};

struct LOG_INFO
{
	int Active;
	char Directory[256];
	int Day;
	int Month;
	int Year;
	char Filename[256];
	HANDLE File;
};

class CLog
{
public:

	CLog();

	~CLog();

	void AddLog(int active, char* directory);

	void Output(eLogType type, char* text, ...);

private:

	LOG_INFO m_LogInfo[MAX_LOG];

	int m_count;
};

extern CLog gLog;