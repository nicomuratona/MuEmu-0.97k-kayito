
namespace kayitoTools.Forms
{
	partial class MonsterEditor
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MonsterEditor));
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle5 = new System.Windows.Forms.DataGridViewCellStyle();
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle7 = new System.Windows.Forms.DataGridViewCellStyle();
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle8 = new System.Windows.Forms.DataGridViewCellStyle();
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle6 = new System.Windows.Forms.DataGridViewCellStyle();
			this.toolStrip1 = new System.Windows.Forms.ToolStrip();
			this.Load_Monster_Menu_Btn = new System.Windows.Forms.ToolStripDropDownButton();
			this.Load_Monstertxt_AV = new System.Windows.Forms.ToolStripMenuItem();
			this.Load_NPCNametxt_AV = new System.Windows.Forms.ToolStripMenuItem();
			this.Save_Monster_Menu_Btn = new System.Windows.Forms.ToolStripDropDownButton();
			this.Save_Monstertxt_AV = new System.Windows.Forms.ToolStripMenuItem();
			this.Search_Monster_Panel = new System.Windows.Forms.Panel();
			this.Search_Box_Text = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.Search_Box_Btn = new System.Windows.Forms.Button();
			this.Goto_Text_Box = new System.Windows.Forms.NumericUpDown();
			this.Monster_List_Box = new System.Windows.Forms.DataGridView();
			this.Row_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Name_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Level_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.MaxLife_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.MaxMana_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.MinDamage_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.MaxDamage_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Defense_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.MagicDefense_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.AttackRate_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.DefenseRate_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.MoveRange_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.AttackType_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.AttackRange_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.ViewRange_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.MoveSpeed_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.AttackSpeed_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.RegenTime_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Attribute_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.ItemRate_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Money_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.MaxItemLevel_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.MonsterSkill_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.ResistanceIce_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.ResistancePoison_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.ResistanceLightning_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.ResistanceFire_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.OpenMonsterFile = new System.Windows.Forms.OpenFileDialog();
			this.SaveMonsterFile = new System.Windows.Forms.SaveFileDialog();
			this.Save_NPCNametxt_AV = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStrip1.SuspendLayout();
			this.Search_Monster_Panel.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.Goto_Text_Box)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.Monster_List_Box)).BeginInit();
			this.SuspendLayout();
			// 
			// toolStrip1
			// 
			this.toolStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Load_Monster_Menu_Btn,
            this.Save_Monster_Menu_Btn});
			this.toolStrip1.Location = new System.Drawing.Point(0, 0);
			this.toolStrip1.Name = "toolStrip1";
			this.toolStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
			this.toolStrip1.ShowItemToolTips = false;
			this.toolStrip1.Size = new System.Drawing.Size(1264, 25);
			this.toolStrip1.TabIndex = 1;
			this.toolStrip1.Text = "toolStrip1";
			// 
			// Load_Monster_Menu_Btn
			// 
			this.Load_Monster_Menu_Btn.AutoSize = false;
			this.Load_Monster_Menu_Btn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this.Load_Monster_Menu_Btn.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Load_Monstertxt_AV,
            this.Load_NPCNametxt_AV});
			this.Load_Monster_Menu_Btn.Image = ((System.Drawing.Image)(resources.GetObject("Load_Monster_Menu_Btn.Image")));
			this.Load_Monster_Menu_Btn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.Load_Monster_Menu_Btn.Margin = new System.Windows.Forms.Padding(10, 0, 10, 0);
			this.Load_Monster_Menu_Btn.Name = "Load_Monster_Menu_Btn";
			this.Load_Monster_Menu_Btn.Size = new System.Drawing.Size(130, 22);
			this.Load_Monster_Menu_Btn.Text = "LOAD";
			// 
			// Load_Monstertxt_AV
			// 
			this.Load_Monstertxt_AV.Name = "Load_Monstertxt_AV";
			this.Load_Monstertxt_AV.Size = new System.Drawing.Size(180, 22);
			this.Load_Monstertxt_AV.Text = "Load Monster.txt";
			this.Load_Monstertxt_AV.Click += new System.EventHandler(this.Load_Monstertxt_Click);
			// 
			// Load_NPCNametxt_AV
			// 
			this.Load_NPCNametxt_AV.Name = "Load_NPCNametxt_AV";
			this.Load_NPCNametxt_AV.Size = new System.Drawing.Size(180, 22);
			this.Load_NPCNametxt_AV.Text = "Load NPCName.txt";
			this.Load_NPCNametxt_AV.Click += new System.EventHandler(this.Load_NPCNametxt_Click);
			// 
			// Save_Monster_Menu_Btn
			// 
			this.Save_Monster_Menu_Btn.AutoSize = false;
			this.Save_Monster_Menu_Btn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this.Save_Monster_Menu_Btn.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Save_Monstertxt_AV,
            this.Save_NPCNametxt_AV});
			this.Save_Monster_Menu_Btn.Enabled = false;
			this.Save_Monster_Menu_Btn.Image = ((System.Drawing.Image)(resources.GetObject("Save_Monster_Menu_Btn.Image")));
			this.Save_Monster_Menu_Btn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.Save_Monster_Menu_Btn.Margin = new System.Windows.Forms.Padding(10, 0, 10, 0);
			this.Save_Monster_Menu_Btn.Name = "Save_Monster_Menu_Btn";
			this.Save_Monster_Menu_Btn.Size = new System.Drawing.Size(130, 22);
			this.Save_Monster_Menu_Btn.Text = "SAVE";
			// 
			// Save_Monstertxt_AV
			// 
			this.Save_Monstertxt_AV.Name = "Save_Monstertxt_AV";
			this.Save_Monstertxt_AV.Size = new System.Drawing.Size(180, 22);
			this.Save_Monstertxt_AV.Text = "Save Monster.txt";
			this.Save_Monstertxt_AV.Click += new System.EventHandler(this.Save_Monstertxt_Click);
			// 
			// Search_Monster_Panel
			// 
			this.Search_Monster_Panel.BackColor = System.Drawing.Color.Maroon;
			this.Search_Monster_Panel.Controls.Add(this.Search_Box_Text);
			this.Search_Monster_Panel.Controls.Add(this.label1);
			this.Search_Monster_Panel.Controls.Add(this.Search_Box_Btn);
			this.Search_Monster_Panel.Controls.Add(this.Goto_Text_Box);
			this.Search_Monster_Panel.Dock = System.Windows.Forms.DockStyle.Top;
			this.Search_Monster_Panel.Enabled = false;
			this.Search_Monster_Panel.Location = new System.Drawing.Point(0, 25);
			this.Search_Monster_Panel.Name = "Search_Monster_Panel";
			this.Search_Monster_Panel.Size = new System.Drawing.Size(1264, 30);
			this.Search_Monster_Panel.TabIndex = 2;
			// 
			// Search_Box_Text
			// 
			this.Search_Box_Text.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.Search_Box_Text.Location = new System.Drawing.Point(823, 5);
			this.Search_Box_Text.Name = "Search_Box_Text";
			this.Search_Box_Text.Size = new System.Drawing.Size(161, 20);
			this.Search_Box_Text.TabIndex = 3;
			// 
			// label1
			// 
			this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.label1.AutoSize = true;
			this.label1.BackColor = System.Drawing.Color.Transparent;
			this.label1.ForeColor = System.Drawing.Color.White;
			this.label1.Location = new System.Drawing.Point(1071, 7);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(55, 13);
			this.label1.TabIndex = 9;
			this.label1.Text = "Go to line:";
			// 
			// Search_Box_Btn
			// 
			this.Search_Box_Btn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.Search_Box_Btn.Location = new System.Drawing.Point(990, 5);
			this.Search_Box_Btn.Name = "Search_Box_Btn";
			this.Search_Box_Btn.Size = new System.Drawing.Size(75, 20);
			this.Search_Box_Btn.TabIndex = 4;
			this.Search_Box_Btn.Text = "Search";
			this.Search_Box_Btn.UseVisualStyleBackColor = true;
			this.Search_Box_Btn.Click += new System.EventHandler(this.Search_Box_Btn_Click);
			// 
			// Goto_Text_Box
			// 
			this.Goto_Text_Box.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.Goto_Text_Box.Location = new System.Drawing.Point(1132, 5);
			this.Goto_Text_Box.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
			this.Goto_Text_Box.Name = "Goto_Text_Box";
			this.Goto_Text_Box.Size = new System.Drawing.Size(120, 20);
			this.Goto_Text_Box.TabIndex = 8;
			this.Goto_Text_Box.ValueChanged += new System.EventHandler(this.Goto_Text_Box_ValueChanged);
			// 
			// Monster_List_Box
			// 
			this.Monster_List_Box.AllowUserToAddRows = false;
			this.Monster_List_Box.AllowUserToDeleteRows = false;
			this.Monster_List_Box.AllowUserToResizeRows = false;
			this.Monster_List_Box.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.Monster_List_Box.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
			this.Monster_List_Box.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.Monster_List_Box.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.EnableWithoutHeaderText;
			this.Monster_List_Box.ColumnHeadersBorderStyle = System.Windows.Forms.DataGridViewHeaderBorderStyle.Single;
			dataGridViewCellStyle5.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle5.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
			dataGridViewCellStyle5.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			dataGridViewCellStyle5.ForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle5.SelectionBackColor = System.Drawing.Color.DimGray;
			dataGridViewCellStyle5.SelectionForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle5.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
			this.Monster_List_Box.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle5;
			this.Monster_List_Box.ColumnHeadersHeight = 50;
			this.Monster_List_Box.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Row_Col,
            this.Name_Col,
            this.Level_Col,
            this.MaxLife_Col,
            this.MaxMana_Col,
            this.MinDamage_Col,
            this.MaxDamage_Col,
            this.Defense_Col,
            this.MagicDefense_Col,
            this.AttackRate_Col,
            this.DefenseRate_Col,
            this.MoveRange_Col,
            this.AttackType_Col,
            this.AttackRange_Col,
            this.ViewRange_Col,
            this.MoveSpeed_Col,
            this.AttackSpeed_Col,
            this.RegenTime_Col,
            this.Attribute_Col,
            this.ItemRate_Col,
            this.Money_Col,
            this.MaxItemLevel_Col,
            this.MonsterSkill_Col,
            this.ResistanceIce_Col,
            this.ResistancePoison_Col,
            this.ResistanceLightning_Col,
            this.ResistanceFire_Col});
			dataGridViewCellStyle7.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle7.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(50)))), ((int)(((byte)(60)))));
			dataGridViewCellStyle7.Font = new System.Drawing.Font("Consolas", 10F);
			dataGridViewCellStyle7.ForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle7.SelectionBackColor = System.Drawing.Color.LightGray;
			dataGridViewCellStyle7.SelectionForeColor = System.Drawing.Color.Black;
			dataGridViewCellStyle7.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
			this.Monster_List_Box.DefaultCellStyle = dataGridViewCellStyle7;
			this.Monster_List_Box.EnableHeadersVisualStyles = false;
			this.Monster_List_Box.GridColor = System.Drawing.Color.FromArgb(((int)(((byte)(15)))), ((int)(((byte)(15)))), ((int)(((byte)(15)))));
			this.Monster_List_Box.Location = new System.Drawing.Point(0, 53);
			this.Monster_List_Box.Margin = new System.Windows.Forms.Padding(0);
			this.Monster_List_Box.Name = "Monster_List_Box";
			this.Monster_List_Box.RowHeadersBorderStyle = System.Windows.Forms.DataGridViewHeaderBorderStyle.Single;
			dataGridViewCellStyle8.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle8.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
			dataGridViewCellStyle8.ForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle8.SelectionBackColor = System.Drawing.Color.DimGray;
			dataGridViewCellStyle8.SelectionForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle8.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
			this.Monster_List_Box.RowHeadersDefaultCellStyle = dataGridViewCellStyle8;
			this.Monster_List_Box.RowHeadersWidth = 30;
			this.Monster_List_Box.RowTemplate.Resizable = System.Windows.Forms.DataGridViewTriState.False;
			this.Monster_List_Box.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
			this.Monster_List_Box.Size = new System.Drawing.Size(1264, 628);
			this.Monster_List_Box.TabIndex = 3;
			this.Monster_List_Box.Visible = false;
			this.Monster_List_Box.Scroll += new System.Windows.Forms.ScrollEventHandler(this.Text_List_Box_Scroll);
			// 
			// Row_Col
			// 
			this.Row_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			dataGridViewCellStyle6.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle6.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(15)))), ((int)(((byte)(15)))), ((int)(((byte)(15)))));
			dataGridViewCellStyle6.ForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle6.SelectionBackColor = System.Drawing.Color.DimGray;
			dataGridViewCellStyle6.SelectionForeColor = System.Drawing.Color.White;
			this.Row_Col.DefaultCellStyle = dataGridViewCellStyle6;
			this.Row_Col.HeaderText = "Row";
			this.Row_Col.Name = "Row_Col";
			this.Row_Col.ReadOnly = true;
			this.Row_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Row_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Row_Col.ToolTipText = "Number of Row";
			this.Row_Col.Width = 60;
			// 
			// Name_Col
			// 
			this.Name_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Name_Col.HeaderText = "Name";
			this.Name_Col.Name = "Name_Col";
			this.Name_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Name_Col.ToolTipText = "Name of the Monster";
			this.Name_Col.Width = 400;
			// 
			// Level_Col
			// 
			this.Level_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Level_Col.HeaderText = "Level";
			this.Level_Col.Name = "Level_Col";
			this.Level_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Level_Col.Width = 80;
			// 
			// MaxLife_Col
			// 
			this.MaxLife_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.MaxLife_Col.HeaderText = "Max Life";
			this.MaxLife_Col.Name = "MaxLife_Col";
			this.MaxLife_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.MaxLife_Col.Width = 80;
			// 
			// MaxMana_Col
			// 
			this.MaxMana_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.MaxMana_Col.HeaderText = "Max Mana";
			this.MaxMana_Col.Name = "MaxMana_Col";
			this.MaxMana_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.MaxMana_Col.Width = 80;
			// 
			// MinDamage_Col
			// 
			this.MinDamage_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.MinDamage_Col.HeaderText = "Min Damage";
			this.MinDamage_Col.Name = "MinDamage_Col";
			this.MinDamage_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.MinDamage_Col.Width = 80;
			// 
			// MaxDamage_Col
			// 
			this.MaxDamage_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.MaxDamage_Col.HeaderText = "Max Damage";
			this.MaxDamage_Col.Name = "MaxDamage_Col";
			this.MaxDamage_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.MaxDamage_Col.Width = 80;
			// 
			// Defense_Col
			// 
			this.Defense_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Defense_Col.HeaderText = "Defense";
			this.Defense_Col.Name = "Defense_Col";
			this.Defense_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Defense_Col.Width = 80;
			// 
			// MagicDefense_Col
			// 
			this.MagicDefense_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.MagicDefense_Col.HeaderText = "Magic Defense";
			this.MagicDefense_Col.Name = "MagicDefense_Col";
			this.MagicDefense_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.MagicDefense_Col.Width = 80;
			// 
			// AttackRate_Col
			// 
			this.AttackRate_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.AttackRate_Col.HeaderText = "Attack Rate";
			this.AttackRate_Col.Name = "AttackRate_Col";
			this.AttackRate_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.AttackRate_Col.Width = 80;
			// 
			// DefenseRate_Col
			// 
			this.DefenseRate_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.DefenseRate_Col.HeaderText = "Defense Rate";
			this.DefenseRate_Col.Name = "DefenseRate_Col";
			this.DefenseRate_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.DefenseRate_Col.Width = 80;
			// 
			// MoveRange_Col
			// 
			this.MoveRange_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.MoveRange_Col.HeaderText = "Move Range";
			this.MoveRange_Col.Name = "MoveRange_Col";
			this.MoveRange_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.MoveRange_Col.Width = 80;
			// 
			// AttackType_Col
			// 
			this.AttackType_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.AttackType_Col.HeaderText = "Attack Type";
			this.AttackType_Col.Name = "AttackType_Col";
			this.AttackType_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.AttackType_Col.Width = 80;
			// 
			// AttackRange_Col
			// 
			this.AttackRange_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.AttackRange_Col.HeaderText = "Attack Range";
			this.AttackRange_Col.Name = "AttackRange_Col";
			this.AttackRange_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.AttackRange_Col.Width = 80;
			// 
			// ViewRange_Col
			// 
			this.ViewRange_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.ViewRange_Col.HeaderText = "View Range";
			this.ViewRange_Col.Name = "ViewRange_Col";
			this.ViewRange_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.ViewRange_Col.Width = 80;
			// 
			// MoveSpeed_Col
			// 
			this.MoveSpeed_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.MoveSpeed_Col.HeaderText = "Move Speed";
			this.MoveSpeed_Col.Name = "MoveSpeed_Col";
			this.MoveSpeed_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.MoveSpeed_Col.Width = 80;
			// 
			// AttackSpeed_Col
			// 
			this.AttackSpeed_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.AttackSpeed_Col.HeaderText = "Attack Speed";
			this.AttackSpeed_Col.Name = "AttackSpeed_Col";
			this.AttackSpeed_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.AttackSpeed_Col.Width = 80;
			// 
			// RegenTime_Col
			// 
			this.RegenTime_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.RegenTime_Col.HeaderText = "Regen Time";
			this.RegenTime_Col.Name = "RegenTime_Col";
			this.RegenTime_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.RegenTime_Col.Width = 80;
			// 
			// Attribute_Col
			// 
			this.Attribute_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Attribute_Col.HeaderText = "Attribute";
			this.Attribute_Col.Name = "Attribute_Col";
			this.Attribute_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Attribute_Col.Width = 80;
			// 
			// ItemRate_Col
			// 
			this.ItemRate_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.ItemRate_Col.HeaderText = "Item Rate";
			this.ItemRate_Col.Name = "ItemRate_Col";
			this.ItemRate_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.ItemRate_Col.Width = 80;
			// 
			// Money_Col
			// 
			this.Money_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Money_Col.HeaderText = "Money";
			this.Money_Col.Name = "Money_Col";
			this.Money_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Money_Col.Width = 80;
			// 
			// MaxItemLevel_Col
			// 
			this.MaxItemLevel_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.MaxItemLevel_Col.HeaderText = "Max Item Level";
			this.MaxItemLevel_Col.Name = "MaxItemLevel_Col";
			this.MaxItemLevel_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.MaxItemLevel_Col.Width = 80;
			// 
			// MonsterSkill_Col
			// 
			this.MonsterSkill_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.MonsterSkill_Col.HeaderText = "Monster Skill";
			this.MonsterSkill_Col.Name = "MonsterSkill_Col";
			this.MonsterSkill_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.MonsterSkill_Col.Width = 80;
			// 
			// ResistanceIce_Col
			// 
			this.ResistanceIce_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.ResistanceIce_Col.HeaderText = "Resistance Ice";
			this.ResistanceIce_Col.Name = "ResistanceIce_Col";
			this.ResistanceIce_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.ResistanceIce_Col.Width = 80;
			// 
			// ResistancePoison_Col
			// 
			this.ResistancePoison_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.ResistancePoison_Col.HeaderText = "Resistance Poison";
			this.ResistancePoison_Col.Name = "ResistancePoison_Col";
			this.ResistancePoison_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.ResistancePoison_Col.Width = 80;
			// 
			// ResistanceLightning_Col
			// 
			this.ResistanceLightning_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.ResistanceLightning_Col.HeaderText = "Resistance Lightning";
			this.ResistanceLightning_Col.Name = "ResistanceLightning_Col";
			this.ResistanceLightning_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.ResistanceLightning_Col.Width = 80;
			// 
			// ResistanceFire_Col
			// 
			this.ResistanceFire_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.ResistanceFire_Col.HeaderText = "Resistance Fire";
			this.ResistanceFire_Col.Name = "ResistanceFire_Col";
			this.ResistanceFire_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.ResistanceFire_Col.Width = 80;
			// 
			// OpenMonsterFile
			// 
			this.OpenMonsterFile.FileName = "Monster";
			// 
			// Save_NPCNametxt_AV
			// 
			this.Save_NPCNametxt_AV.Name = "Save_NPCNametxt_AV";
			this.Save_NPCNametxt_AV.Size = new System.Drawing.Size(180, 22);
			this.Save_NPCNametxt_AV.Text = "Save NPCName.txt";
			this.Save_NPCNametxt_AV.Click += new System.EventHandler(this.Save_NPCNametxt_Click);
			// 
			// MonsterEditor
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.BackColor = System.Drawing.Color.Black;
			this.ClientSize = new System.Drawing.Size(1264, 681);
			this.Controls.Add(this.Monster_List_Box);
			this.Controls.Add(this.Search_Monster_Panel);
			this.Controls.Add(this.toolStrip1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "MonsterEditor";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "NPCNameEditor";
			this.Load += new System.EventHandler(this.MonsterEditor_Load);
			this.toolStrip1.ResumeLayout(false);
			this.toolStrip1.PerformLayout();
			this.Search_Monster_Panel.ResumeLayout(false);
			this.Search_Monster_Panel.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.Goto_Text_Box)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.Monster_List_Box)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ToolStrip toolStrip1;
		private System.Windows.Forms.ToolStripDropDownButton Load_Monster_Menu_Btn;
		private System.Windows.Forms.ToolStripMenuItem Load_NPCNametxt_AV;
		private System.Windows.Forms.ToolStripDropDownButton Save_Monster_Menu_Btn;
		private System.Windows.Forms.ToolStripMenuItem Save_Monstertxt_AV;
		private System.Windows.Forms.Panel Search_Monster_Panel;
		private System.Windows.Forms.TextBox Search_Box_Text;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Button Search_Box_Btn;
		private System.Windows.Forms.NumericUpDown Goto_Text_Box;
		private System.Windows.Forms.DataGridView Monster_List_Box;
		private System.Windows.Forms.OpenFileDialog OpenMonsterFile;
		private System.Windows.Forms.SaveFileDialog SaveMonsterFile;
		private System.Windows.Forms.ToolStripMenuItem Load_Monstertxt_AV;
		private System.Windows.Forms.DataGridViewTextBoxColumn Row_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Name_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Level_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn MaxLife_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn MaxMana_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn MinDamage_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn MaxDamage_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Defense_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn MagicDefense_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn AttackRate_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn DefenseRate_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn MoveRange_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn AttackType_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn AttackRange_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn ViewRange_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn MoveSpeed_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn AttackSpeed_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn RegenTime_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Attribute_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn ItemRate_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Money_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn MaxItemLevel_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn MonsterSkill_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn ResistanceIce_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn ResistancePoison_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn ResistanceLightning_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn ResistanceFire_Col;
		private System.Windows.Forms.ToolStripMenuItem Save_NPCNametxt_AV;
	}
}