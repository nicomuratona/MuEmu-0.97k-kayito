using kayito_Editor.Source;
#if MYSQL
using MySql.Data.MySqlClient;
#else
using System.Data.OleDb;
#endif
using System;
using System.Data;
using System.Windows.Forms;

namespace kayito_Editor.Forms
{
	public partial class Warehouse : Form
	{
		private string AccountName;

		private ItemEditor m_ItemEditor = new ItemEditor();

		private ItemBoxPanel m_ItemBoxPanel = new ItemBoxPanel(8, 15);

		public Warehouse(string account)
		{
			this.AccountName = account;

			InitializeComponent();
		}

		private void Warehouse_Load(object sender, EventArgs e)
		{
			this.CreateItemEditorForm();

			this.CreateWarehouseItemsForm();

			this.m_ItemBoxPanel.Editor = this.m_ItemEditor;

			this.User_Box.Text = this.AccountName;

			this.LoadWarehouse();
		}

		private void CreateItemEditorForm()
		{
			this.m_ItemEditor.TopLevel = false;

			this.m_ItemEditor.FormBorderStyle = FormBorderStyle.None;

			this.m_ItemEditor.Dock = DockStyle.Fill;

			this.Item_Editor_Panel.Controls.Add(this.m_ItemEditor);

			this.Item_Editor_Panel.Tag = this.m_ItemEditor;

			this.Item_Editor_Panel.Visible = true;

			this.m_ItemEditor.BringToFront();

			this.m_ItemEditor.Show();
		}

		private void CreateWarehouseItemsForm()
		{
			this.m_ItemBoxPanel.TopLevel = false;

			this.m_ItemBoxPanel.FormBorderStyle = FormBorderStyle.None;

			this.m_ItemBoxPanel.Dock = DockStyle.Fill;

			this.Warehouse_Items_Panel.Controls.Add(this.m_ItemBoxPanel);

			this.Warehouse_Items_Panel.Tag = this.m_ItemBoxPanel;

			this.Warehouse_Items_Panel.Visible = true;

			this.m_ItemBoxPanel.BringToFront();

			this.m_ItemBoxPanel.Show();
		}

