using System;
using System.Collections.Generic;
using System.Windows.Forms;
using static kayito_Editor.Source.ReadScript;

namespace kayito_Editor.Source
{
	public class ItemManager
	{
		public const int MAX_ITEM_SECTION = 16;
		public const int MAX_ITEM_TYPE = 32;
		public const int MAX_ITEM = (MAX_ITEM_SECTION * MAX_ITEM_TYPE);

		public static int GET_ITEM(int x, int y)
		{
			return (x * MAX_ITEM_TYPE) + y;
		}

		public const int WAREHOUSE_SIZE = 120;

		public const int INVENTORY_WEAR_SIZE = 12;

		public const int INVENTORY_BOX_SIZE = 64;

		public const int INVENTORY_FULL_SIZE = 76;

		public struct ITEM_TXT_INFO
		{
			public int Section;
			public int Index;
			public int Slot;
			public int Skill;
			public int Width;
			public int Height;
			public int HaveSerial;
			public int HaveOption;
			public int DropItem;
			public string Name;
			public int Level;
			public int DamageMin;
			public int DamageMax;
			public int MagicDamageRate;
			public int TwoHand;
			public int Defense;
			public int MagicDefense;
			public int DefenseSuccessRate;
			public int AttackSpeed;
			public int WalkSpeed;
			public int Durability;
			public int MagicDurability;
			public int Value;
			public int BuyMoney;
			public int IceResistance;
			public int PoisonResistance;
			public int EarthResistance;
			public int FireResistance;
			public int RequireLevel;
			public int RequireStrength;
			public int RequireDexterity;
			public int RequireEnergy;
			public int RequireVitality;
			public int RequireDWClass;
			public int RequireDKClass;
			public int RequireFEClass;
			public int RequireMGClass;
		};



		public static IDictionary<int, string> m_ItemSection = new Dictionary<int, string>()
		{
			{0, "Swords"},
			{1, "Axes"},
			{2, "Maces/Scepters"},
			{3, "Spears"},
			{4, "Bows/Crossbows"},
			{5, "Staffs"},
			{6, "Shields"},
			{7, "Helms"},
			{8, "Armors"},
			{9, "Pants"},
			{10, "Gloves"},
			{11, "Boots"},
			{12, "Wings"},
			{13, "Pets/Rings/Pendants"},
			{14, "Consumables/Jewels"},
			{15, "Scrolls"},
		};

		public static IDictionary<int, string> m_ItemLevel = new Dictionary<int, string>()
		{
			{0, "0"},
			{1, "+1"},
			{2, "+2"},
			{3, "+3"},
			{4, "+4"},
			{5, "+5"},
			{6, "+6"},
			{7, "+7"},
			{8, "+8"},
			{9, "+9"},
			{10, "+10"},
			{11, "+11"}
		};

		public static IDictionary<int, string> m_ItemAddOption = new Dictionary<int, string>()
		{
			{0, "0"},
			{1, "+4"},
			{2, "+8"},
			{3, "+12"},
			{4, "+16"},
		};

		public static IDictionary<int, string> m_WeaponExeOptions = new Dictionary<int, string>()
		{
			{1, "Excellent Damage rate +10%"},
			{2, "Increase Damage +level/20"},
			{4, "Increase Damage +2%"},
			{8, "Increases Attacking(Wizardry)speed +7"},
			{16, "Increases life rate after hunt +life/8"},
			{32, "Increases mana rate after hunt +mana/8"},
		};

		public static IDictionary<int, string> m_StaffExeOptions = new Dictionary<int, string>()
		{
			{1, "Excellent Damage rate +10%"},
			{2, "Increase Wizardry Dmg +level/20"},
			{4, "Increase Wizardry Dmg +1%"},
			{8, "Increases Attacking(Wizardry)speed +7"},
			{16, "Increases life rate after hunt +life/8"},
			{32, "Increases mana rate after hunt +mana/8"},
		};

		public static IDictionary<int, string> m_SetExeOptions = new Dictionary<int, string>()
		{
			{1, "Increases zen rate after hunt +30%"},
			{2, "Defense sucess rate +10%"},
			{4, "Reflect Damage +5%"},
			{8, "Damage Decrease +4%"},
			{16, "Increase Max Mana +4%"},
			{32, "Increase Max HP +4%"},
		};

