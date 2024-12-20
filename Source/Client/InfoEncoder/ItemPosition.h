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

	void Init();

	void Load(char* path);

	void SetInfo(ITEM_POSITION_INFO info);

public:

	ITEM_POSITION_INFO m_ItemPositionInfo[MAX_ITEM];
};

extern CItemPosition gItemPosition;