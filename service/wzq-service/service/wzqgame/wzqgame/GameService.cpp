#include "GameService.h"

#include "log.h"
#include "luaini.h"
#include "splitstring.h"
#include <sstream>
#include "globalfunc.h"
#include "mcgevent.h"
#include "TableMgr.h"
#include "PlayerMgr.h"
#include "Table.h"
#include "Player.h"
#include "WZQGame.h"
#include "WZQChat.hpp"
#include "ProtocolDef.h"
#include <boost/regex.hpp>
#include <json/json.h>

extern CLuaIni g_oLuaIni;

#define CONTEXT_SEP				 " "							//
#define APP_SERVICE_NAME		 "AppServiceName"				//
#define APP_LOG_PATH			 "AppLogPath"					//
#define GAMEINFO_SERVICE_IP		 "GameInfoSvrdIP"				//
#define GAMEINFO_SERVICE_PORT	 "GameInfoSvrdPort"				//
#define AI_SERVICE_COUNT		 "AiSvrdCount"					//
#define AI_SERVICE_IP			 "AiSvrdIP"						//
#define AI_SERVICE_PORT			 "AiSvrdPort"					//
#define CLEAR_DATA				 "ClearData"					//
#define REDIS_IP				 "RedisIP"						//
#define REDIS_PORT				 "RedisPort"					//
#define REDIS_PASSWD			 "RedisPasswd"					//
#define CLEAR_SHAREDATA			 "ClearShareData"					// 是否清除缓存数据（0[]=不是,1[]=是）
#define SHAREREDIS_IP			 "ShareRedisIP"						// 缓存服务IP
#define SHAREREDIS_PORT			 "ShareRedisPort"					// 缓存服务端口号
#define SHAREREDIS_PASSWD		 "ShareRedisPasswd"					// 缓存服务器密码

#define TIMER_SCAN_REQSIT         (TIMER_GAMESERVER+2)
#define TIMER_KEEPALIVE           (TIMER_GAMESERVER+3)
CGameService::CGameService()
{
	m_bInitOk = false;

	m_pPlayerMgr = new CPlayerMgr();
    m_pPlayerMgr->m_pService = this;
	m_pTableMgr = new CTableMgr();
	m_pRedis = NULL;
	m_pLogicLog =  NULL;
    m_bSvrdClose = false ;
    
    m_pThreadPool = new Threadpool(5);
}

CGameService::~CGameService()
{
	if (IsNotNULL(m_hTrySendTimer))
	{
		CMcgTimer::DeleteTimer(m_hTrySendTimer, true);
	}

	m_bInitOk = false;
}
void* CGameService::printMessage(void* ptr,unsigned int a) {
    // 打印消息的实际实现
    // 可以根据需要进行调整
    std::cout << "debug-----------------回调触发!"<<a << std::endl;
    
    return nullptr;
}

bool CGameService::OnInit()
{
	if (!CService::OnInit()) return false;
	m_pTableMgr = new CTableMgr();
	m_pPlayerMgr = new CPlayerMgr();

	if (!InitAppLog())
	{
		mcgWriteLog("Error CGameService::OnInit, call InitAppLog failed");
		return false;
	}

    if(!InitSvrdID())
    {
        mcgWriteLog("Error CGameService::OnInit, call  GetSvrdID failed");
        return false ;
    }
	//
	if (!InitRedis())
	{
		mcgWriteLog("Error CGameService::OnInit, call InitRedis failed");
		return false;
	}
    
    //初始化消息队列缓存
    if(! InitMQRedis())
    {
        mcgWriteLog("OnInit func>>InitMQRedis failed");
        return false;
    }

	//
	//m_hTrySendTimer = CMcgTimer::CreateTimer(CGameService::printMessage, this, 10000, 1*1000, 1);
    m_hTrySendTimer = CMcgTimer::CreateTimer(nullptr, this, 0, 1*1000, TIMER_GAMESERVER+1);
	if (IsNULL(m_hTrySendTimer))
	{
		mcgWriteLog("Error CGameService::OnInit, call CreateTimer failed");
		return false;
	}
    //初始化读取消息队列的线程
    InitRecvMsgQueueThread();
	m_bInitOk = true;
	printf("Init service success\n");
	return true;
}

bool CGameService::OnTimeOut(DWORD dwEventID)
{
	if(CService::OnTimeOut(dwEventID)) return true;
	return true;
}