		public static IDictionary<int, string> m_WingsExeOption = new Dictionary<int, string>()
		{
			{1, "HP +105 increased"},
			{2, "Mana +105 increased"},
			{4, "Ignore opponent's defense by 3%"},
			{8, "Max AG +50 increased"},
			{16, "Increases Attacking(Wizardry)speed +5"},
			{32, "Enabled: +HP / Disabled: +DMG"},
		};

		public static IDictionary<int, ITEM_TXT_INFO> m_ItemInfo = new Dictionary<int, ITEM_TXT_INFO>();

		public static void ReadItemTxt()
		{
			string path = ".\\Data\\Item.txt";

			ReadScript lpReadScript = new ReadScript();

			if (!lpReadScript.SetBuffer(path))
			{
				MessageBox.Show(lpReadScript.GetLastError(), "ReadItemTxt");

				return;
			}

			ItemManager.m_ItemInfo.Clear();

			try
			{
				eTokenResult token;

				while (true)
				{
					token = lpReadScript.GetToken();

					if (token == eTokenResult.TOKEN_END || token == eTokenResult.TOKEN_END_SECTION)
					{
						break;
					}

					int section = lpReadScript.GetNumber();

					while (true)
					{
						token = lpReadScript.GetToken();

						if (token == eTokenResult.TOKEN_END || token == eTokenResult.TOKEN_END_SECTION)
						{
							break;
						}

						ITEM_TXT_INFO info = new ITEM_TXT_INFO();

						info.Section = section;

						info.Index = lpReadScript.GetNumber();

						info.Slot = lpReadScript.GetAsNumber();

						info.Skill = lpReadScript.GetAsNumber();

						info.Width = lpReadScript.GetAsNumber();

						info.Height = lpReadScript.GetAsNumber();

						info.HaveSerial = lpReadScript.GetAsNumber();

						info.HaveOption = lpReadScript.GetAsNumber();

						info.DropItem = lpReadScript.GetAsNumber();

						info.Name = lpReadScript.GetAsString();

						if (section >= 0 && section <= 5)
						{
							info.Level = lpReadScript.GetAsNumber();

							info.DamageMin = lpReadScript.GetAsNumber();

							info.DamageMax = lpReadScript.GetAsNumber();

							info.AttackSpeed = lpReadScript.GetAsNumber();

							info.Durability = lpReadScript.GetAsNumber();

							info.MagicDurability = lpReadScript.GetAsNumber();

							info.MagicDamageRate = lpReadScript.GetAsNumber();

							info.RequireLevel = lpReadScript.GetAsNumber();

							info.RequireStrength = lpReadScript.GetAsNumber();

							info.RequireDexterity = lpReadScript.GetAsNumber();

							info.RequireEnergy = lpReadScript.GetAsNumber();

							info.RequireVitality = lpReadScript.GetAsNumber();

							info.TwoHand = ((info.Width >= 2) ? 1 : 0);

							info.Durability += info.MagicDurability;
						}
						else if (section >= 6 && section <= 11)
						{
							info.Level = lpReadScript.GetAsNumber();

							if (section == 6)
							{
								info.Defense = lpReadScript.GetAsNumber();

								info.DefenseSuccessRate = lpReadScript.GetAsNumber();
							}
							else if (section >= 7 && section <= 9)
							{
								info.Defense = lpReadScript.GetAsNumber();

								info.MagicDefense = lpReadScript.GetAsNumber();
							}
							else if (section == 10)
							{
								info.Defense = lpReadScript.GetAsNumber();

								info.AttackSpeed = lpReadScript.GetAsNumber();
							}
							else if (section == 11)
							{
								info.Defense = lpReadScript.GetAsNumber();

								info.WalkSpeed = lpReadScript.GetAsNumber();
							}

							info.Durability = lpReadScript.GetAsNumber();

							info.RequireLevel = lpReadScript.GetAsNumber();

							info.RequireStrength = lpReadScript.GetAsNumber();

							info.RequireDexterity = lpReadScript.GetAsNumber();

							info.RequireEnergy = lpReadScript.GetAsNumber();

							info.RequireVitality = lpReadScript.GetAsNumber();
						}
						else if (section == 12)
						{
							info.Level = lpReadScript.GetAsNumber();

							info.Defense = lpReadScript.GetAsNumber();

							info.Durability = lpReadScript.GetAsNumber();

							info.RequireLevel = lpReadScript.GetAsNumber();

							info.RequireEnergy = lpReadScript.GetAsNumber();

							info.RequireStrength = lpReadScript.GetAsNumber();

							info.RequireDexterity = lpReadScript.GetAsNumber();

							info.BuyMoney = lpReadScript.GetAsNumber();
						}
						else if (section == 13)
						{
							info.Level = lpReadScript.GetAsNumber();

							info.Durability = lpReadScript.GetAsNumber();

							info.IceResistance = lpReadScript.GetAsNumber();

							info.PoisonResistance = lpReadScript.GetAsNumber();

							info.EarthResistance = lpReadScript.GetAsNumber();

							info.FireResistance = lpReadScript.GetAsNumber();

							info.RequireLevel = info.Level;
						}
						else if (section == 14)
						{
							info.Value = lpReadScript.GetAsNumber();

							info.Level = lpReadScript.GetAsNumber();

							info.Durability = 1;
						}
						else if (section == 15)
						{
							info.Level = lpReadScript.GetAsNumber();

							info.RequireLevel = lpReadScript.GetAsNumber();

							info.RequireEnergy = lpReadScript.GetAsNumber();

							info.BuyMoney = lpReadScript.GetAsNumber();

							info.DamageMin = info.Level;

							info.DamageMax = info.Level + (info.Level / 2);
						}

						if (section <= 11 || section == 13)
						{
							lpReadScript.GetToken();
						}

						if (section != 14)
						{
							info.RequireDWClass = lpReadScript.GetAsNumber();

							info.RequireDKClass = lpReadScript.GetAsNumber();

							info.RequireFEClass = lpReadScript.GetAsNumber();

							info.RequireMGClass = lpReadScript.GetAsNumber();
						}

						ItemManager.m_ItemInfo.Add(GET_ITEM(info.Section, info.Index), info);
					}
				}
			}
			catch
			{
				MessageBox.Show(lpReadScript.GetLastError(), "ReadItemTxt");

				Environment.Exit(0);
			}
		}

