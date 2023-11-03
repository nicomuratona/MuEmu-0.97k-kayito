using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

namespace kayito_Editor.Source
{
	public class InitReader
	{
		[DllImport("kernel32", CharSet = CharSet.None, ExactSpelling = false)]
		private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal, int size, string filePath);

		[DllImport("kernel32", CharSet = CharSet.None, ExactSpelling = false)]
		private static extern int GetPrivateProfileInt(string section, string key, int def, string filePath);

		public string ReadString(string Section, string Key, string DefValue)
		{
			StringBuilder stringBuilder = new StringBuilder(255);

			if (GetPrivateProfileString(Section, Key, "", stringBuilder, 255, ".\\Config.ini") <= 0)
			{
				return DefValue;
			}

			return stringBuilder.ToString();
		}

		public InitReader()
		{
			string path = ".\\Config.ini";

			try
			{
				if (!File.Exists(path))
				{
					throw new NullReferenceException("Config.ini not found.");
				}

				Import.MU_TRUSTED = GetPrivateProfileInt("MuOnline", "EnableTrusted", 1, path);
				Import.MU_SERVER = ReadString("MuOnline", "SERVER", "(local)");
				Import.MU_PORT = ReadString("MuOnline", "PORT", "1433");
				Import.MU_DB = ReadString("MuOnline", "MU_DB", "MuOnline");
				Import.MU_DB_USER = ReadString("MuOnline", "USER", "sa");
				Import.MU_DB_PASS = ReadString("MuOnline", "PASS", "");

				Import.USE_ME = GetPrivateProfileInt("Me_MuOnline", "USE_ME", 0, path);
				Import.ME_TRUSTED = GetPrivateProfileInt("Me_MuOnline", "EnableTrusted", 1, path);
				Import.ME_SERVER = ReadString("Me_MuOnline", "SERVER", "(local)");
				Import.ME_PORT = ReadString("Me_MuOnline", "PORT", "1433");
				Import.ME_DB = ReadString("Me_MuOnline", "ME_DB", "MuOnline");
				Import.ME_DB_USER = ReadString("Me_MuOnline", "USER", "sa");
				Import.ME_DB_PASS = ReadString("Me_MuOnline", "PASS", "");
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message);

				Environment.Exit(0);
			}
		}
	}
}