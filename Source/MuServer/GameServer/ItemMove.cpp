#include "stdafx.h"
#include "ItemMove.h"
#include "ItemManager.h"
#include "ReadScript.h"
#include "Util.h"

CItemMove gItemMove;

CItemMove::CItemMove()
{
	this->m_ItemMoveInfo.clear();
}

CItemMove::~CItemMove()
{

}

void CItemMove::Load(char* path)
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

	this->m_ItemMoveInfo.clear();

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

			ITEM_MOVE_INFO info;

			info.Index = lpReadScript->GetNumber();

			info.Index = SafeGetItem(GET_ITEM(info.Index, lpReadScript->GetAsNumber()));

			info.AllowDrop = lpReadScript->GetAsNumber();

			info.AllowSell = lpReadScript->GetAsNumber();

			info.AllowTrade = lpReadScript->GetAsNumber();

			info.AllowVault = lpReadScript->GetAsNumber();

			this->m_ItemMoveInfo.insert(std::pair<int, ITEM_MOVE_INFO>(info.Index, info));
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript->GetError());
	}

	delete lpReadScript;
}

bool CItemMove::CheckItemMoveAllowDrop(int index)
{
	std::map<int, ITEM_MOVE_INFO>::iterator it = this->m_ItemMoveInfo.find(index);

	if (it == this->m_ItemMoveInfo.end())
	{
		return 1;
	}
	else
	{
		return ((it->second.AllowDrop == 0) ? 0 : 1);
	}
}

bool CItemMove::CheckItemMoveAllowSell(int index)
{
	std::map<int, ITEM_MOVE_INFO>::iterator it = this->m_ItemMoveInfo.find(index);

	if (it == this->m_ItemMoveInfo.end())
	{
		return 1;
	}
	else
	{
		return ((it->second.AllowSell == 0) ? 0 : 1);
	}
}

bool CItemMove::CheckItemMoveAllowTrade(int index)
{
	std::map<int, ITEM_MOVE_INFO>::iterator it = this->m_ItemMoveInfo.find(index);

	if (it == this->m_ItemMoveInfo.end())
	{
		return 1;
	}
	else
	{
		return ((it->second.AllowTrade == 0) ? 0 : 1);
	}
}

bool CItemMove::CheckItemMoveAllowVault(int index)
{
	std::map<int, ITEM_MOVE_INFO>::iterator it = this->m_ItemMoveInfo.find(index);

	if (it == this->m_ItemMoveInfo.end())
	{
		return 1;
	}
	else
	{
		return ((it->second.AllowVault == 0) ? 0 : 1);
	}
}