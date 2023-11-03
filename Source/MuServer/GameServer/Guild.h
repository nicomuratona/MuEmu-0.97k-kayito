#pragma once

#include "User.h"
#include "Protocol.h"

enum eGuildWarTeam
{
	GUILD_WAR_TEAM_BLUE = 0,
	GUILD_WAR_TEAM_RED = 1,
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_GUILD_CREATE_RECV
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

struct SDHP_GUILD_DELETE_RECV
{
	PSBMSG_HEAD header; // C1:07:01
	BYTE result;
	BYTE flag;
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
	BYTE result;
	BYTE flag;
	WORD index;
	char GuildName[9];
	char Name[11];
	WORD server;
};

struct SDHP_GUILD_MEMBER_DELETE_RECV
{
	PSBMSG_HEAD header; // C1:07:04
	BYTE result;
	BYTE flag;
	WORD index;
	char GuildName[9];
	char Name[11];
};

struct SDHP_GUILD_MEMBER_INFO_RECV
{
	PSBMSG_HEAD header; // C1:07:05
	char GuildName[9];
	char Name[11];
	BYTE status;
	WORD server;
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

struct SDHP_GUILD_INFO_RECV
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
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_GUILD_CREATE_SEND
{
	PSBMSG_HEAD header; // C1:07:00
	WORD index;
	char GuildName[9];
	char Name[11];
	BYTE Mark[32];
};

struct SDHP_GUILD_DELETE_SEND
{
	PSBMSG_HEAD header; // C1:07:01
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
	WORD index;
	char GuildName[9];
	char Name[11];
};

struct SDHP_GUILD_MEMBER_DELETE_SEND
{
	PSBMSG_HEAD header; // C1:07:04
	WORD index;
	char GuildName[9];
	char Name[11];
};

struct SDHP_GUILD_MEMBER_GET_INFO_SEND
{
	PSBMSG_HEAD header; // C1:07:05
	char Name[11];
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

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_GUILD_REQUEST_RECV
{
	PBMSG_HEAD header; // C1:50
	BYTE index[2];
};

struct PMSG_GUILD_RESULT_RECV
{
	PBMSG_HEAD header; // C1:51
	BYTE result;
	BYTE index[2];
};

struct PMSG_GUILD_DELETE_RECV
{
	PBMSG_HEAD header; // C1:53
	char name[10];
	char PersonalCode[10];
};

struct PMSG_GUILD_MASTER_OPEN_RECV
{
	PBMSG_HEAD header; // C1:54
	BYTE result;
};

struct PMSG_GUILD_CREATE_RECV
{
	PBMSG_HEAD header; // C1:55
	char GuildName[8];
	BYTE Mark[32];
};

struct PMSG_GUILD_WAR_REQUEST_RESULT_RECV
{
	PBMSG_HEAD header; // C1:61
	BYTE result;
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_GUILD_REQUEST_SEND
{
	PBMSG_HEAD header; // C1:50
	BYTE index[2];
};

struct PMSG_GUILD_RESULT_SEND
{
	PBMSG_HEAD header; // C1:51
	BYTE result;
};

struct PMSG_GUILD_LIST_SEND
{
	PWMSG_HEAD header; // C2:52
	BYTE result;
	BYTE count;
	DWORD TotalScore;
	BYTE score;
};

struct PMSG_GUILD_LIST
{
	char name[10];
	BYTE number;
	BYTE connected;
};

struct PMSG_GUILD_DELETE_SEND
{
	PBMSG_HEAD header; // C1:53
	BYTE result;
};

struct PMSG_GUILD_CREATE_RESULT_SEND
{
	PBMSG_HEAD header; // C1:56
	BYTE result;
};

struct PMSG_GUILD_VIEWPORT_DELETE_SEND
{
	PBMSG_HEAD header; // C1:5D
	BYTE index[2];
};

struct PMSG_GUILD_WAR_REQUEST_RESULT_SEND
{
	PBMSG_HEAD header; // C1:60
	BYTE result;
};

struct PMSG_GUILD_WAR_DECLARE_SEND
{
	PBMSG_HEAD header; // C1:61
	char GuildName[8];
	BYTE type;
};

struct PMSG_GUILD_WAR_START_SEND
{
	PBMSG_HEAD header; // C1:62
	char GuildName[8];
	BYTE type;
	BYTE team;
};

struct PMSG_GUILD_WAR_END_SEND
{
	PBMSG_HEAD header; // C1:63
	BYTE result;
	char GuildName[8];
};

struct PMSG_GUILD_WAR_SCORE_SEND
{
	PBMSG_HEAD header; // C1:64
	BYTE score[2];
};

//**********************************************//
//**********************************************//
//**********************************************//

class CGuild
{
public:

	CGuild();

	~CGuild();

	// CREATE GUILD

	void CGGuildMasterOpenRecv(PMSG_GUILD_MASTER_OPEN_RECV* lpMsg, int aIndex); // From Client 0x54 | To Client 0x55

	void CGGuildMasterCancelRecv(int aIndex); // From Client 0x57

	void CGGuildCreateRecv(PMSG_GUILD_CREATE_RECV* lpMsg, int aIndex); // From client 0x55 | To DataServer 0x07, 0x00

	void DGGuildCreateRecv(SDHP_GUILD_CREATE_RECV* lpMsg); // From DataServer 0x07, 0x00

	void GCGuildCreateResultSend(int aIndex, int result); // To Client 0x56

