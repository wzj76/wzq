#include "ConnService.h"
#include "ConnClient.h"
#include "log.h"
#include "mcgevent.h"
#include "mcgcommon/datatype.h"
#include <json/json.h>
#include "./proto/wzqProtocol.pb.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include "ProtocolDef.h"

#define DB_HOST		             "DBHost"                  //
#define DB_PORT				     "DBPort"                  //
#define DB_USERNAME			     "DBUserName"       	   //
#define DB_PASSWORD			     "DBPassword"              //
#define DB_NAME				     "DBName"				   //
#define DB_CONN_NUM			     "DBConnNum"  			   //
#define REDIS_IP                 "RedisIP"
#define REDIS_PORT			     "RedisPort"
#define REDIS_PWD                "RedisPwd"

#define SHAREREDIS_IP			 "ShareRedisIP"            // 缓存服务IP
#define SHAREREDIS_PORT			 "ShareRedisPort"          // 缓存服务端口号
#define SHAREREDIS_PASSWD		 "ShareRedisPasswd"        // 缓存服务器密码

extern DWORD seqid_new();

CConnService::CConnService()
{
}

CConnService::~CConnService()
{
}

IEventPump *CConnService::mcgclient(DWORD dwIP,WORD wPort)
{
	CConnClient *pClient=new CConnClient(dwIP,wPort);
	return pClient;
}


bool CConnService::OnInit()
{
	if (!CService::OnInit()) return false;
	m_pPlayerMgr = new CPlayerMgr();
    
	if (!InitRedis())
	{
		mcgWriteLog("初始化 redis 失败了");
		return false;
	}
    //初始化读取消息队列的线程
    InitRecvMsgQueueThread();
	return true;
}

bool CConnService::InitRecvMsgQueueThread()
{
    pthread_t nThreadID ;
    int ret = pthread_create(&nThreadID,NULL,CConnService::RecvProc,this);
    return ret == 0 ;
}

void* CConnService::RecvProc(void* pParam)
{
    CConnService* pService = (CConnService*)(pParam);
    if(! pService) return nullptr ;
    std::string key = pService ->GetConnSvrdMsgRoute();
    printf("CConnService::RecvProc key : %s \n",key.c_str());
    while (true)
    {
        if(pService ->SvrdNeedClose()) break ;
        //消息队列
        std::string strBuffer;
        int nResult = pService ->mqr() ->list_blpop(key, strBuffer, 1);
        if(IsNotZero(nResult))
        {
            continue ;
        }
        
//        int nResult = pService ->mqr() ->list_lpop(key, strBuffer);
//        if(nResult != 0)
//        {
//            usleep(1000);
//            continue ;
//        }
        pService ->AddEventQueueMsg(strBuffer);
    }
    return nullptr ;
}

bool CConnService::InitRedis()
{
    m_pRedis = cache();
//	char *szIP = g_oLuaIni.GetString(REDIS_IP);
//    mcgWriteLog("CConnService::InitRedis ip : %s" , szIP);
//	if (NULL == szIP)
//	{
//		return false;
//	}
//    std::string strRedisIp = szIP;
//
//	int dwRedisPort = g_oLuaIni.GetInt(REDIS_PORT);
//    mcgWriteLog("CConnService::InitRedis  dwRedisPort: %d" , dwRedisPort);
//    char* szPwd = g_oLuaIni.GetString(REDIS_PWD);
//    mcgWriteLog("CConnService::InitRedis pwd : %s" , szPwd);
//    if (NULL == szPwd)
//    {
//		return false;
//    }
//    std::string strRedisPwd = szPwd;
//
//	m_pRedis = new CRedisIntf("cache");
//	if (NULL == m_pRedis)
//	{
//		mcgWriteLog("new CRedisIntf failed");
//		return false;
//	}
//    if(! m_pRedis ->init(strRedisIp.c_str(), dwRedisPort, strRedisPwd.c_str() , 0))
//    {
//        mcgWriteLog("Error CGameService::InitRedis, init redis failed");
//        return false ;
//    }
	return true;   
}

