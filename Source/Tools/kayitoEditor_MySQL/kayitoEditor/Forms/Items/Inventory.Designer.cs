
namespace kayito_Editor.Forms
{
	partial class Inventory
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Inventory));
			this.Item_Editor_Panel = new System.Windows.Forms.Panel();
			this.Buttons_Panel = new System.Windows.Forms.Panel();
			this.Character_Box = new System.Windows.Forms.TextBox();
			this.Character_txt = new System.Windows.Forms.Label();
			this.Btn_Close = new System.Windows.Forms.Button();
			this.Info_Box = new System.Windows.Forms.TextBox();
			this.Btn_Inventory_Save = new System.Windows.Forms.Button();
			this.User_Box = new System.Windows.Forms.TextBox();
			this.Btn_Inventory_Reset = new System.Windows.Forms.Button();
			this.Account_txt = new System.Windows.Forms.Label();
			this.Warehouse_Panel_Back = new System.Windows.Forms.Panel();
			this.Character_Equipment_Panel = new System.Windows.Forms.Panel();
			this.Character_Inventory_Panel = new System.Windows.Forms.Panel();
			this.Character_Zen = new System.Windows.Forms.NumericUpDown();
			this.Buttons_Panel.SuspendLayout();
			this.Warehouse_Panel_Back.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.Character_Zen)).BeginInit();
			this.SuspendLayout();
			// 
			// Item_Editor_Panel
			// 
			this.Item_Editor_Panel.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.Item_Editor_Panel.Location = new System.Drawing.Point(0, 0);
			this.Item_Editor_Panel.Name = "Item_Editor_Panel";
			this.Item_Editor_Panel.Size = new System.Drawing.Size(237, 539);
			this.Item_Editor_Panel.TabIndex = 452;
			// 
			// Buttons_Panel
			// 
			this.Buttons_Panel.BackgroundImage = global::kayito_Editor.Properties.Resources.Interface_Back;
			this.Buttons_Panel.Controls.Add(this.Character_Box);
			this.Buttons_Panel.Controls.Add(this.Character_txt);
			this.Buttons_Panel.Controls.Add(this.Btn_Close);
			this.Buttons_Panel.Controls.Add(this.Info_Box);
			this.Buttons_Panel.Controls.Add(this.Btn_Inventory_Save);
			this.Buttons_Panel.Controls.Add(this.User_Box);
			this.Buttons_Panel.Controls.Add(this.Btn_Inventory_Reset);
			this.Buttons_Panel.Controls.Add(this.Account_txt);
			this.Buttons_Panel.Location = new System.Drawing.Point(474, 0);
			this.Buttons_Panel.Name = "Buttons_Panel";
			this.Buttons_Panel.Size = new System.Drawing.Size(237, 539);
			this.Buttons_Panel.TabIndex = 458;
			// 
			// Character_Box
			// 
			this.Character_Box.Font = new System.Drawing.Font("Tahoma", 10F);
			this.Character_Box.Location = new System.Drawing.Point(33, 135);
			this.Character_Box.MaxLength = 10;
			this.Character_Box.Name = "Character_Box";
			this.Character_Box.ReadOnly = true;
			this.Character_Box.Size = new System.Drawing.Size(169, 24);
			this.Character_Box.TabIndex = 2;
			this.Character_Box.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			// 
			// Character_txt
			// 
			this.Character_txt.AutoSize = true;
			this.Character_txt.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.Character_txt.Font = new System.Drawing.Font("Tahoma", 10F);
			this.Character_txt.ForeColor = System.Drawing.Color.Red;
			this.Character_txt.Location = new System.Drawing.Point(81, 115);
			this.Character_txt.Name = "Character_txt";
			this.Character_txt.Size = new System.Drawing.Size(68, 17);
			this.Character_txt.TabIndex = 458;
			this.Character_txt.Text = "Character";
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
			this.Btn_Close.TabIndex = 7;
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
			this.Info_Box.Location = new System.Drawing.Point(33, 185);
			this.Info_Box.Multiline = true;
			this.Info_Box.Name = "Info_Box";
			this.Info_Box.ReadOnly = true;
			this.Info_Box.Size = new System.Drawing.Size(169, 179);
			this.Info_Box.TabIndex = 3;
			this.Info_Box.Text = resources.GetString("Info_Box.Text");
			this.Info_Box.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			// 
			// Btn_Inventory_Save
			// 
			this.Btn_Inventory_Save.BackColor = System.Drawing.Color.Lime;
			this.Btn_Inventory_Save.Cursor = System.Windows.Forms.Cursors.Hand;
			this.Btn_Inventory_Save.Font = new System.Drawing.Font("Microsoft Sans Serif", 13F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Btn_Inventory_Save.Location = new System.Drawing.Point(23, 407);
			this.Btn_Inventory_Save.Name = "Btn_Inventory_Save";
			this.Btn_Inventory_Save.Size = new System.Drawing.Size(86, 34);
			this.Btn_Inventory_Save.TabIndex = 5;
			this.Btn_Inventory_Save.Text = "SAVE";
			this.Btn_Inventory_Save.UseVisualStyleBackColor = false;
			this.Btn_Inventory_Save.Click += new System.EventHandler(this.Btn_Inventory_Save_Click);
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
			// Btn_Inventory_Reset
			// 
			this.Btn_Inventory_Reset.BackColor = System.Drawing.Color.Red;
			this.Btn_Inventory_Reset.Cursor = System.Windows.Forms.Cursors.Hand;
			this.Btn_Inventory_Reset.Font = new System.Drawing.Font("Microsoft Sans Serif", 13F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Btn_Inventory_Reset.Location = new System.Drawing.Point(127, 407);
			this.Btn_Inventory_Reset.Name = "Btn_Inventory_Reset";
			this.Btn_Inventory_Reset.Size = new System.Drawing.Size(86, 34);
			this.Btn_Inventory_Reset.TabIndex = 6;
			this.Btn_Inventory_Reset.Text = "RESET";
			this.Btn_Inventory_Reset.UseVisualStyleBackColor = false;
			this.Btn_Inventory_Reset.Click += new System.EventHandler(this.Btn_Inventory_Reset_Click);
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
			// Warehouse_Panel_Back
			// 
			this.Warehouse_Panel_Back.BackgroundImage = global::kayito_Editor.Properties.Resources.Inventory_Back;
			this.Warehouse_Panel_Back.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
			this.Warehouse_Panel_Back.Controls.Add(this.Character_Equipment_Panel);
			this.Warehouse_Panel_Back.Controls.Add(this.Character_Inventory_Panel);
			this.Warehouse_Panel_Back.Controls.Add(this.Character_Zen);
			this.Warehouse_Panel_Back.Location = new System.Drawing.Point(237, 0);
			this.Warehouse_Panel_Back.Name = "Warehouse_Panel_Back";
			this.Warehouse_Panel_Back.Size = new System.Drawing.Size(237, 539);
			this.Warehouse_Panel_Back.TabIndex = 453;
			// 
			// Character_Equipment_Panel
			// 
			this.Character_Equipment_Panel.BackColor = System.Drawing.Color.Transparent;
			this.Character_Equipment_Panel.Location = new System.Drawing.Point(18, 55);
			this.Character_Equipment_Panel.Name = "Character_Equipment_Panel";
			this.Character_Equipment_Panel.Size = new System.Drawing.Size(200, 186);
			this.Character_Equipment_Panel.TabIndex = 450;
			// 
			// Character_Inventory_Panel
			// 
			this.Character_Inventory_Panel.BackColor = System.Drawing.Color.Transparent;
			this.Character_Inventory_Panel.Location = new System.Drawing.Point(19, 249);
			this.Character_Inventory_Panel.Name = "Character_Inventory_Panel";
			this.Character_Inventory_Panel.Size = new System.Drawing.Size(200, 200);
			this.Character_Inventory_Panel.TabIndex = 449;
			// 
			// Character_Zen
			// 
			this.Character_Zen.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(33)))), ((int)(((byte)(33)))), ((int)(((byte)(33)))));
			this.Character_Zen.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.Character_Zen.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Character_Zen.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(225)))), ((int)(((byte)(208)))), ((int)(((byte)(170)))));
			this.Character_Zen.Location = new System.Drawing.Point(66, 461);
			this.Character_Zen.Maximum = new decimal(new int[] {
            2000000000,
            0,
            0,
            0});
			this.Character_Zen.Name = "Character_Zen";
			this.Character_Zen.Size = new System.Drawing.Size(133, 18);
			this.Character_Zen.TabIndex = 4;
			this.Character_Zen.Value = new decimal(new int[] {
            2000000000,
            0,
            0,
            0});
			// 
			// Inventory
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.ClientSize = new System.Drawing.Size(711, 539);
			this.Controls.Add(this.Buttons_Panel);
			this.Controls.Add(this.Warehouse_Panel_Back);
			this.Controls.Add(this.Item_Editor_Panel);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "Inventory";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Inventory Editor";
			this.Load += new System.EventHandler(this.Inventory_Load);
			this.Buttons_Panel.ResumeLayout(false);
			this.Buttons_Panel.PerformLayout();
			this.Warehouse_Panel_Back.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.Character_Zen)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Panel Item_Editor_Panel;
		private System.Windows.Forms.Panel Warehouse_Panel_Back;
		private System.Windows.Forms.Panel Character_Inventory_Panel;
		private System.Windows.Forms.NumericUpDown Character_Zen;
		private System.Windows.Forms.Panel Character_Equipment_Panel;
		private System.Windows.Forms.Panel Buttons_Panel;
		private System.Windows.Forms.Button Btn_Close;
		private System.Windows.Forms.TextBox Info_Box;
		private System.Windows.Forms.Button Btn_Inventory_Save;
		private System.Windows.Forms.TextBox User_Box;
		private System.Windows.Forms.Button Btn_Inventory_Reset;
		private System.Windows.Forms.Label Account_txt;
		private System.Windows.Forms.TextBox Character_Box;
		private System.Windows.Forms.Label Character_txt;
	}
}