#pragma once

class CBlackList
{
public:
	CBlackList()  = default;
	~CBlackList() = default;

	void Load(const char* path);

	bool CheckIpAddress(const char* Ip);
	bool CheckHardwareID(const char* Hwid);

private:
	std::vector<std::string> m_BlacklistIP;
	std::vector<std::string> m_BlacklistHWID;
};

extern CBlackList gBlackList;
