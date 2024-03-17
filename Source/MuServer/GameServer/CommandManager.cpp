#include "stdafx.h"
#include "CommandManager.h"
#include "BonusManager.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "GameMaster.h"
#include "Guild.h"
#include "ItemManager.h"
#include "Log.h"
#include "MemScript.h"
#include "Message.h"
#include "Move.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "Protocol.h"
#include "Quest.h"
#include "QuestReward.h"
#include "ResetTable.h"
#include "ServerInfo.h"
#include "Util.h"
#include "Monster.h"

CCommandManager gCommandManager;

CCommandManager::CCommandManager()
{

}

CCommandManager::~CCommandManager()
{

}

void CCommandManager::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == NULL)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (!lpMemScript->SetBuffer(path))
	{
		ErrorMessageBox(lpMemScript->GetLastError());

		delete lpMemScript;

		return;
	}

	this->m_CommandInfo.clear();

	int index = 0;

	try
	{
		eTokenResult token;

		while (true)
		{
			token = lpMemScript->GetToken();

			if (token == TOKEN_END || token == TOKEN_END_SECTION)
			{
				break;
			}

			COMMAND_LIST info;

			info.Index = index++;

			strcpy_s(info.Command, lpMemScript->GetString());

			info.Enable[0] = lpMemScript->GetAsNumber();

			info.Enable[1] = lpMemScript->GetAsNumber();

			info.Enable[2] = lpMemScript->GetAsNumber();

			info.Enable[3] = lpMemScript->GetAsNumber();

			info.Money[0] = lpMemScript->GetAsNumber();

			info.Money[1] = lpMemScript->GetAsNumber();

			info.Money[2] = lpMemScript->GetAsNumber();

			info.Money[3] = lpMemScript->GetAsNumber();

			info.MinLevel[0] = lpMemScript->GetAsNumber();

			info.MinLevel[1] = lpMemScript->GetAsNumber();

			info.MinLevel[2] = lpMemScript->GetAsNumber();

			info.MinLevel[3] = lpMemScript->GetAsNumber();

			info.MaxLevel[0] = lpMemScript->GetAsNumber();

			info.MaxLevel[1] = lpMemScript->GetAsNumber();

			info.MaxLevel[2] = lpMemScript->GetAsNumber();

			info.MaxLevel[3] = lpMemScript->GetAsNumber();

			info.MinReset[0] = lpMemScript->GetAsNumber();

			info.MinReset[1] = lpMemScript->GetAsNumber();

			info.MinReset[2] = lpMemScript->GetAsNumber();

			info.MinReset[3] = lpMemScript->GetAsNumber();

			info.MaxReset[0] = lpMemScript->GetAsNumber();

			info.MaxReset[1] = lpMemScript->GetAsNumber();

			info.MaxReset[2] = lpMemScript->GetAsNumber();

			info.MaxReset[3] = lpMemScript->GetAsNumber();

			info.Delay = lpMemScript->GetAsNumber();

			info.GameMaster = lpMemScript->GetAsNumber();

			this->m_CommandInfo.insert(std::pair<int, COMMAND_LIST>(info.Index, info));
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CCommandManager::MainProc()
{
	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnectedGP(n) == false)
		{
			continue;
		}

		if (gObj[n].AutoAddPointCount > 0)
		{
			this->CommandAddPointAutoProc(&gObj[n]);
		}

		if (gObj[n].AutoResetEnable != 0)
		{
			this->CommandResetAutoProc(&gObj[n]);
		}
	}
}

long CCommandManager::GetNumber(char* arg, int pos)
{
	int count = 0, p = 0;

	char buffer[60] = { 0 };

	int len = strlen(arg);

	len = ((len >= sizeof(buffer)) ? (sizeof(buffer) - 1) : len);

	for (int n = 0; n < len; n++)
	{
		if (arg[n] == 0x20)
		{
			count++;
		}
		else if (count == pos)
		{
			buffer[p] = arg[n];
			p++;
		}
	}

	return atoi(buffer);
}

void CCommandManager::GetString(char* arg, char* out, int size, int pos)
{
	int count = 0, p = 0;

	char buffer[60] = { 0 };

	int len = strlen(arg);

	len = ((len >= sizeof(buffer)) ? (sizeof(buffer) - 1) : len);

	for (int n = 0; n < len; n++)
	{
		if (arg[n] == 0x20)
		{
			count++;
		}
		else if (count == pos)
		{
			buffer[p] = arg[n];

			p++;
		}
	}

	memcpy(out, buffer, (size - 1));
}

bool CCommandManager::GetInfoByName(char* label, COMMAND_LIST* lpInfo)
{
	for (std::map<int, COMMAND_LIST>::iterator it = this->m_CommandInfo.begin(); it != this->m_CommandInfo.end(); it++)
	{
		if (_stricmp(it->second.Command, label) == 0)
		{
			(*lpInfo) = it->second;

			return true;
		}
	}

	return false;
}

