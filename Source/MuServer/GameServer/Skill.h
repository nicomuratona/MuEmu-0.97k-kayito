#pragma once

class CSkill
{
public:

	CSkill();

	~CSkill();

	void Clear();

	bool IsSkill();

	bool Set(int index, int level);

public:

	BYTE m_level;

	BYTE m_skill;

	BYTE m_index;

	int m_DamageMin;

	int m_DamageMax;
};