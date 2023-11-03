namespace kayito_Editor.Forms
{
    partial class AccountManager
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AccountManager));
			this.Account_Container = new System.Windows.Forms.GroupBox();
			this.Code_Box = new System.Windows.Forms.TextBox();
			this.Name_Box = new System.Windows.Forms.TextBox();
			this.Password_Box = new System.Windows.Forms.TextBox();
			this.Account_Box = new System.Windows.Forms.TextBox();
			this.Btn_Clear = new System.Windows.Forms.Button();
			this.Btn_Close = new System.Windows.Forms.Button();
			this.Btn_Create = new System.Windows.Forms.Button();
			this.Code_txt = new System.Windows.Forms.Label();
			this.Name_txt = new System.Windows.Forms.Label();
			this.Password_txt = new System.Windows.Forms.Label();
			this.Account_txt = new System.Windows.Forms.Label();
			this.Account_Container.SuspendLayout();
			this.SuspendLayout();
			// 
			// Account_Container
			// 
			this.Account_Container.Controls.Add(this.Code_Box);
			this.Account_Container.Controls.Add(this.Name_Box);
			this.Account_Container.Controls.Add(this.Password_Box);
			this.Account_Container.Controls.Add(this.Account_Box);
			this.Account_Container.Controls.Add(this.Btn_Clear);
			this.Account_Container.Controls.Add(this.Btn_Close);
			this.Account_Container.Controls.Add(this.Btn_Create);
			this.Account_Container.Controls.Add(this.Code_txt);
			this.Account_Container.Controls.Add(this.Name_txt);
			this.Account_Container.Controls.Add(this.Password_txt);
			this.Account_Container.Controls.Add(this.Account_txt);
			this.Account_Container.Font = new System.Drawing.Font("Lucida Console", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Account_Container.Location = new System.Drawing.Point(12, 12);
			this.Account_Container.Name = "Account_Container";
			this.Account_Container.Size = new System.Drawing.Size(206, 233);
			this.Account_Container.TabIndex = 0;
			this.Account_Container.TabStop = false;
			this.Account_Container.Text = "Account Manager";
			// 
			// Code_Box
			// 
			this.Code_Box.Location = new System.Drawing.Point(92, 150);
			this.Code_Box.MaxLength = 18;
			this.Code_Box.Name = "Code_Box";
			this.Code_Box.Size = new System.Drawing.Size(108, 18);
			this.Code_Box.TabIndex = 4;
			this.Code_Box.Text = "111111111111111111";
			// 
			// Name_Box
			// 
			this.Name_Box.Location = new System.Drawing.Point(92, 110);
			this.Name_Box.MaxLength = 10;
			this.Name_Box.Name = "Name_Box";
			this.Name_Box.Size = new System.Drawing.Size(108, 18);
			this.Name_Box.TabIndex = 3;
			// 
			// Password_Box
			// 
			this.Password_Box.Location = new System.Drawing.Point(92, 70);
			this.Password_Box.MaxLength = 10;
			this.Password_Box.Name = "Password_Box";
			this.Password_Box.Size = new System.Drawing.Size(108, 18);
			this.Password_Box.TabIndex = 2;
			// 
			// Account_Box
			// 
			this.Account_Box.Location = new System.Drawing.Point(92, 30);
			this.Account_Box.MaxLength = 10;
			this.Account_Box.Name = "Account_Box";
			this.Account_Box.Size = new System.Drawing.Size(108, 18);
			this.Account_Box.TabIndex = 1;
			// 
			// Btn_Clear
			// 
			this.Btn_Clear.Font = new System.Drawing.Font("Consolas", 8F);
			this.Btn_Clear.Location = new System.Drawing.Point(74, 189);
			this.Btn_Clear.Name = "Btn_Clear";
			this.Btn_Clear.Size = new System.Drawing.Size(59, 30);
			this.Btn_Clear.TabIndex = 6;
			this.Btn_Clear.Text = "Clear";
			this.Btn_Clear.UseVisualStyleBackColor = true;
			this.Btn_Clear.Click += new System.EventHandler(this.Btn_Clear_Click);
			// 
			// Btn_Close
			// 
			this.Btn_Close.Font = new System.Drawing.Font("Consolas", 8F);
			this.Btn_Close.Location = new System.Drawing.Point(141, 189);
			this.Btn_Close.Name = "Btn_Close";
			this.Btn_Close.Size = new System.Drawing.Size(59, 30);
			this.Btn_Close.TabIndex = 7;
			this.Btn_Close.Text = "Close";
			this.Btn_Close.UseVisualStyleBackColor = true;
			this.Btn_Close.Click += new System.EventHandler(this.Btn_Close_Click);
			// 
			// Btn_Create
			// 
			this.Btn_Create.Font = new System.Drawing.Font("Consolas", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Btn_Create.Location = new System.Drawing.Point(6, 189);
			this.Btn_Create.Name = "Btn_Create";
			this.Btn_Create.Size = new System.Drawing.Size(59, 30);
			this.Btn_Create.TabIndex = 5;
			this.Btn_Create.Text = "Create";
			this.Btn_Create.UseVisualStyleBackColor = true;
			this.Btn_Create.Click += new System.EventHandler(this.Btn_Create_Click);
			// 
			// Code_txt
			// 
			this.Code_txt.AutoSize = true;
			this.Code_txt.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Code_txt.Location = new System.Drawing.Point(38, 151);
			this.Code_txt.Name = "Code_txt";
			this.Code_txt.Size = new System.Drawing.Size(48, 17);
			this.Code_txt.TabIndex = 3;
			this.Code_txt.Text = "Code:";
			// 
			// Name_txt
			// 
			this.Name_txt.AutoSize = true;
			this.Name_txt.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Name_txt.Location = new System.Drawing.Point(38, 110);
			this.Name_txt.Name = "Name_txt";
			this.Name_txt.Size = new System.Drawing.Size(48, 17);
			this.Name_txt.TabIndex = 2;
			this.Name_txt.Text = "Name:";
			// 
			// Password_txt
			// 
			this.Password_txt.AutoSize = true;
			this.Password_txt.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Password_txt.Location = new System.Drawing.Point(6, 71);
			this.Password_txt.Name = "Password_txt";
			this.Password_txt.Size = new System.Drawing.Size(80, 17);
			this.Password_txt.TabIndex = 1;
			this.Password_txt.Text = "Password:";
			// 
			// Account_txt
			// 
			this.Account_txt.AutoSize = true;
			this.Account_txt.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Account_txt.Location = new System.Drawing.Point(14, 30);
			this.Account_txt.Name = "Account_txt";
			this.Account_txt.Size = new System.Drawing.Size(72, 17);
			this.Account_txt.TabIndex = 0;
			this.Account_txt.Text = "Account:";
			// 
			// AccountManager
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.Color.LightGray;
			this.ClientSize = new System.Drawing.Size(230, 257);
			this.Controls.Add(this.Account_Container);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.Name = "AccountManager";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Account Manager";
			this.Account_Container.ResumeLayout(false);
			this.Account_Container.PerformLayout();
			this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox Account_Container;
        private System.Windows.Forms.TextBox Code_Box;
        private System.Windows.Forms.TextBox Name_Box;
        private System.Windows.Forms.TextBox Password_Box;
        private System.Windows.Forms.TextBox Account_Box;
        private System.Windows.Forms.Button Btn_Clear;
        private System.Windows.Forms.Button Btn_Close;
        private System.Windows.Forms.Button Btn_Create;
        private System.Windows.Forms.Label Code_txt;
        private System.Windows.Forms.Label Name_txt;
        private System.Windows.Forms.Label Password_txt;
        private System.Windows.Forms.Label Account_txt;
    }
}