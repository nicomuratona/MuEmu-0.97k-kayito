#include "stdafx.h"
#include "IpManager.h"
#include "ServerInfo.h"

CIpManager gIpManager;

CIpManager::CIpManager()
{

}

CIpManager::~CIpManager()
{

}

bool CIpManager::CheckIpAddress(char* IpAddress)
{
	std::map<std::string, IP_ADDRESS_INFO>::iterator it = this->m_IpAddressInfo.find(std::string(IpAddress));

	if (it == this->m_IpAddressInfo.end())
	{
		return ((gServerInfo.m_MaxIpConnection == 0) ? 0 : 1);
	}
	else
	{
		return ((it->second.IpAddressCount >= gServerInfo.m_MaxIpConnection) ? 0 : 1);
	}
}

void CIpManager::InsertIpAddress(char* IpAddress)
{
	IP_ADDRESS_INFO info;

	strcpy_s(info.IpAddress, IpAddress);

	info.IpAddressCount = 1;

	std::map<std::string, IP_ADDRESS_INFO>::iterator it = this->m_IpAddressInfo.find(std::string(IpAddress));

	if (it == this->m_IpAddressInfo.end())
	{
		this->m_IpAddressInfo.insert(std::pair<std::string, IP_ADDRESS_INFO>(std::string(IpAddress), info));
	}
	else
	{
		it->second.IpAddressCount++;
	}
}

void CIpManager::RemoveIpAddress(char* IpAddress)
{
	std::map<std::string, IP_ADDRESS_INFO>::iterator it = this->m_IpAddressInfo.find(std::string(IpAddress));

	if (it != this->m_IpAddressInfo.end())
	{
		if ((--it->second.IpAddressCount) == 0)
		{
			this->m_IpAddressInfo.erase(it);
		}
	}
}