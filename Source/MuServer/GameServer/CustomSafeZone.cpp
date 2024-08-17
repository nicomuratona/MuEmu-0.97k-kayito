#include "stdafx.h"
#include "CustomSafeZone.h"
#include "ReadScript.h"
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
	CReadScript* lpReadScript = new CReadScript;

	if (lpReadScript == NULL)
	{
		ErrorMessageBox(READ_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (!lpReadScript->Load(path))
	{
		ErrorMessageBox(READ_SCRIPT_FILE_ERROR, path);

		delete lpReadScript;

		return;
	}

	this->m_SafeZoneInfo.clear();

	try
	{
		eTokenResult token;

		while (true)
		{
			token = lpReadScript->GetToken();

			if (token == TOKEN_END || token == TOKEN_END_SECTION)
			{
				break;
			}

			SAFE_ZONE_INFO info;

			info.Map = lpReadScript->GetNumber();

			info.X = lpReadScript->GetAsNumber();

			info.Y = lpReadScript->GetAsNumber();

			info.TX = lpReadScript->GetAsNumber();

			info.TY = lpReadScript->GetAsNumber();

			this->m_SafeZoneInfo.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript->GetError());
	}

	delete lpReadScript;
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