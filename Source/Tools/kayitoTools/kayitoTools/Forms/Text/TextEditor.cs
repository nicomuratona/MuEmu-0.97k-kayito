using System;
using System.IO;
using System.Text;
using System.Windows.Forms;
using kayitoTools.Source_Files;
using static kayitoTools.Source_Files.ReadScript;

namespace kayitoTools.Forms
{
	public partial class TextEditor : Form
	{
		private string textPath;

		public TextEditor()
		{
			InitializeComponent();
		}

		private void TextEditor_Load(object sender, EventArgs e)
		{
			Globals.SetDoubleBuffer(this.Text_List_Box, true);

			this.Text_List_Box.RowCount = TextManager.MAX_ROWS_TEXT;

			Console.WriteLine("[TextManager] Text Editor Opened");
		}

		private void InitializeGridList()
		{
			this.Text_List_Box.Visible = false;

			foreach (DataGridViewRow row in this.Text_List_Box.Rows)
			{
				row.Cells["Row_Col"].Value = row.Index;

				row.Cells["Text_Col"].Value = null;
			}
		}

		private void LoadMessagetxt_Click(object sender, EventArgs e)
		{
			StartBrowsing("message.txt");
		}

		private void LoadTextbmd_Click(object sender, EventArgs e)
		{
			StartBrowsing("text.bmd");
		}

		private void StartBrowsing(string file)
		{
			string fileType = file.Substring(file.Length - 3);

			this.OpenTextFile.FileName = file;

			this.OpenTextFile.Filter = fileType.ToUpper() + " files (*." + fileType + ")|*." + fileType;

			if (this.OpenTextFile.ShowDialog() == DialogResult.OK)
			{
				this.textPath = this.OpenTextFile.FileName;

				this.InitializeGridList();

				bool result = false;

				if (fileType == "txt")
				{
					result = this.ReadMessageTXT(this.textPath);
				}
				else if (fileType == "bmd")
				{
					result = this.ReadTextBMD(this.textPath);
				}

				if (result == true)
				{
					this.Text_List_Box.Visible = true;

					this.Save_Text_Menu_Btn.Enabled = true;

					this.Search_Text_Panel.Enabled = true;

					Console.WriteLine("[TextManager] {0} loaded", file);
				}
				else
				{
					this.Text_List_Box.Visible = false;

					this.Save_Text_Menu_Btn.Enabled = false;

					this.Search_Text_Panel.Enabled = false;
				}
			}
		}

		private bool ReadMessageTXT(string path)
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

					int TextIndex = lpReadScript.GetNumber();

