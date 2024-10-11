#include "stdafx.h"
#include "CustomBow.h"
#include "CustomItem.h"

CCustomBow gCustomBow;

CCustomBow::CCustomBow()
{
	this->Init();
}

CCustomBow::~CCustomBow()
{

}

void CCustomBow::Init()
{
	for (int n = 0; n < MAX_CUSTOM_BOW; n++)
	{
		this->m_CustomBowInfo[n].ItemIndex = -1;
	}
}

void CCustomBow::Load(char* path)
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
		while (true)
		{
			eTokenResult token = lpReadScript->GetToken();

			if (token == TOKEN_END || token == TOKEN_END_SECTION)
			{
				break;
			}

			CUSTOM_BOW_INFO info;

			CUSTOM_ITEM_INFO itemInfo;

			info.ItemIndex = lpReadScript->GetNumber();

			itemInfo.ItemIndex = GET_ITEM(4, info.ItemIndex);

			info.ItemType = lpReadScript->GetAsNumber();

			info.SkillShot = lpReadScript->GetAsNumber();

			itemInfo.SkillNumber = 24;

			strcpy_s(itemInfo.ModelName, lpReadScript->GetAsString());

			this->SetInfo(info);

			gCustomItem.SetInfo(itemInfo);

		}
	}
	catch (...)
	{
		printf(lpReadScript->GetError());
	}

	delete lpReadScript;
}

void CCustomBow::SetInfo(CUSTOM_BOW_INFO info)
{
	if (info.ItemIndex < 0 || info.ItemIndex >= MAX_CUSTOM_BOW)
	{
		return;
	}

	this->m_CustomBowInfo[info.ItemIndex] = info;
}