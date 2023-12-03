#include "stdafx.h"
#include "GuildManager.h"
#include "Guild.h"
#include "QueryManager.h"
#include "Util.h"

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
	for (std::vector<GUILD_INFO*>::iterator it = this->vGuildList.begin(); it != this->vGuildList.end(); it++)
	{
		delete (*it);
	}

	this->vGuildList.clear();
}

void CGuildManager::Init()
{
	this->vGuildList.clear();

	if (gQueryManager.ExecQuery("SELECT * FROM Guild") != false)
	{
		GUILD_INFO* GuildInfo;

		while (gQueryManager.Fetch() != SQL_NO_DATA)
		{
			GuildInfo = new GUILD_INFO();

			gQueryManager.GetAsString("G_Name", GuildInfo->szName, sizeof(GuildInfo->szName));

			gQueryManager.GetAsBinary("G_Mark", GuildInfo->arMark, sizeof(GuildInfo->arMark));

			GuildInfo->dwScore = gQueryManager.GetAsInteger("G_Score");

			gQueryManager.GetAsString("G_Master", GuildInfo->szMaster, sizeof(GuildInfo->szMaster));

			gQueryManager.GetAsString("G_Notice", GuildInfo->szNotice, sizeof(GuildInfo->szNotice));

			GuildInfo->dwNumber = gQueryManager.GetAsInteger("Number");

			this->vGuildList.push_back(GuildInfo);
		}
	}

	gQueryManager.Close();

	if (gQueryManager.ExecQuery("SELECT * FROM GuildMember") != false)
	{
		while (gQueryManager.Fetch() != SQL_NO_DATA)
		{
			char GuildName[9] = { 0 };

			GUILD_MEMBER_INFO GuildMemberInfo;

			GuildMemberInfo.Clear();

			gQueryManager.GetAsString("Name", GuildMemberInfo.szGuildMember, sizeof(GuildMemberInfo.szGuildMember));

			gQueryManager.GetAsString("G_Name", GuildName, sizeof(GuildName));

			GuildMemberInfo.btStatus = gQueryManager.GetAsInteger("G_Status");

			GUILD_INFO* lpGuildInfo = this->GetGuildInfo(GuildName);

			if (lpGuildInfo != 0)
			{
				if (_stricmp(lpGuildInfo->szMaster, GuildMemberInfo.szGuildMember) == 0)
				{
					lpGuildInfo->arGuildMember[0] = GuildMemberInfo;
				}
				else
				{
					for (int n = 1; n < MAX_GUILD_MEMBER; n++)
					{
						if (lpGuildInfo->arGuildMember[n].IsEmpty() != false)
						{
							lpGuildInfo->arGuildMember[n] = GuildMemberInfo;

							break;
						}
					}
				}
			}
		}
	}

	gQueryManager.Close();
}

GUILD_INFO* CGuildManager::GetGuildInfo(char* szName)
{
	for (std::vector<GUILD_INFO*>::iterator it = this->vGuildList.begin(); it != this->vGuildList.end(); it++)
	{
		if ((*it)->szName[0] == szName[0])
		{
			if (_stricmp((*it)->szName, szName) == 0)
			{
				return (*it);
			}
		}
	}

	return 0;
}

GUILD_INFO* CGuildManager::GetGuildInfo(DWORD dwNumber)
{
	for (std::vector<GUILD_INFO*>::iterator it = this->vGuildList.begin(); it != this->vGuildList.end(); it++)
	{
		if ((*it)->dwNumber == dwNumber)
		{
			return (*it);
		}
	}

	return 0;
}

GUILD_INFO* CGuildManager::GetMemberGuildInfo(char* szGuildMember)
{
	for (std::vector<GUILD_INFO*>::iterator it = this->vGuildList.begin(); it != this->vGuildList.end(); it++)
	{
		for (int n = 0; n < MAX_GUILD_MEMBER; n++)
		{
			if ((*it)->arGuildMember[n].szGuildMember[0] == szGuildMember[0])
			{
				if (_stricmp((*it)->arGuildMember[n].szGuildMember, szGuildMember) == 0)
				{
					return (*it);
				}
			}
		}
	}

	return 0;
}

GUILD_MEMBER_INFO* CGuildManager::GetGuildMemberInfo(char* szGuildMember)
{
	for (std::vector<GUILD_INFO*>::iterator it = this->vGuildList.begin(); it != this->vGuildList.end(); it++)
	{
		for (int n = 0; n < MAX_GUILD_MEMBER; n++)
		{
			if ((*it)->arGuildMember[n].szGuildMember[0] == szGuildMember[0])
			{
				if (_stricmp((*it)->arGuildMember[n].szGuildMember, szGuildMember) == 0)
				{
					return &(*it)->arGuildMember[n];
				}
			}
		}
	}

	return 0;
}

void CGuildManager::ConnectMember(char* szGuildMember, WORD btServer)
{
	GUILD_MEMBER_INFO* lpGuildMemberInfo = this->GetGuildMemberInfo(szGuildMember);

	if (lpGuildMemberInfo != 0)
	{
		lpGuildMemberInfo->btServer = btServer;
	}
}

void CGuildManager::DisconnectMember(char* szGuildMember)
{
	GUILD_MEMBER_INFO* lpGuildMemberInfo = this->GetGuildMemberInfo(szGuildMember);

	if (lpGuildMemberInfo != 0)
	{
		lpGuildMemberInfo->btServer = 0xFFFF;
	}
}