	// DELETE GUILD

	void CGGuildDeleteRecv(PMSG_GUILD_DELETE_RECV* lpMsg, int aIndex); // From Client 0x53 | To DataServer 0x07, 0x01

	void DGGuildDeleteRecv(SDHP_GUILD_DELETE_RECV* lpMsg); // From DataServer 0x07, 0x01

	void GCGuildDeleteSend(int aIndex, int result); // To Client 0x53

	// DISCONNECT MEMBER

	void GDGuildMemberDisconnectSend(int aIndex, char* GuildName, char* Name); // To DataServer 0x07, 0x02

	void DGGuildMemberDisconnectRecv(SDHP_GUILD_MEMBER_DISCONNECT_RECV* lpMsg); // From DataServer 0x07, 0x02

	// ADD MEMBER

	void CGGuildRequestRecv(PMSG_GUILD_REQUEST_RECV* lpMsg, int aIndex); // From Client 0x50 | To Client 0x50

	void CGGuildResultRecv(PMSG_GUILD_RESULT_RECV* lpMsg, int aIndex); // From Client 0x51

	int GetMaxGuildUser(LPOBJ lpObj); // Support

	void GDGuildMemberJoinSend(int aIndex, char* GuildName, char* Name); // To DataServer 0x07, 0x03

	void DGGuildMemberJoinRecv(SDHP_GUILD_MEMBER_JOIN_RECV* lpMsg); // From DataServer 0x07, 0x03

	void GCGuildResultSend(int aIndex, int result); // To Client 0x51

	// DELETE MEMBER

	void GDGuildMemberDeleteSend(int aIndex, char* GuildName, char* Name); // To DataServer 0x07, 0x04

	void DGGuildMemberDeleteRecv(SDHP_GUILD_MEMBER_DELETE_RECV* lpMsg); // From DataServer 0x07, 0x04

	void GCGuildViewportDeleteSend(int aIndex); // To Client 0x5D

	// GET GUILD AND MEMBER INFO

	void GDGuildMemberGetInfoSend(int aIndex, char* Name); // To DataServer 0x07, 0x05

	void DGGuildInfoRecv(SDHP_GUILD_INFO_RECV* lpMsg); // From DataServer 0x07, 0x09

	void DGGuildMemberInfoRecv(SDHP_GUILD_MEMBER_INFO_RECV* lpMsg); // From DataServer 0x07, 0x05

	void CGGuildListRecv(int aIndex); // From Client 0x52 | To Client 0x52

	// SCORE UPDATE

	void GDGuildScoreUpdateSend(char* GuildName, int score); // To DataServer 0x07, 0x06

	void DGGuildScoreUpdateRecv(SDHP_GUILD_SCORE_UPDATE_RECV* lpMsg); // From DataServer 0x07, 0x06

	// GLOBAL CHAT

	void GDGuildGlobalChatSend(char* GuildName, char* Name, char* message); // To DataServer 0x07, 0x07

	void DGGuildGlobalChatRecv(SDHP_GUILD_GLOBAL_CHAT_RECV* lpMsg); // From DataServer 0x07, 0x07 | To Client 0x00

	void DataSendToAll(GUILD_INFO* lpGuild, BYTE* lpMsg, int size); // Support

	// GLOBAL NOTICE

	void GDGuildGlobalNoticeSend(char* GuildName, char* message); // To DataServer 0x07, 0x08

	void DGGuildGlobalNoticeRecv(SDHP_GUILD_GLOBAL_NOTICE_RECV* lpMsg); // From DataServer 0x07, 0x08

	// START GUILD WAR / BATTLE SOCCER

	void GuildWarRequestSend(int aIndex, char* GuildName, int type);
	
	void CGGuildWarRequestResultRecv(PMSG_GUILD_WAR_REQUEST_RESULT_RECV* lpMsg, int aIndex); // From Client 0x61

	void GuildWarDeclareSet(int aIndex, char* GuildName, int type, int team); // Support

	void GCGuildWarRequestResultSend(int aIndex, int result); // To Client 0x60

	void GCGuildWarDeclareSend(int aIndex, char* GuildName, int type);

	void GCGuildWarStartSend(int aIndex, char* GuildName, int type, int team); // To Client 0x62

	void GCGuildWarScoreSend(int aIndex); // To Client 0x64

	void NoticeSendToAll(GUILD_INFO* lpLevel, int type, int message, ...); // Support

	// DURING GUILD WAR / BATTLE SOCCER

	bool GuildWarCheck(LPOBJ lpObj, LPOBJ lpTargetObj);  // Support

	bool GuildWarStateCheck(LPOBJ lpObj, LPOBJ lpTargetObj);  // Support

	bool GuildWarScoreCheck(GUILD_INFO* lpGuild1, GUILD_INFO* lpGuild2, int score);  // Support

	// END GUILD WAR / BATTLE SOCCER

	bool GuildMasterCloseCheck(LPOBJ lpObj); // Support

	void GuildWarStateEnd(GUILD_INFO* lpGuild, GUILD_INFO* lpTargetGuild); // Support

	void GuildWarEndSend(LPOBJ lpObj, BYTE Result1, BYTE Result2); // Support

	void GuildWarEndSend(GUILD_INFO* lpGuild1, GUILD_INFO* lpGuild2, BYTE Result1, BYTE Result2); // Support

	void GCGuildWarEndSend(int aIndex, int result, char* GuildName); // To Client 0x63
};

extern CGuild gGuild;