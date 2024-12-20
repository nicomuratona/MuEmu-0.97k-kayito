#pragma once

#define MAX_CUSTOM_WING 7

struct CUSTOM_WING_INFO
{
	int Index;
	int ItemIndex;
	int DefenseConstA;
	int IncDamageConstA;
	int IncDamageConstB;
	int DecDamageConstA;
	int DecDamageConstB;
};

class CCustomWing
{
public:

	CCustomWing();

	virtual ~CCustomWing();

	void Init();

	void Load(char* path);

	void SetInfo(CUSTOM_WING_INFO info);

	CUSTOM_WING_INFO* GetInfo(int index);

	CUSTOM_WING_INFO* GetInfoByItem(int ItemIndex);

	bool CheckCustomWing(int index);

	bool CheckCustomWingByItem(int ItemIndex);

	int GetCustomWingIndex(int ItemIndex);

	int GetCustomWingDefense(int ItemIndex, int ItemLevel);

	int GetCustomWingIncDamage(int ItemIndex, int ItemLevel);

	int GetCustomWingDecDamage(int ItemIndex, int ItemLevel);

public:

	CUSTOM_WING_INFO m_CustomWingInfo[MAX_CUSTOM_WING];
};

extern CCustomWing gCustomWing;