void CGameService::UpLogLevel()
{
	if (IsNotNULL(m_pLogicLog))
	{
		m_pLogicLog->UpLevel();
	}
}

void CGameService::DownLogLevel()
{
	if (IsNotNULL(m_pLogicLog))
	{
		m_pLogicLog->DownLevel();
	}
}

void CGameService::OnSvrdClose()
{
    m_bSvrdClose = true ;
}

bool CGameService::SvrdNeedClose()
{
    return m_bSvrdClose ;
}

void CGameService::OnExitProcess(UINT uExitCode)
{
	printf("Error CGameService::OnExitProcess\n");
	CService::OnExitProcess(uExitCode);
}

std::shared_ptr<CPlayer> CGameService::GetPlayerByID(int64 user_id)
{
	return m_pPlayerMgr->GetPlayer(user_id);
}

std::shared_ptr<CTable> CGameService::GetTableByID(int nTableID)
{
	return m_pTableMgr->GetTable(nTableID);
}

int CGameService::GetTotalPlayer()
{
	return m_pPlayerMgr->Size();
}
int CGameService::GetTotalTable()
{
	return m_pTableMgr->Size();
}

bool CGameService::InitAppLog()
{
	//
	char *szRet = g_oLuaIni.GetString(APP_LOG_PATH);
//	char *szAppServiceName = g_oLuaIni.GetString(APP_SERVICE_NAME);
    char *szAppServiceName = const_cast<char *>(getServiceName().c_str());
	if (IsNULL(szRet))
	{
		m_strAppLogPath = "./";
	}
	else
	{
		m_strAppLogPath = szRet;
	}
	m_strAppServiceName = szAppServiceName;

	char szServiceName[MAX_PATH];
	GetAppName(szServiceName);

	m_pLogicLog = new CLog();
	if (IsNULL(m_pLogicLog) || !m_pLogicLog->Init(szAppServiceName,"./",LOG_LEVEL_INFO))
	{
		mcgWriteLog("CGameService::InitAppLog, init log failed");
		return false;
	}

	return true;
}

bool CGameService::InitRedis()
{
    m_pRedis = cache();
//	char *szIP = g_oLuaIni.GetString(REDIS_IP);
//	if (IsNULL(szIP))
//	{
//		return false;
//	}
//
//	m_strRedisSvrIp = szIP;
//	m_nRedisSvrPort = g_oLuaIni.GetInt(REDIS_PORT);
//
//	char *szPasswd = g_oLuaIni.GetString(REDIS_PASSWD);
//	if (IsNULL(szPasswd))
//	{
//		return false;
//	}
//	m_strRedisSvrPasswd = szPasswd;
//
//	m_pRedis = new CRedisIntf();
//	if (IsNULL(m_pRedis))
//	{
//		mcgWriteLog("Error CGameService::InitRedis, new redis instance failed");
//		return false;
//	}
//
//	if (!m_pRedis->init(szIP,m_nRedisSvrPort,szPasswd,0))
//	{
//		mcgWriteLog("Error CGameService::InitRedis, init redis failed");
//		return false;
//	}
//
	int iClear = g_oLuaIni.GetInt(CLEAR_DATA);
	if(iClear != 0)
	{
		// todo
	}
	mcgWriteLog("Info CGameService::InitRedis, IP:(%s), port:%d", getCacheConf().ip.c_str(), getCacheConf().port);
	return true;
}

//初始化消息队列缓存
bool CGameService::InitMQRedis()
{
    m_pMQRedis = mqw();

    REDIS_CONFIG _cache_conf = getCacheConf();
    mcgWriteLog("Info CGameService::InitMQRedis, IP:(%s), port:%u", _cache_conf.ip.c_str(), _cache_conf.port);
    return true ;
}

bool CGameService::InitSvrdID()
{
//    char* szSvrdID = g_oLuaIni.GetString("SvrdID");
//    if(IsNULL(szSvrdID))
//    {
//        return false ;
//    }
//    m_strSvrdID = szSvrdID ;
    m_strSvrdID = getServiceID();
    return true ;
}


