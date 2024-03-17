#include "stdafx.h"
#include "Viewport.h"
#include "EffectManager.h"
#include "Guild.h"
#include "Map.h"
#include "MapManager.h"
#include "Util.h"

CViewport gViewport;

CViewport::CViewport()
{

}

CViewport::~CViewport()
{

}

bool CViewport::CheckViewportObjectPosition(int aIndex, int map, int x, int y, int view)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Map != map)
	{
		return 0;
	}

	if (lpObj->X < (x - view) || lpObj->X >(x + view) || lpObj->Y < (y - view) || lpObj->Y >(y + view))
	{
		return 0;
	}

	return 1;
}

bool CViewport::CheckViewportObject1(int aIndex, int bIndex, int type)
{
	LPOBJ lpObj = &gObj[aIndex];

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer[n].state != VIEWPORT_NONE && lpObj->VpPlayer[n].index == bIndex && lpObj->VpPlayer[n].type == type)
		{
			return 0;
		}
	}

	return 1;
}

bool CViewport::CheckViewportObject2(int aIndex, int bIndex, int type)
{
	LPOBJ lpObj = &gObj[aIndex];

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state != VIEWPORT_NONE && lpObj->VpPlayer2[n].index == bIndex && lpObj->VpPlayer2[n].type == type)
		{
			return 0;
		}
	}

	return 1;
}

bool CViewport::CheckViewportObjectItem(int aIndex, int bIndex, int type)
{
	LPOBJ lpObj = &gObj[aIndex];

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayerItem[n].state != VIEWPORT_NONE && lpObj->VpPlayerItem[n].index == bIndex && lpObj->VpPlayerItem[n].type == type)
		{
			return 0;
		}
	}

	return 1;
}

bool CViewport::AddViewportObject1(int aIndex, int bIndex, int type)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (this->CheckViewportObject1(aIndex, bIndex, type) == 0 || (type == OBJECT_MONSTER && gObj[bIndex].Class == 523))
	{
		return 0;
	}

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer[n].state == VIEWPORT_NONE)
		{
			lpObj->VpPlayer[n].state = VIEWPORT_SEND;

			lpObj->VpPlayer[n].index = bIndex;

			lpObj->VpPlayer[n].type = type;

			lpObj->VPCount++;

			return 1;
		}
	}

	return 0;
}

bool CViewport::AddViewportObject2(int aIndex, int bIndex, int type)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (this->CheckViewportObject2(aIndex, bIndex, type) == 0)
	{
		return 0;
	}

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
		{
			lpObj->VpPlayer2[n].state = VIEWPORT_SEND;

			lpObj->VpPlayer2[n].index = bIndex;

			lpObj->VpPlayer2[n].type = type;

			lpObj->VPCount2++;

			return 1;
		}
	}

	return 0;
}

bool CViewport::AddViewportObjectItem(int aIndex, int bIndex, int type)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (this->CheckViewportObjectItem(aIndex, bIndex, type) == 0)
	{
		return 0;
	}

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayerItem[n].state == VIEWPORT_NONE)
		{
			lpObj->VpPlayerItem[n].state = VIEWPORT_SEND;

			lpObj->VpPlayerItem[n].index = bIndex;

			lpObj->VpPlayerItem[n].type = type;

			lpObj->VPCountItem++;

			return 1;
		}
	}

	return 0;
}

void CViewport::DestroyViewportPlayer1(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer[n].state != VIEWPORT_SEND && lpObj->VpPlayer[n].state != VIEWPORT_WAIT)
		{
			continue;
		}

		if (lpObj->VpPlayer[n].type != OBJECT_USER)
		{
			continue;
		}

		if (OBJECT_RANGE(lpObj->VpPlayer[n].index) == 0)
		{
			continue;
		}

		LPOBJ lpTarget = &gObj[lpObj->VpPlayer[n].index];

		if (lpTarget->State == OBJECT_CREATE && lpTarget->Live == 0)
		{
			lpObj->VpPlayer[n].state = VIEWPORT_DESTROY;

			continue;
		}

		if (lpTarget->Connected != OBJECT_ONLINE || lpTarget->Teleport != 0)
		{
			lpObj->VpPlayer[n].state = VIEWPORT_DESTROY;

			continue;
		}

		if (lpTarget->State == OBJECT_DIECMD || lpTarget->State == OBJECT_DELCMD)
		{
			lpObj->VpPlayer[n].state = VIEWPORT_DESTROY;

			continue;
		}

		if (this->CheckViewportObjectPosition(aIndex, lpTarget->Map, lpTarget->X, lpTarget->Y, gMapManager.GetMapViewRange(lpObj->Map)) == 0)
		{
			lpObj->VpPlayer[n].state = VIEWPORT_DESTROY;

			continue;
		}
	}
}

