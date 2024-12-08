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

	void Load(char* path);

private:

	void Init();

	void SetInfo(CUSTOM_WING_INFO info);

public:

	CUSTOM_WING_INFO m_CustomWingInfo[MAX_CUSTOM_WING];
};

extern CCustomWing gCustomWing;