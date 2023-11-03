
namespace kayitoTools.Forms
{
	class QuestManager
	{
		public static int MAX_QUESTS = 200;

		public static int QUEST_BMD_SIZE = 584;
		public static int QUEST_CLASS_ACT_SIZE = 18;
		public static int QUEST_CLASS_REQUEST_SIZE = 16;
		/*
		+00 WORD QuestConditionNumber;
		+02 WORD QuestRequestNumber;
		+04 BYTE NpcNumber;
		+05 char QuestName[32];
		+37 BYTE FILL
		QUEST_CLASS_ACT QuestAct[16];
			+38 BYTE Live;
			+39 BYTE QuestType;
			+40 BYTE ItemType;
			+41 BYTE ItemSubType;
			+42 BYTE ItemNum;
			+43 BYTE RequestType;
			+44 BYTE RequestClass[MAX_CLASS];
			+48 WORD QuestStartText[4];

		+326 BYTE FILL (Pragma Pack 4)
		+327 BYTE FILL (Pragma Pack 4)

		QUEST_CLASS_REQUEST QuestRequest[16];
			+328 BYTE Live;
			+329 BYTE Type;
			+330 WORD CompleteQuestIndex;
			+332 WORD LevelMin;
			+334 WORD LevelMax;
			+336 DWORD Zen;
			+340 WORD ErrorText;
			+342 BYTE FILL
			+343 BYTE FILL
		*/
	}
}