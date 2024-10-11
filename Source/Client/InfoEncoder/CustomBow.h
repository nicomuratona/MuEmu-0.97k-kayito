#pragma once

#define MAX_CUSTOM_BOW 32

struct CUSTOM_BOW_INFO
{
	int ItemIndex;
	int ItemType;
	int SkillShot;
};

class CCustomBow
{
public:

	CCustomBow();

	virtual ~CCustomBow();

	void Load(char* path);

private:

	void Init();

	void SetInfo(CUSTOM_BOW_INFO info);

public:

	CUSTOM_BOW_INFO m_CustomBowInfo[MAX_CUSTOM_BOW];
};

extern CCustomBow gCustomBow;