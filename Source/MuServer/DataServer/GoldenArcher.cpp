#include "stdafx.h"
#include "GoldenArcher.h"
#include "QueryManager.h"
#include "SocketManager.h"

CGoldenArcher gGoldenArcher;

CGoldenArcher::CGoldenArcher()
{

}

CGoldenArcher::~CGoldenArcher()
{

}

void CGoldenArcher::GDGoldenArcherGetValuesRecv(SDHP_GOLDEN_ARCHER_GET_VALUES_RECV* lpMsg, int index)
{
	BYTE send[8192];

	SDHP_GOLDEN_ARCHER_GET_VALUES_SEND pMsg;

	pMsg.header.set(0x04, 0x00, 0);

	int size = sizeof(pMsg);

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account, lpMsg->account, sizeof(pMsg.account));

#ifndef MYSQL
	if (gQueryManager.ExecQuery("SELECT * FROM GoldenArcherCoin WHERE AccountID = '%s'", lpMsg->account) == false || gQueryManager.Fetch() == SQL_NO_DATA)
	#else
	if (gQueryManager.ExecResultQuery("SELECT * FROM GoldenArcherCoin WHERE AccountID = '%s'", lpMsg->account) == false || gQueryManager.Fetch() == false)
	#endif
	{
		gQueryManager.Close();

	#ifndef MYSQL

		gQueryManager.ExecQuery("INSERT INTO GoldenArcherCoin (AccountID) VALUES ('%s')", lpMsg->account);

	#else

		gQueryManager.ExecUpdateQuery("INSERT INTO GoldenArcherCoin (AccountID) VALUES ('%s')", lpMsg->account);

	#endif

		gQueryManager.Close();

		pMsg.count[GOLDEN_ARCHER_COIN_RENA] = 0;

		pMsg.count[GOLDEN_ARCHER_COIN_STONE] = 0;

		memset(pMsg.LuckyNumber, 0, sizeof(pMsg.LuckyNumber));
	}
	else
	{
		pMsg.count[GOLDEN_ARCHER_COIN_RENA] = gQueryManager.GetAsInteger("Renas");

		pMsg.count[GOLDEN_ARCHER_COIN_STONE] = gQueryManager.GetAsInteger("Stones");

		gQueryManager.GetAsString("LuckyNumber", pMsg.LuckyNumber, sizeof(pMsg.LuckyNumber));

		gQueryManager.Close();
	}

	pMsg.registered = 0;

	LUCKY_NUMBERS_REGISTERED info;

#ifndef MYSQL
	if (gQueryManager.ExecQuery("SELECT * FROM GoldenArcherLuckyNumbers WHERE AccountID = '%s'", lpMsg->account))
	#else
	if (gQueryManager.ExecResultQuery("SELECT * FROM GoldenArcherLuckyNumbers WHERE AccountID = '%s'", lpMsg->account))
	#endif
	{
	#ifndef MYSQL
		while (gQueryManager.Fetch() != SQL_NO_DATA)
		#else
		while (gQueryManager.Fetch())
		#endif
		{
			gQueryManager.GetAsString("LuckyNumber", info.LuckyNumber, sizeof(info.LuckyNumber));

			memcpy(&send[size], &info, sizeof(info));

			size += sizeof(info);

			pMsg.registered++;
		}
	}

	gQueryManager.Close();

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	gSocketManager.DataSend(index, send, size);
}

void CGoldenArcher::GDGoldenArcherSaveCoinRecv(SDHP_GOLDEN_ARCHER_SAVE_COIN_RECV* lpMsg)
{
#ifndef MYSQL

	gQueryManager.ExecQuery("UPDATE GoldenArcherCoin SET Renas = %d, Stones = %d WHERE AccountID = '%s'", lpMsg->count[GOLDEN_ARCHER_COIN_RENA], lpMsg->count[GOLDEN_ARCHER_COIN_STONE], lpMsg->account);

	gQueryManager.Close();

#else

	gQueryManager.ExecUpdateQuery("UPDATE GoldenArcherCoin SET Renas = %d, Stones = %d WHERE AccountID = '%s'", lpMsg->count[GOLDEN_ARCHER_COIN_RENA], lpMsg->count[GOLDEN_ARCHER_COIN_STONE], lpMsg->account);

	gQueryManager.Close();

#endif
}

