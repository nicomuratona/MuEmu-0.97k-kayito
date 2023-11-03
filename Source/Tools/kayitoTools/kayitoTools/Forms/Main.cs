using kayitoTools.Forms;
using kayitoTools.Source_Files;
using System;
using System.Windows.Forms;

namespace kayitoTools
{
	public partial class Main : Form
	{
		private Form activeForm = null;

		public Main()
		{
			InitializeComponent();

			Console.SetOut(new ControlWriter(this.Log_Text_Box));

			Globals.gMain = this;
		}

		private void OpenChildForm(Form newForm)
		{
			if (activeForm != null)
			{
				activeForm.Close();
			}

			activeForm = newForm;

			newForm.TopLevel = false;

			newForm.FormBorderStyle = FormBorderStyle.None;

			newForm.Dock = DockStyle.Fill;

			panelChildForm.Controls.Add(newForm);

			panelChildForm.Tag = newForm;

			panelChildForm.Visible = true;

			newForm.BringToFront();

			newForm.Show();
		}

		private void Clear_Text_Box_Btn_Click(object sender, EventArgs e)
		{
			this.Log_Text_Box.Clear();
		}

		private void Dialog_Menu_Btn_Click(object sender, EventArgs e)
		{
			OpenChildForm(new DialogEditor());
		}

		private void Filter_Menu_Btn_Click(object sender, EventArgs e)
		{
			OpenChildForm(new FilterEditor());
		}

		private void Gate_Menu_Btn_Click(object sender, EventArgs e)
		{
			OpenChildForm(new GateEditor());
		}

		private void Item_Menu_Btn_Click(object sender, EventArgs e)
		{
			OpenChildForm(new ItemEditor());
		}

		private void Monster_Menu_Btn_Click(object sender, EventArgs e)
		{
			OpenChildForm(new MonsterEditor());
		}

		private void Quest_Menu_Btn_Click(object sender, EventArgs e)
		{
			OpenChildForm(new QuestEditor());
		}

		private void Skill_Menu_Btn_Click(object sender, EventArgs e)
		{
			OpenChildForm(new SkillEditor());
		}

		private void Text_Menu_Btn_Click(object sender, EventArgs e)
		{
			OpenChildForm(new TextEditor());
		}
	}
}