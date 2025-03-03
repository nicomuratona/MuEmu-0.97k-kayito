#include "stdafx.h"
#include "MapFog.h"

CMapFog gMapFog;

CMapFog::CMapFog()
{
	this->Init();
}

CMapFog::~CMapFog()
{

}

void CMapFog::Init()
{
	for (int i = 0; i < MAX_MAPS; i++)
	{
		this->m_MapFog[i].MapNumber = -1;
	}
}

void CMapFog::Load(char* path)
{
	CReadScript* lpReadScript = new CReadScript;

	if (lpReadScript == NULL)
	{
		printf(READ_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (!lpReadScript->Load(path))
	{
		printf(READ_SCRIPT_FILE_ERROR, path);

		delete lpReadScript;

		return;
	}

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

			MAP_FOG_INFO info;

			info.MapNumber = lpReadScript->GetNumber();

			info.DayRGB[0] = lpReadScript->GetAsNumber();

			info.DayRGB[1] = lpReadScript->GetAsNumber();

			info.DayRGB[2] = lpReadScript->GetAsNumber();

			info.NightRGB[0] = lpReadScript->GetAsNumber();

			info.NightRGB[1] = lpReadScript->GetAsNumber();

			info.NightRGB[2] = lpReadScript->GetAsNumber();

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		printf(lpReadScript->GetError());
	}

	delete lpReadScript;
}

void CMapFog::SetInfo(MAP_FOG_INFO info)
{
	if (info.MapNumber < 0 || info.MapNumber >= MAX_MAPS)
	{
		return;
	}

	this->m_MapFog[info.MapNumber] = info;
}