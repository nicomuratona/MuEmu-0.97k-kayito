#include "stdafx.h"
#include "Map.h"
#include "BloodCastle.h"
#include "ReadScript.h"
#include "Message.h"
#include "Notice.h"
#include "ServerInfo.h"
#include "Util.h"

CMap gMap[MAX_MAP];

CMap::CMap()
{
	this->m_Weather = 0;

	this->m_WeatherTimer = GetTickCount();

	this->m_NextWeatherTimer = (GetLargeRand() % 10000) + 10000;

	this->m_MapAttr = 0;

	this->m_width = 0;

	this->m_height = 0;

	this->m_path = new CMapPath;

	this->m_ItemCount = 0;

	SetRect(&this->m_MapRespawn[0], 130, 116, 151, 137);
	SetRect(&this->m_MapRespawn[1], 106, 236, 112, 243);
	SetRect(&this->m_MapRespawn[2], 197, 35, 218, 50);
	SetRect(&this->m_MapRespawn[3], 174, 101, 187, 125);
	SetRect(&this->m_MapRespawn[4], 201, 70, 213, 81);
	SetRect(&this->m_MapRespawn[5], 89, 135, 90, 136);
	SetRect(&this->m_MapRespawn[6], 89, 135, 90, 136);
	SetRect(&this->m_MapRespawn[7], 14, 11, 27, 23);
	SetRect(&this->m_MapRespawn[8], 187, 54, 203, 69);
}

CMap::~CMap()
{

}

