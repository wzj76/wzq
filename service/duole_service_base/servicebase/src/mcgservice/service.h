#ifndef __SERVICE_H__
#define __SERVICE_H__

#include "interface.h"
#include "servicebase.h"
#include "mcgtimer.h"
#include "luaini.h"
#include "globalfunc.h"
#include "protocol.h"
#include "mylibevent.h"
#include "mcgutil/RedisIntf.h"
#include <event.h>
#include <string>
#include <map>
#include <list>
#include <string>
#include <queue>

using namespace std;

struct sockpair;

using google::protobuf::int64;
class CService : public IService
{
public:
	CService();
	virtual ~CService();

public:
	bool StartService();

public:
	//创建client对象
	virtual IEventPump* mcgclient(DWORD dwClientIP, WORD wClientPort);

	//初始化函数
	virtual bool OnInit();

	//接收一个消息
	virtual bool OnMsg(DWORD dwMsgID, DWORD dwSeqID);

	//通讯层告诉service后处理接收事件
	virtual bool OnReceive(BYTE* pbyBuffer, int nBufferLen);

    virtual bool OnReceiveQueueMsg(BYTE* pbyBuffer, int nBufferLen);
	//定时函数
	virtual bool OnTimeOut(DWORD dwEventID);

	//程序要终止了
	virtual void OnExitProcess(UINT uExitCode);

	//处理一个异步消息
	virtual bool OnAsyncMsg(DWORD dwMsgID, DWORD dwSeqID, int nErrorCode, void* pvContext);

    //处理业务逻辑
    virtual bool DoBusinessWork(){ return true ;}
    virtual void OnSvrdClose(){}
public:
	int AddEvent(EVENTITEM* pstEventItem);						//挂载异步消息入队

	bool GetSvrConnRecvMsgPacket(int iSize);					//获取挂在异步消息
    
    int AddEventQueueMsg(const std::string& strMsg) ;
    bool GetSvrConnQueueMsgPacket(int iSize);

	bool McgPopPacket(string &buffer);						//获得第一原始包的数据

	bool McgPopPacket(PROTOBUF_MESSAGE &message);		//获得第一解析完包的数据

	DWORD CurrentClientCount();									//当前在线客户端数

	DWORD GetClientID();										//获得ClientID编号

	IEventPump* GetClientByID(DWORD dwSeqID);					//通过ClientID编号获取Client

	DWORD GetLocalIP();											//获得本服务器IP

	WORD  GetFirstPort();										//获得第一个监听端口
	
	void SetAppServiceVersion(string strAppServiceVersion);		//设置App服务版本号
    bool PopSndQueueMsg(std::string& strMsg);
    
    int AddSndQueueMsg(const std::string& strMsg);
private:
	bool InitServiceBase();										//初始化基类	
    
    //create socketpair
    sockpair* CreateSocketPair2(conn_type type);

	bool CreateSocketPair();									//创建socket_pair

	int GetSocketPair(void *pSvrConnAddr);						//获取socket pair

	void DelSocketPair(void *pSvrConnAddr);						//删除socket pair

public:
	struct event_base *main_base;								//网络库实例

	struct event time_ev;										//定时器扫描频率（单位秒）
	struct timeval time_tv;										//定时器扫描频率（单位微秒）
	DWORD m_tTimerHeartBeat;									//客户端心跳频率（单位毫秒）
	DWORD m_tTimerHalfStatus;									//半决状态检测频率（毫秒）
	DWORD m_tTimerStatBaseStatus;								//基类服务状态统计频率（单位毫秒）
	list <int> m_listListenPortList;							//监听端口号列表

	map <DWORD, IEventPump *> m_mapClientList;					//在线Client列表

	DWORD m_dwAsyncType;										//异步结构类型

	int	m_nAsyncErrorCode;										//异步消息错误码

	void *m_paContext;											//异步消息APP上下文指针
    std::queue<std::string> m_qQueueMsgs ;
	
