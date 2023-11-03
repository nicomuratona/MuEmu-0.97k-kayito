#pragma once

#include "CriticalSection.h"

#define MAX_ACCOUNT 400000

struct ACCOUNT_INFO
{
	char Account[11];
	char IpAddress[16];
	WORD UserIndex;
	WORD GameServerCode;
};

class CAccountManager
{
public:

	CAccountManager();

	~CAccountManager();

	void ClearServerAccountInfo(WORD ServerCode);

	bool GetAccountInfo(ACCOUNT_INFO* lpAccountInfo, char* account);

	void InsertAccountInfo(ACCOUNT_INFO AccountInfo);

	void RemoveAccountInfo(ACCOUNT_INFO AccountInfo);

	long GetAccountCount();

private:

	CCriticalSection m_critical;

	std::map<std::string, ACCOUNT_INFO> m_AccountInfo;
};

extern CAccountManager gAccountManager;