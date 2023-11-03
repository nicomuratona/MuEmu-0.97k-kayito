#include "stdafx.h"
#include "Move.h"
#include "EffectManager.h"
#include "Gate.h"
#include "Map.h"
#include "MemScript.h"
#include "Message.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "ServerInfo.h"
#include "SkillManager.h"
#include "Util.h"

CMove gMove;

CMove::CMove()
{
	this->m_MoveInfo.clear();
}

CMove::~CMove()
{

}

void CMove::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());

		delete lpMemScript;

		return;
	}

	this->m_MoveInfo.clear();

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if (strcmp("end", lpMemScript->GetString()) == 0)
			{
				break;
			}

			MOVE_INFO info;

			info.Index = lpMemScript->GetNumber();

			strcpy_s(info.Name, lpMemScript->GetAsString());

			info.Money = lpMemScript->GetAsNumber();

			info.MinLevel = lpMemScript->GetAsNumber();

			info.MaxLevel = lpMemScript->GetAsNumber();

			info.MinReset = lpMemScript->GetAsNumber();

			info.MaxReset = lpMemScript->GetAsNumber();

			info.AccountLevel = lpMemScript->GetAsNumber();

			info.Gate = lpMemScript->GetAsNumber();

			this->m_MoveInfo.insert(std::pair<int, MOVE_INFO>(info.Index, info));
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CMove::GetInfo(int index, MOVE_INFO* lpInfo)
{
	std::map<int, MOVE_INFO>::iterator it = this->m_MoveInfo.find(index);

	if (it == this->m_MoveInfo.end())
	{
		return 0;
	}
	else
	{
		(*lpInfo) = it->second;

		return 1;
	}
}

bool CMove::GetInfoByName(char* name, MOVE_INFO* lpInfo)
{
	for (std::map<int, MOVE_INFO>::iterator it = this->m_MoveInfo.begin(); it != this->m_MoveInfo.end(); it++)
	{
		if (_stricmp(it->second.Name, name) == 0)
		{
			(*lpInfo) = it->second;

			return 1;
		}
	}

	return 0;
}

void CMove::Move(LPOBJ lpObj, int index)
{
	MOVE_INFO MoveInfo;

	if (this->GetInfo(index, &MoveInfo) == 0)
	{
		return;
	}

	if (MoveInfo.MinLevel != -1 && lpObj->Level < gGate.GetMoveLevel(lpObj, gGate.GetGateMap(MoveInfo.Gate), MoveInfo.MinLevel))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(59, lpObj->Lang), gGate.GetMoveLevel(lpObj, gGate.GetGateMap(MoveInfo.Gate), MoveInfo.MinLevel));

		return;
	}

	if (MoveInfo.MaxLevel != -1 && lpObj->Level > MoveInfo.MaxLevel)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(60, lpObj->Lang), MoveInfo.MaxLevel);

		return;
	}

	if (MoveInfo.MinReset != -1 && lpObj->Reset < MoveInfo.MinReset)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(61, lpObj->Lang), MoveInfo.MinReset);

		return;
	}

	if (MoveInfo.MaxReset != -1 && lpObj->Reset > MoveInfo.MaxReset)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(62, lpObj->Lang), MoveInfo.MaxReset);

		return;
	}

	if (lpObj->Money < ((DWORD)MoveInfo.Money))
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(58, lpObj->Lang), MoveInfo.Money);

		return;
	}

	if (lpObj->AccountLevel < MoveInfo.AccountLevel)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(57, lpObj->Lang));

		return;
	}

	if (gServerInfo.m_PKLimitFree == 0 && lpObj->PKLevel >= PKLVL_OUTLAW)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(66, lpObj->Lang));

		return;
	}

	if (lpObj->Interface.use != 0 || lpObj->Teleport != 0 || lpObj->DieRegen != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(65, lpObj->Lang));

		return;
	}

	if (gGate.GetGateMap(MoveInfo.Gate) == MAP_ATLANS && (lpObj->Inventory[8].m_Index == GET_ITEM(13, 2) || lpObj->Inventory[8].m_Index == GET_ITEM(13, 3))) // Uniria,Dinorant
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(67, lpObj->Lang));

		return;
	}

	if (gGate.GetGateMap(MoveInfo.Gate) == MAP_ICARUS && (lpObj->Inventory[7].IsItem() == 0 && lpObj->Inventory[8].m_Index != GET_ITEM(13, 3))) // Wings
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(68, lpObj->Lang));

		return;
	}

	if (gGate.GetGateMap(MoveInfo.Gate) == MAP_ICARUS && lpObj->Inventory[8].m_Index == GET_ITEM(13, 2)) // Uniria
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(67, lpObj->Lang));

		return;
	}

	if (gObjMoveGate(lpObj->Index, MoveInfo.Gate) != 0)
	{
		lpObj->Money -= MoveInfo.Money;

		GCMoneySend(lpObj->Index, lpObj->Money);
	}
}

