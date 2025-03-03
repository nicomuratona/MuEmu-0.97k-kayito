#pragma once

class CItemManager
{
public:

	CItemManager();

	virtual ~CItemManager();

	int GetInventoryEmptySlot(int ItemWidth, int ItemHeight);

	bool CheckPickedItemOverlay(int MousePosX, int MousePosY, DWORD InterfaceOffset, int InterfaceWidth, int InterfaceHeight);

	static bool GetInterfaceEmptySlot(int MousePosX, int MousePosY, DWORD InterfaceOffset, int InterfaceWidth, int InterfaceHeight);

	BYTE InterfaceRectCheck(int x, int y, int width, int height, DWORD InterfaceOffset, int InterfaceWidth, int InterfaceHeight);

	int GetTargetSlotEquiped(ITEM* lpItem, int slot);

	void GetItemName(int iType, int iLevel, char* Text);

	void MyRenderItem3D(float sx, float sy, float Width, float Height, ITEM* Item, bool PickUp);

	int GetInventoryItemCount(int index, int level);

	int GetInventoryItemSlot(int index, int level);
};

extern CItemManager gItemManager;