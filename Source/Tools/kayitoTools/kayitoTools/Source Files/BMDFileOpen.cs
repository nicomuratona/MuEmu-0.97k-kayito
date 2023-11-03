using System;
using System.Text;

namespace kayitoTools.Source_Files
{
	static class BMDFileOpen
	{
		public static ushort MAKE_NUMBERW(byte x, byte y)
		{
			return (ushort)((x & 0xFF) | ((y & 0xFF) << 8));
		}

		public static uint MAKE_NUMBERDW(ushort x, ushort y)
		{
			return (uint)((x & 0xFFFF) | ((y & 0xFFFF) << 16));
		}

		public static ulong MAKE_NUMBERQW(uint x, uint y)
		{
			return (ulong)((x & 0xFFFFFFFF) | ((y & 0xFFFFFFFF) << 32));
		}

		public static byte[] BuxConvert(byte[] Buffer, int Size)
		{
			byte[] bBuxCode = { 0xFC, 0xCF, 0xAB };

			for (int i = 0; i < Size; i++)
			{
				Buffer[i] ^= bBuxCode[i % 3];
			}

			return Buffer;
		}

		public static uint GenerateCheckSum(byte[] buffer, int dwSize, uint dwKey)
		{
			uint dwResult = dwKey << 9;

			for (int dwChecked = 0; dwChecked <= dwSize - 4; dwChecked += 4)
			{
				uint dwTemp = BitConverter.ToUInt32(buffer, dwChecked);

				switch ((dwChecked / 4 + dwKey) % 2)
				{
					case 0:
					{
						dwResult ^= dwTemp;

						break;
					}

					case 1:
					{
						dwResult += dwTemp;

						break;
					}
				}

				if ((dwChecked % 16) == 0)
				{
					dwResult ^= ((dwKey + dwResult) >> ((dwChecked / 4) % 8 + 1));
				}
			}

			return dwResult;
		}

		public static T[] SubArray<T>(T[] array, int offset, int length)
		{
			T[] result = new T[length];

			Array.Copy(array, offset, result, 0, length);

			return result;
		}

		public static byte[] StringToByteArray(string str, int length)
		{
			return Encoding.Default.GetBytes(str.PadRight(length, '\0'));
		}

		public static byte[] StringToByteArrayFF(string str, int length)
		{
			return Encoding.Default.GetBytes(str.PadRight(length, 'ÿ'));
		}
	}
}