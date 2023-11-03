
namespace kayitoTools.Forms
{
	partial class GateEditor
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(GateEditor));
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
			this.toolStrip1 = new System.Windows.Forms.ToolStrip();
			this.Load_Gate_Menu_Btn = new System.Windows.Forms.ToolStripDropDownButton();
			this.gatetxtToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.gatebmdToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.Save_Gate_Menu_Btn = new System.Windows.Forms.ToolStripDropDownButton();
			this.gatetxtToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
			this.gatebmdToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
			this.Gate_List_Box = new System.Windows.Forms.DataGridView();
			this.Index_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Flag_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Map_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.StartX_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.StartY_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.EndX_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.EndY_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Target_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Direction_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.MinLevel_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.MaxLevel_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.MinReset_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.MaxReset_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.AccountLevel_Col = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.OpenGateFile = new System.Windows.Forms.OpenFileDialog();
			this.SaveGateFile = new System.Windows.Forms.SaveFileDialog();
			this.toolStrip1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.Gate_List_Box)).BeginInit();
			this.SuspendLayout();
			// 
			// toolStrip1
			// 
			this.toolStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Load_Gate_Menu_Btn,
            this.Save_Gate_Menu_Btn});
			this.toolStrip1.Location = new System.Drawing.Point(0, 0);
			this.toolStrip1.Name = "toolStrip1";
			this.toolStrip1.Padding = new System.Windows.Forms.Padding(0);
			this.toolStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
			this.toolStrip1.ShowItemToolTips = false;
			this.toolStrip1.Size = new System.Drawing.Size(1264, 25);
			this.toolStrip1.TabIndex = 2;
			this.toolStrip1.Text = "toolStrip1";
			// 
			// Load_Gate_Menu_Btn
			// 
			this.Load_Gate_Menu_Btn.AutoSize = false;
			this.Load_Gate_Menu_Btn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this.Load_Gate_Menu_Btn.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.gatetxtToolStripMenuItem,
            this.gatebmdToolStripMenuItem});
			this.Load_Gate_Menu_Btn.Image = ((System.Drawing.Image)(resources.GetObject("Load_Gate_Menu_Btn.Image")));
			this.Load_Gate_Menu_Btn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.Load_Gate_Menu_Btn.Margin = new System.Windows.Forms.Padding(10, 0, 10, 0);
			this.Load_Gate_Menu_Btn.Name = "Load_Gate_Menu_Btn";
			this.Load_Gate_Menu_Btn.Size = new System.Drawing.Size(130, 22);
			this.Load_Gate_Menu_Btn.Text = "LOAD";
			// 
			// gatetxtToolStripMenuItem
			// 
			this.gatetxtToolStripMenuItem.Name = "gatetxtToolStripMenuItem";
			this.gatetxtToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
			this.gatetxtToolStripMenuItem.Text = "Load Gate.txt";
			this.gatetxtToolStripMenuItem.Click += new System.EventHandler(this.LoadGatetxt_Click);
			// 
			// gatebmdToolStripMenuItem
			// 
			this.gatebmdToolStripMenuItem.Name = "gatebmdToolStripMenuItem";
			this.gatebmdToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
			this.gatebmdToolStripMenuItem.Text = "Load Gate.bmd";
			this.gatebmdToolStripMenuItem.Click += new System.EventHandler(this.LoadGatebmd_Click);
			// 
			// Save_Gate_Menu_Btn
			// 
			this.Save_Gate_Menu_Btn.AutoSize = false;
			this.Save_Gate_Menu_Btn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this.Save_Gate_Menu_Btn.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.gatetxtToolStripMenuItem1,
            this.gatebmdToolStripMenuItem1});
			this.Save_Gate_Menu_Btn.Enabled = false;
			this.Save_Gate_Menu_Btn.Image = ((System.Drawing.Image)(resources.GetObject("Save_Gate_Menu_Btn.Image")));
			this.Save_Gate_Menu_Btn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.Save_Gate_Menu_Btn.Margin = new System.Windows.Forms.Padding(10, 0, 10, 0);
			this.Save_Gate_Menu_Btn.Name = "Save_Gate_Menu_Btn";
			this.Save_Gate_Menu_Btn.Size = new System.Drawing.Size(130, 22);
			this.Save_Gate_Menu_Btn.Text = "SAVE";
			// 
			// gatetxtToolStripMenuItem1
			// 
			this.gatetxtToolStripMenuItem1.Name = "gatetxtToolStripMenuItem1";
			this.gatetxtToolStripMenuItem1.Size = new System.Drawing.Size(153, 22);
			this.gatetxtToolStripMenuItem1.Text = "Save Gate.txt";
			this.gatetxtToolStripMenuItem1.Click += new System.EventHandler(this.SaveGatetxt_Click);
			// 
			// gatebmdToolStripMenuItem1
			// 
			this.gatebmdToolStripMenuItem1.Name = "gatebmdToolStripMenuItem1";
			this.gatebmdToolStripMenuItem1.Size = new System.Drawing.Size(153, 22);
			this.gatebmdToolStripMenuItem1.Text = "Save Gate.bmd";
			this.gatebmdToolStripMenuItem1.Click += new System.EventHandler(this.SaveGatebmd_Click);
			// 
			// Gate_List_Box
			// 
			this.Gate_List_Box.AllowUserToAddRows = false;
			this.Gate_List_Box.AllowUserToDeleteRows = false;
			this.Gate_List_Box.AllowUserToResizeRows = false;
			this.Gate_List_Box.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
			this.Gate_List_Box.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.Gate_List_Box.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.EnableWithoutHeaderText;
			this.Gate_List_Box.ColumnHeadersBorderStyle = System.Windows.Forms.DataGridViewHeaderBorderStyle.Single;
			dataGridViewCellStyle1.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
			dataGridViewCellStyle1.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			dataGridViewCellStyle1.ForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle1.SelectionBackColor = System.Drawing.Color.DimGray;
			dataGridViewCellStyle1.SelectionForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle1.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
			this.Gate_List_Box.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle1;
			this.Gate_List_Box.ColumnHeadersHeight = 60;
			this.Gate_List_Box.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Index_Col,
            this.Flag_Col,
            this.Map_Col,
            this.StartX_Col,
            this.StartY_Col,
            this.EndX_Col,
            this.EndY_Col,
            this.Target_Col,
            this.Direction_Col,
            this.MinLevel_Col,
            this.MaxLevel_Col,
            this.MinReset_Col,
            this.MaxReset_Col,
            this.AccountLevel_Col});
			dataGridViewCellStyle2.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(50)))), ((int)(((byte)(60)))));
			dataGridViewCellStyle2.Font = new System.Drawing.Font("Consolas", 10F);
			dataGridViewCellStyle2.ForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle2.SelectionBackColor = System.Drawing.Color.LightGray;
			dataGridViewCellStyle2.SelectionForeColor = System.Drawing.Color.Black;
			dataGridViewCellStyle2.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
			this.Gate_List_Box.DefaultCellStyle = dataGridViewCellStyle2;
			this.Gate_List_Box.Dock = System.Windows.Forms.DockStyle.Fill;
			this.Gate_List_Box.EnableHeadersVisualStyles = false;
			this.Gate_List_Box.GridColor = System.Drawing.Color.FromArgb(((int)(((byte)(15)))), ((int)(((byte)(15)))), ((int)(((byte)(15)))));
			this.Gate_List_Box.Location = new System.Drawing.Point(0, 25);
			this.Gate_List_Box.Margin = new System.Windows.Forms.Padding(0);
			this.Gate_List_Box.Name = "Gate_List_Box";
			this.Gate_List_Box.RowHeadersBorderStyle = System.Windows.Forms.DataGridViewHeaderBorderStyle.Single;
			dataGridViewCellStyle3.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
			dataGridViewCellStyle3.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
			dataGridViewCellStyle3.ForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle3.SelectionBackColor = System.Drawing.Color.DimGray;
			dataGridViewCellStyle3.SelectionForeColor = System.Drawing.Color.White;
			dataGridViewCellStyle3.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
			this.Gate_List_Box.RowHeadersDefaultCellStyle = dataGridViewCellStyle3;
			this.Gate_List_Box.RowHeadersWidth = 30;
			this.Gate_List_Box.RowTemplate.Resizable = System.Windows.Forms.DataGridViewTriState.False;
			this.Gate_List_Box.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
			this.Gate_List_Box.Size = new System.Drawing.Size(1264, 656);
			this.Gate_List_Box.TabIndex = 3;
			this.Gate_List_Box.Visible = false;
			this.Gate_List_Box.CellValidating += new System.Windows.Forms.DataGridViewCellValidatingEventHandler(this.Gate_List_Box_CellValidating);
			// 
			// Index_Col
			// 
			this.Index_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Index_Col.HeaderText = "Index";
			this.Index_Col.Name = "Index_Col";
			this.Index_Col.ReadOnly = true;
			this.Index_Col.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Index_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Index_Col.ToolTipText = "Number of the Gate";
			this.Index_Col.Width = 60;
			// 
			// Flag_Col
			// 
			this.Flag_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Flag_Col.HeaderText = "Flag";
			this.Flag_Col.Name = "Flag_Col";
			this.Flag_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Flag_Col.ToolTipText = "0 = Move Destination / 1 = Portal In / 2 = Portal Out";
			this.Flag_Col.Width = 80;
			// 
			// Map_Col
			// 
			this.Map_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Map_Col.HeaderText = "Map";
			this.Map_Col.Name = "Map_Col";
			this.Map_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Map_Col.ToolTipText = "Map of the Gate";
			this.Map_Col.Width = 80;
			// 
			// StartX_Col
			// 
			this.StartX_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.StartX_Col.HeaderText = "Start X";
			this.StartX_Col.Name = "StartX_Col";
			this.StartX_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.StartX_Col.ToolTipText = "Position Start X of the Gate";
			this.StartX_Col.Width = 80;
			// 
			// StartY_Col
			// 
			this.StartY_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.StartY_Col.HeaderText = "Start Y";
			this.StartY_Col.Name = "StartY_Col";
			this.StartY_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.StartY_Col.ToolTipText = "Position Start Y of the Gate";
			this.StartY_Col.Width = 80;
			// 
			// EndX_Col
			// 
			this.EndX_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.EndX_Col.HeaderText = "End X";
			this.EndX_Col.Name = "EndX_Col";
			this.EndX_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.EndX_Col.ToolTipText = "Position End X of the Gate";
			this.EndX_Col.Width = 80;
			// 
			// EndY_Col
			// 
			this.EndY_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.EndY_Col.HeaderText = "End Y";
			this.EndY_Col.Name = "EndY_Col";
			this.EndY_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.EndY_Col.ToolTipText = "Position End Y of the Gate";
			this.EndY_Col.Width = 80;
			// 
			// Target_Col
			// 
			this.Target_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Target_Col.HeaderText = "Target";
			this.Target_Col.Name = "Target_Col";
			this.Target_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Target_Col.ToolTipText = "[if Flag = 1] Target Gate Index";
			this.Target_Col.Width = 80;
			// 
			// Direction_Col
			// 
			this.Direction_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.Direction_Col.HeaderText = "Direction";
			this.Direction_Col.Name = "Direction_Col";
			this.Direction_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.Direction_Col.ToolTipText = "Direction of the player in the Gate";
			this.Direction_Col.Width = 80;
			// 
			// MinLevel_Col
			// 
			this.MinLevel_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.MinLevel_Col.HeaderText = "Min Level";
			this.MinLevel_Col.Name = "MinLevel_Col";
			this.MinLevel_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.MinLevel_Col.ToolTipText = "Min Level to use this Gate";
			this.MinLevel_Col.Width = 80;
			// 
			// MaxLevel_Col
			// 
			this.MaxLevel_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.MaxLevel_Col.HeaderText = "Max Level";
			this.MaxLevel_Col.Name = "MaxLevel_Col";
			this.MaxLevel_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.MaxLevel_Col.ToolTipText = "Max Level to use this Gate";
			this.MaxLevel_Col.Width = 80;
			// 
			// MinReset_Col
			// 
			this.MinReset_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.MinReset_Col.HeaderText = "Min Reset";
			this.MinReset_Col.Name = "MinReset_Col";
			this.MinReset_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.MinReset_Col.ToolTipText = "[Only MuEmu] Min Reset to use this Gate";
			this.MinReset_Col.Width = 80;
			// 
			// MaxReset_Col
			// 
			this.MaxReset_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.MaxReset_Col.HeaderText = "Max Reset";
			this.MaxReset_Col.Name = "MaxReset_Col";
			this.MaxReset_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.MaxReset_Col.ToolTipText = "[Only MuEmu] Max Reset to use this Gate";
			this.MaxReset_Col.Width = 80;
			// 
			// AccountLevel_Col
			// 
			this.AccountLevel_Col.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
			this.AccountLevel_Col.HeaderText = "Account Level";
			this.AccountLevel_Col.Name = "AccountLevel_Col";
			this.AccountLevel_Col.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			this.AccountLevel_Col.ToolTipText = "[Only MuEmu] Account Level required to use this Gate";
			this.AccountLevel_Col.Width = 80;
			// 
			// OpenGateFile
			// 
			this.OpenGateFile.FileName = "Gate";
			// 
			// GateEditor
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.BackColor = System.Drawing.Color.Black;
			this.ClientSize = new System.Drawing.Size(1264, 681);
			this.Controls.Add(this.Gate_List_Box);
			this.Controls.Add(this.toolStrip1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "GateEditor";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Gate Manager";
			this.Load += new System.EventHandler(this.GateEditor_Load);
			this.toolStrip1.ResumeLayout(false);
			this.toolStrip1.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.Gate_List_Box)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ToolStrip toolStrip1;
		private System.Windows.Forms.ToolStripDropDownButton Load_Gate_Menu_Btn;
		private System.Windows.Forms.ToolStripDropDownButton Save_Gate_Menu_Btn;
		private System.Windows.Forms.DataGridView Gate_List_Box;
		private System.Windows.Forms.ToolStripMenuItem gatetxtToolStripMenuItem;
		private System.Windows.Forms.OpenFileDialog OpenGateFile;
		private System.Windows.Forms.SaveFileDialog SaveGateFile;
		private System.Windows.Forms.ToolStripMenuItem gatetxtToolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem gatebmdToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem gatebmdToolStripMenuItem1;
		private System.Windows.Forms.DataGridViewTextBoxColumn Index_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Flag_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Map_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn StartX_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn StartY_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn EndX_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn EndY_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Target_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn Direction_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn MinLevel_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn MaxLevel_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn MinReset_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn MaxReset_Col;
		private System.Windows.Forms.DataGridViewTextBoxColumn AccountLevel_Col;
	}
}