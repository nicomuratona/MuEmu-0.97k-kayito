#pragma once

#define MAX_CUSTOM_WING 7

struct CUSTOM_WING_INFO
{
	int Index;
	int ItemIndex;
	int DefenseConstA;
	int IncDamageConstA;
	int IncDamageConstB;
	int DecDamageConstA;
	int DecDamageConstB;
};

class CCustomWing
{
public:

	CCustomWing();

	virtual ~CCustomWing();

	void Init(CUSTOM_WING_INFO* info);

	CUSTOM_WING_INFO* GetInfo(int Index);

	CUSTOM_WING_INFO* GetInfoByIndex(int ItemIndex);

	int GetCustomWingDefense(int ItemIndex, int ItemLevel);

	int GetCustomWingIncDamage(int ItemIndex, int ItemLevel);

	int GetCustomWingDecDamage(int ItemIndex, int ItemLevel);

private:

	void SetInfo(CUSTOM_WING_INFO info);

	void Init();

	// Funciones

	static void CustomWingRenderPartObjectEffect_SetLevel();

	static void CustomWingRenderPartObjectEffect_SetGlow();

	static void CustomWingRenderItemInfo_SetTextColor();

	static void CustomWingRenderItemInfo_SetName();

	static void CustomWingRenderItemInfo_SetIncDecDamage();

	static void CustomWingRenderItemInfo_SetDurability();

	static void CustomWingRenderItemName_SetName();

	static void CustomWingRenderRepairInfo_SetColor();

	static void CustomWingRenderRepairInfo_SetName();

	static void CustomWingCheckGate_SetMessage();

	static void CustomWing_InventoryCheck();

	static void CustomWing_EquipmentCheck();

	static void CustomWingInventoryEquipment_IcarusWingCount();

	static void CustomWingInventoryEquipment_PickCheck();

	static void CustomWingInventoryProcess_AllowInsertJewels();

	static void CustomWingChaosMix_AddToPlusItemMix();

	static void CustomWingIsHighValueItem_CheckWing();

	static void CustomWingChangeCharacterExt();

public:

	CUSTOM_WING_INFO m_CustomWingInfo[MAX_CUSTOM_WING];
};

extern CCustomWing gCustomWing;