#pragma once

#include "User.h"

class CMonsterAI
{
public:

	CMonsterAI();

	~CMonsterAI();

	static void MonsterAIProc();

	static BOOL RunAI(int aIndex, int MonsterClass);

	static void MonsterStateMsgProc(int aIndex);

	static void ProcessStateMsg(LPOBJ lpObj, int code, int aIndex, int SubCode);

	static BOOL UpdateCurrentAIUnit(int aIndex);

	static void MonsterMoveProc();

	static void MonsterMove(int aIndex);
};