#pragma once

#include "User.h"

#define MAX_BONUS 30

enum eBonusState
{
	BONUS_STATE_BLANK = 0,
	BONUS_STATE_EMPTY = 1,
	BONUS_STATE_START = 2,
};

enum eBonusIndex
{
	BONUS_INDEX_EXPERIENCE_RATE = 0,
	BONUS_INDEX_ITEM_DROP_RATE = 1,
	BONUS_INDEX_CMN_ITEM_DROP_RATE = 2,
	BONUS_INDEX_EXC_ITEM_DROP_RATE = 3,
	BONUS_INDEX_RESET_AMOUNT = 4,
	BONUS_INDEX_GRAND_RESET_AMOUNT = 5,
};

struct BONUS_START_TIME
{
	int Year;
	int Month;
	int Day;
	int DayOfWeek;
	int Hour;
	int Minute;
	int Second;
};

struct BONUS_VALUE_INFO
{
	int BonusIndex;
	int BonusValue[MAX_ACCOUNT_LEVEL];
	int ItemIndex;
	int ItemLevel;
	int MapNumber;
	int MonsterClass;
	int MonsterLevelMin;
	int MonsterLevelMax;
};

struct BONUS_INFO
{
	int Index;
	int State;
	int RemainTime;
	int TargetTime;
	int TickCount;
	int StartMessage;
	int FinalMessage;
	int BonusTime;
	int MinutesLeft;
	int WarningTime;
	int WarningMsg;
	char BonusName[32];
	std::vector<BONUS_START_TIME> StartTime;
	std::vector<BONUS_VALUE_INFO> ValueInfo;
};

class CBonusManager
{
public:

	CBonusManager();

	~CBonusManager();

	void Init();

	void Load(char* path);

	void MainProc();

	void ProcState_BLANK(BONUS_INFO* lpInfo);

	void ProcState_EMPTY(BONUS_INFO* lpInfo);

	void ProcState_START(BONUS_INFO* lpInfo);

	void SetState(BONUS_INFO* lpInfo, int state);

	void SetState_BLANK(BONUS_INFO* lpInfo);

	void SetState_EMPTY(BONUS_INFO* lpInfo);

	void SetState_START(BONUS_INFO* lpInfo);

	void CheckSync(BONUS_INFO* lpInfo);

	int GetState(int index);

	char* GetBonusName(int index);

	int GetCurrentRemainTime(int index);

	int GetRemainTime(int index);

	int GetBonusValue(LPOBJ lpObj, int BonusIndex, int BonusValue, int ItemIndex, int ItemLevel, int MonsterClass, int MonsterLevel);

	void StartBonus(int BonusIndex);

private:

	BONUS_INFO m_BonusInfo[MAX_BONUS];
};

extern CBonusManager gBonusManager;