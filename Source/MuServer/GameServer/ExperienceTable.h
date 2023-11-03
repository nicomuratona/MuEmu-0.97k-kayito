#pragma once

#include "User.h"

struct EXPERIENCE_TABLE_INFO
{
	int MinLevel;
	int MaxLevel;
	int MinReset;
	int MaxReset;
	int MinGrandReset;
	int MaxGrandReset;
	int ExperienceRate;
};

class CExperienceTable
{
public:

	CExperienceTable();

	~CExperienceTable();

	void Load(char* path);

	int GetExperienceRate(LPOBJ lpObj);

private:

	std::vector<EXPERIENCE_TABLE_INFO> m_ExperienceTableInfo;
};

extern CExperienceTable gExperienceTable;