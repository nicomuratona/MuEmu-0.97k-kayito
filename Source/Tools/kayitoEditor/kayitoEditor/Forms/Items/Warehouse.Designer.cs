namespace kayito_Editor.Forms
{
	partial class Warehouse
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Warehouse));
			this.Item_Editor_Panel = new System.Windows.Forms.Panel();
			this.Buttons_Panel = new System.Windows.Forms.Panel();
			this.Btn_Close = new System.Windows.Forms.Button();
			this.Info_Box = new System.Windows.Forms.TextBox();
			this.Btn_Ware_Save = new System.Windows.Forms.Button();
			this.User_Box = new System.Windows.Forms.TextBox();
			this.Btn_Ware_Reset = new System.Windows.Forms.Button();
			this.Warehouse_Pass_txt = new System.Windows.Forms.Label();
			this.Account_txt = new System.Windows.Forms.Label();
			this.Warehouse_Pass = new System.Windows.Forms.NumericUpDown();
			this.Warehouse_Panel_Back = new System.Windows.Forms.Panel();
			this.Warehouse_Items_Panel = new System.Windows.Forms.Panel();
			this.Wareouse_Zen = new System.Windows.Forms.NumericUpDown();
			this.Warehouse_Number = new System.Windows.Forms.NumericUpDown();
			this.Buttons_Panel.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.Warehouse_Pass)).BeginInit();
			this.Warehouse_Panel_Back.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.Wareouse_Zen)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.Warehouse_Number)).BeginInit();
			this.SuspendLayout();
			// 
			// Item_Editor_Panel
			// 
			this.Item_Editor_Panel.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.Item_Editor_Panel.Location = new System.Drawing.Point(0, 0);
			this.Item_Editor_Panel.Name = "Item_Editor_Panel";
			this.Item_Editor_Panel.Size = new System.Drawing.Size(237, 539);
			this.Item_Editor_Panel.TabIndex = 451;
			// 
			// Buttons_Panel
			// 
			this.Buttons_Panel.BackgroundImage = global::kayito_Editor.Properties.Resources.Interface_Back;
			this.Buttons_Panel.Controls.Add(this.Btn_Close);
			this.Buttons_Panel.Controls.Add(this.Info_Box);
			this.Buttons_Panel.Controls.Add(this.Btn_Ware_Save);
			this.Buttons_Panel.Controls.Add(this.User_Box);
			this.Buttons_Panel.Controls.Add(this.Btn_Ware_Reset);
			this.Buttons_Panel.Controls.Add(this.Warehouse_Pass_txt);
			this.Buttons_Panel.Controls.Add(this.Account_txt);
			this.Buttons_Panel.Controls.Add(this.Warehouse_Pass);
			this.Buttons_Panel.Location = new System.Drawing.Point(474, 0);
			this.Buttons_Panel.Name = "Buttons_Panel";
			this.Buttons_Panel.Size = new System.Drawing.Size(237, 539);
			this.Buttons_Panel.TabIndex = 457;
			// 
			// Btn_Close
			// 
			this.Btn_Close.BackgroundImage = global::kayito_Editor.Properties.Resources.Exit_n;
			this.Btn_Close.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
			this.Btn_Close.Cursor = System.Windows.Forms.Cursors.Hand;
			this.Btn_Close.FlatAppearance.BorderSize = 0;
			this.Btn_Close.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.Btn_Close.Location = new System.Drawing.Point(37, 479);
			this.Btn_Close.Name = "Btn_Close";
			this.Btn_Close.Size = new System.Drawing.Size(35, 35);
			this.Btn_Close.TabIndex = 8;
			this.Btn_Close.UseVisualStyleBackColor = true;
			this.Btn_Close.Click += new System.EventHandler(this.Btn_Close_Click);
			this.Btn_Close.MouseDown += new System.Windows.Forms.MouseEventHandler(this.Btn_Close_MouseDown);
			this.Btn_Close.MouseUp += new System.Windows.Forms.MouseEventHandler(this.Btn_Close_MouseUp);
			// 
			// Info_Box
			// 
			this.Info_Box.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.Info_Box.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Info_Box.ForeColor = System.Drawing.Color.Lime;
			this.Info_Box.Location = new System.Drawing.Point(33, 119);
			this.Info_Box.Multiline = true;
			this.Info_Box.Name = "Info_Box";
			this.Info_Box.ReadOnly = true;
			this.Info_Box.Size = new System.Drawing.Size(169, 179);
			this.Info_Box.TabIndex = 2;
			this.Info_Box.Text = resources.GetString("Info_Box.Text");
			this.Info_Box.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			// 
			// Btn_Ware_Save
			// 
			this.Btn_Ware_Save.BackColor = System.Drawing.Color.Lime;
			this.Btn_Ware_Save.Cursor = System.Windows.Forms.Cursors.Hand;
			this.Btn_Ware_Save.Font = new System.Drawing.Font("Microsoft Sans Serif", 13F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Btn_Ware_Save.Location = new System.Drawing.Point(23, 407);
			this.Btn_Ware_Save.Name = "Btn_Ware_Save";
			this.Btn_Ware_Save.Size = new System.Drawing.Size(86, 34);
			this.Btn_Ware_Save.TabIndex = 6;
			this.Btn_Ware_Save.Text = "SAVE";
			this.Btn_Ware_Save.UseVisualStyleBackColor = false;
			this.Btn_Ware_Save.Click += new System.EventHandler(this.Btn_Ware_Save_Click);
			// 
			// User_Box
			// 
			this.User_Box.Font = new System.Drawing.Font("Tahoma", 10F);
			this.User_Box.Location = new System.Drawing.Point(33, 78);
			this.User_Box.MaxLength = 10;
			this.User_Box.Name = "User_Box";
			this.User_Box.ReadOnly = true;
			this.User_Box.Size = new System.Drawing.Size(169, 24);
			this.User_Box.TabIndex = 1;
			this.User_Box.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			// 
			// Btn_Ware_Reset
			// 
			this.Btn_Ware_Reset.BackColor = System.Drawing.Color.Red;
			this.Btn_Ware_Reset.Cursor = System.Windows.Forms.Cursors.Hand;
			this.Btn_Ware_Reset.Font = new System.Drawing.Font("Microsoft Sans Serif", 13F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Btn_Ware_Reset.Location = new System.Drawing.Point(127, 407);
			this.Btn_Ware_Reset.Name = "Btn_Ware_Reset";
			this.Btn_Ware_Reset.Size = new System.Drawing.Size(86, 34);
			this.Btn_Ware_Reset.TabIndex = 7;
			this.Btn_Ware_Reset.Text = "RESET";
			this.Btn_Ware_Reset.UseVisualStyleBackColor = false;
			this.Btn_Ware_Reset.Click += new System.EventHandler(this.Btn_Ware_Reset_Click);
			// 
			// Warehouse_Pass_txt
			// 
			this.Warehouse_Pass_txt.AutoSize = true;
			this.Warehouse_Pass_txt.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(33)))), ((int)(((byte)(33)))), ((int)(((byte)(33)))));
			this.Warehouse_Pass_txt.Font = new System.Drawing.Font("Tahoma", 10F);
			this.Warehouse_Pass_txt.ForeColor = System.Drawing.Color.Lime;
			this.Warehouse_Pass_txt.Location = new System.Drawing.Point(33, 317);
			this.Warehouse_Pass_txt.Name = "Warehouse_Pass_txt";
			this.Warehouse_Pass_txt.Size = new System.Drawing.Size(170, 17);
			this.Warehouse_Pass_txt.TabIndex = 450;
			this.Warehouse_Pass_txt.Text = "WAREHOUSE PASSWORD";
			// 
			// Account_txt
			// 
			this.Account_txt.AutoSize = true;
			this.Account_txt.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.Account_txt.Font = new System.Drawing.Font("Tahoma", 10F);
			this.Account_txt.ForeColor = System.Drawing.Color.Red;
			this.Account_txt.Location = new System.Drawing.Point(85, 58);
			this.Account_txt.Name = "Account_txt";
			this.Account_txt.Size = new System.Drawing.Size(59, 17);
			this.Account_txt.TabIndex = 451;
			this.Account_txt.Text = "Account";
			// 
			// Warehouse_Pass
			// 
			this.Warehouse_Pass.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(225)))), ((int)(((byte)(208)))), ((int)(((byte)(170)))));
			this.Warehouse_Pass.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.Warehouse_Pass.Font = new System.Drawing.Font("Tahoma", 13F, System.Drawing.FontStyle.Bold);
			this.Warehouse_Pass.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(33)))), ((int)(((byte)(33)))), ((int)(((byte)(33)))));
			this.Warehouse_Pass.Location = new System.Drawing.Point(33, 337);
			this.Warehouse_Pass.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
			this.Warehouse_Pass.Name = "Warehouse_Pass";
			this.Warehouse_Pass.Size = new System.Drawing.Size(170, 28);
			this.Warehouse_Pass.TabIndex = 3;
			this.Warehouse_Pass.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			this.Warehouse_Pass.Value = new decimal(new int[] {
            9999,
            0,
            0,
            0});
			// 
			// Warehouse_Panel_Back
			// 
			this.Warehouse_Panel_Back.BackgroundImage = global::kayito_Editor.Properties.Resources.Warehouse_Back;
			this.Warehouse_Panel_Back.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
			this.Warehouse_Panel_Back.Controls.Add(this.Warehouse_Items_Panel);
			this.Warehouse_Panel_Back.Controls.Add(this.Wareouse_Zen);
			this.Warehouse_Panel_Back.Controls.Add(this.Warehouse_Number);
			this.Warehouse_Panel_Back.Location = new System.Drawing.Point(237, 0);
			this.Warehouse_Panel_Back.Name = "Warehouse_Panel_Back";
			this.Warehouse_Panel_Back.Size = new System.Drawing.Size(237, 539);
			this.Warehouse_Panel_Back.TabIndex = 448;
			// 
			// Warehouse_Items_Panel
			// 
			this.Warehouse_Items_Panel.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.Warehouse_Items_Panel.Location = new System.Drawing.Point(19, 62);
			this.Warehouse_Items_Panel.Name = "Warehouse_Items_Panel";
			this.Warehouse_Items_Panel.Size = new System.Drawing.Size(200, 375);
			this.Warehouse_Items_Panel.TabIndex = 449;
			// 
			// Wareouse_Zen
			// 
			this.Wareouse_Zen.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(33)))), ((int)(((byte)(33)))), ((int)(((byte)(33)))));
			this.Wareouse_Zen.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.Wareouse_Zen.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Wareouse_Zen.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(225)))), ((int)(((byte)(208)))), ((int)(((byte)(170)))));
			this.Wareouse_Zen.Location = new System.Drawing.Point(66, 442);
			this.Wareouse_Zen.Maximum = new decimal(new int[] {
            2000000000,
            0,
            0,
            0});
			this.Wareouse_Zen.Name = "Wareouse_Zen";
			this.Wareouse_Zen.Size = new System.Drawing.Size(133, 18);
			this.Wareouse_Zen.TabIndex = 4;
			this.Wareouse_Zen.Value = new decimal(new int[] {
            2000000000,
            0,
            0,
            0});
			// 
			// Warehouse_Number
			// 
			this.Warehouse_Number.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(33)))), ((int)(((byte)(33)))), ((int)(((byte)(33)))));
			this.Warehouse_Number.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.Warehouse_Number.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Warehouse_Number.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(225)))), ((int)(((byte)(208)))), ((int)(((byte)(170)))));
			this.Warehouse_Number.Location = new System.Drawing.Point(66, 476);
			this.Warehouse_Number.Maximum = new decimal(new int[] {
            10,
            0,
            0,
            0});
			this.Warehouse_Number.Name = "Warehouse_Number";
			this.Warehouse_Number.Size = new System.Drawing.Size(133, 18);
			this.Warehouse_Number.TabIndex = 5;
			this.Warehouse_Number.ValueChanged += new System.EventHandler(this.Warehouse_Number_ValueChanged);
			// 
			// Warehouse
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.ClientSize = new System.Drawing.Size(711, 539);
			this.Controls.Add(this.Buttons_Panel);
			this.Controls.Add(this.Item_Editor_Panel);
			this.Controls.Add(this.Warehouse_Panel_Back);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "Warehouse";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Warehouse Editor";
			this.Load += new System.EventHandler(this.Warehouse_Load);
			this.Buttons_Panel.ResumeLayout(false);
			this.Buttons_Panel.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.Warehouse_Pass)).EndInit();
			this.Warehouse_Panel_Back.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.Wareouse_Zen)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.Warehouse_Number)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion
		private System.Windows.Forms.NumericUpDown Warehouse_Number;
		private System.Windows.Forms.Panel Warehouse_Panel_Back;
		private System.Windows.Forms.NumericUpDown Wareouse_Zen;
		private System.Windows.Forms.NumericUpDown Warehouse_Pass;
		private System.Windows.Forms.Label Warehouse_Pass_txt;
		private System.Windows.Forms.Panel Warehouse_Items_Panel;
		private System.Windows.Forms.Panel Item_Editor_Panel;
		private System.Windows.Forms.Button Btn_Ware_Save;
		private System.Windows.Forms.Button Btn_Ware_Reset;
		private System.Windows.Forms.Label Account_txt;
		private System.Windows.Forms.TextBox User_Box;
		private System.Windows.Forms.TextBox Info_Box;
		private System.Windows.Forms.Panel Buttons_Panel;
		private System.Windows.Forms.Button Btn_Close;
	}
}