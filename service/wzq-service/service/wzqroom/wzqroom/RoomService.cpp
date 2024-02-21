#include "RoomService.h"
#include "log.h"
#include "mcgevent.h"
#include "mcgcommon/datatype.h"
#include <boost/regex.hpp>
#include <json/json.h>
#include <random>
#include <iostream>

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

CRoomService::CRoomService()
{
    m_bSvrdClose = false;
    m_matcher = new Matcher(this);
    m_inviteFriendGame = new InviteFriendGame(this);
}

CRoomService::~CRoomService()
{
}

IEventPump *CRoomService::mcgclient(DWORD dwIP,WORD wPort)
{
    return nullptr;
}


bool CRoomService::OnInit()
{
	if (!CService::OnInit()) return false;
    printf("debug,当前服务器的路由地址:%s\n",GetRoomSvrdMsgRoute().c_str());
	if (!InitRedis())
	{
		mcgWriteLog("初始化 redis 失败了");
		return false;
	}
    //初始化消息队列缓存
    if(! InitMQRedis())
    {
        mcgWriteLog("CRoomService::OnInit func>>InitMQRedis failed");
        return false;
    }
    //初始化读取消息队列的线程
    InitRecvMsgQueueThread();
	return true;
}

bool CRoomService::InitRecvMsgQueueThread()
{
    pthread_t nThreadID ;
    int ret = pthread_create(&nThreadID,NULL,CRoomService::RecvProc,this);
    return ret == 0 ;
}

void* CRoomService::RecvProc(void* pParam)
{
    CRoomService* pService = (CRoomService*)(pParam);
    if(! pService) return nullptr ;
    std::string key = pService ->GetRoomSvrdMsgRoute();
    while (! pService ->SvrdNeedClose())
    {
        //消息队列
        std::string strBuffer;
        int nResult = pService ->mqr() ->list_blpop(key, strBuffer , 1);
        if(nResult != 0)
        {
            continue ;
        }
        mcgWriteLog("CRoomService::RecvProc recv one msg");
        pService ->AddEventQueueMsg(strBuffer);
    }
    return nullptr ;
}

