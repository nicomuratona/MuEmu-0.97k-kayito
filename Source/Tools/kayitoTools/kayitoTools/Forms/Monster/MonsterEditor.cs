using System;
using System.IO;
using System.Windows.Forms;
using kayitoTools.Source_Files;
using static kayitoTools.Source_Files.ReadScript;

namespace kayitoTools.Forms
{
	public partial class MonsterEditor : Form
	{
		private string monsterPath;

		public MonsterEditor()
		{
			InitializeComponent();
		}

		private void MonsterEditor_Load(object sender, EventArgs e)
		{
			Globals.SetDoubleBuffer(this.Monster_List_Box, true);

			this.Monster_List_Box.RowCount = MonsterManager.MAX_ROWS_MONSTER;

			Console.WriteLine("[MonsterManager] Monster Editor Opened");
		}

		private void InitializeGridList()
		{
			this.Monster_List_Box.Visible = false;

			foreach (DataGridViewRow row in this.Monster_List_Box.Rows)
			{
				for (int i = 2; i < this.Monster_List_Box.ColumnCount; i++)
				{
					row.Cells[i].Value = 0;
				}

				row.Cells["Row_Col"].Value = row.Index;

				row.Cells["Name_Col"].Value = null;
			}
		}

		private void Load_Monstertxt_Click(object sender, EventArgs e)
		{
			this.StartBrowsing("Monster.txt", (int)MonsterManager.MonsterType.TYPE_MONSTER);
		}

		private void Load_NPCNametxt_Click(object sender, EventArgs e)
		{
			this.StartBrowsing("NPCName.txt", (int)MonsterManager.MonsterType.TYPE_NPCNAME);
		}

		private void StartBrowsing(string file, int type)
		{
			string fileType = file.Substring(file.Length - 3);

			this.OpenMonsterFile.FileName = file;

			this.OpenMonsterFile.Filter = fileType.ToUpper() + " files (*." + fileType + ")|*." + fileType;

			if (this.OpenMonsterFile.ShowDialog() == DialogResult.OK)
			{
				this.monsterPath = this.OpenMonsterFile.FileName;

				this.InitializeGridList();

				bool result = false;

				if (fileType == "txt")
				{
					if (type == (int)MonsterManager.MonsterType.TYPE_MONSTER)
					{
						result = this.ReadMonsterTXT(this.monsterPath);
					}
					else if (type == (int)MonsterManager.MonsterType.TYPE_NPCNAME)
					{
						result = this.ReadNPCNameTXT(this.monsterPath);
					}
				}

				if (result == true)
				{
					this.Monster_List_Box.Visible = true;

					this.Save_Monster_Menu_Btn.Enabled = true;

					this.Search_Monster_Panel.Enabled = true;

					Console.WriteLine("[MonsterManager] {0} loaded", file);
				}
				else
				{
					this.Monster_List_Box.Visible = false;

					this.Save_Monster_Menu_Btn.Enabled = false;

					this.Search_Monster_Panel.Enabled = false;
				}
			}
		}

