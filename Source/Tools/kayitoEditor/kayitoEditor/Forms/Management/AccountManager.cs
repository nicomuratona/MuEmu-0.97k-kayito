using kayito_Editor.Source;
using System;
using System.Data;
using System.Data.OleDb;
using System.Windows.Forms;

namespace kayito_Editor.Forms
{
	public partial class AccountManager : Form
	{
		public AccountManager()
		{
			InitializeComponent();
		}

		private void Btn_Create_Click(object sender, EventArgs e)
		{
			if (Import.Me_Connection.State != ConnectionState.Open)
			{
				return;
			}

			string query = null;

			try
			{
				if (!Validator.UserName(this.Account_Box.Text.Trim()))
				{
					MessageBox.Show($"Error: Account names must be between 4-10 characters, and only contain numbers, letters or underscores.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
				}
				else if (!Validator.PassWord(this.Password_Box.Text.Trim()))
				{
					MessageBox.Show($"Error: Passwords must be between 4-10 characters.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
				}
				else if (!Validator.Number(this.Code_Box.Text.Trim()))
				{
					MessageBox.Show($"Error: Code must be 18 numbers only.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
				}
				else if (this.Code_Box.Text.Length < 18)
				{
					MessageBox.Show($"Error: Code must be 18 numbers only.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
				}
				else if (!Validator.UserName(this.Name_Box.Text.Trim()))
				{
					MessageBox.Show($"Error: Account names must be between 4-10 characters, and only contain numbers, letters or underscores.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
				}
				else
				{
					query = $"SELECT memb___id FROM MEMB_INFO WHERE memb___id = '{this.Account_Box.Text.Trim()}'";

					OleDbDataReader reader = new OleDbCommand(query, Import.Me_Connection).ExecuteReader();

					if (reader.Read())
					{
						MessageBox.Show($"Error: The account '{this.Account_Box.Text.Trim()}' already exists.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

						reader.Close();
					}
					else
					{
						reader.Close();

						query = $"INSERT INTO MEMB_INFO(memb___id, memb__pwd, memb_name, sno__numb, bloc_code, AccountLevel) VALUES('{this.Account_Box.Text.Trim()}', '{this.Password_Box.Text.Trim()}', '{this.Name_Box.Text.Trim()}', '{this.Code_Box.Text.Trim()}', 0, 0)";

						if (MuOnline.Me_ExecuteSQL(query))
						{
							MessageBox.Show($"Account created.", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
						}
						else
						{
							MessageBox.Show($"Error: Account creation failed.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
						}
					}
				}
			}
			catch (Exception exception)
			{
				MessageBox.Show($"[SQL] {query}\n[Error] {exception.Message}\n[Source] {exception.Source}\n[Trace] {exception.StackTrace}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
			}
		}

		private void Btn_Clear_Click(object sender, EventArgs e)
		{
			Account_Box.Text = "";
			Password_Box.Text = "";
			Name_Box.Text = "";
			Code_Box.Text = "";
		}

		private void Btn_Close_Click(object sender, EventArgs e)
		{
			this.Dispose();
		}
	}
}