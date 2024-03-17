#pragma once

class CItem
{
public:

	CItem();

	virtual ~CItem();

	void Init();

private:

	static void ItemConvert(ITEM* ip, BYTE Attribute1, BYTE Attribute2);

	static void InsertOptionText();

	static int OptionAddExcellentDamageRate(char* _Dest, const char* _Format);

	static int OptionAddPhysiDamageByLevel(char* _Dest, const char* _Format);

	static int OptionMulPhysiDamage(char* _Dest, const char* _Format);

	static int OptionAddMagicDamageByLevel(char* _Dest, const char* _Format);

	static int OptionAddHuntHP(char* _Dest, const char* _Format);

	static int OptionAddHuntMP(char* _Dest, const char* _Format);

	static WORD myCalcMaxDurability(ITEM* ip, ITEM_ATTRIBUTE* p, int Level);

	static DWORD ItemValue(ITEM* ip, int goldType);

	static DWORD ConvertRepairGold(int Gold, int Durability, int MaxDurability, short Type, char* Text);
};

extern CItem gItem;