		private void LoadWarehouse()
		{
			if (Import.Mu_Connection.State != ConnectionState.Open)
			{
				return;
			}

			this.m_ItemBoxPanel.ClearItems();

			string query = null;

			try
			{
				query = $"SELECT Items, Money, pw FROM warehouse WHERE AccountID = '{this.AccountName}'";

			#if MYSQL
				MySqlDataReader reader = new MySqlCommand(query, Import.Mu_Connection).ExecuteReader();
			#else
				OleDbDataReader reader = new OleDbCommand(query, Import.Mu_Connection).ExecuteReader();
			#endif

				if (reader.Read())
				{
					byte[] warehouse = new byte[CItem.ITEM_SIZE * ItemManager.WAREHOUSE_SIZE];

					byte[] item = new byte[CItem.ITEM_SIZE];

					reader.GetBytes(0, 0, warehouse, 0, CItem.ITEM_SIZE * ItemManager.WAREHOUSE_SIZE);

					for (int i = 0; i < ItemManager.WAREHOUSE_SIZE; i++)
					{
						Buffer.BlockCopy(warehouse, (i * CItem.ITEM_SIZE), item, 0, CItem.ITEM_SIZE);

						CItem newitem = new CItem();

						newitem.Convert(item);

						this.m_ItemBoxPanel.InsertItem(newitem, i);
					}

					this.Wareouse_Zen.Value = reader.GetInt32(1);

					this.Warehouse_Pass.Value = reader.GetInt16(2);

					reader.Close();
				}
				else
				{
					reader.Close();

					query = $"INSERT INTO warehouse (AccountID, Items, Money, pw) VALUES('{this.AccountName}', 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, 0, 0)";

					if (MuOnline.Mu_ExecuteSQL(query))
					{
						MessageBox.Show($"Warehouse created.", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
					}
					else
					{
						MessageBox.Show($"Error: Warehouse creation failed.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
					}

					for (int i = 0; i < ItemManager.WAREHOUSE_SIZE; i++)
					{
						this.m_ItemBoxPanel.InsertItem(new CItem(), i);
					}

					this.Wareouse_Zen.Value = 0;

					this.Warehouse_Pass.Value = 0;
				}
			}
			catch (Exception exception)
			{
				MessageBox.Show($"[SQL] {query}\n[Error] {exception.Message}\n[Source] {exception.Source}\n[Trace] {exception.StackTrace}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
			}
		}

		private void LoadExtWarehouse()
		{
			if (Import.Mu_Connection.State != ConnectionState.Open)
			{
				return;
			}

			this.m_ItemBoxPanel.ClearItems();

			string query = null;

			try
			{
				query = $"SELECT Items, Money FROM ExtWarehouse WHERE AccountID = '{this.AccountName}' AND Number = {this.Warehouse_Number.Value}";

#if MYSQL
				MySqlDataReader reader = new MySqlCommand(query, Import.Mu_Connection).ExecuteReader();
#else
				OleDbDataReader reader = new OleDbCommand(query, Import.Mu_Connection).ExecuteReader();
#endif

				if (reader.Read())
				{
					byte[] warehouse = new byte[CItem.ITEM_SIZE * ItemManager.WAREHOUSE_SIZE];

					byte[] item = new byte[CItem.ITEM_SIZE];

					reader.GetBytes(0, 0, warehouse, 0, CItem.ITEM_SIZE * ItemManager.WAREHOUSE_SIZE);

					for (int i = 0; i < ItemManager.WAREHOUSE_SIZE; i++)
					{
						Buffer.BlockCopy(warehouse, (i * CItem.ITEM_SIZE), item, 0, CItem.ITEM_SIZE);

						CItem newitem = new CItem();

						newitem.Convert(item);

						this.m_ItemBoxPanel.InsertItem(newitem, i);
					}

					this.Wareouse_Zen.Value = reader.GetInt32(1);

					reader.Close();
				}
				else
				{
					reader.Close();

					query = $"INSERT INTO ExtWarehouse (AccountID, Items, Money, Number) VALUES('{this.AccountName}', 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, 0, {this.Warehouse_Number.Value})";

					if (MuOnline.Mu_ExecuteSQL(query))
					{
						MessageBox.Show($"Extended Warehouse Number {this.Warehouse_Number.Value} created.", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
					}
					else
					{
						MessageBox.Show($"Error: Extended Warehouse Number {this.Warehouse_Number.Value} creation failed.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
					}

					for (int i = 0; i < ItemManager.WAREHOUSE_SIZE; i++)
					{
						this.m_ItemBoxPanel.InsertItem(new CItem(), i);
					}

					this.Wareouse_Zen.Value = 0;
				}
			}
			catch (Exception exception)
			{
				MessageBox.Show($"[SQL] {query}\n[Error] {exception.Message}\n[Source] {exception.Source}\n[Trace] {exception.StackTrace}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
			}
		}

		private void Btn_Ware_Save_Click(object sender, EventArgs e)
		{
			if (Import.Mu_Connection.State != ConnectionState.Open)
			{
				return;
			}

			string query = null;

			try
			{
				if (this.Warehouse_Number.Value != 0)
				{
					query = $"UPDATE ExtWarehouse SET Items = 0x{this.m_ItemBoxPanel.GetItemsToHex()}, Money = {this.Wareouse_Zen.Value} WHERE AccountID = '{this.AccountName}' AND Number = {this.Warehouse_Number.Value}";
				}
				else
				{
					query = $"UPDATE warehouse SET Items = 0x{this.m_ItemBoxPanel.GetItemsToHex()}, Money = {this.Wareouse_Zen.Value}, pw = {this.Warehouse_Pass.Value} WHERE AccountID = '{this.AccountName}'";
				}

				if (MuOnline.Mu_ExecuteSQL(query))
				{
					MessageBox.Show($"Warehouse updated.", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
				}
				else
				{
					MessageBox.Show($"Error: warehouse update failed.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				}
			}
			catch (Exception exception)
			{
				MessageBox.Show($"[SQL] {query}\n[Error] {exception.Message}\n[Source] {exception.Source}\n[Trace] {exception.StackTrace}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
			}
		}

		private void Btn_Ware_Reset_Click(object sender, EventArgs e)
		{
			DialogResult result = MessageBox.Show($"Do you want to RESET '{this.AccountName}' warehouse?\n\nAfter reseting, you'll have to press SAVE to apply the changes.", "RESTORE WAREHOUSE", MessageBoxButtons.YesNo);

			if (result == DialogResult.Yes)
			{
				this.m_ItemBoxPanel.ClearItems();

				this.Wareouse_Zen.Value = 0;

				this.Warehouse_Pass.Value = 0;

				this.m_ItemBoxPanel.ReloadForm();
			}
		}

		private void Btn_Close_Click(object sender, EventArgs e)
		{
			this.Close();
		}

		private void Btn_Close_MouseDown(object sender, MouseEventArgs e)
		{
			this.Btn_Close.BackgroundImage = Properties.Resources.Exit_p;
		}

		private void Btn_Close_MouseUp(object sender, MouseEventArgs e)
		{
			this.Btn_Close.BackgroundImage = Properties.Resources.Exit_n;
		}

		private void Warehouse_Number_ValueChanged(object sender, EventArgs e)
		{
			if (((NumericUpDown)sender).Value != 0)
			{
				this.LoadExtWarehouse();
			}
			else
			{
				this.LoadWarehouse();
			}
		}
	}
}