#include "stdafx.h"
#include "RandomManager.h"
#include "Util.h"

CRandomManager::CRandomManager()
{
	this->Clear();
}

CRandomManager::~CRandomManager()
{

}

void CRandomManager::Clear()
{
	this->m_MaxRate = 0;

	this->m_RandomManagerInfo.clear();
}

bool CRandomManager::GetRandomElement(int* value)
{
	if (this->m_MaxRate == 0)
	{
		return 0;
	}

	if (this->m_RandomManagerInfo.empty() != 0)
	{
		return 0;
	}

	int RandomNumber = GetLargeRand() % this->m_MaxRate;

	for (std::vector<RANDOM_MANAGER_INFO>::iterator it = this->m_RandomManagerInfo.begin(); it != this->m_RandomManagerInfo.end(); it++)
	{
		if (RandomNumber >= it->MinRate && RandomNumber < it->MaxRate)
		{
			(*value) = it->value;

			return 1;
		}
	}

	return 0;
}

bool CRandomManager::GetRandomElement(WORD* value)
{
	if (this->m_MaxRate == 0)
	{
		return 0;
	}

	if (this->m_RandomManagerInfo.empty() != 0)
	{
		return 0;
	}

	int RandomNumber = GetLargeRand() % this->m_MaxRate;

	for (std::vector<RANDOM_MANAGER_INFO>::iterator it = this->m_RandomManagerInfo.begin(); it != this->m_RandomManagerInfo.end(); it++)
	{
		if (RandomNumber >= it->MinRate && RandomNumber < it->MaxRate)
		{
			(*value) = it->value;

			return 1;
		}
	}

	return 0;
}

bool CRandomManager::GetRandomElement(BYTE* value)
{
	if (this->m_MaxRate == 0)
	{
		return 0;
	}

	if (this->m_RandomManagerInfo.empty() != 0)
	{
		return 0;
	}

	int RandomNumber = GetLargeRand() % this->m_MaxRate;

	for (std::vector<RANDOM_MANAGER_INFO>::iterator it = this->m_RandomManagerInfo.begin(); it != this->m_RandomManagerInfo.end(); it++)
	{
		if (RandomNumber >= it->MinRate && RandomNumber < it->MaxRate)
		{
			(*value) = it->value;

			return 1;
		}
	}

	return 0;
}

void CRandomManager::AddElement(int value, int rate)
{
	if (rate == 0)
	{
		return;
	}

	RANDOM_MANAGER_INFO info;

	info.value = value;

	info.MinRate = this->m_MaxRate;

	info.MaxRate = (this->m_MaxRate = (this->m_MaxRate + rate));

	this->m_RandomManagerInfo.push_back(info);
}