#include "stdafx.h"
#include "ItemOption.h"

CItemOption gItemOption;

CItemOption::CItemOption()
{

}

CItemOption::~CItemOption()
{

}

void CItemOption::Init()
{
	for (int n = 0; n < MAX_ITEM_OPTION_INFO; n++)
	{
		this->m_ItemOptionInfo[n].Index = -1;
	}
}

void CItemOption::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		printf(MEM_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (lpMemScript->SetBuffer(path) == false)
	{
		printf(lpMemScript->GetLastError());

		delete lpMemScript;

		return;
	}

	this->Init();

	int index = 0;

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if (strcmp("end", lpMemScript->GetString()) == 0)
			{
				break;
			}

			ITEM_OPTION_INFO info;

			info.Index = lpMemScript->GetNumber();

			info.OptionIndex = lpMemScript->GetAsNumber();

			info.OptionValue = lpMemScript->GetAsNumber();

			info.ItemMinIndex = GET_ITEM(lpMemScript->GetAsNumber(), lpMemScript->GetAsNumber());

			info.ItemMaxIndex = GET_ITEM(lpMemScript->GetAsNumber(), lpMemScript->GetAsNumber());

			info.ItemSkillOption = lpMemScript->GetAsNumber();

			info.ItemLuckOption = lpMemScript->GetAsNumber();

			info.ItemAddOption = lpMemScript->GetAsNumber();

			info.ItemExceOption = lpMemScript->GetAsNumber();

			this->SetInfo(info, index);

			index++;
		}
	}
	catch (...)
	{
		printf(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CItemOption::SetInfo(ITEM_OPTION_INFO info, int index)
{
	if (index < 0 || index >= MAX_ITEM_OPTION_INFO)
	{
		return;
	}

	this->m_ItemOptionInfo[index] = info;
}