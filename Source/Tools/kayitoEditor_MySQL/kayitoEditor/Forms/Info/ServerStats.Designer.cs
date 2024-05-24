namespace kayito_Editor.Forms
{
    partial class ServerStats
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ServerStats));
			this.ServerStats_Box = new System.Windows.Forms.GroupBox();
			this.Characters_List = new System.Windows.Forms.ListView();
			this.Class = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.Rate = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.Onlines_Cant = new System.Windows.Forms.Label();
			this.Characters_Cant = new System.Windows.Forms.Label();
			this.Accounts_Cant = new System.Windows.Forms.Label();
			this.Onlines_txt = new System.Windows.Forms.Label();
			this.Characters_txt = new System.Windows.Forms.Label();
			this.Accounts_txt = new System.Windows.Forms.Label();
			this.ServerStats_Box.SuspendLayout();
			this.SuspendLayout();
			// 
			// ServerStats_Box
			// 
			this.ServerStats_Box.Controls.Add(this.Characters_List);
			this.ServerStats_Box.Controls.Add(this.Onlines_Cant);
			this.ServerStats_Box.Controls.Add(this.Characters_Cant);
			this.ServerStats_Box.Controls.Add(this.Accounts_Cant);
			this.ServerStats_Box.Controls.Add(this.Onlines_txt);
			this.ServerStats_Box.Controls.Add(this.Characters_txt);
			this.ServerStats_Box.Controls.Add(this.Accounts_txt);
			this.ServerStats_Box.Location = new System.Drawing.Point(12, 12);
			this.ServerStats_Box.Name = "ServerStats_Box";
			this.ServerStats_Box.Size = new System.Drawing.Size(260, 437);
			this.ServerStats_Box.TabIndex = 0;
			this.ServerStats_Box.TabStop = false;
			// 
			// Characters_List
			// 
			this.Characters_List.BackColor = System.Drawing.SystemColors.ActiveBorder;
			this.Characters_List.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.Class,
            this.Rate});
			this.Characters_List.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
			this.Characters_List.HideSelection = false;
			this.Characters_List.Location = new System.Drawing.Point(6, 104);
			this.Characters_List.Name = "Characters_List";
			this.Characters_List.Size = new System.Drawing.Size(248, 272);
			this.Characters_List.TabIndex = 7;
			this.Characters_List.TabStop = false;
			this.Characters_List.UseCompatibleStateImageBehavior = false;
			this.Characters_List.View = System.Windows.Forms.View.Details;
			// 
			// Class
			// 
			this.Class.Text = "Class";
			this.Class.Width = 135;
			// 
			// Rate
			// 
			this.Rate.Text = "Rate (Quant)";
			this.Rate.Width = 90;
			// 
			// Onlines_Cant
			// 
			this.Onlines_Cant.AutoSize = true;
			this.Onlines_Cant.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Onlines_Cant.ForeColor = System.Drawing.Color.Red;
			this.Onlines_Cant.Location = new System.Drawing.Point(145, 407);
			this.Onlines_Cant.Name = "Onlines_Cant";
			this.Onlines_Cant.Size = new System.Drawing.Size(16, 17);
			this.Onlines_Cant.TabIndex = 3;
			this.Onlines_Cant.Text = "0";
			// 
			// Characters_Cant
			// 
			this.Characters_Cant.AutoSize = true;
			this.Characters_Cant.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Characters_Cant.ForeColor = System.Drawing.Color.Red;
			this.Characters_Cant.Location = new System.Drawing.Point(145, 60);
			this.Characters_Cant.Name = "Characters_Cant";
			this.Characters_Cant.Size = new System.Drawing.Size(16, 17);
			this.Characters_Cant.TabIndex = 2;
			this.Characters_Cant.Text = "0";
			// 
			// Accounts_Cant
			// 
			this.Accounts_Cant.AutoSize = true;
			this.Accounts_Cant.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Accounts_Cant.ForeColor = System.Drawing.Color.Red;
			this.Accounts_Cant.Location = new System.Drawing.Point(145, 29);
			this.Accounts_Cant.Name = "Accounts_Cant";
			this.Accounts_Cant.Size = new System.Drawing.Size(16, 17);
			this.Accounts_Cant.TabIndex = 1;
			this.Accounts_Cant.Text = "0";
			// 
			// Onlines_txt
			// 
			this.Onlines_txt.AutoSize = true;
			this.Onlines_txt.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Onlines_txt.Location = new System.Drawing.Point(58, 407);
			this.Onlines_txt.Name = "Onlines_txt";
			this.Onlines_txt.Size = new System.Drawing.Size(64, 17);
			this.Onlines_txt.TabIndex = 3;
			this.Onlines_txt.Text = "Online:";
			// 
			// Characters_txt
			// 
			this.Characters_txt.AutoSize = true;
			this.Characters_txt.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Characters_txt.Location = new System.Drawing.Point(26, 60);
			this.Characters_txt.Name = "Characters_txt";
			this.Characters_txt.Size = new System.Drawing.Size(96, 17);
			this.Characters_txt.TabIndex = 2;
			this.Characters_txt.Text = "Characters:";
			// 
			// Accounts_txt
			// 
			this.Accounts_txt.AutoSize = true;
			this.Accounts_txt.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Accounts_txt.Location = new System.Drawing.Point(42, 29);
			this.Accounts_txt.Name = "Accounts_txt";
			this.Accounts_txt.Size = new System.Drawing.Size(80, 17);
			this.Accounts_txt.TabIndex = 1;
			this.Accounts_txt.Text = "Accounts:";
			// 
			// ServerStats
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.SystemColors.ActiveBorder;
			this.ClientSize = new System.Drawing.Size(284, 461);
			this.Controls.Add(this.ServerStats_Box);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "ServerStats";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Server Statistics";
			this.Load += new System.EventHandler(this.ServerStats_Load);
			this.ServerStats_Box.ResumeLayout(false);
			this.ServerStats_Box.PerformLayout();
			this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox ServerStats_Box;
        private System.Windows.Forms.Label Characters_txt;
        private System.Windows.Forms.Label Accounts_txt;
        private System.Windows.Forms.Label Onlines_txt;
        private System.Windows.Forms.Label Onlines_Cant;
        private System.Windows.Forms.Label Characters_Cant;
        private System.Windows.Forms.Label Accounts_Cant;
        private System.Windows.Forms.ListView Characters_List;
        private System.Windows.Forms.ColumnHeader Class;
        private System.Windows.Forms.ColumnHeader Rate;
    }
}