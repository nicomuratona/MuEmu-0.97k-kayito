#pragma once

#include "User.h"
#include "BattleSoccer.h"

#define MAX_BATTLESOCCER_TIME 600000

#define BATTLE_SOCCER_GROUND_RANGE(x) (((x)<0)?false:((x)>=MAX_BATTLESOCCER_GROUND)?false:true)

#define MAX_BATTLESOCCER_GROUND 1

class CBattleSoccerManager
{
public:

	CBattleSoccerManager();

	~CBattleSoccerManager();

	void InitBattleSoccer();

	void ClearBattleSoccer();

	int CheckBlankBattleGround();

	int CheckBattleGroundTimer();

	void BattleGroundEnable(int n, bool enable);

	void SetBattleTeamMaster(int ground, int team, char* szname, GUILD_INFO* lpGuild);

	void BattleGetTeamPosition(int ground, int team, short& X, short& Y);

	char* GetBattleTeamName(int ground, int team);

	void SetBattleTeamScore(int ground, int team, int score);

	int GetBattleTeamScore(int ground, int team);

	void BattleSoccerGoalStart(int ground);

	void BattleSoccerGoalEnd(int ground);

	int GetBattleSoccerGoalMove(int ground);

	bool BattleSoccerScoreUpdate(int ground, int team);

	int CheckGoal(int x, int y, int& ground);

	int CheckBattleGround(LPOBJ lpObj);

public:

	CBattleSoccer* gBSGround[MAX_BATTLESOCCER_GROUND];
};

extern CBattleSoccerManager gBattleSoccer;