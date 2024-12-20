#include "stdafx.h"
#include "ItemOptionRate.h"
#include "CustomWing.h"
#include "ItemManager.h"
#include "ReadScript.h"
#include "RandomManager.h"
#include "Util.h"

CItemOptionRate gItemOptionRate;

CItemOptionRate::CItemOptionRate()
{
	this->m_ItemLevelOptionRateInfo.clear();

	this->m_ItemSkillOptionRateInfo.clear();

	this->m_ItemLuckOptionRateInfo.clear();

	this->m_ItemAddOptionRateInfo.clear();

	this->m_ItemExceOptionRateInfo.clear();
}

CItemOptionRate::~CItemOptionRate()
{

}

void CItemOptionRate::Load(char* path)
{
	CReadScript* lpReadScript = new CReadScript;

	if (lpReadScript == NULL)
	{
		ErrorMessageBox(READ_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (!lpReadScript->Load(path))
	{
		ErrorMessageBox(READ_SCRIPT_FILE_ERROR, path);

		delete lpReadScript;

		return;
	}

	this->m_ItemLevelOptionRateInfo.clear();

	this->m_ItemSkillOptionRateInfo.clear();

	this->m_ItemLuckOptionRateInfo.clear();

	this->m_ItemAddOptionRateInfo.clear();

	this->m_ItemExceOptionRateInfo.clear();

	try
	{
		eTokenResult token;

		while (true)
		{
			token = lpReadScript->GetToken();

			if (token == TOKEN_END || token == TOKEN_END_SECTION)
			{
				break;
			}

			int section = lpReadScript->GetNumber();

			while (true)
			{
				token = lpReadScript->GetToken();

				if (token == TOKEN_END || token == TOKEN_END_SECTION)
				{
					break;
				}

				if (section == 0)
				{
					ITEM_LEVEL_OPTION_RATE_INFO info;

					info.Index = lpReadScript->GetNumber();

					for (int n = 0; n < MAX_ITEM_LEVEL_OPTION_RATE; n++)
					{
						info.Rate[n] = lpReadScript->GetAsNumber();
					}

					this->m_ItemLevelOptionRateInfo.insert(std::pair<int, ITEM_LEVEL_OPTION_RATE_INFO>(info.Index, info));
				}
				else if (section == 1)
				{
					ITEM_SKILL_OPTION_RATE_INFO info;

					info.Index = lpReadScript->GetNumber();

					for (int n = 0; n < MAX_ITEM_SKILL_OPTION_RATE; n++)
					{
						info.Rate[n] = lpReadScript->GetAsNumber();
					}

					this->m_ItemSkillOptionRateInfo.insert(std::pair<int, ITEM_SKILL_OPTION_RATE_INFO>(info.Index, info));
				}
				else if (section == 2)
				{
					ITEM_LUCK_OPTION_RATE_INFO info;

					info.Index = lpReadScript->GetNumber();

					for (int n = 0; n < MAX_LUCK_ITEM_OPTION_RATE; n++)
					{
						info.Rate[n] = lpReadScript->GetAsNumber();
					}

					this->m_ItemLuckOptionRateInfo.insert(std::pair<int, ITEM_LUCK_OPTION_RATE_INFO>(info.Index, info));
				}
				else if (section == 3)
				{
					ITEM_ADD_OPTION_RATE_INFO info;

					info.Index = lpReadScript->GetNumber();

					for (int n = 0; n < MAX_ITEM_ADD_OPTION_RATE; n++)
					{
						info.Rate[n] = lpReadScript->GetAsNumber();
					}

					this->m_ItemAddOptionRateInfo.insert(std::pair<int, ITEM_ADD_OPTION_RATE_INFO>(info.Index, info));
				}
				else if (section == 4)
				{
					ITEM_EXCE_OPTION_RATE_INFO info;

					info.Index = lpReadScript->GetNumber();

					for (int n = 0; n < MAX_ITEM_EXCE_OPTION_RATE; n++)
					{
						info.Rate[n] = lpReadScript->GetAsNumber();
					}

					this->m_ItemExceOptionRateInfo.insert(std::pair<int, ITEM_EXCE_OPTION_RATE_INFO>(info.Index, info));
				}
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript->GetError());
	}

	delete lpReadScript;
}

bool CItemOptionRate::GetItemLevelOption(int index, BYTE* option)
{
	std::map<int, ITEM_LEVEL_OPTION_RATE_INFO>::iterator it = this->m_ItemLevelOptionRateInfo.find(index);

	if (it == this->m_ItemLevelOptionRateInfo.end())
	{
		return 0;
	}

	CRandomManager RandomManager;

	for (int n = 0; n < MAX_ITEM_LEVEL_OPTION_RATE; n++)
	{
		RandomManager.AddElement(n, it->second.Rate[n]);
	}

	return RandomManager.GetRandomElement(option);
}

bool CItemOptionRate::GetItemSkillOption(int index, BYTE* option)
{
	std::map<int, ITEM_SKILL_OPTION_RATE_INFO>::iterator it = this->m_ItemSkillOptionRateInfo.find(index);

	if (it == this->m_ItemSkillOptionRateInfo.end())
	{
		return 0;
	}

	CRandomManager RandomManager;

	for (int n = 0; n < MAX_ITEM_SKILL_OPTION_RATE; n++)
	{
		RandomManager.AddElement(n, it->second.Rate[n]);
	}

	return RandomManager.GetRandomElement(option);
}

bool CItemOptionRate::GetItemLuckOption(int index, BYTE* option)
{
	std::map<int, ITEM_LUCK_OPTION_RATE_INFO>::iterator it = this->m_ItemLuckOptionRateInfo.find(index);

	if (it == this->m_ItemLuckOptionRateInfo.end())
	{
		return 0;
	}

	CRandomManager RandomManager;

	for (int n = 0; n < MAX_LUCK_ITEM_OPTION_RATE; n++)
	{
		RandomManager.AddElement(n, it->second.Rate[n]);
	}

	return RandomManager.GetRandomElement(option);
}

bool CItemOptionRate::GetItemAddOption(int index, BYTE* option)
{
	std::map<int, ITEM_ADD_OPTION_RATE_INFO>::iterator it = this->m_ItemAddOptionRateInfo.find(index);

	if (it == this->m_ItemAddOptionRateInfo.end())
	{
		return 0;
	}

	CRandomManager RandomManager;

	for (int n = 0; n < MAX_ITEM_ADD_OPTION_RATE; n++)
	{
		RandomManager.AddElement(n, it->second.Rate[n]);
	}

	return RandomManager.GetRandomElement(option);
}

bool CItemOptionRate::GetItemExceOption(int index, BYTE* option)
{
	std::map<int, ITEM_EXCE_OPTION_RATE_INFO>::iterator it = this->m_ItemExceOptionRateInfo.find(index);

	if (it == this->m_ItemExceOptionRateInfo.end())
	{
		return 0;
	}

	CRandomManager RandomManager;

	for (int n = 0; n < MAX_ITEM_EXCE_OPTION_RATE; n++)
	{
		RandomManager.AddElement(n, it->second.Rate[n]);
	}

	return RandomManager.GetRandomElement(option);
}

bool CItemOptionRate::MakeExceOption(int ItemIndex, int value, BYTE* option)
{
	(*option) = 0;

	int count = 0;

	int MaxExceOption = MAX_EXC_OPTION;

	if ((ItemIndex >= GET_ITEM(12, 3) && ItemIndex <= GET_ITEM(12, 6))) // 2sd Wings
	{
		MaxExceOption = 3;
	}

	if (gCustomWing.CheckCustomWingByItem(ItemIndex))
	{
		MaxExceOption = 3;
	}

	if (ItemIndex == GET_ITEM(13, 3)) // Dinorant
	{
		MaxExceOption = 3;
	}

	while (true)
	{
		if (count >= value || count >= MaxExceOption)
		{
			break;
		}

		int value = 1 << (GetLargeRand() % MaxExceOption);

		if (((*option) & value) == 0)
		{
			(*option) |= value;

			count++;
		}
	}

	return 1;
}