		private bool ReadMonsterTXT(string path)
		{
			ReadScript lpReadScript = new ReadScript();

			if (lpReadScript.SetBuffer(path) == false)
			{
				Console.WriteLine("Error 1: {0}", lpReadScript.GetLastError());

				return false;
			}

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

					int MonsterIndex = lpReadScript.GetNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["Type_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["Name_Col"].Value = lpReadScript.GetAsString();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["Level_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["MaxLife_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["MaxMana_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["MinDamage_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["MaxDamage_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["Defense_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["MagicDefense_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["AttackRate_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["DefenseRate_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["MoveRange_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["AttackType_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["AttackRange_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["ViewRange_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["MoveSpeed_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["AttackSpeed_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["RegenTime_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["Attribute_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["ItemRate_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["Money_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["MaxItemLevel_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["MonsterSkill_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["ResistanceIce_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["ResistancePoison_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["ResistanceLightning_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["ResistanceFire_Col"].Value = lpReadScript.GetAsNumber();
				}
			}
			catch
			{
				Console.WriteLine("Error 2: {0}", lpReadScript.GetLastError());

				return false;
			}

			return true;
		}

		private void Save_Monstertxt_Click(object sender, EventArgs e)
		{
			this.SaveMonsterFile.FileName = "Monster.txt";

			this.SaveMonsterFile.Filter = "Text files (*.txt)|*.txt";

			if (this.SaveMonsterFile.ShowDialog() == DialogResult.OK)
			{
				using (Stream fs = new FileStream(this.SaveMonsterFile.FileName, FileMode.Create, FileAccess.Write))
				{
					using (StreamWriter sw = new StreamWriter(fs))
					{
						sw.WriteLine("//Index	Rate	Name					Level	MaxLife	MaxMan	MinDmg	MaxDmg	Defense	MagDef	AtkRate	DefRate	MovRang	AtkType	AtkRang	View	MovSped	AtkSped	Regen	Attrib	ItmRate	Money	ItmLvl	Skill	Ice	Poison	Lightin	Fire");

						foreach (DataGridViewRow row in this.Monster_List_Box.Rows)
						{
							if (row.Cells["Name_Col"].Value == null)
							{
								continue;
							}

							if (String.IsNullOrEmpty(row.Cells["Name_Col"].Value.ToString()))
							{
								continue;
							}

							sw.WriteLine(@"{0}	{1}	""{2}""					{3}	{4}	{5}	{6}	{7}	{8}	{9}	{10}	{11}	{12}	{13}	{14}	{15}	{16}	{17}	{18}	{19}	{20}	{21}	{22}	{23}	{24}	{25}	{26}	{27}",
								row.Index,
								Convert.ToInt32(row.Cells["Type_Col"].Value),
								Globals.GetMaxStringLength(row.Cells["Name_Col"].Value.ToString(), 24),
								Convert.ToInt32(row.Cells["Level_Col"].Value),
								Convert.ToInt32(row.Cells["MaxLife_Col"].Value),
								Convert.ToInt32(row.Cells["MaxMana_Col"].Value),
								Convert.ToInt32(row.Cells["MinDamage_Col"].Value),
								Convert.ToInt32(row.Cells["MaxDamage_Col"].Value),
								Convert.ToInt32(row.Cells["Defense_Col"].Value),
								Convert.ToInt32(row.Cells["MagicDefense_Col"].Value),
								Convert.ToInt32(row.Cells["AttackRate_Col"].Value),
								Convert.ToInt32(row.Cells["DefenseRate_Col"].Value),
								Convert.ToInt32(row.Cells["MoveRange_Col"].Value),
								Convert.ToInt32(row.Cells["AttackType_Col"].Value),
								Convert.ToInt32(row.Cells["AttackRange_Col"].Value),
								Convert.ToInt32(row.Cells["ViewRange_Col"].Value),
								Convert.ToInt32(row.Cells["MoveSpeed_Col"].Value),
								Convert.ToInt32(row.Cells["AttackSpeed_Col"].Value),
								Convert.ToInt32(row.Cells["RegenTime_Col"].Value),
								Convert.ToInt32(row.Cells["Attribute_Col"].Value),
								Convert.ToInt32(row.Cells["ItemRate_Col"].Value),
								Convert.ToInt32(row.Cells["Money_Col"].Value),
								Convert.ToInt32(row.Cells["MaxItemLevel_Col"].Value),
								Convert.ToInt32(row.Cells["MonsterSkill_Col"].Value),
								Convert.ToInt32(row.Cells["ResistanceIce_Col"].Value),
								Convert.ToInt32(row.Cells["ResistancePoison_Col"].Value),
								Convert.ToInt32(row.Cells["ResistanceLightning_Col"].Value),
								Convert.ToInt32(row.Cells["ResistanceFire_Col"].Value));
						}

						sw.WriteLine("end");

						Console.WriteLine("[MonsterManager] {0} saved", this.SaveMonsterFile.FileName);
					}
				}
			}
		}

		private bool ReadNPCNameTXT(string path)
		{
			ReadScript lpReadScript = new ReadScript();

			if (lpReadScript.SetBuffer(path) == false)
			{
				Console.WriteLine("Error 1: {0}", lpReadScript.GetLastError());

				return false;
			}

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

					int MonsterIndex = lpReadScript.GetNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["Type_Col"].Value = lpReadScript.GetAsNumber();

					this.Monster_List_Box.Rows[MonsterIndex].Cells["Name_Col"].Value = lpReadScript.GetAsString();
				}
			}
			catch
			{
				Console.WriteLine("Error 2: {0}", lpReadScript.GetLastError());

				return false;
			}

			return true;
		}

		private void Save_NPCNametxt_Click(object sender, EventArgs e)
		{
			this.SaveMonsterFile.FileName = "NPCName.txt";

			this.SaveMonsterFile.Filter = "Text files (*.txt)|*.txt";

			if (this.SaveMonsterFile.ShowDialog() == DialogResult.OK)
			{
				using (Stream fs = new FileStream(this.SaveMonsterFile.FileName, FileMode.Create, FileAccess.Write))
				{
					using (StreamWriter sw = new StreamWriter(fs))
					{
						sw.WriteLine("//Index	Rate	Name");

						foreach (DataGridViewRow row in this.Monster_List_Box.Rows)
						{
							if (row.Cells["Name_Col"].Value == null)
							{
								continue;
							}

							if (String.IsNullOrEmpty(row.Cells["Name_Col"].Value.ToString()))
							{
								continue;
							}

							sw.WriteLine(@"{0}	{1}	""{2}""",
								row.Index,
								Convert.ToInt32(row.Cells["Type_Col"].Value),
								Globals.GetMaxStringLength(row.Cells["Name_Col"].Value.ToString(), 24));
						}

						sw.WriteLine("end");

						Console.WriteLine("[MonsterManager] {0} saved", this.SaveMonsterFile.FileName);
					}
				}
			}
		}

		private void Goto_Text_Box_ValueChanged(object sender, EventArgs e)
		{
			this.Monster_List_Box.FirstDisplayedScrollingRowIndex = (int)this.Goto_Text_Box.Value;
		}

		private void Text_List_Box_Scroll(object sender, ScrollEventArgs e)
		{
			this.Goto_Text_Box.Value = this.Monster_List_Box.FirstDisplayedScrollingRowIndex;
		}

		private void Search_Box_Btn_Click(object sender, EventArgs e)
		{
			string searchValue = this.Search_Box_Text.Text.ToLower();

			if (string.IsNullOrEmpty(searchValue))
			{
				return;
			}

			try
			{
				int i;

				for (i = this.Monster_List_Box.FirstDisplayedScrollingRowIndex; i < this.Monster_List_Box.RowCount; i++)
				{
					if (i == this.Monster_List_Box.FirstDisplayedScrollingRowIndex)
					{
						continue;
					}

					if (this.Monster_List_Box.Rows[i].Cells["Name_Col"].Value == null)
					{
						continue;
					}

					if (this.Monster_List_Box.Rows[i].Cells["Name_Col"].Value.ToString().ToLower().Contains(searchValue))
					{
						this.Goto_Text_Box.Value = i;

						break;
					}
				}

				if (i == this.Monster_List_Box.RowCount)
				{
					MessageBox.Show("No more results.");
				}
			}
			catch
			{
				MessageBox.Show("No matchs found.");
			}
		}
	}
}