		public static int GetItemDurability(int index, int level, int ExceOption)
		{
			ITEM_TXT_INFO ItemInfo;

			if (!m_ItemInfo.TryGetValue(index, out ItemInfo))
			{
				return 0;
			}

			if (index == GET_ITEM(14, 21)) // Rena
			{
				return 1;
			}

			if (index == GET_ITEM(13, 18) || index == GET_ITEM(14, 19)) // Invisibility Cloak, Devil's Invitation
			{
				return 1;
			}

			int dur = 0;

			if (level >= 5)
			{
				if (level == 10)
				{
					dur = ItemInfo.Durability + ((level * 2) - 3);
				}
				else if (level == 11)
				{
					dur = ItemInfo.Durability + ((level * 2) - 1);
				}
				else
				{
					dur = ItemInfo.Durability + ((level * 2) - 4);
				}
			}
			else
			{
				dur = ItemInfo.Durability + level;
			}

			if (index != GET_ITEM(0, 19) && index != GET_ITEM(2, 13) && index != GET_ITEM(4, 18) && index != GET_ITEM(5, 10) && ItemInfo.Slot != 7) // Sword of Archangel,Scepter of Archangel,Crossbow of Archangel,Staff of Archangel
			{
				if (ExceOption != 0)
				{
					dur += 15;
				}
			}

			return ((dur > 255) ? 255 : dur);
		}

		public static int GetItemSkill(int index)
		{
			ITEM_TXT_INFO ItemInfo;

			if (!m_ItemInfo.TryGetValue(index, out ItemInfo))
			{
				return 0;
			}

			return ItemInfo.Skill;
		}
	}
}