bool CGameService::OnReceiveQueueMsg(BYTE* pbyBuffer, int nBufferLen)
{
    mcgWriteLog("CGameService::OnReceiveQueueMsg");
    MsgPackage msg ;
    if(! msg.ParseFromArray(pbyBuffer, nBufferLen))
    {
        mcgWriteLog("CGameService::OnReceiveQueueMsg parse failed ");
        return false ;
    }

    //处理创建房间的消息
    if(msg.msg_id() == (MSGID_CREATE_GAME | ID_REQ))
    {
        std::cout<<"debug-----------:game收到一条创建房间的信息"<<std::endl;
        REQCreatGame req ;
        req.ParsePartialFromString(msg.msg());
        mcgWriteLog("game收到的请求： %s",req.req_msg().c_str());


        std::shared_ptr<CTable> table =m_pTableMgr->CreateTable(m_pTableMgr->m_nextTableID);
        m_pTableMgr->m_nextTableID++;
        table -> CreateGame(req.black_id(),req.white_id());

        
   
        std::shared_ptr<CPlayer> player1 = m_pPlayerMgr->CreatePlayer(req.userinfo_player1().id(),req.player1_clientid());
        std::shared_ptr<CPlayer> player2 = m_pPlayerMgr->CreatePlayer(req.userinfo_player2().id(),req.player2_clientid());
        player1->m_nTableID = table->m_nTableID;
        player2->m_nTableID = table->m_nTableID;
        
        
        //创建table成功，给两个player发送table创建成功的ack
        ACKCreatGame ack;
        ack.set_result_msg("create_game_sucess");
        ack.set_code(0);
        ack.set_room_id(table->m_nTableID);
        ack.set_black_id(req.black_id());
        ack.set_white_id(req.white_id());
        ack.set_score(100);
        ack.set_coins(200);
        
        SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_CREATE_GAME | ID_ACK), 0, 0, ack.SerializeAsString(), player1->m_nClientID);
        SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_CREATE_GAME | ID_ACK), 0, 0, ack.SerializeAsString(), player2->m_nClientID);
        printf("发给两个用户房间信息:%d\n",table->m_nTableID);
    }

    
    
    
    if(msg.msg_id() == (MSGID_ACTION | ID_REQ))
    {
        std::cout<<"debug-----------:game收到一条用户下棋或悔棋请求的信息"<<std::endl;
        REQAction req ;
        req.ParsePartialFromString(msg.msg());
        mcgWriteLog("game收到的请求： %s",req.req_msg().c_str());
        if(req.req_msg()=="put_chess")
        {
            int table_id = req.table_id();
            if (m_pTableMgr->m_tablesMap.find(table_id) == m_pTableMgr->m_tablesMap.end())
            {
                mcgWriteLog("找不到table_id:%d",table_id);
                return false;
            }
            std::shared_ptr<CTable> table = m_pTableMgr -> m_tablesMap[table_id];
            //table -> m_pGame ->PutChess(req.action_id(),req.action_board_row(),req.action_board_col());

            m_pThreadPool->enqueue([table,req]() {
                table -> m_pGame ->PutChess(req.action_id(),req.action_board_row(),req.action_board_col());
                
            });
            
        }
        else if(req.req_msg()=="undo_chess")
        {
            int table_id = req.table_id();
            if (m_pTableMgr->m_tablesMap.find(table_id) == m_pTableMgr->m_tablesMap.end())
            {
                mcgWriteLog("找不到table_id:%d",table_id);
                return false;
            }
            auto table = m_pTableMgr -> m_tablesMap[table_id];
            table->m_pGame->m_remain_time = req.remain_time();
            printf("悔棋剩余时间:%d\n",table->m_pGame->m_remain_time);
            //table -> m_pGame ->UndoPlayerAsk(req.action_id());
            
            m_pThreadPool->enqueue([table,req]() {
                table -> m_pGame ->UndoPlayerAsk(req.action_id());
                
            });
        }
    }
    
    if(msg.msg_id() == (MSGID_EXIT_GAME | ID_REQ))
    {
        std::cout<<"debug-----------:game收到一条用户退出的请求信息"<<std::endl;
        REQExitGame req ;
        req.ParsePartialFromString(msg.msg());
        mcgWriteLog("game收到的请求： %s",req.req_msg().c_str());
        int table_id = req.exit_table_id();
        if (m_pTableMgr->m_tablesMap.find(table_id) == m_pTableMgr->m_tablesMap.end())
        {
            mcgWriteLog("找不到table_id:%d",table_id);
            return false;
        }
        auto table = m_pTableMgr -> m_tablesMap[table_id];
        int exit_player_id = req.exit_user_id();
        if (m_pPlayerMgr->m_playersMap.find(exit_player_id) == m_pPlayerMgr->m_playersMap.end())
        {
            mcgWriteLog("找不到exit_player_id:%d",exit_player_id);
            return false;
        }
        
        //玩家游戏结束后，退出游戏
        if(table -> m_statu == GAME_OVER)
        {
            table -> m_playerCount --;
            m_pPlayerMgr->RemovePlayer(exit_player_id);
            if(table -> m_playerCount == 0)
            {
                //table
                printf("移除table\n");
                m_pTableMgr->RemoveTable(table ->m_nTableID);
                printf("移除table完成\n");
                
            }
        }
        else
        {
            
            table -> m_playerCount --;
            m_pThreadPool->enqueue([this,table,exit_player_id]() {
                table -> m_pGame ->PlayerForceExitGame(exit_player_id);
                //该table中玩家数量为0，需要销毁该table
                if(table -> m_playerCount == 0)
                {
                    //table
                    printf("移除table\n");
                    m_pTableMgr->RemoveTable(table ->m_nTableID);
                    printf("移除table完成\n");
                    
                }
            });
        }
    }

    if(msg.msg_id() == (MSGID_FORCE_EXIT | ID_REQ))
    {
        std::cout<<"debug-----------:game收到一条用户强制退出的请求信息"<<std::endl;
        REQForcExit req ;
        req.ParsePartialFromString(msg.msg());
        int exit_player_id = req.user_id();
        if(m_pPlayerMgr -> m_playersMap.find(exit_player_id) == m_pPlayerMgr -> m_playersMap.end())
        {
            mcgWriteLog("找不到exit_player_id:%d",exit_player_id);
            return false;
        }
        auto& player = m_pPlayerMgr -> m_playersMap[exit_player_id];
        int table_id = player -> m_nTableID;
        if (m_pTableMgr->m_tablesMap.find(table_id) == m_pTableMgr->m_tablesMap.end())
        {
            mcgWriteLog("找不到table_id:%d",table_id);
            return false;
        }
        auto table = m_pTableMgr -> m_tablesMap[table_id];
        
        
        //玩家游戏结束后，退出游戏
        if(table -> m_statu == GAME_OVER)
        {
            table -> m_playerCount --;
            m_pPlayerMgr->RemovePlayer(exit_player_id);
            if(table -> m_playerCount == 0)
            {
                //table
                printf("移除table\n");
                m_pTableMgr->RemoveTable(table ->m_nTableID);
                printf("移除table完成\n");
                
            }
        }
        else
        {
            
            table -> m_playerCount --;
            //table -> m_pGame ->PlayerForceExitGame(exit_player_id);
            m_pThreadPool->enqueue([this,table,exit_player_id]() {
                table -> m_pGame ->PlayerForceExitGame(exit_player_id);
                //该table中玩家数量为0，需要销毁该table
                if(table -> m_playerCount == 0)
                {
                    //table
                    printf("移除table\n");
                    m_pTableMgr->RemoveTable(table ->m_nTableID);
                    printf("移除table完成\n");
                    
                }
            });
        }
    }
    
    if(msg.msg_id() == (MSGID_ASK_UNDO_CHESS | ID_ACK))
    {
        std::cout<<"debug-----------:game收到悔棋的应答消息"<<std::endl;
        ACKAskUndoChess ack ;
        ack.ParsePartialFromString(msg.msg());

        int table_id = ack.table_id();
        if (m_pTableMgr->m_tablesMap.find(table_id) == m_pTableMgr->m_tablesMap.end())
        {
            mcgWriteLog("找不到table_id:%d",table_id);
            return false;
        }
        auto table = m_pTableMgr -> m_tablesMap[table_id];
        //table -> m_pGame ->UndoChess(ack.action_id(),ack.is_agree_undo_chess());
        m_pThreadPool->enqueue([table,ack]() {
            table -> m_pGame ->UndoChess(ack.action_id(),ack.is_agree_undo_chess());
            
        });
    }
    
    
    if(msg.msg_id() == (MSGID_CHAT | ID_REQ))
    {
        std::cout<<"debug-----------:game收到聊天的消息"<<std::endl;
        REQChat req ;
        req.ParsePartialFromString(msg.msg());
        

        int table_id = req.table_id();
        if (m_pTableMgr->m_tablesMap.find(table_id) == m_pTableMgr->m_tablesMap.end())
        {
            mcgWriteLog("找不到table_id:%d",table_id);
            return false;
        }
        int user_id = req.user_id();
        if(m_pPlayerMgr -> m_playersMap.find(user_id) == m_pPlayerMgr -> m_playersMap.end())
        {
            mcgWriteLog("找不到user_id:%d",user_id);
            return false;
        }
        int opt_id = req.opp_id();
        if(m_pPlayerMgr -> m_playersMap.find(opt_id) == m_pPlayerMgr -> m_playersMap.end())
        {
            mcgWriteLog("找不到opt_id:%d",opt_id);
            return false;
        }
        
        
        auto table = m_pTableMgr -> m_tablesMap[table_id];
        //table -> m_pChat -> Chat(req.text(), user_id, opt_id);
        m_pThreadPool->enqueue([table,req,user_id, opt_id]() {
            table -> m_pChat -> Chat(req.text(), user_id, opt_id);
            
        });
    }
    
    
    if(msg.msg_id() == (MSGID_CONTINUE_GAME | ID_REQ))
    {
        printf("收到一条继续游戏的请求\n");
        REQContinueGame req ;
        req.ParsePartialFromString(msg.msg());
        int user_id = req.user_id();
        int enemy_id = req.enemy_user_id();
        int table_id = req.table_id();
        if (m_pTableMgr->m_tablesMap.find(table_id) == m_pTableMgr->m_tablesMap.end())
        {
            mcgWriteLog("找不到table_id:%d",table_id);
            return false;
        }
        std::shared_ptr<CTable> table = m_pTableMgr -> m_tablesMap[table_id];
        
        m_pThreadPool->enqueue([user_id,enemy_id,table]() {
            table->ContinueGame(user_id, enemy_id);
            
        });
        
        return true;
    }
    if(msg.msg_id() == (MSGID_ASK_CONTINUE_GAME | ID_ACK))
    {
        ACKAskContinueGame ack ;
        ack.ParsePartialFromString(msg.msg());
        int user_id = ack.user_id();
        int enemy_id = ack.enemy_user_id();
        int table_id = ack.table_id();
        bool is_agree = ack.is_agree_continue_game();
        if (m_pTableMgr->m_tablesMap.find(table_id) == m_pTableMgr->m_tablesMap.end())
        {
            mcgWriteLog("找不到table_id:%d",table_id);
            return false;
        }
        auto table = m_pTableMgr -> m_tablesMap[table_id];
        m_pThreadPool->enqueue([user_id,enemy_id,table,is_agree]() {
            table->AckContinueGame(is_agree,user_id, enemy_id);
            
        });
      
    }
    
    return true;
}


