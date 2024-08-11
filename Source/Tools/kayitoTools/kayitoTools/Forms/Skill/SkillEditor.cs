using System;
using System.IO;
using System.Text;
using System.Windows.Forms;
using kayitoTools.Source_Files;
using static kayitoTools.Source_Files.ReadScript;

namespace kayitoTools.Forms
{
	public partial class SkillEditor : Form
	{
		private string skillPath;

		private bool ListLoaded = false;

		public SkillEditor()
		{
			InitializeComponent();
		}

		private void SkillEditor_Load(object sender, System.EventArgs e)
		{
			Globals.SetDoubleBuffer(this.Skill_List_Box, true);

			this.Skill_List_Box.RowCount = SkillManager.MAX_SKILLS;

			Console.WriteLine("[SkillManager] Skill Editor Opened");
		}

		private void InitializeGridList()
		{
			this.Skill_List_Box.Visible = false;

			this.ListLoaded = false;

			foreach (DataGridViewRow row in this.Skill_List_Box.Rows)
			{
				for (int j = 0; j < this.Skill_List_Box.Columns.Count; j++)
				{
					row.Cells[j].Value = 0;
				}

				row.Cells["Index_Col"].Value = row.Index;

				row.Cells["Name_Col"].Value = null;
			}

			this.ListLoaded = true;
		}

		private void LoadSkilltxt_Click(object sender, EventArgs e)
		{
			StartBrowsing("skill.txt");
		}

		private void LoadSkillbmd_Click(object sender, EventArgs e)
		{
			StartBrowsing("skill.bmd");
		}

		private void StartBrowsing(string file)
		{
			string fileType = file.Substring(file.Length - 3);

			this.OpenSkillFile.FileName = file;

			this.OpenSkillFile.Filter = fileType.ToUpper() + " files (*." + fileType + ")|*." + fileType;

			if (this.OpenSkillFile.ShowDialog() == DialogResult.OK)
			{
				this.skillPath = this.OpenSkillFile.FileName;

				this.InitializeGridList();

				bool result = false;

				if (fileType == "txt")
				{
					result = this.ReadSkillTXT(this.skillPath);
				}
				else if (fileType == "bmd")
				{
					result = this.ReadSkillBMD(this.skillPath);
				}

				if (result == true)
				{
					this.Skill_List_Box.Visible = true;

					this.Save_Skill_Menu_Btn.Enabled = true;

					Console.WriteLine("[SkillManager] {0} loaded", file);
				}
				else
				{
					this.Skill_List_Box.Visible = false;

					this.Save_Skill_Menu_Btn.Enabled = false;
				}
			}
		}

		private bool ReadSkillTXT(string path)
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

					int SkillIndex = lpReadScript.GetNumber();

					this.Skill_List_Box.Rows[SkillIndex].Cells["Name_Col"].Value = lpReadScript.GetAsString();

					this.Skill_List_Box.Rows[SkillIndex].Cells["Damage_Col"].Value = lpReadScript.GetAsNumber();

					this.Skill_List_Box.Rows[SkillIndex].Cells["Mana_Col"].Value = lpReadScript.GetAsNumber();

					this.Skill_List_Box.Rows[SkillIndex].Cells["Stamina_Col"].Value = lpReadScript.GetAsNumber();

					this.Skill_List_Box.Rows[SkillIndex].Cells["Range_Col"].Value = lpReadScript.GetAsNumber();

					this.Skill_List_Box.Rows[SkillIndex].Cells["Radio_Col"].Value = lpReadScript.GetAsNumber();

					this.Skill_List_Box.Rows[SkillIndex].Cells["Delay_Col"].Value = lpReadScript.GetAsNumber();

					this.Skill_List_Box.Rows[SkillIndex].Cells["Attribute_Col"].Value = lpReadScript.GetAsNumber();

