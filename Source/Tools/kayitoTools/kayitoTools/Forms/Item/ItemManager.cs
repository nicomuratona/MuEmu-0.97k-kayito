using System.Collections.Generic;

namespace kayitoTools.Source_Files
{
	class ItemManager
	{
		public static int MAX_ITEM_GROUP = 16;
		public static int MAX_ITEM_INDEX = 32;
		public static int MAX_ITEM = MAX_ITEM_INDEX * MAX_ITEM_GROUP;

		public static IDictionary<int, string> m_ItemSection = new Dictionary<int, string>()
		{
			{0, "Swords"},
			{1, "Axes"},
			{2, "Maces/Scepters"},
			{3, "Spears"},
			{4, "Bows/Crossbows"},
			{5, "Staffs"},
			{6, "Shields"},
			{7, "Helms"},
			{8, "Armors"},
			{9, "Pants"},
			{10, "Gloves"},
			{11, "Boots"},
			{12, "Wings"},
			{13, "Pets/Rings/Pendants"},
			{14, "Consumables/Jewels"},
			{15, "Scrolls"},
		};

		public static IDictionary<int, string> m_ItemIndex = new Dictionary<int, string>();

		//ITEM_TXT_INFO
		/*
		int Index;
		int Slot;
		int Skill;
		int Width;
		int Height;
		int HaveSerial;
		int HaveOption;
		int DropItem;
		char Name[32];
		int Level;
		int DamageMin;
		int DamageMax;
		int MagicDamageRate;
		int TwoHand;
		int Defense;
		int MagicDefense;
		int DefenseSuccessRate;
		int AttackSpeed;
		int WalkSpeed;
		int Durability;
		int MagicDurability;
		int Value;
		int BuyMoney;
		int IceResis;
		int PoisonResis;
		int LightingResis;
		int FireResis;
		int RequireLevel;
		int RequireStrength;
		int RequireDexterity;
		int RequireEnergy;
		int RequireVitality;
		int RequireDW;
		int RequireDK;
		int RequireFE;
		int RequireMG;
		*/

		public static int ITEM_BMD_SIZE = 64;
		/*
		+00 char Name[30];
		+30 BYTE TwoHand;
		+31 BYTE Level;
		+32 BYTE Width;
		+33 BYTE Height;
		+34 BYTE DamageMin;
		+35 BYTE DamageMax;
		+36 BYTE DefenseRate;
		+37 BYTE Defense;
		+38 BYTE MagicDefense;
		+39 BYTE AttackSpeed;
		+40 BYTE WalkSpeed;
		+41 BYTE Durability;
		+42 BYTE MagicDurability;
		+43 BYTE MagicPower;
		+44 WORD RequireStrength;
		+46 WORD RequireAgility;
		+48 BYTE RequireEnergy;
		+49 BYTE RequireLevel;
		+50 BYTE Value;
		+51 BYTE FILL (PRAGMA PACK 4)
		+52 DWORD Money;
		+56 BYTE RequireDW;
		+57 BYTE RequireDK;
		+58 BYTE RequireELF;
		+59 BYTE RequireMG;
		+60 BYTE ResistanceIce;
		+61 BYTE ResistancePoison;
		+62 BYTE ResistanceLightning;
		+63 BYTE ResistanceFire;
		*/
	}
}