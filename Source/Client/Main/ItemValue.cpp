#include "stdafx.h"
#include "ItemValue.h"
#include "ItemStack.h"

CItemValue gItemValue;

CItemValue::CItemValue()
{
	this->m_ItemValueInfo.clear();
}

CItemValue::~CItemValue()
{

}

bool CItemValue::GetItemBuyValue(ITEM* lpItem, ULONGLONG* value)
{
	for (std::vector<ITEM_VALUE_INFO>::iterator it = this->m_ItemValueInfo.begin(); it != this->m_ItemValueInfo.end(); it++)
	{
		if (it->Index == lpItem->Type)
		{
			if (it->Level == -1 || it->Level == ((lpItem->Level >> 3) & 15))
			{
				if (it->BuyValue != -1)
				{
					if (gItemStack.GetItemMaxStack(it->Index, ((lpItem->Level >> 3) & 15)) == 0 || it->Index == GET_ITEM(4, 7) || it->Index == GET_ITEM(4, 15))
					{
						(*value) = (ULONGLONG)it->BuyValue;

						return true;
					}
					else
					{
						(*value) = (ULONGLONG)(it->BuyValue * lpItem->Durability);

						return true;
					}
				}
			}
		}
	}

	return false;
}

bool CItemValue::GetItemSellValue(ITEM* lpItem, ULONGLONG* value)
{
	for (std::vector<ITEM_VALUE_INFO>::iterator it = this->m_ItemValueInfo.begin(); it != this->m_ItemValueInfo.end(); it++)
	{
		if (it->Index == lpItem->Type)
		{
			if (it->Level == -1 || it->Level == ((lpItem->Level >> 3) & 15))
			{
				if (it->SellValue != -1)
				{
					if (gItemStack.GetItemMaxStack(it->Index, ((lpItem->Level >> 3) & 15)) == 0 || it->Index == GET_ITEM(4, 7) || it->Index == GET_ITEM(4, 15))
					{
						(*value) = (ULONGLONG)it->SellValue;

						return true;
					}
					else
					{
						(*value) = (ULONGLONG)(it->SellValue * lpItem->Durability);

						return true;
					}
				}
			}
		}
	}

	return false;
}

void CItemValue::GCItemValueListRecv(PMSG_ITEM_VALUE_LIST_RECV* lpMsg)
{
	this->m_ItemValueInfo.clear();

	for (int n = 0; n < lpMsg->count; n++)
	{
		ITEM_VALUE_INFO* lpInfo = (ITEM_VALUE_INFO*)(((BYTE*)lpMsg) + sizeof(PMSG_ITEM_VALUE_LIST_RECV) + (sizeof(ITEM_VALUE_INFO) * n));

		ITEM_VALUE_INFO info;

		info.Index = lpInfo->Index;

		info.Level = lpInfo->Level;

		info.BuyValue = lpInfo->BuyValue;

		info.SellValue = lpInfo->SellValue;

		this->m_ItemValueInfo.push_back(info);
	}
}