BYTE CGuildManager::AddGuild(int index, char* szGuildName, char* szMasterName, BYTE* lpMark)
{
	GUILD_INFO* lpGuildInfo = this->GetGuildInfo(szGuildName);

	if (lpGuildInfo != 0)
	{
		return 0;
	}

	if (CheckTextSyntax(szGuildName, strlen(szGuildName)) == 0 || CheckSpecialText(szGuildName) == false)
	{
		return 5;
	}

	if (gQueryManager.ExecQuery("WZ_GuildCreate '%s','%s'", szGuildName, szMasterName) == false || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		return 6;
	}
	else
	{
		if (gQueryManager.GetResult(0) == 0)
		{
			gQueryManager.Close();

			gQueryManager.BindParameterAsBinary(1, lpMark, 32);

			gQueryManager.ExecQuery("UPDATE Guild SET G_Mark=? WHERE G_Name='%s'", szGuildName);

			gQueryManager.Close();

			gQueryManager.ExecQuery("UPDATE GuildMember SET G_Status=%d WHERE Name='%s'", 0x80, szMasterName);

			gQueryManager.Close();

			GUILD_INFO* GuildInfo = new GUILD_INFO();

			gQueryManager.ExecQuery("SELECT Number FROM Guild WHERE G_Name='%s'", szGuildName);

			gQueryManager.Fetch();

			GuildInfo->dwNumber = gQueryManager.GetAsInteger("Number");

			gQueryManager.Close();

			memcpy(GuildInfo->szName, szGuildName, sizeof(GuildInfo->szName));

			memcpy(GuildInfo->szMaster, szMasterName, sizeof(GuildInfo->szMaster));

			memcpy(GuildInfo->arMark, lpMark, sizeof(GuildInfo->arMark));

			memcpy(GuildInfo->arGuildMember[0].szGuildMember, szMasterName, sizeof(GuildInfo->arGuildMember[0].szGuildMember));

			GuildInfo->arGuildMember[0].btStatus = 0x80;

			GuildInfo->arGuildMember[0].btServer = 0xFFFF;

			this->vGuildList.push_back(GuildInfo);

			return 1;
		}
		else
		{
			gQueryManager.Close();

			return 6;
		}
	}
}

BYTE CGuildManager::DelGuild(int index, char* szGuildName)
{
	GUILD_INFO* lpGuildInfo = this->GetGuildInfo(szGuildName);

	if (lpGuildInfo == 0)
	{
		return 3;
	}

	lpGuildInfo->Clear();

	gQueryManager.ExecQuery("DELETE FROM Guild WHERE G_Name='%s'", szGuildName);

	gQueryManager.Close();

	gQueryManager.ExecQuery("DELETE FROM GuildMember WHERE G_Name='%s'", szGuildName);

	gQueryManager.Close();

	return 1;
}

BYTE CGuildManager::AddGuildMember(int index, char* szGuildName, char* szGuildMember, BYTE btStatus, WORD btServer)
{
	GUILD_INFO* lpGuildInfo = this->GetGuildInfo(szGuildName);

	if (lpGuildInfo == 0)
	{
		return 0;
	}

	if (this->GetGuildMemberInfo(szGuildMember) != 0)
	{
		return 3;
	}

	if (gQueryManager.ExecQuery("INSERT INTO GuildMember (Name,G_Name,G_Status) VALUES ('%s','%s',%d)", szGuildMember, szGuildName, btStatus) == false)
	{
		gQueryManager.Close();

		return 5;
	}
	else
	{
		gQueryManager.Close();

		GUILD_MEMBER_INFO GuildMemberInfo;

		GuildMemberInfo.Clear();

		memcpy(GuildMemberInfo.szGuildMember, szGuildMember, sizeof(GuildMemberInfo.szGuildMember));

		GuildMemberInfo.btStatus = btStatus;

		GuildMemberInfo.btServer = btServer;

		for (int n = 1; n < MAX_GUILD_MEMBER; n++)
		{
			if (lpGuildInfo->arGuildMember[n].IsEmpty() != false)
			{
				lpGuildInfo->arGuildMember[n] = GuildMemberInfo;

				return 1;
			}
		}

		return 2;
	}
}

BYTE CGuildManager::DelGuildMember(int index, char* szGuildMember)
{
	GUILD_MEMBER_INFO* lpGuildMemberInfo = this->GetGuildMemberInfo(szGuildMember);

	if (lpGuildMemberInfo == 0)
	{
		return 3;
	}

	gQueryManager.ExecQuery("DELETE FROM GuildMember WHERE Name='%s'", szGuildMember);

	gQueryManager.Close();

	lpGuildMemberInfo->Clear();

	return 1;
}

BYTE CGuildManager::SetGuildScore(char* szGuildName, DWORD dwScore)
{
	GUILD_INFO* lpGuildInfo = this->GetGuildInfo(szGuildName);

	if (lpGuildInfo == 0)
	{
		return 0;
	}

	if (gQueryManager.ExecQuery("UPDATE Guild SET G_Score=%d WHERE G_Name='%s'", dwScore, szGuildName) == false)
	{
		gQueryManager.Close();

		return 0;
	}
	else
	{
		gQueryManager.Close();

		lpGuildInfo->dwScore = dwScore;

		return 1;
	}
}

BYTE CGuildManager::SetGuildNotice(char* szGuildName, char* szNotice)
{
	GUILD_INFO* lpGuildInfo = this->GetGuildInfo(szGuildName);

	if (lpGuildInfo == 0)
	{
		return 0;
	}

	gQueryManager.BindParameterAsString(1, szNotice, sizeof(lpGuildInfo->szNotice));

	if (gQueryManager.ExecQuery("UPDATE Guild SET G_Notice=? WHERE G_Name='%s'", szGuildName) == false)
	{
		gQueryManager.Close();

		return 0;
	}
	else
	{
		gQueryManager.Close();

		memcpy(lpGuildInfo->szNotice, szNotice, sizeof(lpGuildInfo->szNotice));

		return 1;
	}
}