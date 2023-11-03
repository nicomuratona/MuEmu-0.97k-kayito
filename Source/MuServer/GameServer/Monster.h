#pragma once

#include "Protocol.h"
#include "User.h"

void gObjMonsterDieGiveItem(LPOBJ lpObj, LPOBJ lpTarget);

bool gObjSetPosMonster(int aIndex, int number);

bool gObjSetMonster(int aIndex, int MonsterClass);

bool gObjBallRegen(LPOBJ lpObj);

bool gObjMonsterRegen(LPOBJ lpObj);

bool gObjMonsterMoveCheck(LPOBJ lpObj, int tx, int ty);

void gObjMonsterInitHitDamage(LPOBJ lpObj);

void gObjMonsterSetHitDamage(LPOBJ lpObj, int aIndex, int damage);

int gObjMonsterDelHitDamageUser(LPOBJ lpObj);

int gObjMonsterGetTopHitDamageUser(LPOBJ lpObj);

int gObjMonsterGetTopHitDamageParty(LPOBJ lpObj, int PartyNumber, int* TopHitDamageUser);

int gObjAddMonster(int map);

int gObjAddSummon();

int gObjSummonSetEnemy(LPOBJ lpObj, int aIndex);

void gObjSummonKill(int aIndex);

void gObjMonsterMagicAttack(LPOBJ lpObj, int skill);

void gObjMonsterBeattackRecv(BYTE* aRecv, int aIndex);

void gObjMonsterAttack(LPOBJ lpObj, LPOBJ lpTarget);

void gObjMonsterDie(LPOBJ lpObj, LPOBJ lpTarget);

void gObjMonsterStateProc(LPOBJ lpObj, int code, int aIndex, int SubCode);

//**************************************************************************//
// RAW FUNCTIONS ***********************************************************//
//**************************************************************************//

int gObjMonsterViewportIsCharacter(LPOBJ lpObj);

BOOL gObjMonsterGetTargetPos(LPOBJ lpObj);

BOOL gObjGetTargetPos(LPOBJ lpObj, int sx, int sy, int& tx, int& ty);

int gObjMonsterSearchEnemy(LPOBJ lpObj, BYTE objtype);

int gObjGuardSearchEnemy(LPOBJ lpObj);

void gObjMonsterProcess(LPOBJ lpObj);

BOOL PathFindMoveMsgSend(LPOBJ lpObj);

void gObjMonsterMoveAction(LPOBJ lpObj);

void gObjMonsterBaseAct(LPOBJ lpObj);

void gObjTrapAttackEnemySearchX(LPOBJ lpObj, int count);

void gObjTrapAttackEnemySearchY(LPOBJ lpObj, int count);

void gObjTrapAttackEnemySearch(LPOBJ lpObj);

void gObjTrapAttackEnemySearchRange(LPOBJ lpObj, int iRange);

void gObjMonsterTrapAct(LPOBJ lpObj);

void gObjMonsterStateProcCase0(LPOBJ lpObj, int aIndex);