#ifndef __ITERFACE_H__
#define __ITERFACE_H__

#include "datatype.h"

#include "google/protobuf/generated_message_util.h"
#include "google/protobuf/repeated_field.h"
#include "google/protobuf/extension_set.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/message.h"
#define	PROTOBUF_MESSAGE google::protobuf::Message

#define ROUTINGKEY_MAXLEN	256	

#define TYPE_NETWORK        1	//网络事件
#define TYPE_TIMER			2	//定时器事件
#define TYPE_TERMINATE		3	//终止事件
#define TYPE_PAUSE			4	//暂停事件
#define TYPE_ASYNC_NETWORK 	5	//异步事件
#define TYPE_BROADCAST_NETWORK 6	//广播工作事件
#define TYPE_ASERVER_EVENT	7	//ASERVER服务器事件
#define TYPE_ASYNC_MQ 		8	//异步MQ事件

//网络EVENT
#define EVENT_SEND			1	//发送事件
#define EVENT_RECEIVE		2	//接收事件
#define EVENT_CLOSE			3	//关闭事件
#define EVENT_REMOVESUCCESS 4	//删除成功事件
#define EVENT_SENDERR		5	//发送数据失败事件

//IEVENT Type
#define TYPE_SERVICE						1 //SERVICE	   类型
#define TYPE_CLIENT						2 //标准client 类型
#define TYPE_SVRCONNECT					3 //SVRCONNECT 类型

//定时器事件EVENT
#define TIMER_SERVICEBASE						0x00000000 
#define TIMER_GAMESERVER						0x00100000
//#define TIMER_RECYCLE                           (TIMER_GAMESERVER + 1)
//#define TIMER_EPT1                              (TIMER_GAMESERVER + 2)
//#define TIMER_EPT2                              (TIMER_GAMESERVER + 3) 

#define TIMER_SLEEPTIMEOUT						(TIMER_SERVICEBASE+0x00000001) //休眠超时 需要断掉客户端
#define TIMER_TIMEOUT							(TIMER_SERVICEBASE+0x00000002) //超时时间
#define TIMER_TERMINATE							(TIMER_SERVICEBASE+0x00000003) //终止TIMER
#define TIMER_SLEEPRESTORE						(TIMER_SERVICEBASE+0x00000004) //休眠恢复TIMER
#define TIMER_RUN								(TIMER_SERVICEBASE+0x00000005) //运行TIMER 为了使其运行起来
#define TIMER_TERMINATEAFTERSEND				(TIMER_SERVICEBASE+0x00000006) //处理完后终止TIMER
#define TIMER_IDLE								(TIMER_SERVICEBASE+0x00000007) //service idle TIMER
#define TIMER_SETTING							(TIMER_SERVICEBASE+0x00000008) //读配置文件专用 TIMER
#define TIMER_DELETECLIENT						(TIMER_SERVICEBASE+0x00000009) //删除client队列 TIMER
#define TIMER_KS								(TIMER_SERVICEBASE+0x0000000a) //ks TIMER
#define TIMER_STAT								(TIMER_SERVICEBASE+0x0000000b) //STAT TIMER

#pragma pack(1)
typedef struct _EVENTITEM
{
	DWORD dwType;								//结构类型
	DWORD dwEventID;							//事件类型
	int nBufferLen;								//接收到的数据长度 
	BYTE* pbyBuffer;							//接收到的数据缓冲区
	void* pvContext;							//上下文指针
	void* paContext;							//APP上下文指针
	char szRoutingKey[ROUTINGKEY_MAXLEN];		//MQ RoutingKey
}EVENTITEM,*PEVENTITEM;
#pragma pack()

typedef struct _ASYNCMSG
{
	DWORD dwMsgID;//返回消息id
	int	nErrorCode;//错误码
	void* pvContext;//上下文指针
	int nBufferLen;			//接收到的数据长度 
	BYTE* pbyBuffer;		//接收到的数据缓冲区
}ASYNCMSG,*PASYNCMSG;

//IEventPump对象
class IEventPump
{
public:
	IEventPump(){};
	virtual ~IEventPump(){}; 
	virtual int AddEvent(EVENTITEM* pstEventItem)=0;
	DWORD GetClientID() const { return m_dwClientID; }
	void SetClientID(DWORD dwClientID) {m_dwClientID = dwClientID; }

public:
	WORD m_wIEventType;

private:
	DWORD	m_dwClientID; //唯一序列号(sequence)
};


//IService对象
class IService : public IEventPump
{
public:
	virtual IEventPump* mcgclient(DWORD dwIP,WORD wPort)=0;
};


//IClient对象
class IClient : public IEventPump
{
};


#endif //__ITERFACE_H__
