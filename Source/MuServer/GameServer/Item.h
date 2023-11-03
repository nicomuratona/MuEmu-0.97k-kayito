#pragma once

#define INVENTORY_WEAR_SIZE 12
#define INVENTORY_SIZE 76
#define TRADE_SIZE 32
#define WAREHOUSE_SIZE 120
#define CHAOS_BOX_SIZE 32
#define MAX_EXC_OPTION 6
#define MAX_SPECIAL_OPTION 8
#define MAX_RESISTANCE_TYPE 4
#define MAX_ITEM_PRICE 2000000000
#define MAX_ITEM_BYTE_SIZE 10

enum eSpecialNumber
{
	SPECIAL_SKILL_OPTION = 0, // SKILL
	SPECIAL_LUCK_OPTION = 1, // LUCK
	SPECIAL_ADDITIONAL_OPTION = 2, // ADDOPTION
	SPECIAL_EXCELLENT1 = 3,
	SPECIAL_EXCELLENT2 = 4,
	SPECIAL_EXCELLENT3 = 5,
	SPECIAL_EXCELLENT4 = 6,
	SPECIAL_EXCELLENT5 = 7,
	SPECIAL_EXCELLENT6 = 8,
};

class CItem
{
public:

	CItem();

	void Clear();

	bool IsItem();

	bool IsExcItem();

	bool IsClass(int Class, int ChangeUp);

	void Convert(int index, BYTE SkillOption, BYTE LuckOption, BYTE AddOption, BYTE ExceOption);

	void Value();

	void OldValue();

	int GetItemSkill();

	int GetDamageMin();

	int GetDamageMax();

	int GetDefense();

	int GetDefenseSuccessRate();

	int GetBookSuccessRate();

	bool WeaponDurabilityDown(int aIndex, int defense, int type);

	bool ArmorDurabilityDown(int aIndex, int damage);

	bool WingDurabilityDown(int aIndex, int decrease);

	bool PendantDurabilityDown(int aIndex, int decrease);

	bool RingDurabilityDown(int aIndex, int decrease);

	bool CheckDurabilityState();

public:

	DWORD m_Serial;

	short m_Index;

	short m_Level;

	BYTE m_Slot;

	BYTE m_Class;

	BYTE m_TwoHand;

	BYTE m_AttackSpeed;

	BYTE m_WalkSpeed;

	WORD m_DamageMin;

	WORD m_DamageMax;

	BYTE m_DefenseSuccessRate;

	WORD m_Defense;

	WORD m_MagicDefense;

	BYTE m_Speed;

	WORD m_BaseDamageMin;

	WORD m_BaseDefense;

	WORD m_MagicDamageRate;

	float m_Durability;

	float m_BaseDurability;

	WORD m_DurabilitySmall;

	WORD m_SpecialIndex[MAX_SPECIAL_OPTION];

	WORD m_SpecialValue[MAX_SPECIAL_OPTION];

	WORD m_RequireStrength;

	WORD m_RequireDexterity;

	WORD m_RequireEnergy;

	WORD m_RequireLevel;

	WORD m_RequireVitality;

	BYTE m_Resistance[MAX_RESISTANCE_TYPE];

	DWORD m_SellMoney;

	DWORD m_BuyMoney;

	int m_Value;

	int m_ItemExist;

	int m_OldSellMoney;

	int m_OldBuyMoney;

	BYTE m_SkillOption;

	BYTE m_LuckOption;

	BYTE m_AddOption;

	BYTE m_ExceOption;

	float m_DurabilityState[4];

	float m_CurrentDurabilityState;

	bool m_QuestItem;

	bool m_IsValidItem;

	BYTE m_OptionsQuant;
};