bool CCommandManager::ManagementCore(LPOBJ lpObj, char* message)
{
	char command[32] = { 0 };

	memset(command, 0, sizeof(command));

	this->GetString(message, command, sizeof(command), 0);

	COMMAND_LIST CommandData;

	if (this->GetInfoByName(command, &CommandData) == false)
	{
		return false;
	}

	char* argument = &message[strlen(command)];

	if (argument[0] == 0x20)
	{
		argument++;
	}

	if (CommandData.Enable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(57, lpObj->Lang));

		return false;
	}

	if (lpObj->Money < (DWORD)CommandData.Money[lpObj->AccountLevel] && CommandData.Money[lpObj->AccountLevel] != -1)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(58, lpObj->Lang), CommandData.Money[lpObj->AccountLevel]);

		return false;
	}

	if (lpObj->Level < CommandData.MinLevel[lpObj->AccountLevel] && CommandData.MinLevel[lpObj->AccountLevel] != -1)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(59, lpObj->Lang), CommandData.MinLevel[lpObj->AccountLevel]);

		return false;
	}

	if (lpObj->Level > CommandData.MaxLevel[lpObj->AccountLevel] && CommandData.MaxLevel[lpObj->AccountLevel] != -1)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(60, lpObj->Lang), CommandData.MaxLevel[lpObj->AccountLevel]);

		return false;
	}

	if (lpObj->Reset < CommandData.MinReset[lpObj->AccountLevel] && CommandData.MinReset[lpObj->AccountLevel] != -1)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(61, lpObj->Lang), CommandData.MinReset[lpObj->AccountLevel]);

		return false;
	}

	if (lpObj->Reset > CommandData.MaxReset[lpObj->AccountLevel] && CommandData.MaxReset[lpObj->AccountLevel] != -1)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(62, lpObj->Lang), CommandData.MaxReset[lpObj->AccountLevel]);

		return false;
	}

	DWORD tick = (GetTickCount() - lpObj->CommandLastTick[CommandData.Index]) / 1000;

	if (tick < (DWORD)CommandData.Delay)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(63, lpObj->Lang), (CommandData.Delay - tick));

		return false;
	}

	if (CommandData.GameMaster != 0 && gGameMaster.CheckGameMasterLevel(lpObj, CommandData.GameMaster) == false)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(57, lpObj->Lang));

		return false;
	}

	if (lpObj->Interface.type == INTERFACE_TRADE || lpObj->Interface.type == INTERFACE_SHOP || lpObj->Interface.type == INTERFACE_WAREHOUSE || lpObj->Interface.type == INTERFACE_CHAOS_BOX)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(64, lpObj->Lang));

		return false;
	}

	switch (CommandData.Index)
	{
		case COMMAND_MOVE:
		{
			this->CommandMove(lpObj, argument);

			break;
		}

		case COMMAND_POST:
		{
			this->CommandPost(lpObj, argument);

			break;
		}

		case COMMAND_ADD_STR:
		{
			this->CommandAddPoint(lpObj, argument, 0);

			break;
		}

		case COMMAND_ADD_DEX:
		{
			this->CommandAddPoint(lpObj, argument, 1);

			break;
		}

		case COMMAND_ADD_VIT:
		{
			this->CommandAddPoint(lpObj, argument, 2);

			break;
		}

		case COMMAND_ADD_ENE:
		{
			this->CommandAddPoint(lpObj, argument, 3);

			break;
		}

		case COMMAND_RESET:
		{
			this->CommandReset(lpObj, argument);

			break;
		}

		case COMMAND_GRAND_RESET:
		{
			this->CommandGrandReset(lpObj, argument);

			break;
		}

		case COMMAND_PK_CLEAR:
		{
			this->CommandPKClear(lpObj, argument);

			break;
		}

		case COMMAND_ADD_MONEY:
		{
			this->CommandAddMoney(lpObj, argument);

			break;
		}

		case COMMAND_SUB_MONEY:
		{
			this->CommandSubMoney(lpObj, argument);

			break;
		}

		case COMMAND_CHANGE:
		{
			this->CommandChange(lpObj, argument);

			break;
		}

		case COMMAND_WARE:
		{
			this->CommandWare(lpObj, argument);

			break;
		}

		case COMMAND_ONLINES:
		{
			this->CommandOnlines(lpObj, argument);

			break;
		}

		case COMMAND_GUILD_WAR:
		{
			this->CommandGuildWar(lpObj, argument);

			break;
		}

		case COMMAND_BATTLE_SOCCER:
		{
			this->CommandBattleSoccer(lpObj, argument);

			break;
		}

		case COMMAND_REQUEST:
		{
			this->CommandRequest(lpObj, argument);

			break;
		}


		case COMMAND_GM_GLOBAL:
		{
			this->CommandGMGlobal(lpObj, argument);

			break;
		}

		case COMMAND_GM_MOVE:
		{
			this->CommandGMMove(lpObj, argument);

			break;
		}

		case COMMAND_GM_MOVEALL:
		{
			this->CommandGMMoveAll(lpObj, argument);

			break;
		}

		case COMMAND_GM_CHASE:
		{
			this->CommandGMChase(lpObj, argument);

			break;
		}

		case COMMAND_GM_BRING:
		{
			this->CommandGMBring(lpObj, argument);

			break;
		}

		case COMMAND_GM_DISCONNECT:
		{
			this->CommandGMDisconnect(lpObj, argument);

			break;
		}

		case COMMAND_GM_FIREWORKS:
		{
			this->CommandGMFireworks(lpObj, argument);

			break;
		}

		case COMMAND_GM_DROP:
		{
			this->CommandGMDrop(lpObj, argument);

			break;
		}

		case COMMAND_GM_MAKE:
		{
			this->CommandGMMake(lpObj, argument);

			break;
		}

		case COMMAND_GM_MAKESET:
		{
			this->CommandGMMakeSet(lpObj, argument);

			break;
		}

		case COMMAND_GM_CLEARINV:
		{
			this->CommandGMClearInv(lpObj, argument);

			break;
		}

		case COMMAND_GM_SKIN:
		{
			this->CommandGMSkin(lpObj, argument);

			break;
		}

		case COMMAND_GM_MAKEMOB:
		{
			this->CommandGMMakeMob(lpObj, argument);

			break;
		}
	}

	lpObj->Money -= CommandData.Money[lpObj->AccountLevel];

	GCMoneySend(lpObj->Index, lpObj->Money);

	lpObj->CommandLastTick[CommandData.Index] = GetTickCount();

	return true;
}

void CCommandManager::CommandMove(LPOBJ lpObj, char* arg)
{
	MOVE_INFO MoveInfo;

	if (gMove.GetInfoByName(arg, &MoveInfo) != 0)
	{
		gMove.Move(lpObj, MoveInfo.Index);
	}
}

void CCommandManager::CommandPost(LPOBJ lpObj, char* arg)
{
	if (gServerInfo.m_CommandPostType == 0)
	{
		this->GCPostMessageGold(lpObj->Name, gServerInfo.m_ServerName, 69, arg);
	}
	else if (gServerInfo.m_CommandPostType == 1)
	{
		this->GCPostMessageBlue(lpObj->Name, gServerInfo.m_ServerName, 69, arg);
	}
	else if (gServerInfo.m_CommandPostType == 2)
	{
		this->GCPostMessageGreen(lpObj->Name, gServerInfo.m_ServerName, 69, arg);
	}
	else if (gServerInfo.m_CommandPostType == 3)
	{
		this->GDGlobalPostSend(0, lpObj->Name, arg);
	}
	else if (gServerInfo.m_CommandPostType == 4)
	{
		this->GDGlobalPostSend(1, lpObj->Name, arg);
	}
	else if (gServerInfo.m_CommandPostType == 5)
	{
		this->GDGlobalPostSend(2, lpObj->Name, arg);
	}

	gLog.Output(LOG_COMMAND, "[CommandPost][%s][%s] - (Message: %s)", lpObj->Account, lpObj->Name, arg);
}

void CCommandManager::GDGlobalPostSend(BYTE type, char* name, char* message)
{
	SDHP_GLOBAL_POST_SEND pMsg;

	pMsg.header.set(0x05, 0x03, sizeof(pMsg));

	pMsg.type = type;

	memcpy(pMsg.name, name, sizeof(pMsg.name));

	memcpy(pMsg.message, message, sizeof(pMsg.message));

	memcpy(pMsg.serverName, gServerInfo.m_ServerName, sizeof(pMsg.serverName));

	gDataServerConnection.DataSend((BYTE*)&pMsg, sizeof(pMsg));
}

void CCommandManager::DGGlobalPostRecv(SDHP_GLOBAL_POST_RECV* lpMsg)
{
	switch (lpMsg->type)
	{
		case 0:
		{
			this->GCPostMessageGold(lpMsg->name, lpMsg->serverName, 69, lpMsg->message);

			break;
		}

		case 1:
		{
			this->GCPostMessageBlue(lpMsg->name, lpMsg->serverName, 69, lpMsg->message);

			break;
		}

		case 2:
		{
			this->GCPostMessageGreen(lpMsg->name, lpMsg->serverName, 69, lpMsg->message);

			break;
		}
	}
}

