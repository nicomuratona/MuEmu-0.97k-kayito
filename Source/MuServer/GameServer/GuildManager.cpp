#include "stdafx.h"
#include "GuildManager.h"

CGuildManager gGuildManager;

CGuildManager::CGuildManager()
{
	this->vGuildList.clear();
}

CGuildManager::~CGuildManager()
{
	this->AllDelete();
}

void CGuildManager::AllDelete()
{
	for (auto guildPtr : this->vGuildList)
	{
		delete guildPtr;
	}

	this->vGuildList.clear();
}

GUILD_INFO* CGuildManager::SearchGuild(char* GuildName)
{
	for (std::vector<GUILD_INFO*>::iterator it = this->vGuildList.begin(); it != this->vGuildList.end(); it++)
	{
		if (strcmp((*it)->Name, GuildName) == 0)
		{
			return (*it);
		}
	}

	return NULL;
}

GUILD_INFO* CGuildManager::AddGuild(int GuildNumber, char* GuildName, BYTE* Mark, char* GuildMaster, int score)
{
	GUILD_INFO* pSearchGuild = this->SearchGuild(GuildName);

	if (pSearchGuild != NULL)
	{
		return pSearchGuild;
	}

	GUILD_INFO* GuildInfo = new GUILD_INFO();

	strcpy_s(GuildInfo->Name, GuildName);

	if (Mark != 0)
	{
		memcpy(GuildInfo->Mark, Mark, sizeof(GuildInfo->Mark));
	}

	GuildInfo->Number = GuildNumber;

	GuildInfo->GuildScore = score;

	GuildInfo->WarDeclareState = 0;

	GuildInfo->WarState = GUILD_WAR_STATE_NONE;

	memset(GuildInfo->Notice, 0, sizeof(GuildInfo->Notice));

	GUILD_MEMBER_INFO GuildMemberInfo;

	GuildMemberInfo.Clear();

	strcpy_s(GuildMemberInfo.Name, GuildMaster);

	GuildInfo->Members[0] = GuildMemberInfo;

	this->vGuildList.push_back(GuildInfo);

	return this->vGuildList.back();
}

bool CGuildManager::DeleteGuild(char* GuildName)
{
	for (std::vector<GUILD_INFO*>::iterator it = this->vGuildList.begin(); it != this->vGuildList.end();)
	{
		if (strcmp((*it)->Name, GuildName) == 0)
		{
			delete (*it);

			this->vGuildList.erase(it);

			return true;
		}
		else
		{
			it++;
		}
	}

	return false;
}

GUILD_INFO* CGuildManager::AddMember(GUILD_INFO* lpGuild, char* Name, int aIndex, int status, int server)
{
	int blank = -1;

	if (lpGuild == 0)
	{
		return 0;
	}

	if (strcmp(Name, lpGuild->Members[0].Name) == 0)
	{
		blank = 0;
	}
	else
	{
		for (int i = 1; i < MAX_GUILD_MEMBER; i++)
		{
			if (lpGuild->Members[i].Use != false)
			{
				if (strcmp(lpGuild->Members[i].Name, Name) == 0)
				{
					if (aIndex != -1)
					{
						lpGuild->Members[i].Index = aIndex;

						lpGuild->Members[i].Server = server;

						lpGuild->Members[i].GuildStatus = GUILD_STATUS_MEMBER;
					}

					return lpGuild;
				}
			}
			else if (blank < 0)
			{
				blank = i;
			}
		}
	}

	if (blank < 0)
	{
		return 0;
	}

	lpGuild->Members[blank].Clear();

	strcpy_s(lpGuild->Members[blank].Name, Name);

	lpGuild->Members[blank].Use = true;

	lpGuild->Members[blank].Server = server;

	lpGuild->Members[blank].GuildStatus = status;

	if (aIndex != -1)
	{
		lpGuild->Members[blank].Index = aIndex;
	}

	return lpGuild;
}

