#pragma once

#include "Map.h"

struct FLYING_DRAGONS_INFO
{
	bool Active;
	time_t EndTime;
	int EventIndex;
};

class CFlyingDragons
{
public:

	CFlyingDragons();

	~CFlyingDragons();

	void Init();

	void FlyingDragonsDelete();

	void FlyingDragonsAdd(int map, int invasionTime, int index);

	void FlyingDragonsBossDieProc(int map);

	void FlyingDragonsCheck(int map, int index);

private:

	FLYING_DRAGONS_INFO m_FlyingDragonsInfo[MAX_MAP];
};

extern CFlyingDragons gFlyingDragons;