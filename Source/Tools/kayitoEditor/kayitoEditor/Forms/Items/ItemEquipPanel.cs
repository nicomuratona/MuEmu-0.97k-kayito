using kayito_Editor.Source;
using System;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace kayito_Editor.Forms
{
	public partial class ItemEquipPanel : Form
	{
		Rectangle[] positions = new Rectangle[ItemManager.INVENTORY_WEAR_SIZE]
		{
			new Rectangle(3, 59, 45, 70), // Weapon_Sword
			new Rectangle(152, 59, 45, 70), // Weapon_Shield
			new Rectangle(78, 5, 45, 45), // Helm
			new Rectangle(78, 59, 45, 70), // Armor
			new Rectangle(78, 137, 45, 45), // Pants
			new Rectangle(3, 137, 45, 45), // Gloves
			new Rectangle(152, 137, 45, 45), // Boots
			new Rectangle(128, 5, 70, 45), // Wings
			new Rectangle(3, 5, 45, 45), // Pet
			new Rectangle(53, 59, 20, 20), // Pendant
			new Rectangle(53, 137, 20, 20), // Ring1
			new Rectangle(128, 137, 20, 20) // Ring2
		};

		private CItem selectedItem = null;

		public CItem[] m_Items = new CItem[ItemManager.INVENTORY_WEAR_SIZE];

		private ItemEditor editor;

		public ItemEquipPanel()
		{
			InitializeComponent();

			for (int i = 0; i < ItemManager.INVENTORY_WEAR_SIZE; i++)
			{
				this.m_Items[i] = new CItem();

				this.m_Items[i].Clear();
			}
		}

		public ItemEditor Editor
		{
			set
			{
				this.editor = value;
			}
		}

		public void ClearItems()
		{
			for (int i = 0; i < ItemManager.INVENTORY_WEAR_SIZE; i++)
			{
				this.m_Items[i].Clear();
			}

			this.Invalidate();
		}

		public void ClickedOutside()
		{
			this.selectedItem = null;
		}

		public void ReloadForm()
		{
			this.Invalidate();
		}

		private int FindRectangle(int x, int y)
		{
			for (int i = 0; i < ItemManager.INVENTORY_WEAR_SIZE; i++)
			{
				if (this.positions[i].Contains(x, y))
				{
					return i;
				}
			}

			return -1;
		}

        private bool EquipItemCheck(ItemManager.ITEM_TXT_INFO itemInfo, int slot)
		{
			if (itemInfo.Slot == -1)
			{
				return false;
			}

			if (itemInfo.Section == 0 || itemInfo.Section == 1 || itemInfo.Section == 2 || itemInfo.Section == 3)
			{
				if (slot == 0 || slot == 1)
				{
					return true;
				}
				else
				{
					return false;
				}
			}

			if (itemInfo.Slot == 10)
			{
				if (slot == 10 || slot == 11)
				{
					return true;
				}
			}

			if (itemInfo.Slot == slot)
			{
				return true;
			}

			return false;
		}

		public bool InsertItem(CItem item, int slot)
		{
            ItemManager.ITEM_TXT_INFO ItemInfo;

            if (!ItemManager.m_ItemInfo.TryGetValue((ItemManager.MAX_ITEM_TYPE * item.Section) + item.Index, out ItemInfo))
			{
				return false;
			}

			if (this.m_Items[slot].IsEmpty == true)
			{
				bool result = this.EquipItemCheck(ItemInfo, slot);

				if (result)
				{
					this.m_Items[slot] = item;

					return true;
				}
			}

			return false;
		}

		public string GetItemsToHex()
		{
			StringBuilder sb = new StringBuilder();

			for (int slot = 0; slot < ItemManager.INVENTORY_WEAR_SIZE; slot++)
			{
				if (this.m_Items[slot].IsEmpty == false)
				{
					sb.Append(BitConverter.ToString(this.m_Items[slot].GetItemHex()).Replace("-", string.Empty));
				}
				else
				{
					sb.Append("FFFFFFFFFFFFFFFFFFFF");
				}
			}

			return sb.ToString();
		}

		private void ItemEquipPanel_MouseClick(object sender, MouseEventArgs e)
		{
			int index = this.FindRectangle(e.X, e.Y);

			if (index < 0 || index >= ItemManager.INVENTORY_WEAR_SIZE)
			{
				this.selectedItem = null;

				this.Invalidate();

				return;
			}

			if (e.Button == MouseButtons.Left)
			{
				if (this.m_Items[index].IsEmpty == false)
				{
					this.editor.SetItemInfo(this.m_Items[index]);

					this.selectedItem = this.m_Items[index];
				}
				else
				{
					CItem item = this.editor.GetItemInfo();

					if (this.InsertItem(item, index))
					{
						this.selectedItem = item;
					}
				}
			}
			else if (e.Button == MouseButtons.Right)
			{
				this.selectedItem = null;

				if (this.m_Items[index].IsEmpty == false)
				{
					this.editor.SetItemInfo(this.m_Items[index]);

					this.m_Items[index].Clear();
				}
			}

			this.Invalidate();
		}

		private void ItemEquipPanel_Paint(object sender, PaintEventArgs e)
		{
			Graphics graph = e.Graphics;

			for (int slot = 0; slot < ItemManager.INVENTORY_WEAR_SIZE; slot++)
			{
				if (this.m_Items[slot].IsEmpty == false)
				{
					graph.DrawImage(this.m_Items[slot].ItemImage, this.positions[slot]);

					if (this.selectedItem == this.m_Items[slot])
					{
						graph.DrawRectangle(Pens.Blue, this.positions[slot]);
					}

					graph.DrawString("+" + Convert.ToString(this.m_Items[slot].ItemLevel), new Font("Tahoma", 10), Brushes.Red, this.positions[slot].X, this.positions[slot].Y);
				}
			}
		}
	}
}