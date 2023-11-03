#pragma once

class CCriticalSection
{
public:

	CCriticalSection();

	~CCriticalSection();

	void lock();

	void unlock();

private:

	CRITICAL_SECTION m_critical;
};