bool CConnService::OnTimeOut(DWORD dwEventID)
{
	if(CService::OnTimeOut(dwEventID)) return true;
	switch(dwEventID)
	{
	default:
		break;
	}
	return true;
}

void CConnService::OnExitProcess(UINT uExitCode)
{
	CService::OnExitProcess(uExitCode);
}

void CConnService::CheckForDuplicateLogins(int user_id)
{
    //如果重复登录，就先给原来的客户端发送一条退出消息，然后移除player
    auto p_cplayer = m_pPlayerMgr -> GetPlayer(user_id);
    if(p_cplayer !=nullptr)
    {
        //该账号登录过
        //给这个账号发送一条退出的消息
        
        MsgPackage msg;
        
        //发给room一个消息
        printf("发给客户端退出命令\n");
        p_cplayer -> Disconnect(true);
        
        msg.set_msg_id((MSGID_LOGIN_DIFFERENT | ID_REQ));
        
        SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), p_cplayer->m_mClientID);
        
        
        auto it = m_mapClientList.find(p_cplayer->m_mClientID);
        ((CConnClient*)(it->second))->SetPlayer(nullptr) ;
        ((CConnClient*)(it->second))->SetLoginStatus(false);
        m_pPlayerMgr -> RemovePlayer(user_id);
    }
}

