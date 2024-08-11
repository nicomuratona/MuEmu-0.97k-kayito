using System;
using System.IO;
using System.Text;
using System.Windows.Forms;
using kayitoTools.Source_Files;
using static kayitoTools.Source_Files.ReadScript;

namespace kayitoTools.Forms
{
	public partial class FilterEditor : Form
	{
		private string filterPath;

		public FilterEditor()
		{
			InitializeComponent();
		}

		private void FilterEditor_Load(object sender, EventArgs e)
		{
			Globals.SetDoubleBuffer(this.Filter_List_Box, true);

			this.Filter_List_Box.RowCount = FilterManager.MAX_ROWS_FILTER;

			Console.WriteLine("[FilterManager] Filter Editor Opened");
		}

		private void InitializeGridList()
		{
			this.Filter_List_Box.Visible = false;

			foreach (DataGridViewRow row in this.Filter_List_Box.Rows)
			{
				row.Cells["Row_Col"].Value = row.Index;

				row.Cells["Text_Col"].Value = null;
			}
		}

		private void Load_Filtertxt_Click(object sender, EventArgs e)
		{
			StartBrowsing("Filter.txt", (int)FilterManager.FilterType.TYPE_FILTER);
		}

		private void Load_Filterbmd_Click(object sender, EventArgs e)
		{
			StartBrowsing("Filter.bmd", (int)FilterManager.FilterType.TYPE_FILTER);
		}

		private void Load_FilterNamebmd_Click(object sender, EventArgs e)
		{
			StartBrowsing("FilterName.bmd", (int)FilterManager.FilterType.TYPE_FILTER_NAME);
		}

		private void StartBrowsing(string file, int type)
		{
			string fileType = file.Substring(file.Length - 3);

			this.OpenFilterFile.FileName = file;

			this.OpenFilterFile.Filter = fileType.ToUpper() + " files (*." + fileType + ")|*." + fileType;

			if (this.OpenFilterFile.ShowDialog() == DialogResult.OK)
			{
				this.filterPath = this.OpenFilterFile.FileName;

				this.InitializeGridList();

				bool result = false;

				if (fileType == "txt")
				{
					result = this.ReadFilterTXT(this.filterPath);
				}
				else if (fileType == "bmd")
				{
					if (type == (int)FilterManager.FilterType.TYPE_FILTER)
					{
						result = this.ReadFilterBMD(this.filterPath);
					}
					else if (type == (int)FilterManager.FilterType.TYPE_FILTER_NAME)
					{
						result = this.ReadFilterNameBMD(this.filterPath);
					}
				}

				if (result == true)
				{
					this.Filter_List_Box.Visible = true;

					this.Save_Filter_Menu_Btn.Enabled = true;

					this.Search_Text_Panel.Enabled = true;

					Console.WriteLine("[FilterManager] {0} loaded", file);
				}
				else
				{
					this.Filter_List_Box.Visible = false;

					this.Save_Filter_Menu_Btn.Enabled = false;

					this.Search_Text_Panel.Enabled = false;
				}
			}
		}

		private bool ReadFilterTXT(string path)
		{
			ReadScript lpReadScript = new ReadScript();

			if (lpReadScript.SetBuffer(path) == false)
			{
				Console.WriteLine("Error 1: {0}", lpReadScript.GetLastError());

				return false;
			}

			int index = 0;

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

					this.Filter_List_Box.Rows[index++].Cells["Text_Col"].Value = lpReadScript.GetString();
				}
			}
			catch
			{
				Console.WriteLine("Error 2: {0}", lpReadScript.GetLastError());

				return false;
			}

