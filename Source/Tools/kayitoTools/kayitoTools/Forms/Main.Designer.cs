
namespace kayitoTools
{
	partial class Main
	{
		/// <summary>
		/// Variable del diseñador necesaria.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Limpiar los recursos que se estén usando.
		/// </summary>
		/// <param name="disposing">true si los recursos administrados se deben desechar; false en caso contrario.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Código generado por el Diseñador de Windows Forms

		/// <summary>
		/// Método necesario para admitir el Diseñador. No se puede modificar
		/// el contenido de este método con el editor de código.
		/// </summary>
		private void InitializeComponent()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Main));
			this.toolStrip1 = new System.Windows.Forms.ToolStrip();
			this.Dialog_Menu_Btn = new System.Windows.Forms.ToolStripButton();
			this.Filter_Menu_Btn = new System.Windows.Forms.ToolStripButton();
			this.Gate_Menu_Btn = new System.Windows.Forms.ToolStripButton();
			this.Item_Menu_Btn = new System.Windows.Forms.ToolStripButton();
			this.Monster_Menu_Btn = new System.Windows.Forms.ToolStripButton();
			this.Quest_Menu_Btn = new System.Windows.Forms.ToolStripButton();
			this.Skill_Menu_Btn = new System.Windows.Forms.ToolStripButton();
			this.Text_Menu_Btn = new System.Windows.Forms.ToolStripButton();
			this.Log_Text_Box = new System.Windows.Forms.TextBox();
			this.panelChildForm = new System.Windows.Forms.Panel();
			this.Event_Log_txt = new System.Windows.Forms.Label();
			this.Clear_Text_Box_Btn = new System.Windows.Forms.Button();
			this.toolStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// toolStrip1
			// 
			this.toolStrip1.BackColor = System.Drawing.SystemColors.Control;
			this.toolStrip1.Font = new System.Drawing.Font("Tahoma", 9F);
			this.toolStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Dialog_Menu_Btn,
            this.Filter_Menu_Btn,
            this.Gate_Menu_Btn,
            this.Item_Menu_Btn,
            this.Monster_Menu_Btn,
            this.Quest_Menu_Btn,
            this.Skill_Menu_Btn,
            this.Text_Menu_Btn});
			this.toolStrip1.Location = new System.Drawing.Point(0, 0);
			this.toolStrip1.Name = "toolStrip1";
			this.toolStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
			this.toolStrip1.Size = new System.Drawing.Size(1064, 25);
			this.toolStrip1.TabIndex = 0;
			this.toolStrip1.Text = "toolStrip1";
			// 
			// Dialog_Menu_Btn
			// 
			this.Dialog_Menu_Btn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this.Dialog_Menu_Btn.Image = ((System.Drawing.Image)(resources.GetObject("Dialog_Menu_Btn.Image")));
			this.Dialog_Menu_Btn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.Dialog_Menu_Btn.Name = "Dialog_Menu_Btn";
			this.Dialog_Menu_Btn.Size = new System.Drawing.Size(54, 22);
			this.Dialog_Menu_Btn.Text = "DIALOG";
			this.Dialog_Menu_Btn.Click += new System.EventHandler(this.Dialog_Menu_Btn_Click);
			// 
			// Filter_Menu_Btn
			// 
			this.Filter_Menu_Btn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this.Filter_Menu_Btn.Image = ((System.Drawing.Image)(resources.GetObject("Filter_Menu_Btn.Image")));
			this.Filter_Menu_Btn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.Filter_Menu_Btn.Name = "Filter_Menu_Btn";
			this.Filter_Menu_Btn.Size = new System.Drawing.Size(49, 22);
			this.Filter_Menu_Btn.Text = "FILTER";
			this.Filter_Menu_Btn.Click += new System.EventHandler(this.Filter_Menu_Btn_Click);
			// 
			// Gate_Menu_Btn
			// 
			this.Gate_Menu_Btn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this.Gate_Menu_Btn.Image = ((System.Drawing.Image)(resources.GetObject("Gate_Menu_Btn.Image")));
			this.Gate_Menu_Btn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.Gate_Menu_Btn.Margin = new System.Windows.Forms.Padding(0);
			this.Gate_Menu_Btn.Name = "Gate_Menu_Btn";
			this.Gate_Menu_Btn.Size = new System.Drawing.Size(42, 25);
			this.Gate_Menu_Btn.Text = "GATE";
			this.Gate_Menu_Btn.Click += new System.EventHandler(this.Gate_Menu_Btn_Click);
			// 
			// Item_Menu_Btn
			// 
			this.Item_Menu_Btn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this.Item_Menu_Btn.Image = ((System.Drawing.Image)(resources.GetObject("Item_Menu_Btn.Image")));
			this.Item_Menu_Btn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.Item_Menu_Btn.Margin = new System.Windows.Forms.Padding(0);
			this.Item_Menu_Btn.Name = "Item_Menu_Btn";
			this.Item_Menu_Btn.Size = new System.Drawing.Size(39, 25);
			this.Item_Menu_Btn.Text = "ITEM";
			this.Item_Menu_Btn.ToolTipText = "ITEM";
			this.Item_Menu_Btn.Click += new System.EventHandler(this.Item_Menu_Btn_Click);
			// 
			// Monster_Menu_Btn
			// 
			this.Monster_Menu_Btn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this.Monster_Menu_Btn.Image = ((System.Drawing.Image)(resources.GetObject("Monster_Menu_Btn.Image")));
			this.Monster_Menu_Btn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.Monster_Menu_Btn.Name = "Monster_Menu_Btn";
			this.Monster_Menu_Btn.Size = new System.Drawing.Size(66, 22);
			this.Monster_Menu_Btn.Text = "MONSTER";
			this.Monster_Menu_Btn.Click += new System.EventHandler(this.Monster_Menu_Btn_Click);
			// 
			// Quest_Menu_Btn
			// 
			this.Quest_Menu_Btn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this.Quest_Menu_Btn.Image = ((System.Drawing.Image)(resources.GetObject("Quest_Menu_Btn.Image")));
			this.Quest_Menu_Btn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.Quest_Menu_Btn.Name = "Quest_Menu_Btn";
			this.Quest_Menu_Btn.Size = new System.Drawing.Size(50, 22);
			this.Quest_Menu_Btn.Text = "QUEST";
			this.Quest_Menu_Btn.Click += new System.EventHandler(this.Quest_Menu_Btn_Click);
			// 
			// Skill_Menu_Btn
			// 
			this.Skill_Menu_Btn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this.Skill_Menu_Btn.Image = ((System.Drawing.Image)(resources.GetObject("Skill_Menu_Btn.Image")));
			this.Skill_Menu_Btn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.Skill_Menu_Btn.Margin = new System.Windows.Forms.Padding(0);
			this.Skill_Menu_Btn.Name = "Skill_Menu_Btn";
			this.Skill_Menu_Btn.Size = new System.Drawing.Size(41, 25);
			this.Skill_Menu_Btn.Text = "SKILL";
			this.Skill_Menu_Btn.Click += new System.EventHandler(this.Skill_Menu_Btn_Click);
			// 
			// Text_Menu_Btn
			// 
			this.Text_Menu_Btn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this.Text_Menu_Btn.Image = ((System.Drawing.Image)(resources.GetObject("Text_Menu_Btn.Image")));
			this.Text_Menu_Btn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.Text_Menu_Btn.Margin = new System.Windows.Forms.Padding(0);
			this.Text_Menu_Btn.Name = "Text_Menu_Btn";
			this.Text_Menu_Btn.Size = new System.Drawing.Size(41, 25);
			this.Text_Menu_Btn.Text = "TEXT";
			this.Text_Menu_Btn.Click += new System.EventHandler(this.Text_Menu_Btn_Click);
			// 
			// Log_Text_Box
			// 
			this.Log_Text_Box.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.Log_Text_Box.BackColor = System.Drawing.Color.Black;
			this.Log_Text_Box.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.Log_Text_Box.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Log_Text_Box.ForeColor = System.Drawing.Color.White;
			this.Log_Text_Box.Location = new System.Drawing.Point(12, 57);
			this.Log_Text_Box.Multiline = true;
			this.Log_Text_Box.Name = "Log_Text_Box";
			this.Log_Text_Box.ReadOnly = true;
			this.Log_Text_Box.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.Log_Text_Box.Size = new System.Drawing.Size(1040, 108);
			this.Log_Text_Box.TabIndex = 0;
			// 
			// panelChildForm
			// 
			this.panelChildForm.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.panelChildForm.BackColor = System.Drawing.Color.Transparent;
			this.panelChildForm.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
			this.panelChildForm.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.panelChildForm.Location = new System.Drawing.Point(12, 171);
			this.panelChildForm.Name = "panelChildForm";
			this.panelChildForm.Size = new System.Drawing.Size(1040, 498);
			this.panelChildForm.TabIndex = 2;
			this.panelChildForm.Visible = false;
			// 
			// Event_Log_txt
			// 
			this.Event_Log_txt.AutoSize = true;
			this.Event_Log_txt.BackColor = System.Drawing.Color.Transparent;
			this.Event_Log_txt.Font = new System.Drawing.Font("Arial Black", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Event_Log_txt.ForeColor = System.Drawing.Color.White;
			this.Event_Log_txt.Location = new System.Drawing.Point(12, 33);
			this.Event_Log_txt.Name = "Event_Log_txt";
			this.Event_Log_txt.Size = new System.Drawing.Size(89, 19);
			this.Event_Log_txt.TabIndex = 3;
			this.Event_Log_txt.Text = "Event Log:";
			// 
			// Clear_Text_Box_Btn
			// 
			this.Clear_Text_Box_Btn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.Clear_Text_Box_Btn.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.Clear_Text_Box_Btn.Location = new System.Drawing.Point(977, 29);
			this.Clear_Text_Box_Btn.Name = "Clear_Text_Box_Btn";
			this.Clear_Text_Box_Btn.Size = new System.Drawing.Size(75, 23);
			this.Clear_Text_Box_Btn.TabIndex = 4;
			this.Clear_Text_Box_Btn.Text = "Clear Log";
			this.Clear_Text_Box_Btn.UseVisualStyleBackColor = true;
			this.Clear_Text_Box_Btn.Click += new System.EventHandler(this.Clear_Text_Box_Btn_Click);
			// 
			// Main
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
			this.BackgroundImage = global::kayitoTools.Properties.Resources.PanelBackground;
			this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
			this.ClientSize = new System.Drawing.Size(1064, 681);
			this.Controls.Add(this.Clear_Text_Box_Btn);
			this.Controls.Add(this.Log_Text_Box);
			this.Controls.Add(this.Event_Log_txt);
			this.Controls.Add(this.panelChildForm);
			this.Controls.Add(this.toolStrip1);
			this.Font = new System.Drawing.Font("Tahoma", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.Name = "Main";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "kayito Tools";
			this.toolStrip1.ResumeLayout(false);
			this.toolStrip1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ToolStrip toolStrip1;
		public System.Windows.Forms.TextBox Log_Text_Box;
		private System.Windows.Forms.Panel panelChildForm;
		private System.Windows.Forms.Label Event_Log_txt;
		private System.Windows.Forms.Button Clear_Text_Box_Btn;
		private System.Windows.Forms.ToolStripButton Item_Menu_Btn;
		private System.Windows.Forms.ToolStripButton Skill_Menu_Btn;
		private System.Windows.Forms.ToolStripButton Text_Menu_Btn;
		private System.Windows.Forms.ToolStripButton Gate_Menu_Btn;
		private System.Windows.Forms.ToolStripButton Filter_Menu_Btn;
		private System.Windows.Forms.ToolStripButton Dialog_Menu_Btn;
		private System.Windows.Forms.ToolStripButton Monster_Menu_Btn;
		private System.Windows.Forms.ToolStripButton Quest_Menu_Btn;
	}
}