bool CConnService::OnReceiveQueueMsg(BYTE* pbyBuffer, int nBufferLen)
{
    mcgWriteLog("OnReceiveQueueMsg recv one msg");
    MsgPackage msg ;
    if(! msg.ParseFromArray(pbyBuffer, nBufferLen))
    {
        mcgWriteLog("CConnService::OnReceiveQueueMsg parse failed ");
        return false ;
    }
    
    if(msg.msg_id() == (MSGID_EDIT_NAME | ID_ACK))
    {
        printf("收到修改名字成功的ack，转发给客户端\n");
        SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
        return true;
    }
    
    
    if(msg.msg_id() == (MSGID_BUY | ID_ACK))
    {
        printf("收到购买商品的ack，转发给客户端\n");
        SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
        return true;
    }
    
    
    if(msg.msg_id() == (MSGID_REFUSE_INVITE_FRIEND_GAME | ID_ACK))
    {
        printf("收到拒绝邀请的ack，转发给客户端\n");
        SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
        return true;
    }
    
    

    if(msg.msg_id() == (MSGID_ASK_INVITE_FRIEND_GAME | ID_REQ))
    {
        printf("收到room发来的询问是否继续游戏的请求，转发给客户端\n");
        REQAskInviteFriendGame req;
        req.ParsePartialFromString(msg.msg());
        CPlayer*  p_player = m_pPlayerMgr -> GetPlayer(req.user_id());
        if(p_player != nullptr)
        {
            int clientid= p_player -> m_mClientID;
            SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(),clientid);
        }
        else
        {
            printf("找不到player，转发失败\n");
            //给玩家A发送一个掉线信息
            ACKRefuseInviteFriendGame req1;
            req1.set_code(1);
            req1.set_result_msg("对手掉线，无法游戏");
            //发给A
            CPlayer*  p_player2 = m_pPlayerMgr -> GetPlayer(req.friend_info().id());
            if(p_player2 != nullptr)
            {
                printf("发给客户端对手掉线，无法继续游戏的消息\n");
                int clientid2= p_player2 -> m_mClientID;
                SendClientMsg(0, (MSGID_INVITE_FRIEND_GAME | ID_ACK), req1.SerializeAsString(), msg.seq_id(),clientid2);
            }
            
        }
        return true;
    }
    if(msg.msg_id() == (MSGID_INVITE_FRIEND_GAME | ID_ACK))
    {
        printf("收到room邀请好友的ack，转发给客户端\n");
        SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
        return true;
    }
    
    
    
    if(msg.msg_id() == (MSGID_IS_AGREE_ADD_FRIEND | ID_ACK))
    {
        printf("收到auth发来的同意或拒绝添加好友的ack，转发给客户端\n");
        SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
        return true;
    }
    
    if(msg.msg_id() == (MSGID_REFUSE_GAME | ID_ACK))
    {
        printf("收到拒绝游戏的请求，转发给客户端");
        SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
    }
    
    if(msg.msg_id() == (MSGID_ASK_CONTINUE_GAME | ID_REQ))
    {
        printf("收到game发来的询问对方是否同意的消息，转发给客户端\n");
        REQAskContinueGame req;
        req.ParsePartialFromString(msg.msg());
        CPlayer*  p_player = m_pPlayerMgr -> GetPlayer(req.user_id());
        if(p_player != nullptr)
        {
            int clientid= p_player -> m_mClientID;
            SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(),clientid);
        }
        else
        {
            printf("找不到player，转发失败\n");
            //给玩家A发送一个掉线信息
            ACKRefuseContinueGame req1;
            req1.set_code(1);
            req1.set_result_msg("对手掉线，无法继续游戏");
            //发给A
            CPlayer*  p_player2 = m_pPlayerMgr -> GetPlayer(req.enemy_user_id());
            if(p_player2 != nullptr)
            {
                printf("发给客户端对手掉线，无法继续游戏的消息\n");
                int clientid2= p_player2 -> m_mClientID;
                SendClientMsg(0, (MSGID_REFUSE_GAME | ID_ACK), req1.SerializeAsString(), msg.seq_id(),clientid2);
            }
            
        }
        return true;
    }
    
    
    
    if(msg.msg_id() == (MSGID_CONTINUE_GAME | ID_ACK))
    {
        printf("收到game返回的继续游戏的ack消息，转发给客户端\n");
        SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
        return true;
    }
    
    //删除好友
    if(msg.msg_id() == (MSGID_DELETE_FRIEND | ID_ACK))
    {
        printf("收到auth返回的删除好友的ack，转发给客户端\n");
        SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
        return true;
    }
    
    
    //获取wait好友列表
    if(msg.msg_id() == (MSGID_GET_WAIT_FRIEND_LIST | ID_ACK))
    {
        printf("收到auth返回的获取wait好友列表，转发给客户端\n");
        SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
        return true;
    }
    
    
    
    //获取好友列表
    if(msg.msg_id() == (MSGID_GET_FRIENDS_LIST | ID_ACK))
    {
        printf("收到auth返回的获取好友列表，转发给客户端\n");
        //判断好友的在线状态
        ACKFriendsList ack;
        ack.ParsePartialFromString(msg.msg());
        for (int i = 0; i < ack.firendinfo_size(); ++i)
        {
            UserInfo* userInfo = ack.mutable_firendinfo(i);
            
            int id = userInfo-> id();
            if(m_pPlayerMgr -> GetPlayer(id) == nullptr)
            {
                userInfo ->set_is_online(false);
            }
            else
            {
                userInfo ->set_is_online(true);
            }
        }
        SendClientMsg(0, msg.msg_id(), ack.SerializeAsString(), msg.seq_id(), msg.clientid());
        return true;
    }
    
    
    //添加好友的ack
    if(msg.msg_id() == (MSGID_ADD_FRIEND | ID_ACK))
    {
        printf("收到auth返回的添加好友信息，转发给客户端\n");
        SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
        return true;
    }
    
    //告知添加好友的响应
    if(msg.msg_id() == (MSGID_REQ_FRIEND | ID_REQ))
    {
        printf("收到auth返回的提醒正在添加好友的人的数量，转发给客户端\n");
        REQRequestFriend req;
        req.ParsePartialFromString(msg.msg());
        CPlayer*  p_player = m_pPlayerMgr -> GetPlayer(req.user_id());
        if(p_player != nullptr)
        {
            int clientid= p_player -> m_mClientID;
            SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(),clientid);
        }
        else
        {
            printf("找不到player，转发失败\n");
        }
        return true;
    }
    
