#ifndef __GAME_SERVICE_H__
#define __GAME_SERVICE_H__

#include "service.h"
#include "RedisIntf.h"
#include "AppDef.h"
#include <string>
#include <list>
#include <map>
#include <set>
#include "TableMgr.h"
#include "PlayerMgr.h"
#include "log.h"
#include "gameProtocol.pb.h"
#include "ThreadPool.hpp"
using namespace std;

class CGameService : public CService
{
public:
	CGameService();
	virtual ~CGameService();
public:
	virtual bool OnInit();									//
	virtual bool OnTimeOut(DWORD dwEventID);				//
	virtual void OnExitProcess(UINT uExitCode);				//
    virtual bool OnReceiveQueueMsg(BYTE* pbyBuffer, int nBufferLen);
public:
    std::shared_ptr<CPlayer> GetPlayerByID(int64 user_id);
    std::shared_ptr<CTable> GetTableByID(int nTableID);
	int GetTotalPlayer();
	int GetTotalTable();
    bool InitSvrdID();
    std::string GetSvrdID(){return m_strSvrdID;}
    
	bool InitAppLog();
	bool InitRedis();
    
    //初始化消息队列缓存
    bool InitMQRedis();
    std::string GetGameSvrdMsgRoute();
    //获取该连接服务器的路由消息数据
    std::string GetRoomSvrdMsgRoute();
    //获取鉴权服务的路由信息
    std::string GetAuthSvrdMsgRoute();
    int SendSvrdMsg(const std::string& msgroute , int64 msg_id , int64 seq_id , int64 user_id , const std::string& msg,int64 nClientID);

    //获取连接服的路由信息
    std::string GetConnSvrdMsgRoute();
    static void* printMessage(void* ptr,unsigned int a);
private:
    static void* RecvProc(void* pParam);
    
    bool InitRecvMsgQueueThread();
public:
	void UpLogLevel();			//
	void DownLogLevel();		//
    void OnSvrdClose();
    bool SvrdNeedClose();
    

public:
	CTableMgr* m_pTableMgr;
	CPlayerMgr* m_pPlayerMgr;
	CRedisIntf* m_pRedis;
	CLog* m_pLogicLog;
    
    //消息队列缓存
    CRedisIntf*               m_pMQRedis;
    
    Threadpool*               m_pThreadPool;
private:
	HANDLE  m_hTrySendTimer;
	bool    m_bInitOk;											//
	string  m_strAppLogPath;
	string  m_strAppServiceName;
    bool    m_bSvrdClose        ;                             //服务器收到关闭的指令 ,即将关闭
    std::string m_strSvrdID    ;                             //服务器ID
};

#endif
