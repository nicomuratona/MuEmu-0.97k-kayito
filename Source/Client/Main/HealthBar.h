#pragma once

struct MONSTER_HEALTH_BAR
{
	WORD index;
	BYTE type;
	BYTE rateHP;
};

class CHealthBar
{
public:

	CHealthBar();

	virtual ~CHealthBar();

	void ClearHealthBar();

	void InsertHealthBar(WORD index, BYTE type, BYTE HP);

	MONSTER_HEALTH_BAR* GetHealthBar(WORD index, BYTE type);

	void Init();

	static void DrawHealthBar();

	static void DrawPointingHealthBar();

public:

	bool DeleteHealthBar;

private:

	MONSTER_HEALTH_BAR m_MonsterHealthBar[MAX_MAIN_VIEWPORT];
};

extern CHealthBar gHealthBar;