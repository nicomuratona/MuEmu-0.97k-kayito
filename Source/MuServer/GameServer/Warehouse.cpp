#include "stdafx.h"
#include "Warehouse.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "NpcTalk.h"
#include "Shop.h"
#include "Util.h"

CWarehouse gWarehouse;

CWarehouse::CWarehouse()
{

}

CWarehouse::~CWarehouse()
{

}

int CWarehouse::GetWarehouseTaxMoney(int level, int lock)
{
	int tax = (int)(((level * level) * 0.1) * 0.4);

	if (lock != 0)
	{
		tax += (level * 2);
	}

	if (tax <= 0)
	{
		tax = 1;
	}

	if (tax >= 1000)
	{
		tax = (tax / 100) * 100;
	}
	else if (tax >= 100)
	{
		tax = (tax / 10) * 10;
	}

	return tax;
}

void CWarehouse::CGWarehouseMoneyRecv(PMSG_WAREHOUSE_MONEY_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_WAREHOUSE || lpObj->Interface.state == 0)
	{
		return;
	}

	if (lpMsg->type != 0 && lpMsg->type != 1)
	{
		return;
	}

	if (lpMsg->type == 0)
	{
		if (lpMsg->money <= 0 || lpMsg->money > MAX_WAREHOUSE_MONEY)
		{
			this->GCWarehouseMoneySend(aIndex, 0, 0, 0);

			return;
		}

		if ((lpObj->WarehouseMoney + lpMsg->money) > MAX_WAREHOUSE_MONEY)
		{
			this->GCWarehouseMoneySend(aIndex, 0, 0, 0);

			return;
		}

		if (lpMsg->money > lpObj->Money)
		{
			this->GCWarehouseMoneySend(aIndex, 0, 0, 0);

			return;
		}

		lpObj->Money -= lpMsg->money;

		lpObj->WarehouseMoney += lpMsg->money;
	}
	else if (lpMsg->type == 1)
	{
		if (lpObj->WarehouseLock == 1)
		{
			this->GCWarehouseMoneySend(aIndex, 0, 0, 0);

			return;
		}

		if (lpMsg->money <= 0 || lpMsg->money > MAX_WAREHOUSE_MONEY)
		{
			this->GCWarehouseMoneySend(aIndex, 0, 0, 0);

			return;
		}

		if (lpMsg->money > ((DWORD)lpObj->WarehouseMoney))
		{
			this->GCWarehouseMoneySend(aIndex, 0, 0, 0);

			return;
		}

		if ((lpObj->Money + lpMsg->money) > MAX_MONEY)
		{
			this->GCWarehouseMoneySend(aIndex, 0, 0, 0);

			return;
		}

		lpObj->WarehouseMoney -= lpMsg->money;

		lpObj->Money += lpMsg->money;

		int tax = this->GetWarehouseTaxMoney(lpObj->Level, lpObj->WarehousePW);

		if (lpObj->Money >= ((DWORD)tax))
		{
			lpObj->Money -= tax;
		}
		else if (lpObj->WarehouseMoney >= tax)
		{
			lpObj->WarehouseMoney -= tax;
		}
		else
		{
			lpObj->WarehouseMoney += lpMsg->money;

			lpObj->Money -= lpMsg->money;

			this->GCWarehouseMoneySend(aIndex, 0, 0, 0);

			return;
		}
	}

	lpObj->WarehouseCount++;

	this->GCWarehouseMoneySend(aIndex, 1, lpObj->Money, lpObj->WarehouseMoney);
}

void CWarehouse::CGWarehouseClose(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_WAREHOUSE || lpObj->Interface.state == 0)
	{
		return;
	}

	if (lpObj->LoadWarehouse == 0)
	{
		return;
	}

	GDInventoryItemSaveSend(aIndex);

	this->GDWarehouseItemSaveSend(aIndex);

	memset(lpObj->WarehouseMap, 0xFF, WAREHOUSE_SIZE);

	for (int n = 0; n < WAREHOUSE_SIZE; n++)
	{
		lpObj->Warehouse[n].Clear();
	}

	lpObj->WarehouseMoney = 0;

	lpObj->Interface.use = 0;

	lpObj->Interface.type = INTERFACE_NONE;

	lpObj->Interface.state = 0;

	lpObj->LoadWarehouse = 0;

	PBMSG_HEAD pMsg;

	pMsg.set(0x82, sizeof(pMsg));

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.size);
}

