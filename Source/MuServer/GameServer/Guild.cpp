#include "stdafx.h"
#include "Guild.h"
#include "GuildManager.h"
#include "BattleSoccerManager.h"
#include "GameMain.h"
#include "Map.h"
#include "Message.h"
#include "Monster.h"
#include "Notice.h"
#include "ServerInfo.h"
#include "Util.h"
#include "Viewport.h"

CGuild gGuild;

CGuild::CGuild()
{

}

CGuild::~CGuild()
{

}

void CGuild::CGGuildMasterOpenRecv(PMSG_GUILD_MASTER_OPEN_RECV* lpMsg, int aIndex)
{
	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpMsg->result == 1)
	{
		if (lpObj->Level < gServerInfo.m_GuildCreateMinLevel)
		{
			goto CLEAR_JUMP;
		}

		if (lpObj->Reset < gServerInfo.m_GuildCreateMinReset)
		{
			goto CLEAR_JUMP;
		}

		PBMSG_HEAD pMsg;

		pMsg.set(0x55, sizeof(pMsg));

		DataSend(aIndex, (BYTE*)&pMsg, pMsg.size);

		return;
	}
	else
	{
		goto CLEAR_JUMP;
	}

CLEAR_JUMP:

	if (lpObj->Interface.use != 0 && lpObj->Interface.type == INTERFACE_GUILD_CREATE)
	{
		lpObj->Interface.use = 0;
	}
}

void CGuild::CGGuildMasterCancelRecv(int aIndex)
{
	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->GuildNumber > 0)
	{
		return;
	}

	if (lpObj->Interface.use != 0 && lpObj->Interface.type == INTERFACE_GUILD_CREATE)
	{
		lpObj->Interface.use = 0;
	}
}

void CGuild::CGGuildCreateRecv(PMSG_GUILD_CREATE_RECV* lpMsg, int aIndex)
{
	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	char GuildName[9] = { 0 };

	memcpy(GuildName, lpMsg->GuildName, sizeof(lpMsg->GuildName));

	if (strlen(GuildName) < 2 || strlen(GuildName) > 8)
	{
		this->GCGuildCreateResultSend(aIndex, 2);
	}
	else if (CheckSpaceCharacter(GuildName) == false)
	{
		this->GCGuildCreateResultSend(aIndex, 4);
	}
	else if (CheckSpecialText(GuildName) == false)
	{
		this->GCGuildCreateResultSend(aIndex, 5);
	}
	else
	{
		SDHP_GUILD_CREATE_SEND pMsg;

		pMsg.header.set(0x07, 0x00, sizeof(pMsg));

		pMsg.index = aIndex;

		memcpy(pMsg.GuildName, GuildName, sizeof(pMsg.GuildName));

		memcpy(pMsg.Name, lpObj->Name, sizeof(pMsg.Name));

		memcpy(pMsg.Mark, lpMsg->Mark, sizeof(pMsg.Mark));

		gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
	}

	if (lpObj->Interface.use != 0 && lpObj->Interface.type == INTERFACE_GUILD_CREATE)
	{
		lpObj->Interface.use = 0;
	}
}

