#include "stdafx.h"
#include "BattleSoccerManager.h"
#include "Protocol.h"
#include "Util.h"

CBattleSoccerManager gBattleSoccer;

CBattleSoccerManager::CBattleSoccerManager()
{

}

CBattleSoccerManager::~CBattleSoccerManager()
{

}

void CBattleSoccerManager::InitBattleSoccer()
{
	for (int n = 0; n < MAX_BATTLESOCCER_GROUND; n++)
	{
		this->gBSGround[n] = new CBattleSoccer;

		this->gBSGround[n]->m_BlueTeam = NULL;

		this->gBSGround[n]->m_RedTeam = NULL;
	}

	this->gBSGround[0]->SetBattleType(1);

	this->gBSGround[0]->SetGroundRect(6, 54, 141, 70, 180);

	this->gBSGround[0]->SetGoalLeftRect(60, 139, 64, 140);

	this->gBSGround[0]->SetGoalRightRect(60, 181, 64, 182);
}

void CBattleSoccerManager::ClearBattleSoccer()
{
	for (int n = 0; n < MAX_BATTLESOCCER_GROUND; n++)
	{
		delete this->gBSGround[n];
	}
}

int CBattleSoccerManager::CheckBlankBattleGround()
{
	for (int n = 0; n < MAX_BATTLESOCCER_GROUND; n++)
	{
		if (this->gBSGround[n]->GetUsed() == false)
		{
			return n;
		}
	}

	return 0xFF;
}

int CBattleSoccerManager::CheckBattleGroundTimer()
{
	int time = 0;

	//return 0;

	int n = 0;

	if (this->gBSGround[n]->GetUsed() != false)
	{
		time = this->gBSGround[n]->GetTimer();

		if (this->gBSGround[n]->m_BlueTeam == NULL || this->gBSGround[n]->m_RedTeam == NULL)
		{
			return 0;
		}

		for (int i = 0; i < MAX_GUILD_MEMBER; i++)
		{
			int index = this->gBSGround[n]->m_BlueTeam->Members[i].Index;

			if (index != -1)
			{
				GCTimeViewSend(index, time);
			}

			index = this->gBSGround[n]->m_RedTeam->Members[i].Index;

			if (index != -1)
			{
				GCTimeViewSend(index, time);
			}
		}
	}

	return time;
}

void CBattleSoccerManager::BattleGroundEnable(int n, bool enable)
{
	if (BATTLE_SOCCER_GROUND_RANGE(n) == false)
	{
		return;
	}

	if (enable == true)
	{
		this->gBSGround[n]->Enable(enable);

		this->gBSGround[n]->SetTimer(MAX_BATTLESOCCER_TIME);
	}
	else
	{
		this->gBSGround[n]->Enable(false);

		this->gBSGround[n]->SetTeamMaster(0, "none");

		this->gBSGround[n]->SetTeamMaster(1, "none");

		this->gBSGround[n]->m_RedTeam = 0;

		this->gBSGround[n]->m_BlueTeam = 0;
	}
}

void CBattleSoccerManager::SetBattleTeamMaster(int ground, int team, char* szname, GUILD_INFO* lpGuild)
{
	if (BATTLE_SOCCER_GROUND_RANGE(ground) == false)
	{
		return;
	}

	this->gBSGround[ground]->SetTeamMaster(team, szname);

	this->gBSGround[ground]->SetTeamName(team, lpGuild->Name);

	if (team == 0)
	{
		this->gBSGround[ground]->m_RedTeam = lpGuild;
	}
	else
	{
		this->gBSGround[ground]->m_BlueTeam = lpGuild;
	}
}

void CBattleSoccerManager::BattleGetTeamPosition(int ground, int team, short& X, short& Y)
{
	if (BATTLE_SOCCER_GROUND_RANGE(ground) == false)
	{
		return;
	}

	int w = this->gBSGround[ground]->m_rect.right - this->gBSGround[ground]->m_rect.left;

	int h = this->gBSGround[ground]->m_rect.bottom - this->gBSGround[ground]->m_rect.top;

	if (w < 1)
	{
		w = 1;
	}

	if (h < 1)
	{
		h = 1;
	}

	X = (short)this->gBSGround[ground]->m_rect.left + (short)(GetLargeRand() % w);

	Y = (short)this->gBSGround[ground]->m_rect.top + (short)(GetLargeRand() % h);
}

char* CBattleSoccerManager::GetBattleTeamName(int ground, int team)
{
	if (BATTLE_SOCCER_GROUND_RANGE(ground) == false)
	{
		return NULL;
	}

	return this->gBSGround[ground]->GetTeamName(team);
}

void CBattleSoccerManager::SetBattleTeamScore(int ground, int team, int score)
{
	if (BATTLE_SOCCER_GROUND_RANGE(ground) == false)
	{
		return;
	}

	this->gBSGround[ground]->SetScore(team, score);
}

