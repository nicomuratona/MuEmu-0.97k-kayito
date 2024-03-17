#pragma once

enum eQueueTimerIndex
{
	QUEUE_TIMER_MONSTER = 0,
	QUEUE_TIMER_MONSTER_MOVE = 1,
	QUEUE_TIMER_EVENT = 2,
	QUEUE_TIMER_VIEWPORT = 3,
	QUEUE_TIMER_FIRST = 4,
	QUEUE_TIMER_CLOSE = 5,
	QUEUE_TIMER_ACCOUNT_LEVEL = 6,
};

struct QUEUE_TIMER_INFO
{
	int TimerIndex;
	HANDLE QueueTimerTimer;
};

class CQueueTimer
{
public:

	CQueueTimer();

	~CQueueTimer();

	void CreateTimer(int TimerIndex, int TimerDelay, WAITORTIMERCALLBACK CallbackFunction);

	void DeleteTimer(int TimerIndex);

private:

	HANDLE m_QueueTimer;

	std::map<int, QUEUE_TIMER_INFO> m_QueueTimerInfo;
};

extern CQueueTimer gQueueTimer;