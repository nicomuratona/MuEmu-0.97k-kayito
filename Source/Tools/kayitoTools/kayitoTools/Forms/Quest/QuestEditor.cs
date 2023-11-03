using System;
using System.IO;
using System.Text;
using System.Windows.Forms;
using kayitoTools.Source_Files;

namespace kayitoTools.Forms
{
	public partial class QuestEditor : Form
	{
		private string questPath;

		private bool ListLoaded = false;

		public QuestEditor()
		{
			InitializeComponent();
		}

		private void QuestEditor_Load(object sender, EventArgs e)
		{
			Globals.SetDoubleBuffer(this.Quest_List_Box, true);

			this.Quest_List_Box.RowCount = QuestManager.MAX_QUESTS;

			Console.WriteLine("[QuestManager] Quest Editor Opened");
		}

		private void InitializeGridList()
		{
			this.Quest_List_Box.Visible = false;

			this.ListLoaded = false;

			foreach (DataGridViewRow row in this.Quest_List_Box.Rows)
			{
				for (int j = 0; j < this.Quest_List_Box.Columns.Count; j++)
				{
					row.Cells[j].Value = 0;
				}

				row.Cells["Index_Col"].Value = row.Index;

				row.Cells["QuestName_Col"].Value = null;
			}

			this.ListLoaded = true;
		}

		private void Load_Questbmd97_Click(object sender, EventArgs e)
		{
			StartBrowsing("Quest.bmd");
		}

		private void StartBrowsing(string file)
		{
			string fileType = file.Substring(file.Length - 3);

			this.OpenQuestFile.FileName = file;

			this.OpenQuestFile.Filter = fileType.ToUpper() + " files (*." + fileType + ")|*." + fileType;

			if (this.OpenQuestFile.ShowDialog() == DialogResult.OK)
			{
				this.questPath = this.OpenQuestFile.FileName;

				this.InitializeGridList();

				bool result = false;

				if (fileType == "bmd")
				{
					result = this.ReadQuestBMD97(this.questPath);
				}

				if (result == true)
				{
					this.Quest_List_Box.Visible = true;

					this.Save_Quest_Menu_Btn.Enabled = true;

					Console.WriteLine("[QuestManager] {0} loaded", file);
				}
				else
				{
					this.Quest_List_Box.Visible = false;

					this.Save_Quest_Menu_Btn.Enabled = false;
				}
			}
		}

