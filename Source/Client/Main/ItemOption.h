#pragma once

#define MAX_ITEM_OPTION_INFO 4000

struct ITEM_OPTION_INFO
{
	short Index;
	short OptionIndex;
	short OptionValue;
	short ItemMinIndex;
	short ItemMaxIndex;
	short ItemSkillOption;
	short ItemLuckOption;
	short ItemAddOption;
	short ItemExceOption;
};

class CItemOption
{
public:

	CItemOption();

	virtual ~CItemOption();

	void Init(ITEM_OPTION_INFO* info);

	bool GetItemOption(int index, WORD ItemIndex, BYTE ItemSkill, BYTE ItemLuck, BYTE ItemOption, BYTE ItemExcellent, BYTE* pItemOption, BYTE* pItemValue);

	int GetItemOptionValue(int index, int value, BYTE ItemLevel);

private:

	std::map<int, std::vector<ITEM_OPTION_INFO>> m_ItemOptionInfo;
};

extern CItemOption gItemOption;