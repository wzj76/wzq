#include "workqueue.h"

USING_NS_CC;

//_____________________________________________________________________________________________________________
CWorkQueue::CWorkQueue()
{
    m_bPauseExecute = false;
	pthread_mutex_init(&m_cs, nullptr);
    Director::getInstance()->getScheduler()->schedule(schedule_selector(CWorkQueue::Execute), this, 0, true);
}
//_____________________________________________________________________________________________________________
CWorkQueue::~CWorkQueue()
{
    m_bPauseExecute = false;
	Clear();
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(CWorkQueue::Execute), this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(CWorkQueue::ResumePause), this);
	pthread_mutex_destroy(&m_cs);
}
//_____________________________________________________________________________________________________________
bool CWorkQueue::PushItem(Ref* pObj, SEL_workQHandler pFunc, void* pParam, bool bDelay)
{
	if(!pObj || !pFunc)
    {
        return false;
    }
	PQUEUEITEM pItem = new QUEUEITEM();
	if(!pItem)
    {
        return false;
    }
	pObj->retain();
	pItem->pObj = pObj;
	pItem->pFunc = pFunc;
	pItem->pParam = pParam;
    pItem->bDelay = bDelay;
	Lock();
	m_queue.PushTail((void*)pItem);
	Unlock();
    Director::getInstance()->getScheduler()->resumeTarget(this);
	return true;
}
//_____________________________________________________________________________________________________________
void CWorkQueue::Clear()
{
	Lock();
	while(!m_queue.IsEmpty())
	{
		PQUEUEITEM pItem = (PQUEUEITEM)m_queue.PopHead();
		if(pItem)
		{
            pItem->pObj->release();
			delete pItem;
		}
	}
	Unlock();
}
//_____________________________________________________________________________________________________________
void CWorkQueue::Execute(float delta)
{
	if(m_queue.IsEmpty())
	{
        Director::getInstance()->getScheduler()->pauseTarget(this);
		return;
	}
    if (m_bPauseExecute)
    {
        return;
    }
    Lock();
	PQUEUEITEM pItem = (PQUEUEITEM)m_queue.PopHead();
	Unlock();
    
	if(pItem)
	{
        int nOverStockCount = 0; // 消息积压数量
        m_queue.EnumpPtrQueue(CheckOverStockCount, &nOverStockCount);
        if (pItem->bDelay && nOverStockCount > 1)
        {
            float fDelayTime = 0;
            if (nOverStockCount >= 10)
            {
                fDelayTime = 0.2;
            }
            else if (nOverStockCount >= 5)
            {
                fDelayTime = 0.3;
            }
            else
            {
                fDelayTime = 0.4;
            }
            if (fDelayTime != 0)
            {
                m_bPauseExecute = true;
                Director::getInstance()->getScheduler()->schedule(schedule_selector(CWorkQueue::ResumePause), this, fDelayTime, false);
            }
        }
        (pItem->pObj->*pItem->pFunc)(pItem->pParam);
        pItem->pObj->release();
        delete pItem;
        pItem = 0;
    }
}
//_____________________________________________________________________________________________________________
void CWorkQueue::Lock()
{
	pthread_mutex_lock(&m_cs);
}
//_____________________________________________________________________________________________________________
void CWorkQueue::Unlock()
{
	pthread_mutex_unlock(&m_cs);
}
//_____________________________________________________________________________________________________________
void CWorkQueue::ResumePause(float t)
{
    m_bPauseExecute = false;
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(CWorkQueue::ResumePause), this);
}
//_____________________________________________________________________________________________________________
//检测消息积压数量
bool CheckOverStockCount(void* pData, void* pContext)
{
    if (PQUEUEITEM(pData)->bDelay)
    {
        (*(int*)(pContext)) += 1;
    }
    return true;
}
