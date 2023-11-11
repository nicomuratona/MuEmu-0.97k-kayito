#pragma once

struct CUSTOM_ITEM_INFO
{
	short ItemIndex;
	BYTE SkillNumber;
	char ModelName[32];
};

class CCustomItem
{
public:

	CCustomItem();

	virtual ~CCustomItem();

	void Init();

	void Load(char* path);

	void SetInfo(CUSTOM_ITEM_INFO info);

public:

	CUSTOM_ITEM_INFO m_CustomItemInfo[MAX_ITEM];
};

extern CCustomItem gCustomItem;