void CViewport::DestroyViewportPlayer2(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state != VIEWPORT_SEND && lpObj->VpPlayer2[n].state != VIEWPORT_WAIT)
		{
			continue;
		}

		if (lpObj->VpPlayer2[n].type != OBJECT_USER)
		{
			continue;
		}

		if (OBJECT_RANGE(lpObj->VpPlayer2[n].index) == 0)
		{
			continue;
		}

		LPOBJ lpTarget = &gObj[lpObj->VpPlayer2[n].index];

		if (lpTarget->Connected != OBJECT_ONLINE)
		{
			lpObj->VpPlayer2[n].state = VIEWPORT_NONE;

			lpObj->VpPlayer2[n].index = -1;

			lpObj->VPCount2--;

			continue;
		}

		if (this->CheckViewportObjectPosition(aIndex, lpTarget->Map, lpTarget->X, lpTarget->Y, gMapManager.GetMapViewRange(lpObj->Map)) == 0)
		{
			lpObj->VpPlayer2[n].state = VIEWPORT_NONE;

			lpObj->VpPlayer2[n].index = -1;

			lpObj->VPCount2--;

			continue;
		}
	}
}

void CViewport::DestroyViewportMonster1(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer[n].state != VIEWPORT_SEND && lpObj->VpPlayer[n].state != VIEWPORT_WAIT)
		{
			continue;
		}

		if (lpObj->VpPlayer[n].type != OBJECT_MONSTER && lpObj->VpPlayer[n].type != OBJECT_NPC)
		{
			continue;
		}

		if (OBJECT_RANGE(lpObj->VpPlayer[n].index) == 0)
		{
			continue;
		}

		LPOBJ lpTarget = &gObj[lpObj->VpPlayer[n].index];

		if (lpTarget->State == OBJECT_CREATE && lpTarget->Live == 0)
		{
			lpObj->VpPlayer[n].state = VIEWPORT_DESTROY;

			continue;
		}

		if (lpTarget->Connected != OBJECT_ONLINE || lpTarget->Teleport != 0)
		{
			lpObj->VpPlayer[n].state = VIEWPORT_DESTROY;

			continue;
		}

		if (lpTarget->State == OBJECT_DIECMD || lpTarget->State == OBJECT_DELCMD)
		{
			lpObj->VpPlayer[n].state = VIEWPORT_DESTROY;

			continue;
		}

		if (this->CheckViewportObjectPosition(aIndex, lpTarget->Map, lpTarget->X, lpTarget->Y, gMapManager.GetMapViewRange(lpObj->Map)) == 0)
		{
			lpObj->VpPlayer[n].state = VIEWPORT_DESTROY;

			continue;
		}
	}
}

void CViewport::DestroyViewportMonster2(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer2[n].state != VIEWPORT_SEND && lpObj->VpPlayer2[n].state != VIEWPORT_WAIT)
		{
			continue;
		}

		if (lpObj->VpPlayer2[n].type != OBJECT_MONSTER && lpObj->VpPlayer2[n].type != OBJECT_NPC)
		{
			continue;
		}

		if (OBJECT_RANGE(lpObj->VpPlayer2[n].index) == 0)
		{
			continue;
		}

		LPOBJ lpTarget = &gObj[lpObj->VpPlayer2[n].index];

		if (lpTarget->Connected != OBJECT_ONLINE)
		{
			lpObj->VpPlayer2[n].state = VIEWPORT_NONE;

			lpObj->VpPlayer2[n].index = -1;

			lpObj->VPCount2--;

			continue;
		}

		if (this->CheckViewportObjectPosition(aIndex, lpTarget->Map, lpTarget->X, lpTarget->Y, gMapManager.GetMapViewRange(lpObj->Map)) == 0)
		{
			lpObj->VpPlayer2[n].state = VIEWPORT_NONE;

			lpObj->VpPlayer2[n].index = -1;

			lpObj->VPCount2--;

			continue;
		}
	}
}