					this.Text_List_Box.Rows[TextIndex].Cells["Text_Col"].Value = lpReadScript.GetAsString();
				}
			}
			catch
			{
				Console.WriteLine("Error 2: {0}", lpReadScript.GetLastError());

				return false;
			}

			return true;
		}

		private void SaveMessagetxt_Click(object sender, EventArgs e)
		{
			this.SaveTextFile.FileName = "message.txt";

			this.SaveTextFile.Filter = "Text files (*.txt)|*.txt";

			if (this.SaveTextFile.ShowDialog() == DialogResult.OK)
			{
				using (Stream fs = new FileStream(this.SaveTextFile.FileName, FileMode.Create, FileAccess.Write))
				{
					using (StreamWriter sw = new StreamWriter(fs))
					{
						sw.WriteLine("//Index	Message");

						foreach (DataGridViewRow row in this.Text_List_Box.Rows)
						{
							if (row.Cells["Text_Col"].Value == null)
							{
								continue;
							}

							if (String.IsNullOrEmpty(row.Cells["Text_Col"].Value.ToString()))
							{
								continue;
							}

							sw.WriteLine(@"{0}	""{1}""",
								Convert.ToInt32(row.Cells["Row_Col"].Value),
								Globals.GetMaxStringLength(row.Cells["Text_Col"].Value.ToString(), TextManager.MAX_TEXT_SIZE));
						}

						sw.WriteLine("end");

						Console.WriteLine("[SkillManager] {0} saved", this.SaveTextFile.FileName);
					}
				}
			}
		}

		private bool ReadTextBMD(string path)
		{
			try
			{
				byte[] buffer = new byte[TextManager.MAX_ROWS_TEXT * TextManager.MAX_TEXT_SIZE];

				byte[] file = File.ReadAllBytes(path);

				Array.Copy(file, buffer, (file.Length > buffer.Length) ? buffer.Length : file.Length);

				buffer = BMDFileOpen.BuxConvert(buffer, buffer.Length);

				for (int i = 0; i < TextManager.MAX_ROWS_TEXT; i++)
				{
					this.Text_List_Box.Rows[i].Cells["Text_Col"].Value = Encoding.Default.GetString(BMDFileOpen.SubArray(buffer, (i * TextManager.MAX_TEXT_SIZE), TextManager.MAX_TEXT_SIZE)).Trim('\0');
				}
			}
			catch
			{
				Console.WriteLine("[TextManager] Text file is not correct. Please, use another file");

				return false;
			}

			return true;
		}

		private void SaveTextbmd_Click(object sender, EventArgs e)
		{
			this.SaveTextFile.FileName = "text.bmd";

			this.SaveTextFile.Filter = "BMD files (*.bmd)|*.bmd";

			if (this.SaveTextFile.ShowDialog() == DialogResult.OK)
			{
				byte[] buffer = new byte[TextManager.MAX_ROWS_TEXT * TextManager.MAX_TEXT_SIZE];

				for (int i = 0; i < TextManager.MAX_ROWS_TEXT; i++)
				{
					if (this.Text_List_Box.Rows[i].Cells["Text_Col"].Value != null && !String.IsNullOrEmpty(this.Text_List_Box.Rows[i].Cells["Text_Col"].Value.ToString()))
					{
						Array.Copy(BMDFileOpen.StringToByteArray(this.Text_List_Box.Rows[i].Cells["Text_Col"].Value.ToString(), TextManager.MAX_TEXT_SIZE), 0, buffer, i * TextManager.MAX_TEXT_SIZE, TextManager.MAX_TEXT_SIZE);
					}
					else
					{
						Array.Copy(BMDFileOpen.StringToByteArray("", TextManager.MAX_TEXT_SIZE), 0, buffer, i * TextManager.MAX_TEXT_SIZE, TextManager.MAX_TEXT_SIZE);
					}
				}

				buffer = BMDFileOpen.BuxConvert(buffer, buffer.Length);

				File.WriteAllBytes(this.SaveTextFile.FileName, buffer);

				Console.WriteLine("[TextManager] {0} saved", this.SaveTextFile.FileName);
			}
		}

		private void Goto_Text_Box_ValueChanged(object sender, EventArgs e)
		{
			this.Text_List_Box.FirstDisplayedScrollingRowIndex = (int)this.Goto_Text_Box.Value;
		}

		private void Text_List_Box_Scroll(object sender, ScrollEventArgs e)
		{
			this.Goto_Text_Box.Value = this.Text_List_Box.FirstDisplayedScrollingRowIndex;
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

				for (i = this.Text_List_Box.FirstDisplayedScrollingRowIndex; i < this.Text_List_Box.RowCount; i++)
				{
					if (i == this.Text_List_Box.FirstDisplayedScrollingRowIndex)
					{
						continue;
					}

					if (this.Text_List_Box.Rows[i].Cells["Text_Col"].Value == null)
					{
						continue;
					}

					if (this.Text_List_Box.Rows[i].Cells["Text_Col"].Value.ToString().ToLower().Contains(searchValue))
					{
						this.Goto_Text_Box.Value = i;

						break;
					}
				}

				if (i == this.Text_List_Box.RowCount)
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