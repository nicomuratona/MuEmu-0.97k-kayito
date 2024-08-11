using System;
using System.IO;
using System.Windows.Forms;
using kayitoTools.Source_Files;
using static kayitoTools.Source_Files.ReadScript;

namespace kayitoTools.Forms
{
	public partial class GateEditor : Form
	{
		private string gatePath;

		public GateEditor()
		{
			InitializeComponent();
		}

		private void GateEditor_Load(object sender, EventArgs e)
		{
			Globals.SetDoubleBuffer(this.Gate_List_Box, true);

			this.Gate_List_Box.RowCount = GateManager.MAX_GATES;

			Console.WriteLine("[GateManager] Gate Editor Opened");
		}

		private void InitializeGridList()
		{
			this.Gate_List_Box.Visible = false;

			foreach (DataGridViewRow row in this.Gate_List_Box.Rows)
			{
				for (int j = 0; j < this.Gate_List_Box.Columns.Count; j++)
				{
					row.Cells[j].Value = 0;
				}

				row.Cells["Index_Col"].Value = row.Index;
			}
		}

		private void LoadGatetxt_Click(object sender, EventArgs e)
		{
			StartBrowsing("Gate.txt");
		}

		private void LoadGatebmd_Click(object sender, EventArgs e)
		{
			StartBrowsing("Gate.bmd");
		}

		private void StartBrowsing(string file)
		{
			string fileType = file.Substring(file.Length - 3);

			this.OpenGateFile.FileName = file;

			this.OpenGateFile.Filter = fileType.ToUpper() + " files (*." + fileType + ")|*." + fileType;

			if (this.OpenGateFile.ShowDialog() == DialogResult.OK)
			{
				this.gatePath = this.OpenGateFile.FileName;

				this.InitializeGridList();

				bool result = false;

				if (fileType == "txt")
				{
					result = this.ReadGateTXT(this.gatePath);
				}
				else if (fileType == "bmd")
				{
					result = this.ReadGateBMD(this.gatePath);
				}

				if (result == true)
				{
					this.Gate_List_Box.Visible = true;

					this.Save_Gate_Menu_Btn.Enabled = true;

					Console.WriteLine("[GateManager] {0} loaded", file);
				}
				else
				{
					this.Gate_List_Box.Visible = false;

					this.Save_Gate_Menu_Btn.Enabled = false;
				}
			}
		}

		private bool ReadGateTXT(string path)
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

					int GateIndex = lpReadScript.GetNumber();

					this.Gate_List_Box.Rows[GateIndex].Cells["Flag_Col"].Value = lpReadScript.GetAsNumber();

					this.Gate_List_Box.Rows[GateIndex].Cells["Map_Col"].Value = lpReadScript.GetAsNumber();

					this.Gate_List_Box.Rows[GateIndex].Cells["StartX_Col"].Value = lpReadScript.GetAsNumber();

					this.Gate_List_Box.Rows[GateIndex].Cells["StartY_Col"].Value = lpReadScript.GetAsNumber();

					this.Gate_List_Box.Rows[GateIndex].Cells["EndX_Col"].Value = lpReadScript.GetAsNumber();

					this.Gate_List_Box.Rows[GateIndex].Cells["EndY_Col"].Value = lpReadScript.GetAsNumber();

					this.Gate_List_Box.Rows[GateIndex].Cells["Target_Col"].Value = lpReadScript.GetAsNumber();

					this.Gate_List_Box.Rows[GateIndex].Cells["Direction_Col"].Value = lpReadScript.GetAsNumber();

					this.Gate_List_Box.Rows[GateIndex].Cells["MinLevel_Col"].Value = lpReadScript.GetAsNumber();

					this.Gate_List_Box.Rows[GateIndex].Cells["MaxLevel_Col"].Value = lpReadScript.GetAsNumber();

					this.Gate_List_Box.Rows[GateIndex].Cells["MinReset_Col"].Value = lpReadScript.GetAsNumber();

					this.Gate_List_Box.Rows[GateIndex].Cells["MaxReset_Col"].Value = lpReadScript.GetAsNumber();

