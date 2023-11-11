#pragma once

#define MIN_PORT 55901

#define MAX_PORT 55950

#define PORT_RANGE(x) (((x)<MIN_PORT)?0:((x)>MAX_PORT)?0:1)

class CHackCheck
{
private:

	typedef int(WINAPI* WSRECV)(SOCKET, BYTE*, int, int);

	typedef int(WINAPI* WSSEND)(SOCKET, BYTE*, int, int);

public:

	void Init();

	static int WINAPI MySend(SOCKET s, BYTE* buf, int len, int flags);

	bool CheckSocketPort(SOCKET s);

private:

	static int WINAPI MyRecv(SOCKET s, BYTE* buf, int len, int flags);

	void DecryptData(BYTE* lpMsg, int size);

	void EncryptData(BYTE* lpMsg, int size);

private:

	BYTE EncDecKey1;

	BYTE EncDecKey2;

	WSRECV HookRecv;

	WSSEND HookSend;
};

extern CHackCheck gHackCheck;