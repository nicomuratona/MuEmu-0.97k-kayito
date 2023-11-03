#include "stdafx.h"
#include "QueueTimer.h"

CQueueTimer gQueueTimer;

CQueueTimer::CQueueTimer()
{
	this->m_QueueTimer = CreateTimerQueue();

	this->m_QueueTimerInfo.clear();
}

CQueueTimer::~CQueueTimer()
{
	DeleteTimerQueue(this->m_QueueTimer);

	this->m_QueueTimerInfo.clear();
}

void CQueueTimer::CreateTimer(int TimerIndex, int TimerDelay, WAITORTIMERCALLBACK CallbackFunction)
{
	QUEUE_TIMER_INFO QueueTimerInfo;

	QueueTimerInfo.TimerIndex = TimerIndex;

	CreateTimerQueueTimer(&QueueTimerInfo.QueueTimerTimer, this->m_QueueTimer, CallbackFunction, (PVOID)TimerIndex, 1000, TimerDelay, WT_EXECUTEINTIMERTHREAD);

	this->m_QueueTimerInfo.insert(std::pair<int, QUEUE_TIMER_INFO>(QueueTimerInfo.TimerIndex, QueueTimerInfo));
}

void CQueueTimer::DeleteTimer(int TimerIndex)
{
	std::map<int, QUEUE_TIMER_INFO>::iterator it = this->m_QueueTimerInfo.find(TimerIndex);

	if (it != this->m_QueueTimerInfo.end())
	{
		DeleteTimerQueueTimer(this->m_QueueTimer, it->second.QueueTimerTimer, 0);

		this->m_QueueTimerInfo.erase(it);
	}
}