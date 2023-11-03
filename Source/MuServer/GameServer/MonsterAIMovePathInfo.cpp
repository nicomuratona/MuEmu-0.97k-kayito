#include "stdafx.h"
#include "MonsterAIMovePathInfo.h"

CMonsterAIMovePathInfo::CMonsterAIMovePathInfo()
{
	this->Reset();
}

void CMonsterAIMovePathInfo::Reset()
{
	this->m_Type = -1;

	this->m_MapNumber = -1;

	this->m_PathX = -1;

	this->m_PathY = -1;
}