//    if(msg.msg_id() == (MSGID_UPDATE_FRIEND | ID_ACK))
//    {
//        printf("收到auth更新好友的请求，转发给客户端\n");
//        ACKUpdateFriend ack;
//        ack.ParsePartialFromString(msg.msg());
//        CPlayer*  p_player = m_pPlayerMgr -> GetPlayer(ack.user_id());
//        if(p_player != nullptr)
//        {
//            int clientid= p_player -> m_mClientID;
//            SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(),clientid);
//        }
//        else
//        {
//            printf("找不到player，转发失败\n");
//        }
//        return true;
//    }
    
    
    
    //获取用户信息
    if(msg.msg_id() == (MSGID_GET_FRIEND_INFO | ID_ACK))
    {
        printf("收到auth返回的好友信息，转发给客户端\n");
        SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
        return true;
    }
    
    
    
    
    //聊天
    if(msg.msg_id() == (MSGID_CHAT | ID_ACK))
    {
        printf("收到game返回的聊天消息，转发给客户端\n");
        SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
        return true;
    }
    
    
    //获取历史记录
    if(msg.msg_id() == (MSGID_GET_HISTORY | ID_ACK))
    {
        printf("收到auth返回来历史记录的消息，转发给客户端\n");
        SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
        return true;
    }
    
    //获取用户信息
    if(msg.msg_id() == (MSGID_GET_USER_INFO | ID_ACK))
    {
        printf("收到auth返回来的消息，转发给客户端\n");
        SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
        return true;
    }
    
    
    //注册消息
    if(msg.msg_id() == (MSGID_REGISTER | ID_ACK))
    {
        SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
        return true;
    }
    //登录消息
    if(msg.msg_id() == (MSGID_LOGIN | ID_ACK))
    {
        printf("收到auth返回来的登录消息，转发给客户端\n");
        //登录成功
        ACKLogin ack;
        ack.ParsePartialFromString(msg.msg());
        if(ack.code() == login_result_success)
        {
            //判断是否重复登录
            CheckForDuplicateLogins(ack.userinfo().id());
    
            CPlayer* cplayer = m_pPlayerMgr->CreatePlayer(ack.userinfo().id(), nullptr, nullptr);
            cplayer -> m_mClientID = msg.clientid();
            auto it = m_mapClientList.find((unsigned)msg.clientid());
            ((CConnClient*)(it->second))->SetPlayer(cplayer) ;
            ((CConnClient*)(it->second))->SetLoginStatus(true);
            cplayer->m_pClient = (CConnClient*)it->second;
            printf("debug:::设置client的player,client_id:%d,user_id:%d\n",msg.clientid(),ack.userinfo().id());

            SendClientMsg(msg.user_id(), msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
        }
        else{
            SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
        }
        return true;

    }
    //匹配消息
    if(msg.msg_id() == (MSGID_MATCH | ID_ACK))
    {
        printf("收到room返回来的消息，转发给客户端\n");
        SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
        return true;
    }
    
    //编辑完成
    if(msg.msg_id() == (MSGID_EDIT_HEAD | ID_ACK))
    {
        printf("头像已更改，转发给客户端\n");
        SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
        return true;
    }
    
    //获取排行榜
    if(msg.msg_id() == (RANDING_LIST | ID_ACK))
    {
        printf("收到auth发来的排行榜数据，转发给客户端\n");
        SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
        return true;
    }
    
    //table创建成功
    if(msg.msg_id() == (MSGID_CREATE_GAME | ID_ACK))
    {
        printf("收到game发来的创建table成功的消息，转发给客户端\n");
        SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
        return true;
    }
    
    //用户下棋
    if(msg.msg_id() == (MSGID_ACTION | ID_ACK))
    {
        printf("收到game发来的下棋的消息，转发给客户端\n");
        SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
        return true;
    }
    
    //用户退出消息
    if(msg.msg_id() == (MSGID_EXIT_GAME | ID_ACK))
    {
        printf("收到game发来的退出消息，转发给客户端\n");
        SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
        return true;
    }
    
    //询问用户是否同意悔棋
    if(msg.msg_id() == (MSGID_ASK_UNDO_CHESS | ID_REQ))
    {
        printf("收到game发来的询问是否悔棋的消息，转发给客户端\n");
        SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
        return true;
    }
    
    
    return true ;
}