void CGuild::DGGuildCreateRecv(SDHP_GUILD_CREATE_RECV* lpMsg)
{
	GUILD_INFO* lpGuild = NULL;

	if (lpMsg->result == 1)
	{
		lpGuild = gGuildManager.AddGuild(lpMsg->GuildNumber, lpMsg->GuildName, lpMsg->Mark, lpMsg->Name, 0);
	}

	if (lpMsg->flag != 1)
	{
		return;
	}

	if (gObjIsConnectedGP(lpMsg->index) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	this->GCGuildCreateResultSend(lpObj->Index, lpMsg->result);

	if (lpGuild != NULL)
	{
		lpObj->Guild = lpGuild;

		lpObj->GuildNumber = lpMsg->GuildNumber;

		lpObj->GuildStatus = GUILD_STATUS_MASTER;

		memcpy(lpObj->GuildName, lpMsg->GuildName, sizeof(lpObj->GuildName));

		gViewport.GCViewportSimpleGuildInfoSend(lpObj);

		gViewport.GCViewportSimpleGuildMemberSend(lpObj);
	}

	if (lpObj->Interface.use != 0 && lpObj->Interface.type == INTERFACE_GUILD_CREATE)
	{
		lpObj->Interface.use = 0;
	}
}

void CGuild::GCGuildCreateResultSend(int aIndex, int result)
{
	PMSG_GUILD_CREATE_RESULT_SEND pMsg;

	pMsg.header.set(0x56, sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CGuild::CGGuildDeleteRecv(PMSG_GUILD_DELETE_RECV* lpMsg, int aIndex)
{
	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if (gServerInfo.m_GuildDeleteSwitch == 0)
	{
		this->GCGuildDeleteSend(aIndex, 3);

		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->GuildNumber == 0)
	{
		this->GCGuildDeleteSend(aIndex, 3);

		return;
	}

	char name[11] = { 0 };

	char PersonalCode[14] = { 0 };

	memcpy(name, lpMsg->name, sizeof(lpMsg->name));

	memcpy(PersonalCode, lpMsg->PersonalCode, sizeof(lpMsg->PersonalCode));

	if (strcmp(lpObj->Guild->Members[0].Name, lpObj->Name) == 0)
	{
		if (strcmp(lpObj->Name, name) != 0)
		{
			if (gObjCheckPersonalCode(aIndex, PersonalCode) == 0)
			{
				this->GCGuildDeleteSend(aIndex, 0);

				return;
			}
			else
			{
				this->GDGuildMemberDeleteSend(aIndex, lpObj->Guild->Name, name);

				return;
			}
		}

		if (gObjCheckPersonalCode(aIndex, PersonalCode) == 0)
		{
			this->GCGuildDeleteSend(aIndex, 0);

			return;
		}

		this->GuildMasterCloseCheck(lpObj);

		SDHP_GUILD_DELETE_SEND pMsg;

		pMsg.header.set(0x07, 0x01, sizeof(pMsg));

		pMsg.index = aIndex;

		memcpy(pMsg.GuildName, lpObj->Guild->Name, sizeof(pMsg.GuildName));

		memcpy(pMsg.Name, name, sizeof(pMsg.Name));

		gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
	}
	else
	{
		if (strcmp(lpObj->Name, name) != 0)
		{
			this->GCGuildDeleteSend(aIndex, 2);

			return;
		}

		if (gObjCheckPersonalCode(aIndex, PersonalCode) == 0)
		{
			this->GCGuildDeleteSend(aIndex, 0);

			return;
		}

		this->GDGuildMemberDeleteSend(aIndex, lpObj->Guild->Name, name);
	}
}

void CGuild::DGGuildDeleteRecv(SDHP_GUILD_DELETE_RECV* lpMsg)
{
	LPOBJ lpObj = &gObj[lpMsg->index];

	if (lpObj != 0 && lpMsg->flag != 0 && strcmp(lpMsg->Name, lpObj->Name) == 0)
	{
		this->GCGuildDeleteSend(lpObj->Index, lpMsg->result);

		if (lpMsg->result == 1)
		{
			this->GCGuildViewportDeleteSend(lpObj->Index);
		}
	}

	if (lpMsg->result == 1)
	{
		GUILD_INFO* lpGuild = gGuildManager.SearchGuild(lpMsg->GuildName);

		if (lpGuild == 0)
		{
			return;
		}

		for (int n = 0; n < MAX_GUILD_MEMBER; n++)
		{
			if (lpGuild->Members[n].Use != false && lpGuild->Members[n].Index >= 0)
			{
				if (gObjIsNameValid(lpGuild->Members[n].Index, lpGuild->Members[n].Name) != 0)
				{
					LPOBJ lpTarget = &gObj[lpGuild->Members[n].Index];

					lpTarget->Guild = 0;

					lpTarget->GuildNumber = 0;

					lpTarget->GuildStatus = GUILD_STATUS_NONE;

					memset(lpTarget->GuildName, 0, sizeof(lpTarget->GuildName));

					if (lpObj->Name != lpTarget->Name)
					{
						this->GCGuildDeleteSend(lpTarget->Index, 4);

						this->GCGuildViewportDeleteSend(lpTarget->Index);
					}
				}
			}
		}

		gGuildManager.DeleteGuild(lpMsg->GuildName);
	}
}

void CGuild::GCGuildDeleteSend(int aIndex, int result)
{
	PMSG_GUILD_DELETE_SEND pMsg;

	pMsg.header.set(0x53, sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CGuild::GDGuildMemberDisconnectSend(int aIndex, char* GuildName, char* Name)
{
	SDHP_GUILD_MEMBER_DISCONNECT_SEND pMsg;

	pMsg.header.set(0x07, 0x02, sizeof(pMsg));

	memcpy(pMsg.GuildName, GuildName, sizeof(pMsg.GuildName));

	memcpy(pMsg.Name, Name, sizeof(pMsg.Name));

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void CGuild::DGGuildMemberDisconnectRecv(SDHP_GUILD_MEMBER_DISCONNECT_RECV* lpMsg)
{
	gGuildManager.CloseMember(lpMsg->GuildName, lpMsg->Name);
}

void CGuild::CGGuildRequestRecv(PMSG_GUILD_REQUEST_RECV* lpMsg, int aIndex)
{
	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	int bIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]);

	if (gObjIsConnectedGP(bIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	LPOBJ lpTarget = &gObj[bIndex];

	if ((lpTarget->Option & 1) == 0)
	{
		this->GCGuildResultSend(aIndex, 0);

		return;
	}

	if (lpTarget->GuildNumber == 0)
	{
		this->GCGuildResultSend(aIndex, 4);

		return;
	}

	if (lpTarget->GuildStatus != GUILD_STATUS_MASTER)
	{
		this->GCGuildResultSend(aIndex, 4);

		return;
	}

	if (lpObj->GuildNumber > 0)
	{
		this->GCGuildResultSend(aIndex, 5);

		return;
	}

	if (lpObj->Interface.use != 0 || lpTarget->Interface.use != 0)
	{
		this->GCGuildResultSend(aIndex, 6);

		return;
	}

	if (lpTarget->Guild->WarState != GUILD_WAR_STATE_NONE)
	{
		this->GCGuildResultSend(aIndex, 6);

		return;
	}

	if (lpObj->Level < 6)
	{
		this->GCGuildResultSend(aIndex, 7);

		return;
	}

	if (lpTarget->Guild->GetMemberCount() >= (BYTE)this->GetMaxGuildUser(lpTarget))
	{
		this->GCGuildResultSend(aIndex, 2);

		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_GUILD;
	lpObj->Interface.state = 0;
	lpObj->InterfaceTime = GetTickCount();
	lpObj->TargetNumber = bIndex;

	lpTarget->Interface.use = 1;
	lpTarget->Interface.type = INTERFACE_GUILD;
	lpTarget->Interface.state = 0;
	lpTarget->InterfaceTime = GetTickCount();
	lpTarget->TargetNumber = aIndex;

	PMSG_GUILD_REQUEST_SEND pMsg;

	pMsg.header.set(0x50, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	DataSend(bIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CGuild::CGGuildResultRecv(PMSG_GUILD_RESULT_RECV* lpMsg, int aIndex)
{
	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	int bIndex = MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]);

	if (gObjIsConnectedGP(bIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	LPOBJ lpTarget = &gObj[bIndex];

	if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_GUILD)
	{
		return;
	}

	if (lpTarget->Interface.use == 0 || lpTarget->Interface.type != INTERFACE_GUILD)
	{
		return;
	}

	if (lpMsg->result == 0)
	{
		this->GCGuildResultSend(bIndex, 0);

		goto CLEAR_JUMP;
	}

	if ((lpObj->Option & 1) == 0)
	{
		this->GCGuildResultSend(bIndex, 0);

		goto CLEAR_JUMP;
	}

	if (lpObj->GuildNumber == 0)
	{
		this->GCGuildResultSend(bIndex, 4);

		goto CLEAR_JUMP;
	}

	if (lpObj->GuildStatus != GUILD_STATUS_MASTER)
	{
		this->GCGuildResultSend(bIndex, 4);

		goto CLEAR_JUMP;
	}

	if (lpTarget->GuildNumber > 0)
	{
		this->GCGuildResultSend(bIndex, 5);

		goto CLEAR_JUMP;
	}

	if (lpObj->Guild->WarState != GUILD_WAR_STATE_NONE)
	{
		this->GCGuildResultSend(bIndex, 6);

		goto CLEAR_JUMP;
	}

	if (lpTarget->Level < 6)
	{
		this->GCGuildResultSend(bIndex, 7);

		goto CLEAR_JUMP;
	}

	if (lpObj->Guild->GetMemberCount() >= (BYTE)this->GetMaxGuildUser(lpObj))
	{
		this->GCGuildResultSend(bIndex, 2);

		goto CLEAR_JUMP;
	}

	this->GDGuildMemberJoinSend(bIndex, lpObj->Guild->Name, lpTarget->Name);

CLEAR_JUMP:

	lpObj->Interface.use = 0;
	lpObj->Interface.type = INTERFACE_NONE;
	lpObj->Interface.state = 0;
	lpObj->InterfaceTime = 0;
	lpObj->TargetNumber = -1;

	lpTarget->Interface.use = 0;
	lpTarget->Interface.type = INTERFACE_NONE;
	lpTarget->Interface.state = 0;
	lpTarget->InterfaceTime = 0;
	lpTarget->TargetNumber = -1;
}

int CGuild::GetMaxGuildUser(LPOBJ lpObj)
{
	return ((gServerInfo.m_GuildInsertMaxUser[lpObj->AccountLevel] > MAX_GUILD_MEMBER) ? MAX_GUILD_MEMBER : gServerInfo.m_GuildInsertMaxUser[lpObj->AccountLevel]);
}

void CGuild::GDGuildMemberJoinSend(int aIndex, char* GuildName, char* Name)
{
	SDHP_GUILD_MEMBER_JOIN_SEND pMsg;

	pMsg.header.set(0x07, 0x03, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.GuildName, GuildName, sizeof(pMsg.GuildName));

	memcpy(pMsg.Name, Name, sizeof(pMsg.Name));

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void CGuild::DGGuildMemberJoinRecv(SDHP_GUILD_MEMBER_JOIN_RECV* lpMsg)
{
	GUILD_INFO* lpGuild = NULL;

	if (lpMsg->result == 1)
	{
		lpGuild = gGuildManager.AddMember(lpMsg->GuildName, lpMsg->Name, lpMsg->index, GUILD_STATUS_MEMBER, lpMsg->server);
	}

	if (lpMsg->flag != 1)
	{
		return;
	}

	if (gObjIsConnectedGP(lpMsg->index) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	this->GCGuildResultSend(lpObj->Index, lpMsg->result);

	if (lpGuild != NULL)
	{
		lpObj->Guild = lpGuild;

		lpObj->GuildStatus = GUILD_STATUS_MEMBER;

		lpObj->GuildNumber = lpGuild->Number;

		memcpy(lpObj->GuildName, lpGuild->Name, sizeof(lpObj->GuildName));

		if (strlen(lpGuild->Notice) > 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 2, "%s", lpGuild->Notice);
		}

		gViewport.GCViewportSimpleGuildInfoSend(lpObj);

		gViewport.GCViewportSimpleGuildMemberSend(lpObj);
	}
}

void CGuild::GCGuildResultSend(int aIndex, int result)
{
	PMSG_GUILD_RESULT_SEND pMsg;

	pMsg.header.set(0x51, sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CGuild::GDGuildMemberDeleteSend(int aIndex, char* GuildName, char* Name)
{
	SDHP_GUILD_MEMBER_DELETE_SEND pMsg;

	pMsg.header.set(0x07, 0x04, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.GuildName, GuildName, sizeof(pMsg.GuildName));

	memcpy(pMsg.Name, Name, sizeof(pMsg.Name));

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void CGuild::DGGuildMemberDeleteRecv(SDHP_GUILD_MEMBER_DELETE_RECV* lpMsg)
{
	LPOBJ lpObj = &gObj[lpMsg->index];

	if (lpObj != 0 && lpMsg->flag != 0 && strcmp(lpMsg->Name, lpObj->Name) == 0)
	{
		this->GCGuildDeleteSend(lpObj->Index, lpMsg->result);
	}

	if (lpMsg->result == 1)
	{
		LPOBJ lpTarget = gObjFind(lpMsg->Name);

		if (lpTarget != 0)
		{
			this->GCGuildViewportDeleteSend(lpTarget->Index);

			lpTarget->Guild = 0;

			lpTarget->GuildNumber = 0;

			lpTarget->GuildStatus = GUILD_STATUS_NONE;

			memset(lpTarget->GuildName, 0, sizeof(lpTarget->GuildName));
		}

		gGuildManager.DelMember(lpMsg->GuildName, lpMsg->Name);
	}
}

void CGuild::GCGuildViewportDeleteSend(int aIndex)
{
	PMSG_GUILD_VIEWPORT_DELETE_SEND pMsg;

	pMsg.header.set(0x5D, sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);

	MsgSendV2(&gObj[aIndex], (BYTE*)&pMsg, pMsg.header.size);
}

void CGuild::GDGuildMemberGetInfoSend(int aIndex, char* Name)
{
	SDHP_GUILD_MEMBER_GET_INFO_SEND pMsg;

	pMsg.header.set(0x07, 0x05, sizeof(pMsg));

	memcpy(pMsg.Name, Name, sizeof(pMsg.Name));

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void CGuild::DGGuildInfoRecv(SDHP_GUILD_INFO_RECV* lpMsg)
{
	GUILD_INFO* lpGuild = gGuildManager.AddGuild(lpMsg->GuildNumber, lpMsg->GuildName, lpMsg->Mark, lpMsg->GuildMaster, lpMsg->score);

	if (lpGuild != 0)
	{
		memcpy(lpGuild->Notice, lpMsg->Notice, sizeof(lpGuild->Notice));
	}

	for (int n = 0; n < lpMsg->count; n++)
	{
		SDHP_GUILD_MEMBER_INFO* lpInfo = (SDHP_GUILD_MEMBER_INFO*)(((BYTE*)lpMsg) + sizeof(SDHP_GUILD_INFO_RECV) + (sizeof(SDHP_GUILD_MEMBER_INFO) * n));

		if (gGuildManager.AddMember(lpMsg->GuildName, lpInfo->Name, -1, lpInfo->status, lpInfo->server) == 0)
		{
			break;
		}
	}
}

void CGuild::DGGuildMemberInfoRecv(SDHP_GUILD_MEMBER_INFO_RECV* lpMsg)
{
	LPOBJ lpObj = gObjFind(lpMsg->Name);

	if (lpObj != 0)
	{
		memcpy(lpObj->GuildName, lpMsg->GuildName, sizeof(lpObj->GuildName));

		lpObj->Guild = gGuildManager.SearchGuild(lpObj->GuildName);

		if (lpObj->Guild != 0)
		{
			lpObj->GuildStatus = lpMsg->status;

			lpObj->GuildNumber = lpObj->Guild->Number;

			gGuildManager.ConnectUser(lpObj->Guild, lpMsg->GuildName, lpObj->Name, lpObj->Index, lpMsg->status, lpMsg->server);

			if (strlen(lpObj->Guild->Notice) > 0)
			{
				gNotice.GCNoticeSend(lpObj->Index, 2, "%s", lpObj->Guild->Notice);
			}

			gViewport.GCViewportSimpleGuildInfoSend(lpObj);

			gViewport.GCViewportSimpleGuildMemberSend(lpObj);

			if (lpObj->Guild->WarState != GUILD_WAR_STATE_NONE)
			{
				this->GCGuildWarScoreSend(lpObj->Index);

				this->GCGuildWarStartSend(lpObj->Index, lpObj->Guild->TargetGuildNode->Name, GUILD_WAR_STATE_NONE, GUILD_WAR_TEAM_BLUE);
			}
		}
	}

	gGuildManager.SetServer(lpMsg->GuildName, lpMsg->Name, lpMsg->server);
}

void CGuild::CGGuildListRecv(int aIndex)
{
	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	BYTE send[2048];

	PMSG_GUILD_LIST_SEND pMsg;

	pMsg.header.set(0x52, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	if (lpObj->GuildNumber != 0)
	{
		pMsg.TotalScore = lpObj->Guild->GuildScore;

		pMsg.score = lpObj->Guild->PlayScore;

		PMSG_GUILD_LIST info;

		for (int n = 0; n < MAX_GUILD_MEMBER; n++)
		{
			if (lpObj->Guild->Members[n].Use == false)
			{
				continue;
			}

			memcpy(info.name, lpObj->Guild->Members[n].Name, sizeof(info.name));

			info.number = lpObj->Guild->Members[n].Server;

			info.connected = (lpObj->Guild->Members[n].Server & 0x7F) | ((lpObj->Guild->Members[n].Server >= 0) ? GUILD_STATUS_MASTER : GUILD_STATUS_MEMBER);

			memcpy(&send[size], &info, sizeof(info));

			size += sizeof(info);

			pMsg.count++;
		}
	}

	pMsg.result = (pMsg.count > 0) ? 1 : 0;

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(aIndex, send, size);
}

void CGuild::GDGuildScoreUpdateSend(char* GuildName, int score)
{
	SDHP_GUILD_SCORE_UPDATE_SEND pMsg;

	pMsg.header.set(0x07, 0x06, sizeof(pMsg));

	pMsg.score = score;

	memcpy(pMsg.GuildName, GuildName, sizeof(pMsg.GuildName));

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void CGuild::DGGuildScoreUpdateRecv(SDHP_GUILD_SCORE_UPDATE_RECV* lpMsg)
{
	GUILD_INFO* lpGuild = gGuildManager.SearchGuild(lpMsg->GuildName);

	if (lpGuild != 0)
	{
		lpGuild->GuildScore = lpMsg->score;
	}
}

void CGuild::GDGuildGlobalChatSend(char* GuildName, char* Name, char* message)
{
	int size = strlen(message);

	size = ((size > MAX_CHAT_MESSAGE_SIZE) ? MAX_CHAT_MESSAGE_SIZE : size);

	SDHP_GUILD_GLOBAL_CHAT_SEND pMsg;

	pMsg.header.set(0x07, 0x07, sizeof(pMsg));

	memcpy(pMsg.GuildName, GuildName, sizeof(pMsg.GuildName));

	memcpy(pMsg.Name, Name, sizeof(pMsg.Name));

	memcpy(pMsg.message, message, size);

	pMsg.message[size] = 0;

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void CGuild::DGGuildGlobalChatRecv(SDHP_GUILD_GLOBAL_CHAT_RECV* lpMsg)
{
	GUILD_INFO* lpGuild = gGuildManager.SearchGuild(lpMsg->GuildName);

	if (lpGuild == 0)
	{
		return;
	}

	PMSG_CHAT_SEND pMsg;

	pMsg.header.set(0x00, sizeof(pMsg));

	memcpy(pMsg.name, lpMsg->Name, sizeof(pMsg.name));

	memcpy(pMsg.message, lpMsg->message, sizeof(pMsg.message));

	this->DataSendToAll(lpGuild, (BYTE*)&pMsg, pMsg.header.size);
}

void CGuild::DataSendToAll(GUILD_INFO* lpGuild, BYTE* lpMsg, int size)
{
	if (lpGuild == 0)
	{
		return;
	}

	for (int n = 0; n < MAX_GUILD_MEMBER; n++)
	{
		if (lpGuild->Members[n].Use != false && lpGuild->Members[n].Index >= 0)
		{
			if (gObjIsNameValid(lpGuild->Members[n].Index, lpGuild->Members[n].Name) != 0)
			{
				DataSend(lpGuild->Members[n].Index, lpMsg, size);
			}
		}
	}
}

void CGuild::GDGuildGlobalNoticeSend(char* GuildName, char* message)
{
	int size = strlen(message);

	size = ((size > MAX_CHAT_MESSAGE_SIZE) ? MAX_CHAT_MESSAGE_SIZE : size);

	SDHP_GUILD_GLOBAL_NOTICE_SEND pMsg;

	pMsg.header.set(0x07, 0x08, sizeof(pMsg));

	memcpy(pMsg.GuildName, GuildName, sizeof(pMsg.GuildName));

	memcpy(pMsg.message, message, size);

	pMsg.message[size] = 0;

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void CGuild::DGGuildGlobalNoticeRecv(SDHP_GUILD_GLOBAL_NOTICE_RECV* lpMsg)
{
	GUILD_INFO* lpGuild = gGuildManager.SearchGuild(lpMsg->GuildName);

	if (lpGuild == 0)
	{
		return;
	}

	memcpy(lpGuild->Notice, lpMsg->message, sizeof(lpGuild->Notice));

	for (int n = 0; n < MAX_GUILD_MEMBER; n++)
	{
		if (lpGuild->Members[n].Use != false && lpGuild->Members[n].Index >= 0)
		{
			if (gObjIsConnectedGP(lpGuild->Members[n].Index) == 0)
			{
				continue;
			}

			gNotice.GCNoticeSend(lpGuild->Members[n].Index, 2, "%s", lpMsg->message);
		}
	}
}

void CGuild::GuildWarRequestSend(int aIndex, char* GuildName, int type)
{
	if (gObjIsConnectedGP(aIndex) == false)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->GuildNumber == 0 || lpObj->Guild == 0)
	{
		this->GCGuildWarRequestResultSend(aIndex, 3);

		return;
	}

	if (lpObj->Guild->WarState == GUILD_WAR_STATE_DECLARE || lpObj->Guild->WarDeclareState == GUILD_WAR_STATE_DECLARE)
	{
		this->GCGuildWarRequestResultSend(aIndex, 4);

		return;
	}

	if (lpObj->PKLevel >= PKLVL_KILLER)
	{
		this->GCGuildWarRequestResultSend(aIndex, 4);

		return;
	}

	if (lpObj->GuildStatus != GUILD_STATUS_MASTER)
	{
		this->GCGuildWarRequestResultSend(aIndex, 5);

		return;
	}

	if (!strncmp(lpObj->Guild->Name, GuildName, 8))
	{
		return;
	}

	GUILD_INFO* lpInfo = gGuildManager.SearchGuild(GuildName);

	if (lpInfo == 0)
	{
		this->GCGuildWarRequestResultSend(aIndex, 0);

		return;
	}

	if (lpInfo->WarState == GUILD_WAR_STATE_DECLARE || lpInfo->WarDeclareState == GUILD_WAR_STATE_DECLARE)
	{
		this->GCGuildWarRequestResultSend(aIndex, 4);

		return;
	}

	LPOBJ lpTarget = gObjFind(lpInfo->Members[0].Name);

	if (lpTarget == 0)
	{
		this->GCGuildWarRequestResultSend(aIndex, 2);

		return;
	}

	if (DS_MAP_RANGE(lpObj->Map) != false || BC_MAP_RANGE(lpObj->Map) != false)
	{
		return;
	}

	if (DS_MAP_RANGE(lpTarget->Map) != false || BC_MAP_RANGE(lpTarget->Map) != false)
	{
		return;
	}

	if (lpTarget->PKLevel >= PKLVL_KILLER)
	{
		this->GCGuildWarRequestResultSend(aIndex, 4);

		return;
	}

	if ((lpTarget->Option & 1) != 1)
	{
		this->GCGuildWarRequestResultSend(aIndex, 6);

		return;
	}

	this->GCGuildWarRequestResultSend(aIndex, 1);

	this->GCGuildWarDeclareSend(lpTarget->Index, lpObj->Guild->Name, type);

	lpObj->Guild->WarDeclareState = GUILD_WAR_STATE_DECLARE;
	lpObj->Guild->WarType = type;
	lpObj->Guild->TargetGuildNode = lpInfo;

	lpInfo->WarDeclareState = GUILD_WAR_STATE_DECLARE;
	lpInfo->WarType = type;
	lpInfo->TargetGuildNode = lpObj->Guild;
}

void CGuild::CGGuildWarRequestResultRecv(PMSG_GUILD_WAR_REQUEST_RESULT_RECV* lpMsg, int aIndex)
{
	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	GUILD_INFO* lpGuild1 = lpObj->Guild;

	if (lpGuild1 == 0)
	{
		this->GCGuildWarRequestResultSend(aIndex, 0);

		return;
	}

	GUILD_INFO* lpGuild2 = lpObj->Guild->TargetGuildNode;

	if (lpGuild2 != 0 && lpMsg->result != 0)
	{
		LPOBJ lpTarget = &gObj[lpGuild2->Members[0].Index];

		if (BC_MAP_RANGE(lpTarget->Map) != 0 || DS_MAP_RANGE(lpTarget->Map) != 0)
		{
			lpMsg->result = 0;
		}
	}

	if (lpMsg->result == 0)
	{
		if (lpGuild2 != 0)
		{
			if (lpGuild2->WarDeclareState == GUILD_WAR_STATE_DECLARE)
			{
				lpGuild1->WarDeclareState = GUILD_WAR_STATE_NONE;

				lpGuild2->WarDeclareState = GUILD_WAR_STATE_NONE;

				if (lpGuild2->Members[0].Index >= 0)
				{
					this->GCGuildWarRequestResultSend(lpGuild2->Members[0].Index, 6);
				}
			}
		}
	}
	else
	{
		if (lpGuild1->WarDeclareState == GUILD_WAR_STATE_DECLARE)
		{
			if (lpGuild2 != 0)
			{
				if (lpGuild2->WarDeclareState == GUILD_WAR_STATE_DECLARE)
				{
					if (lpGuild1->WarType == GUILD_WAR_TYPE_SOCCER)
					{
						lpGuild1->BattleGroundIndex = gBattleSoccer.CheckBlankBattleGround();

						if (lpGuild1->BattleGroundIndex == 255)
						{
							lpGuild1->WarDeclareState = GUILD_WAR_STATE_NONE;
							lpGuild1->WarState = GUILD_WAR_STATE_NONE;

							lpGuild2->WarDeclareState = GUILD_WAR_STATE_NONE;
							lpGuild2->WarState = GUILD_WAR_STATE_NONE;

							this->GCGuildWarRequestResultSend(aIndex, 4);

							return;
						}

						lpGuild2->BattleGroundIndex = lpGuild1->BattleGroundIndex;

						if (gBattleSoccer.gBSGround[0]->m_BallIndex >= 0)
						{
							gObjMonsterRegen(&gObj[gBattleSoccer.gBSGround[0]->m_BallIndex]);
						}

						gBattleSoccer.BattleSoccerGoalEnd(0);

						lpGuild1->PlayScore = 0;
						lpGuild2->PlayScore = 0;

						gBattleSoccer.BattleGroundEnable(lpGuild1->BattleGroundIndex, 1);

						gBattleSoccer.SetBattleTeamMaster(lpGuild1->BattleGroundIndex, 0, lpGuild1->Name, lpGuild1);

						gBattleSoccer.SetBattleTeamMaster(lpGuild1->BattleGroundIndex, 1, lpGuild2->Name, lpGuild2);
					}

					lpGuild1->WarDeclareState = GUILD_WAR_STATE_ACTIVE;
					lpGuild1->WarState = GUILD_WAR_STATE_DECLARE;
					lpGuild1->PlayScore = 0;
					lpGuild1->BattleTeamCode = GUILD_WAR_TEAM_BLUE;

					lpGuild2->WarDeclareState = GUILD_WAR_STATE_ACTIVE;
					lpGuild2->WarState = GUILD_WAR_STATE_DECLARE;
					lpGuild2->PlayScore = 0;
					lpGuild2->BattleTeamCode = GUILD_WAR_TEAM_RED;

					for (int index = 0; index < MAX_GUILD_MEMBER; index++)
					{
						if (lpGuild1->Members[index].Use != false && lpGuild1->Members[index].Index >= 0)
						{
							if (gObjIsNameValid(lpGuild1->Members[index].Index, lpGuild1->Members[index].Name) != 0)
							{
								if (lpGuild1->WarType == GUILD_WAR_TYPE_SOCCER && index != 0)
								{
									if (gObj[lpGuild1->Members[0].Index].PartyNumber >= 0)
									{
										if (gObj[lpGuild1->Members[0].Index].PartyNumber == gObj[lpGuild1->Members[index].Index].PartyNumber)
										{
											this->GuildWarDeclareSet(lpGuild1->Members[index].Index, lpGuild2->Name, lpGuild1->WarType, lpGuild1->BattleTeamCode);
										}
									}
								}
								else
								{
									this->GuildWarDeclareSet(lpGuild1->Members[index].Index, lpGuild2->Name, lpGuild1->WarType, lpGuild1->BattleTeamCode);
								}
							}
						}

						if (lpGuild2->Members[index].Use != false && lpGuild2->Members[index].Index >= 0)
						{
							if (gObjIsNameValid(lpGuild2->Members[index].Index, lpGuild2->Members[index].Name) != 0)
							{
								if (lpGuild1->WarType == GUILD_WAR_TYPE_SOCCER && index != 0)
								{
									if (gObj[lpGuild2->Members[0].Index].PartyNumber >= 0)
									{
										if (gObj[lpGuild2->Members[0].Index].PartyNumber == gObj[lpGuild2->Members[index].Index].PartyNumber)
										{
											this->GuildWarDeclareSet(lpGuild2->Members[index].Index, lpGuild1->Name, lpGuild1->WarType, lpGuild2->BattleTeamCode);
										}
									}
								}
								else
								{
									this->GuildWarDeclareSet(lpGuild2->Members[index].Index, lpGuild1->Name, lpGuild1->WarType, lpGuild2->BattleTeamCode);
								}
							}
						}
					}

					if (lpGuild1->WarType == GUILD_WAR_TYPE_SOCCER)
					{
						gObjAddMsgSendDelay(&gObj[aIndex], 5, aIndex, 10000, 0);

						this->NoticeSendToAll(lpGuild1, 1, 23);

						this->NoticeSendToAll(lpGuild2, 1, 23);
					}
				}
			}
		}
	}
}

void CGuild::GuildWarDeclareSet(int aIndex, char* GuildName, int type, int team)
{
	this->GCGuildWarStartSend(aIndex, GuildName, type, team);

	this->GCGuildWarScoreSend(aIndex);

	if (type == GUILD_WAR_TYPE_SOCCER)
	{
		gObj[aIndex].IsInBattleGround = 1;

		if (gObj[aIndex].PKLevel < PKLVL_KILLER)
		{
			gObjTeleport(aIndex, 6, 60, 153);
		}
	}
}

void CGuild::GCGuildWarRequestResultSend(int aIndex, int result)
{
	PMSG_GUILD_WAR_REQUEST_RESULT_SEND pMsg;

	pMsg.header.set(0x60, sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CGuild::GCGuildWarDeclareSend(int aIndex, char* GuildName, int type)
{
	PMSG_GUILD_WAR_DECLARE_SEND pMsg;

	pMsg.header.set(0x61, sizeof(pMsg));

	memcpy(pMsg.GuildName, GuildName, sizeof(pMsg.GuildName));

	pMsg.type = type;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CGuild::GCGuildWarStartSend(int aIndex, char* GuildName, int type, int team)
{
	PMSG_GUILD_WAR_START_SEND pMsg;

	pMsg.header.set(0x62, sizeof(pMsg));

	memcpy(pMsg.GuildName, GuildName, sizeof(pMsg.GuildName));

	pMsg.type = type;

	pMsg.team = team;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CGuild::GCGuildWarScoreSend(int aIndex)
{
	if (gObj[aIndex].GuildNumber == 0)
	{
		return;
	}

	PMSG_GUILD_WAR_SCORE_SEND pMsg;

	pMsg.header.set(0x64, sizeof(pMsg));

	pMsg.score[0] = ((gObj[aIndex].Guild == 0) ? 0 : gObj[aIndex].Guild->PlayScore);

	pMsg.score[1] = ((gObj[aIndex].Guild->TargetGuildNode == 0) ? 0 : gObj[aIndex].Guild->TargetGuildNode->PlayScore);

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CGuild::NoticeSendToAll(GUILD_INFO* lpGuild, int type, int message, ...)
{
	if (lpGuild == 0)
	{
		return;
	}

	char buff[256];

	va_list arg;

	for (int n = 0; n < MAX_GUILD_MEMBER; n++)
	{
		if (lpGuild->Members[n].Use != false && lpGuild->Members[n].Index >= 0)
		{
			if (gObjIsNameValid(lpGuild->Members[n].Index, lpGuild->Members[n].Name) != 0)
			{
				va_start(arg, message);

				vsprintf_s(buff, gMessage.GetTextMessage(message, gObj[lpGuild->Members[n].Index].Lang), arg);

				va_end(arg);

				gNotice.GCNoticeSend(lpGuild->Members[n].Index, type, buff);
			}
		}
	}
}

bool CGuild::GuildWarCheck(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if (this->GuildWarStateCheck(lpObj, lpTargetObj) == false)
	{
		return false;
	}

	int score = 1;

	if (strcmp(lpTargetObj->Name, lpTargetObj->Guild->Members[0].Name) == 0)
	{
		score = 2;
	}

	GUILD_INFO* lpGuild = lpObj->Guild;

	GUILD_INFO* lpTargetGuild = lpTargetObj->Guild;

	if (this->GuildWarScoreCheck(lpGuild, lpTargetGuild, score) != false)
	{
		if (lpGuild->WarType == GUILD_WAR_TYPE_SOCCER)
		{
			this->GuildWarEndSend(lpGuild, lpTargetGuild, 1, 0);

			gBattleSoccer.BattleGroundEnable(lpGuild->BattleGroundIndex, 0);

			this->GuildWarStateEnd(lpGuild, lpTargetGuild);
		}
		else
		{
			this->GuildWarEndSend(lpObj, 1, 0);

			gObjAddMsgSendDelay(lpObj, 4, lpObj->Index, 2000, 0);
		}
	}

	return true;
}

bool CGuild::GuildWarStateCheck(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if (lpObj->Type != OBJECT_USER)
	{
		return false;
	}

	if (lpTargetObj->Type != OBJECT_USER)
	{
		return false;
	}

	if (lpObj->GuildNumber < 1 || lpTargetObj->GuildNumber < 1)
	{
		return false;
	}

	if (lpObj->Guild->WarState != GUILD_WAR_STATE_DECLARE || lpTargetObj->Guild->WarState != GUILD_WAR_STATE_DECLARE)
	{
		return false;
	}

	if (strcmp(lpObj->Guild->TargetGuildNode->Name, lpTargetObj->Guild->Name) != 0)
	{
		return false;
	}

	return true;
}

bool CGuild::GuildWarScoreCheck(GUILD_INFO* lpGuild1, GUILD_INFO* lpGuild2, int score)
{
	if (lpGuild1 == 0 || lpGuild2 == 0)
	{
		return false;
	}

	int totalscore = gServerInfo.m_GuildWarWinPointsGain_Normal;

	int maxscore = gServerInfo.m_GuildWarWinScore;

	lpGuild1->PlayScore += score;

	LogAdd(LOG_BLACK, "%s ( %d ) VS %s ( %d )", lpGuild1->Name, lpGuild1->PlayScore, lpGuild2->Name, lpGuild2->PlayScore);

	if (lpGuild1->WarType == GUILD_WAR_TYPE_SOCCER)
	{
		gBattleSoccer.SetBattleTeamScore(lpGuild1->BattleGroundIndex, lpGuild1->BattleTeamCode, lpGuild1->PlayScore);

		maxscore = gServerInfo.m_GuildWarSoccerWinScore;
	}
	else
	{
		maxscore = gServerInfo.m_GuildWarWinScore;
	}

	if (lpGuild1->PlayScore >= maxscore)
	{
		if (lpGuild1->PlayScore >= maxscore && lpGuild2->PlayScore == 0)
		{
			totalscore = gServerInfo.m_GuildWarWinPointsGain_Max;
		}
		else if (lpGuild1->PlayScore >= maxscore && lpGuild2->PlayScore <= (maxscore / 2))
		{
			totalscore = gServerInfo.m_GuildWarWinPointsGain_Half;
		}

		lpGuild1->GuildScore += totalscore;

		this->GDGuildScoreUpdateSend(lpGuild1->Name, lpGuild1->GuildScore);

		this->GDGuildScoreUpdateSend(lpGuild2->Name, lpGuild2->GuildScore);

		return true;
	}

	for (int n = 0; n < MAX_GUILD_MEMBER; n++)
	{
		if (lpGuild1->Members[n].Use != false && lpGuild1->Members[n].Index >= 0)
		{
			this->GCGuildWarScoreSend(lpGuild1->Members[n].Index);
		}

		if (lpGuild2->Members[n].Use != false && lpGuild2->Members[n].Index >= 0)
		{
			this->GCGuildWarScoreSend(lpGuild2->Members[n].Index);
		}
	}

	return false;
}

bool CGuild::GuildMasterCloseCheck(LPOBJ lpObj)
{
	if (lpObj->GuildNumber < 1)
	{
		return false;
	}

	if (strcmp(lpObj->Guild->Members[0].Name, lpObj->Name) != 0)
	{
		return false;
	}

	this->GuildWarEndSend(lpObj, 3, 2);

	if (lpObj->Guild->WarType == GUILD_WAR_TYPE_SOCCER && lpObj->Guild->BattleGroundIndex < 1)
	{
		gBattleSoccer.BattleGroundEnable(lpObj->Guild->BattleGroundIndex, 0);
	}

	this->GuildWarStateEnd(lpObj->Guild, lpObj->Guild->TargetGuildNode);

	return true;
}

void CGuild::GuildWarStateEnd(GUILD_INFO* lpGuild, GUILD_INFO* lpTargetGuild)
{
	if (lpGuild == 0 || lpTargetGuild == 0)
	{
		return;
	}

	lpGuild->WarDeclareState = 0;

	lpGuild->WarState = GUILD_WAR_STATE_NONE;

	if (lpTargetGuild != 0)
	{
		lpTargetGuild->WarDeclareState = 0;

		lpTargetGuild->WarState = GUILD_WAR_STATE_NONE;

		if (lpGuild->TargetGuildNode != 0)
		{
			GUILD_INFO* lpTguild = lpGuild->TargetGuildNode;

			lpGuild->TargetGuildNode = 0;

			lpTguild->TargetGuildNode = 0;
		}
	}
	else
	{
		lpGuild->TargetGuildNode = 0;
	}
}

void CGuild::GuildWarEndSend(LPOBJ lpObj, BYTE Result1, BYTE Result2)
{
	if (lpObj->Guild->WarState != GUILD_WAR_STATE_DECLARE)
	{
		return;
	}

	if (lpObj->Guild->TargetGuildNode == 0)
	{
		return;
	}

	for (int n = 0; n < MAX_GUILD_MEMBER; n++)
	{
		if (lpObj->Guild->Members[n].Use != false && lpObj->Guild->Members[n].Index >= 0)
		{
			gObj[lpObj->Guild->Members[n].Index].IsInBattleGround = 0;

			this->GCGuildWarEndSend(lpObj->Guild->Members[n].Index, Result1, lpObj->Guild->TargetGuildNode->Name);
		}

		if (lpObj->Guild->TargetGuildNode->Members[n].Use != false && lpObj->Guild->TargetGuildNode->Members[n].Index >= 0)
		{
			gObj[lpObj->Guild->TargetGuildNode->Members[n].Index].IsInBattleGround = 0;

			this->GCGuildWarEndSend(lpObj->Guild->TargetGuildNode->Members[n].Index, Result2, lpObj->Guild->Name);
		}
	}
}

void CGuild::GuildWarEndSend(GUILD_INFO* lpGuild1, GUILD_INFO* lpGuild2, BYTE Result1, BYTE Result2)
{
	if (lpGuild1 == 0 || lpGuild2 == 0)
	{
		return;
	}

	if (lpGuild1->WarState != GUILD_WAR_STATE_DECLARE || lpGuild2->WarState != GUILD_WAR_STATE_DECLARE)
	{
		return;
	}

	for (int n = 0; n < MAX_GUILD_MEMBER; n++)
	{
		if (lpGuild1->Members[n].Use != false && lpGuild1->Members[n].Index >= 0)
		{
			gObj[lpGuild1->Members[n].Index].IsInBattleGround = 0;

			this->GCGuildWarEndSend(lpGuild1->Members[n].Index, Result1, lpGuild2->Name);
		}

		if (lpGuild2->Members[n].Use != false && lpGuild2->Members[n].Index >= 0)
		{
			gObj[lpGuild2->Members[n].Index].IsInBattleGround = 0;

			this->GCGuildWarEndSend(lpGuild2->Members[n].Index, Result2, lpGuild1->Name);
		}
	}
}

void CGuild::GCGuildWarEndSend(int aIndex, int result, char* GuildName)
{
	PMSG_GUILD_WAR_END_SEND pMsg;

	pMsg.header.set(0x63, sizeof(pMsg));

	pMsg.result = result;

	memcpy(pMsg.GuildName, GuildName, sizeof(pMsg.GuildName));

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}