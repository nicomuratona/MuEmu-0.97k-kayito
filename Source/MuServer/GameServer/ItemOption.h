#pragma once

#include "Item.h"
#include "User.h"

enum eItemOption
{
	ITEM_OPTION_ADD_PHYSI_DAMAGE = 60,
	ITEM_OPTION_ADD_MAGIC_DAMAGE = 61,
	ITEM_OPTION_ADD_DEFENSE_SUCCESS_RATE = 62,
	ITEM_OPTION_ADD_DEFENSE = 63,
	ITEM_OPTION_ADD_CRITICAL_DAMAGE_RATE = 64,
	ITEM_OPTION_ADD_HP_RECOVERY_RATE = 65,
	ITEM_OPTION_MUL_HP = 66,
	ITEM_OPTION_MUL_MP = 67,
	ITEM_OPTION_ADD_DAMAGE_REDUCTION = 68,
	ITEM_OPTION_ADD_DAMAGE_REFLECT = 69,
	ITEM_OPTION_MUL_DEFENSE_SUCCESS_RATE = 70,
	ITEM_OPTION_ADD_MONEY_AMOUNT_DROP_RATE = 71,
	ITEM_OPTION_ADD_EXCELLENT_DAMAGE_RATE = 72,
	ITEM_OPTION_ADD_PHYSI_DAMAGE_BY_LEVEL = 73,
	ITEM_OPTION_MUL_PHYSI_DAMAGE = 74,
	ITEM_OPTION_ADD_MAGIC_DAMAGE_BY_LEVEL = 75,
	ITEM_OPTION_MUL_MAGIC_DAMAGE = 76,
	ITEM_OPTION_ADD_SPEED = 77,
	ITEM_OPTION_ADD_HUNT_HP = 78,
	ITEM_OPTION_ADD_HUNT_MP = 79,
	ITEM_OPTION_ADD_WING_HP = 80,
	ITEM_OPTION_ADD_WING_MP = 81,
	ITEM_OPTION_ADD_IGNORE_DEFENSE_RATE = 82,
	ITEM_OPTION_ADD_BP = 83,
	ITEM_OPTION_MUL_BP = 84,
};

struct ITEM_OPTION_INFO
{
	int Index;
	int OptionIndex;
	int OptionValue;
	int ItemMinIndex;
	int ItemMaxIndex;
	int ItemSkillOption;
	int ItemLuckOption;
	int ItemAddOption;
	int ItemExceOption;
};

class CItemOption
{
public:

	CItemOption();

	~CItemOption();

	void Load(char* path);

	bool GetItemOption(int index, CItem* lpItem);

	bool GetItemOption(int index, WORD ItemIndex, BYTE ItemSkill, BYTE ItemLuck, BYTE ItemOption, BYTE ItemExcellent, BYTE* pItemOption, BYTE* pItemValue);

	void CalcItemCommonOption(LPOBJ lpObj);

	void InsertOption(LPOBJ lpObj, CItem* lpItem);

private:

	std::map<int, std::vector<ITEM_OPTION_INFO>> m_ItemOptionInfo;
};

extern CItemOption gItemOption;