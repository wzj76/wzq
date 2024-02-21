//
//  GameSocketProxy.cpp
//  BaoHuang
//
//  Created by 曾琪 on 14-7-21.
//
//

#include "WZQGameSocketProxy.h"

USING_NS_CC;
using namespace std;

WZQMsgCache::WZQMsgCache(unsigned nMsgId, Message *pTemplate, cocos2d::Ref *pTarget, SEL_OnMsgHandler pFunc)
:
m_nMsgId(nMsgId),
m_pTemplate(pTemplate),
m_pTarget(pTarget),
m_pFunc(pFunc)
{
    CC_SAFE_RETAIN(m_pTarget);
}

WZQMsgCache::~WZQMsgCache()
{
    CC_SAFE_RELEASE_NULL(m_pTarget);
    delete m_pTemplate;
}

WZQMsgCache* WZQMsgCache::create(unsigned int nMsgId, Message *pTemplate, Ref *pTarget, SEL_OnMsgHandler pFunc)
{
    WZQMsgCache *pRet = new WZQMsgCache(nMsgId, pTemplate, pTarget, pFunc);
    pRet->autorelease();
    return pRet;
}

unsigned int WZQMsgCache::GetMsgId()
{
    return m_nMsgId;
}

Message* WZQMsgCache::ParseMessage(unsigned char* pBdy, int nBdyLen)
{
    m_pTemplate->Clear();
    if (nBdyLen > 0)
    {
        m_pTemplate->ParseFromArray((void*)pBdy, nBdyLen);
    }
    Message* pRet = m_pTemplate->New();
    pRet->CopyFrom(*m_pTemplate);
    return pRet;
}

void WZQMsgCache::ExecuteOnWorkQ(void *pParam)
{
    Message *pMsg = (Message*)pParam;
    if (m_pTarget && m_pFunc)
    {
        (m_pTarget->*m_pFunc)(pMsg);
    }
    delete pMsg;
}

//------------------------------------------------------------------------------------//
SeqUpdate::SeqUpdate(cocos2d::Ref* pTarget, SEL_OnSeqUpdateHandler pFunc)
:
m_pTarget(pTarget),
m_pFunc(pFunc)
{
    CC_SAFE_RETAIN(m_pTarget);
}

SeqUpdate::~SeqUpdate()
{
    CC_SAFE_RELEASE_NULL(m_pTarget);
    m_pFunc = nullptr;
}

SeqUpdate* SeqUpdate::create(Ref *pTarget, SEL_OnSeqUpdateHandler pFunc)
{
    SeqUpdate *pRet = new SeqUpdate(pTarget, pFunc);
    pRet->autorelease();
    return pRet;
}

void SeqUpdate::updateSeq(unsigned int nSeq)
{
    if (m_pTarget && m_pFunc)
    {
        (m_pTarget->*m_pFunc)(nSeq);
    }
}
//------------------------------------------------------------------------------------//

WZQGameSocketProxy::WZQGameSocketProxy()
:
m_pTargetOnConnected(nullptr),
m_pFuncOnConnected(nullptr),
m_pTargetOnClosed(nullptr),
m_pFuncOnClosed(nullptr),
m_pMsgCaches(nullptr),
m_pWorkQ(nullptr),
m_oSeqUpdates({}),
m_nLastReceivedMsgSeqId(0)
{
    m_pMsgCaches = CCArray::create();
    m_pMsgCaches->retain();
    
    m_pWorkQ = new CWorkQueue();
    m_pWorkQ->autorelease();
    m_pWorkQ->retain();
}

WZQGameSocketProxy::~WZQGameSocketProxy()
{
    CC_SAFE_RELEASE_NULL(m_pTargetOnConnected);
    CC_SAFE_RELEASE_NULL(m_pTargetOnClosed);
    CC_SAFE_RELEASE_NULL(m_pMsgCaches);
    CC_SAFE_RELEASE_NULL(m_pWorkQ);
}

void WZQGameSocketProxy::Reset()
{
    CC_SAFE_RELEASE_NULL(m_pTargetOnConnected);
    m_pFuncOnConnected = nullptr;
    CC_SAFE_RELEASE_NULL(m_pTargetOnClosed);
    m_pFuncOnClosed = nullptr;
    m_pMsgCaches->removeAllObjects();
    m_nLastReceivedMsgSeqId = 0;
    m_oSeqUpdates.clear();
}

