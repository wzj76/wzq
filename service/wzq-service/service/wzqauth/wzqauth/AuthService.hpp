//
//  AuthService.hpp
//  authservice
//
//  Created by 刘挺 on 2022/11/24.
//

#ifndef AuthService_hpp
#define AuthService_hpp

#include "service.h"
#include "sampledbpool.h"
#include "RandingList.hpp"
#include "ThreadPool.hpp"
class RandingList;
class CAuthService : public CService
{
public:
    CAuthService();
    virtual ~CAuthService();

public:
    
    virtual IEventPump *mcgclient(DWORD dwIP,WORD wPort){return nullptr;}

    
    virtual bool OnInit();

    
    virtual bool OnMsg(DWORD dwMsgID, DWORD dwSeqID){return true;}

    
    virtual bool OnTimeOut(DWORD dwEventID){return true;}

    virtual void OnExitProcess(UINT uExitCode){return;}
    
    virtual bool OnReceiveQueueMsg(BYTE* pbyBuffer, int nBufferLen);
    
    virtual bool OnAsyncMsg(DWORD dwMsgID, DWORD dwSeqID, int nErrorCode, void* pvContext){return true;}
    
public :
    void OnSvrdClose(){m_bSvrdClose = true;}
    bool SvrdNeedClose(){return m_bSvrdClose;}
    //获取该服务器的路由配置
    std::string GetAuthSvrdMsgRoute();
    //获取连接服的路由信息
    std::string GetConnSvrdMsgRoute();
    //收取消息线程
    bool InitRecvMsgQueueThread();
    static void* RecvProc(void* pParam);
    //初始化数据库服务
    bool InitDBService();
    bool InitRandingList();
    //向内部服务转发消息
    int SendSvrdMsg(const std::string& msgroute , int64 msg_id , int64 seq_id , int64 user_id , const std::string& msg,int64 nClientID);
    
private:
    //处理注册逻辑
    void UserRegister(std::string username,std::string password,int clientid);
    //处理登录逻辑
    void UserLogin(std::string username,std::string password,int clientid);
    //处理更改头像逻辑
    void ChangeHead(int user_id,int head_id,int clientid);
    //处理修改名字
    void ChangeUserName(int user_id,std::string username,int item_id,int clientid);
    //获取排行榜
    void GetRandingLists(int limit,int offset,int clientid);
    //更新用户对局信息
    void UpdateGameInfo(const REQGameOver&req);
    //获取用户信息
    void GetUserInfo(int user_id,int clientid);
    //获取历史记录
    void GetHistoryList(int user_id,int limit,int offset,int clientid);
    //获取好友信息
    void GetFriendInfo(std::string friend_name,int user_id,int clientid);
    //添加好友
    void AddWaitFriend(int friend_id,int user_id,int clientid);
    //同意或拒绝添加好友
    void IsAgreeFriend(int user_id,int friend_id,bool is_agree,int clientid);
    //获取好友列表
    void GetFriends(int user_id,int clientid);
    //获取wait好友列表
    void GetWaitFriends(int user_id,int clientid);
    //删除好友
    void DeleteFriend(int user_id,int friend_id,int clientid);
    //购买商品
    void BuyItems(int user_id,int item_id,int num,int all_price,int clientid);
public:
    bool                                m_bSvrdClose        ;  //服务器收到关闭的指令 ,即将关闭
    CSampleDBPool*                      m_pSampleDBPool     ;  //数据库
    RandingList*                        m_pRandingList      ; //排行榜缓存
    Threadpool*                         m_pThreadPool       ; //线程池
};

#endif /* AuthService_hpp */