void CCommandManager::GCPostMessageGold(char* name, char* serverName, int message, char* text)
{
	char buff[256] = { 0 };

	PMSG_CHAT_WHISPER_SEND pMsg;

	memcpy(pMsg.name, name, sizeof(pMsg.name));

	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnectedGP(n) != 0)
		{
			wsprintf(buff, gMessage.GetTextMessage(message, gObj[n].Lang), serverName, text);

			int size = strlen(buff);

			size = ((size > MAX_CHAT_MESSAGE_SIZE) ? MAX_CHAT_MESSAGE_SIZE : size);

			pMsg.header.set(0x02, (sizeof(pMsg) - (sizeof(pMsg.message) - (size + 1))));

			memcpy(pMsg.message, buff, size);

			pMsg.message[size] = 0;

			DataSend(n, (BYTE*)&pMsg, pMsg.header.size);
		}
	}
}

void CCommandManager::GCPostMessageBlue(char* name, char* serverName, int message, char* text)
{
	char buff[256] = { '~' };

	PMSG_CHAT_SEND pMsg;

	memcpy(pMsg.name, name, sizeof(pMsg.name));

	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnectedGP(n) != 0)
		{
			wsprintf(&buff[1], gMessage.GetTextMessage(message, gObj[n].Lang), serverName, text);

			int size = strlen(buff);

			size = ((size > MAX_CHAT_MESSAGE_SIZE) ? MAX_CHAT_MESSAGE_SIZE : size);

			pMsg.header.set(0x00, (sizeof(pMsg) - (sizeof(pMsg.message) - (size + 1))));

			memcpy(pMsg.message, buff, size);

			pMsg.message[size] = 0;

			DataSend(n, (BYTE*)&pMsg, pMsg.header.size);
		}
	}
}

void CCommandManager::GCPostMessageGreen(char* name, char* serverName, int message, char* text)
{
	char buff[256] = { '@' };

	PMSG_CHAT_SEND pMsg;

	memcpy(pMsg.name, name, sizeof(pMsg.name));

	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnectedGP(n) != 0)
		{
			wsprintf(&buff[1], gMessage.GetTextMessage(message, gObj[n].Lang), serverName, text);

			int size = strlen(buff);

			size = ((size > MAX_CHAT_MESSAGE_SIZE) ? MAX_CHAT_MESSAGE_SIZE : size);

			pMsg.header.set(0x00, (sizeof(pMsg) - (sizeof(pMsg.message) - (size + 1))));

			memcpy(pMsg.message, buff, size);

			pMsg.message[size] = 0;

			DataSend(n, (BYTE*)&pMsg, pMsg.header.size);
		}
	}
}

void CCommandManager::CommandAddPoint(LPOBJ lpObj, char* arg, int type)
{
	char mode[5] = { 0 };

	this->GetString(arg, mode, sizeof(mode), 0);

	if (strcmp(mode, "auto") == 0)
	{
		this->CommandAddPointAuto(lpObj, arg, type);

		return;
	}

	int amount = this->GetNumber(arg, 0);

	if (amount <= 0 || lpObj->LevelUpPoint < amount)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(70, lpObj->Lang));

		return;
	}

	PMSG_LEVEL_UP_POINT_SEND pMsg;

	pMsg.header.set(0xF3, 0x06, sizeof(pMsg));

	pMsg.result = 0;

	if (gObjectManager.CharacterLevelUpPointAdd(lpObj, type, amount) != 0)
	{
		switch (type)
		{
			case 2: // Vitality
			{
				pMsg.MaxLifeAndMana = GET_MAX_WORD_VALUE((lpObj->MaxLife + lpObj->AddLife));

				break;
			}

			case 3: // Energy
			{
				pMsg.MaxLifeAndMana = GET_MAX_WORD_VALUE((lpObj->MaxMana + lpObj->AddMana));

				break;
			}
		}

		pMsg.MaxBP = GET_MAX_WORD_VALUE((lpObj->MaxBP + lpObj->AddBP));

	#if(GAMESERVER_EXTRA==1)

		pMsg.ViewPoint = (DWORD)(lpObj->LevelUpPoint);

		pMsg.ViewMaxHP = (DWORD)(lpObj->MaxLife + lpObj->AddLife);

		pMsg.ViewMaxMP = (DWORD)(lpObj->MaxMana + lpObj->AddMana);

		pMsg.ViewMaxBP = (DWORD)(lpObj->MaxBP + lpObj->AddBP);

		pMsg.ViewStrength = lpObj->Strength;

		pMsg.ViewDexterity = lpObj->Dexterity;

		pMsg.ViewVitality = lpObj->Vitality;

		pMsg.ViewEnergy = lpObj->Energy;

	#endif

		GCNewCharacterInfoSend(lpObj);

		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(72, lpObj->Lang), amount, lpObj->LevelUpPoint);

		gLog.Output(LOG_COMMAND, "[CommandAddPoint][%s][%s] - (Type: %d, Amount: %d)", lpObj->Account, lpObj->Name, type, amount);
	}

	DataSend(lpObj->Index, (BYTE*)&pMsg, pMsg.header.size);
}

void CCommandManager::CommandAddPointAuto(LPOBJ lpObj, char* arg, int type)
{
	if (gServerInfo.m_CommandAddPointAutoEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(57, lpObj->Lang));

		return;
	}

	int amount = this->GetNumber(arg, 1);

	if (amount > 0)
	{
		lpObj->AutoAddPointCount = 0;

		lpObj->AutoAddPointStats[type] = amount;

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(73, lpObj->Lang), amount);

		gLog.Output(LOG_COMMAND, "[CommandAddPointAuto][%s][%s] - (Type: %d, Amount: %d)", lpObj->Account, lpObj->Name, type, amount);
	}
	else
	{
		lpObj->AutoAddPointCount = 0;

		lpObj->AutoAddPointStats[type] = 0;

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(74, lpObj->Lang));
	}
}

void CCommandManager::CommandAddPointAutoProc(LPOBJ lpObj)
{
	if (lpObj->LevelUpPoint == 0 || lpObj->AutoAddPointCount == 0)
	{
		return;
	}

	int AddStatCount = lpObj->AutoAddPointCount;

	int* stat[4] = { &lpObj->Strength, &lpObj->Dexterity, &lpObj->Vitality, &lpObj->Energy };

	for (int n = 0; n < 4; n++)
	{
		if (lpObj->AutoAddPointStats[n] > 0)
		{
			if (AddStatCount == 0)
			{
				break;
			}

			int AddStat = lpObj->LevelUpPoint / AddStatCount;

			AddStat = (((lpObj->LevelUpPoint % AddStatCount) > 0) ? (AddStat + 1) : AddStat);

			AddStat = ((AddStat > lpObj->AutoAddPointStats[n]) ? lpObj->AutoAddPointStats[n] : AddStat);

			AddStat = ((((*stat[n]) + AddStat) > gServerInfo.m_MaxStatPoint[lpObj->AccountLevel]) ? (gServerInfo.m_MaxStatPoint[lpObj->AccountLevel] - (*stat[n])) : AddStat);

			AddStatCount--;

			(*stat[n]) += AddStat;

			lpObj->LevelUpPoint -= AddStat;

			lpObj->AutoAddPointStats[n] -= (((*stat[n]) >= gServerInfo.m_MaxStatPoint[lpObj->AccountLevel]) ? lpObj->AutoAddPointStats[n] : AddStat);

			lpObj->AutoAddPointCount = 0;

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);
		}
	}

	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	GCNewCharacterInfoSend(lpObj);
}

