using System;
using System.IO;
using System.Text;
using System.Windows.Forms;
using kayitoTools.Source_Files;

namespace kayitoTools.Forms
{
	public partial class DialogEditor : Form
	{
		private string dialogPath;

		public DialogEditor()
		{
			InitializeComponent();
		}

		private void DialogEditor_Load(object sender, EventArgs e)
		{
			Globals.SetDoubleBuffer(this.Dialog_List_Box, true);

			this.Dialog_List_Box.RowCount = DialogManager.MAX_ROWS_DIALOG;

			Console.WriteLine("[DialogManager] Dialog Editor Opened");
		}

		private void InitializeGridList()
		{
			this.Dialog_List_Box.Visible = false;

			foreach (DataGridViewRow row in this.Dialog_List_Box.Rows)
			{
				int j;

				for (j = 2; j < this.Dialog_List_Box.ColumnCount; j++)
				{
					row.Cells[j].Value = 0;
				}

				row.Cells["Row_Col"].Value = row.Index;

				row.Cells["NPCDialog_Col"].Value = null;

				row.Cells["PlayerResponse1_Col"].Value = null;
				row.Cells["PlayerResponse2_Col"].Value = null;
				row.Cells["PlayerResponse3_Col"].Value = null;
				row.Cells["PlayerResponse4_Col"].Value = null;
				row.Cells["PlayerResponse5_Col"].Value = null;
				row.Cells["PlayerResponse6_Col"].Value = null;
				row.Cells["PlayerResponse7_Col"].Value = null;
				row.Cells["PlayerResponse8_Col"].Value = null;
				row.Cells["PlayerResponse9_Col"].Value = null;
				row.Cells["PlayerResponse10_Col"].Value = null;
			}
		}

		private void Load_Dialogbmd(object sender, EventArgs e)
		{
			this.StartBrowsing("Dialog.bmd");
		}

		private void StartBrowsing(string file)
		{
			string fileType = file.Substring(file.Length - 3);

			this.OpenDialogFile.FileName = file;

			this.OpenDialogFile.Filter = fileType.ToUpper() + " files (*." + fileType + ")|*." + fileType;

			if (this.OpenDialogFile.ShowDialog() == DialogResult.OK)
			{
				this.dialogPath = this.OpenDialogFile.FileName;

				this.InitializeGridList();

				bool result = false;

				if (fileType == "bmd")
				{
					result = this.ReadDialogBMD(this.dialogPath);
				}

				if (result == true)
				{
					this.Dialog_List_Box.Visible = true;

					this.Save_Dialog_Menu_Btn.Enabled = true;

					Console.WriteLine("[DialogManager] {0} loaded", file);
				}
				else
				{
					this.Dialog_List_Box.Visible = false;

					this.Save_Dialog_Menu_Btn.Enabled = false;
				}
			}
		}

		private bool ReadDialogBMD(string path)
		{
			try
			{
				byte[] buffer = File.ReadAllBytes(path);

				if (buffer.Length != DialogManager.MAX_ROWS_DIALOG * DialogManager.MAX_DIALOG_BMD_SIZE)
				{
					Console.WriteLine("[DialogManager] Dialog file size is not correct");

					return false;
				}

				byte[] subBuffer = new byte[DialogManager.MAX_DIALOG_BMD_SIZE];

				for (int i = 0; i < DialogManager.MAX_ROWS_DIALOG; i++)
				{
					Array.Copy(buffer, i * DialogManager.MAX_DIALOG_BMD_SIZE, subBuffer, 0, DialogManager.MAX_DIALOG_BMD_SIZE);

					Array.Copy(BMDFileOpen.BuxConvert(subBuffer, DialogManager.MAX_DIALOG_BMD_SIZE), 0, buffer, i * DialogManager.MAX_DIALOG_BMD_SIZE, DialogManager.MAX_DIALOG_BMD_SIZE);

					string NPCDialog = Encoding.Default.GetString(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 0), DialogManager.MAX_DIALOG_NPCMESSAGE_SIZE)).Trim('\0');

					if (String.IsNullOrEmpty(NPCDialog))
					{
						continue;
					}

					this.Dialog_List_Box.Rows[i].Cells["NPCDialog_Col"].Value = NPCDialog;

					this.Dialog_List_Box.Rows[i].Cells["AnswersQuant_Col"].Value = BitConverter.ToInt32(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 300), 4), 0);

