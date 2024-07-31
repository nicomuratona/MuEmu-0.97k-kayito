

namespace kayito_Editor.Source
{
	public static class BitManipulation
	{
		public static byte SET_NUMBERHB(uint x)
		{
			return (byte)(x >> 8);
		}

		public static byte SET_NUMBERLB(uint x)
		{
			return (byte)(x & 0xFF);
		}

		public static ushort SET_NUMBERHW(uint x)
		{
			return (ushort)(x >> 16);
		}

		public static ushort SET_NUMBERLW(uint x)
		{
			return (ushort)(x & 0xFFFF);
		}

		public static uint SET_NUMBERHDW(ulong x)
		{
			return (uint)(x >> 32);
		}

		public static uint SET_NUMBERLDW(ulong x)
		{
			return (uint)(x & 0xFFFFFFFF);
		}

		public static ushort MAKE_NUMBERW(byte x, byte y)
		{
			return (ushort)((y & 0xFF) | ((x & 0xFF) << 8));
		}

		public static uint MAKE_NUMBERDW(ushort x, ushort y)
		{
			return (uint)((y & 0xFFFF) | ((x & 0xFFFF) << 16));
		}

		public static ulong MAKE_NUMBERQW(uint x, uint y)
		{
			return (ulong)((y & 0xFFFFFFFF) | ((x & 0xFFFFFFFF) << 32));
		}
	}
}