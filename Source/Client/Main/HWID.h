#pragma once

class cHwid
{
public:

	cHwid();

	virtual ~cHwid();

	bool GetComputerHardwareId();

	char* GetEncryptedString(BYTE* string, int size);

	void SendHwid();

private:

	char m_ComputerHardwareId[36];
};

extern cHwid gHwid;

static BYTE gProtectString1[04] = { 0xF3, 0x8A, 0xEC, 0xB0 }; //"C:\\"

static BYTE gProtectString2[20] = { 0x95, 0x80, 0x88, 0xE8, 0x9D, 0x95, 0x80, 0x88, 0xE8, 0x9D, 0x95, 0x80, 0x88, 0xE8, 0x9D, 0x95, 0x80, 0x88, 0xE8, 0xB0 }; //"%08X-%08X-%08X-%08X"