void CMap::Load(char* path, int map)
{
	HANDLE file = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, 0);

	if (file == INVALID_HANDLE_VALUE)
	{
		ErrorMessageBox(READ_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	DWORD BytesRead = 0;

	BYTE head;

	if (ReadFile(file, &head, sizeof(head), &BytesRead, 0) == 0)
	{
		ErrorMessageBox(READ_SCRIPT_FILE_ERROR, path);

		CloseHandle(file);

		return;
	}

	BYTE width;

	if (ReadFile(file, &width, sizeof(width), &BytesRead, 0) == 0)
	{
		ErrorMessageBox(READ_SCRIPT_FILE_ERROR, path);

		CloseHandle(file);

		return;
	}

	BYTE height;

	if (ReadFile(file, &height, sizeof(height), &BytesRead, 0) == 0)
	{
		ErrorMessageBox(READ_SCRIPT_FILE_ERROR, path);

		CloseHandle(file);

		return;
	}

	if (width < 0 || width >= MAX_MAP_WIDTH || height < 0 || height >= MAX_MAP_HEIGHT)
	{
		ErrorMessageBox(READ_SCRIPT_UNKNOWN_ERROR, path);

		CloseHandle(file);

		return;
	}

	this->m_width = width + 1;

	this->m_height = height + 1;

	int size = this->m_width * this->m_height;

	if (this->m_MapAttr != 0)
	{
		delete[] this->m_MapAttr;

		this->m_MapAttr = 0;
	}

	this->m_MapAttr = (BYTE*)GlobalAlloc(GPTR, size);

	if (this->m_MapAttr == 0)
	{
		ErrorMessageBox(READ_SCRIPT_ALLOC_ERROR, path);

		CloseHandle(file);

		return;
	}

	if (ReadFile(file, this->m_MapAttr, size, &BytesRead, 0) == 0)
	{
		ErrorMessageBox(READ_SCRIPT_FILE_ERROR, path);

		CloseHandle(file);

		return;
	}

	this->m_path->SetMapDimensions(this->m_width, this->m_height, this->m_MapAttr);

	this->m_MapNumber = map;

	CloseHandle(file);
}

BYTE CMap::GetAttr(int x, int y)
{
	if (x < 0 || x >= this->m_width || y < 0 || y >= this->m_height)
	{
		return 4;
	}

	return this->m_MapAttr[(y * this->m_height) + x];
}

bool CMap::CheckAttr(int x, int y, BYTE attr)
{
	if ((this->GetAttr(x, y) & attr) != 0)
	{
		return true;
	}

	return false;
}

void CMap::SetAttr(int x, int y, BYTE attr)
{
	if (x < 0 || x >= this->m_width || y < 0 || y >= this->m_height)
	{
		return;
	}

	this->m_MapAttr[(y * this->m_height) + x] |= attr;
}

void CMap::DelAttr(int x, int y, BYTE attr)
{
	if (x < 0 || x >= this->m_width || y < 0 || y >= this->m_height)
	{
		return;
	}

	this->m_MapAttr[(y * this->m_height) + x] &= ~attr;
}

bool CMap::CheckStandAttr(int x, int y)
{
	if (this->CheckAttr(x, y, 2) != false)
	{
		return false;
	}

	if (this->CheckAttr(x, y, 4) != false)
	{
		return false;
	}

	if (this->CheckAttr(x, y, 8) != false)
	{
		return false;
	}

	return true;
}

void CMap::SetStandAttr(int x, int y)
{
	if (x < 0 || x >= this->m_width || y < 0 || y >= this->m_height)
	{
		return;
	}

	this->m_MapAttr[(y * this->m_height) + x] |= 2;
}

void CMap::DelStandAttr(int x, int y)
{
	if (x < 0 || x >= this->m_width || y < 0 || y >= this->m_height)
	{
		return;
	}

	this->m_MapAttr[(y * this->m_height) + x] &= ~2;
}

BYTE CMap::GetWeather()
{
	return ((this->m_Weather * 16) | this->m_WeatherVariation);
}

void CMap::SetWeather(BYTE weather, BYTE variation)
{
	this->m_Weather = weather;

	this->m_WeatherVariation = variation;

	this->WeatherAllSend(this->GetWeather());
}

void CMap::WeatherVariationProcess()
{
	if ((GetTickCount() - this->m_WeatherTimer) > this->m_NextWeatherTimer)
	{
		this->m_WeatherTimer = GetTickCount();

		this->m_NextWeatherTimer = (GetLargeRand() % 10000) + 10000;

		this->m_WeatherVariation = GetLargeRand() % 10;

		this->m_Weather = GetLargeRand() % 3;

		this->WeatherAllSend(this->GetWeather());
	}
}

void CMap::WeatherAllSend(BYTE weather)
{
	for (int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if (gObjIsConnected(n) != false && gObj[n].Map == this->m_MapNumber)
		{
			GCWeatherSend(n, weather);
		}
	}
}

void CMap::GetMapPos(int map, short* ox, short* oy)
{
	if (map != MAP_LORENCIA && map != MAP_DUNGEON && map != MAP_DEVIAS && map != MAP_NORIA && map != MAP_LOST_TOWER && map != MAP_ATLANS && map != MAP_TARKAN)
	{
		return;
	}

	for (int n = 0; n < 50; n++)
	{
		int x = this->m_MapRespawn[map].left + (GetLargeRand() % (this->m_MapRespawn[map].right - this->m_MapRespawn[map].left));

		int y = this->m_MapRespawn[map].top + (GetLargeRand() % (this->m_MapRespawn[map].bottom - this->m_MapRespawn[map].top));

		if (this->CheckAttr(x, y, 4) == false && this->CheckAttr(x, y, 8) == false)
		{
			(*ox) = x;

			(*oy) = y;

			return;
		}
	}
}

void CMap::GetMapRandomPos(short* ox, short* oy, int size)
{
	for (int n = 0; n < 50; n++)
	{
		int x = ((*ox) - size);

		int y = ((*oy) - size);

		x = ((x < 0) ? 0 : x);

		y = ((y < 0) ? 0 : y);

		x = x + (GetLargeRand() % (size * 2));

		y = y + (GetLargeRand() % (size * 2));

		if (this->CheckAttr(x, y, 4) == false && this->CheckAttr(x, y, 8) == false)
		{
			(*ox) = x;

			(*oy) = y;

			return;
		}
	}
}

bool CMap::MoneyItemDrop(int money, int x, int y)
{
	if (this->CheckAttr(x, y, 4) != false || this->CheckAttr(x, y, 8) != false)
	{
		return false;
	}

	int count = this->m_ItemCount;

	for (int n = 0; n < MAX_MAP_ITEM; n++)
	{
		if (this->m_Item[count].IsItem() == false)
		{
			this->m_Item[count].m_Index = GET_ITEM(14, 15);

			this->m_Item[count].m_SellMoney = money;

			this->m_Item[count].m_BuyMoney = money;

			this->m_Item[count].m_X = x;

			this->m_Item[count].m_Y = y;

			this->m_Item[count].m_Live = 1;

			this->m_Item[count].m_Give = 0;

			this->m_Item[count].m_State = 1;

			this->m_Item[count].m_Time = GetTickCount() + (gServerInfo.m_MoneyDropTime * 1000);

			this->m_Item[count].m_LootTime = 0;

			this->m_ItemCount = (((++this->m_ItemCount) >= MAX_MAP_ITEM) ? 0 : this->m_ItemCount);

			return true;
		}
		else
		{
			count = (((++count) >= MAX_MAP_ITEM) ? 0 : count);
		}
	}

	return false;
}

bool CMap::MonsterItemDrop(int index, int level, float dur, int x, int y, BYTE SkillOption, BYTE LuckOption, BYTE AddOption, BYTE ExceOption, int aIndex, DWORD serial)
{
	if (this->CheckAttr(x, y, 4) != false || this->CheckAttr(x, y, 8) != false)
	{
		return false;
	}

	int count = this->m_ItemCount;

	for (int n = 0; n < MAX_MAP_ITEM; n++)
	{
		if (this->m_Item[count].IsItem() == false)
		{
			this->m_Item[count].CreateItem(index, level, x, y, dur, SkillOption, LuckOption, AddOption, ExceOption, serial);

			this->m_Item[count].m_UserIndex = aIndex;

			this->m_ItemCount = (((++this->m_ItemCount) >= MAX_MAP_ITEM) ? 0 : this->m_ItemCount);

			return true;
		}
		else
		{
			count = (((++count) >= MAX_MAP_ITEM) ? 0 : count);
		}
	}

	return false;
}

bool CMap::ItemDrop(int index, int level, float dur, int x, int y, BYTE SkillOption, BYTE LuckOption, BYTE AddOption, BYTE ExceOption, DWORD serial, int aIndex)
{
	if (this->CheckAttr(x, y, 4) != false || this->CheckAttr(x, y, 8) != false)
	{
		return false;
	}

	int count = this->m_ItemCount;

	for (int n = 0; n < MAX_MAP_ITEM; n++)
	{
		if (this->m_Item[count].IsItem() == false)
		{
			this->m_Item[count].DropCreateItem(index, level, x, y, dur, SkillOption, LuckOption, AddOption, ExceOption, serial);

			this->m_Item[count].m_UserIndex = aIndex;

			this->m_ItemCount = (((++this->m_ItemCount) >= MAX_MAP_ITEM) ? 0 : this->m_ItemCount);

			if (BC_MAP_RANGE(this->m_MapNumber) != false && gBloodCastle.CheckEventItemSerial(this->m_MapNumber, &this->m_Item[count]) != false)
			{
				gBloodCastle.DropEventItem(this->m_MapNumber, n, aIndex);
			}

			return true;
		}
		else
		{
			count = (((++count) >= MAX_MAP_ITEM) ? 0 : count);
		}
	}

	return false;
}

bool CMap::CheckItemGive(int aIndex, int index)
{
	if (MAP_ITEM_RANGE(index) == 0)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Map != this->m_MapNumber)
	{
		return false;
	}

	if (this->m_Item[index].IsItem() == false)
	{
		return false;
	}

	if (this->m_Item[index].m_Give == 1 || this->m_Item[index].m_Live == 0)
	{
		return false;
	}

	int x = this->m_Item[index].m_X - lpObj->X;

	int y = this->m_Item[index].m_Y - lpObj->Y;

	if (x > 2 || x < -2 || y > 2 || y < -2)
	{
		return false;
	}

	bool loot = true;

	if (OBJECT_RANGE(this->m_Item[index].m_UserIndex) != 0 && this->m_Item[index].m_LootTime > GetTickCount() && this->m_Item[index].m_UserIndex != aIndex)
	{
		loot = false;

		if (this->m_Item[index].m_QuestItem == 0 && OBJECT_RANGE(lpObj->PartyNumber) != 0 && lpObj->PartyNumber == gObj[this->m_Item[index].m_UserIndex].PartyNumber)
		{
			loot = true;
		}
	}

	if (!loot)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, gMessage.GetTextMessage(107, lpObj->Lang));
	}

	return loot;
}

