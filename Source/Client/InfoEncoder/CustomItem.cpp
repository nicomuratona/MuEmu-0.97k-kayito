#include "stdafx.h"
#include "CustomItem.h"

CCustomItem gCustomItem;

CCustomItem::CCustomItem()
{
	this->Init();
}

CCustomItem::~CCustomItem()
{

}

void CCustomItem::Init()
{
	for (int n = 0; n < MAX_ITEM; n++)
	{
		this->m_CustomItemInfo[n].ItemIndex = -1;
	}
}

void CCustomItem::Load(char* path)
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

			CUSTOM_ITEM_INFO info;

			info.ItemIndex = lpReadScript->GetNumber();

			info.ItemIndex = GET_ITEM(info.ItemIndex, lpReadScript->GetAsNumber());

			info.SkillNumber = lpReadScript->GetAsNumber();

			strcpy_s(info.ModelName, lpReadScript->GetAsString());

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		printf(lpReadScript->GetError());
	}

	delete lpReadScript;
}

void CCustomItem::SetInfo(CUSTOM_ITEM_INFO info)
{
	if (info.ItemIndex < 0 || info.ItemIndex >= MAX_ITEM)
	{
		return;
	}

	this->m_CustomItemInfo[info.ItemIndex] = info;
}