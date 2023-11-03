using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace kayito_Editor.Source
{
	public class ItemOption
	{
		public struct ITEM_OPTION_INFO
		{
			public int Index;
			public int OptionIndex;
			public int OptionValue;
			public int ItemMinIndex;
			public int ItemMaxIndex;
			public int ItemSkillOption;
			public int ItemLuckOption;
			public int ItemAddOption;
			public int ItemExceOption;
		};

		public static IDictionary<int, string> m_ItemOptions = new Dictionary<int, string>()
		{
			{ -1, "No Info" },
			{ 0, "Enabled: +HP // Disabled: +Dmg" },
			{ 60, "Additional Dmg +X" },
			{ 61, "Additional Wizardry Dmg +X" },
			{ 62, "Additional defense rate +X%" },
			{ 63, "Additional defense +X" },
			{ 64, "Luck (critical damage rate +5%)" },
			{ 65, "Automatic HP recovery X%" },
			{ 66, "Increase Max HP +4%" },
			{ 67, "Increase Max Mana +4%" },
			{ 68, "Damage Decrease +4%" },
			{ 69, "Reflect Damage +5%" },
			{ 70, "Defense success rate +10%" },
			{ 71, "Increases acquisition rate of\nZen after hunting monsters +30%" },
			{ 72, "Excellent Damage rate +10%" },
			{ 73, "Increase Damage +level/20" },
			{ 74, "Increase Damage +X%" },
			{ 75, "Increase Wizardry Dmg +level/20" },
			{ 76, "Increase Wizardry Dmg +X%" },
			{ 77, "Increase Attacking(Wizardry)\nspeed +X" },
			{ 78, "Increases life acquisition rate\nafter hunt +life/8" },
			{ 79, "Increases mana acquisition rate\nafter hunt +mana/8" },
			{ 80, "HP +X increased" },
			{ 81, "Mana +X increased" },
			{ 82, "Ignor opponent's defensive\npower by X%" },
			{ 83, "Max AG +X increased" },
			{ 84, "Absorb X% additional damage" },
		};

		public static IDictionary<int, List<ITEM_OPTION_INFO>> m_ItemOptionInfo = new Dictionary<int, List<ITEM_OPTION_INFO>>();

		public static void ReadItemOptionTxt()
		{
			string path = ".\\Data\\ItemOption.txt";

			MemScript lpMemScript = new MemScript();

			if (lpMemScript.SetBuffer(path) == false)
			{
				MessageBox.Show(lpMemScript.GetLastError());

				return;
			}

			ItemOption.m_ItemOptionInfo.Clear();

			try
			{
				while (true)
				{
					if (lpMemScript.GetToken() == MemScript.eTokenResult.TOKEN_END)
					{
						break;
					}

					if (String.Compare(lpMemScript.GetString(), "end") == 0)
					{
						break;
					}

					ITEM_OPTION_INFO info = new ITEM_OPTION_INFO();

					info.Index = lpMemScript.GetNumber();

					info.OptionIndex = lpMemScript.GetAsNumber();

					info.OptionValue = lpMemScript.GetAsNumber();

					info.ItemMinIndex = ItemManager.GET_ITEM(lpMemScript.GetAsNumber(), lpMemScript.GetAsNumber());

					info.ItemMaxIndex = ItemManager.GET_ITEM(lpMemScript.GetAsNumber(), lpMemScript.GetAsNumber());

					info.ItemSkillOption = lpMemScript.GetAsNumber();

					info.ItemLuckOption = lpMemScript.GetAsNumber();

					info.ItemAddOption = lpMemScript.GetAsNumber();

					info.ItemExceOption = lpMemScript.GetAsNumber();

					if (ItemOption.m_ItemOptionInfo.ContainsKey(info.Index))
					{
						ItemOption.m_ItemOptionInfo[info.Index].Add(info);
					}
					else
					{
						ItemOption.m_ItemOptionInfo.Add(info.Index, new List<ITEM_OPTION_INFO>(){ info });
					}
				}
			}
			catch
			{
				MessageBox.Show(lpMemScript.GetLastError());

				Environment.Exit(0);
			}
		}

		public static bool GetItemOption(int index, int ItemIndex, out string value)
		{
			bool result = false;

			value = m_ItemOptions[-1];

			List<ITEM_OPTION_INFO> listInfo;

			if (index == 8 && (ItemIndex >= ItemManager.GET_ITEM(12, 3) && ItemIndex <= ItemManager.GET_ITEM(12, 6)))
			{
				result = true;

				value = m_ItemOptions[0];
			}
			else if (m_ItemOptionInfo.TryGetValue(index, out listInfo))
			{
				foreach (ITEM_OPTION_INFO itemInfo in listInfo)
				{
					if (itemInfo.ItemMinIndex != -1 && itemInfo.ItemMinIndex > ItemIndex)
					{
						continue;
					}

					if (itemInfo.ItemMaxIndex != -1 && itemInfo.ItemMaxIndex < ItemIndex)
					{
						continue;
					}

					result = true;

					if (index != 0)
					{
						value = m_ItemOptions[itemInfo.OptionIndex];
					}
					else if (itemInfo.OptionIndex == 0)
					{
						result = false;
					}
				}
			}

			return result;
		}
	}
}