void CViewport::DestroyViewportItem(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayerItem[n].state != VIEWPORT_SEND && lpObj->VpPlayerItem[n].state != VIEWPORT_WAIT)
		{
			continue;
		}

		if (lpObj->VpPlayerItem[n].type != OBJECT_ITEM)
		{
			continue;
		}

		if (MAP_ITEM_RANGE(lpObj->VpPlayerItem[n].index) == 0)
		{
			continue;
		}

		CMapItem* lpMapItem = &gMap[lpObj->Map].m_Item[lpObj->VpPlayerItem[n].index];

		if (lpMapItem->IsItem() == 0)
		{
			lpObj->VpPlayerItem[n].state = VIEWPORT_DESTROY;

			continue;
		}

		if (lpMapItem->m_State == OBJECT_DIECMD)
		{
			lpObj->VpPlayerItem[n].state = VIEWPORT_DESTROY;

			continue;
		}

		if (this->CheckViewportObjectPosition(aIndex, lpObj->Map, lpMapItem->m_X, lpMapItem->m_Y, gMapManager.GetMapViewRange(lpObj->Map)) == 0)
		{
			lpObj->VpPlayerItem[n].state = VIEWPORT_DESTROY;

			continue;
		}
	}
}

void CViewport::CreateViewportPlayer(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Type != OBJECT_USER)
	{
		return;
	}

	for (int n = OBJECT_START_MONSTER; n < MAX_OBJECT; n++)
	{
		if (gObj[n].Connected != OBJECT_ONLINE || n == aIndex)
		{
			continue;
		}

		if (gObj[n].State != OBJECT_CREATE && gObj[n].State != OBJECT_PLAYING)
		{
			continue;
		}

		if (this->CheckViewportObjectPosition(aIndex, gObj[n].Map, gObj[n].X, gObj[n].Y, gMapManager.GetMapViewRange(lpObj->Map)) != 0)
		{
			this->AddViewportObject1(aIndex, n, gObj[n].Type);

			this->AddViewportObject2(n, aIndex, gObj[aIndex].Type);
		}
	}
}

void CViewport::CreateViewportMonster(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Type != OBJECT_MONSTER && lpObj->Type != OBJECT_NPC)
	{
		return;
	}

	for (int n = MAX_OBJECT_MONSTER; n < MAX_OBJECT; n++)
	{
		if (gObj[n].Connected != OBJECT_ONLINE || n == aIndex)
		{
			continue;
		}

		if (gObj[n].State != OBJECT_CREATE && gObj[n].State != OBJECT_PLAYING)
		{
			continue;
		}

		if (this->CheckViewportObjectPosition(aIndex, gObj[n].Map, gObj[n].X, gObj[n].Y, gMapManager.GetMapViewRange(lpObj->Map)) != 0)
		{
			this->AddViewportObject1(aIndex, n, gObj[n].Type);

			this->AddViewportObject2(n, aIndex, gObj[aIndex].Type);
		}
	}
}

void CViewport::CreateViewportItem(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Type != OBJECT_USER)
	{
		return;
	}

	for (int n = 0; n < MAX_MAP_ITEM; n++)
	{
		if (gMap[lpObj->Map].m_Item[n].m_Live == 0)
		{
			continue;
		}

		if (gMap[lpObj->Map].m_Item[n].m_State != OBJECT_CREATE && gMap[lpObj->Map].m_Item[n].m_State != OBJECT_PLAYING)
		{
			continue;
		}

		if (this->CheckViewportObjectPosition(aIndex, lpObj->Map, gMap[lpObj->Map].m_Item[n].m_X, gMap[lpObj->Map].m_Item[n].m_Y, gMapManager.GetMapViewRange(lpObj->Map)) != 0)
		{
			this->AddViewportObjectItem(aIndex, n, OBJECT_ITEM);
		}
	}
}

void CViewport::GCViewportDestroySend(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	BYTE send[256];

	PMSG_VIEWPORT_DESTROY_SEND pMsg;

	pMsg.header.set(0x14, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_VIEWPORT_DESTROY info;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer[n].state != VIEWPORT_DESTROY)
		{
			continue;
		}

		if (lpObj->VpPlayer[n].type != OBJECT_USER && lpObj->VpPlayer[n].type != OBJECT_MONSTER && lpObj->VpPlayer[n].type != OBJECT_NPC)
		{
			continue;
		}

		if (OBJECT_RANGE(lpObj->VpPlayer[n].index) == 0)
		{
			continue;
		}

		info.index[0] = SET_NUMBERHB(lpObj->VpPlayer[n].index);

		info.index[1] = SET_NUMBERLB(lpObj->VpPlayer[n].index);

		memcpy(&send[size], &info, sizeof(info));

		size += sizeof(info);

		pMsg.count++;
	}

	if (pMsg.count > 0)
	{
		pMsg.header.size = size;

		memcpy(send, &pMsg, sizeof(pMsg));

		DataSend(aIndex, send, size);
	}
}