void* CGameService::RecvProc(void* pParam)
{
    CGameService* pService = (CGameService*)(pParam);
    if(! pService) return nullptr ;
    
    while (true)
    {
        //消息队列
        std::string key = pService ->GetGameSvrdMsgRoute();
        std::string strBuffer;
//        int nResult = pService ->mqr() ->list_lpop(key, strBuffer);
//        if(nResult != 0)
//        {
//            usleep(1000);
//            continue ;
//        }
        int nResult = pService ->mqr() ->list_blpop(key, strBuffer , 1);
        if(IsNotZero(nResult))
        {
            continue;
        }
        pService ->AddEventQueueMsg(strBuffer);
    }
    return nullptr ;
}

bool CGameService::InitRecvMsgQueueThread()
{
    pthread_t nThreadID ;
    int ret = pthread_create(&nThreadID,NULL,CGameService::RecvProc,this);
    return ret == 0 ;
}

//获取该服务的路由地址
std::string CGameService::GetGameSvrdMsgRoute()
{
    return "WZQ_MSG_QUEUE_GAMESVRD_gamesvrd";
}


//获取room服务的路由地址
std::string CGameService::GetRoomSvrdMsgRoute()
{
    return "WZQ_MSG_QUEUE_ROOMSVRD_roomsvrd";
}

int CGameService::SendSvrdMsg(const std::string& msgroute , int64 msg_id , int64 seq_id , int64 user_id , const std::string& msg,int64 nClientID)
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
std::string CGameService::GetConnSvrdMsgRoute()
{
    return "WZQ_MSG_QUEUE_CONNSVRD_conn101_01";
}


//获取鉴权服务器的路由信息
std::string CGameService::GetAuthSvrdMsgRoute()
{
    return "WZQ_MSG_QUEUE_AUTHSVRD_wzqauth_01";
}
