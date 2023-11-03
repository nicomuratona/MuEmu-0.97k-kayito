#pragma once

#define MAX_FILTER_SYNTAX 1000

struct FILTER_INFO
{
	char label[16];
};

class CFilter
{
public:

	CFilter();

	~CFilter();

	void Load(char* path);

	void SetInfo(FILTER_INFO info);

	void CheckSyntax(char* text);

private:

	FILTER_INFO m_FilterInfo[MAX_FILTER_SYNTAX];

	int m_count;
};

extern CFilter gFilter;