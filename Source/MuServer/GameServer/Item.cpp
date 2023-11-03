#include "stdafx.h"
#include "Item.h"
#include "ItemManager.h"
#include "ItemOption.h"
#include "ItemValue.h"
#include "ObjectManager.h"
#include "ServerInfo.h"
#include "SkillManager.h"
#include "Util.h"

CItem::CItem()
{
	this->Clear();
}

void CItem::Clear()
{
	this->m_Serial = 0;

	this->m_Index = -1;

	this->m_Level = 0;

	this->m_Slot = 0;

	this->m_Class = 0;

	this->m_TwoHand = 0;

	this->m_AttackSpeed = 0;

	this->m_DamageMin = 0;

	this->m_DamageMax = 0;

	this->m_DefenseSuccessRate = 0;

	this->m_Defense = 0;

	this->m_MagicDefense = 0;

	this->m_Durability = 0;

	this->m_DurabilitySmall = 0;

	memset(this->m_SpecialIndex, 0, sizeof(this->m_SpecialIndex));

	memset(this->m_SpecialValue, 0, sizeof(this->m_SpecialValue));

	memset(this->m_Resistance, 0, sizeof(this->m_Resistance));

	this->m_Value = 0;

	this->m_ItemExist = 1;

	this->m_SkillOption = 0;

	this->m_LuckOption = 0;

	this->m_AddOption = 0;

	this->m_ExceOption = 0;

	this->m_CurrentDurabilityState = 1.0f;

	this->m_QuestItem = 0;

	this->m_OptionsQuant = 0;
}

