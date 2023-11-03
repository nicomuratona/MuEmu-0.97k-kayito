#include "stdafx.h"
#include "Queue.h"

CQueue::CQueue()
{

}

CQueue::~CQueue()
{
	this->ClearQueue();
}

void CQueue::ClearQueue()
{
	this->m_critical.lock();

	this->m_QueueInfo.swap(std::queue<QUEUE_INFO>());

	this->m_critical.unlock();
}

DWORD CQueue::GetQueueSize()
{
	DWORD size = 0;

	this->m_critical.lock();

	size = this->m_QueueInfo.size();

	this->m_critical.unlock();

	return size;
}

bool CQueue::AddToQueue(QUEUE_INFO* lpInfo)
{
	bool result = false;

	this->m_critical.lock();

	if (this->m_QueueInfo.size() < MAX_QUEUE_SIZE)
	{
		this->m_QueueInfo.push((*lpInfo));

		result = true;
	}

	this->m_critical.unlock();

	return result;
}

bool CQueue::GetFromQueue(QUEUE_INFO* lpInfo)
{
	bool result = false;

	this->m_critical.lock();

	if (this->m_QueueInfo.empty() == false)
	{
		(*lpInfo) = this->m_QueueInfo.front();

		this->m_QueueInfo.pop();

		result = true;
	}

	this->m_critical.unlock();

	return result;
}