void CMap::ItemGive(int aIndex, int index)
{
	if (MAP_ITEM_RANGE(index) == 0)
	{
		return;
	}

	this->m_Item[index].m_Live = 0;

	this->m_Item[index].m_Give = 1;

	this->m_Item[index].m_State = OBJECT_DIECMD;
}

void CMap::StateSetDestroy()
{
	for (int n = 0; n < MAX_MAP_ITEM; n++)
	{
		if (this->m_Item[n].IsItem() == false)
		{
			continue;
		}

		switch (this->m_Item[n].m_State)
		{
			case OBJECT_CREATE:
			{
				this->m_Item[n].m_State = OBJECT_PLAYING;

				break;
			}

			case OBJECT_DIECMD:
			{
				this->m_Item[n].Clear();

				this->m_Item[n].m_Live = 0;

				this->m_Item[n].m_Give = 0;

				break;
			}

			default:
			{
				if ((GetTickCount() > this->m_Item[n].m_Time))
				{
					this->m_Item[n].m_State = OBJECT_DIECMD;
				}

				break;
			}
		}
	}
}

BOOL CMap::CheckWall(int sx, int sy, int tx, int ty)
{
	int sx1 = sx, sy1 = sy, sx2 = tx, sy2 = ty;

	int Index = ((BYTE)sy1 * 256 + (BYTE)sx1);

	int nx1;

	int ny1;

	int d1;

	int d2;

	int len1;

	int len2;

	int px1 = sx2 - sx1;

	int py1 = sy2 - sy1;

	if (px1 < 0)
	{
		px1 = -px1;

		nx1 = -1;
	}
	else
	{
		nx1 = 1;
	}

	if (py1 < 0)
	{
		py1 = -py1;

		ny1 = -256;
	}
	else
	{
		ny1 = 256;
	}

	if (px1 > py1)
	{
		len1 = px1;

		len2 = py1;

		d1 = ny1;

		d2 = nx1;
	}
	else
	{
		len1 = py1;

		len2 = px1;

		d1 = nx1;

		d2 = ny1;
	}

	int error = 0;

	int count = 0;

	int Shadow = 0;

	do
	{
		if ((this->m_MapAttr[Index] & 4) == 4)
		{
			return FALSE;
		}

		error += len2;

		if (error > len1 / 2)
		{
			Index += d1;

			error -= len1;
		}

		Index += d2;

		count++;
	}
	while (count <= len1);

	return TRUE;
}

