#include "WZQMsgService.h"

USING_NS_CC;

WZQMsgService::WZQMsgService()
:
m_pSocket(nullptr),
m_Port(0)
{
    memset(m_szIP, 0, sizeof(m_szIP));
    
    InitializeCriticalSection(&m_cs);
    
    m_pSocket = new WZQGameSocketProxy();
    m_pSocket->autorelease();
    m_pSocket->retain();
}

WZQMsgService::~WZQMsgService()
{
    DeleteCriticalSection(&m_cs);
    
    CC_SAFE_RELEASE_NULL(m_pSocket);
}

//______________________________________________________________________________________
//连绑定的Socket，IP和Port为绑定时指定内容
void WZQMsgService::Connect(const char* szIP, unsigned short nPort)
{
    if (szIP == nullptr || strlen(szIP) == 0 || nPort == 0)
	{
        return;
	}
    strncpy(m_szIP, szIP, sizeof(m_szIP) - 1);
	m_Port = nPort;
    m_pSocket->Connect(m_szIP, m_Port);
}
//______________________________________________________________________________________
//关闭连接
void WZQMsgService::Close()
{
    m_pSocket->Close();
}
//______________________________________________________________________________________
//发送消息，如果未曾连接则先连接再发送
void WZQMsgService::SendMsg(Message *pMsg, unsigned int dwType, unsigned int dwSeqID/*=0*/)
{
    if (pMsg == nullptr && dwType == 0 && dwSeqID == 0)
    {
        return;
    }
	EnterCriticalSection(&m_cs);
	if (m_pSocket->IsConnected())
	{
        //已连接直接发消息
        m_pSocket->SendMsg(pMsg, dwType, dwSeqID);
        //log("MsgService::SendMsg >>> Send Complete[ID=0x%08X]", dwType);
	}
	else if (m_pSocket->IsConnecting())
	{
        PSTANDBYMSG pcasi = new STANDBYMSG();
        pcasi->pMsg = nullptr;
        if (pMsg)
        {
            pcasi->pMsg = pMsg->New();
            pcasi->pMsg->CopyFrom(*pMsg);
            pcasi->pMsg->SpaceUsed();
        }
        pcasi->dwType = dwType;
        pcasi->dwSeqID = dwSeqID;
        m_listMsg.AddTail((void*)pcasi);
        log("MsgService::SendMsg >>> Push Standby[ID=0x%08X, Count=%d]", dwType, m_listMsg.GetCount());
    }
    else
    {
        log("MsgService::SendMsg >>> No Connection");
	}
	LeaveCriticalSection(&m_cs);
}
//______________________________________________________________________________________
//获得由于未连接成功导致未发送成功的数据
//将数据复制到pList当中，请自行释放内存，其中pMsg也需要delete。bClear==true复制后清除
//可以重载OnConnected来处理连接失败
void WZQMsgService::DumpStandbyMsg(CPtrlist* pList, bool bClear/*=true*/)
{
	if (!pList) return;
	EnterCriticalSection(&m_cs);
	NODEPOS pos = m_listMsg.GetHeadPos();
	while (pos != 0)
	{
		PSTANDBYMSG p = reinterpret_cast<PSTANDBYMSG>(m_listMsg.GetNext(pos));
		if (p)
		{
			PSTANDBYMSG pTmp = new STANDBYMSG();
			pTmp->pMsg = 0;
			if (p->pMsg)
			{
				pTmp->pMsg = p->pMsg->New();
				pTmp->pMsg->CopyFrom(*(p->pMsg));
				if (bClear)
				{
					delete p->pMsg;
					p->pMsg = 0;
				}
			}
			pTmp->dwType = p->dwType;
			pTmp->dwSeqID = p->dwSeqID;
			pList->AddTail(pTmp);
			if (bClear)
			{
				delete p;
				p = 0;
			}
		}
	}
	if (bClear)
	{
		m_listMsg.RemoveAll();
	}
	LeaveCriticalSection(&m_cs);
}
//______________________________________________________________________________________
//如果连接失败会有未发送数据，可以调用此函数清理
void WZQMsgService::ClearStandbyMsg()
{
	EnterCriticalSection(&m_cs);
	NODEPOS pos = m_listMsg.GetHeadPos();
	while (pos != 0)
	{
		PSTANDBYMSG p = reinterpret_cast<PSTANDBYMSG>(m_listMsg.GetNext(pos));
		if (p)
		{
			if (p->pMsg)
			{
				delete p->pMsg;
				p->pMsg = 0;
			}
			delete p;
			p = 0;
		}
	}
	m_listMsg.RemoveAll();
	LeaveCriticalSection(&m_cs);
}
//______________________________________________________________________________________
unsigned int WZQMsgService::GetLastReceivedMsgSeqId()
{
    return m_pSocket->GetLastReceivedMsgSeqId();
}
//______________________________________________________________________________________
void WZQMsgService::Configure()
{
    m_pSocket->Reset();
    m_pSocket->RegisterOnConnected(this, onConnHandler_selector(WZQMsgService::OnServiceConnected));
    m_pSocket->RegisterOnClosed(this, onCloseHandler_selector(WZQMsgService::OnServiceClosed));
}
//______________________________________________________________________________________
void WZQMsgService::OnServiceConnected(bool bConnected)
{
	EnterCriticalSection(&m_cs);
	log("MsgService::OnServiceConnected >>> %s", bConnected ? "Succeed" : "Failed");
	if (bConnected)
	{
		//发送所有待发数据
		NODEPOS pos = m_listMsg.GetHeadPos();
		while (pos != 0)
		{
			PSTANDBYMSG p = reinterpret_cast<PSTANDBYMSG>(m_listMsg.GetNext(pos));
			if (p != 0)
			{
				//发送消息
                log("MsgService::OnServiceConnected >>> Send Standby Message[ID=0x%08X]", p->dwType);
                m_pSocket->SendMsg(p->pMsg, p->dwType, p->dwSeqID);
				if (p->pMsg)
				{
					delete p->pMsg;
					p->pMsg = 0;
				}
				delete p;
				p = 0;
			}
		}
		m_listMsg.RemoveAll();
	}
	LeaveCriticalSection(&m_cs);
}
//______________________________________________________________________________________
void WZQMsgService::OnServiceClosed()
{
	log("MsgService::OnServiceClosed");
}
