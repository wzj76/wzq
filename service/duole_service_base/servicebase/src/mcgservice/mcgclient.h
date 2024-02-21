#ifndef __CLIENT_AGENT_H__
#define __CLIENT_AGENT_H__

#include "interface.h"
#include "servicebase.h"
#include <list>

#include "SeqUtil.h"

using namespace std;

struct conn;
struct lws ;

class CMCGClient  : public IClient
{
public:
	CMCGClient(DWORD dwClientIP, WORD wClientPort);
	virtual ~CMCGClient();

public:
	virtual bool OnInit();	//初始化

	virtual bool OnMsg(DWORD dwMsgID, DWORD dwSeqID);	//处理一个消息

	virtual bool OnReceive(BYTE* pbyBuffer, int nBufferLen);//通讯层告诉client后处理接收事件

	virtual bool OnSendMsg(BYTE* pbyBuffer, DWORD dwBufLen);//发送消息

	virtual bool OnClose();//通讯层告诉client后处理close事件

	virtual bool OnBreak(DWORD dwType);//在断线时附加处理函数，主要为解决断线续玩而设计的(1=被客户端断开，2=超时未收到心跳)

	virtual bool OnTerminate();	//主动断线后的善后处理

	virtual bool OnTimeOut(DWORD dwEventID);//定时器 返回FALSE 会终止事件泵循环 所以上层使用默认情况都是TURE

public:
	bool SendMsg(DWORD dwMsgID, PROTOBUF_MESSAGE *pMsg, DWORD dwSeqID=0);//client发送数据
    
    bool SendMsg(DWORD dwMsgID, const std::string& strBuffer, DWORD dwSeqID=0);

	bool GetSendMsgPacket(DWORD &dwBufLen, BYTE *&pbyBuffer);	//获取client待发送的消息包

	bool UpdateSendMsgPacket(DWORD dwOffSet, DWORD dwLen);		//move剩余未发送的消息包

	bool DelSendMsgPacket();									//删除client已发送的消息包

	void ReleaseSendMsgQueue();	//释放client发送的消息包队列

	int AddEvent(EVENTITEM* pstEventItem);//挂载异步消息入队

	bool McgPopPacket(string &buffer);//获得第一原始包的数据

	bool McgPopPacket(PROTOBUF_MESSAGE &message);//获得第一解析完包的数据

	void AddTimer(HANDLE hTimer);//增加Timer

	void DelTimer(HANDLE hTimer);//删除Timer

	bool DelAllTimer();//删除所有定时器

public:
	conn *m_pConn;							//网络层连接信息
	lws *m_pWSI;                            //websocket instance
    
	long long m_dwTickCountLastEvent;			//最后消息时间
	
	DWORD m_dwRemoteIP;						//远端客户端IP

	WORD m_wListenPort;						//本client从哪个监听端口来的

    bool m_bWSConn;                         //是否是websocket连接
    BYTE m_nBreak;                          //客户端断线
private:
	list <HANDLE> m_listTimerList;			//定时器列表

	list <EVENTITEM *> m_listSendMsgPacket; //客户端发送消息队列

	BYTE* m_pbyBuffer;						//接收数据Buffer

	int m_nStartPos;						//原始包起始位置

	int m_nCurrentPos;						//原始包当前位置

	int m_nBufferLen;						//原始包接收数据长度

	int m_nPBStartPos;						//PB数据起始位置

	int m_nPBCurrentLen;					//PB数据长度
};

#endif // __CLIENT_AGENT_H__
