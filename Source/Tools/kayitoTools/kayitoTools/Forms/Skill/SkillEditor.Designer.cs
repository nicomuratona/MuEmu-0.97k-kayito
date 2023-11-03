
namespace kayitoTools.Forms
{
	partial class SkillEditor
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SkillEditor));
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
			this.toolStrip1 = new System.Windows.Forms.ToolStrip();
			this.Load_Skill_Menu_Btn = new System.Windows.Forms.ToolStripDropDownButton();
			this.loadSkilltxtToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.loadSkillbmdToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.Save_Skill_Menu_Btn = new System.Windows.Forms.ToolStripDropDownButton();
			this.saveSkilltxtToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.saveSkillbmdToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.Skill_List_Box = new System.Windows.Forms.DataGridView();
			this.OpenSkillFile = new System.Windows.Forms.OpenFileDialog();
			this.SaveSkillFile = new System.Windows.Forms.SaveFileDialog();
			this.Index_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Name_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Damage_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Mana_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Stamina_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Range_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Radio_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Delay_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Attribute_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Effect_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.RequireLevel_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.RequireEnergy_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.RequireDW_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.RequireDK_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.RequireELF_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.RequireMG_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.toolStrip1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.Skill_List_Box)).BeginInit();
			this.SuspendLayout();
			// 
			// toolStrip1
			// 
			this.toolStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Load_Skill_Menu_Btn,
            this.Save_Skill_Menu_Btn});
			this.toolStrip1.Location = new System.Drawing.Point(0, 0);
			this.toolStrip1.Name = "toolStrip1";
			this.toolStrip1.Padding = new System.Windows.Forms.Padding(0);
			this.toolStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
			this.toolStrip1.ShowItemToolTips = false;
			this.toolStrip1.Size = new System.Drawing.Size(1264, 25);
			this.toolStrip1.TabIndex = 1;
			this.toolStrip1.Text = "toolStrip1";
			// 
			// Load_Skill_Menu_Btn
			// 
			this.Load_Skill_Menu_Btn.AutoSize = false;
			this.Load_Skill_Menu_Btn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this.Load_Skill_Menu_Btn.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadSkilltxtToolStripMenuItem,
            this.loadSkillbmdToolStripMenuItem});
			this.Load_Skill_Menu_Btn.Image = ((System.Drawing.Image)(resources.GetObject("Load_Skill_Menu_Btn.Image")));
			this.Load_Skill_Menu_Btn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.Load_Skill_Menu_Btn.Margin = new System.Windows.Forms.Padding(10, 0, 10, 0);
			this.Load_Skill_Menu_Btn.Name = "Load_Skill_Menu_Btn";
			this.Load_Skill_Menu_Btn.Size = new System.Drawing.Size(130, 22);
			this.Load_Skill_Menu_Btn.Text = "LOAD";
			// 
			// loadSkilltxtToolStripMenuItem
			// 
			this.loadSkilltxtToolStripMenuItem.Name = "loadSkilltxtToolStripMenuItem";
			this.loadSkilltxtToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.loadSkilltxtToolStripMenuItem.Text = "Load Skill.txt";
			this.loadSkilltxtToolStripMenuItem.Click += new System.EventHandler(this.LoadSkilltxt_Click);
			// 
			// loadSkillbmdToolStripMenuItem
			// 
			this.loadSkillbmdToolStripMenuItem.Name = "loadSkillbmdToolStripMenuItem";
			this.loadSkillbmdToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.loadSkillbmdToolStripMenuItem.Text = "Load Skill.bmd";
			this.loadSkillbmdToolStripMenuItem.Click += new System.EventHandler(this.LoadSkillbmd_Click);
			// 
			// Save_Skill_Menu_Btn
			// 
			this.Save_Skill_Menu_Btn.AutoSize = false;
			this.Save_Skill_Menu_Btn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this.Save_Skill_Menu_Btn.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.saveSkilltxtToolStripMenuItem,
            this.saveSkillbmdToolStripMenuItem});
			this.Save_Skill_Menu_Btn.Enabled = false;
			this.Save_Skill_Menu_Btn.Image = ((System.Drawing.Image)(resources.GetObject("Save_Skill_Menu_Btn.Image")));
			this.Save_Skill_Menu_Btn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.Save_Skill_Menu_Btn.Margin = new System.Windows.Forms.Padding(10, 0, 10, 0);
			this.Save_Skill_Menu_Btn.Name = "Save_Skill_Menu_Btn";
			this.Save_Skill_Menu_Btn.Size = new System.Drawing.Size(130, 22);
			this.Save_Skill_Menu_Btn.Text = "SAVE";
			// 
			// saveSkilltxtToolStripMenuItem
			// 
			this.saveSkilltxtToolStripMenuItem.Name = "saveSkilltxtToolStripMenuItem";
			this.saveSkilltxtToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
			this.saveSkilltxtToolStripMenuItem.Text = "Save Skill.txt";
			this.saveSkilltxtToolStripMenuItem.Click += new System.EventHandler(this.SaveSkilltxt_Click);
			// 
			// saveSkillbmdToolStripMenuItem
			// 
			this.saveSkillbmdToolStripMenuItem.Name = "saveSkillbmdToolStripMenuItem";
			this.saveSkillbmdToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
			this.saveSkillbmdToolStripMenuItem.Text = "Save Skill.bmd";
			this.saveSkillbmdToolStripMenuItem.Click += new System.EventHandler(this.SaveSkillbmd_Click);
			// 
			// Skill_List_Box
			// 
			this.Skill_List_Box.AllowUserToAddRows = false;
			this.Skill_List_Box.AllowUserToDeleteRows = false;
			this.Skill_List_Box.AllowUserToResizeRows = false;
			this.Skill_List_Box.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
			this.Skill_List_Box.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.Skill_List_Box.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.EnableWithoutHeaderText;
			this.Skill_List_Box.ColumnHeadersBorderStyle = System.Windows.Forms.DataGridViewHeaderBorderStyle.Single;
			dataGridViewCellStyle1.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
			dataGridViewCellStyle1.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			dataGridViewCellStyle1.ForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle1.SelectionBackColor = System.Drawing.Color.DimGray;
			dataGridViewCellStyle1.SelectionForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle1.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
			this.Skill_List_Box.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle1;
			this.Skill_List_Box.ColumnHeadersHeight = 60;
			this.Skill_List_Box.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Index_Col,
            this.Name_Col,
            this.Damage_Col,
            this.Mana_Col,
            this.Stamina_Col,
            this.Range_Col,
            this.Radio_Col,
            this.Delay_Col,
            this.Attribute_Col,
            this.Effect_Col,
            this.RequireLevel_Col,
            this.RequireEnergy_Col,
            this.RequireDW_Col,
            this.RequireDK_Col,
            this.RequireELF_Col,
            this.RequireMG_Col});
			dataGridViewCellStyle2.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(50)))), ((int)(((byte)(60)))));
			dataGridViewCellStyle2.Font = new System.Drawing.Font("Consolas", 10F);
			dataGridViewCellStyle2.ForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle2.SelectionBackColor = System.Drawing.Color.LightGray;
			dataGridViewCellStyle2.SelectionForeColor = System.Drawing.Color.Black;
			dataGridViewCellStyle2.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
			this.Skill_List_Box.DefaultCellStyle = dataGridViewCellStyle2;
			this.Skill_List_Box.Dock = System.Windows.Forms.DockStyle.Fill;
			this.Skill_List_Box.EnableHeadersVisualStyles = false;
			this.Skill_List_Box.GridColor = System.Drawing.Color.FromArgb(((int)(((byte)(15)))), ((int)(((byte)(15)))), ((int)(((byte)(15)))));
			this.Skill_List_Box.Location = new System.Drawing.Point(0, 25);
			this.Skill_List_Box.Margin = new System.Windows.Forms.Padding(0);
			this.Skill_List_Box.Name = "Skill_List_Box";
			this.Skill_List_Box.RowHeadersBorderStyle = System.Windows.Forms.DataGridViewHeaderBorderStyle.Single;
			dataGridViewCellStyle3.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle3.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
			dataGridViewCellStyle3.ForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle3.SelectionBackColor = System.Drawing.Color.DimGray;
			dataGridViewCellStyle3.SelectionForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle3.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
			this.Skill_List_Box.RowHeadersDefaultCellStyle = dataGridViewCellStyle3;
			this.Skill_List_Box.RowHeadersWidth = 30;
			this.Skill_List_Box.RowTemplate.Resizable = System.Windows.Forms.DataGridViewTriState.False;
			this.Skill_List_Box.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
			this.Skill_List_Box.Size = new System.Drawing.Size(1264, 656);
			this.Skill_List_Box.TabIndex = 2;
			this.Skill_List_Box.Visible = false;
			this.Skill_List_Box.CellValidating += new System.Windows.Forms.DataGridViewCellValidatingEventHandler(this.Skill_List_Box_CellValidating);
			this.Skill_List_Box.CellValueChanged += new System.Windows.Forms.DataGridViewCellEventHandler(this.Skill_List_Box_CellValueChanged);
			// 
			// OpenSkillFile
			// 
			this.OpenSkillFile.FileName = "Skill";
			this.OpenSkillFile.Filter = "Every file (*.*)|*.*";
			// 
			// Index_Col
			// 
			this.Index_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Index_Col.HeaderText = "Index";
			this.Index_Col.Name = "Index_Col";
			this.Index_Col.ReadOnly = true;
			this.Index_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Index_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Index_Col.ToolTipText = "Number of the Skill";
			this.Index_Col.Width = 60;
			// 
			// Name_Col
			// 
			this.Name_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Name_Col.HeaderText = "Name";
			this.Name_Col.Name = "Name_Col";
			this.Name_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Name_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Name_Col.ToolTipText = "Name of the Skill";
			this.Name_Col.Width = 300;
			// 
			// Damage_Col
			// 
			this.Damage_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Damage_Col.HeaderText = "Damage";
			this.Damage_Col.Name = "Damage_Col";
			this.Damage_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Damage_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Damage_Col.ToolTipText = "Damage of the Skill";
			this.Damage_Col.Width = 80;
			// 
			// Mana_Col
			// 
			this.Mana_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Mana_Col.HeaderText = "MP";
			this.Mana_Col.Name = "Mana_Col";
			this.Mana_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Mana_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Mana_Col.ToolTipText = "Mana used by the Skill";
			this.Mana_Col.Width = 80;
			// 
			// Stamina_Col
			// 
			this.Stamina_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Stamina_Col.HeaderText = "AG";
			this.Stamina_Col.Name = "Stamina_Col";
			this.Stamina_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Stamina_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Stamina_Col.ToolTipText = "Stamina used by the Skill";
			this.Stamina_Col.Width = 80;
			// 
			// Range_Col
			// 
			this.Range_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Range_Col.HeaderText = "Range";
			this.Range_Col.Name = "Range_Col";
			this.Range_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Range_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Range_Col.ToolTipText = "Distance Range of the Skill";
			this.Range_Col.Width = 80;
			// 
			// Radio_Col
			// 
			this.Radio_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Radio_Col.HeaderText = "Radio";
			this.Radio_Col.Name = "Radio_Col";
			this.Radio_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Radio_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Radio_Col.ToolTipText = "Radio of the Skill";
			this.Radio_Col.Width = 80;
			// 
			// Delay_Col
			// 
			this.Delay_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Delay_Col.HeaderText = "Delay";
			this.Delay_Col.Name = "Delay_Col";
			this.Delay_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Delay_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Delay_Col.ToolTipText = "Delay to use again the Skill";
			this.Delay_Col.Width = 80;
			// 
			// Attribute_Col
			// 
			this.Attribute_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Attribute_Col.HeaderText = "Attribute";
			this.Attribute_Col.Name = "Attribute_Col";
			this.Attribute_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Attribute_Col.ToolTipText = "-1 = None / 0 = Ice / 1 = Poison / 2 = Lightning / 3 = Fire / 4 = Earth / 5 = Win" +
    "d / 6 = Water";
			this.Attribute_Col.Width = 80;
			// 
			// Effect_Col
			// 
			this.Effect_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Effect_Col.HeaderText = "Effect";
			this.Effect_Col.Name = "Effect_Col";
			this.Effect_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Effect_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Effect_Col.ToolTipText = "Effect number of the Skill";
			this.Effect_Col.Width = 80;
			// 
			// RequireLevel_Col
			// 
			this.RequireLevel_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.RequireLevel_Col.HeaderText = "Require Level";
			this.RequireLevel_Col.Name = "RequireLevel_Col";
			this.RequireLevel_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.RequireLevel_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.RequireLevel_Col.ToolTipText = "Require Level to learn the Skill";
			this.RequireLevel_Col.Width = 80;
			// 
			// RequireEnergy_Col
			// 
			this.RequireEnergy_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.RequireEnergy_Col.HeaderText = "Require Energy";
			this.RequireEnergy_Col.Name = "RequireEnergy_Col";
			this.RequireEnergy_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.RequireEnergy_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.RequireEnergy_Col.ToolTipText = "Require Energy to learn the Skill";
			this.RequireEnergy_Col.Width = 80;
			// 
			// RequireDW_Col
			// 
			this.RequireDW_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.RequireDW_Col.HeaderText = "Require DW";
			this.RequireDW_Col.Name = "RequireDW_Col";
			this.RequireDW_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.RequireDW_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.RequireDW_Col.ToolTipText = "Required level of evolution for Dark Wizard to use the Skill (0 = Disabled for th" +
    "is class)";
			this.RequireDW_Col.Width = 80;
			// 
			// RequireDK_Col
			// 
			this.RequireDK_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.RequireDK_Col.HeaderText = "Require DK";
			this.RequireDK_Col.Name = "RequireDK_Col";
			this.RequireDK_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.RequireDK_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.RequireDK_Col.ToolTipText = "Required level of evolution for Dark Knight to use the Skill (0 = Disabled for th" +
    "is class)";
			this.RequireDK_Col.Width = 80;
			// 
			// RequireELF_Col
			// 
			this.RequireELF_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.RequireELF_Col.HeaderText = "Require ELF";
			this.RequireELF_Col.Name = "RequireELF_Col";
			this.RequireELF_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.RequireELF_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.RequireELF_Col.ToolTipText = "Required level of evolution for Fairy Elf to use the Skill (0 = Disabled for this" +
    " class)";
			this.RequireELF_Col.Width = 80;
			// 
			// RequireMG_Col
			// 
			this.RequireMG_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.RequireMG_Col.HeaderText = "Require MG";
			this.RequireMG_Col.Name = "RequireMG_Col";
			this.RequireMG_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.RequireMG_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.RequireMG_Col.ToolTipText = "Required level of evolution for Magic Gladiator to use the Skill (0 = Disabled fo" +
    "r this class)";
			this.RequireMG_Col.Width = 80;
			// 
			// SkillEditor
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.BackColor = System.Drawing.Color.Black;
			this.ClientSize = new System.Drawing.Size(1264, 681);
			this.Controls.Add(this.Skill_List_Box);
			this.Controls.Add(this.toolStrip1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "SkillEditor";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Skill Manager";
			this.Load += new System.EventHandler(this.SkillEditor_Load);
			this.toolStrip1.ResumeLayout(false);
			this.toolStrip1.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.Skill_List_Box)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ToolStrip toolStrip1;
		private System.Windows.Forms.ToolStripDropDownButton Load_Skill_Menu_Btn;
		private System.Windows.Forms.ToolStripDropDownButton Save_Skill_Menu_Btn;
		private System.Windows.Forms.DataGridView Skill_List_Box;
		private System.Windows.Forms.ToolStripMenuItem loadSkilltxtToolStripMenuItem;
		private System.Windows.Forms.OpenFileDialog OpenSkillFile;
		private System.Windows.Forms.SaveFileDialog SaveSkillFile;
		private System.Windows.Forms.ToolStripMenuItem loadSkillbmdToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem saveSkilltxtToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem saveSkillbmdToolStripMenuItem;
		private System.Windows.Forms.DataGridViewTextBoxColumn Index_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Name_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Damage_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Mana_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Stamina_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Range_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Radio_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Delay_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Attribute_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Effect_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn RequireLevel_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn RequireEnergy_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn RequireDW_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn RequireDK_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn RequireELF_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn RequireMG_Col;
	}
}