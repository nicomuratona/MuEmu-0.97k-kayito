#include "stdafx.h"
#include "NpcTalk.h"
#include "QueryManager.h"
#include "SocketManager.h"

CNpcTalk gNpcTalk;

CNpcTalk::CNpcTalk()
{

}

CNpcTalk::~CNpcTalk()
{

}

void CNpcTalk::GDNpcGoldenArcherOpenRecv(SDHP_NPC_GOLDEN_ARCHER_OPEN_RECV* lpMsg, int index)
{
	SDHP_NPC_GOLDEN_ARCHER_OPEN_SEND pMsg;

	pMsg.header.set(0x04, 0x00, sizeof(pMsg));

	pMsg.type = lpMsg->type;

	pMsg.index = lpMsg->index;

#ifndef MYSQL
	if (gQueryManager.ExecQuery("SELECT * FROM GoldenCoin WHERE AccountID='%s'", lpMsg->account) == false || gQueryManager.Fetch() == SQL_NO_DATA)
#else
	if (gQueryManager.ExecResultQuery("SELECT * FROM GoldenCoin WHERE AccountID='%s'", lpMsg->account) == false || gQueryManager.Fetch() == false)
#endif
	{
		gQueryManager.Close();

		char newLuckyNumber[13];

		char Randomize[] = { "0123456789" "ABCDEFGHIJKLMNOPQRSTUVWXYZ" };

		for (int i = 0; i < 12; ++i)
		{
			newLuckyNumber[i] = Randomize[rand() % (sizeof(Randomize) - 1)];
		}

		newLuckyNumber[12] = '\0';

	#ifndef MYSQL

		gQueryManager.ExecQuery("INSERT INTO GoldenCoin (AccountID,RenaCount,StoneCount,LuckyNumber) VALUES ('%s',%d,%d,'%s')", lpMsg->account, 0, 0, newLuckyNumber);

	#else

		gQueryManager.ExecUpdateQuery("INSERT INTO GoldenCoin (AccountID, RenaCount, StoneCount, LuckyNumber) VALUES ('%s', %d, %d, '%s')", lpMsg->account, 0, 0, newLuckyNumber);

	#endif

		gQueryManager.Close();

		pMsg.count = 0;
	}
	else
	{
		if (lpMsg->type == 0)
		{
			pMsg.count = gQueryManager.GetAsInteger("RenaCount");
		}
		else
		{
			pMsg.count = gQueryManager.GetAsInteger("StoneCount");
		}

		gQueryManager.Close();
	}

	gSocketManager.DataSend(index, (BYTE*)&pMsg, sizeof(pMsg));
}

void CNpcTalk::GDNpcGoldenArcherRegisterCountRecv(SDHP_NPC_GOLDEN_ARCHER_REG_COUNT_RECV* lpMsg, int index)
{
	SDHP_NPC_GOLDEN_ARCHER_REG_COUNT_SEND pMsg;

	pMsg.header.set(0x04, 0x01, sizeof(pMsg));

	pMsg.type = lpMsg->type;

	pMsg.index = lpMsg->index;

	pMsg.count = lpMsg->count;

	pMsg.result = 0;

#ifndef MYSQL
	if (gQueryManager.ExecQuery("SELECT * FROM GoldenCoin WHERE AccountID='%s'", lpMsg->account) == false || gQueryManager.Fetch() == SQL_NO_DATA)
#else
	if (gQueryManager.ExecResultQuery("SELECT * FROM GoldenCoin WHERE AccountID='%s'", lpMsg->account) == false || gQueryManager.Fetch() == false)
#endif
	{
		gQueryManager.Close();

		char newLuckyNumber[13];

		char Randomize[] = { "0123456789" "ABCDEFGHIJKLMNOPQRSTUVWXYZ" };

		for (int i = 0; i < 12; ++i)
		{
			newLuckyNumber[i] = Randomize[rand() % (sizeof(Randomize) - 1)];
		}

		newLuckyNumber[12] = '\0';

	#ifndef MYSQL

		gQueryManager.ExecQuery("INSERT INTO GoldenCoin (AccountID,RenaCount,StoneCount,LuckyNumber) VALUES ('%s',%d,%d,'%s')", lpMsg->account, ((lpMsg->type == 0) ? lpMsg->count : 0), ((lpMsg->type == 1) ? lpMsg->count : 0), newLuckyNumber);

	#else

		gQueryManager.ExecUpdateQuery("INSERT INTO GoldenCoin (AccountID, RenaCount, StoneCount, LuckyNumber) VALUES ('%s', %d, %d, '%s')", lpMsg->account, ((lpMsg->type == 0) ? lpMsg->count : 0), ((lpMsg->type == 1) ? lpMsg->count : 0), newLuckyNumber);

	#endif

		gQueryManager.Close();

		pMsg.result = 1;

		pMsg.oldCount = 0;
	}
	else
	{
		pMsg.oldCount = ((lpMsg->type == 0) ? gQueryManager.GetAsInteger("RenaCount") : gQueryManager.GetAsInteger("StoneCount"));

		gQueryManager.GetAsString("LuckyNumber", pMsg.luckyNum, sizeof(pMsg.luckyNum));

		gQueryManager.Close();

		if (lpMsg->type == 0)
		{
		#ifndef MYSQL

			gQueryManager.ExecQuery("Update GoldenCoin SET RenaCount=RenaCount+%d WHERE AccountID = '%s'", lpMsg->count, lpMsg->account);

		#else

			gQueryManager.ExecUpdateQuery("Update GoldenCoin SET RenaCount=RenaCount+%d WHERE AccountID='%s'", lpMsg->count, lpMsg->account);

		#endif
		}
		else
		{
		#ifndef MYSQL

			gQueryManager.ExecQuery("Update GoldenCoin SET StoneCount=StoneCount+%d WHERE AccountID = '%s'", lpMsg->count, lpMsg->account);

		#else

			gQueryManager.ExecUpdateQuery("Update GoldenCoin SET StoneCount=StoneCount+%d WHERE AccountID='%s'", lpMsg->count, lpMsg->account);

		#endif
		}

		gQueryManager.Close();

		pMsg.result = 1;
	}

	gSocketManager.DataSend(index, (BYTE*)&pMsg, sizeof(pMsg));
}

