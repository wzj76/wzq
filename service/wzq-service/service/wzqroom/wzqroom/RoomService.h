#ifndef _GAMEINFO_SERVICE_H_
#define _GAMEINFO_SERVICE_H_

#include "service.h"
#include "protocol.h"
#include "ProtocolDef.h"
#include "RedisIntf.h"
#include "wzqProtocol.pb.h"

#include <unordered_map>
#include <mutex>
#include "PlayerInfo.hpp"
#include "matcher.hpp"
#include "InviteFriendGame.hpp"

using google::protobuf::int32;
using google::protobuf::int64;
class Matcher;
class CRoomService : public CService
{
    friend class CLogSvrd ;
public:
	CRoomService();
	virtual ~CRoomService();

public:
	
	virtual IEventPump *mcgclient(DWORD dwIP,WORD wPort);

	
	virtual bool OnInit();

	virtual bool OnTimeOut(DWORD dwEventID);

	virtual void OnExitProcess(UINT uExitCode);
    
    virtual bool OnReceiveQueueMsg(BYTE* pbyBuffer, int nBufferLen);
    
public :
    void OnSvrdClose();
    bool SvrdNeedClose();
    std::string GetRoomSvrdMsgRoute();
    //获取连接服的路由信息
    std::string GetConnSvrdMsgRoute();
    std::string GetGameSvrdMsgRoute();
    //向内部服务转发消息
    int SendSvrdMsg(const std::string& msgroute , int64 msg_id , int64 seq_id , int64 user_id , const std::string& msg,int64 nClientID);
private:
    bool InitRedis();
    
    bool InitRecvMsgQueueThread();
    
    static void* RecvProc(void* pParam);
    //初始化消息队列缓存
    bool InitMQRedis();
    //初始化连接服务消息队列
    bool InitConnMQRedis();
public:
	CRedisIntf*                         m_pRedis;
    bool                                m_bSvrdClose        ;  //服务器收到关闭的指令 ,即将关闭
    Matcher*                            m_matcher;
    InviteFriendGame*                   m_inviteFriendGame;
};

#endif // _USER_HANDINFO_SERVICE_H_

