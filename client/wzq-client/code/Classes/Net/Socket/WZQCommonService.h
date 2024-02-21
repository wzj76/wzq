//
//  CommonService.h
//  game
//
//  Created by Joi Yao on 16/10/17.
//
//

#pragma once

#include "WZQMsgService.h"
#include "WZQGameSocketProxy.h"
#include "Protocol/ProtocolDef.h"

#define RECEIVE_MSG(_MSGID_, _TEMPLATECLASS_, _FUNC_)\
m_pSocket->RegisterMsg(_MSGID_, new _TEMPLATECLASS_(), this, onMsgHandler_selector(_FUNC_));

class WZQCommonService : public cocos2d::Ref
{
public:
    virtual void Configure(WZQGameSocketProxy* pProxy, WZQMsgService* pMsgService);
    virtual void ConfigureMsgs() = 0;
    virtual void ClearStandbyMsg();
    
protected:
    void SendMsg(Message *pMsg, unsigned int dwType, unsigned int dwSeqID = 0);
    void RegisterMsg(unsigned int nMsgId, Message *pTemplate, cocos2d::Ref *pTarget, SEL_OnMsgHandler pFunc);
    void RegisterSeqUpdate(cocos2d::Ref* pTarget, SEL_OnSeqUpdateHandler pFunc);
    unsigned int GetLastReceivedMsgSeqId();
    
    
    WZQCommonService()
    :
    m_pSocket(nullptr),
    m_pMsgService(nullptr)
    {
    }
    
    ~WZQCommonService()
    {
        m_pSocket = nullptr;
        m_pMsgService = nullptr;
    }
    
protected:
    WZQGameSocketProxy*    m_pSocket;
    WZQMsgService*         m_pMsgService;
};
