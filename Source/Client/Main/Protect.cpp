#include "stdafx.h"
#include "Protect.h"
#include "CCRC32.H"

CProtect gProtect;

CProtect::CProtect()
{

}

CProtect::~CProtect()
{

}

bool CProtect::ReadMainFile(char* name)
{
	CCRC32 CRC32;

	if (!CRC32.FileCRC(name, &this->m_ClientFileCRC, 1024))
	{
		return false;
	}

	HANDLE file = CreateFile(name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, 0);

	if (file == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	if (GetFileSize(file, 0) != sizeof(MAIN_FILE_INFO))
	{
		CloseHandle(file);

		return false;
	}

	DWORD OutSize = 0;

	if (ReadFile(file, &this->m_MainInfo, sizeof(MAIN_FILE_INFO), &OutSize, 0) == FALSE)
	{
		CloseHandle(file);

		return false;
	}

	BYTE XorKey[20] = { 0x9E, 0x98, 0x12, 0x2B, 0xB6, 0x76, 0xF6, 0x5B, 0x76, 0x4D, 0x08, 0xF2, 0xAB, 0xDF, 0x91, 0xB6, 0x4C, 0x1C, 0x93, 0x2F };

	for (int n = 0; n < sizeof(MAIN_FILE_INFO); n++)
	{
		((BYTE*)&this->m_MainInfo)[n] -= (BYTE)(XorKey[n % 20] ^ HIBYTE(n));

		((BYTE*)&this->m_MainInfo)[n] += (BYTE)(XorKey[n % 20] ^ HIBYTE(n));

		((BYTE*)&this->m_MainInfo)[n] ^= (BYTE)(XorKey[n % 20] ^ LOBYTE(n));
	}

	CloseHandle(file);

	return true;
}

void CProtect::CheckLauncher()
{
	if (this->m_MainInfo.LauncherType == 0)
	{
		return;
	}

	if (this->m_MainInfo.LauncherType == 1)
	{
		HANDLE H = OpenMutex(MUTEX_ALL_ACCESS, 0, this->m_MainInfo.LauncherName);

		if (H == NULL)
		{
			MessageBox(NULL, "Open Game with launcher!", "Error", MB_OK | MB_ICONERROR);

			ExitProcess(0);
		}

		ReleaseMutex(H);

		CloseHandle(H);
	}
	else if (this->m_MainInfo.LauncherType == 2)
	{
		if (FindWindow(NULL, this->m_MainInfo.LauncherName) == NULL)
		{
			MessageBox(NULL, "Open Game with launcher!", "Error", MB_OK | MB_ICONERROR);

			ExitProcess(0);
		}
	}
}

void CProtect::CheckInstance()
{
	if ((this->m_MainInfo.MultiInstance & 1) == 0)
	{
		return;
	}

	char buff[256];

	wsprintf(buff, "KAYITO_MAIN_09711_%s", this->m_MainInfo.IpAddress);

	if (OpenMutex(MUTEX_ALL_ACCESS, 0, buff) == NULL)
	{
		CreateMutex(0, 0, buff);
	}
	else
	{
		MessageBox(NULL, "You can only run 1 game at the same time!", "Error", MB_OK | MB_ICONERROR);

		ExitProcess(0);
	}
}

void CProtect::CheckClientFile()
{
	if (this->m_MainInfo.ClientCRC32 == 0)
	{
		return;
	}

	char name[MAX_PATH] = { 0 };

	if (GetModuleFileName(0, name, sizeof(name)) == 0)
	{
		MessageBox(NULL, "Failed on get module file name!", "Error", MB_OK | MB_ICONERROR);

		ExitProcess(0);
	}

	if (_stricmp(ConvertModuleFileName(name), this->m_MainInfo.ClientName) != 0)
	{
		MessageBox(NULL, "Failed on convert module file name!", "Error", MB_OK | MB_ICONERROR);

		ExitProcess(0);
	}

	CCRC32 CRC32;

	DWORD ClientCRC32;

	if (!CRC32.FileCRC(this->m_MainInfo.ClientName, &ClientCRC32, 1024))
	{
		MessageBox(NULL, "Failed on reading the client name crc!", "Error", MB_OK | MB_ICONERROR);

		ExitProcess(0);
	}

	if (this->m_MainInfo.ClientCRC32 != ClientCRC32)
	{
		MessageBox(NULL, "Client file CRC doesn't match!", "Error", MB_OK | MB_ICONERROR);

		ExitProcess(0);
	}
}

void CProtect::CheckPluginFile()
{
	if (this->m_MainInfo.PluginCRC32 == 0)
	{
		return;
	}

	CCRC32 CRC32;

	DWORD PluginCRC32;

	if (!CRC32.FileCRC(this->m_MainInfo.PluginName, &PluginCRC32, 1024))
	{
		MessageBox(NULL, "Failed on reading the plugin name crc!", "Error", MB_OK | MB_ICONERROR);

		ExitProcess(0);
	}

	if (this->m_MainInfo.PluginCRC32 != PluginCRC32)
	{
		MessageBox(NULL, "Plugin file CRC doesn't match!", "Error", MB_OK | MB_ICONERROR);

		ExitProcess(0);
	}

	HMODULE module = GetModuleHandle(this->m_MainInfo.PluginName);

	if (module)
	{
		MessageBox(NULL, "Plugin file already loaded!", "Error", MB_OK | MB_ICONERROR);

		ExitProcess(0);
	}

	module = LoadLibrary(this->m_MainInfo.PluginName);

	if (module == NULL)
	{
		MessageBox(NULL, "Failed loading the plugin!", "Error", MB_OK | MB_ICONERROR);

		ExitProcess(0);
	}

	char filename[MAX_PATH];

	GetModuleFileName(module, filename, MAX_PATH);

	if (!CRC32.FileCRC(filename, &PluginCRC32, 1024))
	{
		MessageBox(NULL, "Failed on reading the plugin name crc!", "Error", MB_OK | MB_ICONERROR);

		ExitProcess(0);
	}

	if (this->m_MainInfo.PluginCRC32 != PluginCRC32)
	{
		MessageBox(NULL, "Plugin file CRC doesn't match!", "Error", MB_OK | MB_ICONERROR);

		ExitProcess(0);
	}

	void(*EntryProc)() = (void(*)())GetProcAddress(module, "EntryProc");

	if (EntryProc != 0)
	{
		EntryProc();
	}
}