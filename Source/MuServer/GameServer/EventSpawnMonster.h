#pragma once

enum EVENT_NUMBER
{
	EVENT_SPAWN_BLOODCASTLE = 0,
	EVENT_SPAWN_DEVILSQUARE = 1
};

struct EVENT_SPAWN_MONSTER_INFO
{
	int Event;
	int Level;
	int Stage;
	int Monster;
	int MaxRegenTime;
};

class CEventSpawnMonster
{
public:

	CEventSpawnMonster();

	~CEventSpawnMonster();

	void Load(char* path);

public:

	std::vector<EVENT_SPAWN_MONSTER_INFO> m_EventSpawnMonsterInfo;
};

extern CEventSpawnMonster gEventSpawnMonster;