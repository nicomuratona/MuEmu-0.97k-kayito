
namespace kayito_Editor.Forms
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

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ItemEditor));
			this.Item_Info_Box = new System.Windows.Forms.GroupBox();
			this.Excellent_Options_Box = new System.Windows.Forms.GroupBox();
			this.Btn_Excellent_Not = new System.Windows.Forms.Button();
			this.Btn_Excellent_All = new System.Windows.Forms.Button();
			this.Excellent6_Box = new System.Windows.Forms.CheckBox();
			this.Excellent5_Box = new System.Windows.Forms.CheckBox();
			this.Excellent4_Box = new System.Windows.Forms.CheckBox();
			this.Excellent3_Box = new System.Windows.Forms.CheckBox();
			this.Excellent2_Box = new System.Windows.Forms.CheckBox();
			this.Excellent1_Box = new System.Windows.Forms.CheckBox();
			this.Luck_Box = new System.Windows.Forms.CheckBox();
			this.Skill_Box = new System.Windows.Forms.CheckBox();
			this.Durability_txt = new System.Windows.Forms.Label();
			this.Durability_Box = new System.Windows.Forms.NumericUpDown();
			this.Option_Box = new System.Windows.Forms.ComboBox();
			this.Level_Box = new System.Windows.Forms.ComboBox();
			this.Option_txt = new System.Windows.Forms.Label();
			this.Level_txt = new System.Windows.Forms.Label();
			this.Item_Box = new System.Windows.Forms.ComboBox();
			this.Item_txt = new System.Windows.Forms.Label();
			this.Section_box = new System.Windows.Forms.ComboBox();
			this.Section_txt = new System.Windows.Forms.Label();
			this.Item_Info_Box.SuspendLayout();
			this.Excellent_Options_Box.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.Durability_Box)).BeginInit();
			this.SuspendLayout();
			// 
			// Item_Info_Box
			// 
			this.Item_Info_Box.BackColor = System.Drawing.Color.LightGray;
			this.Item_Info_Box.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
			this.Item_Info_Box.Controls.Add(this.Excellent_Options_Box);
			this.Item_Info_Box.Controls.Add(this.Luck_Box);
			this.Item_Info_Box.Controls.Add(this.Skill_Box);
			this.Item_Info_Box.Controls.Add(this.Durability_txt);
			this.Item_Info_Box.Controls.Add(this.Durability_Box);
			this.Item_Info_Box.Controls.Add(this.Option_Box);
			this.Item_Info_Box.Controls.Add(this.Level_Box);
			this.Item_Info_Box.Controls.Add(this.Option_txt);
			this.Item_Info_Box.Controls.Add(this.Level_txt);
			this.Item_Info_Box.Controls.Add(this.Item_Box);
			this.Item_Info_Box.Controls.Add(this.Item_txt);
			this.Item_Info_Box.Controls.Add(this.Section_box);
			this.Item_Info_Box.Controls.Add(this.Section_txt);
			this.Item_Info_Box.ForeColor = System.Drawing.Color.Black;
			this.Item_Info_Box.Location = new System.Drawing.Point(0, 0);
			this.Item_Info_Box.Name = "Item_Info_Box";
			this.Item_Info_Box.Size = new System.Drawing.Size(237, 539);
			this.Item_Info_Box.TabIndex = 4;
			this.Item_Info_Box.TabStop = false;
			this.Item_Info_Box.Text = "Item Information";
			// 
			// Excellent_Options_Box
			// 
			this.Excellent_Options_Box.Controls.Add(this.Btn_Excellent_Not);
			this.Excellent_Options_Box.Controls.Add(this.Btn_Excellent_All);
			this.Excellent_Options_Box.Controls.Add(this.Excellent6_Box);
			this.Excellent_Options_Box.Controls.Add(this.Excellent5_Box);
			this.Excellent_Options_Box.Controls.Add(this.Excellent4_Box);
			this.Excellent_Options_Box.Controls.Add(this.Excellent3_Box);
			this.Excellent_Options_Box.Controls.Add(this.Excellent2_Box);
			this.Excellent_Options_Box.Controls.Add(this.Excellent1_Box);
			this.Excellent_Options_Box.Location = new System.Drawing.Point(6, 289);
			this.Excellent_Options_Box.Name = "Excellent_Options_Box";
			this.Excellent_Options_Box.Size = new System.Drawing.Size(225, 244);
			this.Excellent_Options_Box.TabIndex = 39;
			this.Excellent_Options_Box.TabStop = false;
			this.Excellent_Options_Box.Text = "Excellent Options";
			// 
			// Btn_Excellent_Not
			// 
			this.Btn_Excellent_Not.Cursor = System.Windows.Forms.Cursors.Hand;
			this.Btn_Excellent_Not.Location = new System.Drawing.Point(135, 208);
			this.Btn_Excellent_Not.Name = "Btn_Excellent_Not";
			this.Btn_Excellent_Not.Size = new System.Drawing.Size(75, 23);
			this.Btn_Excellent_Not.TabIndex = 15;
			this.Btn_Excellent_Not.Text = "Unselect All";
			this.Btn_Excellent_Not.UseVisualStyleBackColor = true;
			this.Btn_Excellent_Not.Click += new System.EventHandler(this.Btn_Excellent_Unselect_All);
			// 
			// Btn_Excellent_All
			// 
			this.Btn_Excellent_All.Cursor = System.Windows.Forms.Cursors.Hand;
			this.Btn_Excellent_All.Location = new System.Drawing.Point(16, 208);
			this.Btn_Excellent_All.Name = "Btn_Excellent_All";
			this.Btn_Excellent_All.Size = new System.Drawing.Size(75, 23);
			this.Btn_Excellent_All.TabIndex = 14;
			this.Btn_Excellent_All.Text = "Select All";
			this.Btn_Excellent_All.UseVisualStyleBackColor = true;
			this.Btn_Excellent_All.Click += new System.EventHandler(this.Btn_Excellent_Select_All);
			// 
			// Excellent6_Box
			// 
			this.Excellent6_Box.AutoSize = true;
			this.Excellent6_Box.Cursor = System.Windows.Forms.Cursors.Hand;
			this.Excellent6_Box.Font = new System.Drawing.Font("Consolas", 7F);
			this.Excellent6_Box.Location = new System.Drawing.Point(10, 176);
			this.Excellent6_Box.Name = "Excellent6_Box";
			this.Excellent6_Box.Size = new System.Drawing.Size(59, 16);
			this.Excellent6_Box.TabIndex = 13;
			this.Excellent6_Box.Text = "No Info";
			this.Excellent6_Box.TextAlign = System.Drawing.ContentAlignment.TopLeft;
			this.Excellent6_Box.UseVisualStyleBackColor = true;
			// 
			// Excellent5_Box
			// 
			this.Excellent5_Box.AutoSize = true;
			this.Excellent5_Box.Cursor = System.Windows.Forms.Cursors.Hand;
			this.Excellent5_Box.Font = new System.Drawing.Font("Consolas", 7F);
			this.Excellent5_Box.Location = new System.Drawing.Point(10, 146);
			this.Excellent5_Box.Name = "Excellent5_Box";
			this.Excellent5_Box.Size = new System.Drawing.Size(59, 16);
			this.Excellent5_Box.TabIndex = 12;
			this.Excellent5_Box.Text = "No Info";
			this.Excellent5_Box.TextAlign = System.Drawing.ContentAlignment.TopLeft;
			this.Excellent5_Box.UseVisualStyleBackColor = true;
			// 
			// Excellent4_Box
			// 
			this.Excellent4_Box.AutoSize = true;
			this.Excellent4_Box.Cursor = System.Windows.Forms.Cursors.Hand;
			this.Excellent4_Box.Font = new System.Drawing.Font("Consolas", 7F);
			this.Excellent4_Box.Location = new System.Drawing.Point(10, 116);
			this.Excellent4_Box.Name = "Excellent4_Box";
			this.Excellent4_Box.Size = new System.Drawing.Size(59, 16);
			this.Excellent4_Box.TabIndex = 11;
			this.Excellent4_Box.Text = "No Info";
			this.Excellent4_Box.TextAlign = System.Drawing.ContentAlignment.TopLeft;
			this.Excellent4_Box.UseVisualStyleBackColor = true;
			// 
			// Excellent3_Box
			// 
			this.Excellent3_Box.AutoSize = true;
			this.Excellent3_Box.Cursor = System.Windows.Forms.Cursors.Hand;
			this.Excellent3_Box.Font = new System.Drawing.Font("Consolas", 7F);
			this.Excellent3_Box.Location = new System.Drawing.Point(10, 86);
			this.Excellent3_Box.Name = "Excellent3_Box";
			this.Excellent3_Box.Size = new System.Drawing.Size(59, 16);
			this.Excellent3_Box.TabIndex = 10;
			this.Excellent3_Box.Text = "No Info";
			this.Excellent3_Box.TextAlign = System.Drawing.ContentAlignment.TopLeft;
			this.Excellent3_Box.UseVisualStyleBackColor = true;
			// 
			// Excellent2_Box
			// 
			this.Excellent2_Box.AutoSize = true;
			this.Excellent2_Box.Cursor = System.Windows.Forms.Cursors.Hand;
			this.Excellent2_Box.Font = new System.Drawing.Font("Consolas", 7F);
			this.Excellent2_Box.Location = new System.Drawing.Point(10, 56);
			this.Excellent2_Box.Name = "Excellent2_Box";
			this.Excellent2_Box.Size = new System.Drawing.Size(59, 16);
			this.Excellent2_Box.TabIndex = 9;
			this.Excellent2_Box.Text = "No Info";
			this.Excellent2_Box.TextAlign = System.Drawing.ContentAlignment.TopLeft;
			this.Excellent2_Box.UseVisualStyleBackColor = true;
			// 
			// Excellent1_Box
			// 
			this.Excellent1_Box.AutoSize = true;
			this.Excellent1_Box.BackColor = System.Drawing.Color.LightGray;
			this.Excellent1_Box.Cursor = System.Windows.Forms.Cursors.Hand;
			this.Excellent1_Box.Font = new System.Drawing.Font("Consolas", 7F);
			this.Excellent1_Box.Location = new System.Drawing.Point(10, 26);
			this.Excellent1_Box.Name = "Excellent1_Box";
			this.Excellent1_Box.Size = new System.Drawing.Size(59, 16);
			this.Excellent1_Box.TabIndex = 8;
			this.Excellent1_Box.Text = "No Info";
			this.Excellent1_Box.TextAlign = System.Drawing.ContentAlignment.TopLeft;
			this.Excellent1_Box.UseVisualStyleBackColor = true;
			// 
			// Luck_Box
			// 
			this.Luck_Box.AutoSize = true;
			this.Luck_Box.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.Luck_Box.Cursor = System.Windows.Forms.Cursors.Hand;
			this.Luck_Box.Font = new System.Drawing.Font("Consolas", 12F);
			this.Luck_Box.Location = new System.Drawing.Point(78, 260);
			this.Luck_Box.Name = "Luck_Box";
			this.Luck_Box.Size = new System.Drawing.Size(73, 23);
			this.Luck_Box.TabIndex = 7;
			this.Luck_Box.Text = "Luck:";
			this.Luck_Box.UseVisualStyleBackColor = true;
			// 
			// Skill_Box
			// 
			this.Skill_Box.AutoSize = true;
			this.Skill_Box.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.Skill_Box.Cursor = System.Windows.Forms.Cursors.Hand;
			this.Skill_Box.Font = new System.Drawing.Font("Consolas", 12F);
			this.Skill_Box.Location = new System.Drawing.Point(75, 180);
			this.Skill_Box.Name = "Skill_Box";
			this.Skill_Box.Size = new System.Drawing.Size(82, 23);
			this.Skill_Box.TabIndex = 5;
			this.Skill_Box.Text = "Skill:";
			this.Skill_Box.UseVisualStyleBackColor = true;
			// 
			// Durability_txt
			// 
			this.Durability_txt.AutoSize = true;
			this.Durability_txt.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Durability_txt.Location = new System.Drawing.Point(45, 102);
			this.Durability_txt.Name = "Durability_txt";
			this.Durability_txt.Size = new System.Drawing.Size(56, 17);
			this.Durability_txt.TabIndex = 32;
			this.Durability_txt.Text = "Durab:";
			// 
			// Durability_Box
			// 
			this.Durability_Box.BackColor = System.Drawing.Color.White;
			this.Durability_Box.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.Durability_Box.Font = new System.Drawing.Font("Consolas", 12F);
			this.Durability_Box.ForeColor = System.Drawing.Color.Black;
			this.Durability_Box.Location = new System.Drawing.Point(107, 100);
			this.Durability_Box.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
			this.Durability_Box.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
			this.Durability_Box.Name = "Durability_Box";
			this.Durability_Box.Size = new System.Drawing.Size(77, 22);
			this.Durability_Box.TabIndex = 3;
			this.Durability_Box.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			// 
			// Option_Box
			// 
			this.Option_Box.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
			this.Option_Box.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
			this.Option_Box.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.Option_Box.Font = new System.Drawing.Font("Consolas", 11F);
			this.Option_Box.FormattingEnabled = true;
			this.Option_Box.Location = new System.Drawing.Point(106, 220);
			this.Option_Box.Name = "Option_Box";
			this.Option_Box.Size = new System.Drawing.Size(77, 26);
			this.Option_Box.TabIndex = 6;
			// 
			// Level_Box
			// 
			this.Level_Box.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
			this.Level_Box.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
			this.Level_Box.Cursor = System.Windows.Forms.Cursors.Hand;
			this.Level_Box.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.Level_Box.Font = new System.Drawing.Font("Consolas", 11F);
			this.Level_Box.FormattingEnabled = true;
			this.Level_Box.Location = new System.Drawing.Point(106, 140);
			this.Level_Box.Name = "Level_Box";
			this.Level_Box.Size = new System.Drawing.Size(77, 26);
			this.Level_Box.TabIndex = 4;
			// 
			// Option_txt
			// 
			this.Option_txt.AutoSize = true;
			this.Option_txt.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Option_txt.Location = new System.Drawing.Point(36, 225);
			this.Option_txt.Name = "Option_txt";
			this.Option_txt.Size = new System.Drawing.Size(64, 17);
			this.Option_txt.TabIndex = 28;
			this.Option_txt.Text = "Option:";
			// 
			// Level_txt
			// 
			this.Level_txt.AutoSize = true;
			this.Level_txt.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Level_txt.Location = new System.Drawing.Point(44, 144);
			this.Level_txt.Name = "Level_txt";
			this.Level_txt.Size = new System.Drawing.Size(56, 17);
			this.Level_txt.TabIndex = 26;
			this.Level_txt.Text = "Level:";
			// 
			// Item_Box
			// 
			this.Item_Box.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
			this.Item_Box.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
			this.Item_Box.Cursor = System.Windows.Forms.Cursors.Hand;
			this.Item_Box.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.Item_Box.Font = new System.Drawing.Font("Consolas", 8F);
			this.Item_Box.FormattingEnabled = true;
			this.Item_Box.Location = new System.Drawing.Point(78, 60);
			this.Item_Box.Name = "Item_Box";
			this.Item_Box.Size = new System.Drawing.Size(153, 21);
			this.Item_Box.TabIndex = 2;
			this.Item_Box.SelectedIndexChanged += new System.EventHandler(this.Item_Box_SelectedIndexChanged);
			// 
			// Item_txt
			// 
			this.Item_txt.AutoSize = true;
			this.Item_txt.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Item_txt.Location = new System.Drawing.Point(24, 61);
			this.Item_txt.Name = "Item_txt";
			this.Item_txt.Size = new System.Drawing.Size(48, 17);
			this.Item_txt.TabIndex = 22;
			this.Item_txt.Text = "Item:";
			// 
			// Section_box
			// 
			this.Section_box.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
			this.Section_box.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
			this.Section_box.Cursor = System.Windows.Forms.Cursors.Hand;
			this.Section_box.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.Section_box.Font = new System.Drawing.Font("Consolas", 8F);
			this.Section_box.FormattingEnabled = true;
			this.Section_box.Location = new System.Drawing.Point(78, 20);
			this.Section_box.Name = "Section_box";
			this.Section_box.Size = new System.Drawing.Size(153, 21);
			this.Section_box.TabIndex = 1;
			this.Section_box.SelectedIndexChanged += new System.EventHandler(this.Section_box_SelectedIndexChanged);
			// 
			// Section_txt
			// 
			this.Section_txt.AutoSize = true;
			this.Section_txt.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Section_txt.Location = new System.Drawing.Point(6, 21);
			this.Section_txt.Name = "Section_txt";
			this.Section_txt.Size = new System.Drawing.Size(72, 17);
			this.Section_txt.TabIndex = 20;
			this.Section_txt.Text = "Section:";
			// 
			// ItemEditor
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(237, 539);
			this.Controls.Add(this.Item_Info_Box);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "ItemEditor";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Load += new System.EventHandler(this.ItemEditor_Load);
			this.Item_Info_Box.ResumeLayout(false);
			this.Item_Info_Box.PerformLayout();
			this.Excellent_Options_Box.ResumeLayout(false);
			this.Excellent_Options_Box.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.Durability_Box)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.GroupBox Item_Info_Box;
		private System.Windows.Forms.GroupBox Excellent_Options_Box;
		private System.Windows.Forms.Button Btn_Excellent_Not;
		private System.Windows.Forms.Button Btn_Excellent_All;
		private System.Windows.Forms.CheckBox Excellent6_Box;
		private System.Windows.Forms.CheckBox Excellent5_Box;
		private System.Windows.Forms.CheckBox Excellent4_Box;
		private System.Windows.Forms.CheckBox Excellent3_Box;
		private System.Windows.Forms.CheckBox Excellent2_Box;
		private System.Windows.Forms.CheckBox Excellent1_Box;
		private System.Windows.Forms.CheckBox Luck_Box;
		private System.Windows.Forms.CheckBox Skill_Box;
		private System.Windows.Forms.Label Durability_txt;
		private System.Windows.Forms.NumericUpDown Durability_Box;
		private System.Windows.Forms.ComboBox Option_Box;
		private System.Windows.Forms.ComboBox Level_Box;
		private System.Windows.Forms.Label Option_txt;
		private System.Windows.Forms.Label Level_txt;
		private System.Windows.Forms.ComboBox Item_Box;
		private System.Windows.Forms.Label Item_txt;
		private System.Windows.Forms.ComboBox Section_box;
		private System.Windows.Forms.Label Section_txt;
	}
}