void CCommandManager::CommandReset(LPOBJ lpObj, char* arg)
{
	char mode[5] = { 0 };

	this->GetString(arg, mode, sizeof(mode), 0);

	if (strcmp(mode, "auto") == 0)
	{
		this->CommandResetAuto(lpObj, arg);

		return;
	}

	if (lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(75, lpObj->Lang));

		return;
	}

	if (lpObj->Level < gResetTable.GetResetLevel(lpObj))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(59, lpObj->Lang), gResetTable.GetResetLevel(lpObj));

		return;
	}

	if (lpObj->Money < ((DWORD)gResetTable.GetResetMoney(lpObj)))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(58, lpObj->Lang), gResetTable.GetResetMoney(lpObj));

		return;
	}

	if (lpObj->Reset >= gServerInfo.m_CommandResetLimit[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(76, lpObj->Lang), gServerInfo.m_CommandResetLimit[lpObj->AccountLevel]);

		return;
	}

	if (gServerInfo.m_CommandResetCheckItem[lpObj->AccountLevel] != 0)
	{
		for (int n = 0; n < INVENTORY_WEAR_SIZE; n++)
		{
			if (lpObj->Inventory[n].IsItem() != false)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(77, lpObj->Lang));

				return;
			}
		}
	}

	if (((lpObj->CommandManagerTransaction == 0) ? (lpObj->CommandManagerTransaction++) : lpObj->CommandManagerTransaction) != 0)
	{
		return;
	}

	SDHP_COMMAND_RESET_SEND pMsg;

	pMsg.header.set(0x05, 0x04, sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account, lpObj->Account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void CCommandManager::CommandResetAuto(LPOBJ lpObj, char* arg)
{
	if (gServerInfo.m_CommandResetAutoEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(57, lpObj->Lang));

		return;
	}

	if (lpObj->AutoResetEnable == 0)
	{
		lpObj->AutoResetEnable = 1;

		lpObj->AutoResetStats[0] = this->GetNumber(arg, 1);

		lpObj->AutoResetStats[1] = this->GetNumber(arg, 2);

		lpObj->AutoResetStats[2] = this->GetNumber(arg, 3);

		lpObj->AutoResetStats[3] = this->GetNumber(arg, 4);

		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(82, lpObj->Lang));
	}
	else
	{
		lpObj->AutoResetEnable = 0;

		lpObj->AutoResetStats[0] = 0;

		lpObj->AutoResetStats[1] = 0;

		lpObj->AutoResetStats[2] = 0;

		lpObj->AutoResetStats[3] = 0;

		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(83, lpObj->Lang));
	}
}

