#include "stdafx.h"
#include "CharacterManager.h"
#include "ServerManager.h"
#include "Util.h"

int gServerCount = 0;

void ErrorMessageBox(char* message, ...)
{
	char buff[256];

	memset(buff, 0, sizeof(buff));

	va_list arg;

	va_start(arg, message);

	vsprintf_s(buff, message, arg);

	va_end(arg);

	MessageBox(0, buff, "Error", MB_OK | MB_ICONERROR);

	ExitProcess(0);
}

void LogAdd(eLogColor color, char* text, ...)
{
	SYSTEMTIME now;

	GetLocalTime(&now);

	char time[10];

	wsprintf(time, "%02d:%02d:%02d", now.wHour, now.wMinute, now.wSecond);

	char temp[1024];

	va_list arg;

	va_start(arg, text);

	vsprintf_s(temp, text, arg);

	va_end(arg);

	char log[1024];

	wsprintf(log, "[%s] %s", time, temp);

	gServerDisplayer.LogAddText(color, log, strlen(log));
}

bool GetCharacterSlot(char CharacterName[5][11], char* name, BYTE* slot)
{
	for (int n = 0; n < 5; n++)
	{
		if (_stricmp(CharacterName[n], name) == 0)
		{
			(*slot) = n;

			return true;
		}
	}

	return false;
}

bool CheckTextSyntax(char* text, int size)
{
	for (int n = 0; n < size; n++)
	{
		if (text[n] == 0x20 || text[n] == 0x22 || text[n] == 0x27)
		{
			return false;
		}
	}

	return true;
}

bool CheckSpecialText(char* Text)
{
	for (unsigned char* lpszCheck = (unsigned char*)Text; *lpszCheck; ++lpszCheck)
	{
		if (_mbclen(lpszCheck) == 1) // One byte
		{
			//if ( *lpszCheck < 0x21 || *lpszCheck > 0x7E)
			if (*lpszCheck < 48 || (58 <= *lpszCheck && *lpszCheck < 65) || (91 <= *lpszCheck && *lpszCheck < 97) || *lpszCheck > 122)
			{
				return false;
			}
		}
		else // Two bytes
		{
			unsigned char* lpszTrail = lpszCheck + 1;

			if (0x81 <= *lpszCheck && *lpszCheck <= 0xC8) // Korean
			{
				if ((0x41 <= *lpszTrail && *lpszTrail <= 0x5A)
				    || (0x61 <= *lpszTrail && *lpszTrail <= 0x7A)
				    || (0x81 <= *lpszTrail && *lpszTrail <= 0xFE))
				{ // Excluding transparent characters
					// Areas of special characters that are not allowed
					if (0xA1 <= *lpszCheck && *lpszCheck <= 0xAF && 0xA1 <= *lpszTrail)
					{
						return false;
					}
					else if (*lpszCheck == 0xC6 && 0x53 <= *lpszTrail && *lpszTrail <= 0xA0)
					{
						return false;
					}
					else if (0xC7 <= *lpszCheck && *lpszCheck <= 0xC8 && *lpszTrail <= 0xA0)
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}

			++lpszCheck;
		}
	}

	return true;
}

int GetFreeServerIndex()
{
	int index = -1;

	int count = gServerCount;

	if (SearchFreeServerIndex(&index, 0, MAX_SERVER, 10000) != 0)
	{
		return index;
	}

	for (int n = 0; n < MAX_SERVER; n++)
	{
		if (gServerManager[count].CheckState() == false)
		{
			return count;
		}
		else
		{
			count = (((++count) >= MAX_SERVER) ? 0 : count);
		}
	}

	return -1;
}

int SearchFreeServerIndex(int* index, int MinIndex, int MaxIndex, DWORD MinTime)
{
	DWORD CurOnlineTime = 0;

	DWORD MaxOnlineTime = 0;

	for (int n = MinIndex; n < MaxIndex; n++)
	{
		if (gServerManager[n].CheckState() == false && gServerManager[n].CheckAlloc() != false)
		{
			if ((CurOnlineTime = (GetTickCount() - gServerManager[n].m_OnlineTime)) > MinTime && CurOnlineTime > MaxOnlineTime)
			{
				(*index) = n;

				MaxOnlineTime = CurOnlineTime;
			}
		}
	}

	return (((*index) == -1) ? 0 : 1);
}

WORD GetServerCodeByName(char* name)
{
	CHARACTER_INFO CharacterInfo;

	if (gCharacterManager.GetCharacterInfo(&CharacterInfo, name) == false)
	{
		return 0xFFFF;
	}
	else
	{
		return CharacterInfo.GameServerCode;
	}
}

CServerManager* FindServerByCode(WORD ServerCode)
{
	for (int n = 0; n < MAX_SERVER; n++)
	{
		if (gServerManager[n].CheckState() != false && gServerManager[n].m_ServerCode == ServerCode)
		{
			return &gServerManager[n];
		}
	}

	return 0;
}