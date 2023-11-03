#include "stdafx.h"
#include "Guild.h"
#include "GuildManager.h"
#include "ServerManager.h"
#include "SocketManager.h"
#include "Util.h"

CGuild gGuild;

CGuild::CGuild()
{

}

CGuild::~CGuild()
{

}

void CGuild::GDGuildCreateRecv(SDHP_GUILD_CREATE_RECV* lpMsg, int aIndex)
{
	SDHP_GUILD_CREATE_SEND pMsg;

	pMsg.header.set(0x07, 0x00, sizeof(pMsg));

	pMsg.result = 0;

	pMsg.flag = 1;

	pMsg.index = lpMsg->index;

	pMsg.GuildNumber = 0;

	memcpy(pMsg.GuildName, lpMsg->GuildName, sizeof(pMsg.GuildName));

	memcpy(pMsg.Name, lpMsg->Name, sizeof(pMsg.Name));

	memcpy(pMsg.Mark, lpMsg->Mark, sizeof(pMsg.Mark));

	if ((pMsg.result = gGuildManager.AddGuild(aIndex, lpMsg->GuildName, lpMsg->Name, lpMsg->Mark)) != 1)
	{
		gSocketManager.DataSend(aIndex, (BYTE*)&pMsg, sizeof(pMsg));

		return;
	}

	GUILD_INFO* lpGuildInfo = gGuildManager.GetGuildInfo(lpMsg->GuildName);

	GUILD_MEMBER_INFO* lpGuildMemberInfo = gGuildManager.GetGuildMemberInfo(lpMsg->Name);

	if (lpGuildInfo != 0 && lpGuildMemberInfo != 0)
	{
		pMsg.GuildNumber = lpGuildInfo->dwNumber;

		for (int n = 0; n < MAX_SERVER; n++)
		{
			if (gServerManager[n].CheckState() != false)
			{
				pMsg.flag = ((n == aIndex) ? 1 : 0);

				gSocketManager.DataSend(n, (BYTE*)&pMsg, sizeof(pMsg));
			}
		}

		gGuildManager.ConnectMember(lpMsg->Name, GetServerCodeByName(lpMsg->Name));

		this->DGGuildInfoSend(lpGuildInfo->dwNumber);

		this->DGGuildMemberInfoSend(lpMsg->GuildName, lpMsg->Name, lpGuildMemberInfo->btStatus, lpGuildMemberInfo->btServer);
	}
}

void CGuild::DGGuildMemberInfoSend(char* GuildName, char* Name, BYTE status, WORD server)
{
	SDHP_GUILD_MEMBER_INFO_SEND pMsg;

	pMsg.header.set(0x07, 0x05, sizeof(pMsg));

	memcpy(pMsg.GuildName, GuildName, sizeof(pMsg.GuildName));

	memcpy(pMsg.Name, Name, sizeof(pMsg.Name));

	pMsg.status = status;

	pMsg.server = server;

	for (int n = 0; n < MAX_SERVER; n++)
	{
		if (gServerManager[n].CheckState() != false)
		{
			gSocketManager.DataSend(n, (BYTE*)&pMsg, sizeof(pMsg));
		}
	}
}

