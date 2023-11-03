#include "stdafx.h"
#include "Blacklist.h"
#include "MemScript.h"
#include "Util.h"

CBlackList gBlackList;

CBlackList::CBlackList()
{
	this->m_BlacklistIP.clear();

	this->m_BlacklistHWID.clear();
}

CBlackList::~CBlackList()
{

}

void CBlackList::Load(char* path)
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

	this->m_BlacklistIP.clear();

	this->m_BlacklistHWID.clear();

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while (true)
			{
				if (section == 0)
				{
					if (strcmp("end", lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					BLACK_LIST_IP_INFO info;

					memcpy(info.IpAddr, lpMemScript->GetString(), sizeof(info.IpAddr));

					this->m_BlacklistIP.push_back(info);
				}
				else if (section == 1)
				{
					if (strcmp("end", lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					BLACK_LIST_HWID_INFO info;

					memcpy(info.HardwareId, lpMemScript->GetString(), sizeof(info.HardwareId));

					this->m_BlacklistHWID.push_back(info);
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

int CBlackList::CheckIpAddress(char* Ip)
{
	for (std::vector<BLACK_LIST_IP_INFO>::iterator it = this->m_BlacklistIP.begin(); it != this->m_BlacklistIP.end(); it++)
	{
		if (_strcmpi(it->IpAddr, Ip) != 0)
		{
			continue;
		}

		if (it == this->m_BlacklistIP.end())
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}

	return 0;
}

int CBlackList::CheckHardwareID(char* Hwid)
{
	if (strlen(Hwid) == 0)
	{
		return 1;
	}

	for (std::vector<BLACK_LIST_HWID_INFO>::iterator it = this->m_BlacklistHWID.begin(); it != this->m_BlacklistHWID.end(); it++)
	{
		if (_strcmpi(it->HardwareId, Hwid) != 0)
		{
			continue;
		}

		if (it == this->m_BlacklistHWID.end())
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}

	return 0;
}