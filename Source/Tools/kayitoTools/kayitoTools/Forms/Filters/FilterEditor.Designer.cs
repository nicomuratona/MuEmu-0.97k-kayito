
namespace kayitoTools.Forms
{
	partial class FilterEditor
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FilterEditor));
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle4 = new System.Windows.Forms.DataGridViewCellStyle();
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
			this.toolStrip1 = new System.Windows.Forms.ToolStrip();
			this.Load_Filter_Menu_Btn = new System.Windows.Forms.ToolStripDropDownButton();
			this.loadFilterbmdToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.loadFilterNamebmdToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.loadFiltertxtToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.Save_Filter_Menu_Btn = new System.Windows.Forms.ToolStripDropDownButton();
			this.saveFilterbmdToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.saveFilterNamebmdToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.saveFiltertxtToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.Filter_List_Box = new System.Windows.Forms.DataGridView();
			this.Row_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Text_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.OpenFilterFile = new System.Windows.Forms.OpenFileDialog();
			this.SaveFilterFile = new System.Windows.Forms.SaveFileDialog();
			this.Search_Text_Panel = new System.Windows.Forms.Panel();
			this.Search_Box_Text = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.Search_Box_Btn = new System.Windows.Forms.Button();
			this.Goto_Text_Box = new System.Windows.Forms.NumericUpDown();
			this.toolStrip1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.Filter_List_Box)).BeginInit();
			this.Search_Text_Panel.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.Goto_Text_Box)).BeginInit();
			this.SuspendLayout();
			// 
			// toolStrip1
			// 
			this.toolStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Load_Filter_Menu_Btn,
            this.Save_Filter_Menu_Btn});
			this.toolStrip1.Location = new System.Drawing.Point(0, 0);
			this.toolStrip1.Name = "toolStrip1";
			this.toolStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
			this.toolStrip1.ShowItemToolTips = false;
			this.toolStrip1.Size = new System.Drawing.Size(1264, 25);
			this.toolStrip1.TabIndex = 1;
			this.toolStrip1.Text = "toolStrip1";
			// 
			// Load_Filter_Menu_Btn
			// 
			this.Load_Filter_Menu_Btn.AutoSize = false;
			this.Load_Filter_Menu_Btn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this.Load_Filter_Menu_Btn.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadFilterbmdToolStripMenuItem,
            this.loadFilterNamebmdToolStripMenuItem,
            this.loadFiltertxtToolStripMenuItem});
			this.Load_Filter_Menu_Btn.Image = ((System.Drawing.Image)(resources.GetObject("Load_Filter_Menu_Btn.Image")));
			this.Load_Filter_Menu_Btn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.Load_Filter_Menu_Btn.Margin = new System.Windows.Forms.Padding(10, 0, 10, 0);
			this.Load_Filter_Menu_Btn.Name = "Load_Filter_Menu_Btn";
			this.Load_Filter_Menu_Btn.Size = new System.Drawing.Size(130, 22);
			this.Load_Filter_Menu_Btn.Text = "LOAD";
			// 
			// loadFilterbmdToolStripMenuItem
			// 
			this.loadFilterbmdToolStripMenuItem.Name = "loadFilterbmdToolStripMenuItem";
			this.loadFilterbmdToolStripMenuItem.Size = new System.Drawing.Size(189, 22);
			this.loadFilterbmdToolStripMenuItem.Text = "Load Filter.bmd";
			this.loadFilterbmdToolStripMenuItem.Click += new System.EventHandler(this.Load_Filterbmd_Click);
			// 
			// loadFilterNamebmdToolStripMenuItem
			// 
			this.loadFilterNamebmdToolStripMenuItem.Name = "loadFilterNamebmdToolStripMenuItem";
			this.loadFilterNamebmdToolStripMenuItem.Size = new System.Drawing.Size(189, 22);
			this.loadFilterNamebmdToolStripMenuItem.Text = "Load FilterName.bmd";
			this.loadFilterNamebmdToolStripMenuItem.Click += new System.EventHandler(this.Load_FilterNamebmd_Click);
			// 
			// loadFiltertxtToolStripMenuItem
			// 
			this.loadFiltertxtToolStripMenuItem.Name = "loadFiltertxtToolStripMenuItem";
			this.loadFiltertxtToolStripMenuItem.Size = new System.Drawing.Size(189, 22);
			this.loadFiltertxtToolStripMenuItem.Text = "Load Filter.txt";
			this.loadFiltertxtToolStripMenuItem.Click += new System.EventHandler(this.Load_Filtertxt_Click);
			// 
			// Save_Filter_Menu_Btn
			// 
			this.Save_Filter_Menu_Btn.AutoSize = false;
			this.Save_Filter_Menu_Btn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this.Save_Filter_Menu_Btn.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.saveFilterbmdToolStripMenuItem,
            this.saveFilterNamebmdToolStripMenuItem,
            this.saveFiltertxtToolStripMenuItem});
			this.Save_Filter_Menu_Btn.Enabled = false;
			this.Save_Filter_Menu_Btn.Image = ((System.Drawing.Image)(resources.GetObject("Save_Filter_Menu_Btn.Image")));
			this.Save_Filter_Menu_Btn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.Save_Filter_Menu_Btn.Margin = new System.Windows.Forms.Padding(10, 0, 10, 0);
			this.Save_Filter_Menu_Btn.Name = "Save_Filter_Menu_Btn";
			this.Save_Filter_Menu_Btn.Size = new System.Drawing.Size(130, 22);
			this.Save_Filter_Menu_Btn.Text = "SAVE";
			// 
			// saveFilterbmdToolStripMenuItem
			// 
			this.saveFilterbmdToolStripMenuItem.Name = "saveFilterbmdToolStripMenuItem";
			this.saveFilterbmdToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
			this.saveFilterbmdToolStripMenuItem.Text = "Save Filter.bmd";
			this.saveFilterbmdToolStripMenuItem.Click += new System.EventHandler(this.SaveFilterbmd_Click);
			// 
			// saveFilterNamebmdToolStripMenuItem
			// 
			this.saveFilterNamebmdToolStripMenuItem.Name = "saveFilterNamebmdToolStripMenuItem";
			this.saveFilterNamebmdToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
			this.saveFilterNamebmdToolStripMenuItem.Text = "Save FilterName.bmd";
			this.saveFilterNamebmdToolStripMenuItem.Click += new System.EventHandler(this.SaveFilterNamebmd_Click);
			// 
			// saveFiltertxtToolStripMenuItem
			// 
			this.saveFiltertxtToolStripMenuItem.Name = "saveFiltertxtToolStripMenuItem";
			this.saveFiltertxtToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
			this.saveFiltertxtToolStripMenuItem.Text = "Save Filter.txt";
			this.saveFiltertxtToolStripMenuItem.Click += new System.EventHandler(this.SaveFiltertxt_Click);
			// 
			// Filter_List_Box
			// 
			this.Filter_List_Box.AllowUserToAddRows = false;
			this.Filter_List_Box.AllowUserToDeleteRows = false;
			this.Filter_List_Box.AllowUserToResizeRows = false;
			this.Filter_List_Box.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.Filter_List_Box.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
			this.Filter_List_Box.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.Filter_List_Box.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.EnableWithoutHeaderText;
			this.Filter_List_Box.ColumnHeadersBorderStyle = System.Windows.Forms.DataGridViewHeaderBorderStyle.Single;
			dataGridViewCellStyle1.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
			dataGridViewCellStyle1.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			dataGridViewCellStyle1.ForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle1.SelectionBackColor = System.Drawing.Color.DimGray;
			dataGridViewCellStyle1.SelectionForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle1.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
			this.Filter_List_Box.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle1;
			this.Filter_List_Box.ColumnHeadersHeight = 50;
			this.Filter_List_Box.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Row_Col,
            this.Text_Col});
			dataGridViewCellStyle3.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle3.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(50)))), ((int)(((byte)(60)))));
			dataGridViewCellStyle3.Font = new System.Drawing.Font("Consolas", 10F);
			dataGridViewCellStyle3.ForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle3.SelectionBackColor = System.Drawing.Color.LightGray;
			dataGridViewCellStyle3.SelectionForeColor = System.Drawing.Color.Black;
			dataGridViewCellStyle3.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
			this.Filter_List_Box.DefaultCellStyle = dataGridViewCellStyle3;
			this.Filter_List_Box.EnableHeadersVisualStyles = false;
			this.Filter_List_Box.GridColor = System.Drawing.Color.FromArgb(((int)(((byte)(15)))), ((int)(((byte)(15)))), ((int)(((byte)(15)))));
			this.Filter_List_Box.Location = new System.Drawing.Point(0, 53);
			this.Filter_List_Box.Margin = new System.Windows.Forms.Padding(0);
			this.Filter_List_Box.Name = "Filter_List_Box";
			this.Filter_List_Box.RowHeadersBorderStyle = System.Windows.Forms.DataGridViewHeaderBorderStyle.Single;
			dataGridViewCellStyle4.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle4.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
			dataGridViewCellStyle4.ForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle4.SelectionBackColor = System.Drawing.Color.DimGray;
			dataGridViewCellStyle4.SelectionForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle4.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
			this.Filter_List_Box.RowHeadersDefaultCellStyle = dataGridViewCellStyle4;
			this.Filter_List_Box.RowHeadersWidth = 30;
			this.Filter_List_Box.RowTemplate.Resizable = System.Windows.Forms.DataGridViewTriState.False;
			this.Filter_List_Box.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
			this.Filter_List_Box.Size = new System.Drawing.Size(1264, 628);
			this.Filter_List_Box.TabIndex = 3;
			this.Filter_List_Box.Visible = false;
			this.Filter_List_Box.Scroll += new System.Windows.Forms.ScrollEventHandler(this.Filter_List_Box_Scroll);
			// 
			// Row_Col
			// 
			this.Row_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			dataGridViewCellStyle2.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(15)))), ((int)(((byte)(15)))), ((int)(((byte)(15)))));
			dataGridViewCellStyle2.ForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle2.SelectionBackColor = System.Drawing.Color.DimGray;
			dataGridViewCellStyle2.SelectionForeColor = System.Drawing.Color.White;
			this.Row_Col.DefaultCellStyle = dataGridViewCellStyle2;
			this.Row_Col.HeaderText = "Row";
			this.Row_Col.Name = "Row_Col";
			this.Row_Col.ReadOnly = true;
			this.Row_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Row_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Row_Col.ToolTipText = "Number of Row";
			this.Row_Col.Width = 60;
			// 
			// Text_Col
			// 
			this.Text_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Text_Col.HeaderText = "Text";
			this.Text_Col.Name = "Text_Col";
			this.Text_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Text_Col.ToolTipText = "Message that the game will block";
			this.Text_Col.Width = 300;
			// 
			// OpenFilterFile
			// 
			this.OpenFilterFile.FileName = "Filter";
			// 
			// Search_Text_Panel
			// 
			this.Search_Text_Panel.BackColor = System.Drawing.Color.Maroon;
			this.Search_Text_Panel.Controls.Add(this.Search_Box_Text);
			this.Search_Text_Panel.Controls.Add(this.label1);
			this.Search_Text_Panel.Controls.Add(this.Search_Box_Btn);
			this.Search_Text_Panel.Controls.Add(this.Goto_Text_Box);
			this.Search_Text_Panel.Dock = System.Windows.Forms.DockStyle.Top;
			this.Search_Text_Panel.Enabled = false;
			this.Search_Text_Panel.Location = new System.Drawing.Point(0, 25);
			this.Search_Text_Panel.Name = "Search_Text_Panel";
			this.Search_Text_Panel.Size = new System.Drawing.Size(1264, 30);
			this.Search_Text_Panel.TabIndex = 4;
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
			// FilterEditor
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.BackColor = System.Drawing.Color.Black;
			this.ClientSize = new System.Drawing.Size(1264, 681);
			this.Controls.Add(this.Search_Text_Panel);
			this.Controls.Add(this.Filter_List_Box);
			this.Controls.Add(this.toolStrip1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "FilterEditor";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Filter Manager";
			this.Load += new System.EventHandler(this.FilterEditor_Load);
			this.toolStrip1.ResumeLayout(false);
			this.toolStrip1.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.Filter_List_Box)).EndInit();
			this.Search_Text_Panel.ResumeLayout(false);
			this.Search_Text_Panel.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.Goto_Text_Box)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ToolStrip toolStrip1;
		private System.Windows.Forms.ToolStripDropDownButton Load_Filter_Menu_Btn;
		private System.Windows.Forms.ToolStripDropDownButton Save_Filter_Menu_Btn;
		private System.Windows.Forms.DataGridView Filter_List_Box;
		private System.Windows.Forms.OpenFileDialog OpenFilterFile;
		private System.Windows.Forms.SaveFileDialog SaveFilterFile;
		private System.Windows.Forms.Panel Search_Text_Panel;
		private System.Windows.Forms.TextBox Search_Box_Text;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Button Search_Box_Btn;
		private System.Windows.Forms.NumericUpDown Goto_Text_Box;
		private System.Windows.Forms.ToolStripMenuItem loadFilterbmdToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem loadFilterNamebmdToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem saveFilterbmdToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem saveFilterNamebmdToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem loadFiltertxtToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem saveFiltertxtToolStripMenuItem;
		private System.Windows.Forms.DataGridViewTextBoxColumn Row_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Text_Col;
	}
}