void CMove::CGTeleportRecv(PMSG_TELEPORT_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if (lpMsg->gate == 0)
	{
		if (gObjCheckTeleportArea(aIndex, lpMsg->x, lpMsg->y) == 0)
		{
			this->GCTeleportSend(lpObj->Index, 0, lpObj->Map, (BYTE)lpObj->X, (BYTE)lpObj->Y, lpObj->Dir);

			return;
		}

		CSkill* lpSkill = gSkillManager.GetSkill(lpObj, SKILL_TELEPORT);

		if (lpSkill != 0)
		{
			if (gSkillManager.CheckSkillMana(lpObj, lpSkill->m_index) == 0 || gSkillManager.CheckSkillBP(lpObj, lpSkill->m_index) == 0)
			{
				return;
			}

			gSkillManager.GCSkillAttackSend(lpObj, lpSkill->m_index, aIndex, 1);

			gObjTeleportMagicUse(aIndex, lpMsg->x, lpMsg->y);

			lpObj->Mana -= (gSkillManager.GetSkillMana(lpSkill->m_index) * lpObj->MPConsumptionRate) / 100;

			lpObj->BP -= (gSkillManager.GetSkillBP(lpSkill->m_index) * lpObj->BPConsumptionRate) / 100;

			GCManaSend(lpObj->Index, 0xFF, (int)lpObj->Mana, lpObj->BP);
		}
	}
	else
	{
		if (lpObj->Interface.use != 0 || lpObj->DieRegen != 0)
		{
			gObjClearViewport(lpObj);

			this->GCTeleportSend(lpObj->Index, lpMsg->gate, lpObj->Map, (BYTE)lpObj->X, (BYTE)lpObj->Y, lpObj->Dir);

			gObjectManager.CharacterUpdateMapEffect(lpObj);

			gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(65, lpObj->Lang));

			return;
		}

		if (gGate.IsInGate(lpObj, lpMsg->gate) == 0)
		{
			gObjClearViewport(lpObj);

			this->GCTeleportSend(lpObj->Index, lpMsg->gate, lpObj->Map, (BYTE)lpObj->X, (BYTE)lpObj->Y, lpObj->Dir);

			gObjectManager.CharacterUpdateMapEffect(lpObj);
		}
		else
		{
			gObjMoveGate(aIndex, lpMsg->gate);
		}
	}
}

void CMove::GCTeleportSend(int aIndex, int gate, BYTE map, BYTE x, BYTE y, BYTE dir)
{
	PMSG_TELEPORT_SEND pMsg;

	pMsg.header.setE(0x1C, sizeof(pMsg));

	pMsg.gate = ((gate > 0) ? 1 : gate);

	pMsg.map = map;

	pMsg.x = x;

	pMsg.y = y;

	pMsg.dir = dir;

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CMove::GCMoveListSend(LPOBJ lpObj)
{
	BYTE send[4096];

	PMSG_MOVE_LIST_SEND pMsg;

	pMsg.header.set(0xF3, 0xE5, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	MOVE_LIST_INFO info;

	for (std::map<int, MOVE_INFO>::iterator it = this->m_MoveInfo.begin(); it != this->m_MoveInfo.end(); it++)
	{
		info.MapNumber = gGate.GetGateMap(it->second.Gate);

		memcpy(info.MapName, it->second.Name, sizeof(info.MapName));

		info.LevelMin = it->second.MinLevel;

		info.Money = it->second.Money;

		memcpy(&send[size], &info, sizeof(info));

		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(lpObj->Index, send, size);
}

void CMove::ReloadMoveList()
{
	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnectedGP(n) != 0)
		{
			this->GCMoveListSend(&gObj[n]);
		}
	}
}