//
//  GameSocketProxy.h
//  BaoHuang
//
//  Created by 曾琪 on 14-7-21.
//
//

#ifndef BaoHuang_GameSocketProxy_h
#define BaoHuang_GameSocketProxy_h

#include "socketx/ccgamesocket.h"
#include "eff_comm/workqueue.h"
#include "cocos2d.h"
#include "Protocol/ProtocolDef.h"

typedef void (cocos2d::Ref::*SEL_OnConnHandler)(bool bConn);
typedef void (cocos2d::Ref::*SEL_OnCloseHandler)();
typedef void (cocos2d::Ref::*SEL_OnMsgHandler)(const Message*);
typedef void (cocos2d::Ref::*SEL_OnSeqUpdateHandler)(unsigned int nSeq);

#define onConnHandler_selector(_SELECTOR) (SEL_OnConnHandler)(&_SELECTOR)
#define onCloseHandler_selector(_SELECTOR) (SEL_OnCloseHandler)(&_SELECTOR)
#define onMsgHandler_selector(_SELECTOR) (SEL_OnMsgHandler)(&_SELECTOR)
#define onSeqUpdateHandler_selector(_SELECTOR) (SEL_OnSeqUpdateHandler)(&_SELECTOR)

class WZQMsgCache : public cocos2d::Ref
{
public:
    WZQMsgCache(unsigned nMsgId, Message *pTemplate, cocos2d::Ref *pTarget, SEL_OnMsgHandler pFunc);
    ~WZQMsgCache();
    
    static WZQMsgCache* create(unsigned nMsgId, Message *pTemplate, cocos2d::Ref *pTarget, SEL_OnMsgHandler pFunc);
    
    unsigned int GetMsgId();
    
    Message* ParseMessage(unsigned char* pBdy, int nBdyLen);
    
    void ExecuteOnWorkQ(void* pParam);
    
private:
    unsigned int        m_nMsgId;
    Message             *m_pTemplate;
    cocos2d::Ref        *m_pTarget;
    SEL_OnMsgHandler    m_pFunc;
};

class SeqUpdate : public cocos2d::Ref
{
public:
    SeqUpdate(cocos2d::Ref* pTarget, SEL_OnSeqUpdateHandler pFunc);
    ~SeqUpdate();
    static SeqUpdate* create(cocos2d::Ref* pTarget, SEL_OnSeqUpdateHandler pFunc);
    void updateSeq(unsigned int nSeq);
    
private:
    cocos2d::Ref*           m_pTarget;
    SEL_OnSeqUpdateHandler  m_pFunc;
};

class WZQGameSocketProxy : public cocos2d::Ref, public CCGameSocket
{
public:
    WZQGameSocketProxy();
    ~WZQGameSocketProxy();
    
    void Reset();
    
    void RegisterOnConnected(cocos2d::Ref *pTarget, SEL_OnConnHandler pFunc);
    void RegisterOnClosed(cocos2d::Ref *pTarget, SEL_OnCloseHandler pFunc);
    void RegisterMsg(unsigned int nMsgId, Message *pTemplate, cocos2d::Ref *pTarget, SEL_OnMsgHandler pFunc);
    void RegisterSeqUpdate(cocos2d::Ref* pTarget, SEL_OnSeqUpdateHandler pFunc);
    
    unsigned int GetLastReceivedMsgSeqId();
    
public:
    //重载函数
	void OnConnected(bool bConnected);
    void OnClose();
    
protected:
    void OnConnectedOnWorkQ(void* pParam);
    void OnClosedOnWorkQ(void* pParam);
    void OnSeqUpdate(unsigned int nSeq);
    
    void OnProcessMsg(PMSGHEADER pHdr, unsigned char* pBdy, int nBdyLen);
    
    void PushWorkQ(cocos2d::Ref *pTarget, SEL_workQHandler pFunc, void* pParam, bool bDelay = false);
    
    bool NeedDelayExecute(unsigned int nMsgId);
    
protected:
    cocos2d::Ref            *m_pTargetOnConnected;
    SEL_OnConnHandler       m_pFuncOnConnected;
    
    cocos2d::Ref            *m_pTargetOnClosed;
    SEL_OnCloseHandler      m_pFuncOnClosed;
    
    cocos2d::__Array        *m_pMsgCaches;
    
    CWorkQueue              *m_pWorkQ;
    
    unsigned int            m_nLastReceivedMsgSeqId;
    
    cocos2d::Vector<SeqUpdate*> m_oSeqUpdates;
};

#endif
