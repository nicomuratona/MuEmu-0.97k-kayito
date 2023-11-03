#include "stdafx.h"
#include "ItemOptionRate.h"
#include "ItemManager.h"
#include "MemScript.h"
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
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (lpMemScript->SetBuffer(path) == false)
	{
		ErrorMessageBox(lpMemScript->GetLastError());

		delete lpMemScript;

		return;
	}

	this->m_ItemLevelOptionRateInfo.clear();

	this->m_ItemSkillOptionRateInfo.clear();

	this->m_ItemLuckOptionRateInfo.clear();

	this->m_ItemAddOptionRateInfo.clear();

	this->m_ItemExceOptionRateInfo.clear();

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while (true)
			{
				if (section == 0)
				{
					if (strcmp("end", lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ITEM_LEVEL_OPTION_RATE_INFO info;

					info.Index = lpMemScript->GetNumber();

					for (int n = 0; n < MAX_ITEM_LEVEL_OPTION_RATE; n++)
					{
						info.Rate[n] = lpMemScript->GetAsNumber();
					}

					this->m_ItemLevelOptionRateInfo.insert(std::pair<int, ITEM_LEVEL_OPTION_RATE_INFO>(info.Index, info));
				}
				else if (section == 1)
				{
					if (strcmp("end", lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ITEM_SKILL_OPTION_RATE_INFO info;

					info.Index = lpMemScript->GetNumber();

					for (int n = 0; n < MAX_ITEM_SKILL_OPTION_RATE; n++)
					{
						info.Rate[n] = lpMemScript->GetAsNumber();
					}

					this->m_ItemSkillOptionRateInfo.insert(std::pair<int, ITEM_SKILL_OPTION_RATE_INFO>(info.Index, info));
				}
				else if (section == 2)
				{
					if (strcmp("end", lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ITEM_LUCK_OPTION_RATE_INFO info;

					info.Index = lpMemScript->GetNumber();

					for (int n = 0; n < MAX_LUCK_ITEM_OPTION_RATE; n++)
					{
						info.Rate[n] = lpMemScript->GetAsNumber();
					}

					this->m_ItemLuckOptionRateInfo.insert(std::pair<int, ITEM_LUCK_OPTION_RATE_INFO>(info.Index, info));
				}
				else if (section == 3)
				{
					if (strcmp("end", lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ITEM_ADD_OPTION_RATE_INFO info;

					info.Index = lpMemScript->GetNumber();

					for (int n = 0; n < MAX_ITEM_ADD_OPTION_RATE; n++)
					{
						info.Rate[n] = lpMemScript->GetAsNumber();
					}

					this->m_ItemAddOptionRateInfo.insert(std::pair<int, ITEM_ADD_OPTION_RATE_INFO>(info.Index, info));
				}
				else if (section == 4)
				{
					if (strcmp("end", lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ITEM_EXCE_OPTION_RATE_INFO info;

					info.Index = lpMemScript->GetNumber();

					for (int n = 0; n < MAX_ITEM_EXCE_OPTION_RATE; n++)
					{
						info.Rate[n] = lpMemScript->GetAsNumber();
					}

					this->m_ItemExceOptionRateInfo.insert(std::pair<int, ITEM_EXCE_OPTION_RATE_INFO>(info.Index, info));
				}
				else
				{
					break;
				}
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
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