void CCommandManager::CommandResetAutoProc(LPOBJ lpObj)
{
	if (lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0)
	{
		return;
	}

	if (lpObj->Level < gResetTable.GetResetLevel(lpObj))
	{
		return;
	}

	if (lpObj->Money < ((DWORD)gResetTable.GetResetMoney(lpObj)))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(58, lpObj->Lang), gResetTable.GetResetMoney(lpObj));

		return;
	}

	if (lpObj->Reset >= gServerInfo.m_CommandResetLimit[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(76, lpObj->Lang), gServerInfo.m_CommandResetLimit[lpObj->AccountLevel]);

		return;
	}

	if (gServerInfo.m_CommandResetCheckItem[lpObj->AccountLevel] != 0)
	{
		for (int n = 0; n < INVENTORY_WEAR_SIZE; n++)
		{
			if (lpObj->Inventory[n].IsItem() != false)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(77, lpObj->Lang));

				return;
			}
		}
	}

	if (((lpObj->CommandManagerTransaction == 0) ? (lpObj->CommandManagerTransaction++) : lpObj->CommandManagerTransaction) != 0)
	{
		return;
	}

	SDHP_COMMAND_RESET_SEND pMsg;

	pMsg.header.set(0x05, 0x04, sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account, lpObj->Account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void CCommandManager::DGCommandResetRecv(SDHP_COMMAND_RESET_RECV* lpMsg)
{
	if (gObjIsAccountValid(lpMsg->index, lpMsg->account) == false)
	{
		LogAdd(LOG_RED, "[DGCommandResetRecv] Invalid Account [%d](%s)", lpMsg->index, lpMsg->account);

		CloseClient(lpMsg->index);

		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	lpObj->CommandManagerTransaction = 0;

	if (lpMsg->ResetDay >= ((DWORD)gServerInfo.m_CommandResetLimitDay[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(78, lpObj->Lang), gServerInfo.m_CommandResetLimitDay[lpObj->AccountLevel]);

		return;
	}

	if (lpMsg->ResetWek >= ((DWORD)gServerInfo.m_CommandResetLimitWek[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(79, lpObj->Lang), gServerInfo.m_CommandResetLimitWek[lpObj->AccountLevel]);

		return;
	}

	if (lpMsg->ResetMon >= ((DWORD)gServerInfo.m_CommandResetLimitMon[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(80, lpObj->Lang), gServerInfo.m_CommandResetLimitMon[lpObj->AccountLevel]);

		return;
	}

	lpObj->Money -= gResetTable.GetResetMoney(lpObj);

	GCMoneySend(lpObj->Index, lpObj->Money);

	lpObj->Level = ((gServerInfo.m_CommandResetStartLevel[lpObj->AccountLevel] == -1) ? (lpObj->Level - gServerInfo.m_CommandResetStartLevel[lpObj->AccountLevel]) : gServerInfo.m_CommandResetStartLevel[lpObj->AccountLevel]);

	lpObj->Experience = gLevelExperience[lpObj->Level - 1];

	lpObj->Reset += gBonusManager.GetBonusValue(lpObj, BONUS_INDEX_RESET_AMOUNT, gServerInfo.m_CommandResetCount[lpObj->AccountLevel], -1, -1, -1, -1);

	lpMsg->ResetDay += gBonusManager.GetBonusValue(lpObj, BONUS_INDEX_RESET_AMOUNT, gServerInfo.m_CommandResetCount[lpObj->AccountLevel], -1, -1, -1, -1);

	lpMsg->ResetWek += gBonusManager.GetBonusValue(lpObj, BONUS_INDEX_RESET_AMOUNT, gServerInfo.m_CommandResetCount[lpObj->AccountLevel], -1, -1, -1, -1);

	lpMsg->ResetMon += gBonusManager.GetBonusValue(lpObj, BONUS_INDEX_RESET_AMOUNT, gServerInfo.m_CommandResetCount[lpObj->AccountLevel], -1, -1, -1, -1);

	lpObj->AutoAddPointCount = 0;

	lpObj->AutoAddPointStats[0] = 0;

	lpObj->AutoAddPointStats[1] = 0;

	lpObj->AutoAddPointStats[2] = 0;

	lpObj->AutoAddPointStats[3] = 0;

	if (gServerInfo.m_CommandResetQuest[lpObj->AccountLevel] != 0)
	{
		lpObj->DBClass = ((lpObj->DBClass / 16) * 16);

		lpObj->ChangeUp = lpObj->DBClass % 16;

		gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);

		BYTE Class = (lpObj->ChangeUp * 16);

		Class -= (Class / 32);

		Class += (lpObj->Class * 32);

		gQuest.GCQuestRewardSend(lpObj->Index, 201, Class);

		memset(lpObj->Quest, 0xFF, sizeof(lpObj->Quest));

		gQuest.GCQuestInfoSend(lpObj->Index);
	}

	if (gServerInfo.m_CommandResetSkill[lpObj->AccountLevel] != 0)
	{
		for (int n = 0; n < MAX_SKILL_LIST; n++)
		{
			lpObj->Skill[n].Clear();
		}

		gSkillManager.GCSkillListSend(lpObj);

		gObjectManager.CharacterCalcAttribute(lpObj->Index);
	}

	if (gServerInfo.m_CommandResetStats[lpObj->AccountLevel] != 0)
	{
		int point = gResetTable.GetResetPoint(lpObj);

		point = (point * gServerInfo.m_CommandResetPointRate[lpObj->Class]) / 100;

		point += (lpObj->Level - 1) * gServerInfo.m_LevelUpPoint[lpObj->Class];

		point += ((gQuest.CheckQuestListState(lpObj, 2, QUEST_FINISH) == 0) ? 0 : ((lpObj->Level > 220) ? ((lpObj->Level - 220) * gServerInfo.m_PlusStatPoint) : 0));

		point += gQuest.GetQuestRewardLevelUpPoint(lpObj);

		point += lpObj->FruitAddPoint;

		lpObj->LevelUpPoint = point;

		lpObj->Strength = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Strength;

		lpObj->Dexterity = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Dexterity;

		lpObj->Vitality = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Vitality;

		lpObj->Energy = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Energy;
	}

	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	GCNewCharacterInfoSend(lpObj);

	GDCharacterInfoSaveSend(lpObj->Index);

	GDResetInfoSaveSend(lpObj->Index, lpMsg->ResetDay, lpMsg->ResetWek, lpMsg->ResetMon);

	GCLevelUpSend(lpObj);

	gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(81, lpObj->Lang), lpObj->Reset);

	if (gServerInfo.m_CommandResetMove[lpObj->AccountLevel] != 0)
	{
		if (gServerInfo.m_CommandResetMove[lpObj->AccountLevel] == 2)
		{
			lpObj->CloseCount = 1;

			lpObj->CloseType = 1;
		}
		else
		{
			switch (lpObj->Class)
			{
				case CLASS_DW:
				{
					gObjMoveGate(lpObj->Index, 17);

					break;
				}

				case CLASS_DK:
				{
					gObjMoveGate(lpObj->Index, 17);

					break;
				}

				case CLASS_FE:
				{
					gObjMoveGate(lpObj->Index, 27);

					break;
				}

				case CLASS_MG:
				{
					gObjMoveGate(lpObj->Index, 17);

					break;
				}
			}
		}
	}

	lpObj->AutoAddPointStats[0] = lpObj->AutoResetStats[0];

	lpObj->AutoAddPointStats[1] = lpObj->AutoResetStats[1];

	lpObj->AutoAddPointStats[2] = lpObj->AutoResetStats[2];

	lpObj->AutoAddPointStats[3] = lpObj->AutoResetStats[3];

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

	this->CommandAddPointAutoProc(lpObj);

	gLog.Output(LOG_COMMAND, "[CommandReset][%s][%s] - (Reset: %d)", lpObj->Account, lpObj->Name, lpObj->Reset);
}

void CCommandManager::CommandGrandReset(LPOBJ lpObj, char* arg)
{
	if (lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(84, lpObj->Lang));

		return;
	}

	if (lpObj->Level < gServerInfo.m_CommandGrandResetLevel[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(59, lpObj->Lang), gServerInfo.m_CommandGrandResetLevel[lpObj->AccountLevel]);

		return;
	}

	if (lpObj->Reset < gServerInfo.m_CommandGrandResetReset[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(61, lpObj->Lang), gServerInfo.m_CommandGrandResetReset[lpObj->AccountLevel]);

		return;
	}

	if (lpObj->Money < ((DWORD)gServerInfo.m_CommandGrandResetMoney[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(58, lpObj->Lang), gServerInfo.m_CommandGrandResetMoney[lpObj->AccountLevel]);

		return;
	}

	if (lpObj->GrandReset >= gServerInfo.m_CommandGrandResetLimit[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(85, lpObj->Lang), gServerInfo.m_CommandGrandResetLimit[lpObj->AccountLevel]);

		return;
	}

	if (gServerInfo.m_CommandGrandResetCheckItem[lpObj->AccountLevel] != 0)
	{
		for (int n = 0; n < INVENTORY_WEAR_SIZE; n++)
		{
			if (lpObj->Inventory[n].IsItem() != false)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(86, lpObj->Lang));

				return;
			}
		}
	}

	if (((lpObj->CommandManagerTransaction == 0) ? (lpObj->CommandManagerTransaction++) : lpObj->CommandManagerTransaction) != 0)
	{
		return;
	}

	SDHP_COMMAND_RESET_SEND pMsg;

	pMsg.header.set(0x05, 0x05, sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account, lpObj->Account, sizeof(pMsg.account));

	memcpy(pMsg.name, lpObj->Name, sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void CCommandManager::DGCommandGrandResetRecv(SDHP_COMMAND_RESET_RECV* lpMsg)
{
	if (gObjIsAccountValid(lpMsg->index, lpMsg->account) == 0)
	{
		LogAdd(LOG_RED, "[DGCommandGrandResetRecv] Invalid Account [%d](%s)", lpMsg->index, lpMsg->account);

		CloseClient(lpMsg->index);

		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	lpObj->CommandManagerTransaction = 0;

	if (lpMsg->ResetDay >= ((DWORD)gServerInfo.m_CommandGrandResetLimitDay[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(87, lpObj->Lang), gServerInfo.m_CommandGrandResetLimitDay[lpObj->AccountLevel]);

		return;
	}

	if (lpMsg->ResetWek >= ((DWORD)gServerInfo.m_CommandGrandResetLimitWek[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(88, lpObj->Lang), gServerInfo.m_CommandGrandResetLimitWek[lpObj->AccountLevel]);

		return;
	}

	if (lpMsg->ResetMon >= ((DWORD)gServerInfo.m_CommandGrandResetLimitMon[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(89, lpObj->Lang), gServerInfo.m_CommandGrandResetLimitMon[lpObj->AccountLevel]);

		return;
	}

	lpObj->Money -= gServerInfo.m_CommandGrandResetMoney[lpObj->AccountLevel];

	GCMoneySend(lpObj->Index, lpObj->Money);

	lpObj->Level = ((gServerInfo.m_CommandGrandResetStartLevel[lpObj->AccountLevel] == -1) ? (lpObj->Level - gServerInfo.m_CommandGrandResetStartLevel[lpObj->AccountLevel]) : gServerInfo.m_CommandGrandResetStartLevel[lpObj->AccountLevel]);

	lpObj->Experience = gLevelExperience[lpObj->Level - 1];

	lpObj->Reset = ((gServerInfo.m_CommandGrandResetStartReset[lpObj->AccountLevel] == -1) ? (lpObj->Reset - gServerInfo.m_CommandGrandResetReset[lpObj->AccountLevel]) : gServerInfo.m_CommandGrandResetStartReset[lpObj->AccountLevel]);

	lpObj->GrandReset += gBonusManager.GetBonusValue(lpObj, BONUS_INDEX_GRAND_RESET_AMOUNT, gServerInfo.m_CommandGrandResetCount[lpObj->AccountLevel], -1, -1, -1, -1);

	lpMsg->ResetDay += gBonusManager.GetBonusValue(lpObj, BONUS_INDEX_GRAND_RESET_AMOUNT, gServerInfo.m_CommandGrandResetCount[lpObj->AccountLevel], -1, -1, -1, -1);

	lpMsg->ResetWek += gBonusManager.GetBonusValue(lpObj, BONUS_INDEX_GRAND_RESET_AMOUNT, gServerInfo.m_CommandGrandResetCount[lpObj->AccountLevel], -1, -1, -1, -1);

	lpMsg->ResetMon += gBonusManager.GetBonusValue(lpObj, BONUS_INDEX_GRAND_RESET_AMOUNT, gServerInfo.m_CommandGrandResetCount[lpObj->AccountLevel], -1, -1, -1, -1);

	if (gServerInfo.m_CommandGrandResetQuest[lpObj->AccountLevel] != 0)
	{
		lpObj->DBClass = ((lpObj->DBClass / 16) * 16);

		lpObj->ChangeUp = lpObj->DBClass % 16;

		gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);

		BYTE Class = (lpObj->ChangeUp * 16);

		Class -= (Class / 32);

		Class += (lpObj->Class * 32);

		gQuest.GCQuestRewardSend(lpObj->Index, 201, Class);

		memset(lpObj->Quest, 0xFF, sizeof(lpObj->Quest));

		gQuest.GCQuestInfoSend(lpObj->Index);
	}

	if (gServerInfo.m_CommandGrandResetSkill[lpObj->AccountLevel] != 0)
	{
		for (int n = 0; n < MAX_SKILL_LIST; n++)
		{
			lpObj->Skill[n].Clear();
		}

		gSkillManager.GCSkillListSend(lpObj);

		gObjectManager.CharacterCalcAttribute(lpObj->Index);
	}

	if (gServerInfo.m_CommandGrandResetStats[lpObj->AccountLevel] != 0)
	{
		int point = 0;

		point = gServerInfo.m_CommandGrandResetPoint[lpObj->AccountLevel] * lpObj->GrandReset;

		point = (point * gServerInfo.m_CommandGrandResetPointRate[lpObj->Class]) / 100;

		point += (lpObj->Level - 1) * gServerInfo.m_LevelUpPoint[lpObj->Class];

		point += ((gQuest.CheckQuestListState(lpObj, 2, QUEST_FINISH) == 0) ? 0 : ((lpObj->Level > 220) ? ((lpObj->Level - 220) * gServerInfo.m_PlusStatPoint) : 0));

		point += gQuest.GetQuestRewardLevelUpPoint(lpObj);

		point += lpObj->FruitAddPoint;

		lpObj->LevelUpPoint = point;

		lpObj->Strength = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Strength;

		lpObj->Dexterity = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Dexterity;

		lpObj->Vitality = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Vitality;

		lpObj->Energy = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Energy;
	}

	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	GCNewCharacterInfoSend(lpObj);

	GDCharacterInfoSaveSend(lpObj->Index);

	GDGrandResetInfoSaveSend(lpObj->Index, lpMsg->ResetDay, lpMsg->ResetWek, lpMsg->ResetMon);

	GCLevelUpSend(lpObj);

	gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(90, lpObj->Lang), lpObj->GrandReset);

	if (gServerInfo.m_CommandGrandResetMove[lpObj->AccountLevel] != 0)
	{
		if (gServerInfo.m_CommandGrandResetMove[lpObj->AccountLevel] == 2)
		{
			lpObj->CloseCount = 1;

			lpObj->CloseType = 1;
		}
		else
		{
			switch (lpObj->Class)
			{
				case CLASS_DW:
				{
					gObjMoveGate(lpObj->Index, 17);

					break;
				}

				case CLASS_DK:
				{
					gObjMoveGate(lpObj->Index, 17);

					break;
				}

				case CLASS_FE:
				{
					gObjMoveGate(lpObj->Index, 27);

					break;
				}

				case CLASS_MG:
				{
					gObjMoveGate(lpObj->Index, 17);

					break;
				}
			}
		}
	}

	gLog.Output(LOG_COMMAND, "[CommandGrandReset][%s][%s] - (GrandReset: %d)", lpObj->Account, lpObj->Name, lpObj->GrandReset);
}

void CCommandManager::CommandPKClear(LPOBJ lpObj, char* arg)
{
	if (lpObj->PKLevel <= PKLVL_WARNING)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(91, lpObj->Lang));

		return;
	}

	lpObj->PKLevel = PKLVL_COMMONER;

	GCPKLevelSend(lpObj->Index, lpObj->PKLevel);

	gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(92, lpObj->Lang));

	gLog.Output(LOG_COMMAND, "[CommandPKClear][%s][%s] - (PKLevel: %d)", lpObj->Account, lpObj->Name, lpObj->PKLevel);
}

void CCommandManager::CommandAddMoney(LPOBJ lpObj, char* arg)
{
	int money = this->GetNumber(arg, 0);

	if (money <= 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(93, lpObj->Lang));

		return;
	}

	lpObj->Money = ((DWORD)money + lpObj->Money > MAX_MONEY) ? MAX_MONEY : (lpObj->Money + money);

	GCMoneySend(lpObj->Index, lpObj->Money);

	gLog.Output(LOG_COMMAND, "[CommandAddMoney][%s][%s] - (Money: %d)", lpObj->Account, lpObj->Name, money);
}

void CCommandManager::CommandSubMoney(LPOBJ lpObj, char* arg)
{
	int money = this->GetNumber(arg, 0);

	if (money <= 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(93, lpObj->Lang));

		return;
	}

	lpObj->Money = ((DWORD)money > lpObj->Money) ? 0 : lpObj->Money - money;

	GCMoneySend(lpObj->Index, lpObj->Money);

	gLog.Output(LOG_COMMAND, "[CommandSubMoney][%s][%s] - (Money: %d)", lpObj->Account, lpObj->Name, money);
}