					this.Skill_List_Box.Rows[SkillIndex].Cells["Effect_Col"].Value = lpReadScript.GetAsNumber();

					this.Skill_List_Box.Rows[SkillIndex].Cells["RequireLevel_Col"].Value = lpReadScript.GetAsNumber();

					this.Skill_List_Box.Rows[SkillIndex].Cells["RequireEnergy_Col"].Value = lpReadScript.GetAsNumber();

					this.Skill_List_Box.Rows[SkillIndex].Cells["RequireDW_Col"].Value = lpReadScript.GetAsNumber();

					this.Skill_List_Box.Rows[SkillIndex].Cells["RequireDK_Col"].Value = lpReadScript.GetAsNumber();

					this.Skill_List_Box.Rows[SkillIndex].Cells["RequireELF_Col"].Value = lpReadScript.GetAsNumber();

					this.Skill_List_Box.Rows[SkillIndex].Cells["RequireMG_Col"].Value = lpReadScript.GetAsNumber();
				}
			}
			catch
			{
				Console.WriteLine("Error 2: {0}", lpReadScript.GetLastError());

				return false;
			}

			return true;
		}

		private void SaveSkilltxt_Click(object sender, EventArgs e)
		{
			this.SaveSkillFile.FileName = "skill.txt";

			this.SaveSkillFile.Filter = "Text files (*.txt)|*.txt";

			if (this.SaveSkillFile.ShowDialog() == DialogResult.OK)
			{
				using (Stream fs = new FileStream(this.SaveSkillFile.FileName, FileMode.Create, FileAccess.Write))
				{
					using (StreamWriter sw = new StreamWriter(fs))
					{
						sw.WriteLine("//Index	Name				Damage	MP	BP	Range	Radio	Delay	Attrib	Effect	ReqLvl	ReqEne	ReqCmd	ReqKill	GuildSt	DW	DK	FE	MG	DL");

						for (int index = 0; index < SkillManager.MAX_SKILLS; index++)
						{
							if (this.Skill_List_Box.Rows[index].Cells["Name_Col"].Value == null)
							{
								continue;
							}

							if (String.IsNullOrEmpty(this.Skill_List_Box.Rows[index].Cells["Name_Col"].Value.ToString()))
							{
								continue;
							}

							sw.WriteLine(@"{0}	""{1}""				{2}	{3}	{4}	{5}	{6}	{7}	{8}	{9}	{10}	{11}	{12}	{13}	{14}	{15}",
								Convert.ToInt32(this.Skill_List_Box.Rows[index].Cells["Index_Col"].Value),
								Globals.GetMaxStringLength(this.Skill_List_Box.Rows[index].Cells["Name_Col"].Value.ToString(), 32),
								Convert.ToInt32(this.Skill_List_Box.Rows[index].Cells["Damage_Col"].Value),
								Convert.ToInt32(this.Skill_List_Box.Rows[index].Cells["Mana_Col"].Value),
								Convert.ToInt32(this.Skill_List_Box.Rows[index].Cells["Stamina_Col"].Value),
								Convert.ToInt32(this.Skill_List_Box.Rows[index].Cells["Range_Col"].Value),
								Convert.ToInt32(this.Skill_List_Box.Rows[index].Cells["Radio_Col"].Value),
								Convert.ToInt32(this.Skill_List_Box.Rows[index].Cells["Delay_Col"].Value),
								Convert.ToInt32(this.Skill_List_Box.Rows[index].Cells["Attribute_Col"].Value),
								Convert.ToInt32(this.Skill_List_Box.Rows[index].Cells["Effect_Col"].Value),
								Convert.ToInt32(this.Skill_List_Box.Rows[index].Cells["RequireLevel_Col"].Value),
								Convert.ToInt32(this.Skill_List_Box.Rows[index].Cells["RequireEnergy_Col"].Value),
								Convert.ToInt32(this.Skill_List_Box.Rows[index].Cells["RequireDW_Col"].Value),
								Convert.ToInt32(this.Skill_List_Box.Rows[index].Cells["RequireDK_Col"].Value),
								Convert.ToInt32(this.Skill_List_Box.Rows[index].Cells["RequireELF_Col"].Value),
								Convert.ToInt32(this.Skill_List_Box.Rows[index].Cells["RequireMG_Col"].Value));
						}

						sw.WriteLine("end");

						Console.WriteLine("[SkillManager] {0} saved", this.SaveSkillFile.FileName);
					}
				}
			}
		}

		private bool ReadSkillBMD(string path)
		{
			try
			{
				byte[] buffer = File.ReadAllBytes(path);

				if (buffer.Length != SkillManager.MAX_SKILLS * SkillManager.SKILL_BMD_SIZE + 4)
				{
					Console.WriteLine("[SkillManager] Skill file size is not correct");

					return false;
				}

				byte[] subBuffer = new byte[SkillManager.SKILL_BMD_SIZE];

				for (int i = 0; i < SkillManager.MAX_SKILLS; i++)
				{
					Array.Copy(buffer, i * SkillManager.SKILL_BMD_SIZE, subBuffer, 0, SkillManager.SKILL_BMD_SIZE);

					Array.Copy(BMDFileOpen.BuxConvert(subBuffer, SkillManager.SKILL_BMD_SIZE), 0, buffer, i * SkillManager.SKILL_BMD_SIZE, SkillManager.SKILL_BMD_SIZE);

					string skillName = Encoding.Default.GetString(BMDFileOpen.SubArray(buffer, (SkillManager.SKILL_BMD_SIZE * i + 0), 32)).Trim('\0');

					if (String.IsNullOrEmpty(skillName))
					{
						continue;
					}

					this.Skill_List_Box.Rows[i].Cells["Name_Col"].Value = skillName;
					this.Skill_List_Box.Rows[i].Cells["RequireLevel_Col"].Value = Globals.GetRealByteValue(buffer[SkillManager.SKILL_BMD_SIZE * i + 32]);
					this.Skill_List_Box.Rows[i].Cells["Damage_Col"].Value = Globals.GetRealByteValue(buffer[SkillManager.SKILL_BMD_SIZE * i + 33]);
					this.Skill_List_Box.Rows[i].Cells["Mana_Col"].Value = BitConverter.ToUInt16(BMDFileOpen.SubArray(buffer, (SkillManager.SKILL_BMD_SIZE * i + 34), 2), 0);
					this.Skill_List_Box.Rows[i].Cells["Stamina_Col"].Value = BitConverter.ToUInt16(BMDFileOpen.SubArray(buffer, (SkillManager.SKILL_BMD_SIZE * i + 36), 2), 0);
					this.Skill_List_Box.Rows[i].Cells["Range_Col"].Value = Globals.GetRealByteValue(buffer[SkillManager.SKILL_BMD_SIZE * i + 38]);
					this.Skill_List_Box.Rows[i].Cells["RequireEnergy_Col"].Value = Globals.GetRealByteValue(buffer[SkillManager.SKILL_BMD_SIZE * i + 39]);
				}
			}
			catch
			{
				Console.WriteLine("[SkillManager] Skill file is not correct. Please, use another file");

				return false;
			}

			return true;
		}

		private void SaveSkillbmd_Click(object sender, EventArgs e)
		{
			this.SaveSkillFile.FileName = "skill.bmd";

			this.SaveSkillFile.Filter = "BMD files (*.bmd)|*.bmd";

			if (this.SaveSkillFile.ShowDialog() == DialogResult.OK)
			{
				byte[] buffer = new byte[SkillManager.MAX_SKILLS * SkillManager.SKILL_BMD_SIZE + 4];

				byte[] subBuffer = new byte[SkillManager.SKILL_BMD_SIZE];

				for (int i = 0; i < SkillManager.MAX_SKILLS; i++)
				{
					if (this.Skill_List_Box.Rows[i].Cells["Name_Col"].Value != null && !String.IsNullOrEmpty(this.Skill_List_Box.Rows[i].Cells["Name_Col"].Value.ToString()))
					{
						Array.Copy(BMDFileOpen.StringToByteArray(this.Skill_List_Box.Rows[i].Cells["Name_Col"].Value.ToString(), 32), 0, subBuffer, 0, 32);
						subBuffer[32] = Globals.GetMaxByteValue(Convert.ToInt32(this.Skill_List_Box.Rows[i].Cells["RequireLevel_Col"].Value));
						subBuffer[33] = Globals.GetMaxByteValue(Convert.ToInt32(this.Skill_List_Box.Rows[i].Cells["Damage_Col"].Value));
						Array.Copy(BitConverter.GetBytes(Globals.GetMaxWordValue(Convert.ToInt32(this.Skill_List_Box.Rows[i].Cells["Mana_Col"].Value))), 0, subBuffer, 34, 2);
						Array.Copy(BitConverter.GetBytes(Globals.GetMaxWordValue(Convert.ToInt32(this.Skill_List_Box.Rows[i].Cells["Stamina_Col"].Value))), 0, subBuffer, 36, 2);
						subBuffer[38] = Globals.GetMaxByteValue(Convert.ToInt32(this.Skill_List_Box.Rows[i].Cells["Range_Col"].Value));
						subBuffer[39] = Globals.GetMaxByteValue(Convert.ToInt32(this.Skill_List_Box.Rows[i].Cells["RequireEnergy_Col"].Value));
					}
					else
					{
						Array.Copy(BMDFileOpen.StringToByteArray("", 32), 0, subBuffer, 0, 32);
						subBuffer[32] = 0;
						subBuffer[33] = 0;
						subBuffer[34] = 0;
						subBuffer[35] = 0;
						subBuffer[36] = 0;
						subBuffer[37] = 0;
						subBuffer[38] = 0;
						subBuffer[39] = 0;
					}

					Array.Copy(BMDFileOpen.BuxConvert(subBuffer, SkillManager.SKILL_BMD_SIZE), 0, buffer, i * SkillManager.SKILL_BMD_SIZE, SkillManager.SKILL_BMD_SIZE);
				}

				Array.Copy(BitConverter.GetBytes(BMDFileOpen.GenerateCheckSum(buffer, SkillManager.MAX_SKILLS * SkillManager.SKILL_BMD_SIZE, 0x5A18)), 0, buffer, SkillManager.MAX_SKILLS * SkillManager.SKILL_BMD_SIZE, 4);

				File.WriteAllBytes(this.SaveSkillFile.FileName, buffer);

				Console.WriteLine("[SkillManager] {0} saved", this.SaveSkillFile.FileName);
			}
		}

		private void Skill_List_Box_CellValidating(object sender, DataGridViewCellValidatingEventArgs e)
		{
			if (e.ColumnIndex != this.Skill_List_Box.Columns["Name_Col"].Index)
			{
				int i;

				if (!int.TryParse(Convert.ToString(e.FormattedValue), out i))
				{
					e.Cancel = true;

					MessageBox.Show("Only numbers allowed");
				}
			}
		}

		private void Skill_List_Box_CellValueChanged(object sender, DataGridViewCellEventArgs e)
		{
			if (this.ListLoaded != true)
			{
				return;
			}

			if (e.ColumnIndex == this.Skill_List_Box.Columns["Name_Col"].Index)
			{
				if (this.Skill_List_Box.Rows[e.RowIndex].Cells[e.ColumnIndex].Value != null)
				{
					this.Skill_List_Box.Rows[e.RowIndex].Cells[e.ColumnIndex].Value = Globals.GetMaxStringLength(this.Skill_List_Box.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString(), 32);
				}
			}
		}
	}
}