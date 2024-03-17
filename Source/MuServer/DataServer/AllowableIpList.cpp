#include "stdafx.h"
#include "AllowableIpList.h"
#include "MemScript.h"
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

	this->m_AllowableIpInfo.clear();

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

			int section = lpMemScript->GetNumber();

			while (true)
			{
				token = lpMemScript->GetToken();

				if (token == TOKEN_END || token == TOKEN_END_SECTION)
				{
					break;
				}

				if (section == 0)
				{
					ALLOWABLE_IP_INFO info;

					strcpy_s(info.IpAddr, lpMemScript->GetString());

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
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
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