int CBattleSoccerManager::GetBattleTeamScore(int ground, int team)
{
	if (BATTLE_SOCCER_GROUND_RANGE(ground) == false)
	{
		return 0;
	}

	return this->gBSGround[ground]->GetScore(team);
}

void CBattleSoccerManager::BattleSoccerGoalStart(int ground)
{
	if (BATTLE_SOCCER_GROUND_RANGE(ground) == false)
	{
		return;
	}

	this->gBSGround[ground]->SetGoalMove(1);
}

void CBattleSoccerManager::BattleSoccerGoalEnd(int ground)
{
	if (BATTLE_SOCCER_GROUND_RANGE(ground) == false)
	{
		return;
	}

	this->gBSGround[ground]->SetGoalMove(0);
}

int CBattleSoccerManager::GetBattleSoccerGoalMove(int ground)
{
	if (BATTLE_SOCCER_GROUND_RANGE(ground) == false)
	{
		return 0;
	}

	return this->gBSGround[ground]->GetGoalMove();
}

bool CBattleSoccerManager::BattleSoccerScoreUpdate(int ground, int team)
{
	if (BATTLE_SOCCER_GROUND_RANGE(ground) == false)
	{
		return false;
	}

	int score = 20;

	int totalscore = 1;

	GUILD_INFO* lpGuild;

	GUILD_INFO* lpTargetGuild;

	bool bGoalTeam;

	if (team == 0)
	{
		lpGuild = this->gBSGround[ground]->m_BlueTeam;

		lpTargetGuild = this->gBSGround[ground]->m_RedTeam;

		bGoalTeam = true;
	}
	else
	{
		lpGuild = this->gBSGround[ground]->m_RedTeam;

		lpTargetGuild = this->gBSGround[ground]->m_BlueTeam;

		bGoalTeam = false;
	}

	if (lpGuild == NULL)
	{
		return false;
	}

	if (gGuild.GuildWarScoreCheck(lpGuild, lpTargetGuild, score) != false)
	{
		if (lpGuild->WarType == GUILD_WAR_TYPE_SOCCER)
		{
			gGuild.GuildWarEndSend(lpGuild, lpTargetGuild, 1, 0);

			this->BattleGroundEnable(lpGuild->BattleGroundIndex, false);

			gGuild.GuildWarStateEnd(lpGuild, lpTargetGuild);

			//gGuild.gObjGuildGoalSend(lpGuild, lpTargetGuild, 255, 255);
		}
	}

	return true;
}

int CBattleSoccerManager::CheckGoal(int x, int y, int& ground) // R:[-1:FAIL ; 0:Team_0 ; 1:Team_1]
{
	for (int n = 0; n < MAX_BATTLESOCCER_GROUND; n++)
	{
		if (this->gBSGround[n]->m_Set == TRUE)
		{
			if (x >= this->gBSGround[n]->m_GoalLeft.left && x <= this->gBSGround[n]->m_GoalLeft.right && y >= this->gBSGround[n]->m_GoalLeft.top && y <= this->gBSGround[n]->m_GoalLeft.bottom)
			{
				ground = n;

				return 0;
			}

			if (x >= this->gBSGround[n]->m_GoalRight.left && x <= this->gBSGround[n]->m_GoalRight.right && y >= this->gBSGround[n]->m_GoalRight.top && y <= this->gBSGround[n]->m_GoalRight.bottom)
			{
				ground = n;

				return 1;
			}
		}
	}

	return -1;
}

int CBattleSoccerManager::CheckBattleGround(LPOBJ lpObj) // R:[-1:FAIL else OBJGround]
{
	for (int n = 0; n < MAX_BATTLESOCCER_GROUND; n++)
	{
		if (this->gBSGround[n]->m_Set == TRUE)
		{
			if (lpObj->X >= this->gBSGround[n]->m_rect.left && lpObj->X <= this->gBSGround[n]->m_rect.right && lpObj->Y >= this->gBSGround[n]->m_rect.top && lpObj->Y <= this->gBSGround[n]->m_rect.bottom)
			{
				return n;
			}

			if (lpObj->X >= this->gBSGround[n]->m_GoalLeft.left && lpObj->X <= this->gBSGround[n]->m_GoalLeft.right && lpObj->Y >= this->gBSGround[n]->m_GoalLeft.top && lpObj->Y <= this->gBSGround[n]->m_GoalLeft.bottom)
			{
				return n;
			}

			if (lpObj->X >= this->gBSGround[n]->m_GoalRight.left && lpObj->X <= this->gBSGround[n]->m_GoalRight.right && lpObj->Y >= this->gBSGround[n]->m_GoalRight.top && lpObj->Y <= this->gBSGround[n]->m_GoalRight.bottom)
			{
				return n;
			}
		}
	}

	return -1;
}