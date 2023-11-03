#pragma once

class CRightClickMove
{
public:

	CRightClickMove();

	virtual ~CRightClickMove();

	void Init();

private:

	static bool MoveItemToInterface(int MousePosX, int MousePosY, DWORD InterfaceOffset, int InterfaceWidth, int InterfaceHeight);

	static void CheckEquipItem();

	static void CheckRighClickToPickItem();

	static void CheckUnequipItem();
};

extern CRightClickMove gRightClickMove;