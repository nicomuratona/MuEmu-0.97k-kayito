#pragma once

struct MESSAGE_INFO
{
	int Index;
	char Message[128];
};

class CMessage
{
public:

	CMessage();

	~CMessage();

	void Load(char* path, int lang);

	char* GetTextMessage(int index, int lang);

private:

	char m_DefaultMessage[128];

	std::map<int, MESSAGE_INFO> m_MessageInfo[MAX_LANGUAGE];
};

extern CMessage gMessage;