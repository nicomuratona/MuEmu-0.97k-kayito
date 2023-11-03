namespace kayito_Editor.Forms
{
    partial class CharacterManager
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(CharacterManager));
			this.Character_Container = new System.Windows.Forms.GroupBox();
			this.Account_List = new System.Windows.Forms.ComboBox();
			this.Class_Box = new System.Windows.Forms.ComboBox();
			this.Name_Box = new System.Windows.Forms.TextBox();
			this.Btn_Clear = new System.Windows.Forms.Button();
			this.Btn_Close = new System.Windows.Forms.Button();
			this.Btn_Create = new System.Windows.Forms.Button();
			this.Mail_txt = new System.Windows.Forms.Label();
			this.Password_txt = new System.Windows.Forms.Label();
			this.Account_txt = new System.Windows.Forms.Label();
			this.Character_Container.SuspendLayout();
			this.SuspendLayout();
			// 
			// Character_Container
			// 
			this.Character_Container.Controls.Add(this.Account_List);
			this.Character_Container.Controls.Add(this.Class_Box);
			this.Character_Container.Controls.Add(this.Name_Box);
			this.Character_Container.Controls.Add(this.Btn_Clear);
			this.Character_Container.Controls.Add(this.Btn_Close);
			this.Character_Container.Controls.Add(this.Btn_Create);
			this.Character_Container.Controls.Add(this.Mail_txt);
			this.Character_Container.Controls.Add(this.Password_txt);
			this.Character_Container.Controls.Add(this.Account_txt);
			this.Character_Container.Font = new System.Drawing.Font("Lucida Console", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Character_Container.Location = new System.Drawing.Point(12, 12);
			this.Character_Container.Name = "Character_Container";
			this.Character_Container.Size = new System.Drawing.Size(206, 233);
			this.Character_Container.TabIndex = 1;
			this.Character_Container.TabStop = false;
			this.Character_Container.Text = "Character Manager";
			// 
			// Account_List
			// 
			this.Account_List.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
			this.Account_List.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
			this.Account_List.FormattingEnabled = true;
			this.Account_List.Location = new System.Drawing.Point(92, 40);
			this.Account_List.Name = "Account_List";
			this.Account_List.Size = new System.Drawing.Size(108, 19);
			this.Account_List.TabIndex = 1;
			// 
			// Class_Box
			// 
			this.Class_Box.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
			this.Class_Box.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
			this.Class_Box.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.Class_Box.FormattingEnabled = true;
			this.Class_Box.Location = new System.Drawing.Point(92, 140);
			this.Class_Box.Name = "Class_Box";
			this.Class_Box.Size = new System.Drawing.Size(108, 19);
			this.Class_Box.TabIndex = 3;
			// 
			// Name_Box
			// 
			this.Name_Box.Location = new System.Drawing.Point(92, 90);
			this.Name_Box.MaxLength = 10;
			this.Name_Box.Name = "Name_Box";
			this.Name_Box.Size = new System.Drawing.Size(108, 18);
			this.Name_Box.TabIndex = 2;
			// 
			// Btn_Clear
			// 
			this.Btn_Clear.Font = new System.Drawing.Font("Consolas", 8F);
			this.Btn_Clear.Location = new System.Drawing.Point(74, 186);
			this.Btn_Clear.Name = "Btn_Clear";
			this.Btn_Clear.Size = new System.Drawing.Size(59, 30);
			this.Btn_Clear.TabIndex = 5;
			this.Btn_Clear.Text = "Clear";
			this.Btn_Clear.UseVisualStyleBackColor = true;
			this.Btn_Clear.Click += new System.EventHandler(this.Btn_Clear_Click);
			// 
			// Btn_Close
			// 
			this.Btn_Close.Font = new System.Drawing.Font("Consolas", 8F);
			this.Btn_Close.Location = new System.Drawing.Point(141, 186);
			this.Btn_Close.Name = "Btn_Close";
			this.Btn_Close.Size = new System.Drawing.Size(59, 30);
			this.Btn_Close.TabIndex = 6;
			this.Btn_Close.Text = "Close";
			this.Btn_Close.UseVisualStyleBackColor = true;
			this.Btn_Close.Click += new System.EventHandler(this.Btn_Close_Click);
			// 
			// Btn_Create
			// 
			this.Btn_Create.Font = new System.Drawing.Font("Consolas", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Btn_Create.Location = new System.Drawing.Point(6, 186);
			this.Btn_Create.Name = "Btn_Create";
			this.Btn_Create.Size = new System.Drawing.Size(59, 30);
			this.Btn_Create.TabIndex = 4;
			this.Btn_Create.Text = "Create";
			this.Btn_Create.UseVisualStyleBackColor = true;
			this.Btn_Create.Click += new System.EventHandler(this.Btn_Create_Click);
			// 
			// Mail_txt
			// 
			this.Mail_txt.AutoSize = true;
			this.Mail_txt.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Mail_txt.Location = new System.Drawing.Point(30, 142);
			this.Mail_txt.Name = "Mail_txt";
			this.Mail_txt.Size = new System.Drawing.Size(56, 17);
			this.Mail_txt.TabIndex = 2;
			this.Mail_txt.Text = "Class:";
			// 
			// Password_txt
			// 
			this.Password_txt.AutoSize = true;
			this.Password_txt.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Password_txt.Location = new System.Drawing.Point(38, 90);
			this.Password_txt.Name = "Password_txt";
			this.Password_txt.Size = new System.Drawing.Size(48, 17);
			this.Password_txt.TabIndex = 1;
			this.Password_txt.Text = "Name:";
			// 
			// Account_txt
			// 
			this.Account_txt.AutoSize = true;
			this.Account_txt.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Account_txt.Location = new System.Drawing.Point(14, 40);
			this.Account_txt.Name = "Account_txt";
			this.Account_txt.Size = new System.Drawing.Size(72, 17);
			this.Account_txt.TabIndex = 0;
			this.Account_txt.Text = "Account:";
			// 
			// CharacterManager
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.Color.LightGray;
			this.ClientSize = new System.Drawing.Size(230, 257);
			this.Controls.Add(this.Character_Container);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.Name = "CharacterManager";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Character Manager";
			this.Character_Container.ResumeLayout(false);
			this.Character_Container.PerformLayout();
			this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox Character_Container;
        private System.Windows.Forms.TextBox Name_Box;
        private System.Windows.Forms.Button Btn_Clear;
        private System.Windows.Forms.Button Btn_Close;
        private System.Windows.Forms.Button Btn_Create;
        private System.Windows.Forms.Label Mail_txt;
        private System.Windows.Forms.Label Password_txt;
        private System.Windows.Forms.Label Account_txt;
        private System.Windows.Forms.ComboBox Class_Box;
        private System.Windows.Forms.ComboBox Account_List;
    }
}