void WZQGameSocketProxy::RegisterOnConnected(Ref *pTarget, SEL_OnConnHandler pFunc)
{
    if (pTarget == nullptr || pFunc == nullptr)
    {
        return;
    }
    CC_SAFE_RELEASE(m_pTargetOnConnected);
    m_pTargetOnConnected = pTarget;
    m_pFuncOnConnected = pFunc;
    CC_SAFE_RETAIN(m_pTargetOnConnected);
}

void WZQGameSocketProxy::RegisterOnClosed(Ref *pTarget, SEL_OnCloseHandler pFunc)
{
    if (pTarget == nullptr || pFunc == nullptr)
    {
        return;
    }
    CC_SAFE_RELEASE(m_pTargetOnClosed);
    m_pTargetOnClosed = pTarget;
    m_pFuncOnClosed = pFunc;
    CC_SAFE_RETAIN(m_pTargetOnClosed);
}

void WZQGameSocketProxy::RegisterMsg(unsigned int nMsgId, Message *pTemplate, Ref *pTarget, SEL_OnMsgHandler pFunc)
{
    if (nMsgId == 0 || pTemplate == nullptr || pTarget == nullptr || pFunc == nullptr)
    {
        return;
    }
    WZQMsgCache *pCache = WZQMsgCache::create(nMsgId, pTemplate, pTarget, pFunc);
    m_pMsgCaches->addObject(pCache);
}

void WZQGameSocketProxy::RegisterSeqUpdate(cocos2d::Ref *pTarget, SEL_OnSeqUpdateHandler pFunc)
{
    if (pTarget == nullptr || pFunc == nullptr)
    {
        return;
    }
    SeqUpdate *pSeqUpdate = SeqUpdate::create(pTarget, pFunc);
    m_oSeqUpdates.pushBack(pSeqUpdate);
}

unsigned int WZQGameSocketProxy::GetLastReceivedMsgSeqId()
{
    return m_nLastReceivedMsgSeqId;
}

void WZQGameSocketProxy::OnConnected(bool bConnected)
{
    CCGameSocket::OnConnected(bConnected);
    PushWorkQ(this, workQ_selector(WZQGameSocketProxy::OnConnectedOnWorkQ), (void*)bConnected);
}

void WZQGameSocketProxy::OnClose()
{
    CCGameSocket::OnClose();
    PushWorkQ(this, workQ_selector(WZQGameSocketProxy::OnClosedOnWorkQ), nullptr);
}

void WZQGameSocketProxy::OnConnectedOnWorkQ(void *pParam)
{
    if (m_pTargetOnConnected && m_pFuncOnConnected)
    {
        (m_pTargetOnConnected->*m_pFuncOnConnected)((bool)pParam);
    }
}

void WZQGameSocketProxy::OnClosedOnWorkQ(void *pParam)
{
    if (m_pTargetOnClosed && m_pFuncOnClosed)
    {
        (m_pTargetOnClosed->*m_pFuncOnClosed)();
    }
}

void WZQGameSocketProxy::OnSeqUpdate(unsigned int nSeq)
{
    for (size_t i = 0; i < m_oSeqUpdates.size(); i ++)
    {
        m_oSeqUpdates.at(i)->updateSeq(nSeq);
    }
}

void WZQGameSocketProxy::OnProcessMsg(PMSGHEADER pHdr, unsigned char *pBdy, int nBdyLen)
{
    Ref *pObj;
    CCARRAY_FOREACH(m_pMsgCaches, pObj)
    {
        WZQMsgCache *pCache = (WZQMsgCache*)pObj;
        if (pCache->GetMsgId() == pHdr->dwType)
        {
//            if (pCache->GetMsgId() == (MSGID_HOLD | MSGID_ACK))
//            {
//                pCache->ExecuteOnWorkQ((void*)pCache->ParseMessage(pBdy, nBdyLen));
//                break;
//            }
            bool bDelay = NeedDelayExecute(pCache->GetMsgId());
            PushWorkQ(pCache, workQ_selector(WZQMsgCache::ExecuteOnWorkQ), (void*)pCache->ParseMessage(pBdy, nBdyLen), bDelay);
            break;
        }
    }
    m_nLastReceivedMsgSeqId = pHdr->dwSeqID;
    OnSeqUpdate(m_nLastReceivedMsgSeqId);
}

void WZQGameSocketProxy::PushWorkQ(Ref *pTarget, SEL_workQHandler pFunc, void *pParam, bool bDelay)
{
    m_pWorkQ->PushItem(pTarget, pFunc, pParam, bDelay);
}

bool WZQGameSocketProxy::NeedDelayExecute(unsigned int nMsgId)
{
    return false;
}
