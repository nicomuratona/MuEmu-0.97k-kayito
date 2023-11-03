#pragma once

#include "User.h"

struct NPC_MOVE_INFO
{
	int MonsterClass;
	int Map;
	int X;
	int Y;
	int MoveMap;
	int MoveX;
	int MoveY;
	int MinLevel;
	int MaxLevel;
	int MinReset;
	int MaxReset;
	int AccountLevel;
	int PkMove;
};

class CCustomNpcMove
{
public:

	CCustomNpcMove();

	~CCustomNpcMove();

	void Load(char* path);

	bool GetNpcMove(LPOBJ lpObj, int MonsterClass, int Map, int X, int Y);

private:

	std::map<int, NPC_MOVE_INFO> m_CustomNpcMove;
};

extern CCustomNpcMove gCustomNpcMove;