bool CRoomService::InitRedis()
{
    m_pRedis = cache();
//	char *szIP = g_oLuaIni.GetString(REDIS_IP);
//    mcgWriteLog("CRoomService::InitRedis ip : %s" , szIP);
//	if (NULL == szIP)
//	{
//		return false;
//	}
//    std::string strRedisIp = szIP;
//
//	int dwRedisPort = g_oLuaIni.GetInt(REDIS_PORT);
//    mcgWriteLog("CRoomService::InitRedis  dwRedisPort: %d" , dwRedisPort);
//    char* szPwd = g_oLuaIni.GetString(REDIS_PWD);
//    mcgWriteLog("CRoomService::InitRedis pwd : %s" , szPwd);
//    if (NULL == szPwd)
//    {
//		return false;
//    }
//    std::string strRedisPwd = szPwd;
//
//	m_pRedis = new CRedisIntf();
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

bool CRoomService::OnTimeOut(DWORD dwEventID)
{
	if(CService::OnTimeOut(dwEventID)) return true;
	switch(dwEventID)
	{
	default:
		break;
	}
	return true;
}

void CRoomService::OnExitProcess(UINT uExitCode)
{
	CService::OnExitProcess(uExitCode);
}




bool CRoomService::OnReceiveQueueMsg(BYTE* pbyBuffer, int nBufferLen)
{
    mcgWriteLog("OnReceiveQueueMsg recv one msg");
    MsgPackage msg ;
    if(! msg.ParseFromArray(pbyBuffer, nBufferLen))
    {
        mcgWriteLog("CRoomService::OnReceiveQueueMsg parse failed ");
        return false ;
    }
    
    
    //处理匹配的消息
    if(msg.msg_id() == (MSGID_MATCH | ID_REQ))
    {
        std::cout<<"debug-----------:room收到一条匹配消息"<<std::endl;
        REQRoom req ;
        req.ParsePartialFromString(msg.msg());
        mcgWriteLog("room收到的请求： %s",req.req_msg().c_str());
        std::cout<<"请求的用户信息:"<<req.userinfo().username()<<" "<<req.userinfo().score()<<std::endl;
        //收到用户的匹配消息
        if(req.req_msg()=="req_match_start")
        {
            //开始匹配
            PlayerInfo playerInfo;
            playerInfo.m_userid=req.userinfo().id();
            playerInfo.m_userinfo=req.userinfo();
            playerInfo.m_clientid=msg.clientid();
            m_matcher->AddUser(playerInfo.m_userid,playerInfo);
        }
        else if(req.req_msg()=="req_match_stop")
        {
            // 取消匹配
            m_matcher->DelUser(req.userinfo().id());
        }

    }
    if(msg.msg_id() == (MSGID_FORCE_EXIT | ID_REQ))
    {
        std::cout<<"debug-----------:room收到一条用户强制退出的消息"<<std::endl;
        REQForcExit req ;
        req.ParsePartialFromString(msg.msg());
        int id = req.user_id();
        m_matcher->DelUser(id);
    }
    
    if(msg.msg_id() == (MSGID_INVITE_FRIEND_GAME | ID_REQ))
    {
        std::cout<<"debug-----------:room收到一条邀请好友游戏的消息"<<std::endl;
        REQInviteFriendGame req ;
        req.ParsePartialFromString(msg.msg());
        
        UserInfo userinfo = req.userinfo();
        int friend_id = req.friend_id();
        PlayerInfo playerinfo;
        playerinfo.m_clientid = msg.clientid();
        playerinfo.m_userinfo=userinfo;
        playerinfo.m_userid=userinfo.id();
        m_inviteFriendGame->InviteGame(userinfo.id(), playerinfo, friend_id);
       
    }
    if(msg.msg_id() == (MSGID_ASK_INVITE_FRIEND_GAME | ID_ACK))
    {
        std::cout<<"debug-----------:room同意或拒绝邀请游戏的消息"<<std::endl;
        ACKAskInviteFriendGame ack ;
        ack.ParsePartialFromString(msg.msg());
        UserInfo userinfo = ack.userinfo();
        PlayerInfo playerinfo;
        playerinfo.m_clientid = msg.clientid();
        playerinfo.m_userinfo=userinfo;
        playerinfo.m_userid=userinfo.id();
        bool is_agree = ack.is_agree_invite_game();
        int friend_id = ack.friend_id();
        std::string msg = ack.result_msg();
        m_inviteFriendGame -> IsAgreeInviteGame(userinfo.id(),playerinfo,friend_id,is_agree,msg);
        
    }
    
    
    
//    if(msg.msg_id() == (MSGID_CONTINUE_GAME | ID_REQ))
//    {
//        std::cout<<"debug-----------:room收到一条用户要与对手继续下棋的消息"<<std::endl;
//        REQContinueGame req ;
//        req.ParsePartialFromString(msg.msg());
//        int user_id = req.userinfo().id();
//        int enemy_id = req.enemy_user_id();
//        
//        int id1 = std::min(user_id,enemy_id);
//        int id2 = std::max(user_id,enemy_id);
//        std::unique_lock<std::mutex> lock(m_mutex);
//        if(m_umContinueGame.find(id1)!=m_umContinueGame.end()&&m_umContinueGame[id1] == id2)
//        {
//            //两个人都发了继续匹配的请求，这种情况的概率比较小
//            if(m_umPlayerInfo.find(user_id)!=m_umPlayerInfo.end())
//            {
//                //说明user_id继续游戏的消息发了多次，这次不做处理
//                return false;
//            }
//            else if(m_umPlayerInfo.find(user_id)==m_umPlayerInfo.end()&&m_umPlayerInfo.find(enemy_id)!=m_umPlayerInfo.end())
//            {
//                //说明user_id和enemy_id都发了继续游戏的请求，直接给他们创建房间
//                PlayerInfo playerInfo =m_umPlayerInfo[user_id];
//                m_umPlayerInfo.erase(user_id);
//                
//                PlayerInfo playerInfoEnemy;
//                playerInfoEnemy.m_clientid = msg.clientid();
//                playerInfoEnemy.m_userid = req.userinfo().id();
//                playerInfoEnemy.m_userinfo = req.userinfo();
//                
//                //为这两个人创建房间
//                //随机选择一个人成为黑棋
//                int black_id = 0;
//                int white_id = 0;
//                if(std::rand()%2 ==0)
//                {
//                    black_id = playerInfo.m_userid;
//                    white_id = playerInfoEnemy.m_userid;
//                }
//                else
//                {
//                    black_id = playerInfoEnemy.m_userid;
//                    white_id = playerInfo.m_userid;
//                }
//                
//                m_umContinueGame.erase(playerInfo.m_userid);
//                m_umContinueGame.erase(playerInfoEnemy.m_userid);
//                m_umPlayerInfo.erase(playerInfo.m_userid);
//                m_umPlayerInfo.erase(playerInfoEnemy.m_userid);
//                
//                
//                SendMatchMessage(playerInfo,playerInfoEnemy,black_id,white_id);
//                SendMatchMessage(playerInfoEnemy,playerInfo,black_id,white_id);
//                
//                //需要向game服务器发送消息创建房间
//                SendCreateGameMessage(playerInfo,playerInfoEnemy,black_id,white_id);
//            }
//        }
//        else
//        {
//            m_umContinueGame[id1] = id2;
//            PlayerInfo playerInfo;
//            playerInfo.m_userid=req.userinfo().id();
//            playerInfo.m_userinfo=req.userinfo();
//            playerInfo.m_clientid=msg.clientid();
//            m_umPlayerInfo[user_id] = playerInfo;
//
//            
//            //询问对手是否同意继续游戏
//            REQAskContinueGame req_ask;
//            req_ask.mutable_enemyinfo()->CopyFrom(req.userinfo());
//            req_ask.set_user_id(enemy_id);
//            SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_ASK_CONTINUE_GAME | ID_REQ), 0, 0, req_ask.SerializeAsString(), 0);
//            
//        }
//    }
//    if(msg.msg_id() == (MSGID_ASK_CONTINUE_GAME | ID_ACK))
//    {
//        std::cout<<"debug-----------:room收到一条用户同意或拒绝继续下棋的消息"<<std::endl;
//        ACKAskContinueGame ack ;
//        ack.ParsePartialFromString(msg.msg());
//        int enemy_user_id = ack.enemy_user_id();
//        if(m_umPlayerInfo.find(enemy_user_id)==m_umPlayerInfo.end())
//        {
//            printf("找不到enemy_user的信息，无法创建game %d\n",enemy_user_id);
//            return false;
//        }
//        if(ack.is_agree_continue_game() == false)
//        {
//            SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_REFUSE_GAME | ID_REQ), 0, 0, "", m_umPlayerInfo[enemy_user_id].m_clientid);
//            m_umContinueGame.erase(enemy_user_id);
//            m_umContinueGame.erase(ack.userinfo().id());
//            m_umPlayerInfo.erase(enemy_user_id);
//            m_umPlayerInfo.erase(ack.userinfo().id());
//            return false;
//        }
//        
//        
//        PlayerInfo playerInfo =m_umPlayerInfo[enemy_user_id];
//        m_umPlayerInfo.erase(enemy_user_id);
//        
//        PlayerInfo playerInfoEnemy;
//        playerInfoEnemy.m_clientid = msg.clientid();
//        playerInfoEnemy.m_userid = ack.userinfo().id();
//        playerInfoEnemy.m_userinfo = ack.userinfo();
//        
//        //为这两个人创建房间
//        //随机选择一个人成为黑棋
//        int black_id = 0;
//        int white_id = 0;
//        if(std::rand()%2 ==0)
//        {
//            black_id = playerInfo.m_userid;
//            white_id = playerInfoEnemy.m_userid;
//        }
//        else
//        {
//            black_id = playerInfoEnemy.m_userid;
//            white_id = playerInfo.m_userid;
//        }
//        m_umContinueGame.erase(enemy_user_id);
//        m_umContinueGame.erase(ack.userinfo().id());
//        m_umPlayerInfo.erase(enemy_user_id);
//        m_umPlayerInfo.erase(ack.userinfo().id());
//        
//        SendMatchMessage(playerInfo,playerInfoEnemy,black_id,white_id);
//        SendMatchMessage(playerInfoEnemy,playerInfo,black_id,white_id);
//        
//        //需要向game服务器发送消息创建房间
//        SendCreateGameMessage(playerInfo,playerInfoEnemy,black_id,white_id);
//        
//    }
    
    
    
    return true ;
}

void CRoomService::OnSvrdClose()
{
    m_bSvrdClose = true ;
}

bool CRoomService::SvrdNeedClose()
{
    return m_bSvrdClose ;
}

//初始化消息队列缓存
bool CRoomService::InitMQRedis()
{
    //todo
    return true ;
}

//获取该连接服务器的路由消息数据
std::string CRoomService::GetRoomSvrdMsgRoute()
{
    return "WZQ_MSG_QUEUE_ROOMSVRD_roomsvrd";
}

int CRoomService::SendSvrdMsg(const std::string& msgroute , int64 msg_id , int64 seq_id , int64 user_id , const std::string& msg,int64 nClientID)
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

//获取连接服的路由信息
std::string CRoomService::GetConnSvrdMsgRoute()
{
    return "WZQ_MSG_QUEUE_CONNSVRD_conn101_01";
}

//获取game服务的路由地址
std::string CRoomService::GetGameSvrdMsgRoute()
{
    return "WZQ_MSG_QUEUE_GAMESVRD_gamesvrd";
}
