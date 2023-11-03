#pragma once

#include "ItemManager.h"

class CMapItem : public CItem
{
public:

	CMapItem();

	~CMapItem();

	void Init();

	void CreateItem(int index, int level, int x, int y, float dur, BYTE SkillOption, BYTE LuckOption, BYTE AddOption, BYTE ExceOption, DWORD serial);

	void DropCreateItem(int index, int level, int x, int y, float dur, BYTE SkillOption, BYTE LuckOption, BYTE AddOption, BYTE ExceOption, DWORD serial);

public:

	BYTE m_X;

	BYTE m_Y;

	BYTE m_Live;

	BYTE m_Give;

	DWORD m_State;

	DWORD m_Time;

	int m_UserIndex;

	DWORD m_LootTime;
};