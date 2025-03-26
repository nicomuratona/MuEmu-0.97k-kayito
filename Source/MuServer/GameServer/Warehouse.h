#pragma once

#include "ProtocolDefines.h"
#include "User.h"

#define MAX_WAREHOUSE_MONEY 2000000000

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_WAREHOUSE_MONEY_RECV
{
	PBMSG_HEAD header; // C1:81
	BYTE type;
	DWORD money;
};

struct PMSG_WAREHOUSE_PASSWORD_RECV
{
	PBMSG_HEAD header; // C1:83
	BYTE type;
	WORD password;
	char PersonalCode[10];
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_WAREHOUSE_MONEY_SEND
{
	PBMSG_HEAD header; // C1:81
	BYTE result;
	DWORD WarehouseMoney;
	DWORD InventoryMoney;
};

struct PMSG_WAREHOUSE_STATE_SEND
{
	PBMSG_HEAD header; // C1:83
	BYTE state;
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_WAREHOUSE_ITEM_RECV
{
	PSWMSG_HEAD header; // C2:02:02
	WORD index;
	char account[11];
	BYTE WarehouseItem[WAREHOUSE_SIZE][MAX_ITEM_BYTE_SIZE];
	UINT WarehouseMoney;
	WORD WarehousePassword;
};

struct SDHP_WAREHOUSE_FREE_RECV
{
	PSBMSG_HEAD header; // C1:02:03
	WORD index;
	char account[11];
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_WAREHOUSE_ITEM_SEND
{
	PSBMSG_HEAD header; // C1:02:02
	WORD index;
	char account[11];
	UINT WarehouseNumber;
};

struct SDHP_WAREHOUSE_ITEM_SAVE_SEND
{
	PSWMSG_HEAD header; // C2:02:04
	WORD index;
	char account[11];
	BYTE WarehouseItem[WAREHOUSE_SIZE][MAX_ITEM_BYTE_SIZE];
	UINT WarehouseMoney;
	UINT WarehousePassword;
	UINT WarehouseNumber;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CWarehouse
{
public:

	CWarehouse();

	~CWarehouse();

	int GetWarehouseTaxMoney(int level, int lock);

	void CGWarehouseMoneyRecv(PMSG_WAREHOUSE_MONEY_RECV* lpMsg, int aIndex);

	void CGWarehouseClose(int aIndex);

	void CGWarehousePasswordRecv(PMSG_WAREHOUSE_PASSWORD_RECV* lpMsg, int aIndex);

	void GCWarehouseListSend(LPOBJ lpObj);

	void GCWarehouseMoneySend(int aIndex, BYTE result, int InventoryMoney, int WarehouseMoney);

	void GCWarehouseStateSend(int aIndex, BYTE state);

	void DGWarehouseItemRecv(SDHP_WAREHOUSE_ITEM_RECV* lpMsg);

	void DGWarehouseFreeRecv(SDHP_WAREHOUSE_FREE_RECV* lpMsg);

	void GDWarehouseItemSend(int aIndex, char* account);

	void GDWarehouseItemSaveSend(int aIndex);
};

extern CWarehouse gWarehouse;