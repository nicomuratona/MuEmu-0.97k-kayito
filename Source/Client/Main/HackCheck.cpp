#include "stdafx.h"
#include "HackCheck.h"
#include "Protect.h"

typedef int(WINAPI* WSRECV)(SOCKET, BYTE*, int, int);

typedef int(WINAPI* WSSEND)(SOCKET, BYTE*, int, int);

WSRECV HookRecv;

WSSEND HookSend;

BYTE EncDecKey1;

BYTE EncDecKey2;

void DecryptData(BYTE* lpMsg, int size)
{
	for (int n = 0; n < size; n++)
	{
		lpMsg[n] = (lpMsg[n] ^ EncDecKey1) - (EncDecKey2 * EncDecKey1);
	}
}

void EncryptData(BYTE* lpMsg, int size)
{
	for (int n = 0; n < size; n++)
	{
		lpMsg[n] = (lpMsg[n] + (EncDecKey2 * EncDecKey1)) ^ EncDecKey1;
	}
}

bool CheckSocketPort(SOCKET s)
{
	SOCKADDR_IN addr;

	int addr_len = sizeof(addr);

	if (getpeername(s, (SOCKADDR*)&addr, &addr_len) == SOCKET_ERROR)
	{
		return false;
	}

	if (PORT_RANGE(ntohs(addr.sin_port)) == 0)
	{
		return false;
	}

	return true;
}

int WINAPI MyRecv(SOCKET s, BYTE* buf, int len, int flags)
{
	int result = HookRecv(s, buf, len, flags);

	if (result == SOCKET_ERROR || result == 0)
	{
		return result;
	}

	if (CheckSocketPort(s) != false)
	{
		DecryptData(buf, result);
	}

	return result;
}

int WINAPI MySend(SOCKET s, BYTE* buf, int len, int flags)
{
	if (CheckSocketPort(s) != false)
	{
		EncryptData(buf, len);
	}

	return HookSend(s, buf, len, flags);
}

void InitHackCheck()
{
	WORD EncDecKey = 0;

	for (int n = 0; n < sizeof(gProtect.m_MainInfo.CustomerName); n++)
	{
		EncDecKey += (BYTE)(gProtect.m_MainInfo.CustomerName[n] ^ gProtect.m_MainInfo.ClientSerial[(n % sizeof(gProtect.m_MainInfo.ClientSerial))]);

		EncDecKey ^= (BYTE)(gProtect.m_MainInfo.CustomerName[n] - gProtect.m_MainInfo.ClientSerial[(n % sizeof(gProtect.m_MainInfo.ClientSerial))]);
	}

	EncDecKey1 = (BYTE)0xB0;

	EncDecKey2 = (BYTE)0xF8;

	EncDecKey1 += LOBYTE(EncDecKey);

	EncDecKey2 += HIBYTE(EncDecKey);

	HookRecv = *(WSRECV*)(0x00552430);

	HookSend = *(WSSEND*)(0x00552440);

	SetDword(0x00552430, (DWORD)&MyRecv);

	SetDword(0x00552440, (DWORD)&MySend);
}