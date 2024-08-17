#include "stdafx.h"
#include "AllowableIpList.h"
#include "ReadScript.h"
#include "Util.h"

CAllowableIpList gAllowableIpList;

CAllowableIpList::CAllowableIpList()
{
	this->m_AllowableIpInfo.clear();
}

CAllowableIpList::~CAllowableIpList()
{

}

void CAllowableIpList::Load(char* path)
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

	this->m_AllowableIpInfo.clear();

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

			int section = lpReadScript->GetNumber();

			while (true)
			{
				token = lpReadScript->GetToken();

				if (token == TOKEN_END || token == TOKEN_END_SECTION)
				{
					break;
				}

				if (section == 0)
				{
					ALLOWABLE_IP_INFO info;

					strcpy_s(info.IpAddr, lpReadScript->GetString());

					this->m_AllowableIpInfo.insert(std::pair<std::string, ALLOWABLE_IP_INFO>(std::string(info.IpAddr), info));
				}
				else
				{
					break;
				}
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript->GetError());
	}

	delete lpReadScript;
}

bool CAllowableIpList::CheckAllowableIp(char* ip)
{
	std::map<std::string, ALLOWABLE_IP_INFO>::iterator it = this->m_AllowableIpInfo.find(std::string(ip));

	if (it == this->m_AllowableIpInfo.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}