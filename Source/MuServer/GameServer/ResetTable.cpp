#include "stdafx.h"
#include "ResetTable.h"
#include "ReadScript.h"
#include "ServerInfo.h"
#include "Util.h"

CResetTable gResetTable;

CResetTable::CResetTable()
{
	this->m_ResetTableInfo.clear();
}

CResetTable::~CResetTable()
{

}

void CResetTable::Load(char* path)
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

	this->m_ResetTableInfo.clear();

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

			RESET_TABLE_INFO info;

			info.MinReset = lpReadScript->GetNumber();

			info.MaxReset = lpReadScript->GetAsNumber();

			info.Level[0] = lpReadScript->GetAsNumber();

			info.Level[1] = lpReadScript->GetAsNumber();

			info.Level[2] = lpReadScript->GetAsNumber();

			info.Level[3] = lpReadScript->GetAsNumber();

			info.Money[0] = lpReadScript->GetAsNumber();

			info.Money[1] = lpReadScript->GetAsNumber();

			info.Money[2] = lpReadScript->GetAsNumber();

			info.Money[3] = lpReadScript->GetAsNumber();

			info.Point[0] = lpReadScript->GetAsNumber();

			info.Point[1] = lpReadScript->GetAsNumber();

			info.Point[2] = lpReadScript->GetAsNumber();

			info.Point[3] = lpReadScript->GetAsNumber();

			this->m_ResetTableInfo.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript->GetError());
	}

	delete lpReadScript;
}

int CResetTable::GetResetLevel(LPOBJ lpObj)
{
	for (std::vector<RESET_TABLE_INFO>::iterator it = this->m_ResetTableInfo.begin(); it != this->m_ResetTableInfo.end(); it++)
	{
		if ((lpObj->Reset + 1) >= it->MinReset && (lpObj->Reset + 1) <= it->MaxReset)
		{
			if (it->Level[lpObj->AccountLevel] == -1)
			{
				return gServerInfo.m_CommandResetLevel[lpObj->AccountLevel];
			}
			else
			{
				return it->Level[lpObj->AccountLevel];
			}
		}
	}

	return gServerInfo.m_CommandResetLevel[lpObj->AccountLevel];
}

int CResetTable::GetResetMoney(LPOBJ lpObj)
{
	for (std::vector<RESET_TABLE_INFO>::iterator it = this->m_ResetTableInfo.begin(); it != this->m_ResetTableInfo.end(); it++)
	{
		if ((lpObj->Reset + 1) >= it->MinReset && (lpObj->Reset + 1) <= it->MaxReset)
		{
			if (it->Money[lpObj->AccountLevel] == -1)
			{
				return gServerInfo.m_CommandResetMoney[lpObj->AccountLevel];
			}
			else
			{
				return it->Money[lpObj->AccountLevel];
			}
		}
	}

	return gServerInfo.m_CommandResetMoney[lpObj->AccountLevel];
}

int CResetTable::GetResetPoint(LPOBJ lpObj)
{
	int point = 0;

	for (int n = 1; n <= lpObj->Reset; n++)
	{
		int AddPoint = gServerInfo.m_CommandResetPoint[lpObj->AccountLevel];

		for (std::vector<RESET_TABLE_INFO>::iterator it = this->m_ResetTableInfo.begin(); it != this->m_ResetTableInfo.end(); it++)
		{
			if (n >= it->MinReset && n <= it->MaxReset)
			{
				if (it->Point[lpObj->AccountLevel] == -1)
				{
					AddPoint = gServerInfo.m_CommandResetPoint[lpObj->AccountLevel];

					break;
				}
				else
				{
					AddPoint = it->Point[lpObj->AccountLevel];

					break;
				}
			}
		}

		point += AddPoint;
	}

	return point;
}