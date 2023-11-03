#pragma once

#define MAX_MONSTER_AI_AGRO 20

class CMonsterAIAgroInfo
{
public:
	CMonsterAIAgroInfo()
	{
		this->Reset();
	}

	void Reset()
	{
		this->m_UserIndex = -1;

		this->m_AgroValue = -1;
	}

	int GetUserIndex()
	{
		return this->m_UserIndex;
	}

	int GetAgroValue()
	{
		return this->m_AgroValue;
	}

	void SetAgro(int aIndex, int value)
	{
		this->m_UserIndex = aIndex;

		this->m_AgroValue = value;
	}

	int IncAgro(int value)
	{
		this->m_AgroValue += value;

		return this->m_AgroValue;
	}

	int DecAgro(int value)
	{
		this->m_AgroValue -= value;

		return this->m_AgroValue;
	}

private:

	int m_UserIndex;

	int m_AgroValue;
};

class CMonsterAIAgro
{
public:

	CMonsterAIAgro();

	~CMonsterAIAgro();

	void ResetAll();

	bool SetAgro(int aIndex, int value);

	bool DelAgro(int aIndex);

	int GetAgro(int aIndex);

	int IncAgro(int aIndex, int value);

	int DecAgro(int aIndex, int value);

	void DecAllAgro(int value);

	int GetMaxAgroUserIndex(int aIndex);

private:

	CMonsterAIAgroInfo m_Agro[MAX_MONSTER_AI_AGRO];
};