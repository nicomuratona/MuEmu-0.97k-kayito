using System;
using System.IO;
using System.Text;
using System.Windows.Forms;
using kayitoTools.Source_Files;

namespace kayitoTools.Forms
{
	public partial class ItemEditor : Form
	{
		private string itemPath;

		private bool ListLoaded = false;

		public ItemEditor()
		{
			InitializeComponent();
		}

		private void ItemManager_Load(object sender, System.EventArgs e)
		{
			Globals.SetDoubleBuffer(this.Item_List_Box, true);

			this.Item_Group_Selector_Box.DataSource = new BindingSource(ItemManager.m_ItemSection, null);
			this.Item_Group_Selector_Box.DisplayMember = "Value";
			this.Item_Group_Selector_Box.ValueMember = "Key";

			this.Item_List_Box.RowCount = ItemManager.MAX_ITEM;

			Console.WriteLine("[ItemManager] Item Editor Opened");
		}

		private void InitializeGridList()
		{
			this.Item_List_Box.Visible = false;

			this.ListLoaded = false;

			foreach (DataGridViewRow row in this.Item_List_Box.Rows)
			{
				for (int j = 0; j < this.Item_List_Box.Columns.Count; j++)
				{
					row.Cells[j].Value = 0;
				}

				row.Cells["Row_Col"].Value = row.Index;

				row.Cells["Index_Col"].Value = row.Index % ItemManager.MAX_ITEM_INDEX;

				row.Cells["Name_Col"].Value = null;

				row.Cells["Serial_Col"].Value = 1;

				row.Cells["Option_Col"].Value = 1;

				row.Cells["Drop_Col"].Value = 1;
			}

			this.ListLoaded = true;
		}

		private void LoadItemtxt_Click(object sender, EventArgs e)
		{
			StartBrowsing("item.txt");
		}

		private void LoadItembmd_Click(object sender, EventArgs e)
		{
			StartBrowsing("item.bmd");
		}

		private void StartBrowsing(string file)
		{
			string fileType = file.Substring(file.Length - 3);

			this.OpenItemFile.FileName = file;

			this.OpenItemFile.Filter = fileType.ToUpper() + " files (*." + fileType + ")|*." + fileType;

			if (this.OpenItemFile.ShowDialog() == DialogResult.OK)
			{
				this.itemPath = this.OpenItemFile.FileName;

				this.InitializeGridList();

				bool result = true;

				if (fileType == "txt")
				{
					result = this.ReadItemTXT(this.itemPath);
				}
				else if (fileType == "bmd")
				{
					result = this.ReadItemBMD(this.itemPath);
				}

				if (result == true)
				{
					this.Item_List_Box.Visible = true;

					this.Save_Item_Menu_Btn.Enabled = true;

					Console.WriteLine("[ItemManager] {0} loaded", file);
				}
				else
				{
					this.Item_List_Box.Visible = false;

					this.Save_Item_Menu_Btn.Enabled = false;
				}
			}
		}