void CGuild::DGGuildInfoSend(DWORD GuildNumber)
{
	GUILD_INFO* lpGuildInfo = gGuildManager.GetGuildInfo(GuildNumber);

	if (lpGuildInfo == 0)
	{
		return;
	}

	BYTE send[2048];

	SDHP_GUILD_INFO_SEND pMsg;

	pMsg.header.set(0x07, 0x09, 0);

	int size = sizeof(pMsg);

	pMsg.GuildNumber = lpGuildInfo->dwNumber;

	memcpy(pMsg.GuildName, lpGuildInfo->szName, sizeof(pMsg.GuildName));

	memcpy(pMsg.GuildMaster, lpGuildInfo->szMaster, sizeof(pMsg.GuildMaster));

	memcpy(pMsg.Mark, lpGuildInfo->arMark, sizeof(pMsg.Mark));

	memcpy(pMsg.Notice, lpGuildInfo->szNotice, sizeof(pMsg.Notice));

	pMsg.score = lpGuildInfo->dwScore;

	pMsg.count = 0;

	SDHP_GUILD_MEMBER_INFO info;

	for (int n = 0; n < MAX_GUILD_MEMBER; n++)
	{
		GUILD_MEMBER_INFO* lpGuildMemberInfo = &lpGuildInfo->arGuildMember[n];

		if (lpGuildMemberInfo->IsEmpty() == false)
		{
			memcpy(info.Name, lpGuildMemberInfo->szGuildMember, sizeof(info.Name));

			info.status = lpGuildMemberInfo->btStatus;

			info.server = lpGuildMemberInfo->btServer;

			memcpy(&send[size], &info, sizeof(info));

			size += sizeof(info);

			pMsg.count++;
		}
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	for (int n = 0; n < MAX_SERVER; n++)
	{
		if (gServerManager[n].CheckState() != false)
		{
			gSocketManager.DataSend(n, send, size);
		}
	}
}

void CGuild::GDGuildDeleteRecv(SDHP_GUILD_DELETE_RECV* lpMsg, int aIndex)
{
	SDHP_GUILD_DELETE_SEND pMsg;

	pMsg.header.set(0x07, 0x01, sizeof(pMsg));

	pMsg.result = 0;

	pMsg.flag = 1;

	pMsg.index = lpMsg->index;

	memcpy(pMsg.GuildName, lpMsg->GuildName, sizeof(pMsg.GuildName));

	memcpy(pMsg.Name, lpMsg->Name, sizeof(pMsg.Name));

	if ((pMsg.result = gGuildManager.DelGuild(aIndex, lpMsg->GuildName)) != 1)
	{
		gSocketManager.DataSend(aIndex, (BYTE*)&pMsg, sizeof(pMsg));

		return;
	}

	for (int n = 0; n < MAX_SERVER; n++)
	{
		if (gServerManager[n].CheckState() != false)
		{
			pMsg.flag = ((n == aIndex) ? 1 : 0);

			gSocketManager.DataSend(n, (BYTE*)&pMsg, sizeof(pMsg));
		}
	}
}

void CGuild::GDGuildMemberDisconnectRecv(SDHP_GUILD_MEMBER_DISCONNECT_RECV* lpMsg)
{
	SDHP_GUILD_MEMBER_DISCONNECT_SEND pMsg;

	pMsg.header.set(0x07, 0x02, sizeof(pMsg));

	memcpy(pMsg.GuildName, lpMsg->GuildName, sizeof(pMsg.GuildName));

	memcpy(pMsg.Name, lpMsg->Name, sizeof(pMsg.Name));

	for (int n = 0; n < MAX_SERVER; n++)
	{
		if (gServerManager[n].CheckState() != false)
		{
			gSocketManager.DataSend(n, (BYTE*)&pMsg, sizeof(pMsg));
		}
	}
}

void CGuild::GDGuildMemberJoinRecv(SDHP_GUILD_MEMBER_JOIN_RECV* lpMsg, int aIndex)
{
	SDHP_GUILD_MEMBER_JOIN_SEND pMsg;

	pMsg.header.set(0x07, 0x03, sizeof(pMsg));

	pMsg.result = 0;

	pMsg.flag = 1;

	pMsg.index = lpMsg->index;

	memcpy(pMsg.GuildName, lpMsg->GuildName, sizeof(pMsg.GuildName));

	memcpy(pMsg.Name, lpMsg->Name, sizeof(pMsg.Name));

	pMsg.server = GetServerCodeByName(lpMsg->Name);

	if ((pMsg.result = gGuildManager.AddGuildMember(aIndex, lpMsg->GuildName, lpMsg->Name, 0, pMsg.server)) != 1)
	{
		gSocketManager.DataSend(aIndex, (BYTE*)&pMsg, sizeof(pMsg));

		return;
	}

	for (int n = 0; n < MAX_SERVER; n++)
	{
		if (gServerManager[n].CheckState() != false)
		{
			pMsg.flag = ((n == aIndex) ? 1 : 0);

			gSocketManager.DataSend(n, (BYTE*)&pMsg, sizeof(pMsg));
		}
	}
}

void CGuild::GDGuildMemberDeleteRecv(SDHP_GUILD_MEMBER_DELETE_RECV* lpMsg, int aIndex)
{
	SDHP_GUILD_MEMBER_DELETE_SEND pMsg;

	pMsg.header.set(0x07, 0x04, sizeof(pMsg));

	pMsg.result = 0;

	pMsg.flag = 1;

	pMsg.index = lpMsg->index;

	memcpy(pMsg.GuildName, lpMsg->GuildName, sizeof(pMsg.GuildName));

	memcpy(pMsg.Name, lpMsg->Name, sizeof(pMsg.Name));

	if ((pMsg.result = gGuildManager.DelGuildMember(aIndex, lpMsg->Name)) != 1)
	{
		gSocketManager.DataSend(aIndex, (BYTE*)&pMsg, sizeof(pMsg));

		return;
	}

	for (int n = 0; n < MAX_SERVER; n++)
	{
		if (gServerManager[n].CheckState() != false)
		{
			pMsg.flag = ((n == aIndex) ? 1 : 0);

			gSocketManager.DataSend(n, (BYTE*)&pMsg, sizeof(pMsg));
		}
	}
}

void CGuild::GDGuildMemberGetInfoRecv(SDHP_GUILD_MEMBER_GET_INFO_RECV* lpMsg, int aIndex)
{
	GUILD_INFO* lpGuildInfo = gGuildManager.GetMemberGuildInfo(lpMsg->Name);

	GUILD_MEMBER_INFO* lpGuildMemberInfo = gGuildManager.GetGuildMemberInfo(lpMsg->Name);

	if (lpGuildInfo == 0 || lpGuildMemberInfo == 0)
	{
		return;
	}

	gGuildManager.ConnectMember(lpMsg->Name, GetServerCodeByName(lpMsg->Name));

	this->DGGuildInfoSend(lpGuildInfo->dwNumber);

	this->DGGuildMemberInfoSend(lpGuildInfo->szName, lpMsg->Name, lpGuildMemberInfo->btStatus, lpGuildMemberInfo->btServer);
}

void CGuild::GDGuildScoreUpdateRecv(SDHP_GUILD_SCORE_UPDATE_RECV* lpMsg)
{
	if (gGuildManager.SetGuildScore(lpMsg->GuildName, lpMsg->score) != 0)
	{
		SDHP_GUILD_SCORE_UPDATE_SEND pMsg;

		pMsg.header.set(0x07, 0x06, sizeof(pMsg));

		memcpy(pMsg.GuildName, lpMsg->GuildName, sizeof(pMsg.GuildName));

		pMsg.score = lpMsg->score;

		for (int n = 0; n < MAX_SERVER; n++)
		{
			if (gServerManager[n].CheckState() != false)
			{
				gSocketManager.DataSend(n, (BYTE*)&pMsg, sizeof(pMsg));
			}
		}
	}
}

void CGuild::GDGuildGlobalChatRecv(SDHP_GUILD_GLOBAL_CHAT_RECV* lpMsg)
{
	SDHP_GUILD_GLOBAL_CHAT_SEND pMsg;

	pMsg.header.set(0x07, 0x07, sizeof(pMsg));

	memcpy(pMsg.GuildName, lpMsg->GuildName, sizeof(pMsg.GuildName));

	memcpy(pMsg.Name, lpMsg->Name, sizeof(pMsg.Name));

	memcpy(pMsg.message, lpMsg->message, sizeof(pMsg.message));

	for (int n = 0; n < MAX_SERVER; n++)
	{
		if (gServerManager[n].CheckState() != false)
		{
			gSocketManager.DataSend(n, (BYTE*)&pMsg, sizeof(pMsg));
		}
	}
}

void CGuild::GDGuildGlobalNoticeRecv(SDHP_GUILD_GLOBAL_NOTICE_RECV* lpMsg)
{
	if (gGuildManager.SetGuildNotice(lpMsg->GuildName, lpMsg->message) != 0)
	{
		SDHP_GUILD_GLOBAL_NOTICE_SEND pMsg;

		pMsg.header.set(0x07, 0x08, sizeof(pMsg));

		memcpy(pMsg.GuildName, lpMsg->GuildName, sizeof(pMsg.GuildName));

		memcpy(pMsg.message, lpMsg->message, sizeof(pMsg.message));

		for (int n = 0; n < MAX_SERVER; n++)
		{
			if (gServerManager[n].CheckState() != false)
			{
				gSocketManager.DataSend(n, (BYTE*)&pMsg, sizeof(pMsg));
			}
		}
	}
}