			return true;
		}

		private void SaveFiltertxt_Click(object sender, EventArgs e)
		{
			this.SaveFilterFile.FileName = "Filter.txt";

			this.SaveFilterFile.Filter = "Text files (*.txt)|*.txt";

			if (this.SaveFilterFile.ShowDialog() == DialogResult.OK)
			{
				using (Stream fs = new FileStream(this.SaveFilterFile.FileName, FileMode.Create, FileAccess.Write))
				{
					using (StreamWriter sw = new StreamWriter(fs))
					{
						sw.WriteLine("//Filter");

						foreach (DataGridViewRow row in this.Filter_List_Box.Rows)
						{
							if (row.Cells["Text_Col"].Value == null)
							{
								continue;
							}

							if (String.IsNullOrEmpty(row.Cells["Text_Col"].Value.ToString()))
							{
								continue;
							}

							sw.WriteLine(@"""{0}""", Globals.GetMaxStringLength(row.Cells["Text_Col"].Value.ToString(), 16));
						}

						sw.WriteLine("end");

						Console.WriteLine("[FilterManager] {0} saved", this.SaveFilterFile.FileName);
					}
				}
			}
		}

		private bool ReadFilterBMD(string path)
		{
			try
			{
				byte[] buffer = File.ReadAllBytes(path);

				if (buffer.Length != FilterManager.MAX_ROWS_FILTER * FilterManager.MAX_FILTER_TEXT_SIZE + 4)
				{
					Console.WriteLine("[FilterManager] Filter file size is not correct");

					return false;
				}

				for (int i = 0; i < FilterManager.MAX_ROWS_FILTER; i++)
				{
					byte[] subBuffer = BMDFileOpen.SubArray(buffer, (i * FilterManager.MAX_FILTER_TEXT_SIZE), FilterManager.MAX_FILTER_TEXT_SIZE);

					this.Filter_List_Box.Rows[i].Cells["Text_Col"].Value = Encoding.Default.GetString(BMDFileOpen.BuxConvert(subBuffer, FilterManager.MAX_FILTER_TEXT_SIZE)).Trim('\0');
				}
			}
			catch
			{
				Console.WriteLine("[FilterManager] Filter file is not correct. Please, use another file");

				return false;
			}

			return true;
		}

		private void SaveFilterbmd_Click(object sender, EventArgs e)
		{
			this.SaveFilterFile.FileName = "Filter.bmd";

			this.SaveFilterFile.Filter = "BMD files (*.bmd)|*.bmd";

			if (this.SaveFilterFile.ShowDialog() == DialogResult.OK)
			{
				byte[] buffer = new byte[FilterManager.MAX_ROWS_FILTER * FilterManager.MAX_FILTER_TEXT_SIZE + 4];

				foreach (DataGridViewRow row in this.Filter_List_Box.Rows)
				{
					string TextValue = string.Empty;

					if (row.Cells["Text_Col"].Value != null && !String.IsNullOrEmpty(row.Cells["Text_Col"].Value.ToString()))
					{
						TextValue = Globals.GetMaxStringLength(row.Cells["Text_Col"].Value.ToString(), FilterManager.MAX_FILTER_TEXT_SIZE);
					}

					byte[] TextBytes = BMDFileOpen.StringToByteArray(TextValue, FilterManager.MAX_FILTER_TEXT_SIZE);

					TextBytes = BMDFileOpen.BuxConvert(TextBytes, FilterManager.MAX_FILTER_TEXT_SIZE);

					Array.Copy(TextBytes, 0, buffer, row.Index * FilterManager.MAX_FILTER_TEXT_SIZE, FilterManager.MAX_FILTER_TEXT_SIZE);
				}

				Array.Copy(BitConverter.GetBytes(BMDFileOpen.GenerateCheckSum(buffer, FilterManager.MAX_ROWS_FILTER * FilterManager.MAX_FILTER_TEXT_SIZE, 0x3E7D)), 0, buffer, FilterManager.MAX_ROWS_FILTER * FilterManager.MAX_FILTER_TEXT_SIZE, 4);
				
				File.WriteAllBytes(this.SaveFilterFile.FileName, buffer);

				Console.WriteLine("[FilterManager] {0} saved", this.SaveFilterFile.FileName);
			}
		}

		private bool ReadFilterNameBMD(string path)
		{
			try
			{
				byte[] buffer = File.ReadAllBytes(path);

				if (buffer.Length != FilterManager.MAX_ROWS_FILTERNAME * FilterManager.MAX_FILTER_TEXT_SIZE + 4)
				{
					Console.WriteLine("[FilterManager] Filter file size is not correct");

					return false;
				}

				for (int i = 0; i < FilterManager.MAX_ROWS_FILTERNAME; i++)
				{
					byte[] subBuffer = BMDFileOpen.SubArray(buffer, (i * FilterManager.MAX_FILTER_TEXT_SIZE), FilterManager.MAX_FILTER_TEXT_SIZE);

					this.Filter_List_Box.Rows[i].Cells["Text_Col"].Value = Encoding.Default.GetString(BMDFileOpen.BuxConvert(subBuffer, FilterManager.MAX_FILTER_TEXT_SIZE)).Trim('\0');
				}
			}
			catch
			{
				Console.WriteLine("[FilterManager] Filter file is not correct. Please, use another file");

				return false;
			}

			return true;
		}

		private void SaveFilterNamebmd_Click(object sender, EventArgs e)
		{
			this.SaveFilterFile.FileName = "FilterName.bmd";

			this.SaveFilterFile.Filter = "BMD files (*.bmd)|*.bmd";

			if (this.SaveFilterFile.ShowDialog() == DialogResult.OK)
			{
				byte[] buffer = new byte[FilterManager.MAX_ROWS_FILTERNAME * FilterManager.MAX_FILTER_TEXT_SIZE + 4];

				foreach (DataGridViewRow row in this.Filter_List_Box.Rows)
				{
					string TextValue = string.Empty;

					if (row.Cells["Text_Col"].Value != null && !String.IsNullOrEmpty(row.Cells["Text_Col"].Value.ToString()))
					{
						TextValue = Globals.GetMaxStringLength(row.Cells["Text_Col"].Value.ToString(), FilterManager.MAX_FILTER_TEXT_SIZE);
					}

					byte[] TextBytes = BMDFileOpen.StringToByteArray(TextValue, FilterManager.MAX_FILTER_TEXT_SIZE);

					TextBytes = BMDFileOpen.BuxConvert(TextBytes, FilterManager.MAX_FILTER_TEXT_SIZE);

					Array.Copy(TextBytes, 0, buffer, row.Index * FilterManager.MAX_FILTER_TEXT_SIZE, FilterManager.MAX_FILTER_TEXT_SIZE);
				}

				Array.Copy(BitConverter.GetBytes(BMDFileOpen.GenerateCheckSum(buffer, FilterManager.MAX_ROWS_FILTERNAME * FilterManager.MAX_FILTER_TEXT_SIZE, 0x2BC1)), 0, buffer, FilterManager.MAX_ROWS_FILTERNAME * FilterManager.MAX_FILTER_TEXT_SIZE, 4);
				
				File.WriteAllBytes(this.SaveFilterFile.FileName, buffer);

				Console.WriteLine("[FilterManager] {0} saved", this.SaveFilterFile.FileName);
			}
		}

		private void Goto_Text_Box_ValueChanged(object sender, EventArgs e)
		{
			this.Filter_List_Box.FirstDisplayedScrollingRowIndex = (int)this.Goto_Text_Box.Value;
		}

		private void Filter_List_Box_Scroll(object sender, ScrollEventArgs e)
		{
			this.Goto_Text_Box.Value = this.Filter_List_Box.FirstDisplayedScrollingRowIndex;
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

				for (i = this.Filter_List_Box.FirstDisplayedScrollingRowIndex; i < this.Filter_List_Box.RowCount; i++)
				{
					if (i == this.Filter_List_Box.FirstDisplayedScrollingRowIndex)
					{
						continue;
					}

					if (this.Filter_List_Box.Rows[i].Cells["Text_Col"].Value == null)
					{
						continue;
					}

					if (this.Filter_List_Box.Rows[i].Cells["Text_Col"].Value.ToString().ToLower().Contains(searchValue))
					{
						this.Goto_Text_Box.Value = i;

						break;
					}
				}

				if (i == this.Filter_List_Box.RowCount)
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