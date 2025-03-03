#include "stdafx.h"
#include "FlyingDragons.h"
#include "MapManager.h"
#include "Protocol.h"
#include "ServerInfo.h"

CFlyingDragons gFlyingDragons;

CFlyingDragons::CFlyingDragons()
{
	this->Init();
}

CFlyingDragons::~CFlyingDragons()
{

}

void CFlyingDragons::Init()
{
	for (int n = 0; n < MAX_MAP; n++)
	{
		this->m_FlyingDragonsInfo[n].Active = false;

		this->m_FlyingDragonsInfo[n].EndTime = 0;

		this->m_FlyingDragonsInfo[n].EventIndex = -1;
	}
}

void CFlyingDragons::FlyingDragonsDelete() //Dragones verificar si alguno no se finalizó
{
	if (gServerInfo.m_FlyingDragonsSwitch == 0)
	{
		return;
	}

	for (int n = 0; n < MAX_MAP; n++)
	{
		if (this->m_FlyingDragonsInfo[n].Active == false)
		{
			continue;
		}

		if (this->m_FlyingDragonsInfo[n].EndTime > time(0))
		{
			continue;
		}

		this->m_FlyingDragonsInfo[n].Active = false;

		GCEventStateSendToAll(n, 0, this->m_FlyingDragonsInfo[n].EventIndex);

		this->m_FlyingDragonsInfo[n].EventIndex = -1;
	}
}

void CFlyingDragons::FlyingDragonsAdd(int map, int invasionTime, int index) //Dragones agregar al mapa y setearlos
{
	if (gServerInfo.m_FlyingDragonsSwitch == 0 || gMapManager.GetMapFlyingDragons(map) == 0 || (index != 1 && index != 3))
	{
		return;
	}

	if (this->m_FlyingDragonsInfo[map].Active == true && this->m_FlyingDragonsInfo[map].EventIndex == index)
	{
		if (this->m_FlyingDragonsInfo[map].EndTime < (time(0) + invasionTime))
		{
			this->m_FlyingDragonsInfo[map].EndTime = (time(0) + invasionTime);
		}
	}
	else
	{
		this->m_FlyingDragonsInfo[map].Active = true;

		this->m_FlyingDragonsInfo[map].EventIndex = index;

		this->m_FlyingDragonsInfo[map].EndTime = (time(0) + invasionTime);

		GCEventStateSendToAll(map, 1, index); //Dragones meter al iniciar una invasión
	}
}

void CFlyingDragons::FlyingDragonsBossDieProc(int map) //Quitar dragones al matar al boss
{
	if (gServerInfo.m_FlyingDragonsSwitch == 0)
	{
		return;
	}

	for (int n = 0; n < MAX_MAP; n++)
	{
		if (this->m_FlyingDragonsInfo[n].EventIndex != this->m_FlyingDragonsInfo[map].EventIndex)
		{
			continue;
		}

		this->m_FlyingDragonsInfo[n].Active = false;

		GCEventStateSendToAll(n, 0, this->m_FlyingDragonsInfo[n].EventIndex);

		this->m_FlyingDragonsInfo[n].EventIndex = -1;
	}
}

void CFlyingDragons::FlyingDragonsCheck(int map, int index) //Dragones verificar mapa para insertar
{
	if (this->m_FlyingDragonsInfo[map].Active == true)
	{
		GCEventStateSend(index, 1, this->m_FlyingDragonsInfo[map].EventIndex);
	}
}