void CViewport::GCViewportDestroyItemSend(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	BYTE send[1024];

	PMSG_VIEWPORT_DESTROY_ITEM_SEND pMsg;

	pMsg.header.set(0x21, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_VIEWPORT_DESTROY info;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayerItem[n].state != VIEWPORT_DESTROY)
		{
			continue;
		}

		if (lpObj->VpPlayerItem[n].type != OBJECT_ITEM)
		{
			continue;
		}

		if (MAP_ITEM_RANGE(lpObj->VpPlayerItem[n].index) == 0)
		{
			continue;
		}

		info.index[0] = SET_NUMBERHB(lpObj->VpPlayerItem[n].index);

		info.index[1] = SET_NUMBERLB(lpObj->VpPlayerItem[n].index);

		memcpy(&send[size], &info, sizeof(info));

		size += sizeof(info);

		pMsg.count++;
	}

	if (pMsg.count > 0)
	{
		pMsg.header.size[0] = SET_NUMBERHB(size);

		pMsg.header.size[1] = SET_NUMBERLB(size);

		memcpy(send, &pMsg, sizeof(pMsg));

		DataSend(aIndex, send, size);
	}
}

void CViewport::GCViewportPlayerSend(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	BYTE send[8192];

	PMSG_VIEWPORT_SEND pMsg;

	pMsg.header.set(0x12, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_VIEWPORT_PLAYER info;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer[n].state != VIEWPORT_SEND)
		{
			continue;
		}

		if (lpObj->VpPlayer[n].type != OBJECT_USER)
		{
			continue;
		}

		if (OBJECT_RANGE(lpObj->VpPlayer[n].index) == 0)
		{
			continue;
		}

		LPOBJ lpTarget = &gObj[lpObj->VpPlayer[n].index];

		if (gObjIsChangeSkin(lpTarget->Index) != 0)
		{
			continue;
		}

		info.index[0] = SET_NUMBERHB(lpTarget->Index);

		info.index[1] = SET_NUMBERLB(lpTarget->Index);

		if (lpTarget->State == OBJECT_CREATE && lpTarget->Teleport == 0)
		{
			info.index[0] |= 0x80;
		}

		info.x = (BYTE)lpTarget->X;

		info.y = (BYTE)lpTarget->Y;

		lpTarget->CharSet[0] &= 0xF0;

		lpTarget->CharSet[0] |= lpTarget->ViewState & 0x0F;

		memcpy(info.CharSet, lpTarget->CharSet, sizeof(info.CharSet));

		memcpy(info.name, lpTarget->Name, sizeof(info.name));

		info.tx = (BYTE)lpTarget->TX;

		info.ty = (BYTE)lpTarget->TY;

		info.DirAndPkLevel = (lpTarget->Dir * 16) | (lpTarget->PKLevel & 0x0F);

		int InfoSize = sizeof(info);

		info.ViewSkillState = lpTarget->ViewSkillState;

		memcpy(&send[size], &info, sizeof(info));

		size += InfoSize;

		pMsg.count++;
	}

	if (pMsg.count > 0)
	{
		pMsg.header.size[0] = SET_NUMBERHB(size);

		pMsg.header.size[1] = SET_NUMBERLB(size);

		memcpy(send, &pMsg, sizeof(pMsg));

		DataSend(aIndex, send, size);
	}
}

void CViewport::GCViewportMonsterSend(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	BYTE send[8192];

	PMSG_VIEWPORT_SEND pMsg;

	pMsg.header.set(0x13, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_VIEWPORT_MONSTER info;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer[n].state != VIEWPORT_SEND)
		{
			continue;
		}

		if (lpObj->VpPlayer[n].type != OBJECT_MONSTER && lpObj->VpPlayer[n].type != OBJECT_NPC)
		{
			continue;
		}

		if (OBJECT_RANGE(lpObj->VpPlayer[n].index) == 0)
		{
			continue;
		}

		LPOBJ lpTarget = &gObj[lpObj->VpPlayer[n].index];

		if (OBJECT_RANGE(lpTarget->SummonIndex) != 0)
		{
			continue;
		}

		info.index[0] = SET_NUMBERHB(lpTarget->Index);

		info.index[1] = SET_NUMBERLB(lpTarget->Index);

		if (lpTarget->State == OBJECT_CREATE)
		{
			info.index[0] |= 0x80;

			if (lpTarget->Teleport != 0)
			{
				info.index[0] |= 0x40;
			}
		}

		info.type = lpTarget->Class;

		info.x = (BYTE)lpTarget->X;

		info.y = (BYTE)lpTarget->Y;

		info.tx = (BYTE)lpTarget->TX;

		info.ty = (BYTE)lpTarget->TY;

		info.DirAndPkLevel = (lpTarget->Dir * 16) | (lpTarget->PKLevel & 0x0F);

		int InfoSize = sizeof(info);

		info.ViewSkillState = lpTarget->ViewSkillState;

		memcpy(&send[size], &info, sizeof(info));

		size += InfoSize;

		pMsg.count++;
	}

	if (pMsg.count > 0)
	{
		pMsg.header.size[0] = SET_NUMBERHB(size);

		pMsg.header.size[1] = SET_NUMBERLB(size);

		memcpy(send, &pMsg, sizeof(pMsg));

		DataSend(aIndex, send, size);
	}
}

