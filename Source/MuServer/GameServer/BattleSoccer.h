#pragma once

#include "BattleGround.h"
#include "Guild.h"

class CBattleSoccer : public CBattleGround
{
public:

	CBattleSoccer();

	~CBattleSoccer();

	void SetGoalMove(int set_code);

	int GetGoalMove();

	void SetGoalLeftRect(int x, int y, int xl, int yl);

	void SetGoalRightRect(int x, int y, int xl, int yl);

public:

	int m_goalmove;

	int m_BallIndex;

	RECT m_GoalLeft;

	RECT m_GoalRight;

	GUILD_INFO* m_RedTeam;

	GUILD_INFO* m_BlueTeam;
};