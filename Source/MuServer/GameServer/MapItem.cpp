#include "stdafx.h"
#include "MapItem.h"
#include "BloodCastle.h"
#include "ServerInfo.h"

CMapItem::CMapItem()
{

}

CMapItem::~CMapItem()
{

}

void CMapItem::Init()
{
	this->m_State = OBJECT_EMPTY;
}

void CMapItem::CreateItem(int index, int level, int x, int y, float dur, BYTE SkillOption, BYTE LuckOption, BYTE AddOption, BYTE ExceOption, DWORD serial)
{
	this->Init();

	this->m_Level = level;

	this->m_Durability = dur;

	this->Convert(index, SkillOption, LuckOption, AddOption, ExceOption);

	this->m_X = x;

	this->m_Y = y;

	this->m_Live = 1;

	this->m_Give = 0;

	this->m_State = OBJECT_CREATE;

	this->m_Time = GetTickCount() + (gServerInfo.m_ItemDropTime * 1000);

	this->m_LootTime = GetTickCount() + (gServerInfo.m_ItemDropTime * 500);

	this->m_Serial = serial;
}

void CMapItem::DropCreateItem(int index, int level, int x, int y, float dur, BYTE SkillOption, BYTE LuckOption, BYTE AddOption, BYTE ExceOption, DWORD serial)
{
	this->Init();

	this->m_Level = level;

	this->m_Durability = dur;

	this->Convert(index, SkillOption, LuckOption, AddOption, ExceOption);

	this->m_X = x;

	this->m_Y = y;

	this->m_Live = 1;

	this->m_Give = 0;

	this->m_State = OBJECT_CREATE;

	this->m_Time = GetTickCount() + (gServerInfo.m_ItemDropTime * 1000);

	this->m_LootTime = GetTickCount() + (gServerInfo.m_ItemDropTime * 500);

	this->m_Serial = serial;
}