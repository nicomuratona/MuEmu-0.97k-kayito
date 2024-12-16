#include "stdafx.h"
#include "CustomWing.h"
#include "CustomItem.h"

CCustomWing gCustomWing;

CCustomWing::CCustomWing()
{
	this->Init();
}

CCustomWing::~CCustomWing()
{

}

void CCustomWing::Init()
{
	for (int n = 0; n < MAX_CUSTOM_WING; n++)
	{
		this->m_CustomWingInfo[n].Index = -1;
	}
}

void CCustomWing::Load(char* path)
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

	int Index = 0;

	try
	{
		while (true)
		{
			eTokenResult token = lpReadScript->GetToken();

			if (token == TOKEN_END || token == TOKEN_END_SECTION)
			{
				break;
			}

			CUSTOM_WING_INFO info;

			CUSTOM_ITEM_INFO itemInfo;

			info.Index = Index++;

			info.ItemIndex = lpReadScript->GetNumber();

			info.ItemIndex = GET_ITEM(info.ItemIndex, lpReadScript->GetAsNumber());

			itemInfo.ItemIndex = info.ItemIndex;

			itemInfo.SkillNumber = -1;

			info.DefenseConstA = lpReadScript->GetAsNumber();

			info.IncDamageConstA = lpReadScript->GetAsNumber();

			info.IncDamageConstB = lpReadScript->GetAsNumber();

			info.DecDamageConstA = lpReadScript->GetAsNumber();

			info.DecDamageConstB = lpReadScript->GetAsNumber();

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

void CCustomWing::SetInfo(CUSTOM_WING_INFO info)
{
	if (info.Index < 0 || info.Index >= MAX_CUSTOM_WING)
	{
		return;
	}

	this->m_CustomWingInfo[info.Index] = info;
}