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

	int GetTargetSlotEquiped(short itemIndex, int slot);
};

extern CItemManager gItemManager;