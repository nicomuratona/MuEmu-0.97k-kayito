#pragma once

#define MAX_GUILD_MEMBER 40

enum eGuildStatus
{
	GUILD_STATUS_NONE = -1,
	GUILD_STATUS_MEMBER = 0,
	GUILD_STATUS_MASTER = 128,
};

enum eGuildWarState
{
	GUILD_WAR_STATE_NONE = 0,
	GUILD_WAR_STATE_DECLARE = 1,
	GUILD_WAR_STATE_ACTIVE = 2,
};

enum eGuildWarType
{
	GUILD_WAR_TYPE_NORMAL = 0,
	GUILD_WAR_TYPE_SOCCER = 1,
};

struct GUILD_MEMBER_INFO
{
	void Clear()
	{
		memset(this->Name, 0, sizeof(this->Name));
		this->Index = -1;
		this->Use = false;
		this->Server = -1;
		this->GuildStatus = GUILD_STATUS_NONE;
	}

	char Name[11];
	short Index;
	bool Use;
	char Server;
	BYTE GuildStatus;
};

struct GUILD_INFO
{
	GUILD_INFO()
	{
		this->Clear();
	}

	void Clear()
	{
		this->Number = 0;
		this->TargetGuildNode = NULL;
		this->WarDeclareState = 0;
		this->WarState = GUILD_WAR_STATE_NONE;
		this->WarType = GUILD_WAR_TYPE_NORMAL;
		this->BattleGroundIndex = 0;
		this->BattleTeamCode = 0;
		this->PlayScore = 0;
		this->GuildScore = 0;

		memset(this->Name, 0, sizeof(this->Name));

		memset(this->Mark, 0, sizeof(this->Mark));

		memset(this->Notice, 0, sizeof(this->Notice));

		for (int n = 0; n < MAX_GUILD_MEMBER; n++)
		{
			this->Members[n].Clear();
		}
	}

	BYTE GetMemberCount()
	{
		BYTE count = 0;

		for (int n = 0; n < MAX_GUILD_MEMBER; n++)
		{
			if (this->Members[n].Use != false)
			{
				count++;
			}
		}

		return count;
	}

	int Number;
	char Name[9];
	BYTE Mark[32];
	int GuildScore;
	char Notice[60];
	GUILD_INFO* TargetGuildNode;
	BYTE WarDeclareState;
	BYTE WarState;
	BYTE WarType;
	BYTE BattleGroundIndex;
	BYTE BattleTeamCode;
	BYTE PlayScore;
	GUILD_MEMBER_INFO Members[MAX_GUILD_MEMBER];
};

class CGuildManager
{
public:

	CGuildManager();

	~CGuildManager();

	void AllDelete();

	GUILD_INFO* SearchGuild(char* GuildName);

	GUILD_INFO* AddGuild(int GuildNumber, char* GuildName, BYTE* Mark, char* GuildMaster, int score);

	bool DeleteGuild(char* GuildName);

	GUILD_INFO* AddMember(GUILD_INFO* lpGuild, char* Name, int aIndex, int status, int server);

	GUILD_INFO* AddMember(char* GuildName, char* Name, int aIndex, int status, int server);

	bool DelMember(char* GuildName, char* Name);

	bool ConnectUser(GUILD_INFO* lpGuild, char* GuildName, char* Name, int aIndex, int status, int server);

	bool CloseMember(char* GuildName, char* Name);

	bool SetServer(char* GuildName, char* Name, int server);

private:

	std::vector<GUILD_INFO*> vGuildList;
};

extern CGuildManager gGuildManager;