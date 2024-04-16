using kayito_Editor.Source;
using System;
using System.Data;
using System.Data.OleDb;
using System.Windows.Forms;

namespace kayito_Editor.Forms
{
	public partial class CharacterManager : Form
	{
		public CharacterManager(string id)
		{
			InitializeComponent();

			this.Class_Box.DataSource = new BindingSource(Import.CreateCharList, null);
			this.Class_Box.DisplayMember = "Value";
			this.Class_Box.ValueMember = "Key";

			if (Import.Me_Connection.State != ConnectionState.Open)
			{
				return;
			}

			string query = null;

			try
			{
				query = $"SELECT memb___id FROM MEMB_INFO";

				OleDbDataReader reader = new OleDbCommand(query, Import.Me_Connection).ExecuteReader();

				object value = null;

				while (reader.Read())
				{
					value = reader.GetValue(0);

					if (this.Account_List.Items.Contains(value))
					{
						continue;
					}

					this.Account_List.Items.Add(value);
				}

				reader.Close();

				this.Account_List.Text = id;
			}
			catch (Exception exception)
			{
				MessageBox.Show($"[SQL] {query}\n[Error] {exception.Message}\n[Source] {exception.Source}\n[Trace] {exception.StackTrace}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
			}
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
				query = $"SELECT memb___id FROM MEMB_INFO WHERE memb___id = '{this.Account_List.Text.Trim()}'";

				OleDbDataReader reader = new OleDbCommand(query, Import.Me_Connection).ExecuteReader();

				if (reader.Read())
				{
					reader.Close();

					query = $"WZ_CreateCharacter";

					OleDbCommand cmd = new OleDbCommand(query, Import.Mu_Connection);

					cmd.CommandType = CommandType.StoredProcedure;

					cmd.Parameters.AddWithValue("@AccountID", this.Account_List.Text);

					cmd.Parameters.AddWithValue("@CharName", this.Name_Box.Text);

					cmd.Parameters.AddWithValue("@CharClass", this.Class_Box.SelectedValue);

					int result = -1;

					using (reader = cmd.ExecuteReader())
					{
						if (reader.Read())
						{
							result = reader.GetInt32(0);
						}
					}

					reader.Close();

					if (result == 1)
					{
						MessageBox.Show($"Character created", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
					}
					else if (result == 0)
					{
						MessageBox.Show($"Character already exists", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
					}
					else if (result == 2)
					{
						MessageBox.Show($"You can't create more characters in this account", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
					}
					else
					{
						MessageBox.Show($"Can't create character", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
					}
				}
				else
				{
					MessageBox.Show($"Error: The account '{this.Account_List.Text.Trim()}' doesn't exist.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

					reader.Close();
				}
			}
			catch (Exception exception)
			{
				MessageBox.Show($"[SQL] {query}\n[Error] {exception.Message}\n[Source] {exception.Source}\n[Trace] {exception.StackTrace}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
			}
		}

		private void Btn_Clear_Click(object sender, EventArgs e)
		{
			this.Account_List.Text = "";

			this.Name_Box.Text = "";
		}

		private void Btn_Close_Click(object sender, EventArgs e)
		{
			this.Dispose();
		}
	}
}