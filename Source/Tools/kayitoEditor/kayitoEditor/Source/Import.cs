using System.Collections.Generic;
using System.Data.OleDb;

namespace kayito_Editor.Source
{
	class Import
	{
		public static int MU_TRUSTED;
		public static string MU_SERVER;
		public static string MU_PORT;
		public static string MU_DB;
		public static string MU_DB_USER;
		public static string MU_DB_PASS;

		public static int USE_ME;

		public static int ME_TRUSTED;
		public static string ME_SERVER;
		public static string ME_PORT;
		public static string ME_DB;
		public static string ME_DB_USER;
		public static string ME_DB_PASS;

		public static OleDbConnection Mu_Connection;
		public static OleDbConnection Me_Connection;

		public static IDictionary<int, string> Classes = new Dictionary<int, string>()
		{
			{-1, "UNKNOWN"},
			{0, "Dark Wizard"},
			{1, "Soul Master"},
			{16, "Dark Knight"},
			{17, "Blade Knight"},
			{32, "Fairy Elf"},
			{33, "Muse Elf"},
			{48, "Magic Gladiator"},
		};

		public static IDictionary<int, string> Types = new Dictionary<int, string>()
		{
			{0, "Player"},
			{1, "Banned"},
			{32, "GameMaster"},
		};

		public static IDictionary<int, string> Maps = new Dictionary<int, string>()
		{
			{-1, "UNKNOWN"},
			{0, "Lorencia"},
			{1, "Dungeon"},
			{2, "Devias"},
			{3, "Noria"},
			{4, "Lost Tower"},
			{5, "Dare Devil"},
			{6, "Arena"},
			{7, "Atlans"},
			{8, "Tarkan"},
			{9, "Devil Square 1"},
			{10, "Icarus"},
			{11, "Blood Castle 1"},
			{12, "Blood Castle 2"},
			{13, "Blood Castle 3"},
			{14, "Blood Castle 4"},
			{15, "Blood Castle 5"},
			{16, "Blood Castle 6"}
		};

		public static int MaxAccountCharacter = 5;

		public static IDictionary<int, string> CreateCharList = new Dictionary<int, string>()
		{
			{0, "Dark Wizard"},
			{16, "Dark Knight"},
			{32, "Fairy Elf"},
			{48, "Magic Gladiator"},
		};

		public static List<string> deleteAcc = new List<string>()
		{
			"DELETE FROM AccountCharacter WHERE Id = {0}",
			"DELETE FROM Character WHERE AccountID = {0}",
			"DELETE FROM ExtWarehouse WHERE AccountID = {0}",
			"DELETE FROM GoldenCoin WHERE AccountID = {0}",
			"DELETE FROM MEMB_INFO WHERE memb___id = {0}",
			"DELETE FROM MEMB_STAT WHERE memb___id = {0}",
			"DELETE FROM warehouse WHERE AccountID = {0}",
		};

		public static List<string> deleteAllDB = new List<string>()
		{
			"TRUNCATE TABLE AccountCharacter",
			"TRUNCATE TABLE Character",
			"TRUNCATE TABLE ExtWarehouse",
			"TRUNCATE TABLE GameServerInfo",
			"TRUNCATE TABLE GoldenCoin",
			"TRUNCATE TABLE Guild",
			"TRUNCATE TABLE GuildMember",
			"TRUNCATE TABLE MEMB_INFO",
			"TRUNCATE TABLE MEMB_STAT",
			"TRUNCATE TABLE OptionData",
			"TRUNCATE TABLE RankingBloodCastle",
			"TRUNCATE TABLE RankingDevilSquare",
			"TRUNCATE TABLE ResetInfo",
			"TRUNCATE TABLE warehouse",
		};

		public static List<string> deleteAllChars = new List<string>()
		{
			"TRUNCATE TABLE AccountCharacter",
			"TRUNCATE TABLE Character",
			"TRUNCATE TABLE Guild",
			"TRUNCATE TABLE GuildMember",
			"TRUNCATE TABLE OptionData",
			"TRUNCATE TABLE RankingBloodCastle",
			"TRUNCATE TABLE RankingDevilSquare",
			"TRUNCATE TABLE ResetInfo",
		};
	}
}