void CViewport::GCViewportSummonSend(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	BYTE send[8192];

	PMSG_VIEWPORT_SEND pMsg;

	pMsg.header.set(0x1F, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_VIEWPORT_SUMMON info;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer[n].state != VIEWPORT_SEND)
		{
			continue;
		}

		if (lpObj->VpPlayer[n].type != OBJECT_MONSTER && lpObj->VpPlayer[n].type != OBJECT_NPC)
		{
			continue;
		}

		if (OBJECT_RANGE(lpObj->VpPlayer[n].index) == 0)
		{
			continue;
		}

		LPOBJ lpTarget = &gObj[lpObj->VpPlayer[n].index];

		if (OBJECT_RANGE(lpTarget->SummonIndex) == 0)
		{
			continue;
		}

		info.index[0] = SET_NUMBERHB(lpTarget->Index);

		info.index[1] = SET_NUMBERLB(lpTarget->Index);

		if (lpTarget->State == OBJECT_CREATE)
		{
			info.index[0] |= 0x80;
		}

		info.type = lpTarget->Class;

		info.x = (BYTE)lpTarget->X;

		info.y = (BYTE)lpTarget->Y;

		info.tx = (BYTE)lpTarget->TX;

		info.ty = (BYTE)lpTarget->TY;

		info.DirAndPkLevel = (lpTarget->Dir * 16) | (lpTarget->PKLevel & 0x0F);

		memcpy(info.name, gObj[lpTarget->SummonIndex].Name, sizeof(info.name));

		int InfoSize = sizeof(info);

		info.ViewSkillState = lpTarget->ViewSkillState;

		memcpy(&send[size], &info, sizeof(info));

		size += InfoSize;

		pMsg.count++;
	}

	if (pMsg.count > 0)
	{
		pMsg.header.size[0] = SET_NUMBERHB(size);

		pMsg.header.size[1] = SET_NUMBERLB(size);

		memcpy(send, &pMsg, sizeof(pMsg));

		DataSend(aIndex, send, size);
	}
}

void CViewport::GCViewportItemSend(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	BYTE send[4096];

	PMSG_VIEWPORT_SEND pMsg;

	pMsg.header.set(0x20, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_VIEWPORT_ITEM info;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayerItem[n].state != VIEWPORT_SEND)
		{
			continue;
		}

		if (lpObj->VpPlayerItem[n].type != OBJECT_ITEM)
		{
			continue;
		}

		if (MAP_ITEM_RANGE(lpObj->VpPlayerItem[n].index) == 0)
		{
			continue;
		}

		CMapItem* lpMapItem = &gMap[lpObj->Map].m_Item[lpObj->VpPlayerItem[n].index];

		info.index[0] = SET_NUMBERHB(lpObj->VpPlayerItem[n].index);

		info.index[1] = SET_NUMBERLB(lpObj->VpPlayerItem[n].index);

		if (lpMapItem->m_State == OBJECT_CREATE)
		{
			info.index[0] |= 0x80;
		}

		info.x = lpMapItem->m_X;

		info.y = lpMapItem->m_Y;

		memset(info.ItemInfo, 0, sizeof(info.ItemInfo));

		if (lpMapItem->m_Index == GET_ITEM(14, 15)) // Money
		{
			info.ItemInfo[0] = lpMapItem->m_Index & 0xFF;

			info.ItemInfo[1] = SET_NUMBERLB(SET_NUMBERHW(lpMapItem->m_BuyMoney));

			info.ItemInfo[2] = SET_NUMBERHB(SET_NUMBERLW(lpMapItem->m_BuyMoney));

			info.ItemInfo[3] = (lpMapItem->m_Index & 256) >> 1;

			info.ItemInfo[4] = SET_NUMBERLB(SET_NUMBERLW(lpMapItem->m_BuyMoney));
		}
		else
		{
			gItemManager.ItemByteConvert(info.ItemInfo, (*lpMapItem));
		}

		memcpy(&send[size], &info, sizeof(info));

		size += sizeof(info);

		pMsg.count++;
	}

	if (pMsg.count > 0)
	{
		pMsg.header.size[0] = SET_NUMBERHB(size);

		pMsg.header.size[1] = SET_NUMBERLB(size);

		memcpy(send, &pMsg, sizeof(pMsg));

		DataSend(aIndex, send, size);
	}
}

