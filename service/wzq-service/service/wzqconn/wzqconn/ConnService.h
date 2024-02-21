#ifndef _GAMEINFO_SERVICE_H_
#define _GAMEINFO_SERVICE_H_

#include "service.h"
#include "protocol.h"
#include "ProtocolDef.h"
#include "RedisIntf.h"
#include "PlayerMgr.h"
#include <set>

using google::protobuf::int32;
using google::protobuf::int64;
class CConnService : public CService
{
    friend class CLogSvrd ;
public:
	CConnService();
	virtual ~CConnService();

public:
	
	virtual IEventPump *mcgclient(DWORD dwIP,WORD wPort);

	
	virtual bool OnInit();

	
	virtual bool OnMsg(DWORD dwMsgID, DWORD dwSeqID);

	
	virtual bool OnTimeOut(DWORD dwEventID);

	virtual void OnExitProcess(UINT uExitCode);
    
    virtual bool OnReceiveQueueMsg(BYTE* pbyBuffer, int nBufferLen);
    
    virtual bool OnAsyncMsg(DWORD dwMsgID, DWORD dwSeqID, int nErrorCode, void* pvContext){return true;}
    
public :
    void OnSvrdClose();
    bool SvrdNeedClose();
    //获取该连接服务器的路由消息数据
    std::string GetConnSvrdMsgRoute();
    //获取鉴权服务路由信息
    std::string GetAuthSvrdMsgRoute();
    //获取匹配服务路由信息
    std::string GetRoomSvrdMsgRoute();
    //获取游戏服务器路由信息
    std::string GetGameSvrdMsgRoute();
    //向内部服务转发消息
    int SendSvrdMsg(const std::string& msgroute , int64 msg_id , int64 seq_id , int64 user_id , const std::string& msg, int64 nClientID);
    //向客户端转发消息
    int SendClientMsg(int64 userid , DWORD dwMsgID, const std::string& strBuffer, DWORD dwSeqID , int64 nClientID);
    //检查客户端重复登录
    void CheckForDuplicateLogins(int user_id);
private:
    bool InitRedis();
    
    bool InitRecvMsgQueueThread();
    
    static void* RecvProc(void* pParam);
    //初始化消息队列缓存
    bool InitMQRedis();
public:
	CRedisIntf*                         m_pRedis;
    CPlayerMgr*                         m_pPlayerMgr;
    bool                                m_bSvrdClose        ;  //服务器收到关闭的指令 ,即将关闭
};

#endif // _USER_HANDINFO_SERVICE_H_

