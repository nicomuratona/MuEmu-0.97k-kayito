#pragma once

#define MAX_GUILD_MEMBER 40

struct GUILD_MEMBER_INFO
{
	void Clear()
	{
		memset(this->szGuildMember, 0, sizeof(this->szGuildMember));

		this->btServer = 0xFFFF;

		this->btStatus = 0;
	}

	bool IsEmpty()
	{
		return ((this->szGuildMember[0] == 0) ? 1 : 0);
	}

	char szGuildMember[11];
	BYTE btStatus;
	WORD btServer;
};

struct GUILD_INFO
{
	GUILD_INFO()
	{
		this->Clear();
	}

	void Clear()
	{
		this->dwNumber = 0;

		this->dwScore = 0;

		memset(this->szName, 0, sizeof(this->szName));

		memset(this->szMaster, 0, sizeof(this->szMaster));

		memset(this->szNotice, 0, sizeof(this->szNotice));

		memset(this->arMark, 0, sizeof(this->arMark));

		for (int n = 0; n < MAX_GUILD_MEMBER; n++)
		{
			this->arGuildMember[n].Clear();
		}
	}

	BYTE GetMemberCount()
	{
		BYTE count = 0;

		for (int n = 0; n < MAX_GUILD_MEMBER; n++)
		{
			if (this->arGuildMember[n].IsEmpty() == false)
			{
				count++;
			}
		}

		return count;
	}

	DWORD dwNumber;
	DWORD dwScore;
	char szName[9];
	char szMaster[11];
	char szNotice[60];
	BYTE arMark[32];
	GUILD_MEMBER_INFO arGuildMember[MAX_GUILD_MEMBER];
};

class CGuildManager
{
public:

	CGuildManager();

	~CGuildManager();

	void AllDelete();

	void Init();

	GUILD_INFO* GetGuildInfo(char* szName);

	GUILD_INFO* GetGuildInfo(DWORD dwNumber);

	GUILD_INFO* GetMemberGuildInfo(char* szGuildMember);

	GUILD_MEMBER_INFO* GetGuildMemberInfo(char* szGuildMember);

	void ConnectMember(char* szGuildMember, WORD btServer);

	void DisconnectMember(char* szGuildMember);

	BYTE AddGuild(int index, char* szGuildName, char* szMasterName, BYTE* lpMark);

	BYTE DelGuild(int index, char* szGuildName);

	BYTE AddGuildMember(int index, char* szGuildName, char* szGuildMember, BYTE btStatus, WORD btServer);

	BYTE DelGuildMember(int index, char* szGuildMember);

	BYTE SetGuildScore(char* szGuildName, DWORD dwScore);

	BYTE SetGuildNotice(char* szGuildName, char* szNotice);

private:

	std::vector<GUILD_INFO*> vGuildList;
};

extern CGuildManager gGuildManager;