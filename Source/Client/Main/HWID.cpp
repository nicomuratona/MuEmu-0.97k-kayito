#include "stdafx.h"
#include "HWID.h"
#include "Protocol.h"

cHwid gHwid;

cHwid::cHwid()
{
	memset(this->m_ComputerHardwareId, 0, sizeof(this->m_ComputerHardwareId));

	if (this->GetComputerHardwareId() == 0)
	{
		MessageBoxA(NULL, "Failed getting HWID", "Error", MB_OK);

		ExitProcess(0);
	}
}

cHwid::~cHwid()
{

}

bool cHwid::GetComputerHardwareId()
{
	DWORD VolumeSerialNumber;

	if (GetVolumeInformation(this->GetEncryptedString(gProtectString1, sizeof(gProtectString1)), 0, 0, &VolumeSerialNumber, 0, 0, 0, 0) == 0)
	{
		return 0;
	}

	UUID uuid;

	UuidCreateSequential(&uuid);

	SYSTEM_INFO SystemInfo;

	GetSystemInfo(&SystemInfo);

	DWORD ComputerHardwareId1 = VolumeSerialNumber ^ 0x12B586FE;

	DWORD ComputerHardwareId2 = *(DWORD*)(&uuid.Data4[2]) ^ 0x5D78A569;

	DWORD ComputerHardwareId3 = ((*(WORD*)(&uuid.Data4[6]) & 0xFFFF) | (SystemInfo.wProcessorArchitecture << 16)) ^ 0xF45BC123;

	DWORD ComputerHardwareId4 = ((SystemInfo.wProcessorLevel & 0xFFFF) | (SystemInfo.wProcessorRevision << 16)) ^ 0xB542D8E1;

	wsprintf(this->m_ComputerHardwareId, this->GetEncryptedString(gProtectString2, sizeof(gProtectString2)), ComputerHardwareId1, ComputerHardwareId2, ComputerHardwareId3, ComputerHardwareId4);

	return 1;
}

char* cHwid::GetEncryptedString(BYTE* string, int size)
{
	static char buff[256];

	memset(buff, 0, sizeof(buff));

	for (int n = 0; n < size; n++)
	{
		buff[n] = string[n] ^ 0xB0;
	}

	return buff;
}

void cHwid::SendHwid()
{
	PMSG_SET_HWID_SEND pMsg;

	pMsg.header.setE(0xF1, 0x05, sizeof(pMsg));

	memcpy(pMsg.HardwareId, this->m_ComputerHardwareId, sizeof(pMsg.HardwareId));

	gProtocol.DataSend((BYTE*)&pMsg, pMsg.header.size);
}