bool CItem::IsItem()
{
	if (CHECK_RANGE(this->m_Index, MAX_ITEM) == false)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

bool CItem::IsExcItem()
{
	if (this->m_Index == GET_ITEM(13, 3)) // Dinorant
	{
		return 0;
	}

	if (this->m_ExceOption == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

bool CItem::IsClass(int Class, int ChangeUp)
{
	if (CHECK_RANGE(Class, MAX_CLASS) == false)
	{
		return 0;
	}

	ITEM_INFO ItemInfo;

	if (gItemManager.GetInfo(this->m_Index, &ItemInfo) == 0)
	{
		return 0;
	}

	if (ItemInfo.RequireClass[Class] <= 0 || (ChangeUp + 1) < ItemInfo.RequireClass[Class])
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void CItem::Convert(int index, BYTE SkillOption, BYTE LuckOption, BYTE AddOption, BYTE ExceOption)
{
	ITEM_INFO ItemInfo;

	if (gItemManager.GetInfo(index, &ItemInfo) == 0)
	{
		return;
	}

	this->m_Index = index;

	this->m_Level &= 15;

	this->m_Slot = ItemInfo.Slot;

	this->m_SkillOption = SkillOption;

	this->m_LuckOption = LuckOption;

	this->m_AddOption = AddOption;

	this->m_ExceOption = ExceOption;

	if (this->m_Index == GET_ITEM(0, 19) || this->m_Index == GET_ITEM(2, 13) || this->m_Index == GET_ITEM(4, 18) || this->m_Index == GET_ITEM(5, 10)) // Sword of Archangel,Scepter of Archangel,Crossbow of Archangel,Staff of Archangel
	{
		ExceOption = 0;
	}

	if (this->m_Slot == 7)
	{
		ExceOption = 0;
	}

	if (this->m_Index == GET_ITEM(13, 3)) // Dinorant
	{
		ExceOption = 0;
	}

	if (this->m_Index == GET_ITEM(14, 23) || this->m_Index == GET_ITEM(14, 24) || this->m_Index == GET_ITEM(14, 25) || this->m_Index == GET_ITEM(14, 26))
	{
		this->m_QuestItem = 1;
	}
	else
	{
		this->m_QuestItem = 0;
	}

	if (gItemManager.GetItemDurability(this->m_Index, 0, 0) == 0)
	{
		this->m_BaseDurability = 0;
	}
	else
	{
		this->m_BaseDurability = (float)gItemManager.GetItemDurability(this->m_Index, this->m_Level, ExceOption);
	}

	this->m_Value = ItemInfo.Value;

	this->m_AttackSpeed = ItemInfo.AttackSpeed;

	this->m_TwoHand = ItemInfo.TwoHand;

	this->m_DamageMin = ItemInfo.DamageMin;

	this->m_DamageMax = ItemInfo.DamageMax;

	this->m_DefenseSuccessRate = ItemInfo.DefenseSuccessRate;

	this->m_Defense = ItemInfo.Defense;

	this->m_MagicDefense = ItemInfo.MagicDefense;

	this->m_WalkSpeed = ItemInfo.WalkSpeed;

	this->m_MagicDamageRate = ItemInfo.MagicDamageRate;

	this->m_DurabilityState[0] = this->m_BaseDurability * 0.5f;

	this->m_DurabilityState[1] = this->m_BaseDurability * 0.4f;

	this->m_DurabilityState[2] = this->m_BaseDurability * 0.3f;

	this->m_DurabilityState[3] = this->m_BaseDurability * 0.2f;

	this->m_CurrentDurabilityState = 1.0f;

	if (this->m_Durability == 0)
	{
		this->m_CurrentDurabilityState = 0.0f;
	}
	else if (this->m_Durability < this->m_DurabilityState[3])
	{
		this->m_CurrentDurabilityState = 0.5f;
	}
	else if (this->m_Durability < this->m_DurabilityState[2])
	{
		this->m_CurrentDurabilityState = 0.6f;
	}
	else if (this->m_Durability < this->m_DurabilityState[1])
	{
		this->m_CurrentDurabilityState = 0.7f;
	}
	else if (this->m_Durability < this->m_DurabilityState[0])
	{
		this->m_CurrentDurabilityState = 0.8f;
	}

	for (int n = 0; n < MAX_RESISTANCE_TYPE; n++)
	{
		this->m_Resistance[n] = (((ItemInfo.Resistance[n] * this->m_Level) > 255) ? 255 : (ItemInfo.Resistance[n] * this->m_Level));
	}

	int ItemLevel = ItemInfo.Level;

	if (ExceOption != 0)
	{
		ItemLevel += 25;
	}

	this->m_RequireStrength = 0;

	this->m_RequireDexterity = 0;

	this->m_RequireEnergy = 0;

	this->m_RequireLevel = 0;

	this->m_RequireVitality = 0;

	if (ItemInfo.RequireStrength != 0)
	{
		this->m_RequireStrength = (((ItemInfo.RequireStrength * ((this->m_Level * 3) + ItemLevel)) * 3) / 100) + 20;
	}

	if (ItemInfo.RequireDexterity != 0)
	{
		this->m_RequireDexterity = (((ItemInfo.RequireDexterity * ((this->m_Level * 3) + ItemLevel)) * 3) / 100) + 20;
	}

	if (ItemInfo.RequireVitality != 0)
	{
		this->m_RequireVitality = (((ItemInfo.RequireVitality * ((this->m_Level * 3) + ItemLevel)) * 3) / 100) + 20;
	}

	if ((this->m_Index / MAX_ITEM_TYPE) == 5 && ItemInfo.Slot == 1)
	{
		if (ItemInfo.RequireEnergy != 0)
		{
			this->m_RequireEnergy = (((ItemInfo.RequireEnergy * (this->m_Level + ItemLevel)) * 3) / 100) + 20;
		}
	}
	else
	{
		if (ItemInfo.RequireEnergy != 0)
		{
			this->m_RequireEnergy = (((ItemInfo.RequireEnergy * ((this->m_Level * 3) + ItemLevel)) * 4) / 100) + 20;
		}
	}

	if (ItemInfo.RequireLevel != 0)
	{
		if (this->m_Index >= GET_ITEM(0, 0) && this->m_Index < GET_ITEM(12, 0))
		{
			this->m_RequireLevel = ItemInfo.RequireLevel;
		}
		else if ((this->m_Index >= GET_ITEM(12, 3) && this->m_Index <= GET_ITEM(12, 6))) // Wings
		{
			this->m_RequireLevel = ItemInfo.RequireLevel + (this->m_Level * 5);
		}
		else if ((this->m_Index >= GET_ITEM(12, 7) && this->m_Index <= GET_ITEM(12, 19) && this->m_Index != GET_ITEM(12, 15))) // Orbs,Scrolls
		{
			this->m_RequireLevel = ItemInfo.RequireLevel;
		}
		else
		{
			this->m_RequireLevel = ItemInfo.RequireLevel + (this->m_Level * 4);
		}
	}

	if (this->m_Index == GET_ITEM(13, 10)) // Transformation Ring
	{
		if (ItemLevel <= 2)
		{
			this->m_RequireLevel = 20;
		}
		else
		{
			this->m_RequireLevel = 50;
		}
	}

	if (ExceOption != 0 && this->m_RequireLevel > 0)
	{
		if (this->m_Index >= GET_ITEM(12, 0))
		{
			this->m_RequireLevel += 20;
		}
	}

	int ChaosItem = 0;

	if (this->m_Index == GET_ITEM(2, 6))
	{
		ChaosItem = 15;
	}
	else if (this->m_Index == GET_ITEM(5, 7))
	{
		ChaosItem = 25;
	}
	else if (this->m_Index == GET_ITEM(4, 6))
	{
		ChaosItem = 30;
	}

	if (this->m_DamageMax > 0)
	{
		if (ExceOption != 0)
		{
			if (ChaosItem != 0)
			{
				this->m_DamageMax += ChaosItem;
			}
			else if (ItemInfo.Level != 0)
			{
				this->m_DamageMax += ((this->m_DamageMin * 25) / ItemInfo.Level) + 5;
			}
		}

		this->m_DamageMax += (this->m_Level * 3);

		if (this->m_Level >= 10)
		{
			this->m_DamageMax += ((this->m_Level - 9) * (this->m_Level - 8)) / 2;
		}
	}

	if (this->m_DamageMin > 0)
	{
		if (ExceOption != 0)
		{
			if (ChaosItem != 0)
			{
				this->m_DamageMin += ChaosItem;
			}
			else if (ItemInfo.Level != 0)
			{
				this->m_DamageMin += ((this->m_DamageMin * 25) / ItemInfo.Level) + 5;
			}
		}

		this->m_DamageMin += (this->m_Level * 3);

		if (this->m_Level >= 10)
		{
			this->m_DamageMin += ((this->m_Level - 9) * (this->m_Level - 8)) / 2;
		}
	}

	if (this->m_MagicDamageRate > 0)
	{
		if (ExceOption != 0)
		{
			if (ChaosItem != 0)
			{
				this->m_MagicDamageRate += ChaosItem;
			}
			else if (ItemInfo.Level != 0)
			{
				this->m_MagicDamageRate += ((this->m_MagicDamageRate * 25) / ItemInfo.Level) + 5;
			}
		}

		this->m_MagicDamageRate += (this->m_Level * 3);

		if (this->m_Level >= 10)
		{
			this->m_MagicDamageRate += ((this->m_Level - 9) * (this->m_Level - 8)) / 2;
		}
	}

	if (this->m_DefenseSuccessRate > 0)
	{
		if (ExceOption != 0 && ItemInfo.Level != 0)
		{
			this->m_DefenseSuccessRate += ((this->m_DefenseSuccessRate * 25) / ItemInfo.Level) + 5;
		}

		this->m_DefenseSuccessRate += (this->m_Level * 3);

		if (this->m_Level >= 10)
		{
			this->m_DefenseSuccessRate += ((this->m_Level - 9) * (this->m_Level - 8)) / 2;
		}
	}

	if (this->m_Defense > 0)
	{
		if (this->m_Index >= GET_ITEM(6, 0) && this->m_Index < GET_ITEM(7, 0)) // Shields
		{
			this->m_Defense += this->m_Level;
		}
		else
		{
			if (ExceOption != 0 && ItemInfo.Level != 0)
			{
				this->m_Defense += (((this->m_Defense * 12) / ItemInfo.Level) + (ItemInfo.Level / 5)) + 4;
			}

			if ((this->m_Index >= GET_ITEM(12, 3) && this->m_Index <= GET_ITEM(12, 6))) // 2sd Wings
			{
				this->m_Defense += (this->m_Level * 2);
			}
			else
			{
				this->m_Defense += (this->m_Level * 3);
			}

			if (this->m_Level >= 10)
			{
				this->m_Defense += ((this->m_Level - 9) * (this->m_Level - 8)) / 2;
			}
		}
	}

	if (this->m_MagicDefense > 0)
	{
		this->m_MagicDefense += (this->m_Level * 3);

		if (this->m_Level >= 10)
		{
			this->m_MagicDefense += ((this->m_Level - 9) * (this->m_Level - 8)) / 2;
		}
	}

	this->m_OptionsQuant = 0;

	memset(this->m_SpecialIndex, 0, sizeof(this->m_SpecialIndex));

	memset(this->m_SpecialValue, 0, sizeof(this->m_SpecialValue));

	BYTE pItemOption = 0;

	BYTE pItemValue = 0;

	if (gItemOption.GetItemOption(SPECIAL_SKILL_OPTION, this->m_Index, this->m_SkillOption, this->m_LuckOption, this->m_AddOption, this->m_ExceOption, &pItemOption, &pItemValue))
	{
		if (this->m_OptionsQuant < MAX_SPECIAL_OPTION)
		{
			this->m_SkillOption = ((this->m_SkillOption == 0) ? 1 : this->m_SkillOption);

			this->m_SpecialIndex[this->m_OptionsQuant] = ((ItemInfo.Skill == SKILL_NONE) ? pItemOption : ItemInfo.Skill);

			this->m_SpecialValue[this->m_OptionsQuant] = pItemValue;

			this->m_OptionsQuant++;
		}
		else
		{
			this->m_SkillOption = 0;
		}
	}
	else
	{
		this->m_SkillOption = 0;
	}

	if (gItemOption.GetItemOption(SPECIAL_LUCK_OPTION, this->m_Index, this->m_SkillOption, this->m_LuckOption, this->m_AddOption, this->m_ExceOption, &pItemOption, &pItemValue))
	{
		if (this->m_OptionsQuant < MAX_SPECIAL_OPTION)
		{
			this->m_LuckOption = ((this->m_LuckOption == 0) ? 1 : this->m_LuckOption);

			this->m_SpecialIndex[this->m_OptionsQuant] = pItemOption;

			this->m_SpecialValue[this->m_OptionsQuant] = pItemValue;

			this->m_OptionsQuant++;
		}
		else
		{
			this->m_LuckOption = 0;
		}
	}
	else
	{
		this->m_LuckOption = 0;
	}

	if (gItemOption.GetItemOption(SPECIAL_ADDITIONAL_OPTION, this->m_Index, this->m_SkillOption, this->m_LuckOption, this->m_AddOption, this->m_ExceOption, &pItemOption, &pItemValue))
	{
		if (this->m_OptionsQuant < MAX_SPECIAL_OPTION)
		{
			this->m_AddOption = ((this->m_AddOption == 0) ? 1 : this->m_AddOption);

			this->m_SpecialIndex[this->m_OptionsQuant] = pItemOption;

			this->m_SpecialValue[this->m_OptionsQuant] = pItemValue * this->m_AddOption;

			switch (this->m_Index)
			{
				case GET_ITEM(12, 1): // Wings of Angel
				{
					this->m_RequireStrength += (this->m_AddOption * 4);

					break;
				}

				case GET_ITEM(12, 2): // Wings of Satan
				{
					this->m_RequireStrength += (this->m_AddOption * 4);

					break;
				}

				case GET_ITEM(12, 3): // Wings of Spirit
				{
					this->m_RequireStrength += (this->m_AddOption * 4);

					break;
				}

				case GET_ITEM(12, 4): // Wings of Soul
				{
					this->m_RequireStrength += (this->m_AddOption * 4);

					break;
				}

				case GET_ITEM(12, 5): // Wings of Devil
				{
					this->m_RequireStrength += (this->m_AddOption * 4);

					break;
				}

				case GET_ITEM(12, 6): // Wings of Darkness
				{
					this->m_RequireStrength += (this->m_AddOption * 4);

					break;
				}

				default:
				{
					this->m_RequireStrength += ((this->m_Index < GET_ITEM(12, 0)) ? (this->m_AddOption * 4) : 0);

					break;
				}
			}

			this->m_OptionsQuant++;
		}
		else
		{
			this->m_AddOption = 0;
		}
	}
	else
	{
		this->m_AddOption = 0;
	}

	if (gItemOption.GetItemOption(SPECIAL_EXCELLENT1, this->m_Index, this->m_SkillOption, this->m_LuckOption, this->m_AddOption, this->m_ExceOption, &pItemOption, &pItemValue))
	{
		if (this->m_OptionsQuant < MAX_SPECIAL_OPTION)
		{
			this->m_SpecialIndex[this->m_OptionsQuant] = pItemOption;

			this->m_SpecialValue[this->m_OptionsQuant] = pItemValue;

			this->m_OptionsQuant++;
		}
	}

	if (gItemOption.GetItemOption(SPECIAL_EXCELLENT2, this->m_Index, this->m_SkillOption, this->m_LuckOption, this->m_AddOption, this->m_ExceOption, &pItemOption, &pItemValue))
	{
		if (this->m_OptionsQuant < MAX_SPECIAL_OPTION)
		{
			this->m_SpecialIndex[this->m_OptionsQuant] = pItemOption;

			this->m_SpecialValue[this->m_OptionsQuant] = pItemValue;

			this->m_OptionsQuant++;
		}
	}

	if (gItemOption.GetItemOption(SPECIAL_EXCELLENT3, this->m_Index, this->m_SkillOption, this->m_LuckOption, this->m_AddOption, this->m_ExceOption, &pItemOption, &pItemValue))
	{
		if (this->m_OptionsQuant < MAX_SPECIAL_OPTION)
		{
			this->m_SpecialIndex[this->m_OptionsQuant] = pItemOption;

			this->m_SpecialValue[this->m_OptionsQuant] = pItemValue;

			this->m_OptionsQuant++;
		}
	}

	if (gItemOption.GetItemOption(SPECIAL_EXCELLENT4, this->m_Index, this->m_SkillOption, this->m_LuckOption, this->m_AddOption, this->m_ExceOption, &pItemOption, &pItemValue))
	{
		if (this->m_OptionsQuant < MAX_SPECIAL_OPTION)
		{
			this->m_SpecialIndex[this->m_OptionsQuant] = pItemOption;

			this->m_SpecialValue[this->m_OptionsQuant] = pItemValue;

			this->m_OptionsQuant++;
		}
	}

	if (gItemOption.GetItemOption(SPECIAL_EXCELLENT5, this->m_Index, this->m_SkillOption, this->m_LuckOption, this->m_AddOption, this->m_ExceOption, &pItemOption, &pItemValue))
	{
		if (this->m_OptionsQuant < MAX_SPECIAL_OPTION)
		{
			this->m_SpecialIndex[this->m_OptionsQuant] = pItemOption;

			this->m_SpecialValue[this->m_OptionsQuant] = pItemValue;

			this->m_OptionsQuant++;
		}
	}

	if (gItemOption.GetItemOption(SPECIAL_EXCELLENT6, this->m_Index, this->m_SkillOption, this->m_LuckOption, this->m_AddOption, this->m_ExceOption, &pItemOption, &pItemValue))
	{
		if (this->m_OptionsQuant < MAX_SPECIAL_OPTION)
		{
			this->m_SpecialIndex[this->m_OptionsQuant] = pItemOption;

			this->m_SpecialValue[this->m_OptionsQuant] = pItemValue;

			this->m_OptionsQuant++;
		}
	}

	this->m_OptionsQuant = (this->m_OptionsQuant > MAX_SPECIAL_OPTION) ? MAX_SPECIAL_OPTION : this->m_OptionsQuant;

	this->Value();

	if (this->m_Index != GET_ITEM(4, 7) && this->m_Index != GET_ITEM(4, 15)) // Arrow,Bolt
	{
		this->m_BaseDamageMin = this->m_DamageMin;

		this->m_BaseDefense = this->m_Defense;

		this->m_DamageMin = (WORD)(this->m_DamageMin * this->m_CurrentDurabilityState);

		this->m_DamageMax = (WORD)(this->m_DamageMax * this->m_CurrentDurabilityState);

		this->m_Defense = (WORD)(this->m_Defense * this->m_CurrentDurabilityState);

		this->m_DefenseSuccessRate = (BYTE)(this->m_DefenseSuccessRate * this->m_CurrentDurabilityState);
	}
}

void CItem::Value()
{
	if (this->IsItem() == 0)
	{
		return;
	}

	ITEM_INFO ItemInfo;

	if (gItemManager.GetInfo(this->m_Index, &ItemInfo) == 0)
	{
		return;
	}

	if (ItemInfo.BuyMoney != 0)
	{
		this->m_BuyMoney = ItemInfo.BuyMoney;

		this->m_BuyMoney = ((this->m_BuyMoney >= 100) ? ((this->m_BuyMoney / 10) * 10) : this->m_BuyMoney);

		this->m_BuyMoney = ((this->m_BuyMoney >= 1000) ? ((this->m_BuyMoney / 100) * 100) : this->m_BuyMoney);

		this->m_SellMoney = ItemInfo.BuyMoney / 3;

		this->m_SellMoney = ((this->m_SellMoney >= 100) ? ((this->m_SellMoney / 10) * 10) : this->m_SellMoney);

		this->m_SellMoney = ((this->m_SellMoney >= 1000) ? ((this->m_SellMoney / 100) * 100) : this->m_SellMoney);

		return;
	}

	int value = 0;

	if (gItemValue.GetItemValue(this, &value) != false)
	{
		this->m_BuyMoney = value;

		this->m_BuyMoney = ((this->m_BuyMoney >= 100) ? ((this->m_BuyMoney / 10) * 10) : this->m_BuyMoney);

		this->m_BuyMoney = ((this->m_BuyMoney >= 1000) ? ((this->m_BuyMoney / 100) * 100) : this->m_BuyMoney);

		this->m_SellMoney = value / 3;

		this->m_SellMoney = ((this->m_SellMoney >= 100) ? ((this->m_SellMoney / 10) * 10) : this->m_SellMoney);

		this->m_SellMoney = ((this->m_SellMoney >= 1000) ? ((this->m_SellMoney / 100) * 100) : this->m_SellMoney);

		return;
	}

	ULONGLONG price = 0;

	if (ItemInfo.Value > 0)
	{
		price = ((ItemInfo.Value * ItemInfo.Value) * 10) / 12;

		if (this->m_Index >= GET_ITEM(14, 0) && this->m_Index <= GET_ITEM(14, 8))
		{
			if (this->m_Index == GET_ITEM(14, 3) || this->m_Index == GET_ITEM(14, 6))
			{
				price *= 2;
			}

			price *= (ULONGLONG)(1 << this->m_Level);

			price *= (ULONGLONG)this->m_Durability;

			price = ((price > MAX_ITEM_PRICE) ? MAX_ITEM_PRICE : price);

			this->m_BuyMoney = (DWORD)price;

			this->m_BuyMoney = ((this->m_BuyMoney >= 10) ? ((this->m_BuyMoney / 10) * 10) : this->m_BuyMoney);

			this->m_SellMoney = (DWORD)(price / 3);

			this->m_SellMoney = ((this->m_SellMoney >= 10) ? ((this->m_SellMoney / 10) * 10) : this->m_SellMoney);

			return;
		}
	}
	else
	{
		int ItemLevel = ItemInfo.Level + (this->m_Level * 3);

		for (int n = 0; n < this->m_OptionsQuant; n++)
		{
			if (this->m_SpecialIndex[n] != 0 && this->m_Index < GET_ITEM(12, 0))
			{
				ItemLevel += 25;

				break;
			}
		}

		if (((this->m_Index / MAX_ITEM_TYPE) == 12 && this->m_Index > GET_ITEM(12, 6)) || (this->m_Index / MAX_ITEM_TYPE) == 13 || (this->m_Index / MAX_ITEM_TYPE) == 15)
		{
			price = ((ItemLevel * ItemLevel) * ItemLevel) + 100;

			for (int n = 0; n < this->m_OptionsQuant; n++)
			{
				if (this->m_SpecialIndex[n] == ITEM_OPTION_ADD_HP_RECOVERY_RATE)
				{
					price += (price * this->m_AddOption);

					break;
				}
			}
		}
		else
		{
			switch (this->m_Level)
			{
				case 5:
				{
					ItemLevel += 4;

					break;
				}

				case 6:
				{
					ItemLevel += 10;

					break;
				}

				case 7:
				{
					ItemLevel += 25;

					break;
				}

				case 8:
				{
					ItemLevel += 45;

					break;
				}

				case 9:
				{
					ItemLevel += 65;

					break;
				}

				case 10:
				{
					ItemLevel += 95;

					break;
				}

				case 11:
				{
					ItemLevel += 135;

					break;
				}
			}

			if ((this->m_Index >= GET_ITEM(12, 0) && this->m_Index <= GET_ITEM(12, 6))) // Wings
			{
				price = ((((ItemLevel + 40) * ItemLevel) * ItemLevel) * 11) + 40000000;
			}
			else
			{
				price = ((((ItemLevel + 40) * ItemLevel) * ItemLevel) / 8) + 100;
			}

			if (this->m_Index >= GET_ITEM(0, 0) && this->m_Index < GET_ITEM(6, 0))
			{
				if (ItemInfo.TwoHand == 0)
				{
					price = (price * 80) / 100;
				}
			}

			if (this->m_LuckOption != 0)
			{
				price += (price * 25) / 100;
			}

			if (this->m_AddOption != 0)
			{
				price += ((this->m_AddOption == 1) ? ((price * 60) / 100) : 0);

				price += ((this->m_AddOption == 2) ? ((price * 140) / 100) : 0);

				price += ((this->m_AddOption == 3) ? ((price * 280) / 100) : 0);

				price += ((this->m_AddOption == 4) ? ((price * 560) / 100) : 0);
			}

			for (int n = 0; n < 6; n++)
			{
				if ((this->m_ExceOption & (1 << n)) != 0)
				{
					price += ((this->m_Index < GET_ITEM(12, 0)) ? ((price * 100) / 100) : ((price * 25) / 100));
				}
			}
		}
	}

	this->m_BuyMoney = (DWORD)price;

	this->m_BuyMoney = ((this->m_BuyMoney >= 100) ? ((this->m_BuyMoney / 10) * 10) : this->m_BuyMoney);

	this->m_BuyMoney = ((this->m_BuyMoney >= 1000) ? ((this->m_BuyMoney / 100) * 100) : this->m_BuyMoney);

	this->m_SellMoney = (DWORD)(price / 3);

	this->m_SellMoney = ((ItemInfo.Slot >= 0 && ItemInfo.Slot <= 11) ? (this->m_SellMoney - (DWORD)((this->m_SellMoney * 0.6) * (1 - (this->m_Durability / this->m_BaseDurability)))) : this->m_SellMoney);

	this->m_SellMoney = ((this->m_SellMoney >= 100) ? ((this->m_SellMoney / 10) * 10) : this->m_SellMoney);

	this->m_SellMoney = ((this->m_SellMoney >= 1000) ? ((this->m_SellMoney / 100) * 100) : this->m_SellMoney);
}

void CItem::OldValue()
{
	if (this->IsItem() == 0)
	{
		return;
	}

	if (this->m_Index == GET_ITEM(14, 13)) // Jewel of Bless
	{
		this->m_OldBuyMoney = 100000;
	}
	else if (this->m_Index == GET_ITEM(14, 14)) // Jewel of Soul
	{
		this->m_OldBuyMoney = 70000;
	}
	else if (this->m_Index == GET_ITEM(12, 15)) // Jewel of Chaos
	{
		this->m_OldBuyMoney = 40000;
	}
	else if (this->m_Index == GET_ITEM(14, 22)) // Jewel of Creation
	{
		this->m_OldBuyMoney = 450000;
	}
	else if (this->m_Index == GET_ITEM(14, 16)) // Jewel of Life
	{
		this->m_OldBuyMoney = 450000;
	}
	else
	{
		this->m_OldBuyMoney = this->m_BuyMoney;
	}
}

int CItem::GetItemSkill()
{
	if (this->IsItem() == 0 || this->m_IsValidItem == 0 || this->m_SkillOption == 0)
	{
		return 0;
	}

	ITEM_INFO ItemInfo;

	if (gItemManager.GetInfo(this->m_Index, &ItemInfo) == 0)
	{
		return 0;
	}

	BYTE pItemOption = 0;

	BYTE pItemValue = 0;

	if (gItemOption.GetItemOption(SPECIAL_SKILL_OPTION, this->m_Index, this->m_SkillOption, this->m_LuckOption, this->m_AddOption, this->m_ExceOption, &pItemOption, &pItemValue))
	{
		return ((ItemInfo.Skill == SKILL_NONE) ? pItemOption : ItemInfo.Skill);
	}

	return 0;
}

int CItem::GetDamageMin()
{
	if (this->IsItem() == 0 || this->m_IsValidItem == 0 || this->m_Durability == 0)
	{
		return 0;
	}
	else
	{
		return this->m_DamageMin;
	}
}

int CItem::GetDamageMax()
{
	if (this->IsItem() == 0 || this->m_IsValidItem == 0 || this->m_Durability == 0)
	{
		return 0;
	}
	else
	{
		return this->m_DamageMax;
	}
}

int CItem::GetDefense()
{
	if (this->IsItem() == 0 || this->m_IsValidItem == 0 || this->m_Durability == 0)
	{
		return 0;
	}
	else
	{
		return this->m_Defense;
	}
}

int CItem::GetDefenseSuccessRate()
{
	if (this->IsItem() == 0 || this->m_IsValidItem == 0 || this->m_Durability == 0)
	{
		return 0;
	}
	else
	{
		return this->m_DefenseSuccessRate;
	}
}

int CItem::GetBookSuccessRate()
{
	if (this->IsItem() == 0 || this->m_IsValidItem == 0 || this->m_Durability == 0)
	{
		return 0;
	}
	else
	{
		return (this->m_MagicDamageRate / 6);
	}
}

bool CItem::WeaponDurabilityDown(int aIndex, int defense, int type)
{
	if (this->m_Durability < 1)
	{
		return 0;
	}

	if (this->m_Index == GET_ITEM(4, 7) || this->m_Index == GET_ITEM(4, 15))
	{
		return 0;
	}

	int damage = 0;

	int MaxDurSmall = 0;

	switch (type)
	{
		case 0:
		{
			damage = this->m_BaseDamageMin + (this->m_BaseDamageMin / 2);

			damage = damage + (this->m_AddOption * 4);

			MaxDurSmall = (564 * gServerInfo.m_WeaponDurabilityRate) / 100;

			break;
		}

		case 1:
		{
			damage = this->m_BaseDamageMin + (this->m_BaseDamageMin / 2);

			damage = damage + (this->m_AddOption * 4);

			MaxDurSmall = (780 * gServerInfo.m_WeaponDurabilityRate) / 100;

			break;
		}

		case 2:
		{
			damage = (this->m_MagicDamageRate / 2) + (this->m_Level * 2);

			damage = damage + (damage / 3);

			damage = damage + (this->m_AddOption * 4);

			MaxDurSmall = (1050 * gServerInfo.m_WeaponDurabilityRate) / 100;

			break;
		}

		case 3:
		{
			damage = (this->m_MagicDamageRate / 2) + (this->m_Level * 2);

			damage = damage + (damage / 3);

			damage = damage + (this->m_AddOption * 4);

			MaxDurSmall = (1050 * gServerInfo.m_WeaponDurabilityRate) / 100;

			break;
		}
	}

	this->m_DurabilitySmall += ((damage == 0) ? 0 : (defense / damage));

	MaxDurSmall = (MaxDurSmall * gObj[aIndex].WeaponDurabilityRate) / 100;

	if (this->m_DurabilitySmall > MaxDurSmall)
	{
		this->m_Durability = (((--this->m_Durability) < 1) ? 0 : this->m_Durability);

		this->m_DurabilitySmall = 0;

		if (this->CheckDurabilityState() != 0)
		{
			gObjectManager.CharacterCalcAttribute(aIndex);
		}

		return 1;
	}

	return 0;
}

bool CItem::ArmorDurabilityDown(int aIndex, int damage)
{
	if (this->m_Durability < 1)
	{
		return 0;
	}

	ITEM_INFO ItemInfo;

	if (gItemManager.GetInfo(this->m_Index, &ItemInfo) == 0)
	{
		return 0;
	}

	int defense = this->m_BaseDefense;

	int MaxDurSmall = (69 * gServerInfo.m_ArmorDurabilityRate) / 100;

	if (this->m_Index >= GET_ITEM(6, 0) && this->m_Index < GET_ITEM(7, 0)) // Shield
	{
		defense *= 5;

		defense += this->m_AddOption * 5;
	}
	else if (ItemInfo.RequireClass[CLASS_DW] != 0)
	{
		defense *= 3;

		defense += this->m_AddOption * 4;
	}
	else if (ItemInfo.RequireClass[CLASS_DK] != 0)
	{
		defense *= 3;

		defense += this->m_AddOption * 4;
	}
	else if (ItemInfo.RequireClass[CLASS_FE] != 0)
	{
		defense *= 2;

		defense += this->m_AddOption * 4;
	}
	else if (ItemInfo.RequireClass[CLASS_MG] != 0)
	{
		defense *= 7;

		defense += this->m_AddOption * 4;
	}

	this->m_DurabilitySmall += ((defense == 0) ? 0 : (damage / defense));

	MaxDurSmall = (MaxDurSmall * gObj[aIndex].ArmorDurabilityRate) / 100;

	if (this->m_DurabilitySmall > MaxDurSmall)
	{
		this->m_Durability = (((--this->m_Durability) < 1) ? 0 : this->m_Durability);

		this->m_DurabilitySmall = 0;

		if (this->CheckDurabilityState() != 0)
		{
			gObjectManager.CharacterCalcAttribute(aIndex);
		}

		return 1;
	}

	return 0;
}

bool CItem::WingDurabilityDown(int aIndex, int decrease)
{
	if (this->m_Durability < 1)
	{
		return 0;
	}

	this->m_DurabilitySmall += decrease;

	int MaxDurSmall = (564 * gServerInfo.m_WingDurabilityRate) / 100;

	MaxDurSmall = (MaxDurSmall * gObj[aIndex].WingDurabilityRate) / 100;

	if (this->m_DurabilitySmall > MaxDurSmall)
	{
		this->m_Durability = (((--this->m_Durability) < 1) ? 0 : this->m_Durability);

		this->m_DurabilitySmall = 0;

		if (this->CheckDurabilityState() != 0)
		{
			gObjectManager.CharacterCalcAttribute(aIndex);
		}

		return 1;
	}

	return 0;
}

bool CItem::PendantDurabilityDown(int aIndex, int decrease)
{
	if (this->m_Durability < 1)
	{
		return 0;
	}

	this->m_DurabilitySmall += decrease;

	int MaxDurSmall = (564 * gServerInfo.m_PendantDurabilityRate) / 100;

	MaxDurSmall = (MaxDurSmall * gObj[aIndex].PendantDurabilityRate) / 100;

	if (this->m_DurabilitySmall > MaxDurSmall)
	{
		this->m_Durability = (((--this->m_Durability) < 1) ? 0 : this->m_Durability);

		this->m_DurabilitySmall = 0;

		if (this->CheckDurabilityState() != 0)
		{
			gObjectManager.CharacterCalcAttribute(aIndex);
		}

		return 1;
	}

	return 0;
}

bool CItem::RingDurabilityDown(int aIndex, int decrease)
{
	if (this->m_Durability < 1)
	{
		return 0;
	}

	this->m_DurabilitySmall += decrease;

	int MaxDurSmall = (564 * gServerInfo.m_RingDurabilityRate) / 100;

	MaxDurSmall = (MaxDurSmall * gObj[aIndex].RingDurabilityRate) / 100;

	if (this->m_DurabilitySmall > MaxDurSmall)
	{
		this->m_Durability = (((--this->m_Durability) < 1) ? 0 : this->m_Durability);

		this->m_DurabilitySmall = 0;

		if (this->CheckDurabilityState() != 0)
		{
			gObjectManager.CharacterCalcAttribute(aIndex);
		}

		return 1;
	}

	return 0;
}

bool CItem::CheckDurabilityState()
{
	if (this->m_Durability == 0)
	{
		if (this->m_CurrentDurabilityState != 0.0f)
		{
			this->m_CurrentDurabilityState = 0.0f;

			this->Convert(this->m_Index, this->m_SkillOption, this->m_LuckOption, this->m_AddOption, this->m_ExceOption);

			return 1;
		}
	}
	else if (this->m_Durability < this->m_DurabilityState[3])
	{
		if (this->m_CurrentDurabilityState != 0.5f)
		{
			this->m_CurrentDurabilityState = 0.5f;

			this->Convert(this->m_Index, this->m_SkillOption, this->m_LuckOption, this->m_AddOption, this->m_ExceOption);

			return 1;
		}
	}
	else if (this->m_Durability < this->m_DurabilityState[2])
	{
		if (this->m_CurrentDurabilityState != 0.6f)
		{
			this->m_CurrentDurabilityState = 0.6f;

			this->Convert(this->m_Index, this->m_SkillOption, this->m_LuckOption, this->m_AddOption, this->m_ExceOption);

			return 1;
		}
	}
	else if (this->m_Durability < this->m_DurabilityState[1])
	{
		if (this->m_CurrentDurabilityState != 0.7f)
		{
			this->m_CurrentDurabilityState = 0.7f;

			this->Convert(this->m_Index, this->m_SkillOption, this->m_LuckOption, this->m_AddOption, this->m_ExceOption);

			return 1;
		}
	}
	else if (this->m_Durability < this->m_DurabilityState[0])
	{
		if (this->m_CurrentDurabilityState != 0.8f)
		{
			this->m_CurrentDurabilityState = 0.8f;

			this->Convert(this->m_Index, this->m_SkillOption, this->m_LuckOption, this->m_AddOption, this->m_ExceOption);

			return 1;
		}
	}
	else
	{
		if (this->m_CurrentDurabilityState != 1.0f)
		{
			this->m_CurrentDurabilityState = 1.0f;

			this->Convert(this->m_Index, this->m_SkillOption, this->m_LuckOption, this->m_AddOption, this->m_ExceOption);

			return 1;
		}
	}

	return 0;
}