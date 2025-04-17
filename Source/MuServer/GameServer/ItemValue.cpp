#include "stdafx.h"
#include "ItemValue.h"
#include "ItemManager.h"
#include "ItemStack.h"
#include "ReadScript.h"
#include "Util.h"

CItemValue gItemValue;

CItemValue::CItemValue()
{
	this->m_ItemValueInfo.clear();
}

CItemValue::~CItemValue()
{

}

void CItemValue::Load(char* path)
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

	this->m_ItemValueInfo.clear();

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

			ITEM_VALUE_INFO info;

			info.Index = lpReadScript->GetNumber();

			info.Index = SafeGetItem(GET_ITEM(info.Index, lpReadScript->GetAsNumber()));

			info.Level = lpReadScript->GetAsNumber();

			info.BuyValue = lpReadScript->GetAsNumber();

			info.SellValue = lpReadScript->GetAsNumber();

			this->m_ItemValueInfo.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript->GetError());
	}

	delete lpReadScript;
}

bool CItemValue::GetItemBuyValue(CItem* lpItem, ULONGLONG* value)
{
	for (std::vector<ITEM_VALUE_INFO>::iterator it = this->m_ItemValueInfo.begin(); it != this->m_ItemValueInfo.end(); it++)
	{
		if (it->Index == lpItem->m_Index)
		{
			if (it->Level == -1 || it->Level == lpItem->m_Level)
			{
				if (it->BuyValue != -1)
				{
					if (gItemStack.GetItemMaxStack(it->Index, lpItem->m_Level) == 0 || it->Index == GET_ITEM(4, 7) || it->Index == GET_ITEM(4, 15))
					{
						(*value) = (ULONGLONG)it->BuyValue;

						return true;
					}
					else
					{
						(*value) = (ULONGLONG)(it->BuyValue * lpItem->m_Durability);

						return true;
					}
				}
			}
		}
	}

	return false;
}

bool CItemValue::GetItemSellValue(CItem* lpItem, ULONGLONG* value)
{
	for (std::vector<ITEM_VALUE_INFO>::iterator it = this->m_ItemValueInfo.begin(); it != this->m_ItemValueInfo.end(); it++)
	{
		if (it->Index == lpItem->m_Index)
		{
			if (it->Level == -1 || it->Level == lpItem->m_Level)
			{
				if (it->SellValue != -1)
				{
					if (gItemStack.GetItemMaxStack(it->Index, lpItem->m_Level) == 0 || it->Index == GET_ITEM(4, 7) || it->Index == GET_ITEM(4, 15))
					{
						(*value) = (ULONGLONG)it->SellValue;

						return true;
					}
					else
					{
						(*value) = (ULONGLONG)(it->SellValue * lpItem->m_Durability);

						return true;
					}
				}
			}
		}
	}

	return false;
}

void CItemValue::GCItemValueListSend(int aIndex)
{
	BYTE send[2048];

	PMSG_ITEM_VALUE_LIST_SEND pMsg;

	pMsg.header.set(0xF3, 0xE4, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	ITEM_VALUE_INFO info;

	for (std::vector<ITEM_VALUE_INFO>::iterator it = this->m_ItemValueInfo.begin(); it != this->m_ItemValueInfo.end(); it++)
	{
		info.Index = it->Index;

		info.Level = it->Level;

		info.BuyValue = it->BuyValue;

		info.SellValue = it->SellValue;

		memcpy(&send[size], &info, sizeof(info));

		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(aIndex, send, size);
}