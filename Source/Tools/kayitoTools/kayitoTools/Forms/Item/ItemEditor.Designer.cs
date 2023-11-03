
namespace kayitoTools.Forms
{
	partial class ItemEditor
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ItemEditor));
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle11 = new System.Windows.Forms.DataGridViewCellStyle();
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle14 = new System.Windows.Forms.DataGridViewCellStyle();
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle15 = new System.Windows.Forms.DataGridViewCellStyle();
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle12 = new System.Windows.Forms.DataGridViewCellStyle();
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle13 = new System.Windows.Forms.DataGridViewCellStyle();
			this.toolStrip1 = new System.Windows.Forms.ToolStrip();
			this.Load_Item_Menu_Btn = new System.Windows.Forms.ToolStripDropDownButton();
			this.loadItemtxtToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.loadItembmdToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.Save_Item_Menu_Btn = new System.Windows.Forms.ToolStripDropDownButton();
			this.saveItemtxtToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
			this.saveItembmdToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.OpenItemFile = new System.Windows.Forms.OpenFileDialog();
			this.Item_List_Box = new System.Windows.Forms.DataGridView();
			this.Row_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Index_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Name_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Level_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Slot_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Skill_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Width_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Height_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Serial_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Option_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Drop_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.DamageMin_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.DamageMax_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.DefenseRate_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Defense_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.MagicDefense_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.AttackSpeed_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.WalkSpeed_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Durability_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.MagicDurability_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.MagicPower_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.RequireStrength_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.RequireAgility_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.RequireEnergy_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.RequireVitality_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.RequireLevel_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Value_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Money_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Attribute_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.RequireDW_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.RequireDK_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.RequireELF_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.RequireMG_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.ResistanceIce_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.ResistancePoison_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.ResistanceLightning_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.ResistanceFire_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.SaveItemFile = new System.Windows.Forms.SaveFileDialog();
			this.Item_Group_Selector_Box = new System.Windows.Forms.ComboBox();
			this.label1 = new System.Windows.Forms.Label();
			this.toolStrip1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.Item_List_Box)).BeginInit();
			this.SuspendLayout();
			// 
			// toolStrip1
			// 
			this.toolStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Load_Item_Menu_Btn,
            this.Save_Item_Menu_Btn});
			this.toolStrip1.Location = new System.Drawing.Point(0, 0);
			this.toolStrip1.Name = "toolStrip1";
			this.toolStrip1.Padding = new System.Windows.Forms.Padding(0);
			this.toolStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
			this.toolStrip1.ShowItemToolTips = false;
			this.toolStrip1.Size = new System.Drawing.Size(1264, 25);
			this.toolStrip1.TabIndex = 0;
			this.toolStrip1.Text = "toolStrip1";
			// 
			// Load_Item_Menu_Btn
			// 
			this.Load_Item_Menu_Btn.AutoSize = false;
			this.Load_Item_Menu_Btn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this.Load_Item_Menu_Btn.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadItemtxtToolStripMenuItem,
            this.loadItembmdToolStripMenuItem});
			this.Load_Item_Menu_Btn.Image = ((System.Drawing.Image)(resources.GetObject("Load_Item_Menu_Btn.Image")));
			this.Load_Item_Menu_Btn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.Load_Item_Menu_Btn.Margin = new System.Windows.Forms.Padding(10, 0, 10, 0);
			this.Load_Item_Menu_Btn.Name = "Load_Item_Menu_Btn";
			this.Load_Item_Menu_Btn.Size = new System.Drawing.Size(130, 22);
			this.Load_Item_Menu_Btn.Text = "LOAD";
			// 
			// loadItemtxtToolStripMenuItem
			// 
			this.loadItemtxtToolStripMenuItem.Name = "loadItemtxtToolStripMenuItem";
			this.loadItemtxtToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
			this.loadItemtxtToolStripMenuItem.Text = "Load Item.txt";
			this.loadItemtxtToolStripMenuItem.Click += new System.EventHandler(this.LoadItemtxt_Click);
			// 
			// loadItembmdToolStripMenuItem
			// 
			this.loadItembmdToolStripMenuItem.Name = "loadItembmdToolStripMenuItem";
			this.loadItembmdToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
			this.loadItembmdToolStripMenuItem.Text = "Load Item.bmd";
			this.loadItembmdToolStripMenuItem.Click += new System.EventHandler(this.LoadItembmd_Click);
			// 
			// Save_Item_Menu_Btn
			// 
			this.Save_Item_Menu_Btn.AutoSize = false;
			this.Save_Item_Menu_Btn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this.Save_Item_Menu_Btn.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.saveItemtxtToolStripMenuItem1,
            this.saveItembmdToolStripMenuItem});
			this.Save_Item_Menu_Btn.Enabled = false;
			this.Save_Item_Menu_Btn.Image = ((System.Drawing.Image)(resources.GetObject("Save_Item_Menu_Btn.Image")));
			this.Save_Item_Menu_Btn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.Save_Item_Menu_Btn.Margin = new System.Windows.Forms.Padding(10, 0, 10, 0);
			this.Save_Item_Menu_Btn.Name = "Save_Item_Menu_Btn";
			this.Save_Item_Menu_Btn.Size = new System.Drawing.Size(130, 22);
			this.Save_Item_Menu_Btn.Text = "SAVE";
			// 
			// saveItemtxtToolStripMenuItem1
			// 
			this.saveItemtxtToolStripMenuItem1.Name = "saveItemtxtToolStripMenuItem1";
			this.saveItemtxtToolStripMenuItem1.Size = new System.Drawing.Size(153, 22);
			this.saveItemtxtToolStripMenuItem1.Text = "Save Item.txt";
			this.saveItemtxtToolStripMenuItem1.Click += new System.EventHandler(this.SaveItemtxt_Click);
			// 
			// saveItembmdToolStripMenuItem
			// 
			this.saveItembmdToolStripMenuItem.Name = "saveItembmdToolStripMenuItem";
			this.saveItembmdToolStripMenuItem.Size = new System.Drawing.Size(153, 22);
			this.saveItembmdToolStripMenuItem.Text = "Save Item.bmd";
			this.saveItembmdToolStripMenuItem.Click += new System.EventHandler(this.SaveItembmd_Click);
			// 
			// OpenItemFile
			// 
			this.OpenItemFile.FileName = "Item";
			this.OpenItemFile.Filter = "Every file (*.*)|*.*";
			// 
			// Item_List_Box
			// 
			this.Item_List_Box.AllowUserToAddRows = false;
			this.Item_List_Box.AllowUserToDeleteRows = false;
			this.Item_List_Box.AllowUserToResizeRows = false;
			this.Item_List_Box.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
			this.Item_List_Box.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.Item_List_Box.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.EnableWithoutHeaderText;
			this.Item_List_Box.ColumnHeadersBorderStyle = System.Windows.Forms.DataGridViewHeaderBorderStyle.Single;
			dataGridViewCellStyle11.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle11.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
			dataGridViewCellStyle11.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			dataGridViewCellStyle11.ForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle11.SelectionBackColor = System.Drawing.Color.DimGray;
			dataGridViewCellStyle11.SelectionForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle11.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
			this.Item_List_Box.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle11;
			this.Item_List_Box.ColumnHeadersHeight = 50;
			this.Item_List_Box.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Row_Col,
            this.Index_Col,
            this.Name_Col,
            this.Level_Col,
            this.Slot_Col,
            this.Skill_Col,
            this.Width_Col,
            this.Height_Col,
            this.Serial_Col,
            this.Option_Col,
            this.Drop_Col,
            this.DamageMin_Col,
            this.DamageMax_Col,
            this.DefenseRate_Col,
            this.Defense_Col,
            this.MagicDefense_Col,
            this.AttackSpeed_Col,
            this.WalkSpeed_Col,
            this.Durability_Col,
            this.MagicDurability_Col,
            this.MagicPower_Col,
            this.RequireStrength_Col,
            this.RequireAgility_Col,
            this.RequireEnergy_Col,
            this.RequireVitality_Col,
            this.RequireLevel_Col,
            this.Value_Col,
            this.Money_Col,
            this.Attribute_Col,
            this.RequireDW_Col,
            this.RequireDK_Col,
            this.RequireELF_Col,
            this.RequireMG_Col,
            this.ResistanceIce_Col,
            this.ResistancePoison_Col,
            this.ResistanceLightning_Col,
            this.ResistanceFire_Col});
			dataGridViewCellStyle14.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle14.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(50)))), ((int)(((byte)(60)))));
			dataGridViewCellStyle14.Font = new System.Drawing.Font("Consolas", 10F);
			dataGridViewCellStyle14.ForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle14.SelectionBackColor = System.Drawing.Color.LightGray;
			dataGridViewCellStyle14.SelectionForeColor = System.Drawing.Color.Black;
			dataGridViewCellStyle14.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
			this.Item_List_Box.DefaultCellStyle = dataGridViewCellStyle14;
			this.Item_List_Box.Dock = System.Windows.Forms.DockStyle.Fill;
			this.Item_List_Box.EnableHeadersVisualStyles = false;
			this.Item_List_Box.GridColor = System.Drawing.Color.FromArgb(((int)(((byte)(15)))), ((int)(((byte)(15)))), ((int)(((byte)(15)))));
			this.Item_List_Box.Location = new System.Drawing.Point(0, 25);
			this.Item_List_Box.Margin = new System.Windows.Forms.Padding(0);
			this.Item_List_Box.Name = "Item_List_Box";
			this.Item_List_Box.RowHeadersBorderStyle = System.Windows.Forms.DataGridViewHeaderBorderStyle.Single;
			dataGridViewCellStyle15.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle15.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
			dataGridViewCellStyle15.ForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle15.SelectionBackColor = System.Drawing.Color.DimGray;
			dataGridViewCellStyle15.SelectionForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle15.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
			this.Item_List_Box.RowHeadersDefaultCellStyle = dataGridViewCellStyle15;
			this.Item_List_Box.RowHeadersWidth = 30;
			this.Item_List_Box.RowTemplate.Resizable = System.Windows.Forms.DataGridViewTriState.False;
			this.Item_List_Box.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
			this.Item_List_Box.Size = new System.Drawing.Size(1264, 656);
			this.Item_List_Box.TabIndex = 1;
			this.Item_List_Box.Visible = false;
			this.Item_List_Box.CellValidating += new System.Windows.Forms.DataGridViewCellValidatingEventHandler(this.Item_List_Box_CellValidating);
			this.Item_List_Box.CellValueChanged += new System.Windows.Forms.DataGridViewCellEventHandler(this.Item_List_Box_CellValueChanged);
			this.Item_List_Box.Scroll += new System.Windows.Forms.ScrollEventHandler(this.Item_List_Box_Scroll);
			// 
			// Row_Col
			// 
			this.Row_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			dataGridViewCellStyle12.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle12.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(15)))), ((int)(((byte)(15)))), ((int)(((byte)(15)))));
			dataGridViewCellStyle12.ForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle12.SelectionBackColor = System.Drawing.Color.DimGray;
			dataGridViewCellStyle12.SelectionForeColor = System.Drawing.Color.White;
			this.Row_Col.DefaultCellStyle = dataGridViewCellStyle12;
			this.Row_Col.HeaderText = "Row";
			this.Row_Col.Name = "Row_Col";
			this.Row_Col.ReadOnly = true;
			this.Row_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Row_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Row_Col.ToolTipText = "Number of Row";
			this.Row_Col.Width = 60;
			// 
			// Index_Col
			// 
			this.Index_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Index_Col.HeaderText = "Index";
			this.Index_Col.Name = "Index_Col";
			this.Index_Col.ReadOnly = true;
			this.Index_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Index_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Index_Col.ToolTipText = "Number of the Item (0 ~ 512)";
			this.Index_Col.Width = 60;
			// 
			// Name_Col
			// 
			this.Name_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			dataGridViewCellStyle13.NullValue = null;
			this.Name_Col.DefaultCellStyle = dataGridViewCellStyle13;
			this.Name_Col.HeaderText = "Name";
			this.Name_Col.Name = "Name_Col";
			this.Name_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Name_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Name_Col.ToolTipText = "Name of the Item";
			this.Name_Col.Width = 300;
			// 
			// Level_Col
			// 
			this.Level_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Level_Col.HeaderText = "Level";
			this.Level_Col.Name = "Level_Col";
			this.Level_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Level_Col.ToolTipText = "Level of the Item";
			this.Level_Col.Width = 80;
			// 
			// Slot_Col
			// 
			this.Slot_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Slot_Col.HeaderText = "Slot";
			this.Slot_Col.Name = "Slot_Col";
			this.Slot_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Slot_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Slot_Col.ToolTipText = "Slot to occupy by the Item";
			this.Slot_Col.Width = 80;
			// 
			// Skill_Col
			// 
			this.Skill_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Skill_Col.HeaderText = "Skill";
			this.Skill_Col.Name = "Skill_Col";
			this.Skill_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Skill_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Skill_Col.ToolTipText = "Skill that the Item has";
			this.Skill_Col.Width = 80;
			// 
			// Width_Col
			// 
			this.Width_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Width_Col.HeaderText = "Width";
			this.Width_Col.Name = "Width_Col";
			this.Width_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Width_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Width_Col.ToolTipText = "X columns to ocuppy by the Item";
			this.Width_Col.Width = 80;
			// 
			// Height_Col
			// 
			this.Height_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Height_Col.HeaderText = "Height";
			this.Height_Col.Name = "Height_Col";
			this.Height_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Height_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Height_Col.ToolTipText = "Y columns to ocuppy by the Item";
			this.Height_Col.Width = 80;
			// 
			// Serial_Col
			// 
			this.Serial_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Serial_Col.HeaderText = "Serial";
			this.Serial_Col.Name = "Serial_Col";
			this.Serial_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Serial_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Serial_Col.ToolTipText = "[ServerOnly] The item has serial (0 / 1)";
			this.Serial_Col.Width = 80;
			// 
			// Option_Col
			// 
			this.Option_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Option_Col.HeaderText = "Option";
			this.Option_Col.Name = "Option_Col";
			this.Option_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Option_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Option_Col.ToolTipText = "[ServerOnly] The item has option (0 / 1)";
			this.Option_Col.Width = 80;
			// 
			// Drop_Col
			// 
			this.Drop_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Drop_Col.HeaderText = "Drop";
			this.Drop_Col.Name = "Drop_Col";
			this.Drop_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Drop_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Drop_Col.ToolTipText = "[ServerOnly] The item will be dropped (0 / 1)";
			this.Drop_Col.Width = 80;
			// 
			// DamageMin_Col
			// 
			this.DamageMin_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.DamageMin_Col.HeaderText = "Damage Min";
			this.DamageMin_Col.Name = "DamageMin_Col";
			this.DamageMin_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.DamageMin_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.DamageMin_Col.ToolTipText = "Minimal Damage of the Item";
			this.DamageMin_Col.Width = 80;
			// 
			// DamageMax_Col
			// 
			this.DamageMax_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.DamageMax_Col.HeaderText = "Damage Max";
			this.DamageMax_Col.Name = "DamageMax_Col";
			this.DamageMax_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.DamageMax_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.DamageMax_Col.ToolTipText = "Maximum Damage of the Item";
			this.DamageMax_Col.Width = 80;
			// 
			// DefenseRate_Col
			// 
			this.DefenseRate_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.DefenseRate_Col.HeaderText = "Defense Rate";
			this.DefenseRate_Col.Name = "DefenseRate_Col";
			this.DefenseRate_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.DefenseRate_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.DefenseRate_Col.ToolTipText = "Rate of Defense of the Item";
			this.DefenseRate_Col.Width = 80;
			// 
			// Defense_Col
			// 
			this.Defense_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Defense_Col.HeaderText = "Defense";
			this.Defense_Col.Name = "Defense_Col";
			this.Defense_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Defense_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Defense_Col.ToolTipText = "Defense of the Item";
			this.Defense_Col.Width = 80;
			// 
			// MagicDefense_Col
			// 
			this.MagicDefense_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.MagicDefense_Col.HeaderText = "Magic Defense";
			this.MagicDefense_Col.Name = "MagicDefense_Col";
			this.MagicDefense_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.MagicDefense_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.MagicDefense_Col.ToolTipText = "Magic Defense of the Item";
			this.MagicDefense_Col.Width = 80;
			// 
			// AttackSpeed_Col
			// 
			this.AttackSpeed_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.AttackSpeed_Col.HeaderText = "Attack Speed";
			this.AttackSpeed_Col.Name = "AttackSpeed_Col";
			this.AttackSpeed_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.AttackSpeed_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.AttackSpeed_Col.ToolTipText = "Attack Speed of the Item";
			this.AttackSpeed_Col.Width = 80;
			// 
			// WalkSpeed_Col
			// 
			this.WalkSpeed_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.WalkSpeed_Col.HeaderText = "Walk Speed";
			this.WalkSpeed_Col.Name = "WalkSpeed_Col";
			this.WalkSpeed_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.WalkSpeed_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.WalkSpeed_Col.ToolTipText = "Walk Speed of the Item";
			this.WalkSpeed_Col.Width = 80;
			// 
			// Durability_Col
			// 
			this.Durability_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Durability_Col.HeaderText = "Durability";
			this.Durability_Col.Name = "Durability_Col";
			this.Durability_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Durability_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Durability_Col.ToolTipText = "Durability of the Item";
			this.Durability_Col.Width = 80;
			// 
			// MagicDurability_Col
			// 
			this.MagicDurability_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.MagicDurability_Col.HeaderText = "Magic Durability";
			this.MagicDurability_Col.Name = "MagicDurability_Col";
			this.MagicDurability_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.MagicDurability_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.MagicDurability_Col.ToolTipText = "Magic Durability of the Item";
			this.MagicDurability_Col.Width = 80;
			// 
			// MagicPower_Col
			// 
			this.MagicPower_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.MagicPower_Col.HeaderText = "Magic Power";
			this.MagicPower_Col.Name = "MagicPower_Col";
			this.MagicPower_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.MagicPower_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.MagicPower_Col.ToolTipText = "Magic Power of the Item";
			this.MagicPower_Col.Width = 80;
			// 
			// RequireStrength_Col
			// 
			this.RequireStrength_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.RequireStrength_Col.HeaderText = "Require Strength";
			this.RequireStrength_Col.Name = "RequireStrength_Col";
			this.RequireStrength_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.RequireStrength_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.RequireStrength_Col.ToolTipText = "Required Strength to use this Item";
			this.RequireStrength_Col.Width = 80;
			// 
			// RequireAgility_Col
			// 
			this.RequireAgility_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.RequireAgility_Col.HeaderText = "Require Agility";
			this.RequireAgility_Col.Name = "RequireAgility_Col";
			this.RequireAgility_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.RequireAgility_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.RequireAgility_Col.ToolTipText = "Required Agility to use this Item";
			this.RequireAgility_Col.Width = 80;
			// 
			// RequireEnergy_Col
			// 
			this.RequireEnergy_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.RequireEnergy_Col.HeaderText = "Require Energy";
			this.RequireEnergy_Col.Name = "RequireEnergy_Col";
			this.RequireEnergy_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.RequireEnergy_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.RequireEnergy_Col.ToolTipText = "Required Energy to use this Item";
			this.RequireEnergy_Col.Width = 80;
			// 
			// RequireVitality_Col
			// 
			this.RequireVitality_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.RequireVitality_Col.HeaderText = "Require Vitality";
			this.RequireVitality_Col.Name = "RequireVitality_Col";
			this.RequireVitality_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.RequireVitality_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.RequireVitality_Col.ToolTipText = "Required Vitality to use this Item";
			this.RequireVitality_Col.Width = 80;
			// 
			// RequireLevel_Col
			// 
			this.RequireLevel_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.RequireLevel_Col.HeaderText = "Require Level";
			this.RequireLevel_Col.Name = "RequireLevel_Col";
			this.RequireLevel_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.RequireLevel_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.RequireLevel_Col.ToolTipText = "Required Level to use this Item";
			this.RequireLevel_Col.Width = 80;
			// 
			// Value_Col
			// 
			this.Value_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Value_Col.HeaderText = "Value";
			this.Value_Col.Name = "Value_Col";
			this.Value_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Value_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Value_Col.ToolTipText = "Value of the Item";
			this.Value_Col.Width = 80;
			// 
			// Money_Col
			// 
			this.Money_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Money_Col.HeaderText = "Money";
			this.Money_Col.Name = "Money_Col";
			this.Money_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Money_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Money_Col.ToolTipText = "Buy Money of the Item";
			this.Money_Col.Width = 160;
			// 
			// Attribute_Col
			// 
			this.Attribute_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Attribute_Col.HeaderText = "Attribute";
			this.Attribute_Col.Name = "Attribute_Col";
			this.Attribute_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Attribute_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Attribute_Col.ToolTipText = "Attribute of the Item";
			this.Attribute_Col.Width = 80;
			// 
			// RequireDW_Col
			// 
			this.RequireDW_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.RequireDW_Col.HeaderText = "Require DW";
			this.RequireDW_Col.Name = "RequireDW_Col";
			this.RequireDW_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.RequireDW_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.RequireDW_Col.ToolTipText = "Required level of evolution for Dark Wizard to use the Item (0 = Disabled for thi" +
    "s class)";
			this.RequireDW_Col.Width = 80;
			// 
			// RequireDK_Col
			// 
			this.RequireDK_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.RequireDK_Col.HeaderText = "Require DK";
			this.RequireDK_Col.Name = "RequireDK_Col";
			this.RequireDK_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.RequireDK_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.RequireDK_Col.ToolTipText = "Required level of evolution for Dark Knight to use the Item (0 = Disabled for thi" +
    "s class)";
			this.RequireDK_Col.Width = 80;
			// 
			// RequireELF_Col
			// 
			this.RequireELF_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.RequireELF_Col.HeaderText = "Require ELF";
			this.RequireELF_Col.Name = "RequireELF_Col";
			this.RequireELF_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.RequireELF_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.RequireELF_Col.ToolTipText = "Required level of evolution for Fairy Elf to use the Item (0 = Disabled for this " +
    "class)";
			this.RequireELF_Col.Width = 80;
			// 
			// RequireMG_Col
			// 
			this.RequireMG_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.RequireMG_Col.HeaderText = "Require MG";
			this.RequireMG_Col.Name = "RequireMG_Col";
			this.RequireMG_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.RequireMG_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.RequireMG_Col.ToolTipText = "Required level of evolution for Magic Gladiator to use the Item (0 = Disabled for" +
    " this class)";
			this.RequireMG_Col.Width = 80;
			// 
			// ResistanceIce_Col
			// 
			this.ResistanceIce_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.ResistanceIce_Col.HeaderText = "Resistance Ice";
			this.ResistanceIce_Col.Name = "ResistanceIce_Col";
			this.ResistanceIce_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.ResistanceIce_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.ResistanceIce_Col.ToolTipText = "Resistance level for Ice of the Item";
			this.ResistanceIce_Col.Width = 80;
			// 
			// ResistancePoison_Col
			// 
			this.ResistancePoison_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.ResistancePoison_Col.HeaderText = "Resistance Poison";
			this.ResistancePoison_Col.Name = "ResistancePoison_Col";
			this.ResistancePoison_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.ResistancePoison_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.ResistancePoison_Col.ToolTipText = "Resistance level for Poison of the Item";
			this.ResistancePoison_Col.Width = 80;
			// 
			// ResistanceLightning_Col
			// 
			this.ResistanceLightning_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.ResistanceLightning_Col.HeaderText = "Resistance Lightning";
			this.ResistanceLightning_Col.Name = "ResistanceLightning_Col";
			this.ResistanceLightning_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.ResistanceLightning_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.ResistanceLightning_Col.ToolTipText = "Resistance level for Lightning of the Item";
			this.ResistanceLightning_Col.Width = 80;
			// 
			// ResistanceFire_Col
			// 
			this.ResistanceFire_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.ResistanceFire_Col.HeaderText = "Resistance Fire";
			this.ResistanceFire_Col.Name = "ResistanceFire_Col";
			this.ResistanceFire_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.ResistanceFire_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.ResistanceFire_Col.ToolTipText = "Resistance level for Fire of the Item";
			this.ResistanceFire_Col.Width = 80;
			// 
			// Item_Group_Selector_Box
			// 
			this.Item_Group_Selector_Box.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.Item_Group_Selector_Box.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.Item_Group_Selector_Box.FormattingEnabled = true;
			this.Item_Group_Selector_Box.Location = new System.Drawing.Point(1131, 1);
			this.Item_Group_Selector_Box.Name = "Item_Group_Selector_Box";
			this.Item_Group_Selector_Box.Size = new System.Drawing.Size(121, 21);
			this.Item_Group_Selector_Box.TabIndex = 3;
			this.Item_Group_Selector_Box.SelectedIndexChanged += new System.EventHandler(this.Item_Group_Selector_Box_SelectedIndexChanged);
			// 
			// label1
			// 
			this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.label1.AutoSize = true;
			this.label1.BackColor = System.Drawing.Color.Transparent;
			this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label1.ForeColor = System.Drawing.Color.White;
			this.label1.Location = new System.Drawing.Point(1056, 3);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(69, 17);
			this.label1.TabIndex = 6;
			this.label1.Text = "Category:";
			// 
			// ItemEditor
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.BackColor = System.Drawing.Color.Black;
			this.ClientSize = new System.Drawing.Size(1264, 681);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.Item_Group_Selector_Box);
			this.Controls.Add(this.Item_List_Box);
			this.Controls.Add(this.toolStrip1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "ItemEditor";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Item Manager";
			this.Load += new System.EventHandler(this.ItemManager_Load);
			this.toolStrip1.ResumeLayout(false);
			this.toolStrip1.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.Item_List_Box)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ToolStrip toolStrip1;
		private System.Windows.Forms.ToolStripDropDownButton Load_Item_Menu_Btn;
		private System.Windows.Forms.ToolStripMenuItem loadItembmdToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem loadItemtxtToolStripMenuItem;
		private System.Windows.Forms.ToolStripDropDownButton Save_Item_Menu_Btn;
		private System.Windows.Forms.ToolStripMenuItem saveItembmdToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem saveItemtxtToolStripMenuItem1;
		private System.Windows.Forms.OpenFileDialog OpenItemFile;
		private System.Windows.Forms.DataGridView Item_List_Box;
		private System.Windows.Forms.SaveFileDialog SaveItemFile;
		private System.Windows.Forms.ComboBox Item_Group_Selector_Box;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.DataGridViewTextBoxColumn Row_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Index_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Name_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Level_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Slot_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Skill_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Width_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Height_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Serial_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Option_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Drop_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn DamageMin_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn DamageMax_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn DefenseRate_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Defense_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn MagicDefense_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn AttackSpeed_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn WalkSpeed_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Durability_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn MagicDurability_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn MagicPower_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn RequireStrength_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn RequireAgility_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn RequireEnergy_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn RequireVitality_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn RequireLevel_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Value_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Money_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Attribute_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn RequireDW_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn RequireDK_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn RequireELF_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn RequireMG_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn ResistanceIce_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn ResistancePoison_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn ResistanceLightning_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn ResistanceFire_Col;
	}
}