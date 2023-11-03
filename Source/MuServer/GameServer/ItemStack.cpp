#include "stdafx.h"
#include "ItemStack.h"
#include "ItemManager.h"
#include "MemScript.h"
#include "Util.h"

CItemStack gItemStack;

CItemStack::CItemStack()
{
	this->m_ItemStackInfo.clear();
}

CItemStack::~CItemStack()
{

}

void CItemStack::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (lpMemScript->SetBuffer(path) == false)
	{
		ErrorMessageBox(lpMemScript->GetLastError());

		delete lpMemScript;

		return;
	}

	this->m_ItemStackInfo.clear();

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

			ITEM_STACK_INFO info;

			info.ItemIndex = SafeGetItem(GET_ITEM(lpMemScript->GetNumber(), lpMemScript->GetAsNumber()));

			info.Level = lpMemScript->GetAsNumber();

			info.MaxStack = lpMemScript->GetAsNumber();

			info.CreateItemIndex = lpMemScript->GetAsNumber();

			if (info.CreateItemIndex != -1)
			{
				info.CreateItemIndex = SafeGetItem(GET_ITEM(info.CreateItemIndex, lpMemScript->GetAsNumber()));
			}

			this->m_ItemStackInfo.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

int CItemStack::GetItemMaxStack(int index, int level)
{
	for (std::vector<ITEM_STACK_INFO>::iterator it = this->m_ItemStackInfo.begin(); it != this->m_ItemStackInfo.end(); it++)
	{
		if (it->ItemIndex != index)
		{
			continue;
		}

		if (it->Level != -1 && it->Level != level)
		{
			continue;
		}

		return it->MaxStack;
	}

	return 0;
}

int CItemStack::GetCreateItemIndex(int index)
{
	for (std::vector<ITEM_STACK_INFO>::iterator it = this->m_ItemStackInfo.begin(); it != this->m_ItemStackInfo.end(); it++)
	{
		if (it->ItemIndex != index)
		{
			continue;
		}

		return it->CreateItemIndex;
	}

	return -1;
}

void CItemStack::GCItemStackListSend(int aIndex)
{
	BYTE send[2048];

	PMSG_ITEM_STACK_LIST_SEND pMsg;

	pMsg.header.set(0xF3, 0xE3, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	ITEM_STACK info;

	for (std::vector<ITEM_STACK_INFO>::iterator it = this->m_ItemStackInfo.begin(); it != this->m_ItemStackInfo.end(); it++)
	{
		info.ItemIndex = it->ItemIndex;

		info.Level = it->Level;

		info.MaxStack = it->MaxStack;

		memcpy(&send[size], &info, sizeof(info));

		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(aIndex, send, size);
}