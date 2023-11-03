#pragma once

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

	void Init();

	void Load(char* path);

	void SetInfo(ITEM_OPTION_INFO info, int index);

public:

	ITEM_OPTION_INFO m_ItemOptionInfo[MAX_ITEM_OPTION_INFO];
};

extern CItemOption gItemOption;