void CViewport::GCViewportChangeSend(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	BYTE send[8192];

	PMSG_VIEWPORT_SEND pMsg;

	pMsg.header.set(0x45, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_VIEWPORT_CHANGE info;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer[n].state != VIEWPORT_SEND)
		{
			continue;
		}

		if (lpObj->VpPlayer[n].type != OBJECT_USER)
		{
			continue;
		}

		if (OBJECT_RANGE(lpObj->VpPlayer[n].index) == 0)
		{
			continue;
		}

		LPOBJ lpTarget = &gObj[lpObj->VpPlayer[n].index];

		if (gObjIsChangeSkin(lpTarget->Index) == 0)
		{
			continue;
		}

		info.index[0] = SET_NUMBERHB(lpTarget->Index);

		info.index[1] = SET_NUMBERLB(lpTarget->Index);

		if (lpTarget->State == OBJECT_CREATE && lpTarget->Teleport == 0)
		{
			info.index[0] |= 0x80;
		}

		info.x = (BYTE)lpTarget->X;

		info.y = (BYTE)lpTarget->Y;

		info.skin = lpTarget->Change;

		memcpy(info.name, lpTarget->Name, sizeof(info.name));

		info.tx = (BYTE)lpTarget->TX;

		info.ty = (BYTE)lpTarget->TY;

		info.DirAndPkLevel = (lpTarget->Dir * 16) | (lpTarget->PKLevel & 0x0F);

		lpTarget->CharSet[0] &= 0xF0;

		lpTarget->CharSet[0] |= lpTarget->ViewState & 0x0F;

		memcpy(info.CharSet, lpTarget->CharSet, sizeof(info.CharSet));

		int InfoSize = sizeof(info);

		info.ViewSkillState = lpTarget->ViewSkillState;

		memcpy(&send[size], &info, sizeof(info));

		size += InfoSize;

		pMsg.count++;
	}

	if (pMsg.count > 0)
	{
		pMsg.header.size[0] = SET_NUMBERHB(size);

		pMsg.header.size[1] = SET_NUMBERLB(size);

		memcpy(send, &pMsg, sizeof(pMsg));

		DataSend(aIndex, send, size);
	}
}

void CViewport::GCViewportGuildInfoSend(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	BYTE send[4096];

	PMSG_VIEWPORT_SEND pMsg;

	pMsg.header.set(0x5A, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_VIEWPORT_GUILD_INFO info;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer[n].state != VIEWPORT_SEND)
		{
			continue;
		}

		if (lpObj->VpPlayer[n].type != OBJECT_USER)
		{
			continue;
		}

		if (OBJECT_RANGE(lpObj->VpPlayer[n].index) == false)
		{
			continue;
		}

		LPOBJ lpTarget = &gObj[lpObj->VpPlayer[n].index];

		if (lpTarget->Guild == 0)
		{
			continue;
		}

		info.index[0] = SET_NUMBERHB(lpTarget->Guild->Number);

		info.index[1] = SET_NUMBERLB(lpTarget->Guild->Number);

		memcpy(info.GuildName, lpTarget->Guild->Name, sizeof(info.GuildName));

		memcpy(info.Mark, lpTarget->Guild->Mark, sizeof(info.Mark));

		memcpy(&send[size], &info, sizeof(info));

		size += sizeof(info);

		pMsg.count++;
	}

	if (pMsg.count > 0)
	{
		pMsg.header.size[0] = SET_NUMBERHB(size);

		pMsg.header.size[1] = SET_NUMBERLB(size);

		memcpy(send, &pMsg, sizeof(pMsg));

		DataSend(aIndex, send, size);
	}
}