    // 获取服务器id
    std::string GetSvrdID() ;               // deprecated, 用 getServiceID 替代；
	// 获取服务器名字
	std::string GetSvrdName() ;             // deprecated, 用 getServiceID 替代；
private:
	char m_szServiceName[MAX_PATH];								//服务名称

	char m_szConfigFileFullPath[MAX_PATH];						//配置文件全路径

	string m_strListenPortList;									//监听端口号列表
    string m_strListenPortListWS;

	DWORD m_dwLocalIP;											//获得本地IP

	WORD m_wFirstPort;											//第一个监听端口

	list <EVENTITEM *> m_listSvrConnRecvMsgPacket;				//挂载对象收到数据队列

	BYTE* m_pbyBuffer;											//异步数据Buffer

	int m_nBufferLen;											//异步数据长度

	string m_strLogPath;										//日志路径

	char m_szRoutingKey[ROUTINGKEY_MAXLEN];						//MQ RoutingKey

	DWORD m_dwClientID;											//客户端ID编号

	char m_szSendBuffer[TCP_READ_BUFFER_SIZE];					//发送Buffer

	HEADER m_stHeader;										//包头

	HANDLE m_hInitTimer;										//初始化定时器

	long long m_dwStartRead;									//统计第三方库性能

	sockpair *m_pSocketPair;									//socket_pair 
	
	string m_strAppServiceVersion;								//App服务版本号
    
    sockpair* m_pMsgQueueSocketPair ;
    
    //  deprecated: m_strServiceID 替代；
    std::string     m_svrdID       ;                            //服务器id
    //  deprecated: m_strServiceID 替代；
    std::string     m_svrdname     ;                            //服务器名称

	
    //发送消息队列
    std::queue<std::string>                     m_qSndQueueMsgs ;
    
public:
    // 获取 redis 操作句柄;
    inline CRedisIntf* cache() { return this->m_pCache; };
    inline CRedisIntf* mqr() { return this->m_pMQR; };
    inline CRedisIntf* mqw() { return this->m_pMQW; };
    // 获取 redis 配置信息;
    inline REDIS_CONFIG & getCacheConf() { return m_conf_cache; };
    inline REDIS_CONFIG & getMQRConf() { return m_conf_mqr; };
    inline REDIS_CONFIG & getMQWConf() { return m_conf_mqw; };
    // 获取服务名称: 仅用于标识服务器程序名称，启动脚本中使用，消息路由寻址不应该使用这个值；
    inline std::string & getServiceName() { return m_strServiceName; };
    inline std::string & getServiceID() { return m_strServiceID; };
    inline std::string & getAppType() { return m_strAppType; };
    // 服务唯一识别特征串: APP_TYPE + SERVICE_TYPE + SERVICE_ID;
    inline std::string & getServiceTag() { return m_strServiceTag; };
private:
    bool _init_redis_cache();
    bool _init_redis_mqr();
    bool _init_redis_mqw();
    void _init_baseinfo();
    bool _init_svrinfo();
    //  初始化网络服务；
    bool _init_network();
private:
    //  redis 配置解析和初始化；
    //  conf/redis.conf
    CLuaIni m_hLuaIni_Redis;
    //  CACHE 对象句柄；
    CRedisIntf * m_pCache;
    REDIS_CONFIG m_conf_cache;
    //  本地进程消息队列：读
    CRedisIntf * m_pMQR;
    REDIS_CONFIG m_conf_mqr;
    //  其它进程消息队列：写
    CRedisIntf * m_pMQW;
    REDIS_CONFIG m_conf_mqw;
    
    //  服务属性信息；
    std::string m_strAppType;        //  游戏类型
    std::string m_strServiceID;      //  服务ID
    std::string m_strServiceName;    //  服务名称
    std::string m_strServiceTag;     //  服务平台级唯一标识：APP_TYPE + SERVICE_TYPE + SERVICE_ID;
    
};


extern CLuaIni g_oLuaIni;										//Lua全局配置文件读对象

extern pthread_mutex_t third_party_lib_lock;					//第三方库全局锁

#endif // __SERVICE_H__
