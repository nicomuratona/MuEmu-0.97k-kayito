#pragma once

struct BLACK_LIST_IP_INFO
{
	char IpAddr[16];
};

struct BLACK_LIST_HWID_INFO
{
	char HardwareId[36];
};

class CBlackList
{
public:

	CBlackList();

	~CBlackList();

	void Load(char* path);

	int CheckIpAddress(char* Ip);

	int CheckHardwareID(char* Hwid);

private:

	std::vector<BLACK_LIST_IP_INFO> m_BlacklistIP;

	std::vector<BLACK_LIST_HWID_INFO> m_BlacklistHWID;
};

extern CBlackList gBlackList;