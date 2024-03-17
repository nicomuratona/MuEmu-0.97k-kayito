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

	if (lpMemScript == NULL)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (!lpMemScript->SetBuffer(path))
	{
		ErrorMessageBox(lpMemScript->GetLastError());

		delete lpMemScript;

		return;
	}

	this->m_SafeZoneInfo.clear();

	try
	{
		eTokenResult token;

		while (true)
		{
			token = lpMemScript->GetToken();

			if (token == TOKEN_END || token == TOKEN_END_SECTION)
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