void CWarehouse::CGWarehousePasswordRecv(PMSG_WAREHOUSE_PASSWORD_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_WAREHOUSE || lpObj->Interface.state == 0)
	{
		return;
	}

	if (lpMsg->type == 0)
	{
		if (lpObj->WarehouseLock != 0)
		{
			if (lpObj->WarehousePW == lpMsg->password)
			{
				lpObj->WarehouseLock = 0;

				this->GCWarehouseStateSend(aIndex, 12);
			}
			else
			{
				this->GCWarehouseStateSend(aIndex, 10);
			}
		}
	}
	else if (lpMsg->type == 1)
	{
		if (lpObj->WarehouseLock == 0 || lpObj->WarehouseLock == 0xFF)
		{
			if (gObjCheckPersonalCode(aIndex, lpMsg->PersonalCode) == 0)
			{
				this->GCWarehouseStateSend(aIndex, 13);

				return;
			}

			lpObj->WarehousePW = lpMsg->password;

			lpObj->WarehouseLock = 1;

			this->GCWarehouseStateSend(aIndex, 1);
		}
		else
		{
			this->GCWarehouseStateSend(aIndex, 11);
		}
	}
	else if (lpMsg->type == 2)
	{
		if (gObjCheckPersonalCode(aIndex, lpMsg->PersonalCode) == 0)
		{
			this->GCWarehouseStateSend(aIndex, 13);

			return;
		}

		lpObj->WarehousePW = 0;

		lpObj->WarehouseLock = 0;

		this->GCWarehouseStateSend(aIndex, 0);
	}
}

