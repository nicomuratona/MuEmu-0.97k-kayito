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

bool CItemValue::GetItemValue(ITEM* lpItem, int* value)
{
	for (std::vector<ITEM_VALUE>::iterator it = this->m_ItemValueInfo.begin(); it != this->m_ItemValueInfo.end(); it++)
	{
		if (it->Index == lpItem->Type)
		{
			if (it->Level == 0xFF || it->Level == ((lpItem->Level >> 3) & 15))
			{
				if (it->Grade == 0xFF || it->Grade == (lpItem->Option1 & 63))
				{
					if (gItemStack.GetItemMaxStack(it->Index, ((lpItem->Level >> 3) & 15)) == 0 || it->Index == GET_ITEM(4, 7) || it->Index == GET_ITEM(4, 15))
					{
						(*value) = it->Value;

						return true;
					}
					else
					{
						(*value) = (int)(it->Value * lpItem->Durability);

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
		ITEM_VALUE* lpInfo = (ITEM_VALUE*)(((BYTE*)lpMsg) + sizeof(PMSG_ITEM_VALUE_LIST_RECV) + (sizeof(ITEM_VALUE) * n));

		ITEM_VALUE info;

		info.Index = lpInfo->Index;

		info.Level = lpInfo->Level;

		info.Grade = lpInfo->Grade;

		info.Value = lpInfo->Value;

		this->m_ItemValueInfo.push_back(info);
	}
}