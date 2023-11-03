#include "stdafx.h"
#include "ShopManager.h"
#include "MemScript.h"
#include "NpcTalk.h"
#include "Path.h"
#include "Util.h"

CShopManager gShopManager;

CShopManager::CShopManager()
{
	this->m_ShopManagerInfo.clear();
}

CShopManager::~CShopManager()
{

}

void CShopManager::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());

		delete lpMemScript;

		return;
	}

	this->m_ShopManagerInfo.clear();

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

			SHOP_MANAGER_INFO info;

			info.Index = lpMemScript->GetNumber();

			info.MonsterClass = lpMemScript->GetAsNumber();

			this->m_ShopManagerInfo.insert(std::pair<int, SHOP_MANAGER_INFO>(info.Index, info));
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CShopManager::LoadShop()
{
	std::map<int, int> LoadShop;

	char wildcard_path[MAX_PATH];

	wsprintf(wildcard_path, "%s*", gPath.GetFullPath("Shop\\"));

	WIN32_FIND_DATA data;

	HANDLE file = FindFirstFile(wildcard_path, &data);

	if (file == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			if (isdigit(data.cFileName[0]) != 0 && isdigit(data.cFileName[1]) != 0 && isdigit(data.cFileName[2]) != 0)
			{
				if (data.cFileName[3] == ' ' && data.cFileName[4] == '-' && data.cFileName[5] == ' ')
				{
					std::map<int, SHOP_MANAGER_INFO>::iterator it = this->m_ShopManagerInfo.find(atoi(data.cFileName));

					if (it != this->m_ShopManagerInfo.end())
					{
						if (LoadShop.find(it->first) == LoadShop.end())
						{
							char path[MAX_PATH];

							wsprintf(path, "Shop\\%s", data.cFileName);

							it->second.Shop.Load(gPath.GetFullPath(path));

							LoadShop.insert(std::pair<int, int>(it->first, 1));
						}
					}
				}
			}
		}
	}
	while (FindNextFile(file, &data) != 0);
}

void CShopManager::ReloadShopInterface()
{
	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnectedGP(n) != 0)
		{
			if (gObj[n].Interface.use != 0 && gObj[n].Interface.type == INTERFACE_SHOP)
			{
				PMSG_NPC_TALK_SEND pMsg;

				pMsg.header.setE(0x30, sizeof(pMsg));

				pMsg.result = 0;

				DataSend(n, (BYTE*)&pMsg, pMsg.header.size);

				this->GCShopItemListSendByIndex(gObj[n].TargetShopNumber, n);
			}
		}
	}
}

long CShopManager::GetShopNumber(int MonsterClass)
{
	for (std::map<int, SHOP_MANAGER_INFO>::iterator it = this->m_ShopManagerInfo.begin(); it != this->m_ShopManagerInfo.end(); it++)
	{
		if (it->second.MonsterClass != -1 && it->second.MonsterClass == MonsterClass)
		{
			return it->second.Index;
		}
	}

	return -1;
}

bool CShopManager::GetItemByIndex(int index, CItem* lpItem, int slot)
{
	std::map<int, SHOP_MANAGER_INFO>::iterator it = this->m_ShopManagerInfo.find(index);

	if (it == this->m_ShopManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return it->second.Shop.GetItem(lpItem, slot);
	}
}

bool CShopManager::GetItemByMonsterClass(int MonsterClass, CItem* lpItem, int slot)
{
	for (std::map<int, SHOP_MANAGER_INFO>::iterator it = this->m_ShopManagerInfo.begin(); it != this->m_ShopManagerInfo.end(); it++)
	{
		if (it->second.MonsterClass != -1 && it->second.MonsterClass == MonsterClass)
		{
			return it->second.Shop.GetItem(lpItem, slot);
		}
	}

	return 0;
}

long CShopManager::GetItemCountByIndex(int index)
{
	std::map<int, SHOP_MANAGER_INFO>::iterator it = this->m_ShopManagerInfo.find(index);

	if (it == this->m_ShopManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return it->second.Shop.GetItemCount();
	}
}

long CShopManager::GetItemCountByMonsterClass(int MonsterClass)
{
	for (std::map<int, SHOP_MANAGER_INFO>::iterator it = this->m_ShopManagerInfo.begin(); it != this->m_ShopManagerInfo.end(); it++)
	{
		if (it->second.MonsterClass != -1 && it->second.MonsterClass == MonsterClass)
		{
			return it->second.Shop.GetItemCount();
		}
	}

	return 0;
}

bool CShopManager::GCShopItemListSendByIndex(int index, int aIndex)
{
	std::map<int, SHOP_MANAGER_INFO>::iterator it = this->m_ShopManagerInfo.find(index);

	if (it == this->m_ShopManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return it->second.Shop.GCShopItemListSend(aIndex);
	}
}

bool CShopManager::GCShopItemListSendByMonsterClass(int MonsterClass, int aIndex)
{
	for (std::map<int, SHOP_MANAGER_INFO>::iterator it = this->m_ShopManagerInfo.begin(); it != this->m_ShopManagerInfo.end(); it++)
	{
		if (it->second.MonsterClass != -1 && it->second.MonsterClass == MonsterClass)
		{
			return it->second.Shop.GCShopItemListSend(aIndex);
		}
	}

	return 0;
}