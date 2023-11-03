namespace kayito_Editor.Forms
{
    partial class About
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(About));
			this.About_Logo = new System.Windows.Forms.PictureBox();
			this.About_Title = new System.Windows.Forms.Label();
			this.About_Copy = new System.Windows.Forms.Label();
			this.About_Collab = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			((System.ComponentModel.ISupportInitialize)(this.About_Logo)).BeginInit();
			this.SuspendLayout();
			// 
			// About_Logo
			// 
			this.About_Logo.BackgroundImage = global::kayito_Editor.Properties.Resources.icon;
			this.About_Logo.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
			this.About_Logo.Location = new System.Drawing.Point(12, 17);
			this.About_Logo.Name = "About_Logo";
			this.About_Logo.Size = new System.Drawing.Size(100, 100);
			this.About_Logo.TabIndex = 0;
			this.About_Logo.TabStop = false;
			// 
			// About_Title
			// 
			this.About_Title.AutoSize = true;
			this.About_Title.Font = new System.Drawing.Font("Lucida Console", 10F);
			this.About_Title.Location = new System.Drawing.Point(162, 17);
			this.About_Title.Name = "About_Title";
			this.About_Title.Size = new System.Drawing.Size(151, 14);
			this.About_Title.TabIndex = 1;
			this.About_Title.Text = "kayito - Mu Editor";
			// 
			// About_Copy
			// 
			this.About_Copy.AutoSize = true;
			this.About_Copy.Font = new System.Drawing.Font("Lucida Console", 10F);
			this.About_Copy.Location = new System.Drawing.Point(151, 44);
			this.About_Copy.Name = "About_Copy";
			this.About_Copy.Size = new System.Drawing.Size(167, 14);
			this.About_Copy.TabIndex = 2;
			this.About_Copy.Text = "Developed by: kayito";
			// 
			// About_Collab
			// 
			this.About_Collab.AutoSize = true;
			this.About_Collab.Font = new System.Drawing.Font("Lucida Console", 10F);
			this.About_Collab.Location = new System.Drawing.Point(145, 72);
			this.About_Collab.Name = "About_Collab";
			this.About_Collab.Size = new System.Drawing.Size(183, 14);
			this.About_Collab.TabIndex = 3;
			this.About_Collab.Text = "In collaboration with:";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Font = new System.Drawing.Font("Lucida Console", 15F);
			this.label1.ForeColor = System.Drawing.Color.Green;
			this.label1.Location = new System.Drawing.Point(170, 97);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(129, 20);
			this.label1.TabIndex = 4;
			this.label1.Text = "SoftwareRG";
			// 
			// About
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.SystemColors.ActiveBorder;
			this.ClientSize = new System.Drawing.Size(344, 136);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.About_Collab);
			this.Controls.Add(this.About_Copy);
			this.Controls.Add(this.About_Title);
			this.Controls.Add(this.About_Logo);
			this.ForeColor = System.Drawing.Color.Black;
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "About";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "About";
			((System.ComponentModel.ISupportInitialize)(this.About_Logo)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox About_Logo;
        private System.Windows.Forms.Label About_Title;
        private System.Windows.Forms.Label About_Copy;
		private System.Windows.Forms.Label About_Collab;
		private System.Windows.Forms.Label label1;
	}
}