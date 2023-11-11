#pragma once

enum eReconnectStatus
{
	RECONNECT_STATUS_NONE = 0,
	RECONNECT_STATUS_RECONNECT = 1,
	RECONNECT_STATUS_DISCONNECT = 2,
};

enum eReconnectProgress
{
	RECONNECT_PROGRESS_NONE = 0,
	RECONNECT_PROGRESS_CONNECTED = 1,
	RECONNECT_PROGRESS_JOINED = 2,
	RECONNECT_PROGRESS_CHAR_LIST = 3,
	RECONNECT_PROGRESS_CHAR_INFO = 4,
};

class CReconnect
{
public:

	CReconnect();

	virtual ~CReconnect();

	void Init();

private:

	static void ReconnectDrawInterface();

	static void ReconnectMainProc();

	void ReconnectToGameServer();

	void ReconnectIntoAccount();

	static void ReconnectGetAccountInfo();

	static void ReconnectCheckConnection();

	static BOOL ReconnectCloseSocket(DWORD This);

	void ReconnectOnCloseSocket();

	void ReconnectSetInfo(eReconnectStatus status, eReconnectProgress progress, DWORD CurWait, DWORD MaxWait);

	void ReconnectViewportDestroy();

	static void ReconnectMenuExitGame();

	static void ReconnectServerConnect();

	static BOOL ReconnectCreateConnection(char* IpAddr, unsigned short Port);

public:

	void ReconnectOnConnectAccount(BYTE result);

	void ReconnectOnCloseClient(BYTE result);

	void ReconnectOnCharacterList();

	void ReconnectOnCharacterInfo();

private:

	char GameServerAddress[16];

	WORD GameServerPort;

	char ReconnectAccount[11];

	char ReconnectPassword[11];

	char ReconnectName[11];

	eReconnectStatus ReconnectStatus;

	eReconnectProgress ReconnectProgress;

	DWORD ReconnectCurTime;

	DWORD ReconnectMaxTime;

	DWORD ReconnectCurWait;

	DWORD ReconnectMaxWait;

	BYTE ReconnectAuthSend;
};

extern CReconnect gReconnect;