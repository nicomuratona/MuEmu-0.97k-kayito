#include "stdafx.h"
#include "CustomSafeZone.h"
#include "MemScript.h"
#include "Util.h"

CSafeZone gSafeZone;

CSafeZone::CSafeZone()
{
	this->m_SafeZoneInfo.clear();
}

CSafeZone::~CSafeZone()
{

}

void CSafeZone::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());

		delete lpMemScript;

		return;
	}

	this->m_SafeZoneInfo.clear();

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if (strcmp("end", lpMemScript->GetString()) == 0)
			{
				break;
			}

			SAFE_ZONE_INFO info;

			info.Map = lpMemScript->GetNumber();

			info.X = lpMemScript->GetAsNumber();

			info.Y = lpMemScript->GetAsNumber();

			info.TX = lpMemScript->GetAsNumber();

			info.TY = lpMemScript->GetAsNumber();

			this->m_SafeZoneInfo.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CSafeZone::CheckSafeZone(int map, int x, int y)
{
	for (std::vector<SAFE_ZONE_INFO>::iterator it = this->m_SafeZoneInfo.begin(); it != this->m_SafeZoneInfo.end(); it++)
	{
		if (it->Map != map)
		{
			continue;
		}

		if ((it->X > x || it->TX < x) || (it->Y > y || it->TY < y))
		{
			continue;
		}
		else
		{
			return 1;
		}
	}

	return 0;
}