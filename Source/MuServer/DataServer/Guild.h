#pragma once

#include "ProtocolDefines.h"

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_GUILD_CREATE_RECV
{
	PSBMSG_HEAD header; // C1:07:00
	WORD index;
	char GuildName[9];
	char Name[11];
	BYTE Mark[32];
};

struct SDHP_GUILD_DELETE_RECV
{
	PSBMSG_HEAD header; // C1:07:01
	WORD index;
	char GuildName[9];
	char Name[11];
};

struct SDHP_GUILD_MEMBER_DISCONNECT_RECV
{
	PSBMSG_HEAD header; // C1:07:02
	char GuildName[9];
	char Name[11];
};

struct SDHP_GUILD_MEMBER_JOIN_RECV
{
	PSBMSG_HEAD header; // C1:07:03
	WORD index;
	char GuildName[9];
	char Name[11];
};

struct SDHP_GUILD_MEMBER_DELETE_RECV
{
	PSBMSG_HEAD header; // C1:07:04
	WORD index;
	char GuildName[9];
	char Name[11];
};

struct SDHP_GUILD_MEMBER_GET_INFO_RECV
{
	PSBMSG_HEAD header; // C1:07:05
	char Name[11];
};

struct SDHP_GUILD_SCORE_UPDATE_RECV
{
	PSBMSG_HEAD header; // C1:07:06
	char GuildName[9];
	DWORD score;
};

struct SDHP_GUILD_GLOBAL_CHAT_RECV
{
	PSBMSG_HEAD header; // C1:07:07
	char GuildName[9];
	char Name[11];
	char message[60];
};

struct SDHP_GUILD_GLOBAL_NOTICE_RECV
{
	PSBMSG_HEAD header; // C1:07:08
	char GuildName[9];
	char message[60];
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_GUILD_CREATE_SEND
{
	PSBMSG_HEAD header; // C1:07:00
	BYTE result;
	BYTE flag;
	WORD index;
	DWORD GuildNumber;
	char GuildName[9];
	char Name[11];
	BYTE Mark[32];
};

struct SDHP_GUILD_DELETE_SEND
{
	PSBMSG_HEAD header; // C1:07:01
	BYTE result;
	BYTE flag;
	WORD index;
	char GuildName[9];
	char Name[11];
};

struct SDHP_GUILD_MEMBER_DISCONNECT_SEND
{
	PSBMSG_HEAD header; // C1:07:02
	char GuildName[9];
	char Name[11];
};

struct SDHP_GUILD_MEMBER_JOIN_SEND
{
	PSBMSG_HEAD header; // C1:07:03
	BYTE result;
	BYTE flag;
	WORD index;
	char GuildName[9];
	char Name[11];
	WORD server;
};

struct SDHP_GUILD_MEMBER_DELETE_SEND
{
	PSBMSG_HEAD header; // C1:07:04
	BYTE result;
	BYTE flag;
	WORD index;
	char GuildName[9];
	char Name[11];
};

struct SDHP_GUILD_MEMBER_INFO_SEND
{
	PSBMSG_HEAD header; // C1:07:05
	char GuildName[9];
	char Name[11];
	BYTE status;
	WORD server;
};

struct SDHP_GUILD_SCORE_UPDATE_SEND
{
	PSBMSG_HEAD header; // C1:07:06
	char GuildName[9];
	DWORD score;
};

struct SDHP_GUILD_GLOBAL_CHAT_SEND
{
	PSBMSG_HEAD header; // C1:07:07
	char GuildName[9];
	char Name[11];
	char message[60];
};

struct SDHP_GUILD_GLOBAL_NOTICE_SEND
{
	PSBMSG_HEAD header; // C1:07:08
	char GuildName[9];
	char message[60];
};

struct SDHP_GUILD_INFO_SEND
{
	PSWMSG_HEAD header; // C2:07:09
	DWORD GuildNumber;
	char GuildName[9];
	char GuildMaster[11];
	BYTE Mark[32];
	char Notice[60];
	DWORD score;
	BYTE count;
};

struct SDHP_GUILD_MEMBER_INFO
{
	char Name[11];
	BYTE status;
	WORD server;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CGuild
{
public:

	CGuild();

	~CGuild();

	void GDGuildCreateRecv(SDHP_GUILD_CREATE_RECV* lpMsg, int aIndex);

	void DGGuildInfoSend(DWORD GuildNumber);

	void DGGuildMemberInfoSend(char* GuildName, char* Name, BYTE status, WORD server);

	void GDGuildDeleteRecv(SDHP_GUILD_DELETE_RECV* lpMsg, int aIndex);

	void GDGuildMemberDisconnectRecv(SDHP_GUILD_MEMBER_DISCONNECT_RECV* lpMsg);

	void GDGuildMemberJoinRecv(SDHP_GUILD_MEMBER_JOIN_RECV* lpMsg, int aIndex);

	void GDGuildMemberDeleteRecv(SDHP_GUILD_MEMBER_DELETE_RECV* lpMsg, int aIndex);

	void GDGuildMemberGetInfoRecv(SDHP_GUILD_MEMBER_GET_INFO_RECV* lpMsg, int aIndex);

	void GDGuildScoreUpdateRecv(SDHP_GUILD_SCORE_UPDATE_RECV* lpMsg);

	void GDGuildGlobalChatRecv(SDHP_GUILD_GLOBAL_CHAT_RECV* lpMsg);

	void GDGuildGlobalNoticeRecv(SDHP_GUILD_GLOBAL_NOTICE_RECV* lpMsg);
};

extern CGuild gGuild;