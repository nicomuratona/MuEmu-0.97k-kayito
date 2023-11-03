#include "stdafx.h"
#include "Buff.h"

CBuff gBuff;

CBuff::CBuff()
{

}

CBuff::~CBuff()
{

}

void CBuff::Init()
{
	SetCompleteHook(0xE9, 0x0043BDE0, &this->InsertBuffPhysicalEffect);

	SetCompleteHook(0xE9, 0x0043C070, &this->ClearBuffPhysicalEffect);
}

void CBuff::InsertBuffPhysicalEffect(eEffectState buff, DWORD o)
{
	WORD Type = *(WORD*)(o + 2);

	float Position[3], Angle[3], Light[3];

	if (FindEffect(buff, STATE_POISON) != 0)
	{
		AddBuff(*(DWORD*)(o + 120), STATE_POISON);
	}

	if (FindEffect(buff, STATE_FREEZE) != 0)
	{
		AddBuff(*(DWORD*)(o + 120), STATE_FREEZE);
	}

	if (FindEffect(buff, STATE_ATTACK) != 0)
	{
		AddBuff(*(DWORD*)(o + 120), STATE_ATTACK);
	}

	if (FindEffect(buff, STATE_DEFENSE) != 0)
	{
		AddBuff(*(DWORD*)(o + 120), STATE_DEFENSE);

		DeleteJoint(266, o, 4);

		for (int i = 0; i < 5; i++)
		{
			CreateJoint(266, Position, Position, Angle, 4, o, 20.0f, -1, 0);
		}
	}

	if (FindEffect(buff, STATE_ADD_LIFE) != 0)
	{
		if (Type == 390)
		{
			DeleteEffect(1150, o, 1);

			VectorCopy((float*)(o + 16), Position);

			VectorCopy((float*)(o + 28), Angle);

			VectorCopy((float*)(o + 232), Light);

			CreateEffect(1150, Position, Angle, Light, 1, o, -1, 0, 0);
		}

		AddBuff(*(DWORD*)(o + 120), STATE_ADD_LIFE);
	}

	if (FindEffect(buff, STATE_HARDEN) != 0)
	{
		DeleteEffect(190, o, 1);

		VectorCopy((float*)(o + 16), Position);

		VectorCopy((float*)(o + 28), Angle);

		VectorCopy((float*)(o + 232), Light);

		CreateEffect(190, Position, Angle, Light, 1, o, -1, 0, 0);

		Angle[2] += 180.0f;

		CreateEffect(190, Position, Angle, Light, 2, o, -1, 0, 0);

		AddBuff(*(DWORD*)(o + 120), STATE_HARDEN);
	}

	if (FindEffect(buff, STATE_REDUCE_DEFENSE) != 0)
	{
		DeleteEffect(1274, o, 0);

		VectorCopy((float*)(o + 16), Position);

		VectorCopy((float*)(o + 28), Angle);

		Vector(1.0f, 1.0f, 1.0f, Light);

		CreateEffect(1274, Position, Angle, Light, 0, o, -1, 0, 0);

		PlayBuffer(104, o, 0);

		AddBuff(*(DWORD*)(o + 120), STATE_REDUCE_DEFENSE);
	}

	if (FindEffect(buff, STATE_REDUCE_ATTACKDAMAGE) != 0)
	{
		DeleteEffect(1274, o, 3);

		VectorCopy((float*)(o + 16), Position);

		VectorCopy((float*)(o + 28), Angle);

		Vector(1.0f, 1.0f, 1.0f, Light);

		CreateEffect(1274, Position, Angle, Light, 3, o, -1, 0, 0);

		AddBuff(*(DWORD*)(o + 120), STATE_REDUCE_ATTACKDAMAGE);
	}

	if (FindEffect(buff, STATE_REDUCE_MAGICDEFENSE) != 0)
	{
		if (Type != 325)
		{
			PlayBuffer(103, NULL, 0);

			DeleteJoint(266, o, 0);

			VectorCopy((float*)(o + 16), Position);

			VectorCopy((float*)(o + 28), Angle);

			VectorCopy((float*)(o + 232), Light);

			for (int i = 0; i < 5; i++)
			{
				CreateJoint(266, Position, Position, Angle, 0, o, 50.0f, -1, 0);
			}
		}

		AddBuff(*(DWORD*)(o + 120), STATE_REDUCE_MAGICDEFENSE);
	}

	if (FindEffect(buff, STATE_REDUCE_MAGICPOWER) != 0)
	{
		AddBuff(*(DWORD*)(o + 120), STATE_REDUCE_MAGICPOWER);
	}
}

void CBuff::ClearBuffPhysicalEffect(eEffectState buff, DWORD o)
{
	WORD Type = *(WORD*)(o + 2);

	if (FindEffect(buff, STATE_POISON) != 0)
	{
		DelBuff(*(DWORD*)(o + 120), STATE_POISON);
	}

	if (FindEffect(buff, STATE_FREEZE) != 0)
	{
		DelBuff(*(DWORD*)(o + 120), STATE_FREEZE);
	}

	if (FindEffect(buff, STATE_ATTACK) != 0)
	{
		DelBuff(*(DWORD*)(o + 120), STATE_ATTACK);
	}

	if (FindEffect(buff, STATE_DEFENSE) != 0)
	{
		DelBuff(*(DWORD*)(o + 120), STATE_DEFENSE);

		DeleteJoint(266, o, 4);
	}

	if (FindEffect(buff, STATE_ADD_LIFE) != 0)
	{
		DelBuff(*(DWORD*)(o + 120), STATE_ADD_LIFE);

		if (Type == 390)
		{
			DeleteEffect(1150, o, 1);
		}
	}

	if (FindEffect(buff, STATE_HARDEN) != 0)
	{
		DelBuff(*(DWORD*)(o + 120), STATE_HARDEN);
	}

	if (FindEffect(buff, STATE_REDUCE_DEFENSE) != 0)
	{
		DelBuff(*(DWORD*)(o + 120), STATE_REDUCE_DEFENSE);

		DeleteEffect(1274, o, 0);
	}

	if (FindEffect(buff, STATE_REDUCE_ATTACKDAMAGE) != 0)
	{
		DelBuff(*(DWORD*)(o + 120), STATE_REDUCE_ATTACKDAMAGE);

		DeleteEffect(1274, o, 3);
	}

	if (FindEffect(buff, STATE_REDUCE_MAGICDEFENSE) != 0)
	{
		DelBuff(*(DWORD*)(o + 120), STATE_REDUCE_MAGICDEFENSE);

		DeleteJoint(266, o, 0);
	}

	if (FindEffect(buff, STATE_REDUCE_MAGICPOWER) != 0)
	{
		DelBuff(*(DWORD*)(o + 120), STATE_REDUCE_MAGICPOWER);
	}
}