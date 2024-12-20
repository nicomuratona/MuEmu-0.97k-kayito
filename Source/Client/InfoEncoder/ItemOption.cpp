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
	CReadScript* lpReadScript = new CReadScript;

	if (lpReadScript == NULL)
	{
		printf(READ_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (!lpReadScript->Load(path))
	{
		printf(READ_SCRIPT_FILE_ERROR, path);

		delete lpReadScript;

		return;
	}

	this->Init();

	int index = 0;

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

			ITEM_OPTION_INFO info;

			info.Index = lpReadScript->GetNumber();

			info.OptionIndex = lpReadScript->GetAsNumber();

			info.OptionValue = lpReadScript->GetAsNumber();

			info.ItemMinIndex = lpReadScript->GetAsNumber();

			info.ItemMinIndex = GET_ITEM(info.ItemMinIndex, lpReadScript->GetAsNumber());

			info.ItemMaxIndex = lpReadScript->GetAsNumber();

			info.ItemMaxIndex = GET_ITEM(info.ItemMaxIndex, lpReadScript->GetAsNumber());

			info.ItemSkillOption = lpReadScript->GetAsNumber();

			info.ItemLuckOption = lpReadScript->GetAsNumber();

			info.ItemAddOption = lpReadScript->GetAsNumber();

			info.ItemExceOption = lpReadScript->GetAsNumber();

			this->SetInfo(info, index);

			index++;
		}
	}
	catch (...)
	{
		printf(lpReadScript->GetError());
	}

	delete lpReadScript;
}

void CItemOption::SetInfo(ITEM_OPTION_INFO info, int index)
{
	if (index < 0 || index >= MAX_ITEM_OPTION_INFO)
	{
		return;
	}

	this->m_ItemOptionInfo[index] = info;
}