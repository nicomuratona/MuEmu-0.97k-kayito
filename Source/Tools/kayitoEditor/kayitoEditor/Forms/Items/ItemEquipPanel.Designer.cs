
namespace kayito_Editor.Forms
{
	partial class ItemEquipPanel
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ItemEquipPanel));
			this.SuspendLayout();
			// 
			// ItemEquipPanel
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackgroundImage = global::kayito_Editor.Properties.Resources.Character_Equipment;
			this.ClientSize = new System.Drawing.Size(200, 186);
			this.Cursor = System.Windows.Forms.Cursors.Cross;
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "ItemEquipPanel";
			this.Text = "ItemEquipPanel";
			this.Paint += new System.Windows.Forms.PaintEventHandler(this.ItemEquipPanel_Paint);
			this.MouseClick += new System.Windows.Forms.MouseEventHandler(this.ItemEquipPanel_MouseClick);
			this.ResumeLayout(false);

		}

		#endregion
	}
}