bool CConnService::OnMsg(DWORD dwMsgID, DWORD dwSeqID)
{
	if (CService::OnMsg(dwMsgID, dwSeqID)) return true;
	
	switch(dwMsgID)
	{
        default:
			break;
	}

	mcgWriteLog("recieve message:0x%08X, seqid:%u in CUserHandInfoService::OnMsg()", dwMsgID, dwSeqID);
	
	
	return true;
}

void CConnService::OnSvrdClose()
{
    m_bSvrdClose = true ;
}

bool CConnService::SvrdNeedClose()
{
    return m_bSvrdClose ;
}

//初始化消息队列缓存
bool CConnService::InitMQRedis()
{
    REDIS_CONFIG _cache_conf = getCacheConf();
    
    mcgWriteLog("Info CGameService::InitMQRedis, IP:(%s), port:%u", _cache_conf.ip.c_str(), _cache_conf.port);
    return true ;
}

//获取该连接服务器的路由消息数据
std::string CConnService::GetConnSvrdMsgRoute()
{
    return "WZQ_MSG_QUEUE_CONNSVRD_" + getServiceID();
}

//获取鉴权服务路由信息
std::string CConnService::GetAuthSvrdMsgRoute()
{
    return "WZQ_MSG_QUEUE_AUTHSVRD_wzqauth_01";
}

//获取room服务路由信息
std::string CConnService::GetRoomSvrdMsgRoute()
{
    return "WZQ_MSG_QUEUE_ROOMSVRD_roomsvrd";
}

//获取room服务路由信息
std::string CConnService::GetGameSvrdMsgRoute()
{
    return "WZQ_MSG_QUEUE_GAMESVRD_gamesvrd";
}


int CConnService::SendSvrdMsg(const std::string& msgroute , int64 msg_id , int64 seq_id , int64 user_id , const std::string& msg,int64 nClientID)
{
    if(msgroute != "")
    {
        MsgPackage package ;
        package.set_msg_id(msg_id);
        package.set_seq_id(seq_id);
        package.set_user_id(user_id);
        package.set_msg(msg);
        package.set_src(GetConnSvrdMsgRoute());
        package.set_clientid(nClientID);
        mqw() ->list_rpush(msgroute, package.SerializeAsString());
    }
    return 0 ;
}

int CConnService::SendClientMsg(int64 userid , DWORD dwMsgID, const std::string& strBuffer, DWORD dwSeqID , int64 nClientID)
{
    if(userid <= 0)
    {
        //表示玩家未登录状态
        auto it = m_mapClientList.find((unsigned)nClientID);
        if(it != m_mapClientList.end())
        {
            ((CMCGClient*)it->second)->SendMsg(dwMsgID, strBuffer, dwSeqID);
        }
        return 0;
    }
    
    auto player =  m_pPlayerMgr->GetPlayer(userid);
    if(player)
    {
        mcgWriteLog("CMsgRouter::SendClientMsg userid : %lld , clientID : %lld" , userid , nClientID);
        player ->SendMsg(dwMsgID, strBuffer, dwSeqID);
    }
    return 0 ;
}
