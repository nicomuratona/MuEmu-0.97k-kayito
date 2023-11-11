#include "stdafx.h"
#include "HackCheck.h"
#include "Protect.h"

CHackCheck gHackCheck;

void CHackCheck::Init()
{
	WORD EncDecKey = 0;

	for (int n = 0; n < sizeof(gProtect.m_MainInfo.CustomerName); n++)
	{
		EncDecKey += (BYTE)(gProtect.m_MainInfo.CustomerName[n] ^ gProtect.m_MainInfo.ClientSerial[(n % sizeof(gProtect.m_MainInfo.ClientSerial))]);

		EncDecKey ^= (BYTE)(gProtect.m_MainInfo.CustomerName[n] - gProtect.m_MainInfo.ClientSerial[(n % sizeof(gProtect.m_MainInfo.ClientSerial))]);
	}

	this->EncDecKey1 = (BYTE)0xB0;

	this->EncDecKey2 = (BYTE)0xF8;

	this->EncDecKey1 += LOBYTE(EncDecKey);

	this->EncDecKey2 += HIBYTE(EncDecKey);

	this->HookRecv = *(WSRECV*)(0x00552430);

	this->HookSend = *(WSSEND*)(0x00552440);

	SetDword(0x00552430, (DWORD)&this->MyRecv);

	SetDword(0x00552440, (DWORD)&this->MySend);
}

int WINAPI CHackCheck::MyRecv(SOCKET s, BYTE* buf, int len, int flags)
{
	int result = gHackCheck.HookRecv(s, buf, len, flags);

	if (result == SOCKET_ERROR || result == 0)
	{
		return result;
	}

	if (gHackCheck.CheckSocketPort(s))
	{
		gHackCheck.DecryptData(buf, result);
	}

	return result;
}

int WINAPI CHackCheck::MySend(SOCKET s, BYTE* buf, int len, int flags)
{
	if (gHackCheck.CheckSocketPort(s))
	{
		gHackCheck.EncryptData(buf, len);
	}

	return gHackCheck.HookSend(s, buf, len, flags);
}

bool CHackCheck::CheckSocketPort(SOCKET s)
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

void CHackCheck::DecryptData(BYTE* lpMsg, int size)
{
	for (int n = 0; n < size; n++)
	{
		lpMsg[n] = (lpMsg[n] ^ this->EncDecKey1) - (this->EncDecKey2 * this->EncDecKey1);
	}
}

void CHackCheck::EncryptData(BYTE* lpMsg, int size)
{
	for (int n = 0; n < size; n++)
	{
		lpMsg[n] = (lpMsg[n] + (this->EncDecKey2 * this->EncDecKey1)) ^ this->EncDecKey1;
	}
}