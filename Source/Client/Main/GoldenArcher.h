#pragma once

#include "ProtocolDefines.h"

enum eGoldenArcherInterfaceTypes
{
	GOLDEN_ARCHER_INTERFACE_NONE = 0,
	GOLDEN_ARCHER_INTERFACE_RENA = 1,
	GOLDEN_ARCHER_INTERFACE_STONE = 2,
	GOLDEN_ARCHER_INTERFACE_LUCKY_NUMBER = 3,
	GOLDEN_ARCHER_INTERFACE_BINGO = 4,
	MAX_GOLDEN_ARCHER_INTERFACE_TYPES
};

enum eGoldenArcherTypes
{
	GOLDEN_ARCHER_RENA_COLLECTOR = 0,
	GOLDEN_ARCHER_STONE_COLLECTOR = 1,
	GOLDEN_ARCHER_LUCKY_NUMBER_EXCHANGE = 2,
	GOLDEN_ARCHER_BINGO_LIST = 3,
};

enum eGoldenArcherRenaExchangeType
{
	GOLDEN_ARCHER_EXCHANGE_RENA1 = 0,
	GOLDEN_ARCHER_EXCHANGE_RENA2 = 1,
	GOLDEN_ARCHER_EXCHANGE_RENA3 = 2,
	GOLDEN_ARCHER_EXCHANGE_RENA4 = 3,
	GOLDEN_ARCHER_MAX_EXCHANGE_RENAS
};

enum eGoldenArcherStoneExchangeType
{
	GOLDEN_ARCHER_EXCHANGE_STONE1 = 0,
	GOLDEN_ARCHER_EXCHANGE_STONE2 = 1,
	GOLDEN_ARCHER_EXCHANGE_STONE3 = 2,
	GOLDEN_ARCHER_EXCHANGE_STONE4 = 3,
	GOLDEN_ARCHER_MAX_EXCHANGE_STONES
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_NPC_GOLDEN_ARCHER_RECV
{
	PBMSG_HEAD header; // C1:94
	BYTE Type;
	short Count;
	char LuckyNumber[13];
};

struct PMSG_GOLDEN_ARCHER_REGISTER_RECV
{
	PBMSG_HEAD header; // C1:95
	short Count;
};

struct PMSG_GOLDEN_ARCHER_LUCKY_NUMBER_RECV
{
	PBMSG_HEAD header; // C1:96
	char LuckyNumber[13];
};

struct PMSG_GOLDEN_ARCHER_INFO_RECV
{
	PSBMSG_HEAD header; // C1:97:00
	int RenaExchangeRequiredCount[GOLDEN_ARCHER_MAX_EXCHANGE_RENAS];
	int StoneExchangeRequiredCount[GOLDEN_ARCHER_MAX_EXCHANGE_STONES];
	DWORD StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE4];
};

struct PMSG_GOLDEN_ARCHER_LIST_RECV
{
	PSWMSG_HEAD header; // C1:97:01
	int count;
};

struct LUCKY_NUMBER_INFO
{
	char LuckyNumber[13];
};

struct PMSG_GOLDEN_ARCHER_REGISTER_LUCKY_RECV
{
	PSBMSG_HEAD header; // C1:97:03
	BYTE Result;
};

struct PMSG_GOLDEN_ARCHER_EXCHANGE_LUCKY_RECV
{
	PBMSG_HEAD header; // C1:9D
	BYTE Result;
};

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_GOLDEN_ARCHER_REGISTER_SEND
{
	PBMSG_HEAD header; // C1:95
	BYTE Type;
};

struct PMSG_GOLDEN_ARCHER_EXCHANGE_SEND
{
	PSBMSG_HEAD header; // C1:97:02
	BYTE Type;
	short Count;
};

struct PMSG_GOLDEN_ARCHER_REGISTER_LUCKY_SEND
{
	PSBMSG_HEAD header; // C1:97:03
	char LuckyNumber[13];
};

struct PMSG_GOLDEN_ARCHER_EXCHANGE_LUCKY_SEND
{
	PBMSG_HEAD header; // C1:9D
	char LuckyNumber[13];
};

//**********************************************//
//**********************************************//
//**********************************************//

class CGoldenArcher
{
public:

	CGoldenArcher();

	virtual ~CGoldenArcher();

	void Init();

private: // Interfaces

	static void Render();

	static void UpdateMouse();

	void RenderFrame();

	void RenderTitle();

	void RenderRenaCollector();
	bool CheckRenaCollector();

	void RenderStoneCollector();
	void RenderStoneCollector_Page1(int iPosX, int iPosY, float fBoxWidth, float fBoxHeight);
	void RenderStoneCollector_Page2(int iPosX, int iPosY, float fBoxWidth, float fBoxHeight);
	bool CheckStoneCollector();
	bool CheckStoneCollector_Page1(int iPosX, int iPosY, int iBoxWidth, int iBoxHeight);
	bool CheckStoneCollector_Page2(int iPosX, int iPosY, int iBoxWidth, int iBoxHeight);

	void RenderLuckyNumberExchanger();
	bool CheckLuckyNumberExchanger();

	void RenderLuckyNumberList();
	bool CheckLuckyNumberList();

	void RenderCloseButton();
	bool CheckCloseButton();

	void RenderBox(float PosX, float PosY, float Width, float Height, const char* Text, bool Hoverable = true);

	void RenderPagination();
	bool CheckPagination();

	static void MoveCharacter_GoldenArcherMovement();

	static void MoveCharacter_GoldenArcherChatMessages();

	static void SendMove_GoldenArcherFixClose();

public: // Protocols

	void GCGoldenArcherInfoRecv(PMSG_GOLDEN_ARCHER_INFO_RECV* lpMsg);

	void GCGoldenArcherRecv(PMSG_NPC_GOLDEN_ARCHER_RECV* lpMsg);
	void GCGoldenArcherListRecv(PMSG_GOLDEN_ARCHER_LIST_RECV* lpMsg);

	void CGGoldenArcherRegisterSend(int Type);
	void CGGoldenArcherExchangeSend(int Type, int Count);
	void GCGoldenArcherRegisterRecv(PMSG_GOLDEN_ARCHER_REGISTER_RECV* lpMsg);
	void GCGoldenArcherLuckyNumberRecv(PMSG_GOLDEN_ARCHER_LUCKY_NUMBER_RECV* lpMsg);

	void CGGoldenArcherRegisterLuckySend(const char* LuckyNumber);
	void GCGoldenArcherRegisterLuckyRecv(PMSG_GOLDEN_ARCHER_REGISTER_LUCKY_RECV* lpMsg);

	void CGGoldenArcherExchangeLuckySend(const char* LuckyNumber);
	void GCGoldenArcherExchangeLuckyRecv(PMSG_GOLDEN_ARCHER_EXCHANGE_LUCKY_RECV* lpMsg);

	void GoldenArcherCloseProc();
	void CGGoldenArcherCloseSend();

private:

	int iStartPosX;
	int iStartPosY;

	ITEM m_Rena;

	ITEM m_Stone;

	BYTE m_CurrentPage;
	BYTE m_TotalPages;

	int m_RenaExchangeRequiredCount[GOLDEN_ARCHER_MAX_EXCHANGE_RENAS];

	int m_StoneExchangeRequiredCount[GOLDEN_ARCHER_MAX_EXCHANGE_STONES];
	DWORD m_StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE4];

	char m_LuckyNumber[13];

	std::vector<std::string> m_MyLuckyNumbers;
};

extern CGoldenArcher gGoldenArcher;