void CCommandManager::CommandChange(LPOBJ lpObj, char* arg)
{
	if (lpObj->ChangeUp >= 1 || lpObj->Class == CLASS_MG)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(95, lpObj->Lang));

		return;
	}

	for (int n = 0; n <= 1; n++)
	{
		if (gQuest.CheckQuestListState(lpObj, n, QUEST_FINISH) == 0)
		{
			if (lpObj->Class != CLASS_MG)
			{
				gQuest.AddQuestList(lpObj, n, QUEST_ACCEPT);

				gQuestReward.InsertQuestReward(lpObj, n);

				gQuest.AddQuestList(lpObj, n, QUEST_FINISH);
			}
		}
	}

	lpObj->SendQuestInfo = 0;

	gQuest.GCQuestInfoSend(lpObj->Index);

	gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(96, lpObj->Lang));

	gLog.Output(LOG_COMMAND, "[CommandChange][%s][%s]", lpObj->Account, lpObj->Name);
}

void CCommandManager::CommandWare(LPOBJ lpObj, char* arg)
{
	if (lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->LoadWarehouse != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(97, lpObj->Lang));

		return;
	}

	int number = this->GetNumber(arg, 0);

	if (number < 0 || number >= gServerInfo.m_CommandWareNumber[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(98, lpObj->Lang));

		return;
	}

	lpObj->WarehouseMoney = 0;

	memset(lpObj->WarehouseMap, 0xFF, WAREHOUSE_SIZE);

	for (int n = 0; n < WAREHOUSE_SIZE; n++)
	{
		lpObj->Warehouse[n].Clear();
	}

	lpObj->Interface.use = 0;

	lpObj->Interface.type = INTERFACE_NONE;

	lpObj->Interface.state = 0;

	lpObj->LoadWarehouse = 0;

	lpObj->WarehouseNumber = number;

	gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(99, lpObj->Lang), number);

	gLog.Output(LOG_COMMAND, "[CommandWare][%s][%s] - (Number: %d)", lpObj->Account, lpObj->Name, number);
}

