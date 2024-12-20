#include "stdafx.h"
#include "CustomWing.h"
#include "ItemManager.h"
#include "ReadScript.h"

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

			info.Index = Index++;

			info.ItemIndex = lpReadScript->GetNumber();

			info.ItemIndex = GET_ITEM(info.ItemIndex, lpReadScript->GetAsNumber());

			info.DefenseConstA = lpReadScript->GetAsNumber();

			info.IncDamageConstA = lpReadScript->GetAsNumber();

			info.IncDamageConstB = lpReadScript->GetAsNumber();

			info.DecDamageConstA = lpReadScript->GetAsNumber();

			info.DecDamageConstB = lpReadScript->GetAsNumber();

			token = lpReadScript->GetToken(); // Skip Model String

			this->SetInfo(info);
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

CUSTOM_WING_INFO* CCustomWing::GetInfo(int index)
{
	if (index < 0 || index >= MAX_CUSTOM_WING)
	{
		return NULL;
	}

	if (this->m_CustomWingInfo[index].Index != index)
	{
		return NULL;
	}

	return &this->m_CustomWingInfo[index];
}

CUSTOM_WING_INFO* CCustomWing::GetInfoByItem(int ItemIndex)
{
	for (int n = 0; n < MAX_CUSTOM_WING; n++)
	{
		CUSTOM_WING_INFO* lpInfo = this->GetInfo(n);

		if (lpInfo == NULL)
		{
			continue;
		}

		if (lpInfo->ItemIndex == ItemIndex)
		{
			return lpInfo;
		}
	}

	return NULL;
}

bool CCustomWing::CheckCustomWing(int index)
{
	return (this->GetInfo(index) != NULL);
}

bool CCustomWing::CheckCustomWingByItem(int ItemIndex)
{
	return (this->GetInfoByItem(ItemIndex) != NULL);
}

int CCustomWing::GetCustomWingIndex(int ItemIndex)
{
	CUSTOM_WING_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if (lpInfo == NULL)
	{
		return 0;
	}

	return lpInfo->Index;
}

int CCustomWing::GetCustomWingDefense(int ItemIndex, int ItemLevel)
{
	CUSTOM_WING_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if (lpInfo == NULL)
	{
		return 0;
	}

	return (lpInfo->DefenseConstA * ItemLevel);
}

int CCustomWing::GetCustomWingIncDamage(int ItemIndex, int ItemLevel)
{
	CUSTOM_WING_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if (lpInfo == NULL)
	{
		return 100;
	}

	return (lpInfo->IncDamageConstA + (ItemLevel * lpInfo->IncDamageConstB));
}

int CCustomWing::GetCustomWingDecDamage(int ItemIndex, int ItemLevel)
{
	CUSTOM_WING_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if (lpInfo == NULL)
	{
		return 100;
	}

	return (lpInfo->DecDamageConstA + (ItemLevel * lpInfo->DecDamageConstB));
}