#pragma once

#define MAX_ITEM_LEVEL_OPTION_RATE 12
#define MAX_ITEM_SKILL_OPTION_RATE 2
#define MAX_LUCK_ITEM_OPTION_RATE 2
#define MAX_ITEM_ADD_OPTION_RATE 8
#define MAX_ITEM_EXCE_OPTION_RATE 7

struct ITEM_LEVEL_OPTION_RATE_INFO
{
	int Index;
	int Rate[MAX_ITEM_LEVEL_OPTION_RATE];
};

struct ITEM_SKILL_OPTION_RATE_INFO
{
	int Index;
	int Rate[MAX_ITEM_SKILL_OPTION_RATE];
};

struct ITEM_LUCK_OPTION_RATE_INFO
{
	int Index;
	int Rate[MAX_LUCK_ITEM_OPTION_RATE];
};

struct ITEM_ADD_OPTION_RATE_INFO
{
	int Index;
	int Rate[MAX_ITEM_ADD_OPTION_RATE];
};

struct ITEM_EXCE_OPTION_RATE_INFO
{
	int Index;
	int Rate[MAX_ITEM_EXCE_OPTION_RATE];
};

class CItemOptionRate
{
public:

	CItemOptionRate();

	~CItemOptionRate();

	void Load(char* path);

	bool GetItemLevelOption(int index, BYTE* option);

	bool GetItemSkillOption(int index, BYTE* option);

	bool GetItemLuckOption(int index, BYTE* option);

	bool GetItemAddOption(int index, BYTE* option);

	bool GetItemExceOption(int index, BYTE* option);

	bool MakeExceOption(int ItemIndex, int value, BYTE* option);

private:

	std::map<int, ITEM_LEVEL_OPTION_RATE_INFO> m_ItemLevelOptionRateInfo;

	std::map<int, ITEM_SKILL_OPTION_RATE_INFO> m_ItemSkillOptionRateInfo;

	std::map<int, ITEM_LUCK_OPTION_RATE_INFO> m_ItemLuckOptionRateInfo;

	std::map<int, ITEM_ADD_OPTION_RATE_INFO> m_ItemAddOptionRateInfo;

	std::map<int, ITEM_EXCE_OPTION_RATE_INFO> m_ItemExceOptionRateInfo;
};

extern CItemOptionRate gItemOptionRate;