		private bool ReadQuestBMD97(string path)
		{
			try
			{
				byte[] buffer = File.ReadAllBytes(path);

				if (buffer.Length != QuestManager.MAX_QUESTS * QuestManager.QUEST_BMD_SIZE)
				{
					Console.WriteLine("[QuestManager] Quest file size is not correct");

					return false;
				}

				byte[] subBuffer = new byte[QuestManager.QUEST_BMD_SIZE];

				for (int i = 0; i < QuestManager.MAX_QUESTS; i++)
				{
					Array.Copy(buffer, i * QuestManager.QUEST_BMD_SIZE, subBuffer, 0, QuestManager.QUEST_BMD_SIZE);

					Array.Copy(BMDFileOpen.BuxConvert(subBuffer, QuestManager.QUEST_BMD_SIZE), 0, buffer, i * QuestManager.QUEST_BMD_SIZE, QuestManager.QUEST_BMD_SIZE);

					this.Quest_List_Box.Rows[i].Cells["QuestQuantity_Col"].Value = BitConverter.ToInt16(BMDFileOpen.SubArray(buffer, (QuestManager.QUEST_BMD_SIZE * i + 0), 2), 0);
					this.Quest_List_Box.Rows[i].Cells["ConditionQuantity_Col"].Value = BitConverter.ToInt16(BMDFileOpen.SubArray(buffer, (QuestManager.QUEST_BMD_SIZE * i + 2), 2), 0);
					this.Quest_List_Box.Rows[i].Cells["NpcNumber_Col"].Value = Globals.GetRealByteValue(buffer[QuestManager.QUEST_BMD_SIZE * i + 4]);
					this.Quest_List_Box.Rows[i].Cells["QuestName_Col"].Value = Encoding.Default.GetString(BMDFileOpen.SubArray(buffer, (QuestManager.QUEST_BMD_SIZE * i + 5), 32)).Trim('\0');

					for (int j = 1; j <= 16; j++)
					{
						int offset = QuestManager.QUEST_CLASS_ACT_SIZE * (j - 1);

						int offset2 = (QuestManager.QUEST_CLASS_ACT_SIZE * 16) + (QuestManager.QUEST_CLASS_REQUEST_SIZE * (j - 1));

						this.Quest_List_Box.Rows[i].Cells["Quest" + j + "Enable_Col"].Value = Globals.GetRealByteValue(buffer[QuestManager.QUEST_BMD_SIZE * i + 38 + offset]);
						this.Quest_List_Box.Rows[i].Cells["Quest" + j + "Type_Col"].Value = Globals.GetRealByteValue(buffer[QuestManager.QUEST_BMD_SIZE * i + 39 + offset]);
						this.Quest_List_Box.Rows[i].Cells["Quest" + j + "NeedType_Col"].Value = Globals.GetRealByteValue(buffer[QuestManager.QUEST_BMD_SIZE * i + 40 + offset]);
						this.Quest_List_Box.Rows[i].Cells["Quest" + j + "NeedSubType_Col"].Value = Globals.GetRealByteValue(buffer[QuestManager.QUEST_BMD_SIZE * i + 41 + offset]);
						this.Quest_List_Box.Rows[i].Cells["Quest" + j + "Quantity_Col"].Value = Globals.GetRealByteValue(buffer[QuestManager.QUEST_BMD_SIZE * i + 42 + offset]);
						this.Quest_List_Box.Rows[i].Cells["Quest" + j + "ConditionType_Col"].Value = Globals.GetRealByteValue(buffer[QuestManager.QUEST_BMD_SIZE * i + 43 + offset]);
						this.Quest_List_Box.Rows[i].Cells["Quest" + j + "ReqDW_Col"].Value = Globals.GetRealByteValue(buffer[QuestManager.QUEST_BMD_SIZE * i + 44 + offset]);
						this.Quest_List_Box.Rows[i].Cells["Quest" + j + "ReqDK_Col"].Value = Globals.GetRealByteValue(buffer[QuestManager.QUEST_BMD_SIZE * i + 45 + offset]);
						this.Quest_List_Box.Rows[i].Cells["Quest" + j + "ReqFE_Col"].Value = Globals.GetRealByteValue(buffer[QuestManager.QUEST_BMD_SIZE * i + 46 + offset]);
						this.Quest_List_Box.Rows[i].Cells["Quest" + j + "ReqMG_Col"].Value = Globals.GetRealByteValue(buffer[QuestManager.QUEST_BMD_SIZE * i + 47 + offset]);
						this.Quest_List_Box.Rows[i].Cells["Quest" + j + "Msg1_Col"].Value = BitConverter.ToInt16(BMDFileOpen.SubArray(buffer, (QuestManager.QUEST_BMD_SIZE * i + 48 + offset), 2), 0);
						this.Quest_List_Box.Rows[i].Cells["Quest" + j + "Msg2_Col"].Value = BitConverter.ToInt16(BMDFileOpen.SubArray(buffer, (QuestManager.QUEST_BMD_SIZE * i + 50 + offset), 2), 0);
						this.Quest_List_Box.Rows[i].Cells["Quest" + j + "Msg3_Col"].Value = BitConverter.ToInt16(BMDFileOpen.SubArray(buffer, (QuestManager.QUEST_BMD_SIZE * i + 52 + offset), 2), 0);
						this.Quest_List_Box.Rows[i].Cells["Quest" + j + "Msg4_Col"].Value = BitConverter.ToInt16(BMDFileOpen.SubArray(buffer, (QuestManager.QUEST_BMD_SIZE * i + 54 + offset), 2), 0);

						this.Quest_List_Box.Rows[i].Cells["Condition" + j + "Enable_Col"].Value = Globals.GetRealByteValue(buffer[QuestManager.QUEST_BMD_SIZE * i + 40 + offset2]);
						this.Quest_List_Box.Rows[i].Cells["Condition" + j + "Type_Col"].Value = Globals.GetRealByteValue(buffer[QuestManager.QUEST_BMD_SIZE * i + 41 + offset2]);
						this.Quest_List_Box.Rows[i].Cells["Condition" + j + "RequiredQuestState_Col"].Value = BitConverter.ToInt16(BMDFileOpen.SubArray(buffer, (QuestManager.QUEST_BMD_SIZE * i + 42 + offset2), 2), 0);
						this.Quest_List_Box.Rows[i].Cells["Condition" + j + "LevelMin_Col"].Value = BitConverter.ToInt16(BMDFileOpen.SubArray(buffer, (QuestManager.QUEST_BMD_SIZE * i + 44 + offset2), 2), 0);
						this.Quest_List_Box.Rows[i].Cells["Condition" + j + "LevelMax_Col"].Value = BitConverter.ToInt16(BMDFileOpen.SubArray(buffer, (QuestManager.QUEST_BMD_SIZE * i + 46 + offset2), 2), 0);
						this.Quest_List_Box.Rows[i].Cells["Condition" + j + "ReqZen_Col"].Value = BitConverter.ToInt32(BMDFileOpen.SubArray(buffer, (QuestManager.QUEST_BMD_SIZE * i + 48 + offset2), 4), 0);
						this.Quest_List_Box.Rows[i].Cells["Condition" + j + "ErrorText_Col"].Value = BitConverter.ToInt16(BMDFileOpen.SubArray(buffer, (QuestManager.QUEST_BMD_SIZE * i + 52 + offset2), 2), 0);
					}
				}
			}
			catch
			{
				Console.WriteLine("[QuestManager] Quest file is not correct. Please, use another file");

				return false;
			}

			return true;
		}