					this.Gate_List_Box.Rows[GateIndex].Cells["AccountLevel_Col"].Value = lpReadScript.GetAsNumber();
				}
			}
			catch
			{
				Console.WriteLine("Error 2: {0}", lpReadScript.GetLastError());

				return false;
			}

			return true;
		}

		private void SaveGatetxt_Click(object sender, EventArgs e)
		{
			this.SaveGateFile.FileName = "Gate.txt";

			this.SaveGateFile.Filter = "Text files (*.txt)|*.txt";

			if (this.SaveGateFile.ShowDialog() == DialogResult.OK)
			{
				using (Stream fs = new FileStream(this.SaveGateFile.FileName, FileMode.Create, FileAccess.Write))
				{
					using (StreamWriter sw = new StreamWriter(fs))
					{
						sw.WriteLine("//Index	Flag	Map	X	Y	TX	TY	Target	Dir	MinLvl	MaxLvl	MinRes	MaxRes	AccountLvl");

						foreach (DataGridViewRow row in this.Gate_List_Box.Rows)
						{
							for (int j = 1; j < this.Gate_List_Box.Columns.Count; j++)
							{
								if (Convert.ToInt32(row.Cells[j].Value) != 0)
								{
									goto SAVE;
								}
							}

							continue;

						SAVE:

							sw.WriteLine(@"{0}	{1}	{2}	{3}	{4}	{5}	{6}	{7}	{8}	{9}	{10}	{11}	{12}	{13}",
								Convert.ToInt32(row.Cells["Index_Col"].Value),
								Convert.ToInt32(row.Cells["Flag_Col"].Value),
								Convert.ToInt32(row.Cells["Map_Col"].Value),
								Convert.ToInt32(row.Cells["StartX_Col"].Value),
								Convert.ToInt32(row.Cells["StartY_Col"].Value),
								Convert.ToInt32(row.Cells["EndX_Col"].Value),
								Convert.ToInt32(row.Cells["EndY_Col"].Value),
								Convert.ToInt32(row.Cells["Target_Col"].Value),
								Convert.ToInt32(row.Cells["Direction_Col"].Value),
								Convert.ToInt32(row.Cells["MinLevel_Col"].Value),
								Convert.ToInt32(row.Cells["MaxLevel_Col"].Value),
								Convert.ToInt32(row.Cells["MinReset_Col"].Value),
								Convert.ToInt32(row.Cells["MaxReset_Col"].Value),
								Convert.ToInt32(row.Cells["AccountLevel_Col"].Value));
						}

						sw.WriteLine("end");

						Console.WriteLine("[GateManager] {0} saved", this.SaveGateFile.FileName);
					}
				}
			}
		}

		private bool ReadGateBMD(string path)
		{
			try
			{
				byte[] buffer = File.ReadAllBytes(path);

				if (buffer.Length != GateManager.MAX_GATES * GateManager.GATE_BMD_SIZE)
				{
					Console.WriteLine("[GateManager] Gate file size is not correct");

					return false;
				}

				byte[] subBuffer = new byte[GateManager.GATE_BMD_SIZE];

				for (int i = 0; i < GateManager.MAX_GATES; i++)
				{
					Array.Copy(buffer, i * GateManager.GATE_BMD_SIZE, subBuffer, 0, GateManager.GATE_BMD_SIZE);

					Array.Copy(BMDFileOpen.BuxConvert(subBuffer, GateManager.GATE_BMD_SIZE), 0, buffer, i * GateManager.GATE_BMD_SIZE, GateManager.GATE_BMD_SIZE);

					this.Gate_List_Box.Rows[i].Cells["Flag_Col"].Value = Globals.GetRealByteValue(buffer[GateManager.GATE_BMD_SIZE * i + 0]);
					this.Gate_List_Box.Rows[i].Cells["Map_Col"].Value = Globals.GetRealByteValue(buffer[GateManager.GATE_BMD_SIZE * i + 1]);
					this.Gate_List_Box.Rows[i].Cells["StartX_Col"].Value = Globals.GetRealByteValue(buffer[GateManager.GATE_BMD_SIZE * i + 2]);
					this.Gate_List_Box.Rows[i].Cells["StartY_Col"].Value = Globals.GetRealByteValue(buffer[GateManager.GATE_BMD_SIZE * i + 3]);
					this.Gate_List_Box.Rows[i].Cells["EndX_Col"].Value = Globals.GetRealByteValue(buffer[GateManager.GATE_BMD_SIZE * i + 4]);
					this.Gate_List_Box.Rows[i].Cells["EndY_Col"].Value = Globals.GetRealByteValue(buffer[GateManager.GATE_BMD_SIZE * i + 5]);
					this.Gate_List_Box.Rows[i].Cells["Target_Col"].Value = Globals.GetRealByteValue(buffer[GateManager.GATE_BMD_SIZE * i + 6]);
					this.Gate_List_Box.Rows[i].Cells["Direction_Col"].Value = Globals.GetRealByteValue(buffer[GateManager.GATE_BMD_SIZE * i + 7]);
					this.Gate_List_Box.Rows[i].Cells["MinLevel_Col"].Value = Globals.GetRealByteValue(buffer[GateManager.GATE_BMD_SIZE * i + 8]);
				}
			}
			catch
			{
				Console.WriteLine("[GateManager] Gate file is not correct. Please, use another file");

				return false;
			}

			return true;
		}

		private void SaveGatebmd_Click(object sender, EventArgs e)
		{
			this.SaveGateFile.FileName = "Gate.bmd";

			this.SaveGateFile.Filter = "BMD files (*.bmd)|*.bmd";

			if (this.SaveGateFile.ShowDialog() == DialogResult.OK)
			{
				byte[] buffer = new byte[GateManager.MAX_GATES * GateManager.GATE_BMD_SIZE];

				byte[] subBuffer = new byte[GateManager.GATE_BMD_SIZE];

				for (int i = 0; i < GateManager.MAX_GATES; i++)
				{
					subBuffer[0] = Globals.GetMaxByteValue(Convert.ToInt32(this.Gate_List_Box.Rows[i].Cells["Flag_Col"].Value));
					subBuffer[1] = Globals.GetMaxByteValue(Convert.ToInt32(this.Gate_List_Box.Rows[i].Cells["Map_Col"].Value));
					subBuffer[2] = Globals.GetMaxByteValue(Convert.ToInt32(this.Gate_List_Box.Rows[i].Cells["StartX_Col"].Value));
					subBuffer[3] = Globals.GetMaxByteValue(Convert.ToInt32(this.Gate_List_Box.Rows[i].Cells["StartY_Col"].Value));
					subBuffer[4] = Globals.GetMaxByteValue(Convert.ToInt32(this.Gate_List_Box.Rows[i].Cells["EndX_Col"].Value));
					subBuffer[5] = Globals.GetMaxByteValue(Convert.ToInt32(this.Gate_List_Box.Rows[i].Cells["EndY_Col"].Value));
					subBuffer[6] = Globals.GetMaxByteValue(Convert.ToInt32(this.Gate_List_Box.Rows[i].Cells["Target_Col"].Value));
					subBuffer[7] = Globals.GetMaxByteValue(Convert.ToInt32(this.Gate_List_Box.Rows[i].Cells["Direction_Col"].Value));
					subBuffer[8] = Globals.GetMaxByteValue(Convert.ToInt32(this.Gate_List_Box.Rows[i].Cells["MinLevel_Col"].Value));

					Array.Copy(BMDFileOpen.BuxConvert(subBuffer, GateManager.GATE_BMD_SIZE), 0, buffer, i * GateManager.GATE_BMD_SIZE, GateManager.GATE_BMD_SIZE);
				}

				File.WriteAllBytes(this.SaveGateFile.FileName, buffer);

				Console.WriteLine("[GateManager] {0} saved", this.SaveGateFile.FileName);
			}
		}

		private void Gate_List_Box_CellValidating(object sender, DataGridViewCellValidatingEventArgs e)
		{
			int i;

			if (!int.TryParse(Convert.ToString(e.FormattedValue), out i))
			{
				e.Cancel = true;

				MessageBox.Show("Only numbers allowed");
			}
		}
	}
}