BYTE CMap::CheckWall2(int sx, int sy, int tx, int ty)
{
	int sx1 = sx, sy1 = sy, sx2 = tx, sy2 = ty;

	int Index = ((BYTE)sy1 * 256 + (BYTE)sx1);

	int nx1;

	int ny1;

	int d1;

	int d2;

	int len1;

	int len2;

	int px1 = sx2 - sx1;

	int py1 = sy2 - sy1;

	if (px1 < 0)
	{
		px1 = -px1;

		nx1 = -1;
	}
	else
	{
		nx1 = 1;
	}

	if (py1 < 0)
	{
		py1 = -py1;

		ny1 = -256;
	}
	else
	{
		ny1 = 256;
	}

	if (px1 > py1)
	{
		len1 = px1;

		len2 = py1;

		d1 = ny1;

		d2 = nx1;
	}
	else
	{
		len1 = py1;

		len2 = px1;

		d1 = nx1;

		d2 = ny1;
	}

	int start = 0;

	BOOL error = FALSE;

	int count = 0;

	int Shadow = 0;

	do
	{
		if (start == 0)
		{
			if ((this->m_MapAttr[Index] & 2) == 2)
			{
				return 2;
			}
		}
		else
		{
			start = 1;
		}

		if ((this->m_MapAttr[Index] & 4) == 4)
		{
			return 4;
		}

		error += len2;

		if (error > (len1 / 2))
		{
			Index += d1;

			error -= len1;
		}

		Index += d2;

		count++;
	}
	while (count <= len1);

	return 1;
}

bool CMap::PathFinding2(int sx, int sy, int tx, int ty, PATH_INFO* path)
{
	PATH_INFO* a = path;

	bool Success = this->m_path->FindPath(sx, sy, tx, ty, true);

	if (Success == false)
	{
		Success = this->m_path->FindPath(sx, sy, tx, ty, false);
	}

	if (Success != false)
	{
		int lc3 = this->m_path->GetPath();

		if (lc3 > 1)
		{
			a->PathNum = lc3;

			BYTE* px = this->m_path->GetPathX();

			BYTE* py = this->m_path->GetPathY();

			for (int n = 0; n < a->PathNum; n++)
			{
				a->PathX[n] = px[n];

				a->PathY[n] = py[n];
			}

			a->CurrentPath = 0;

			return true;
		}

	}

	return false;
}

bool CMap::PathFinding3(int sx, int sy, int tx, int ty, PATH_INFO* path)
{
	PATH_INFO* a = path;

	bool Success = this->m_path->FindPath2(sx, sy, tx, ty, TRUE);

	if (Success == false)
	{
		Success = this->m_path->FindPath2(sx, sy, tx, ty, FALSE);
	}

	if (Success != false)
	{
		int lc3 = this->m_path->GetPath();

		if (lc3 > 1)
		{
			a->PathNum = lc3;

			BYTE* px = this->m_path->GetPathX();

			BYTE* py = this->m_path->GetPathY();

			for (int n = 0; n < a->PathNum; n++)
			{
				a->PathX[n] = px[n];

				a->PathY[n] = py[n];
			}

			a->CurrentPath = 0;

			return true;
		}

	}

	return false;
}

bool CMap::PathFinding4(int sx, int sy, int tx, int ty, PATH_INFO* path)
{
	PATH_INFO* a = path;

	bool Success = this->m_path->FindPath3(sx, sy, tx, ty, true);

	if (Success == false)
	{
		Success = this->m_path->FindPath3(sx, sy, tx, ty, false);
	}

	if (Success != false)
	{
		int lc3 = this->m_path->GetPath();

		if (lc3 > 1)
		{
			a->PathNum = lc3;

			BYTE* px = this->m_path->GetPathX();

			BYTE* py = this->m_path->GetPathY();

			for (int n = 0; n < a->PathNum; n++)
			{
				a->PathX[n] = px[n];

				a->PathY[n] = py[n];
			}

			a->CurrentPath = 0;

			return true;
		}

	}

	return false;
}