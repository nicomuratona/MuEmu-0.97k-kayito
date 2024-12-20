#pragma once

struct ITEM_POSITION_INFO
{
	short ItemIndex;
	float PositionX;
	float PositionY;
	float RotationX;
	float RotationY;
	float RotationZ;
	float Scale;
};

class CItemPosition
{
public:

	CItemPosition();

	virtual ~CItemPosition();

	void Load(ITEM_POSITION_INFO* info);

private:

	void SetInfo(ITEM_POSITION_INFO info);

	ITEM_POSITION_INFO* GetInfoByIndex(int ItemIndex);

	void Init();

	static void MyRenderItem3D(float sx, float sy, float Width, float Height, int Type, int Level, int Option1, bool PickUp);

	void MyRenderObjectScreen(int Type, int ItemLevel, int Option1, float Target[3], int Select, bool PickUp);

private:

	ITEM_POSITION_INFO m_ItemPositionInfo[MAX_ITEM];
};

extern CItemPosition gItemPosition;