void CViewport::GCViewportGuildMemberSend(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	BYTE send[4096];

	PMSG_VIEWPORT_SEND pMsg;

	pMsg.header.set(0x5B, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_VIEWPORT_GUILD_MEMBER info;

	for (int n = 0; n < MAX_VIEWPORT; n++)
	{
		if (lpObj->VpPlayer[n].state != VIEWPORT_SEND)
		{
			continue;
		}

		if (lpObj->VpPlayer[n].type != OBJECT_USER)
		{
			continue;
		}

		if (OBJECT_RANGE(lpObj->VpPlayer[n].index) == false)
		{
			continue;
		}

		LPOBJ lpTarget = &gObj[lpObj->VpPlayer[n].index];

		if (lpTarget->Guild == 0)
		{
			continue;
		}

		info.userIndex[0] = SET_NUMBERHB(lpTarget->Index);

		info.userIndex[1] = SET_NUMBERLB(lpTarget->Index);

		info.guildIndex[0] = SET_NUMBERHB(lpTarget->Guild->Number);

		info.guildIndex[1] = SET_NUMBERLB(lpTarget->Guild->Number);

		memcpy(&send[size], &info, sizeof(info));

		size += sizeof(info);

		pMsg.count++;
	}

	if (pMsg.count > 0)
	{
		pMsg.header.size[0] = SET_NUMBERHB(size);

		pMsg.header.size[1] = SET_NUMBERLB(size);

		memcpy(send, &pMsg, sizeof(pMsg));

		DataSend(aIndex, send, size);
	}
}

void CViewport::GCViewportSimpleDestroySend(LPOBJ lpObj)
{
	BYTE send[256];

	PMSG_VIEWPORT_DESTROY_SEND pMsg;

	pMsg.header.set(0x14, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_VIEWPORT_DESTROY info;

	info.index[0] = SET_NUMBERHB(lpObj->Index);

	info.index[1] = SET_NUMBERLB(lpObj->Index);

	memcpy(&send[size], &info, sizeof(info));

	size += sizeof(info);

	pMsg.count++;

	pMsg.header.size = size;

	memcpy(send, &pMsg, sizeof(pMsg));

	MsgSendV2(lpObj, send, size);
}

void CViewport::GCViewportSimplePlayerSend(LPOBJ lpObj)
{
	if (gObjIsChangeSkin(lpObj->Index) != 0)
	{
		return;
	}

	BYTE send[256];

	PMSG_VIEWPORT_SEND pMsg;

	pMsg.header.set(0x12, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_VIEWPORT_PLAYER info;

	info.index[0] = SET_NUMBERHB(lpObj->Index);

	info.index[1] = SET_NUMBERLB(lpObj->Index);

	if (lpObj->State == OBJECT_CREATE && lpObj->Teleport == 0)
	{
		info.index[0] |= 0x80;
	}

	info.x = (BYTE)lpObj->X;

	info.y = (BYTE)lpObj->Y;

	lpObj->CharSet[0] &= 0xF0;

	lpObj->CharSet[0] |= lpObj->ViewState & 0x0F;

	memcpy(info.CharSet, lpObj->CharSet, sizeof(info.CharSet));

	memcpy(info.name, lpObj->Name, sizeof(info.name));

	info.tx = (BYTE)lpObj->TX;

	info.ty = (BYTE)lpObj->TY;

	info.DirAndPkLevel = (lpObj->Dir * 16) | (lpObj->PKLevel & 0x0F);

	int InfoSize = sizeof(info);

	info.ViewSkillState = lpObj->ViewSkillState;

	memcpy(&send[size], &info, sizeof(info));

	size += InfoSize;

	pMsg.count++;

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(lpObj->Index, send, size);

	MsgSendV2(lpObj, send, size);
}

void CViewport::GCViewportSimpleMonsterSend(LPOBJ lpObj)
{
	if (OBJECT_RANGE(lpObj->SummonIndex) != 0)
	{
		return;
	}

	BYTE send[256];

	PMSG_VIEWPORT_SEND pMsg;

	pMsg.header.set(0x13, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_VIEWPORT_MONSTER info;

	info.index[0] = SET_NUMBERHB(lpObj->Index);

	info.index[1] = SET_NUMBERLB(lpObj->Index);

	if (lpObj->State == OBJECT_CREATE)
	{
		info.index[0] |= 0x80;

		if (lpObj->Teleport != 0)
		{
			info.index[0] |= 0x40;
		}
	}

	info.type = lpObj->Class;

	info.x = (BYTE)lpObj->X;

	info.y = (BYTE)lpObj->Y;

	info.tx = (BYTE)lpObj->TX;

	info.ty = (BYTE)lpObj->TY;

	info.DirAndPkLevel = (lpObj->Dir * 16) | (lpObj->PKLevel & 0x0F);

	int InfoSize = sizeof(info);

	info.ViewSkillState = lpObj->ViewSkillState;

	memcpy(&send[size], &info, sizeof(info));

	size += InfoSize;

	pMsg.count++;

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	MsgSendV2(lpObj, send, size);
}

void CViewport::GCViewportSimpleSummonSend(LPOBJ lpObj)
{
	if (OBJECT_RANGE(lpObj->SummonIndex) == 0)
	{
		return;
	}

	BYTE send[256];

	PMSG_VIEWPORT_SEND pMsg;

	pMsg.header.set(0x1F, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_VIEWPORT_SUMMON info;

	info.index[0] = SET_NUMBERHB(lpObj->Index);

	info.index[1] = SET_NUMBERLB(lpObj->Index);

	if (lpObj->State == OBJECT_CREATE)
	{
		info.index[0] |= 0x80;
	}

	info.type = lpObj->Class;

	info.x = (BYTE)lpObj->X;

	info.y = (BYTE)lpObj->Y;

	info.tx = (BYTE)lpObj->TX;

	info.ty = (BYTE)lpObj->TY;

	info.DirAndPkLevel = (lpObj->Dir * 16) | (lpObj->PKLevel & 0x0F);

	memcpy(info.name, gObj[lpObj->SummonIndex].Name, sizeof(info.name));

	int InfoSize = sizeof(info);

	info.ViewSkillState = lpObj->ViewSkillState;

	memcpy(&send[size], &info, sizeof(info));

	size += InfoSize;

	pMsg.count++;

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	MsgSendV2(lpObj, send, size);
}

void CViewport::GCViewportSimpleChangeSend(LPOBJ lpObj)
{
	if (gObjIsChangeSkin(lpObj->Index) == 0)
	{
		return;
	}

	BYTE send[256];

	PMSG_VIEWPORT_SEND pMsg;

	pMsg.header.set(0x45, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_VIEWPORT_CHANGE info;

	info.index[0] = SET_NUMBERHB(lpObj->Index) | 0x80;

	info.index[1] = SET_NUMBERLB(lpObj->Index);

	info.x = (BYTE)lpObj->X;

	info.y = (BYTE)lpObj->Y;

	info.skin = lpObj->Change;

	memcpy(info.name, lpObj->Name, sizeof(info.name));

	info.tx = (BYTE)lpObj->TX;

	info.ty = (BYTE)lpObj->TY;

	info.DirAndPkLevel = (lpObj->Dir * 16) | (lpObj->PKLevel & 0x0F);

	lpObj->CharSet[0] &= 0xF0;

	lpObj->CharSet[0] |= lpObj->ViewState & 0x0F;

	memcpy(info.CharSet, lpObj->CharSet, sizeof(info.CharSet));

	int InfoSize = sizeof(info);

	info.ViewSkillState = lpObj->ViewSkillState;

	memcpy(&send[size], &info, sizeof(info));

	size += InfoSize;

	pMsg.count++;

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(lpObj->Index, send, size);

	MsgSendV2(lpObj, send, size);
}

void CViewport::GCViewportSimpleGuildInfoSend(LPOBJ lpObj)
{
	if (lpObj->Guild == 0)
	{
		return;
	}

	BYTE send[256];

	PMSG_VIEWPORT_SEND pMsg;

	pMsg.header.set(0x5A, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_VIEWPORT_GUILD_INFO info;

	info.index[0] = SET_NUMBERHB(lpObj->Guild->Number);

	info.index[1] = SET_NUMBERLB(lpObj->Guild->Number);

	memcpy(info.GuildName, lpObj->Guild->Name, sizeof(info.GuildName));

	memcpy(info.Mark, lpObj->Guild->Mark, sizeof(info.Mark));

	memcpy(&send[size], &info, sizeof(info));

	size += sizeof(info);

	pMsg.count++;

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(lpObj->Index, send, size);

	MsgSendV2(lpObj, send, size);
}

void CViewport::GCViewportSimpleGuildMemberSend(LPOBJ lpObj)
{
	if (lpObj->Guild == 0)
	{
		return;
	}

	BYTE send[256];

	PMSG_VIEWPORT_SEND pMsg;

	pMsg.header.set(0x5B, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_VIEWPORT_GUILD_MEMBER info;

	info.userIndex[0] = SET_NUMBERHB(lpObj->Index);

	info.userIndex[1] = SET_NUMBERLB(lpObj->Index);

	info.guildIndex[0] = SET_NUMBERHB(lpObj->Guild->Number);

	info.guildIndex[1] = SET_NUMBERLB(lpObj->Guild->Number);

	memcpy(&send[size], &info, sizeof(info));

	size += sizeof(info);

	pMsg.count++;

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));

	DataSend(lpObj->Index, send, size);

	MsgSendV2(lpObj, send, size);
}