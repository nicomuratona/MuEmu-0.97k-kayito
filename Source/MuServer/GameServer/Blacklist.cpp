#include "stdafx.h"
#include "Blacklist.h"
#include "ReadScript.h"
#include "Util.h"

CBlackList gBlackList;

void CBlackList::Load(const char* path)
{
	CReadScript lpReadScript;

	if (!lpReadScript.Load(path))
	{
		ErrorMessageBox(READ_SCRIPT_FILE_ERROR, path);
		return;
	}

	m_BlacklistIP.clear();
	m_BlacklistHWID.clear();

	try
	{
		eTokenResult token;

		while (true)
		{
			token = lpReadScript.GetToken();

			if (token == TOKEN_END || token == TOKEN_END_SECTION)
				break;

			int section = lpReadScript.GetNumber();

			while (true)
			{
				token = lpReadScript.GetToken();

				if (token == TOKEN_END || token == TOKEN_END_SECTION)
					break;

				if (section == 0)
				{
					m_BlacklistIP.push_back(lpReadScript.GetString());
				}
				else if (section == 1)
				{
					m_BlacklistHWID.push_back(lpReadScript.GetString());
				}
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript.GetError());
	}
}

bool CBlackList::CheckIpAddress(const char* Ip)
{
	return std::find(m_BlacklistIP.begin(), m_BlacklistIP.end(), Ip) != m_BlacklistIP.end();
}

bool CBlackList::CheckHardwareID(const char* Hwid)
{
	return std::find(m_BlacklistHWID.begin(), m_BlacklistHWID.end(), Hwid) != m_BlacklistHWID.end();
}
