using System.IO;
using System.Text;
using System.Windows.Forms;

namespace kayitoTools.Source_Files
{
	public class ControlWriter : TextWriter
	{
		private readonly TextBox textbox;
		public ControlWriter(TextBox textbox)
		{
			this.textbox = textbox;
		}

		public override void Write(char value)
		{
			this.textbox.AppendText(value.ToString());
		}

		public override void Write(string value)
		{
			this.textbox.AppendText(value);
		}

		public override Encoding Encoding
		{
			get
			{
				return Encoding.ASCII;
			}
		}
	}
}