void CCommandManager::CommandOnlines(LPOBJ lpObj, char* arg)
{
	SDHP_COUNT_ONLINE_USER_SEND pMsg;

	pMsg.header.set(0x07, sizeof(pMsg));

	pMsg.index = lpObj->Index;

	gJoinServerConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void CCommandManager::JGCommandOnlinesRecv(SDHP_COUNT_ONLINE_USER_RECV* lpMsg)
{
	if (OBJECT_RANGE(lpMsg->index) == false)
	{
		return;
	}

	gNotice.GCNoticeSend(lpMsg->index, 0, gMessage.GetTextMessage(106, gObj[lpMsg->index].Lang), lpMsg->count);
}

void CCommandManager::CommandGuildWar(LPOBJ lpObj, char* arg)
{
	char GuildName[9] = { 0 };

	this->GetString(arg, GuildName, sizeof(GuildName), 0);

	if (strlen(GuildName) >= 1)
	{
		gGuild.GuildWarRequestSend(lpObj->Index, GuildName, 0);
	}
}

void CCommandManager::CommandBattleSoccer(LPOBJ lpObj, char* arg)
{
	char GuildName[9] = { 0 };

	this->GetString(arg, GuildName, sizeof(GuildName), 0);

	if (strlen(GuildName) >= 1)
	{
		gGuild.GuildWarRequestSend(lpObj->Index, GuildName, 1);
	}
}

void CCommandManager::CommandRequest(LPOBJ lpObj, char* arg)
{
	char mode[5] = { 0 };

	this->GetString(arg, mode, sizeof(mode), 0);

	if (strcmp(mode, "on") == 0)
	{
		lpObj->Option |= 1;

		lpObj->Option |= 2;

		lpObj->Option &= ~4;

		memset(lpObj->AutoPartyPassword, 0, sizeof(lpObj->AutoPartyPassword));

		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(103, lpObj->Lang));
	}
	else if (strcmp(mode, "off") == 0)
	{
		lpObj->Option &= ~1;

		lpObj->Option &= ~2;

		lpObj->Option &= ~4;

		memset(lpObj->AutoPartyPassword, 0, sizeof(lpObj->AutoPartyPassword));

		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(104, lpObj->Lang));
	}
	else if (strcmp(mode, "auto") == 0)
	{
		lpObj->Option |= 1;

		lpObj->Option |= 2;

		lpObj->Option |= 4;

		memset(lpObj->AutoPartyPassword, 0, sizeof(lpObj->AutoPartyPassword));

		this->GetString(arg, lpObj->AutoPartyPassword, sizeof(lpObj->AutoPartyPassword), 1);

		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(105, lpObj->Lang));
	}
}

void CCommandManager::CommandGMGlobal(LPOBJ lpObj, char* arg)
{
	char buff[128];

	wsprintf(buff, "[%s][%s] %s", gServerInfo.m_ServerName, lpObj->Name, arg);

	GDGlobalNoticeSend(0, buff);

	gLog.Output(LOG_COMMAND, "[CommandGMGlobal][%s][%s] - (Message: %s)", lpObj->Account, lpObj->Name, arg);
}

void CCommandManager::CommandGMMove(LPOBJ lpObj, char* arg)
{
	char name[11] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	int map = this->GetNumber(arg, 1);

	int x = this->GetNumber(arg, 2);

	int y = this->GetNumber(arg, 3);

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(94, lpObj->Lang));

		return;
	}

	gObjTeleport(lpTarget->Index, map, x, y);

	gLog.Output(LOG_COMMAND, "[CommandGMMove][%s][%s] - (Name: %s, Map: %d, X: %d, Y: %d)", lpObj->Account, lpObj->Name, name, map, x, y);
}

void CCommandManager::CommandGMMoveAll(LPOBJ lpObj, char* arg)
{
	int map = this->GetNumber(arg, 0);

	int x = this->GetNumber(arg, 1);

	int y = this->GetNumber(arg, 2);

	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnectedGP(n) != 0 && gObj[n].Index != lpObj->Index)
		{
			if (gObj[n].Connected > OBJECT_LOGGED)
			{
				gObjTeleport(gObj[n].Index, map, x, y);
			}
		}
	}

	gLog.Output(LOG_COMMAND, "[CommandMoveAll][%s][%s] - (Map: %d, X: %d, Y: %d)", lpObj->Account, lpObj->Name, map, x, y);
}

void CCommandManager::CommandGMChase(LPOBJ lpObj, char* arg)
{
	char name[11] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(94, lpObj->Lang));

		return;
	}

	gObjTeleport(lpObj->Index, lpTarget->Map, lpTarget->X, lpTarget->Y);

	gLog.Output(LOG_COMMAND, "[CommandChase][%s][%s] - (Name: %s)", lpObj->Account, lpObj->Name, name);
}

void CCommandManager::CommandGMBring(LPOBJ lpObj, char* arg)
{
	char name[11] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(94, lpObj->Lang));

		return;
	}

	gObjTeleport(lpTarget->Index, lpObj->Map, lpObj->X, lpObj->Y);

	gLog.Output(LOG_COMMAND, "[CommandBring][%s][%s] - (Name: %s)", lpObj->Account, lpObj->Name, name);
}

void CCommandManager::CommandGMDisconnect(LPOBJ lpObj, char* arg)
{
	char name[11] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	if (name[0] == NULL)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(94, lpObj->Lang));

		return;
	}

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(94, lpObj->Lang));

		return;
	}

	gNotice.GCNoticeSend(lpTarget->Index, 0, gMessage.GetTextMessage(302, lpObj->Lang));

	lpTarget->CloseType = 2;

	lpTarget->CloseCount = 6;

	gLog.Output(LOG_COMMAND, "[CommandDisconnect][%s][%s] - (Name: %s)", lpObj->Account, lpObj->Name, name);
}