void CWarehouse::GCWarehouseListSend(LPOBJ lpObj)
{
	BYTE send[4096];

	PMSG_SHOP_ITEM_LIST_SEND pMsg;

	pMsg.header.set(0x31, 0);

	int size = sizeof(pMsg);

	pMsg.type = 0;

	pMsg.count = 0;

	PMSG_SHOP_ITEM_LIST info;

	for (int n = 0; n < WAREHOUSE_SIZE; n++)
	{
		if (lpObj->Warehouse[n].IsItem() == 0)
		{
			continue;
		}

		info.slot = n;

		gItemManager.ItemByteConvert(info.ItemInfo, lpObj->Warehouse[n]);

		memcpy(&send[size], &info, sizeof(info));

		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(lpObj->Index, send, size);

	this->GCWarehouseMoneySend(lpObj->Index, 1, lpObj->Money, lpObj->WarehouseMoney);
}

void CWarehouse::GCWarehouseMoneySend(int aIndex, BYTE result, int InventoryMoney, int WarehouseMoney)
{
	PMSG_WAREHOUSE_MONEY_SEND pMsg;

	pMsg.header.set(0x81, sizeof(pMsg));

	pMsg.result = result;

	pMsg.WarehouseMoney = WarehouseMoney;

	pMsg.InventoryMoney = InventoryMoney;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CWarehouse::GCWarehouseStateSend(int aIndex, BYTE state)
{
	PMSG_WAREHOUSE_STATE_SEND pMsg;

	pMsg.header.set(0x83, sizeof(pMsg));

	pMsg.state = state;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CWarehouse::DGWarehouseItemRecv(SDHP_WAREHOUSE_ITEM_RECV* lpMsg)
{
	if (gObjIsAccountValid(lpMsg->index, lpMsg->account) == 0)
	{
		LogAdd(LOG_RED, "[DGWarehouseItemRecv] Invalid Account [%d](%s)", lpMsg->index, lpMsg->account);

		CloseClient(lpMsg->index);

		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_WAREHOUSE || lpObj->Interface.state != 0)
	{
		return;
	}

	if (lpObj->LoadWarehouse != 0)
	{
		return;
	}

	memset(lpObj->WarehouseMap, 0xFF, WAREHOUSE_SIZE);

	for (int n = 0; n < WAREHOUSE_SIZE; n++)
	{
		CItem item;

		lpObj->Warehouse[n].Clear();

		if (gItemManager.ConvertItemByte(&item, lpMsg->WarehouseItem[n]) != 0)
		{
			gItemManager.WarehouseAddItem(lpObj->Index, item, n);
		}
	}

	lpObj->WarehouseMoney = lpMsg->WarehouseMoney;

	lpObj->WarehousePW = lpMsg->WarehousePassword;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30, sizeof(pMsg));

	pMsg.result = 2;

	DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);

	gWarehouse.GCWarehouseListSend(lpObj);

	gWarehouse.GCWarehouseStateSend(lpObj->Index, ((lpObj->WarehousePW > 0) ? ((lpObj->WarehouseLock == 0) ? 12 : 1) : 0));

	lpObj->Interface.use = 1;

	lpObj->Interface.type = INTERFACE_WAREHOUSE;

	lpObj->Interface.state = 1;

	lpObj->LoadWarehouse = 1;
}

void CWarehouse::DGWarehouseFreeRecv(SDHP_WAREHOUSE_FREE_RECV* lpMsg)
{
	if (gObjIsAccountValid(lpMsg->index, lpMsg->account) == 0)
	{
		LogAdd(LOG_RED, "[DGWarehouseFreeRecv] Invalid Account [%d](%s)", lpMsg->index, lpMsg->account);

		CloseClient(lpMsg->index);

		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_WAREHOUSE || lpObj->Interface.state != 0)
	{
		return;
	}

	if (lpObj->LoadWarehouse != 0)
	{
		return;
	}

	memset(lpObj->WarehouseMap, 0xFF, WAREHOUSE_SIZE);

	for (int n = 0; n < WAREHOUSE_SIZE; n++)
	{
		lpObj->Warehouse[n].Clear();
	}

	lpObj->WarehouseMoney = 0;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30, sizeof(pMsg));

	pMsg.result = 2;

	DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);

	gWarehouse.GCWarehouseMoneySend(lpObj->Index, 1, lpObj->Money, lpObj->WarehouseMoney);

	lpObj->Interface.use = 1;

	lpObj->Interface.type = INTERFACE_WAREHOUSE;

	lpObj->Interface.state = 1;

	lpObj->LoadWarehouse = 1;
}

void CWarehouse::GDWarehouseItemSend(int aIndex, char* account)
{
	if (gObjIsAccountValid(aIndex, gObj[aIndex].Account) == 0)
	{
		return;
	}

	if (gObj[aIndex].LoadWarehouse != 0)
	{
		return;
	}

	SDHP_WAREHOUSE_ITEM_SEND pMsg;

	pMsg.header.set(0x02, 0x02, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, account, sizeof(pMsg.account));

	pMsg.WarehouseNumber = gObj[aIndex].WarehouseNumber;

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void CWarehouse::GDWarehouseItemSaveSend(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->LoadWarehouse == 0)
	{
		return;
	}

	SDHP_WAREHOUSE_ITEM_SAVE_SEND pMsg;

	pMsg.header.set(0x02, 0x04, sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account, lpObj->Account, sizeof(pMsg.account));

	for (int n = 0; n < WAREHOUSE_SIZE; n++)
	{
		gItemManager.DBItemByteConvert(pMsg.WarehouseItem[n], &lpObj->Warehouse[n]);
	}

	pMsg.WarehouseMoney = lpObj->WarehouseMoney;

	pMsg.WarehousePassword = lpObj->WarehousePW;

	pMsg.WarehouseNumber = lpObj->WarehouseNumber;

	gDataServerConnection.DataSend((BYTE*)&pMsg, sizeof(pMsg));
}