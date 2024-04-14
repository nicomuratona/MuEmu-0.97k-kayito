using System;
using System.IO;
using System.Reflection;
using System.Windows.Forms;

namespace kayitoTools.Source_Files
{
	class Globals
	{
		public static Main gMain;

		public static string rootPath = Directory.GetCurrentDirectory();

		public static void SetDoubleBuffer(Control dgv, bool DoubleBuffered)
		{
			Type dgvType = dgv.GetType();

			PropertyInfo pi = dgvType.GetProperty("DoubleBuffered", BindingFlags.Instance | BindingFlags.NonPublic);

			pi.SetValue(dgv, DoubleBuffered, null);
		}

		public static string GetMaxStringLength(string text, int size)
		{
			text = text.Trim('\0');

			if (text.Length <= size)
			{
				return text;
			}
			else
			{
				return text.Substring(0, size);
			}
		}

		public static byte GetMaxByteValue(int value)
		{
			if (value > 255 || value < 0)
			{
				return 255;
			}

			return (byte)value;
		}

		public static int GetRealByteValue(byte value)
		{
			if (value == 255)
			{
				return -1;
			}

			return value;
		}

		public static ushort GetMaxWordValue(int value)
		{
			if (value > 65535 || value < 0)
			{
				return 65535;
			}

			return (ushort)value;
		}

		public static uint GetMaxDwordValue(int value)
		{
			return (uint)value;
		}
	}
}