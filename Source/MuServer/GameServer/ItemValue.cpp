#include "stdafx.h"
#include "ItemValue.h"
#include "ItemManager.h"
#include "ItemStack.h"
#include "MemScript.h"
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
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == NULL)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (!lpMemScript->SetBuffer(path))
	{
		ErrorMessageBox(lpMemScript->GetLastError());

		delete lpMemScript;

		return;
	}

	this->m_ItemValueInfo.clear();

	try
	{
		eTokenResult token;

		while (true)
		{
			token = lpMemScript->GetToken();

			if (token == TOKEN_END || token == TOKEN_END_SECTION)
			{
				break;
			}

			ITEM_VALUE_INFO info;

			info.Index = lpMemScript->GetNumber();

			info.Index = SafeGetItem(GET_ITEM(info.Index, lpMemScript->GetAsNumber()));

			info.Level = lpMemScript->GetAsNumber();

			info.Grade = lpMemScript->GetAsNumber();

			info.Value = lpMemScript->GetAsNumber();

			this->m_ItemValueInfo.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CItemValue::GetItemValue(CItem* lpItem, int* value)
{
	for (std::vector<ITEM_VALUE_INFO>::iterator it = this->m_ItemValueInfo.begin(); it != this->m_ItemValueInfo.end(); it++)
	{
		if (it->Index == lpItem->m_Index)
		{
			if (it->Level == -1 || it->Level == lpItem->m_Level)
			{
				if (it->Grade == -1 || it->Grade == lpItem->m_ExceOption)
				{
					if (gItemStack.GetItemMaxStack(it->Index, lpItem->m_Level) == 0 || it->Index == GET_ITEM(4, 7) || it->Index == GET_ITEM(4, 15))
					{
						(*value) = it->Value;

						return true;
					}
					else
					{
						(*value) = (int)(it->Value * lpItem->m_Durability);

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

	ITEM_VALUE info;

	for (std::vector<ITEM_VALUE_INFO>::iterator it = this->m_ItemValueInfo.begin(); it != this->m_ItemValueInfo.end(); it++)
	{
		info.Index = GET_MAX_WORD_VALUE(it->Index);

		info.Level = (BYTE)it->Level;

		info.Grade = (BYTE)it->Grade;

		info.Value = it->Value;

		memcpy(&send[size], &info, sizeof(info));

		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(aIndex, send, size);
}