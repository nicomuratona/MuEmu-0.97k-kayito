#pragma once

class CPingSystem
{
public:

	CPingSystem();

	virtual ~CPingSystem();

	void Init();

private:

	static DWORD WINAPI MainThread(CPingSystem* self);

	void SetPing();

	void SetPing2();

public:

	DWORD m_Ping;
};

extern CPingSystem gPing;