void CCommandManager::CommandGMFireworks(LPOBJ lpObj, char* arg)
{
	for (int n = 0; n < 15; n++)
	{
		GCFireworksSend(lpObj, (lpObj->X + (((GetLargeRand() % 5) * 2) - 4)), (lpObj->Y + (((GetLargeRand() % 5) * 2) - 4)));
	}

	gLog.Output(LOG_COMMAND, "[CommandFireworks][%s][%s] - (Map: %d, X: %d, Y: %d)", lpObj->Account, lpObj->Name, lpObj->Map, lpObj->X, lpObj->Y);
}

void CCommandManager::CommandGMDrop(LPOBJ lpObj, char* arg)
{
	int section = this->GetNumber(arg, 0);
	int type = this->GetNumber(arg, 1);
	int level = this->GetNumber(arg, 2);
	int skill = this->GetNumber(arg, 3);
	int luck = this->GetNumber(arg, 4);
	int option = this->GetNumber(arg, 5);
	int exc = this->GetNumber(arg, 6);

	GDCreateItemSend(lpObj->Index, lpObj->Map, (BYTE)lpObj->X, (BYTE)lpObj->Y, GET_ITEM(section, type), level, 0, skill, luck, option, -1, exc);

	gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(100, lpObj->Lang));

	gLog.Output(LOG_COMMAND, "[CommandDrop][%s][%s] - (Section: %d, Type: %d, Level: %d, Skill: %d, Luck: %d, Option: %d, Exc: %d)", lpObj->Account, lpObj->Name, section, type, level, skill, luck, option, exc);
}

void CCommandManager::CommandGMMake(LPOBJ lpObj, char* arg)
{
	int qtd = 0, section, type, level, skill, luck, option, exc;

	section = this->GetNumber(arg, 0);

	if (section < 0)
	{
		qtd = section;
		section = this->GetNumber(arg, 1);
		type = this->GetNumber(arg, 2);
		level = this->GetNumber(arg, 3);
		skill = this->GetNumber(arg, 4);
		luck = this->GetNumber(arg, 5);
		option = this->GetNumber(arg, 6);
		exc = this->GetNumber(arg, 7);
	}
	else
	{
		type = this->GetNumber(arg, 1);
		level = this->GetNumber(arg, 2);
		skill = this->GetNumber(arg, 3);
		luck = this->GetNumber(arg, 4);
		option = this->GetNumber(arg, 5);
		exc = this->GetNumber(arg, 6);
	}

	(qtd < 0) ? qtd -= qtd * 2 : qtd;

	if (qtd > 0)
	{
		for (int n = 0; n < qtd; n++)
		{
			GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(section, type), level, 0, skill, luck, option, -1, exc);
		}
	}
	else
	{
		GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(section, type), level, 0, skill, luck, option, -1, exc);
	}

	gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(100, lpObj->Lang));

	gLog.Output(LOG_COMMAND, "[CommandMake][%s][%s] - (Section: %d, Type: %d, Level: %d, Skill: %d, Luck: %d, Option: %d, Exc: %d, QUANT: %d)", lpObj->Account, lpObj->Name, section, type, level, skill, luck, option, exc, (qtd > 0) ? qtd : 1);
}

void CCommandManager::CommandGMMakeSet(LPOBJ lpObj, char* arg)
{
	int type = this->GetNumber(arg, 0);
	int level = this->GetNumber(arg, 1);
	int skill = this->GetNumber(arg, 2);
	int luck = this->GetNumber(arg, 3);
	int option = this->GetNumber(arg, 4);
	int exc = this->GetNumber(arg, 5);

	GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(7, type), level, 0, skill, luck, option, -1, exc);
	GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(8, type), level, 0, skill, luck, option, -1, exc);
	GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(9, type), level, 0, skill, luck, option, -1, exc);
	GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(10, type), level, 0, skill, luck, option, -1, exc);
	GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(11, type), level, 0, skill, luck, option, -1, exc);

	gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(100, lpObj->Lang));

	gLog.Output(LOG_COMMAND, "[CommandMakeSet][%s][%s] - (Type: %d, Level: %d, Skill: %d, Luck: %d, Option: %d, Exc: %d)", lpObj->Account, lpObj->Name, type, level, skill, luck, option, exc);
}

void CCommandManager::CommandGMClearInv(LPOBJ lpObj, char* arg)
{
	int MaxValue = gItemManager.GetInventoryMaxValue(lpObj);

	for (int i = INVENTORY_WEAR_SIZE; i < MaxValue; i++)
	{
		gItemManager.InventoryDelItem(lpObj->Index, i);

		gItemManager.GCItemDeleteSend(lpObj->Index, i, 1);
	}

	gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(101, lpObj->Lang));

	gLog.Output(LOG_COMMAND, "[Command Clear Iventory] Use for:[%s][%s]", lpObj->Account, lpObj->Name);
}

void CCommandManager::CommandGMSkin(LPOBJ lpObj, char* arg)
{
	char name[11] = { 0 };

	this->GetString(arg, name, sizeof(name), 0);

	if (name[0] == NULL)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(94, lpObj->Lang));

		return;
	}

	LPOBJ lpTarget = gObjFind(name);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(94, lpObj->Lang));

		return;
	}

	lpTarget->Change = this->GetNumber(arg, 1);

	gObjViewportListProtocolCreate(lpTarget);

	gObjectManager.CharacterUpdateMapEffect(lpTarget);

	gLog.Output(LOG_COMMAND, "[CommandSkin][%s][%s] - (Name: %s, Change: %d)", lpObj->Account, lpObj->Name, name, lpTarget->Change);
}

void CCommandManager::CommandGMMakeMob(LPOBJ lpObj, char* arg)
{
	int monster = this->GetNumber(arg, 0);

	int qtd = this->GetNumber(arg, 1);

	qtd = (qtd > 0) ? qtd : 1;

	for (int n = 0; n < qtd; n++)
	{
		int index = gObjAddMonster(lpObj->Map);

		if (OBJECT_RANGE(index) == 0)
		{
			return;
		}

		LPOBJ lpMonster = &gObj[index];

		int px = lpObj->X;

		int py = lpObj->Y;

		if (gObjGetRandomFreeLocation(lpObj->Map, &px, &py, 3, 3, 50) == 0)
		{
			return;
		}

		lpMonster->PosNum = -1;

		lpMonster->X = px;

		lpMonster->Y = py;

		lpMonster->TX = px;

		lpMonster->TY = py;

		lpMonster->OldX = px;

		lpMonster->OldY = py;

		lpMonster->StartX = px;

		lpMonster->StartY = py;

		lpMonster->Dir = 1;

		lpMonster->Map = lpObj->Map;

		lpMonster->MonsterDeleteTime = GetTickCount() + 1800000;

		if (gObjSetMonster(index, monster) == 0)
		{
			gObjDel(index);

			return;
		}
	}

	gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(102, lpObj->Lang));

	gLog.Output(LOG_COMMAND, "[CommandMakeMob][%s][%s] - (Monster: %d, Qtd: %d)", lpObj->Account, lpObj->Name, monster, qtd);
}