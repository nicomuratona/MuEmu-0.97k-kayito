using kayito_Editor.Source;
using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace kayito_Editor.Forms
{
	public partial class ItemEditor : Form
	{
		private IDictionary<int, string> FilteredItems = new Dictionary<int, string>();

		public ItemEditor()
		{
			InitializeComponent();
		}

		private void ItemEditor_Load(object sender, System.EventArgs e)
		{
			this.Section_box.DataSource = new BindingSource(ItemManager.m_ItemSection, null);
			this.Section_box.DisplayMember = "Value";
			this.Section_box.ValueMember = "Key";

			this.Item_Box.DataSource = new BindingSource(this.FilteredItems, null);
			this.Item_Box.DisplayMember = "Value";
			this.Item_Box.ValueMember = "Key";

			this.Level_Box.DataSource = new BindingSource(ItemManager.m_ItemLevel, null);
			this.Level_Box.DisplayMember = "Value";
			this.Level_Box.ValueMember = "Key";

			this.Option_Box.DataSource = new BindingSource(ItemManager.m_ItemAddOption, null);
			this.Option_Box.DisplayMember = "Value";
			this.Option_Box.ValueMember = "Key";
		}

		private void Section_box_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			this.FilteredItems.Clear();

			foreach (ItemManager.ITEM_TXT_INFO item in ItemManager.m_ItemInfo.Values)
			{
				if (item.Section == this.Section_box.SelectedIndex)
				{
					this.FilteredItems.Add(item.Index, item.Name);
				}
			}

			this.Item_Box.DataSource = new BindingSource(this.FilteredItems, null);
		}

		private void Item_Box_SelectedIndexChanged(object sender, EventArgs e)
		{
			try
			{
				string text;

				int ItemIndex = ItemManager.GET_ITEM(this.Section_box.SelectedIndex, (int)this.Item_Box.SelectedValue);

				this.Skill_Box.Enabled = ItemOption.GetItemOption(0, ItemIndex, out text);
				this.Skill_Box.Checked = (this.Skill_Box.Enabled) ? this.Skill_Box.Checked : this.Skill_Box.Enabled;

				this.Luck_Box.Enabled = ItemOption.GetItemOption(1, ItemIndex, out text);
				this.Luck_Box.Checked = (this.Luck_Box.Enabled) ? this.Luck_Box.Checked : this.Luck_Box.Enabled;

				this.Option_Box.Enabled = ItemOption.GetItemOption(2, ItemIndex, out text);
				this.Option_Box.SelectedIndex = (this.Option_Box.Enabled) ? this.Option_Box.SelectedIndex : 0;

				this.Excellent1_Box.Enabled = ItemOption.GetItemOption(3, ItemIndex, out text);
				this.Excellent1_Box.Checked = (this.Excellent1_Box.Enabled) ? this.Excellent1_Box.Checked : this.Excellent1_Box.Enabled;
				this.Excellent1_Box.Text = text;

				this.Excellent2_Box.Enabled = ItemOption.GetItemOption(4, ItemIndex, out text);
				this.Excellent2_Box.Checked = (this.Excellent2_Box.Enabled) ? this.Excellent2_Box.Checked : this.Excellent2_Box.Enabled;
				this.Excellent2_Box.Text = text;

				this.Excellent3_Box.Enabled = ItemOption.GetItemOption(5, ItemIndex, out text);
				this.Excellent3_Box.Checked = (this.Excellent3_Box.Enabled) ? this.Excellent3_Box.Checked : this.Excellent3_Box.Enabled;
				this.Excellent3_Box.Text = text;

				this.Excellent4_Box.Enabled = ItemOption.GetItemOption(6, ItemIndex, out text);
				this.Excellent4_Box.Checked = (this.Excellent4_Box.Enabled) ? this.Excellent4_Box.Checked : this.Excellent4_Box.Enabled;
				this.Excellent4_Box.Text = text;

				this.Excellent5_Box.Enabled = ItemOption.GetItemOption(7, ItemIndex, out text);
				this.Excellent5_Box.Checked = (this.Excellent5_Box.Enabled) ? this.Excellent5_Box.Checked : this.Excellent5_Box.Enabled;
				this.Excellent5_Box.Text = text;

				this.Excellent6_Box.Enabled = ItemOption.GetItemOption(8, ItemIndex, out text);
				this.Excellent6_Box.Checked = (this.Excellent6_Box.Enabled) ? this.Excellent6_Box.Checked : this.Excellent6_Box.Enabled;
				this.Excellent6_Box.Text = text;

				this.Excellent_Options_Box.Enabled = true;
			}
			catch
			{
				this.Excellent1_Box.Checked = false;
				this.Excellent2_Box.Checked = false;
				this.Excellent3_Box.Checked = false;
				this.Excellent4_Box.Checked = false;
				this.Excellent5_Box.Checked = false;
				this.Excellent6_Box.Checked = false;
				this.Excellent_Options_Box.Enabled = false;
			}
		}

		private void Btn_Excellent_Select_All(object sender, EventArgs e)
		{
			this.Excellent1_Box.Checked = true;
			this.Excellent2_Box.Checked = true;
			this.Excellent3_Box.Checked = true;
			this.Excellent4_Box.Checked = true;
			this.Excellent5_Box.Checked = true;
			this.Excellent6_Box.Checked = true;
		}

		private void Btn_Excellent_Unselect_All(object sender, EventArgs e)
		{
			this.Excellent1_Box.Checked = false;
			this.Excellent2_Box.Checked = false;
			this.Excellent3_Box.Checked = false;
			this.Excellent4_Box.Checked = false;
			this.Excellent5_Box.Checked = false;
			this.Excellent6_Box.Checked = false;
		}

		public void SetItemInfo(CItem item)
		{
			this.Section_box.SelectedIndex = item.Section;

			this.Item_Box.SelectedValue = item.Index;

			this.Durability_Box.Value = item.ItemDurability;

			this.Level_Box.SelectedIndex = item.ItemLevel;

			this.Skill_Box.Checked = item.SkillOption != 0;

			this.Option_Box.SelectedIndex = item.AdditionalOption;

			this.Luck_Box.Checked = item.LuckOption != 0;

			this.Excellent1_Box.Checked = ((item.ExcellentOption >> 0) & 0x1) == 0x1;
			this.Excellent2_Box.Checked = ((item.ExcellentOption >> 1) & 0x1) == 0x1;
			this.Excellent3_Box.Checked = ((item.ExcellentOption >> 2) & 0x1) == 0x1;
			this.Excellent4_Box.Checked = ((item.ExcellentOption >> 3) & 0x1) == 0x1;
			this.Excellent5_Box.Checked = ((item.ExcellentOption >> 4) & 0x1) == 0x1;
			this.Excellent6_Box.Checked = ((item.ExcellentOption >> 5) & 0x1) == 0x1;
		}

		public CItem GetItemInfo()
		{
			CItem item = new CItem();

			int excellent = Convert.ToInt32(this.Excellent1_Box.Checked) | (Convert.ToInt32(this.Excellent2_Box.Checked) << 1) | (Convert.ToInt32(this.Excellent3_Box.Checked) << 2) | (Convert.ToInt32(this.Excellent4_Box.Checked) << 3) | (Convert.ToInt32(this.Excellent5_Box.Checked) << 4) | (Convert.ToInt32(this.Excellent6_Box.Checked) << 5);

			int durability = Convert.ToInt32(this.Durability_Box.Value);

			if (durability == -1)
			{
				int ItemIndex = ItemManager.GET_ITEM(this.Section_box.SelectedIndex, (int)this.Item_Box.SelectedValue);

				durability = ItemManager.GetItemDurability(ItemIndex, this.Level_Box.SelectedIndex, excellent);
			}

			item.Convert(this.Section_box.SelectedIndex, (int)this.Item_Box.SelectedValue, this.Level_Box.SelectedIndex, durability, Convert.ToInt32(this.Skill_Box.Checked), Convert.ToInt32(this.Luck_Box.Checked), this.Option_Box.SelectedIndex, excellent);

			return item;
		}
	}
}