#include "stdafx.h"
#include "ItemPosition.h"

CItemPosition gItemPosition;

CItemPosition::CItemPosition()
{

}

CItemPosition::~CItemPosition()
{

}

void CItemPosition::Init()
{
	for (int n = 0; n < MAX_ITEM; n++)
	{
		this->m_ItemPositionInfo[n].ItemIndex = -1;
	}
}

void CItemPosition::Load(char* path)
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

			ITEM_POSITION_INFO info;

			info.ItemIndex = lpReadScript->GetNumber();

			info.ItemIndex = GET_ITEM(info.ItemIndex, lpReadScript->GetAsNumber());

			info.PositionX = lpReadScript->GetAsFloatNumber();

			info.PositionY = lpReadScript->GetAsFloatNumber();

			info.RotationX = lpReadScript->GetAsFloatNumber();

			info.RotationY = lpReadScript->GetAsFloatNumber();

			info.RotationZ = lpReadScript->GetAsFloatNumber();

			info.Scale = lpReadScript->GetAsFloatNumber();

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		printf(lpReadScript->GetError());
	}

	delete lpReadScript;
}

void CItemPosition::SetInfo(ITEM_POSITION_INFO info)
{
	if (info.ItemIndex < 0 || info.ItemIndex >= MAX_ITEM)
	{
		return;
	}

	this->m_ItemPositionInfo[info.ItemIndex] = info;
}