GUILD_INFO* CGuildManager::AddMember(char* GuildName, char* Name, int aIndex, int status, int server)
{
	GUILD_INFO* pNode = this->SearchGuild(GuildName);

	int blank = -1;

	if (pNode == 0)
	{
		return 0;
	}

	if (strcmp(Name, pNode->Members[0].Name) == 0)
	{
		blank = 0;
	}
	else
	{
		for (int i = 1; i < MAX_GUILD_MEMBER; i++)
		{
			if (pNode->Members[i].Use != false)
			{
				if (strcmp(pNode->Members[i].Name, Name) == 0)
				{
					if (aIndex != -1)
					{
						pNode->Members[i].Index = aIndex;

						pNode->Members[i].Server = server;

						pNode->Members[i].GuildStatus = GUILD_STATUS_MEMBER;
					}

					return pNode;
				}
			}
			else if (blank < 0)
			{
				blank = i;
			}
		}
	}

	if (blank < 0)
	{
		return 0;
	}
	else if (blank > 0)
	{
		pNode->Members[blank].Clear();
	}

	strcpy_s(pNode->Members[blank].Name, Name);

	pNode->Members[blank].Use = true;

	pNode->Members[blank].Server = server;

	pNode->Members[blank].GuildStatus = status;

	if (aIndex != -1)
	{
		pNode->Members[blank].Index = aIndex;
	}

	return pNode;
}

bool CGuildManager::DelMember(char* GuildName, char* Name)
{
	GUILD_INFO* lpGuild = this->SearchGuild(GuildName);

	if (lpGuild == 0)
	{
		return 0;
	}

	for (int n = 0; n < MAX_GUILD_MEMBER; n++)
	{
		if (lpGuild->Members[n].Use != false)
		{
			if (strcmp(lpGuild->Members[n].Name, Name) == 0)
			{
				lpGuild->Members[n].Clear();

				return true;
			}
		}
	}

	return false;
}

bool CGuildManager::ConnectUser(GUILD_INFO* lpGuild, char* GuildName, char* Name, int aIndex, int status, int server)
{
	if (lpGuild == 0)
	{
		return 0;
	}

	if (strcmp(GuildName, lpGuild->Name) != 0)
	{
		return 0;
	}

	for (int n = 0; n < MAX_GUILD_MEMBER; n++)
	{
		if (lpGuild->Members[n].Use != false)
		{
			if (strcmp(lpGuild->Members[n].Name, Name) == 0)
			{
				lpGuild->Members[n].Index = aIndex;

				lpGuild->Members[n].Server = server;

				if (n == 0)
				{
					lpGuild->PlayScore = 0;

					lpGuild->WarDeclareState = 0;

					lpGuild->WarState = GUILD_WAR_STATE_NONE;
				}

				return true;
			}
		}
	}

	this->AddMember(lpGuild, Name, aIndex, status, server);

	return false;
}

bool CGuildManager::CloseMember(char* GuildName, char* Name)
{
	GUILD_INFO* lpGuild = this->SearchGuild(GuildName);

	if (lpGuild == 0)
	{
		return 0;
	}

	for (int n = 0; n < MAX_GUILD_MEMBER; n++)
	{
		if (lpGuild->Members[n].Use != false)
		{
			if (strcmp(lpGuild->Members[n].Name, Name) == 0)
			{
				lpGuild->Members[n].Index = -1;

				lpGuild->Members[n].Server = -1;

				return true;
			}
		}
	}

	return false;
}

bool CGuildManager::SetServer(char* GuildName, char* Name, int server)
{
	GUILD_INFO* lpGuild = this->SearchGuild(GuildName);

	if (lpGuild == 0)
	{
		return 0;
	}

	for (int n = 0; n < MAX_GUILD_MEMBER; n++)
	{
		if (lpGuild->Members[n].Use != false)
		{
			if (strcmp(lpGuild->Members[n].Name, Name) == 0)
			{
				lpGuild->Members[n].Server = server;

				return true;
			}
		}
	}

	return false;
}