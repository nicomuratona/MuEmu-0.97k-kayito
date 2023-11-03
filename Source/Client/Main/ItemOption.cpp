#include "stdafx.h"
#include "ItemOption.h"

CItemOption gItemOption;

CItemOption::CItemOption()
{
	this->m_ItemOptionInfo.clear();
}

CItemOption::~CItemOption()
{

}

void CItemOption::Load(ITEM_OPTION_INFO* info)
{
	this->m_ItemOptionInfo.clear();

	std::map<int, std::vector<ITEM_OPTION_INFO>>::iterator it;

	for (int n = 0; n < MAX_ITEM_OPTION_INFO; n++)
	{
		if (info[n].Index == -1)
		{
			continue;
		}

		it = this->m_ItemOptionInfo.find(info[n].Index);

		if (it == this->m_ItemOptionInfo.end())
		{
			this->m_ItemOptionInfo.insert(std::pair<int, std::vector<ITEM_OPTION_INFO>>(info[n].Index, std::vector<ITEM_OPTION_INFO>(1, info[n])));
		}
		else
		{
			it->second.push_back(info[n]);
		}
	}
}

bool CItemOption::GetItemOption(int index, WORD ItemIndex, BYTE ItemSkill, BYTE ItemLuck, BYTE ItemOption, BYTE ItemExcellent, BYTE* pItemOption, BYTE* pItemValue)
{
	bool result = false;

	std::map<int, std::vector<ITEM_OPTION_INFO>>::iterator ItemOptionInfo = this->m_ItemOptionInfo.find(index);

	if (ItemOptionInfo != this->m_ItemOptionInfo.end())
	{
		for (std::vector<ITEM_OPTION_INFO>::iterator it = ItemOptionInfo->second.begin(); it != ItemOptionInfo->second.end(); it++)
		{
			if (it->Index != index)
			{
				continue;
			}

			if (it->ItemMinIndex != -1 && it->ItemMinIndex > ItemIndex)
			{
				continue;
			}

			if (it->ItemMaxIndex != -1 && it->ItemMaxIndex < ItemIndex)
			{
				continue;
			}

			if (it->ItemSkillOption != -1 && it->ItemSkillOption > ItemSkill)
			{
				continue;
			}

			if (it->ItemLuckOption != -1 && it->ItemLuckOption > ItemLuck)
			{
				continue;
			}

			if (it->ItemAddOption != -1 && it->ItemAddOption > ItemOption)
			{
				continue;
			}

			if (it->ItemExceOption != -1 && (ItemExcellent & it->ItemExceOption) == 0)
			{
				continue;
			}

			result = true;

			(*pItemOption) = (BYTE)it->OptionIndex;

			(*pItemValue) = (BYTE)it->OptionValue;
		}
	}

	return result;
}