void CGoldenArcher::GDGoldenArcherSaveLuckyNumberRecv(SDHP_GOLDEN_ARCHER_SAVE_LUCKY_NUMBER_RECV* lpMsg)
{
#ifndef MYSQL

	if (strlen(lpMsg->LuckyNumber) > 0)
	{
		gQueryManager.ExecQuery("UPDATE GoldenArcherCoin SET LuckyNumber = '%s' WHERE AccountID = '%s'", lpMsg->LuckyNumber, lpMsg->account);
	}
	else
	{
		gQueryManager.ExecQuery("UPDATE GoldenArcherCoin SET LuckyNumber = NULL WHERE AccountID = '%s'", lpMsg->account);
	}

	gQueryManager.Close();

#else

	if (strlen(lpMsg->LuckyNumber) > 0)
	{
		gQueryManager.ExecUpdateQuery("UPDATE GoldenArcherCoin SET LuckyNumber = '%s' WHERE AccountID = '%s'", lpMsg->LuckyNumber, lpMsg->account);
	}
	else
	{
		gQueryManager.ExecUpdateQuery("UPDATE GoldenArcherCoin SET LuckyNumber = NULL WHERE AccountID = '%s'", lpMsg->account);
	}

	gQueryManager.Close();

#endif
}

void CGoldenArcher::GDGoldenArcherRegisterLuckyNumberRecv(SDHP_GOLDEN_ARCHER_SAVE_LUCKY_NUMBER_RECV* lpMsg, int index)
{
	SDHP_GOLDEN_ARCHER_SAVE_LUCKY_NUMBER_SEND pMsg;

	pMsg.header.set(0x04, 0x03, sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account, lpMsg->account, sizeof(pMsg.account));

#ifndef MYSQL
	if (gQueryManager.ExecQuery("INSERT INTO GoldenArcherLuckyNumbers (AccountID, LuckyNumber) VALUES ('%s', '%s')", lpMsg->account, lpMsg->LuckyNumber) == false)
	#else
	if (gQueryManager.ExecUpdateQuery("INSERT INTO GoldenArcherLuckyNumbers (AccountID, LuckyNumber) VALUES ('%s', '%s')", lpMsg->account, lpMsg->LuckyNumber) == false)
	#endif
	{
		gQueryManager.Close();

		pMsg.Result = 1;
	}
	else
	{
		gQueryManager.Close();

		pMsg.Result = 0;
	}

	gSocketManager.DataSend(index, (BYTE*)&pMsg, sizeof(pMsg));
}

void CGoldenArcher::GDBingoGetWinnersRecv(SDHP_BINGO_GET_WINNERS_RECV* lpMsg, int index)
{
	BYTE send[4096];

	SDHP_BINGO_GET_WINNERS_SEND pMsg;

	pMsg.header.set(0x04, 0x04, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	BINGO_WINNER info;

#ifndef MYSQL
	if (gQueryManager.ExecQuery("SELECT TOP %d * FROM GoldenArcherLuckyNumbers ORDER BY NEWID()", lpMsg->count))
	#else
	if (gQueryManager.ExecResultQuery("SELECT * FROM GoldenArcherLuckyNumbers ORDER BY RAND() LIMIT %d", lpMsg->count))
	#endif
	{
	#ifndef MYSQL
		while (gQueryManager.Fetch() != SQL_NO_DATA)
		#else
		while (gQueryManager.Fetch())
		#endif
		{
			gQueryManager.GetAsString("AccountID", info.Account, sizeof(info.Account));

			gQueryManager.GetAsString("LuckyNumber", info.LuckyNumber, sizeof(info.LuckyNumber));

			memcpy(&send[size], &info, sizeof(info));

			size += sizeof(info);

			pMsg.count++;
		}
	}

	gQueryManager.Close();

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	gSocketManager.DataSend(index, send, size);
}

void CGoldenArcher::GDBingoClearRegisteredRecv(SDHP_BINGO_CLEAR_REGISTERED_RECV* lpMsg, int index)
{
	SDHP_BINGO_CLEAR_REGISTERED_SEND pMsg;

	pMsg.header.set(0x04, 0x05, sizeof(pMsg));

#ifndef MYSQL
	if (gQueryManager.ExecQuery("TRUNCATE TABLE GoldenArcherLuckyNumbers") == false)
	#else
	if (gQueryManager.ExecUpdateQuery("TRUNCATE TABLE GoldenArcherLuckyNumbers") == false)
	#endif
	{
		gQueryManager.Close();

		pMsg.result = 1;
	}
	else
	{
		gQueryManager.Close();

		pMsg.result = 0;
	}

	gSocketManager.DataSend(index, (BYTE*)&pMsg, sizeof(pMsg));
}