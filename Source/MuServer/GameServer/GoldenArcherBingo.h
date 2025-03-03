#pragma once

#include "ProtocolDefines.h"

enum eGoldenArcherBingoState
{
	BINGO_STATE_BLANK = 0,
	BINGO_STATE_EMPTY = 1,
	BINGO_STATE_STAND = 2,
	BINGO_STATE_START = 3,
	BINGO_STATE_CLEAN = 4
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_BINGO_GET_WINNERS_SEND
{
	PSBMSG_HEAD header; // C1:04:04
	int count;
};

struct SDHP_BINGO_CLEAR_REGISTERED_SEND
{
	PSBMSG_HEAD header; // C1:04:05
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_BINGO_GET_WINNERS_RECV
{
	PSWMSG_HEAD header; // C2:04:04
	int count;
};

struct BINGO_WINNER
{
	char Account[11];
	char LuckyNumber[13];
};

struct SDHP_BINGO_CLEAR_REGISTERED_RECV
{
	PSWMSG_HEAD header; // C2:04:05
	BYTE result;
};

//**********************************************//
//**********************************************//
//**********************************************//

struct GOLDEN_ARCHER_BINGO_START_TIME
{
	int Year;
	int Month;
	int Day;
	int DayOfWeek;
	int Hour;
	int Minute;
	int Second;
};

struct GOLDEN_ARCHER_BINGO_INFO
{
	int WarningTime;
	int WarningMsg;
	int StartMessage;
	int EventTime;
	int FinalMessage;
	char EventName[64];
};

struct GOLDEN_ARCHER_BINGO_MAP_INFO
{
	int Map;
	int PosX;
	int PosY;
	int Dir;
};

class CGoldenArcherBingo
{
public:

	CGoldenArcherBingo();

	virtual ~CGoldenArcherBingo();

	void Init();

	void Load(char* path);

	void MainProc();

	char* GetEventName();

	int GetState();

	int GetCurrentRemainTime();

	int GetGoldenArcherIndex();

	bool CheckBingoWinner(int aIndex, const char* lpLuckyNumber);

private:

	void ProcState_BLANK();

	void ProcState_EMPTY();

	void ProcState_STAND();

	void ProcState_START();

	void ProcState_CLEAN();

	void SetState(int state);

	void SetState_BLANK();

	void SetState_EMPTY();

	void SetState_STAND();

	void SetState_START();

	void SetState_CLEAN();

	void CheckSync();

	void SetGoldenArcher();

	void GDBingoGetWinnersSend();

	void GDBingoClearRegisteredSend();

public:

	void DGBingoGetWinnersRecv(SDHP_BINGO_GET_WINNERS_RECV* lpMsg);

	void DGBingoClearRegisteredRecv(SDHP_BINGO_CLEAR_REGISTERED_RECV* lpMsg);

	void StartGoldenArcherBingo();

private:

	GOLDEN_ARCHER_BINGO_INFO m_GoldenArcherBingoInfo;

	GOLDEN_ARCHER_BINGO_MAP_INFO m_GoldenArcherBingoMapInfo;

	std::vector<GOLDEN_ARCHER_BINGO_START_TIME> m_GoldenArcherBingoStartTime;

	int m_State;
	int m_MinutesLeft;
	int m_RemainTime;
	int m_TargetTime;
	int m_TickCount;

	bool m_WaitForDSResponse;
	std::map<std::string, std::vector<std::string>> m_GoldenArcherBingoWinners;

	int m_GoldenArcherIndex;
};

extern CGoldenArcherBingo gGoldenArcherBingo;