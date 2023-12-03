#pragma once

class CItem
{
public:

	CItem();

	virtual ~CItem();

	void Init();

private:

	static void ItemConvert(ITEM* ip, BYTE Attribute1, BYTE Attribute2);

	static WORD myCalcMaxDurability(ITEM* ip, ITEM_ATTRIBUTE* p, int Level);

	static DWORD ItemValue(ITEM* ip, int goldType);

	static DWORD ConvertRepairGold(int Gold, int Durability, int MaxDurability, short Type, char* Text);

	void ConvertGold(double dGold, char* szText, int iDecimals = 0);
};

extern CItem gItem;