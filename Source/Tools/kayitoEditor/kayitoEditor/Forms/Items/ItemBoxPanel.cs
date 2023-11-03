using System;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using kayito_Editor.Source;

namespace kayito_Editor.Forms
{
	public partial class ItemBoxPanel : Form
	{
		private readonly int pixels = 25;

		private int sizeX;
		private int sizeY;

		private CItem selectedItem = null;

		public CItem[] m_Items;

		private ItemEditor editor;

        private bool EQUIP_INVENTORY_RANGE(int slot)
        {
            return slot >= 0 && (slot < (this.sizeX * this.sizeY));
        }

		public ItemBoxPanel(int SizeX, int SizeY)
		{
			InitializeComponent();

			this.sizeX = SizeX;

			this.sizeY = SizeY;

			int quant = SizeX * SizeY;

			this.m_Items = new CItem[quant];

			for (int i = 0; i < (SizeX * SizeY); i++)
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
			for (int i = 0; i < this.sizeX * this.sizeY; i++)
			{
				this.m_Items[i].Clear();
			}

			this.Invalidate();
		}

		public void ReloadForm()
		{
			this.Invalidate();
		}

		public int EquipRectCheck(int x, int y, int width, int height)
		{
			if ((x + width) > this.sizeX || (y + height) > this.sizeY)
			{
				return -1;
			}

			for (int sy = 0; sy < height; sy++)
			{
				for (int sx = 0; sx < width; sx++)
				{
					if (this.m_Items[(((sy + y) * this.sizeX) + (sx + x))].IsEmpty == false)
					{
						return -1;
					}
				}
			}

			return (y * this.sizeX) + x;
		}

		public void EquipItemSet(int slot)
		{
			if (EQUIP_INVENTORY_RANGE(slot) == false)
			{
				return;
			}

            ItemManager.ITEM_TXT_INFO ItemInfo;

            if (!ItemManager.m_ItemInfo.TryGetValue((ItemManager.MAX_ITEM_TYPE * this.m_Items[slot].Section) + this.m_Items[slot].Index, out ItemInfo))
			{
				return;
			}

			int x = slot % this.sizeX;

			int y = slot / this.sizeX;

			if ((x + ItemInfo.Width) > this.sizeX || (y + ItemInfo.Height) > this.sizeY)
			{
				return;
			}

			for (int sy = 0; sy < ItemInfo.Height; sy++)
			{
				for (int sx = 0; sx < ItemInfo.Width; sx++)
				{
					this.m_Items[((sy + y) * this.sizeX) + sx + x] = this.m_Items[slot];

					this.m_Items[((sy + y) * this.sizeX) + sx + x].Slot = (byte)slot;
				}
			}
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
				int result = this.EquipRectCheck((slot % this.sizeX), (slot / this.sizeX), ItemInfo.Width, ItemInfo.Height);

				if (result != -1)
				{
					this.m_Items[result] = item;

					this.EquipItemSet(result);

					return true;
				}
			}

			return false;
		}

		private void DeleteItem(CItem item)
		{
            ItemManager.ITEM_TXT_INFO ItemInfo;

            if (!ItemManager.m_ItemInfo.TryGetValue((ItemManager.MAX_ITEM_TYPE * item.Section) + item.Index, out ItemInfo))
			{
				return;
			}

			int x = item.Slot % this.sizeX;

			int y = item.Slot / this.sizeX;

			for (int sy = 0; sy < ItemInfo.Height; sy++)
			{
				for (int sx = 0; sx < ItemInfo.Width; sx++)
				{
					this.m_Items[((sy + y) * this.sizeX) + sx + x].Clear();
				}
			}
		}

		public bool GetItem(int slot, out CItem item)
		{
			item = null;

			if (EQUIP_INVENTORY_RANGE(slot) != false)
			{
				if (this.m_Items[slot].IsEmpty == false)
				{
					if (this.m_Items[slot].Slot == slot)
					{
						item = this.m_Items[slot];
					}
					else
					{
						item = this.m_Items[this.m_Items[slot].Slot];
					}

					return true;
				}
			}

			return false;
		}

		public string GetItemsToHex()
		{
			StringBuilder sb = new StringBuilder();

			for (int slot = 0; slot < sizeX * sizeY; slot++)
			{
				if (this.m_Items[slot].IsEmpty == false && this.m_Items[slot].Slot == slot)
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

		private void EquipPanel_MouseClick(object sender, MouseEventArgs e)
		{
			int posX = e.X / this.pixels;

			int posY = e.Y / this.pixels;

			if (e.Button == MouseButtons.Left)
			{
				CItem item;

				if (this.GetItem(posY * sizeX + posX, out item))
				{
					this.editor.SetItemInfo(item);

					this.selectedItem = item;
				}
				else
				{
					item = this.editor.GetItemInfo();

					if (this.InsertItem(item, posY * sizeX + posX))
					{
						this.selectedItem = item;
					}
				}
			}
			else if (e.Button == MouseButtons.Right)
			{
				this.selectedItem = null;

				CItem item;

				if (this.GetItem(posY * sizeX + posX, out item))
				{
					this.editor.SetItemInfo(item);

					this.DeleteItem(item);
				}
			}

			this.Invalidate();
		}

		private void EquipPanel_Paint(object sender, PaintEventArgs e)
		{
			Graphics graph = e.Graphics;

			for (int slot = 0; slot < sizeX * sizeY; slot++)
			{
				if (this.m_Items[slot].IsEmpty == false && this.m_Items[slot].Slot == (slot))
				{
                    ItemManager.ITEM_TXT_INFO ItemInfo;

					if (ItemManager.m_ItemInfo.TryGetValue((32 * this.m_Items[slot].Section + this.m_Items[slot].Index), out ItemInfo))
					{
						graph.DrawImage(this.m_Items[slot].ItemImage, (slot % this.sizeX) * this.pixels, (slot / this.sizeX) * this.pixels, ItemInfo.Width * this.pixels, ItemInfo.Height * this.pixels);

						if (this.selectedItem == this.m_Items[slot])
						{
							graph.DrawRectangle(Pens.Blue, (slot % this.sizeX) * this.pixels + 1, (slot / this.sizeX) * this.pixels + 1, ItemInfo.Width * this.pixels - 2, ItemInfo.Height * this.pixels - 2);
						}

						graph.DrawString("+" + Convert.ToString(this.m_Items[slot].ItemLevel), new Font("Tahoma", 10), Brushes.Red, (slot % this.sizeX) * this.pixels, (slot / this.sizeX) * this.pixels);
					}
				}
			}
		}
	}
}