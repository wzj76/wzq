#pragma once

#include "cocos2d.h"
#include "Core/AppFunc.h"
#include "paohuziProtocol.pb.h"
#include "eff_comm/ptrlist.h"
#include "socketx/ccgamesocket.h"
#include "Net/Socket/WZQGameSocketProxy.h"
#include "WZQEventWrapper.h"
#include "AppEvent.h"

#define SERVICE_FUNC(_NAME_)\
void _NAME_(Message *pMsg)\
{\
if (HasDelegate())\
{\
GetDelegate()->_NAME_(pMsg);\
}\
}

#define DELEGATE_FUNC(_NAME_)\
virtual void _NAME_(Message *pMsg) {}

//______________________________________________________________________________________
//待发消息信息结构
typedef struct
{
    Message *pMsg;
	unsigned int dwType;
	unsigned int dwSeqID;
} STANDBYMSG, *PSTANDBYMSG;
//______________________________________________________________________________________
//网络消息处理基类
class WZQMsgService : public cocos2d::Ref
{
public:
	WZQMsgService();
	~WZQMsgService();
    
	//连接服务器
	void Connect(const char* szIP, unsigned short nPort);
    
    //关闭连接
    void Close();
    
	//发送消息，如果未曾连接则先连接再发送
	void SendMsg(Message *pMsg, unsigned int dwType, unsigned int dwSeqID = 0);
	
	//获得由于未连接成功导致未发送成功的数据
	//将数据复制到pList当中，请自行释放内存，其中pMsg也需要delete。bClear==true复制后清除
	//可以重载OnConnected来处理连接失败
	void DumpStandbyMsg(CPtrlist* pList, bool bClear = true);
    
	//如果连接失败会有未发送数据，可以调用此函数清理
	void ClearStandbyMsg();
    
    unsigned int GetLastReceivedMsgSeqId();
    
public:
    //配置网络消息
    virtual void Configure();
    
protected:
    virtual void OnServiceConnected(bool bConnected);
    virtual void OnServiceClosed();
    
protected:
    WZQGameSocketProxy    *m_pSocket;         //绑定的Socket对象
	CRITICAL_SECTION    m_cs;               //锁
	CPtrlist            m_listMsg;          //存放末发送出去的消息
	char                m_szIP[MAX_IP_LEN];	//IP
	unsigned short      m_Port;             //Port
};
