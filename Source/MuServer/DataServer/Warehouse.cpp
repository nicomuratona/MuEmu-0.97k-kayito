#include "stdafx.h"
#include "Warehouse.h"
#include "QueryManager.h"
#include "SocketManager.h"

CWarehouse gWarehouse;

CWarehouse::CWarehouse()
{

}

CWarehouse::~CWarehouse()
{

}

void CWarehouse::GDWarehouseItemRecv(SDHP_WAREHOUSE_ITEM_RECV* lpMsg, int index)
{
	SDHP_WAREHOUSE_ITEM_SEND pMsg;

	pMsg.header.set(0x02, 0x02, sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account, lpMsg->account, sizeof(pMsg.account));

	if (lpMsg->WarehouseNumber == 0)
	{
		if (gQueryManager.ExecQuery("SELECT AccountID FROM warehouse WHERE AccountID='%s'", lpMsg->account) == false || gQueryManager.Fetch() == SQL_NO_DATA)
		{
			gQueryManager.Close();

			gQueryManager.ExecQuery("INSERT INTO warehouse (AccountID,Money,EndUseDate,DbVersion) VALUES ('%s',0,getdate(),3)", lpMsg->account);

			gQueryManager.Close();

			gQueryManager.ExecQuery("UPDATE warehouse SET Items=CONVERT(varbinary(%d),REPLICATE(char(0xFF),%d)) WHERE AccountID='%s'", sizeof(pMsg.WarehouseItem), sizeof(pMsg.WarehouseItem), lpMsg->account);

			gQueryManager.Close();

			this->DGWarehouseFreeSend(index, lpMsg->index, lpMsg->account);

			return;
		}
		else
		{
			gQueryManager.Close();

			if (gQueryManager.ExecQuery("SELECT Items,Money,pw FROM warehouse WHERE AccountID='%s'", lpMsg->account) == false || gQueryManager.Fetch() == SQL_NO_DATA)
			{
				gQueryManager.Close();

				memset(pMsg.WarehouseItem, 0xFF, sizeof(pMsg.WarehouseItem));

				pMsg.WarehouseMoney = 0;

				pMsg.WarehousePassword = 0;
			}
			else
			{
				gQueryManager.GetAsBinary("Items", pMsg.WarehouseItem[0], sizeof(pMsg.WarehouseItem));

				pMsg.WarehouseMoney = gQueryManager.GetAsInteger("Money");

				pMsg.WarehousePassword = gQueryManager.GetAsInteger("pw");

				gQueryManager.Close();
			}
		}
	}
	else
	{
		if (gQueryManager.ExecQuery("SELECT AccountID FROM ExtWarehouse WHERE AccountID='%s' AND Number=%d", lpMsg->account, lpMsg->WarehouseNumber) == false || gQueryManager.Fetch() == SQL_NO_DATA)
		{
			gQueryManager.Close();

			gQueryManager.ExecQuery("INSERT INTO ExtWarehouse (AccountID,Money,Number) VALUES ('%s',0,%d)", lpMsg->account, lpMsg->WarehouseNumber);

			gQueryManager.Close();

			gQueryManager.ExecQuery("UPDATE ExtWarehouse SET Items=CONVERT(varbinary(%d),REPLICATE(char(0xFF),%d)) WHERE AccountID='%s' AND Number=%d", sizeof(pMsg.WarehouseItem), sizeof(pMsg.WarehouseItem), lpMsg->account, lpMsg->WarehouseNumber);

			gQueryManager.Close();

			this->DGWarehouseFreeSend(index, lpMsg->index, lpMsg->account);

			return;
		}
		else
		{
			gQueryManager.Close();

			if (gQueryManager.ExecQuery("SELECT Items,Money FROM ExtWarehouse WHERE AccountID='%s' AND Number=%d", lpMsg->account, lpMsg->WarehouseNumber) == false || gQueryManager.Fetch() == SQL_NO_DATA)
			{
				gQueryManager.Close();

				memset(pMsg.WarehouseItem, 0xFF, sizeof(pMsg.WarehouseItem));

				pMsg.WarehouseMoney = 0;
			}
			else
			{
				gQueryManager.GetAsBinary("Items", pMsg.WarehouseItem[0], sizeof(pMsg.WarehouseItem));

				pMsg.WarehouseMoney = gQueryManager.GetAsInteger("Money");

				gQueryManager.Close();

				if (gQueryManager.ExecQuery("SELECT pw FROM warehouse WHERE AccountID='%s'", lpMsg->account) == false || gQueryManager.Fetch() == SQL_NO_DATA)
				{
					gQueryManager.Close();

					pMsg.WarehousePassword = 0;
				}
				else
				{
					pMsg.WarehousePassword = gQueryManager.GetAsInteger("pw");

					gQueryManager.Close();
				}
			}
		}
	}

	gSocketManager.DataSend(index, (BYTE*)&pMsg, sizeof(pMsg));
}

void CWarehouse::GDWarehouseItemSaveRecv(SDHP_WAREHOUSE_ITEM_SAVE_RECV* lpMsg)
{
	if (lpMsg->WarehouseNumber == 0)
	{
		gQueryManager.BindParameterAsBinary(1, lpMsg->WarehouseItem[0], sizeof(lpMsg->WarehouseItem));

		gQueryManager.ExecQuery("UPDATE warehouse SET Items=?,Money=%d WHERE AccountID='%s'", lpMsg->WarehouseMoney, lpMsg->account);

		gQueryManager.Close();

		gQueryManager.ExecQuery("UPDATE warehouse SET pw=%d WHERE AccountID='%s'", lpMsg->WarehousePassword, lpMsg->account);

		gQueryManager.Close();
	}
	else
	{
		gQueryManager.BindParameterAsBinary(1, lpMsg->WarehouseItem[0], sizeof(lpMsg->WarehouseItem));

		gQueryManager.ExecQuery("UPDATE ExtWarehouse SET Items=?,Money=%d WHERE AccountID='%s' AND Number=%d", lpMsg->WarehouseMoney, lpMsg->account, lpMsg->WarehouseNumber);

		gQueryManager.Close();

		gQueryManager.ExecQuery("UPDATE warehouse SET pw=%d WHERE AccountID='%s'", lpMsg->WarehousePassword, lpMsg->account);

		gQueryManager.Close();
	}
}

void CWarehouse::DGWarehouseFreeSend(int ServerIndex, WORD index, char* account)
{
	SDHP_WAREHOUSE_FREE_SEND pMsg;

	pMsg.header.set(0x02, 0x03, sizeof(pMsg));

	pMsg.index = index;

	memcpy(pMsg.account, account, sizeof(pMsg.account));

	gSocketManager.DataSend(ServerIndex, (BYTE*)&pMsg, pMsg.header.size);
}