		private void Save_Questbmd_Click(object sender, EventArgs e)
		{
			this.SaveQuestFile.FileName = "quest.bmd";

			this.SaveQuestFile.Filter = "BMD files (*.bmd)|*.bmd";

			if (this.SaveQuestFile.ShowDialog() == DialogResult.OK)
			{
				byte[] buffer = new byte[QuestManager.MAX_QUESTS * QuestManager.QUEST_BMD_SIZE];

				byte[] subBuffer = new byte[QuestManager.QUEST_BMD_SIZE];

				for (int i = 0; i < QuestManager.MAX_QUESTS; i++)
				{
					Array.Copy(BitConverter.GetBytes(Globals.GetMaxWordValue(Convert.ToInt32(this.Quest_List_Box.Rows[i].Cells["QuestQuantity_Col"].Value))), 0, subBuffer, 0, 2);
					Array.Copy(BitConverter.GetBytes(Globals.GetMaxWordValue(Convert.ToInt32(this.Quest_List_Box.Rows[i].Cells["ConditionQuantity_Col"].Value))), 0, subBuffer, 2, 2);
					subBuffer[4] = Globals.GetMaxByteValue(Convert.ToInt32(this.Quest_List_Box.Rows[i].Cells["NpcNumber_Col"].Value));

					if (this.Quest_List_Box.Rows[i].Cells["QuestName_Col"].Value != null && !String.IsNullOrEmpty(this.Quest_List_Box.Rows[i].Cells["QuestName_Col"].Value.ToString()))
					{
						Array.Copy(BMDFileOpen.StringToByteArray(this.Quest_List_Box.Rows[i].Cells["QuestName_Col"].Value.ToString(), 32), 0, subBuffer, 5, 32);
					}
					else
					{
						Array.Copy(BMDFileOpen.StringToByteArray("", 32), 0, subBuffer, 5, 32);
					}

					subBuffer[37] = 0;

					subBuffer[QuestManager.QUEST_CLASS_ACT_SIZE * 16] = 0;
					subBuffer[QuestManager.QUEST_CLASS_ACT_SIZE * 16 + 1] = 0;

					for (int j = 1; j <= 16; j++)
					{
						int offset = QuestManager.QUEST_CLASS_ACT_SIZE * (j - 1);

						int offset2 = (QuestManager.QUEST_CLASS_ACT_SIZE * 16) + (QuestManager.QUEST_CLASS_REQUEST_SIZE * (j - 1));

						subBuffer[38 + offset] = Globals.GetMaxByteValue(Convert.ToInt32(this.Quest_List_Box.Rows[i].Cells["Quest" + j + "Enable_Col"].Value));
						subBuffer[39 + offset] = Globals.GetMaxByteValue(Convert.ToInt32(this.Quest_List_Box.Rows[i].Cells["Quest" + j + "Type_Col"].Value));
						subBuffer[40 + offset] = Globals.GetMaxByteValue(Convert.ToInt32(this.Quest_List_Box.Rows[i].Cells["Quest" + j + "NeedType_Col"].Value));
						subBuffer[41 + offset] = Globals.GetMaxByteValue(Convert.ToInt32(this.Quest_List_Box.Rows[i].Cells["Quest" + j + "NeedSubType_Col"].Value));
						subBuffer[42 + offset] = Globals.GetMaxByteValue(Convert.ToInt32(this.Quest_List_Box.Rows[i].Cells["Quest" + j + "Quantity_Col"].Value));
						subBuffer[43 + offset] = Globals.GetMaxByteValue(Convert.ToInt32(this.Quest_List_Box.Rows[i].Cells["Quest" + j + "ConditionType_Col"].Value));
						subBuffer[44 + offset] = Globals.GetMaxByteValue(Convert.ToInt32(this.Quest_List_Box.Rows[i].Cells["Quest" + j + "ReqDW_Col"].Value));
						subBuffer[45 + offset] = Globals.GetMaxByteValue(Convert.ToInt32(this.Quest_List_Box.Rows[i].Cells["Quest" + j + "ReqDK_Col"].Value));
						subBuffer[46 + offset] = Globals.GetMaxByteValue(Convert.ToInt32(this.Quest_List_Box.Rows[i].Cells["Quest" + j + "ReqFE_Col"].Value));
						subBuffer[47 + offset] = Globals.GetMaxByteValue(Convert.ToInt32(this.Quest_List_Box.Rows[i].Cells["Quest" + j + "ReqMG_Col"].Value));
						Array.Copy(BitConverter.GetBytes(Globals.GetMaxWordValue(Convert.ToInt32(this.Quest_List_Box.Rows[i].Cells["Quest" + j + "Msg1_Col"].Value))), 0, subBuffer, 48 + offset, 2);
						Array.Copy(BitConverter.GetBytes(Globals.GetMaxWordValue(Convert.ToInt32(this.Quest_List_Box.Rows[i].Cells["Quest" + j + "Msg2_Col"].Value))), 0, subBuffer, 50 + offset, 2);
						Array.Copy(BitConverter.GetBytes(Globals.GetMaxWordValue(Convert.ToInt32(this.Quest_List_Box.Rows[i].Cells["Quest" + j + "Msg3_Col"].Value))), 0, subBuffer, 52 + offset, 2);
						Array.Copy(BitConverter.GetBytes(Globals.GetMaxWordValue(Convert.ToInt32(this.Quest_List_Box.Rows[i].Cells["Quest" + j + "Msg4_Col"].Value))), 0, subBuffer, 54 + offset, 2);

						subBuffer[40 + offset2] = Globals.GetMaxByteValue(Convert.ToInt32(this.Quest_List_Box.Rows[i].Cells["Condition" + j + "Enable_Col"].Value));
						subBuffer[41 + offset2] = Globals.GetMaxByteValue(Convert.ToInt32(this.Quest_List_Box.Rows[i].Cells["Condition" + j + "Type_Col"].Value));
						Array.Copy(BitConverter.GetBytes(Globals.GetMaxWordValue(Convert.ToInt32(this.Quest_List_Box.Rows[i].Cells["Condition" + j + "RequiredQuestState_Col"].Value))), 0, subBuffer, 42 + offset2, 2);
						Array.Copy(BitConverter.GetBytes(Globals.GetMaxWordValue(Convert.ToInt32(this.Quest_List_Box.Rows[i].Cells["Condition" + j + "LevelMin_Col"].Value))), 0, subBuffer, 44 + offset2, 2);
						Array.Copy(BitConverter.GetBytes(Globals.GetMaxWordValue(Convert.ToInt32(this.Quest_List_Box.Rows[i].Cells["Condition" + j + "LevelMax_Col"].Value))), 0, subBuffer, 46 + offset2, 2);
						Array.Copy(BitConverter.GetBytes(Globals.GetMaxDwordValue(Convert.ToInt32(this.Quest_List_Box.Rows[i].Cells["Condition" + j + "ReqZen_Col"].Value))), 0, subBuffer, 48 + offset2, 4);
						Array.Copy(BitConverter.GetBytes(Globals.GetMaxWordValue(Convert.ToInt32(this.Quest_List_Box.Rows[i].Cells["Condition" + j + "ErrorText_Col"].Value))), 0, subBuffer, 52 + offset2, 2);
						subBuffer[54 + offset2] = 0;
						subBuffer[55 + offset2] = 0;
					}

					Array.Copy(BMDFileOpen.BuxConvert(subBuffer, QuestManager.QUEST_BMD_SIZE), 0, buffer, i * QuestManager.QUEST_BMD_SIZE, QuestManager.QUEST_BMD_SIZE);
				}

				File.WriteAllBytes(this.SaveQuestFile.FileName, buffer);

				Console.WriteLine("[QuestManager] {0} saved", this.SaveQuestFile.FileName);
			}
		}

		private void Quest_List_Box_CellValidating(object sender, DataGridViewCellValidatingEventArgs e)
		{
			if (e.ColumnIndex != this.Quest_List_Box.Columns["QuestName_Col"].Index)
			{
				int i;

				if (!int.TryParse(Convert.ToString(e.FormattedValue), out i))
				{
					e.Cancel = true;

					MessageBox.Show("Only numbers allowed");
				}
			}
		}

		private void Quest_List_Box_CellValueChanged(object sender, DataGridViewCellEventArgs e)
		{
			if (this.ListLoaded != true)
			{
				return;
			}

			if (e.ColumnIndex == this.Quest_List_Box.Columns["QuestName_Col"].Index)
			{
				if (this.Quest_List_Box.Rows[e.RowIndex].Cells[e.ColumnIndex].Value != null)
				{
					this.Quest_List_Box.Rows[e.RowIndex].Cells[e.ColumnIndex].Value = Globals.GetMaxStringLength(this.Quest_List_Box.Rows[e.RowIndex].Cells[e.ColumnIndex].Value.ToString(), 32);
				}
			}
		}
	}
}