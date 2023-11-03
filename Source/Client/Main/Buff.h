#pragma once

class CBuff
{
public:

	CBuff();

	virtual ~CBuff();

	void Init();

private:

	static void InsertBuffPhysicalEffect(eEffectState buff, DWORD o);

	static void ClearBuffPhysicalEffect(eEffectState buff, DWORD o);
};

extern CBuff gBuff;