					this.Dialog_List_Box.Rows[i].Cells["NPCResponse1_Col"].Value = BitConverter.ToInt32(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 304), 4), 0);
					this.Dialog_List_Box.Rows[i].Cells["NPCResponse2_Col"].Value = BitConverter.ToInt32(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 308), 4), 0);
					this.Dialog_List_Box.Rows[i].Cells["NPCResponse3_Col"].Value = BitConverter.ToInt32(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 312), 4), 0);
					this.Dialog_List_Box.Rows[i].Cells["NPCResponse4_Col"].Value = BitConverter.ToInt32(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 316), 4), 0);
					this.Dialog_List_Box.Rows[i].Cells["NPCResponse5_Col"].Value = BitConverter.ToInt32(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 320), 4), 0);
					this.Dialog_List_Box.Rows[i].Cells["NPCResponse6_Col"].Value = BitConverter.ToInt32(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 324), 4), 0);
					this.Dialog_List_Box.Rows[i].Cells["NPCResponse7_Col"].Value = BitConverter.ToInt32(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 328), 4), 0);
					this.Dialog_List_Box.Rows[i].Cells["NPCResponse8_Col"].Value = BitConverter.ToInt32(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 332), 4), 0);
					this.Dialog_List_Box.Rows[i].Cells["NPCResponse9_Col"].Value = BitConverter.ToInt32(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 336), 4), 0);
					this.Dialog_List_Box.Rows[i].Cells["NPCResponse10_Col"].Value = BitConverter.ToInt32(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 340), 4), 0);

					this.Dialog_List_Box.Rows[i].Cells["NPCCondition1_Col"].Value = BitConverter.ToInt32(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 344), 4), 0);
					this.Dialog_List_Box.Rows[i].Cells["NPCCondition2_Col"].Value = BitConverter.ToInt32(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 348), 4), 0);
					this.Dialog_List_Box.Rows[i].Cells["NPCCondition3_Col"].Value = BitConverter.ToInt32(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 352), 4), 0);
					this.Dialog_List_Box.Rows[i].Cells["NPCCondition4_Col"].Value = BitConverter.ToInt32(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 356), 4), 0);
					this.Dialog_List_Box.Rows[i].Cells["NPCCondition5_Col"].Value = BitConverter.ToInt32(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 360), 4), 0);
					this.Dialog_List_Box.Rows[i].Cells["NPCCondition6_Col"].Value = BitConverter.ToInt32(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 364), 4), 0);
					this.Dialog_List_Box.Rows[i].Cells["NPCCondition7_Col"].Value = BitConverter.ToInt32(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 368), 4), 0);
					this.Dialog_List_Box.Rows[i].Cells["NPCCondition8_Col"].Value = BitConverter.ToInt32(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 372), 4), 0);
					this.Dialog_List_Box.Rows[i].Cells["NPCCondition9_Col"].Value = BitConverter.ToInt32(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 376), 4), 0);
					this.Dialog_List_Box.Rows[i].Cells["NPCCondition10_Col"].Value = BitConverter.ToInt32(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 380), 4), 0);

					this.Dialog_List_Box.Rows[i].Cells["PlayerResponse1_Col"].Value = Encoding.Default.GetString(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 384), DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE)).Trim('\0');
					this.Dialog_List_Box.Rows[i].Cells["PlayerResponse2_Col"].Value = Encoding.Default.GetString(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 448), DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE)).Trim('\0');
					this.Dialog_List_Box.Rows[i].Cells["PlayerResponse3_Col"].Value = Encoding.Default.GetString(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 512), DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE)).Trim('\0');
					this.Dialog_List_Box.Rows[i].Cells["PlayerResponse4_Col"].Value = Encoding.Default.GetString(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 576), DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE)).Trim('\0');
					this.Dialog_List_Box.Rows[i].Cells["PlayerResponse5_Col"].Value = Encoding.Default.GetString(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 640), DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE)).Trim('\0');
					this.Dialog_List_Box.Rows[i].Cells["PlayerResponse6_Col"].Value = Encoding.Default.GetString(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 704), DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE)).Trim('\0');
					this.Dialog_List_Box.Rows[i].Cells["PlayerResponse7_Col"].Value = Encoding.Default.GetString(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 768), DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE)).Trim('\0');
					this.Dialog_List_Box.Rows[i].Cells["PlayerResponse8_Col"].Value = Encoding.Default.GetString(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 832), DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE)).Trim('\0');
					this.Dialog_List_Box.Rows[i].Cells["PlayerResponse9_Col"].Value = Encoding.Default.GetString(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 896), DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE)).Trim('\0');
					this.Dialog_List_Box.Rows[i].Cells["PlayerResponse10_Col"].Value = Encoding.Default.GetString(BMDFileOpen.SubArray(buffer, (DialogManager.MAX_DIALOG_BMD_SIZE * i + 960), DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE)).Trim('\0');
				}
			}
			catch
			{
				Console.WriteLine("[DialogManager] Dialog file is not correct. Please, use another file");

				return false;
			}

			return true;
		}

		private void SaveDialogbmd_Click(object sender, EventArgs e)
		{
			this.SaveDialogFile.FileName = "Dialog.bmd";

			this.SaveDialogFile.Filter = "BMD files (*.bmd)|*.bmd";

			if (this.SaveDialogFile.ShowDialog() == DialogResult.OK)
			{
				byte[] buffer = new byte[DialogManager.MAX_ROWS_DIALOG * DialogManager.MAX_DIALOG_BMD_SIZE];

				byte[] subBuffer = new byte[DialogManager.MAX_DIALOG_BMD_SIZE];

				foreach (DataGridViewRow row in this.Dialog_List_Box.Rows)
				{
					if (row.Cells["NPCDialog_Col"].Value != null && !String.IsNullOrEmpty(row.Cells["NPCDialog_Col"].Value.ToString()))
					{
						Array.Copy(BMDFileOpen.StringToByteArray(row.Cells["NPCDialog_Col"].Value.ToString(), DialogManager.MAX_DIALOG_NPCMESSAGE_SIZE), 0, subBuffer, 0, DialogManager.MAX_DIALOG_NPCMESSAGE_SIZE);
					}
					else
					{
						Array.Copy(BMDFileOpen.StringToByteArray("", DialogManager.MAX_DIALOG_NPCMESSAGE_SIZE), 0, subBuffer, 0, DialogManager.MAX_DIALOG_NPCMESSAGE_SIZE);
					}

					Array.Copy(BitConverter.GetBytes(Globals.GetMaxDwordValue(Convert.ToInt32(row.Cells["AnswersQuant_Col"].Value))), 0, subBuffer, 300, 4);

					Array.Copy(BitConverter.GetBytes(Globals.GetMaxDwordValue(Convert.ToInt32(row.Cells["NPCResponse1_Col"].Value))), 0, subBuffer, 304, 4);
					Array.Copy(BitConverter.GetBytes(Globals.GetMaxDwordValue(Convert.ToInt32(row.Cells["NPCResponse2_Col"].Value))), 0, subBuffer, 308, 4);
					Array.Copy(BitConverter.GetBytes(Globals.GetMaxDwordValue(Convert.ToInt32(row.Cells["NPCResponse3_Col"].Value))), 0, subBuffer, 312, 4);
					Array.Copy(BitConverter.GetBytes(Globals.GetMaxDwordValue(Convert.ToInt32(row.Cells["NPCResponse4_Col"].Value))), 0, subBuffer, 316, 4);
					Array.Copy(BitConverter.GetBytes(Globals.GetMaxDwordValue(Convert.ToInt32(row.Cells["NPCResponse5_Col"].Value))), 0, subBuffer, 320, 4);
					Array.Copy(BitConverter.GetBytes(Globals.GetMaxDwordValue(Convert.ToInt32(row.Cells["NPCResponse6_Col"].Value))), 0, subBuffer, 324, 4);
					Array.Copy(BitConverter.GetBytes(Globals.GetMaxDwordValue(Convert.ToInt32(row.Cells["NPCResponse7_Col"].Value))), 0, subBuffer, 328, 4);
					Array.Copy(BitConverter.GetBytes(Globals.GetMaxDwordValue(Convert.ToInt32(row.Cells["NPCResponse8_Col"].Value))), 0, subBuffer, 332, 4);
					Array.Copy(BitConverter.GetBytes(Globals.GetMaxDwordValue(Convert.ToInt32(row.Cells["NPCResponse9_Col"].Value))), 0, subBuffer, 336, 4);
					Array.Copy(BitConverter.GetBytes(Globals.GetMaxDwordValue(Convert.ToInt32(row.Cells["NPCResponse10_Col"].Value))), 0, subBuffer, 340, 4);

					Array.Copy(BitConverter.GetBytes(Globals.GetMaxDwordValue(Convert.ToInt32(row.Cells["NPCCondition1_Col"].Value))), 0, subBuffer, 344, 4);
					Array.Copy(BitConverter.GetBytes(Globals.GetMaxDwordValue(Convert.ToInt32(row.Cells["NPCCondition2_Col"].Value))), 0, subBuffer, 348, 4);
					Array.Copy(BitConverter.GetBytes(Globals.GetMaxDwordValue(Convert.ToInt32(row.Cells["NPCCondition3_Col"].Value))), 0, subBuffer, 352, 4);
					Array.Copy(BitConverter.GetBytes(Globals.GetMaxDwordValue(Convert.ToInt32(row.Cells["NPCCondition4_Col"].Value))), 0, subBuffer, 356, 4);
					Array.Copy(BitConverter.GetBytes(Globals.GetMaxDwordValue(Convert.ToInt32(row.Cells["NPCCondition5_Col"].Value))), 0, subBuffer, 360, 4);
					Array.Copy(BitConverter.GetBytes(Globals.GetMaxDwordValue(Convert.ToInt32(row.Cells["NPCCondition6_Col"].Value))), 0, subBuffer, 364, 4);
					Array.Copy(BitConverter.GetBytes(Globals.GetMaxDwordValue(Convert.ToInt32(row.Cells["NPCCondition7_Col"].Value))), 0, subBuffer, 368, 4);
					Array.Copy(BitConverter.GetBytes(Globals.GetMaxDwordValue(Convert.ToInt32(row.Cells["NPCCondition8_Col"].Value))), 0, subBuffer, 372, 4);
					Array.Copy(BitConverter.GetBytes(Globals.GetMaxDwordValue(Convert.ToInt32(row.Cells["NPCCondition9_Col"].Value))), 0, subBuffer, 376, 4);
					Array.Copy(BitConverter.GetBytes(Globals.GetMaxDwordValue(Convert.ToInt32(row.Cells["NPCCondition10_Col"].Value))), 0, subBuffer, 380, 4);

					if (row.Cells["PlayerResponse1_Col"].Value != null && !String.IsNullOrEmpty(row.Cells["PlayerResponse1_Col"].Value.ToString()))
					{
						Array.Copy(BMDFileOpen.StringToByteArray(row.Cells["PlayerResponse1_Col"].Value.ToString(), DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE), 0, subBuffer, 384, DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE);
					}
					else
					{
						Array.Copy(BMDFileOpen.StringToByteArray("", DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE), 0, subBuffer, 384, DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE);
					}

					if (row.Cells["PlayerResponse2_Col"].Value != null && !String.IsNullOrEmpty(row.Cells["PlayerResponse2_Col"].Value.ToString()))
					{
						Array.Copy(BMDFileOpen.StringToByteArray(row.Cells["PlayerResponse2_Col"].Value.ToString(), DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE), 0, subBuffer, 448, DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE);
					}
					else
					{
						Array.Copy(BMDFileOpen.StringToByteArray("", DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE), 0, subBuffer, 448, DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE);
					}

					if (row.Cells["PlayerResponse3_Col"].Value != null && !String.IsNullOrEmpty(row.Cells["PlayerResponse3_Col"].Value.ToString()))
					{
						Array.Copy(BMDFileOpen.StringToByteArray(row.Cells["PlayerResponse3_Col"].Value.ToString(), DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE), 0, subBuffer, 512, DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE);
					}
					else
					{
						Array.Copy(BMDFileOpen.StringToByteArray("", DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE), 0, subBuffer, 512, DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE);
					}

					if (row.Cells["PlayerResponse4_Col"].Value != null && !String.IsNullOrEmpty(row.Cells["PlayerResponse4_Col"].Value.ToString()))
					{
						Array.Copy(BMDFileOpen.StringToByteArray(row.Cells["PlayerResponse4_Col"].Value.ToString(), DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE), 0, subBuffer, 576, DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE);
					}
					else
					{
						Array.Copy(BMDFileOpen.StringToByteArray("", DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE), 0, subBuffer, 576, DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE);
					}

					if (row.Cells["PlayerResponse5_Col"].Value != null && !String.IsNullOrEmpty(row.Cells["PlayerResponse5_Col"].Value.ToString()))
					{
						Array.Copy(BMDFileOpen.StringToByteArray(row.Cells["PlayerResponse5_Col"].Value.ToString(), DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE), 0, subBuffer, 640, DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE);
					}
					else
					{
						Array.Copy(BMDFileOpen.StringToByteArray("", DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE), 0, subBuffer, 640, DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE);
					}

					if (row.Cells["PlayerResponse6_Col"].Value != null && !String.IsNullOrEmpty(row.Cells["PlayerResponse6_Col"].Value.ToString()))
					{
						Array.Copy(BMDFileOpen.StringToByteArray(row.Cells["PlayerResponse6_Col"].Value.ToString(), DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE), 0, subBuffer, 704, DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE);
					}
					else
					{
						Array.Copy(BMDFileOpen.StringToByteArray("", DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE), 0, subBuffer, 704, DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE);
					}

					if (row.Cells["PlayerResponse7_Col"].Value != null && !String.IsNullOrEmpty(row.Cells["PlayerResponse7_Col"].Value.ToString()))
					{
						Array.Copy(BMDFileOpen.StringToByteArray(row.Cells["PlayerResponse7_Col"].Value.ToString(), DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE), 0, subBuffer, 768, DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE);
					}
					else
					{
						Array.Copy(BMDFileOpen.StringToByteArray("", DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE), 0, subBuffer, 768, DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE);
					}

					if (row.Cells["PlayerResponse8_Col"].Value != null && !String.IsNullOrEmpty(row.Cells["PlayerResponse8_Col"].Value.ToString()))
					{
						Array.Copy(BMDFileOpen.StringToByteArray(row.Cells["PlayerResponse8_Col"].Value.ToString(), DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE), 0, subBuffer, 832, DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE);
					}
					else
					{
						Array.Copy(BMDFileOpen.StringToByteArray("", DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE), 0, subBuffer, 832, DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE);
					}

					if (row.Cells["PlayerResponse9_Col"].Value != null && !String.IsNullOrEmpty(row.Cells["PlayerResponse9_Col"].Value.ToString()))
					{
						Array.Copy(BMDFileOpen.StringToByteArray(row.Cells["PlayerResponse9_Col"].Value.ToString(), DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE), 0, subBuffer, 896, DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE);
					}
					else
					{
						Array.Copy(BMDFileOpen.StringToByteArray("", DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE), 0, subBuffer, 896, DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE);
					}

					if (row.Cells["PlayerResponse10_Col"].Value != null && !String.IsNullOrEmpty(row.Cells["PlayerResponse10_Col"].Value.ToString()))
					{
						Array.Copy(BMDFileOpen.StringToByteArray(row.Cells["PlayerResponse10_Col"].Value.ToString(), DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE), 0, subBuffer, 960, DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE);
					}
					else
					{
						Array.Copy(BMDFileOpen.StringToByteArray("", DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE), 0, subBuffer, 960, DialogManager.MAX_DIALOG_PLAYERMESSAGE_SIZE);
					}

					Array.Copy(BMDFileOpen.BuxConvert(subBuffer, DialogManager.MAX_DIALOG_BMD_SIZE), 0, buffer, row.Index * DialogManager.MAX_DIALOG_BMD_SIZE, DialogManager.MAX_DIALOG_BMD_SIZE);
				}

				File.WriteAllBytes(this.SaveDialogFile.FileName, buffer);

				Console.WriteLine("[DialogManager] {0} saved", this.SaveDialogFile.FileName);
			}
		}
	}
}