		private bool ReadItemTXT(string path)
		{
			MemScript lpMemScript = new MemScript();

			if (lpMemScript.SetBuffer(path) == false)
			{
				Console.WriteLine("Error 1: {0}", lpMemScript.GetLastError());

				return false;
			}

			try
			{
				while (true)
				{
					if (lpMemScript.GetToken() == MemScript.eTokenResult.TOKEN_END)
					{
						break;
					}

					int Section = lpMemScript.GetNumber();

					while (true)
					{
						if (String.Compare(lpMemScript.GetAsString(), "end") == 0)
						{
							break;
						}

						int ItemIndex = (ItemManager.MAX_ITEM_INDEX * Section + lpMemScript.GetNumber());

						this.Item_List_Box.Rows[ItemIndex].Cells["Slot_Col"].Value = lpMemScript.GetAsNumber();

						this.Item_List_Box.Rows[ItemIndex].Cells["Skill_Col"].Value = lpMemScript.GetAsNumber();

						this.Item_List_Box.Rows[ItemIndex].Cells["Width_Col"].Value = lpMemScript.GetAsNumber();

						this.Item_List_Box.Rows[ItemIndex].Cells["Height_Col"].Value = lpMemScript.GetAsNumber();

						this.Item_List_Box.Rows[ItemIndex].Cells["Serial_Col"].Value = lpMemScript.GetAsNumber();

						this.Item_List_Box.Rows[ItemIndex].Cells["Option_Col"].Value = lpMemScript.GetAsNumber();

						this.Item_List_Box.Rows[ItemIndex].Cells["Drop_Col"].Value = lpMemScript.GetAsNumber();

						this.Item_List_Box.Rows[ItemIndex].Cells["Name_Col"].Value = lpMemScript.GetAsString();

						if (Section >= 0 && Section <= 5)
						{
							this.Item_List_Box.Rows[ItemIndex].Cells["Level_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["DamageMin_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["DamageMax_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["AttackSpeed_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["Durability_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["MagicDurability_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["MagicPower_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["RequireLevel_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["RequireStrength_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["RequireAgility_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["RequireEnergy_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["RequireVitality_Col"].Value = lpMemScript.GetAsNumber();
						}
						else if (Section >= 6 && Section <= 11)
						{
							this.Item_List_Box.Rows[ItemIndex].Cells["Level_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["Defense_Col"].Value = lpMemScript.GetAsNumber();

							if (Section == 6)
							{
								this.Item_List_Box.Rows[ItemIndex].Cells["DefenseRate_Col"].Value = lpMemScript.GetAsNumber();
							}
							else if (Section >= 7 && Section <= 9)
							{
								this.Item_List_Box.Rows[ItemIndex].Cells["MagicDefense_Col"].Value = lpMemScript.GetAsNumber();
							}
							else if (Section == 10)
							{
								this.Item_List_Box.Rows[ItemIndex].Cells["AttackSpeed_Col"].Value = lpMemScript.GetAsNumber();
							}
							else if (Section == 11)
							{
								this.Item_List_Box.Rows[ItemIndex].Cells["WalkSpeed_Col"].Value = lpMemScript.GetAsNumber();
							}

							this.Item_List_Box.Rows[ItemIndex].Cells["Durability_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["RequireLevel_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["RequireStrength_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["RequireAgility_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["RequireEnergy_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["RequireVitality_Col"].Value = lpMemScript.GetAsNumber();
						}
						else if (Section == 12)
						{
							this.Item_List_Box.Rows[ItemIndex].Cells["Level_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["Defense_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["Durability_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["RequireLevel_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["RequireEnergy_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["RequireStrength_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["RequireAgility_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["Money_Col"].Value = lpMemScript.GetAsNumber();
						}
						else if (Section == 13)
						{
							this.Item_List_Box.Rows[ItemIndex].Cells["Level_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["Durability_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["ResistanceIce_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["ResistancePoison_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["ResistanceLightning_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["ResistanceFire_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["RequireLevel_Col"].Value = this.Item_List_Box.Rows[ItemIndex].Cells["Level_Col"].Value;
						}
						else if (Section == 14)
						{
							this.Item_List_Box.Rows[ItemIndex].Cells["Value_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["Level_Col"].Value = lpMemScript.GetAsNumber();
						}
						else if (Section == 15)
						{
							this.Item_List_Box.Rows[ItemIndex].Cells["Level_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["RequireLevel_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["RequireEnergy_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["Money_Col"].Value = lpMemScript.GetAsNumber();
						}

						if (Section <= 11 || Section == 13)
						{
							this.Item_List_Box.Rows[ItemIndex].Cells["Attribute_Col"].Value = lpMemScript.GetAsNumber();
						}

						if (Section != 14)
						{
							this.Item_List_Box.Rows[ItemIndex].Cells["RequireDW_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["RequireDK_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["RequireELF_Col"].Value = lpMemScript.GetAsNumber();

							this.Item_List_Box.Rows[ItemIndex].Cells["RequireMG_Col"].Value = lpMemScript.GetAsNumber();
						}
					}
				}
			}
			catch
			{
				Console.WriteLine("Error 2: {0}", lpMemScript.GetLastError());

				return false;
			}

			return true;
		}

		private void SaveItemtxt_Click(object sender, EventArgs e)
		{
			this.SaveItemFile.FileName = "item.txt";

			this.SaveItemFile.Filter = "Text files (*.txt)|*.txt";

			if (this.SaveItemFile.ShowDialog() == DialogResult.OK)
			{
				using (Stream fs = new FileStream(this.SaveItemFile.FileName, FileMode.Create, FileAccess.Write))
				{
					using (StreamWriter sw = new StreamWriter(fs))
					{
						for (int group = 0; group < ItemManager.MAX_ITEM_GROUP; group++)
						{
							sw.WriteLine("//=====================================");

							switch (group)
							{
								case 0:
								{
									sw.WriteLine("//	SWORDS");

									break;
								}

								case 1:
								{
									sw.WriteLine("//	AXES");

									break;
								}

								case 2:
								{
									sw.WriteLine("//	MACES");

									break;
								}

								case 3:
								{
									sw.WriteLine("//	SPEARS");

									break;
								}

								case 4:
								{
									sw.WriteLine("//	BOWS & CROSSBOWS");

									break;
								}

								case 5:
								{
									sw.WriteLine("//	STAFFS");

									break;
								}

								case 6:
								{
									sw.WriteLine("//	SHIELDS");

									break;
								}

								case 7:
								{
									sw.WriteLine("//	HELMS");

									break;
								}

								case 8:
								{
									sw.WriteLine("//	ARMORS");

									break;
								}

								case 9:
								{
									sw.WriteLine("//	PANTS");

									break;
								}

								case 10:
								{
									sw.WriteLine("//	GLOVES");

									break;
								}

								case 11:
								{
									sw.WriteLine("//	BOOTS");

									break;
								}

								case 12:
								{
									sw.WriteLine("//	WINGS & EXTRAS 1");

									break;
								}

								case 13:
								{
									sw.WriteLine("//	PETS, RINGS, PENDANTS & EXTRAS 2");

									break;
								}

								case 14:
								{
									sw.WriteLine("//	JEWELS, CONSUMABLES & EXTRAS 3");

									break;
								}

								case 15:
								{
									sw.WriteLine("//	SCROLLS");

									break;
								}
							}

							sw.WriteLine("//=====================================");

							sw.WriteLine(group);

							switch (group)
							{
								case 0:
								case 1:
								case 2:
								case 3:
								case 4:
								case 5:
								{
									sw.WriteLine("//Index	Slot	Skill	Width	Height	Serial	Option	Drop	Name					Level	DmgMin	DmgMax	AtkSped	Durab	MagiDur	MagiDmg	ReqLvl	ReqStr	ReqAgi	ReqEne	ReqVit	Atrib	DW	DK	ELF	MG");

									break;
								}

								case 6:
								{
									sw.WriteLine("//Index	Slot	Skill	Width	Height	Serial	Option	Drop	Name					Level	Defense	DefRate	Durab	ReqLvl	ReqStr	ReqAgi	ReqEne	ReqVit	Atrib	DW	DK	ELF	MG");

									break;
								}

								case 7:
								case 8:
								case 9:
								{
									sw.WriteLine("//Index	Slot	Skill	Width	Height	Serial	Option	Drop	Name					Level	Defense	MagiDef	Durab	ReqLvl	ReqStr	ReqAgi	ReqEne	ReqVit	Atrib	DW	DK	ELF	MG");

									break;
								}

								case 10:
								{
									sw.WriteLine("//Index	Slot	Skill	Width	Height	Serial	Option	Drop	Name					Level	Defense	AtkSped	Durab	ReqLvl	ReqStr	ReqAgi	ReqEne	ReqVit	Atrib	DW	DK	ELF	MG");

									break;
								}

								case 11:
								{
									sw.WriteLine("//Index	Slot	Skill	Width	Height	Serial	Option	Drop	Name					Level	Defense	WalkSpd	Durab	ReqLvl	ReqStr	ReqAgi	ReqEne	ReqVit	Atrib	DW	DK	ELF	MG");

									break;
								}

								case 12:
								{
									sw.WriteLine("//Index	Slot	Skill	Width	Height	Serial	Option	Drop	Name					Level	Defense	Durab	ReqLvl	ReqEne	ReqStr	ReqAgi	Money	DW	DK	ELF	MG");

									break;
								}

								case 13:
								{
									sw.WriteLine("//Index	Slot	Skill	Width	Height	Serial	Option	Drop	Name					Level	Durab	Ice	Poison	Lightin	Fire	Atrib	DW	DK	ELF	MG");

									break;
								}

								case 14:
								{
									sw.WriteLine("//Index	Slot	Skill	Width	Height	Serial	Option	Drop	Name					Value	Level");

									break;
								}

								case 15:
								{
									sw.WriteLine("//Index	Slot	Skill	Width	Height	Serial	Option	Drop	Name					Level	ReqLvl	ReqEne	Money	DW	DK	ELF	MG");

									break;
								}
							}

							for (int index = 0; index < ItemManager.MAX_ITEM_INDEX; index++)
							{
								if (this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Name_Col"].Value == null)
								{
									continue;
								}

								if (String.IsNullOrEmpty(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Name_Col"].Value.ToString()))
								{
									continue;
								}

								switch (group)
								{
									case 0:
									case 1:
									case 2:
									case 3:
									case 4:
									case 5:
									{
										sw.WriteLine(@"{0}	{1}	{2}	{3}	{4}	{5}	{6}	{7}	""{8}""					{9}	{10}	{11}	{12}	{13}	{14}	{15}	{16}	{17}	{18}	{19}	{20}	{21}	{22}	{23}	{24}	{25}",
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Index_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Slot_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Skill_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Width_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Height_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Serial_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Option_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Drop_Col"].Value),
											Globals.GetMaxStringLength(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Name_Col"].Value.ToString(), 30),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Level_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["DamageMin_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["DamageMax_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["AttackSpeed_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Durability_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["MagicDurability_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["MagicPower_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireLevel_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireStrength_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireAgility_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireEnergy_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireVitality_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Attribute_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireDW_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireDK_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireELF_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireMG_Col"].Value));

										break;
									}

									case 6:
									case 7:
									case 8:
									case 9:
									case 10:
									case 11:
									{
										sw.WriteLine(@"{0}	{1}	{2}	{3}	{4}	{5}	{6}	{7}	""{8}""					{9}	{10}	{11}	{12}	{13}	{14}	{15}	{16}	{17}	{18}	{19}	{20}	{21}	{22}",
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Index_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Slot_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Skill_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Width_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Height_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Serial_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Option_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Drop_Col"].Value),
											Globals.GetMaxStringLength(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Name_Col"].Value.ToString(), 30),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Level_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Defense_Col"].Value),
											(group == 6) ? (Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["DefenseRate_Col"].Value))
											: (group >= 7 || group <= 9) ? (Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["MagicDefense_Col"].Value))
											: (group == 10) ? (Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["AttackSpeed_Col"].Value))
											: (Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["WalkSpeed_Col"].Value)),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Durability_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireLevel_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireStrength_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireAgility_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireEnergy_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireVitality_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Attribute_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireDW_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireDK_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireELF_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireMG_Col"].Value));

										break;
									}

									case 12:
									{
										sw.WriteLine(@"{0}	{1}	{2}	{3}	{4}	{5}	{6}	{7}	""{8}""					{9}	{10}	{11}	{12}	{13}	{14}	{15}	{16}	{17}	{18}	{19}	{20}",
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Index_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Slot_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Skill_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Width_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Height_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Serial_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Option_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Drop_Col"].Value),
											Globals.GetMaxStringLength(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Name_Col"].Value.ToString(), 30),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Level_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Defense_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Durability_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireLevel_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireEnergy_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireStrength_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireAgility_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Money_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireDW_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireDK_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireELF_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireMG_Col"].Value));

										break;
									}

									case 13:
									{
										sw.WriteLine(@"{0}	{1}	{2}	{3}	{4}	{5}	{6}	{7}	""{8}""					{9}	{10}	{11}	{12}	{13}	{14}	{15}	{16}	{17}	{18}	{19}",
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Index_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Slot_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Skill_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Width_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Height_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Serial_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Option_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Drop_Col"].Value),
											Globals.GetMaxStringLength(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Name_Col"].Value.ToString(), 30),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Level_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Durability_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["ResistanceIce_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["ResistancePoison_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["ResistanceLightning_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["ResistanceFire_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Attribute_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireDW_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireDK_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireELF_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireMG_Col"].Value));

										break;
									}

									case 14:
									{
										sw.WriteLine(@"{0}	{1}	{2}	{3}	{4}	{5}	{6}	{7}	""{8}""					{9}	{10}",
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Index_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Slot_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Skill_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Width_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Height_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Serial_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Option_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Drop_Col"].Value),
											Globals.GetMaxStringLength(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Name_Col"].Value.ToString(), 30),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Value_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Level_Col"].Value));

										break;
									}

									case 15:
									{
										sw.WriteLine(@"{0}	{1}	{2}	{3}	{4}	{5}	{6}	{7}	""{8}""					{9}	{10}	{11}	{12}	{13}	{14}	{15}	{16}",
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Index_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Slot_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Skill_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Width_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Height_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Serial_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Option_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Drop_Col"].Value),
											Globals.GetMaxStringLength(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Name_Col"].Value.ToString(), 30),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Level_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireLevel_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireEnergy_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["Money_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireDW_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireDK_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireELF_Col"].Value),
											Convert.ToInt32(this.Item_List_Box.Rows[group * ItemManager.MAX_ITEM_INDEX + index].Cells["RequireMG_Col"].Value));

										break;
									}
								}
							}

							sw.WriteLine("end");
							sw.WriteLine();
						}

						Console.WriteLine("[ItemManager] {0} saved", this.SaveItemFile.FileName);
					}
				}
			}
		}

		private bool ReadItemBMD(string path)
		{
			try
			{
				byte[] buffer = File.ReadAllBytes(path);

				if (buffer.Length != ItemManager.MAX_ITEM * ItemManager.ITEM_BMD_SIZE + 4)
				{
					Console.WriteLine("[ItemManager] Item file size is not correct");

					return false;
				}

				byte[] subBuffer = new byte[ItemManager.ITEM_BMD_SIZE];

				for (int i = 0; i < ItemManager.MAX_ITEM; i++)
				{
					Array.Copy(buffer, i * ItemManager.ITEM_BMD_SIZE, subBuffer, 0, ItemManager.ITEM_BMD_SIZE);

					Array.Copy(BMDFileOpen.BuxConvert(subBuffer, ItemManager.ITEM_BMD_SIZE), 0, buffer, i * ItemManager.ITEM_BMD_SIZE, ItemManager.ITEM_BMD_SIZE);
				}

				for (int itemIndex = 0; itemIndex < ItemManager.MAX_ITEM; itemIndex++)
				{
					string itemName = Encoding.Default.GetString(BMDFileOpen.SubArray(buffer, (ItemManager.ITEM_BMD_SIZE * itemIndex + 0), 30)).Trim('\0');

					if (String.IsNullOrEmpty(itemName))
					{
						continue;
					}

					this.Item_List_Box.Rows[itemIndex].Cells["Name_Col"].Value = itemName;
					this.Item_List_Box.Rows[itemIndex].Cells["Level_Col"].Value = Globals.GetRealByteValue(buffer[ItemManager.ITEM_BMD_SIZE * itemIndex + 31]);
					this.Item_List_Box.Rows[itemIndex].Cells["Width_Col"].Value = Globals.GetRealByteValue(buffer[ItemManager.ITEM_BMD_SIZE * itemIndex + 32]);
					this.Item_List_Box.Rows[itemIndex].Cells["Height_Col"].Value = Globals.GetRealByteValue(buffer[ItemManager.ITEM_BMD_SIZE * itemIndex + 33]);
					this.Item_List_Box.Rows[itemIndex].Cells["DamageMin_Col"].Value = Globals.GetRealByteValue(buffer[ItemManager.ITEM_BMD_SIZE * itemIndex + 34]);
					this.Item_List_Box.Rows[itemIndex].Cells["DamageMax_Col"].Value = Globals.GetRealByteValue(buffer[ItemManager.ITEM_BMD_SIZE * itemIndex + 35]);
					this.Item_List_Box.Rows[itemIndex].Cells["DefenseRate_Col"].Value = Globals.GetRealByteValue(buffer[ItemManager.ITEM_BMD_SIZE * itemIndex + 36]);
					this.Item_List_Box.Rows[itemIndex].Cells["Defense_Col"].Value = Globals.GetRealByteValue(buffer[ItemManager.ITEM_BMD_SIZE * itemIndex + 37]);
					this.Item_List_Box.Rows[itemIndex].Cells["MagicDefense_Col"].Value = Globals.GetRealByteValue(buffer[ItemManager.ITEM_BMD_SIZE * itemIndex + 38]);
					this.Item_List_Box.Rows[itemIndex].Cells["AttackSpeed_Col"].Value = Globals.GetRealByteValue(buffer[ItemManager.ITEM_BMD_SIZE * itemIndex + 39]);
					this.Item_List_Box.Rows[itemIndex].Cells["WalkSpeed_Col"].Value = Globals.GetRealByteValue(buffer[ItemManager.ITEM_BMD_SIZE * itemIndex + 40]);
					this.Item_List_Box.Rows[itemIndex].Cells["Durability_Col"].Value = Globals.GetRealByteValue(buffer[ItemManager.ITEM_BMD_SIZE * itemIndex + 41]);
					this.Item_List_Box.Rows[itemIndex].Cells["MagicDurability_Col"].Value = Globals.GetRealByteValue(buffer[ItemManager.ITEM_BMD_SIZE * itemIndex + 42]);
					this.Item_List_Box.Rows[itemIndex].Cells["MagicPower_Col"].Value = Globals.GetRealByteValue(buffer[ItemManager.ITEM_BMD_SIZE * itemIndex + 43]);
					this.Item_List_Box.Rows[itemIndex].Cells["RequireStrength_Col"].Value = BitConverter.ToUInt16(BMDFileOpen.SubArray(buffer, (ItemManager.ITEM_BMD_SIZE * itemIndex + 44), 2), 0);
					this.Item_List_Box.Rows[itemIndex].Cells["RequireAgility_Col"].Value = BitConverter.ToUInt16(BMDFileOpen.SubArray(buffer, (ItemManager.ITEM_BMD_SIZE * itemIndex + 46), 2), 0);
					this.Item_List_Box.Rows[itemIndex].Cells["RequireEnergy_Col"].Value = Globals.GetRealByteValue(buffer[ItemManager.ITEM_BMD_SIZE * itemIndex + 48]);
					this.Item_List_Box.Rows[itemIndex].Cells["RequireLevel_Col"].Value = Globals.GetRealByteValue(buffer[ItemManager.ITEM_BMD_SIZE * itemIndex + 49]);
					this.Item_List_Box.Rows[itemIndex].Cells["Value_Col"].Value = Globals.GetRealByteValue(buffer[ItemManager.ITEM_BMD_SIZE * itemIndex + 50]);
					this.Item_List_Box.Rows[itemIndex].Cells["Money_Col"].Value = BitConverter.ToUInt32(BMDFileOpen.SubArray(buffer, (ItemManager.ITEM_BMD_SIZE * itemIndex + 52), 4), 0);
					this.Item_List_Box.Rows[itemIndex].Cells["RequireDW_Col"].Value = Globals.GetRealByteValue(buffer[ItemManager.ITEM_BMD_SIZE * itemIndex + 56]);
					this.Item_List_Box.Rows[itemIndex].Cells["RequireDK_Col"].Value = Globals.GetRealByteValue(buffer[ItemManager.ITEM_BMD_SIZE * itemIndex + 57]);
					this.Item_List_Box.Rows[itemIndex].Cells["RequireELF_Col"].Value = Globals.GetRealByteValue(buffer[ItemManager.ITEM_BMD_SIZE * itemIndex + 58]);
					this.Item_List_Box.Rows[itemIndex].Cells["RequireMG_Col"].Value = Globals.GetRealByteValue(buffer[ItemManager.ITEM_BMD_SIZE * itemIndex + 59]);
					this.Item_List_Box.Rows[itemIndex].Cells["ResistanceIce_Col"].Value = Globals.GetRealByteValue(buffer[ItemManager.ITEM_BMD_SIZE * itemIndex + 60]);
					this.Item_List_Box.Rows[itemIndex].Cells["ResistancePoison_Col"].Value = Globals.GetRealByteValue(buffer[ItemManager.ITEM_BMD_SIZE * itemIndex + 61]);
					this.Item_List_Box.Rows[itemIndex].Cells["ResistanceLightning_Col"].Value = Globals.GetRealByteValue(buffer[ItemManager.ITEM_BMD_SIZE * itemIndex + 62]);
					this.Item_List_Box.Rows[itemIndex].Cells["ResistanceFire_Col"].Value = Globals.GetRealByteValue(buffer[ItemManager.ITEM_BMD_SIZE * itemIndex + 63]);
				}
			}
			catch
			{
				Console.WriteLine("[ItemManager] Item file is not correct. Please, use another file");

				return false;
			}

			return true;
		}

		private void SaveItembmd_Click(object sender, EventArgs e)
		{
			this.SaveItemFile.FileName = "item.bmd";

			this.SaveItemFile.Filter = "BMD files (*.bmd)|*.bmd";

			if (this.SaveItemFile.ShowDialog() == DialogResult.OK)
			{
				byte[] buffer = new byte[ItemManager.MAX_ITEM * ItemManager.ITEM_BMD_SIZE + 4];

				byte[] subBuffer = new byte[ItemManager.ITEM_BMD_SIZE];

				for (int itemIndex = 0; itemIndex < ItemManager.MAX_ITEM; itemIndex++)
				{
					if (this.Item_List_Box.Rows[itemIndex].Cells["Name_Col"].Value != null && !String.IsNullOrEmpty(this.Item_List_Box.Rows[itemIndex].Cells["Name_Col"].Value.ToString()))
					{
						Array.Copy(BMDFileOpen.StringToByteArray(this.Item_List_Box.Rows[itemIndex].Cells["Name_Col"].Value.ToString(), 30), 0, buffer, itemIndex * ItemManager.ITEM_BMD_SIZE, 30);
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 30] = (Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["Width_Col"].Value) > 1) ? (byte)1 : (byte)0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 31] = Globals.GetMaxByteValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["Level_Col"].Value));
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 32] = Globals.GetMaxByteValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["Width_Col"].Value));
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 33] = Globals.GetMaxByteValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["Height_Col"].Value));
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 34] = Globals.GetMaxByteValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["DamageMin_Col"].Value));
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 35] = Globals.GetMaxByteValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["DamageMax_Col"].Value));
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 36] = Globals.GetMaxByteValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["DefenseRate_Col"].Value));
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 37] = Globals.GetMaxByteValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["Defense_Col"].Value));
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 38] = Globals.GetMaxByteValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["MagicDefense_Col"].Value));
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 39] = Globals.GetMaxByteValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["AttackSpeed_Col"].Value));
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 40] = Globals.GetMaxByteValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["WalkSpeed_Col"].Value));
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 41] = Globals.GetMaxByteValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["Durability_Col"].Value));
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 42] = Globals.GetMaxByteValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["MagicDurability_Col"].Value));
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 43] = Globals.GetMaxByteValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["MagicPower_Col"].Value));
						Array.Copy(BitConverter.GetBytes(Globals.GetMaxWordValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["RequireStrength_Col"].Value))), 0, buffer, itemIndex * ItemManager.ITEM_BMD_SIZE + 44, 2);
						Array.Copy(BitConverter.GetBytes(Globals.GetMaxWordValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["RequireAgility_Col"].Value))), 0, buffer, itemIndex * ItemManager.ITEM_BMD_SIZE + 46, 2);
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 48] = Globals.GetMaxByteValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["RequireEnergy_Col"].Value));
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 49] = Globals.GetMaxByteValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["RequireLevel_Col"].Value));
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 50] = Globals.GetMaxByteValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["Value_Col"].Value));
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 51] = 0;
						Array.Copy(BitConverter.GetBytes(Globals.GetMaxDwordValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["Money_Col"].Value))), 0, buffer, itemIndex * ItemManager.ITEM_BMD_SIZE + 52, 4);
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 56] = Globals.GetMaxByteValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["RequireDW_Col"].Value));
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 57] = Globals.GetMaxByteValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["RequireDK_Col"].Value));
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 58] = Globals.GetMaxByteValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["RequireELF_Col"].Value));
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 59] = Globals.GetMaxByteValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["RequireMG_Col"].Value));
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 60] = Globals.GetMaxByteValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["ResistanceIce_Col"].Value));
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 61] = Globals.GetMaxByteValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["ResistancePoison_Col"].Value));
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 62] = Globals.GetMaxByteValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["ResistanceLightning_Col"].Value));
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 63] = Globals.GetMaxByteValue(Convert.ToInt32(this.Item_List_Box.Rows[itemIndex].Cells["ResistanceFire_Col"].Value));
					}
					else
					{
						Array.Copy(BMDFileOpen.StringToByteArray("", 30), 0, buffer, itemIndex * ItemManager.ITEM_BMD_SIZE, 30);
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 30] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 31] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 32] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 33] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 34] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 35] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 36] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 37] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 38] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 39] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 40] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 41] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 42] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 43] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 44] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 45] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 46] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 47] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 48] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 49] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 50] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 51] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 52] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 53] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 54] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 55] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 56] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 57] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 58] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 59] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 60] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 61] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 62] = 0;
						buffer[itemIndex * ItemManager.ITEM_BMD_SIZE + 63] = 0;
					}
					
					Array.Copy(buffer, itemIndex * ItemManager.ITEM_BMD_SIZE, subBuffer, 0, ItemManager.ITEM_BMD_SIZE);

					Array.Copy(BMDFileOpen.BuxConvert(subBuffer, ItemManager.ITEM_BMD_SIZE), 0, buffer, itemIndex * ItemManager.ITEM_BMD_SIZE, ItemManager.ITEM_BMD_SIZE);
				}

				Array.Copy(BitConverter.GetBytes(BMDFileOpen.GenerateCheckSum(buffer, ItemManager.MAX_ITEM * ItemManager.ITEM_BMD_SIZE, 0xE2F1)), 0, buffer, ItemManager.MAX_ITEM * ItemManager.ITEM_BMD_SIZE, 4);

				File.WriteAllBytes(this.SaveItemFile.FileName, buffer);

				Console.WriteLine("[ItemManager] {0} saved", this.SaveItemFile.FileName);
			}
		}

		private void Item_List_Box_CellValidating(object sender, DataGridViewCellValidatingEventArgs e)
		{
			if (e.ColumnIndex != this.Item_List_Box.Columns["Name_Col"].Index)
			{
				int i;

				if (!int.TryParse(Convert.ToString(e.FormattedValue), out i))
				{
					e.Cancel = true;

					MessageBox.Show("Only numbers allowed");
				}
			}
		}

		private void Item_List_Box_CellValueChanged(object sender, DataGridViewCellEventArgs e)
		{
			if (this.ListLoaded != true)
			{
				return;
			}

			if (e.ColumnIndex == this.Item_List_Box.Columns["Name_Col"].Index)
			{
				if (this.Item_List_Box.Rows[e.RowIndex].Cells[e.ColumnIndex].Value != null)
				{
					this.Item_List_Box.Rows[e.RowIndex].Cells[e.ColumnIndex].Value = Globals.GetMaxStringLength(this.Item_List_Box.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString(), 30);
				}
			}
		}

		private void Item_Group_Selector_Box_SelectedIndexChanged(object sender, EventArgs e)
		{
			if (this.Item_List_Box.RowCount == 0)
			{
				return;
			}

			if (this.Item_List_Box.FirstDisplayedScrollingRowIndex / ItemManager.MAX_ITEM_INDEX == this.Item_Group_Selector_Box.SelectedIndex)
			{
				return;
			}

			this.Item_List_Box.FirstDisplayedScrollingRowIndex = ItemManager.MAX_ITEM_INDEX * this.Item_Group_Selector_Box.SelectedIndex;
		}

		private void Item_List_Box_Scroll(object sender, ScrollEventArgs e)
		{
			this.Item_Group_Selector_Box.SelectedIndex = this.Item_List_Box.FirstDisplayedScrollingRowIndex / ItemManager.MAX_ITEM_INDEX;
		}
	}
}