void CNpcTalk::GDNpcGoldenArcherRegisterLuckyNumRecv(SDHP_NPC_GOLDEN_ARCHER_REG_LUCKYNUM_RECV* lpMsg, int index)
{
	SDHP_NPC_GOLDEN_ARCHER_REG_LUCKYNUM_SEND pMsg;

	pMsg.header.set(0x04, 0x02, sizeof(pMsg));

	pMsg.index = lpMsg->index;

	char LuckyNumber[16];

	sprintf_s(LuckyNumber, "%s%s%s", lpMsg->serial1, lpMsg->serial2, lpMsg->serial3);

#ifndef MYSQL
	if (gQueryManager.ExecQuery("SELECT * FROM GoldenCoin WHERE AccountID='%s'", lpMsg->account) == false || gQueryManager.Fetch() == SQL_NO_DATA)
#else
	if (gQueryManager.ExecResultQuery("SELECT * FROM GoldenCoin WHERE AccountID='%s'", lpMsg->account) == false || gQueryManager.Fetch() == false)
#endif
	{
		gQueryManager.Close();

		char newLuckyNumber[13];

		char Randomize[] = { "0123456789" "ABCDEFGHIJKLMNOPQRSTUVWXYZ" };

		for (int i = 0; i < 12; ++i)
		{
			newLuckyNumber[i] = Randomize[rand() % (sizeof(Randomize) - 1)];
		}

		newLuckyNumber[12] = '\0';

	#ifndef MYSQL

		gQueryManager.ExecQuery("INSERT INTO GoldenCoin (AccountID,RenaCount,StoneCount,LuckyNumber) VALUES ('%s',%d,%d,'%s')", lpMsg->account, 0, 0, newLuckyNumber);

	#else

		gQueryManager.ExecUpdateQuery("INSERT INTO GoldenCoin (AccountID, RenaCount, StoneCount, LuckyNumber) VALUES ('%s', %d, %d, '%s')", lpMsg->account, 0, 0, newLuckyNumber);

	#endif

		gQueryManager.Close();

		pMsg.result = 3;
	}
	else
	{
		char LuckyRegistered[16];

		gQueryManager.GetAsString("LuckyNumber", LuckyRegistered, sizeof(LuckyRegistered));

		gQueryManager.Close();

		if ((_stricmp(LuckyRegistered, LuckyNumber) == 0))
		{
			char newLuckyNumber[13];

			char Randomize[] = { "0123456789" "ABCDEFGHIJKLMNOPQRSTUVWXYZ" };

			for (int i = 0; i < 12; ++i)
			{
				newLuckyNumber[i] = Randomize[rand() % (sizeof(Randomize) - 1)];
			}

			newLuckyNumber[12] = '\0';

		#ifndef MYSQL

			gQueryManager.ExecQuery("Update GoldenCoin SET RenaCount = 0, StoneCount = 0, LuckyNumber = '%s' WHERE AccountID = '%s'", newLuckyNumber, lpMsg->account);

		#else

			gQueryManager.ExecUpdateQuery("Update GoldenCoin SET RenaCount=0, StoneCount=0, LuckyNumber='%s' WHERE AccountID = '%s'", newLuckyNumber, lpMsg->account);

		#endif

			gQueryManager.Close();

			pMsg.result = 0;
		}
		else
		{
			pMsg.result = 3;
		}
	}

	gSocketManager.DataSend(index, (BYTE*)&pMsg, sizeof(pMsg));
}