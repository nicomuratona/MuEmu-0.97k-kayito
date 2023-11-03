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
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		printf(MEM_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (!lpMemScript->SetBuffer(path))
	{
		printf(lpMemScript->GetLastError());

		delete lpMemScript;

		return;
	}

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

			CUSTOM_ITEM_INFO info;

			info.ItemIndex = GET_ITEM(lpMemScript->GetNumber(), lpMemScript->GetAsNumber());

			info.SkillNumber = lpMemScript->GetAsNumber();

			strcpy_s(info.ModelName, lpMemScript->GetAsString());

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		printf(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CCustomItem::SetInfo(CUSTOM_ITEM_INFO info)
{
	if (info.ItemIndex < 0 || info.ItemIndex >= MAX_ITEM)
	{
		return;
	}

	this->m_CustomItemInfo[info.ItemIndex] = info;
}