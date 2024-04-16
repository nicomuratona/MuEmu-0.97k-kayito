using kayito_Editor.Source;
using System;
using System.Data;
using System.Data.OleDb;
using System.Windows.Forms;

namespace kayito_Editor.Forms
{
	public partial class Inventory : Form
	{
		private string AccountName;

		private string CharacterName;

		private ItemEditor m_ItemEditor = new ItemEditor();

		private ItemEquipPanel m_ItemEquipPanel = new ItemEquipPanel();

		private ItemBoxPanel m_ItemBoxPanel = new ItemBoxPanel(8, 8);

		public Inventory(string account, string character)
		{
			this.AccountName = account;

			this.CharacterName = character;

			InitializeComponent();
		}

		private void Inventory_Load(object sender, EventArgs e)
		{
			this.CreateItemEditorForm();

			this.CreateCharacterEquipmentForm();

			this.CreateCharacterInventoryForm();

			this.m_ItemBoxPanel.Editor = this.m_ItemEditor;

			this.m_ItemEquipPanel.Editor = this.m_ItemEditor;

			this.User_Box.Text = this.AccountName;

			this.Character_Box.Text = this.CharacterName;

			this.LoadInventory();
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

		private void CreateCharacterEquipmentForm()
		{
			this.m_ItemEquipPanel.TopLevel = false;

			this.m_ItemEquipPanel.FormBorderStyle = FormBorderStyle.None;

			this.m_ItemEquipPanel.Dock = DockStyle.Fill;

			this.Character_Equipment_Panel.Controls.Add(this.m_ItemEquipPanel);

			this.Character_Equipment_Panel.Tag = this.m_ItemEquipPanel;

			this.Character_Equipment_Panel.Visible = true;

			this.m_ItemEquipPanel.BringToFront();

			this.m_ItemEquipPanel.Show();
		}

		private void CreateCharacterInventoryForm()
		{
			this.m_ItemBoxPanel.TopLevel = false;

			this.m_ItemBoxPanel.FormBorderStyle = FormBorderStyle.None;

			this.m_ItemBoxPanel.Dock = DockStyle.Fill;

			this.Character_Inventory_Panel.Controls.Add(this.m_ItemBoxPanel);

			this.Character_Inventory_Panel.Tag = this.m_ItemBoxPanel;

			this.Character_Inventory_Panel.Visible = true;

			this.m_ItemBoxPanel.BringToFront();

			this.m_ItemBoxPanel.Show();
		}

		private void LoadInventory()
		{
			if (Import.Mu_Connection.State != ConnectionState.Open)
			{
				return;
			}

			string query = null;

			try
			{
				query = $"SELECT Inventory, Money FROM Character WHERE AccountID = '{this.AccountName}' AND Name = '{this.CharacterName}'";

				OleDbDataReader reader = new OleDbCommand(query, Import.Mu_Connection).ExecuteReader();

				if (reader.Read())
				{
					byte[] inventory = new byte[CItem.ITEM_SIZE * ItemManager.INVENTORY_FULL_SIZE];

					byte[] item = new byte[CItem.ITEM_SIZE];

					reader.GetBytes(0, 0, inventory, 0, CItem.ITEM_SIZE * ItemManager.INVENTORY_FULL_SIZE);

					for (int i = 0; i < ItemManager.INVENTORY_FULL_SIZE; i++)
					{
						Buffer.BlockCopy(inventory, (i * CItem.ITEM_SIZE), item, 0, CItem.ITEM_SIZE);

						CItem newitem = new CItem();

						newitem.Convert(item);

						if (i < ItemManager.INVENTORY_WEAR_SIZE)
						{
							this.m_ItemEquipPanel.InsertItem(newitem, i);
						}
						else
						{
							this.m_ItemBoxPanel.InsertItem(newitem, i - ItemManager.INVENTORY_WEAR_SIZE);
						}
					}

					this.Character_Zen.Value = reader.GetInt32(1);

					reader.Close();
				}
				else
				{
					reader.Close();

					MessageBox.Show($"Error: Cannot get {this.CharacterName} inventory.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				}
			}
			catch (Exception exception)
			{
				MessageBox.Show($"[SQL] {query}\n[Error] {exception.Message}\n[Source] {exception.Source}\n[Trace] {exception.StackTrace}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
			}
		}

		private void Btn_Inventory_Save_Click(object sender, EventArgs e)
		{
			if (Import.Mu_Connection.State != ConnectionState.Open)
			{
				return;
			}

			string query = null;

			try
			{
				query = $"UPDATE Character SET Inventory = 0x{this.m_ItemEquipPanel.GetItemsToHex()}{this.m_ItemBoxPanel.GetItemsToHex()}, Money = {this.Character_Zen.Value} WHERE AccountID = '{this.AccountName}' AND Name = '{this.CharacterName}'";

				if (MuOnline.Mu_ExecuteSQL(query))
				{
					MessageBox.Show($"Inventory updated.", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
				}
				else
				{
					MessageBox.Show($"Error: Inventory update failed.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				}
			}
			catch (Exception exception)
			{
				MessageBox.Show($"[SQL] {query}\n[Error] {exception.Message}\n[Source] {exception.Source}\n[Trace] {exception.StackTrace}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
			}
		}

		private void Btn_Inventory_Reset_Click(object sender, EventArgs e)
		{
			DialogResult result = MessageBox.Show($"Do you want to RESET '{this.CharacterName}' inventory?\n\nAfter reseting, you'll have to press SAVE to apply the changes.", "RESTORE INVENTORY", MessageBoxButtons.YesNo);

			if (result == DialogResult.Yes)
			{
				this.m_ItemEquipPanel.ClearItems();

				this.m_ItemBoxPanel.ClearItems();

				this.Character_Zen.Value = 0;

				this.m_ItemEquipPanel.ReloadForm();

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
	}
}