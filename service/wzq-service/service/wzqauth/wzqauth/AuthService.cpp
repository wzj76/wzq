//
//  AuthService.cpp
//  authservice
//
//  Created by 刘挺 on 2022/11/24.
//

#include "AuthService.hpp"
#include "AppDef.h"
#include "log.h"
#include "wzqProtocol.pb.h"
#include "ProtocolDef.h"
#include<iostream>

CAuthService::CAuthService()
{
    m_pRandingList = new RandingList(this);
    m_pThreadPool = new Threadpool(5);
}

CAuthService::~CAuthService()
{
    
}
void CAuthService::UserRegister(std::string username,std::string password,int clientid)
{
    ACKRegister ack;
    do{
    int64 userid = 0 ;
    std::string passwd;
    int nresult = m_pSampleDBPool ->QueryUserInfo(username, userid, passwd);
    if(IsNotZero(nresult))
    {
        ack.set_code(register_result_failed_unknown);
        ack.set_result("注册失败，原因未知");
        break;
    }
    if(userid > 0)
    {
        ack.set_code(register_result_failed_exists);
        ack.set_result("注册失败，用户已经存在");
        break;
    }
    nresult = m_pSampleDBPool ->InsertUserInfo(username, password);
    if(IsNotZero(nresult))
    {
        ack.set_code(register_result_failed_unknown);
        ack.set_result("注册失败，原因未知");
        break;
    }
    ack.set_code(register_result_success);
    ack.set_result("注册成功");
    break;
    }while(0);
    SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_REGISTER | ID_ACK), 0, 0, ack.SerializeAsString(), clientid);
}

void CAuthService::UserLogin(std::string account,std::string password,int clientid)
{
    ACKLogin ack;
    do{
        std::string passwd;
        int64 userid = 0 ;
        int nresult = m_pSampleDBPool->QueryUserInfo(account, userid, passwd);
        if(IsNotZero(nresult))
        {
            ack.set_code(login_result_failed_unknown);
            break;
        }
        if(passwd != password)
        {
            ack.set_code(login_result_failed_passwd_error);
            break;
        }
        ack.set_code(login_result_success);
        printf("debug::设置ack.set_code(login_result_success)\n");

        int32 wintimes=0;
        std::string username;
        int32 losetimes=0;
        int32 score=0;
        int32 level=0;
        int32 xp=0;
        int32 head_id=0;
        int32 coins = 0;
        nresult = m_pSampleDBPool->QueryUserAllInfo(userid,username,wintimes,losetimes,score,level,xp,head_id,coins);
        
        if(IsNotZero(nresult))
        {
            ack.set_code(login_result_failed_unknown);
            break;
        }
        UserInfo userInfo;

       // 设置 UserInfo 中的字段值
       userInfo.set_id(userid);
       userInfo.set_username(username);
       userInfo.set_wintimes(wintimes);
       userInfo.set_losetimes(losetimes);
       userInfo.set_score(score);
       userInfo.set_level(level);
       userInfo.set_xp(xp);
       userInfo.set_head_id(head_id);
       userInfo.set_coins(coins);
        userInfo.set_account(account);
        
        //查找背包中道具的数量
        std::vector<std::pair<int,int>> v_prop;
        nresult = m_pSampleDBPool->QueryPropNum(userid,v_prop);
        for(int i=0;i<v_prop.size();i++)
        {
            // 创建一个 Prop 消息对象
            UserInfo_Prop* prop1 = userInfo.add_props();
            prop1->set_id(v_prop[i].first);
            prop1->set_quantity(v_prop[i].second);
        }
    
       // 将 UserInfo 设置到 ACKLogin 中
       ack.mutable_userinfo()->CopyFrom(userInfo);
        break;
    }while(0);
    SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_LOGIN | ID_ACK), 0, 0, ack.SerializeAsString(), clientid);
}

void CAuthService::ChangeHead(int user_id,int head_id,int clientid)
{
    ACKEditHead ack;
    do{

        int nresult = m_pSampleDBPool -> EditUserInfo(user_id, head_id);
        if(IsNotZero(nresult))
        {
            ack.set_code(1);
            break;
        }
        ack.set_code(0);
        break;
    }while(0);
    SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_EDITHEAD | ID_ACK), 0, 0, ack.SerializeAsString(), clientid);
}

void CAuthService::GetRandingLists(int limit,int offset,int clientid)
{
    
    std::vector<UserInfo> userinfo_v;
    if(m_pRandingList->m_randingList.size()>0)
    {
        m_pRandingList->SetRandingList(limit, offset, userinfo_v);
    }
    ACKGetRandingList ack;
    
    
    
    ack.set_code(0);
    ack.set_result_msg("get_randing_list_sucess");
    ack.set_limit(limit);
    ack.set_offset(offset);
    //将数组中的数据放到ack中
    for (const UserInfo& user_info : userinfo_v) {
         UserInfo* new_user_info = ack.add_userinfo_list();
         new_user_info->CopyFrom(user_info);
     }
    SendSvrdMsg(GetConnSvrdMsgRoute(), (RANDING_LIST | ID_ACK), 0, 0, ack.SerializeAsString(), clientid);
        
}

void CAuthService::UpdateGameInfo(const REQGameOver&req)
{
    int winner_id = req.winner_id();
    int loser_id = req.loser_id();
    int score = req.score();
    int coins = req.conis();
    int nresult = m_pSampleDBPool->UpdateWinnerAndLoser(winner_id,loser_id,score,coins);
    printf("更新winner和loser的信息成功\n");
    
    //保存对局信息
    printf("保存对局信息\n");
    m_pSampleDBPool->UpdateGameInfo(req);
}
void CAuthService::GetUserInfo(int user_id,int clientid)
{
    std::string username;
    int32 wintimes=0;
    int32 losetimes=0;
    int32 score=0;
    int32 level=0;
    int32 xp=0;
    int32 head_id=0;
    int32 coins =0;
    int nresult = m_pSampleDBPool->QueryUserAllInfo(user_id,username,wintimes,losetimes,score,level,xp,head_id,coins);
    ACKGetUserInfo ack;
    ack.set_result_id(0);
    if(IsNotZero(nresult))
    {
        ack.set_result_id(1);
    }
    UserInfo userInfo;

   // 设置 UserInfo 中的字段值
   userInfo.set_id(user_id);
   userInfo.set_username(username);
   userInfo.set_wintimes(wintimes);
   userInfo.set_losetimes(losetimes);
   userInfo.set_score(score);
   userInfo.set_level(level);
   userInfo.set_xp(xp);
   userInfo.set_head_id(head_id);
    userInfo.set_coins(coins);
   ack.mutable_userinfo()->CopyFrom(userInfo);
    
    SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_GET_USER_INFO | ID_ACK), 0, 0, ack.SerializeAsString(), clientid);
}
void CAuthService::GetHistoryList(int user_id,int limit,int offset,int clientid)
{
    std::vector<GameInfo> game_infos;
    int nresult = m_pSampleDBPool->GetGameHistory(user_id,limit,offset,game_infos);
    //保存对局信息
    ACKGetGameHistory ack;
    ack.set_player_id(user_id);

    ack.mutable_game_info_list()->CopyFrom({game_infos.begin(),game_infos.end()});
    
    printf("获取的历史记录数量:%d\n",ack.game_info_list_size());
    
    SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_GET_HISTORY | ID_ACK), 0, 0, ack.SerializeAsString(), clientid);
}

//获取好友信息
void CAuthService::GetFriendInfo(std::string friend_name,int user_id,int clientid)
{
    int32 id = 0;
    int32 wintimes=0;
    int32 losetimes=0;
    int32 score=0;
    int32 level=0;
    int32 xp=0;
    int32 head_id=0;
    std::string username;
    int nresult = m_pSampleDBPool->QueryUserAllInfo(friend_name,id,wintimes,losetimes,score,level,xp,head_id,username);

    if(IsNotZero(nresult))
    {
        printf("查询用户信息失败");
        return;
    }
    
    ACKGetFriendInfo ack;
    
    if(id!=0)
    {
        ack.set_is_exist(true);
    }
    else
    {
        ack.set_is_exist(false);
    }
    
    
    UserInfo userInfo;

    // 设置 UserInfo 中的字段值
    userInfo.set_id(id);
    userInfo.set_username(username);
    userInfo.set_wintimes(wintimes);
    userInfo.set_losetimes(losetimes);
    userInfo.set_score(score);
    userInfo.set_level(level);
    userInfo.set_xp(xp);
    userInfo.set_head_id(head_id);
    userInfo.set_account(friend_name);

    ack.mutable_friendinfo()->CopyFrom(userInfo);
    
    //查询是否是好友
    bool is_friend = false;
    nresult = m_pSampleDBPool->QueryIsFriend(id,user_id,is_friend);
    ack.set_is_my_friend(is_friend);
    
    SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_GET_FRIEND_INFO | ID_ACK), 0, 0, ack.SerializeAsString(), clientid);
}

void CAuthService::AddWaitFriend(int friend_id,int user_id,int clientid)
{
    ACKAddFriend ack;
    int nresult = 1;
    if(friend_id != user_id)
    {
        nresult = m_pSampleDBPool->AddWaitFriend(friend_id,user_id);
    }
    
    ack.set_code(nresult);
    if(IsNotZero(nresult))
    {
        ack.set_result_msg("add_friend_failed");
    }
    else
    {
        ack.set_result_msg("add_friend_sucess");
    }
    SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_ADD_FRIEND | ID_ACK), 0, 0, ack.SerializeAsString(), clientid);
    
    //告知对方已经有了好友
    REQRequestFriend req;
    int firned_num = 0;
    nresult = m_pSampleDBPool->FindAddWaitFriendNum(friend_id,firned_num);
    if(IsNotZero(nresult))
    {
        printf("FindAddFriendNum失败\n");
        return;
    }
    req.set_num(firned_num);
    req.set_user_id(friend_id);
    SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_REQ_FRIEND | ID_REQ), 0, 0, req.SerializeAsString(), 0);
    
}

void CAuthService::IsAgreeFriend(int user_id,int friend_id,bool is_agree,int clientid)
{
    int nresult = m_pSampleDBPool->RemoveWaitFriend(user_id,friend_id);
    ACKIsAgreeFriend ack;
    ack.set_code(nresult);
    if(is_agree)
    {
        //在添加好友之前需要先检查这两个人是否是好友
        bool is_friend = false;
        m_pSampleDBPool->QueryIsFriend(user_id,friend_id,is_friend);
        if(is_friend == true)
        {
            ack.set_result_msg("添加好友失败，对方已经是你的好友");
            SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_IS_AGREE_ADD_FRIEND | ID_ACK), 0, 0, ack.SerializeAsString(), clientid);
            return;
        }
        
        
        nresult = m_pSampleDBPool->AddFriend(user_id,friend_id);
        ack.set_result_msg("同意添加好友成功");
        
        printf("添加好友成功\n");
    }
    else
    {
        ack.set_result_msg("拒绝添加好友成功");
    }
    printf("SendSvrdMsg\n");
    SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_IS_AGREE_ADD_FRIEND | ID_ACK), 0, 0, ack.SerializeAsString(), clientid);
}

void CAuthService::GetFriends(int user_id,int clientid)
{
    ACKFriendsList ack;
    std::vector<int> friend_ids;
    int nresult = m_pSampleDBPool->GetFriendIDs(user_id,friend_ids);
    
    //根据id获取所有好友信息
    for(int i=0;i<friend_ids.size();i++)
    {
        UserInfo *userInfo = ack.add_firendinfo();
        int friend_id = friend_ids[i];
        std::string username;
        int32 wintimes=0;
        int32 losetimes=0;
        int32 score=0;
        int32 level=0;
        int32 xp=0;
        int32 head_id=0;
        int nresult = m_pSampleDBPool->QueryUserAllInfo(friend_id,username,wintimes,losetimes,score,level,xp,head_id);

       // 设置 UserInfo 中的字段值
       userInfo->set_id(friend_id);
       userInfo->set_username(username);
       userInfo->set_wintimes(wintimes);
       userInfo->set_losetimes(losetimes);
       userInfo->set_score(score);
       userInfo->set_level(level);
       userInfo->set_xp(xp);
       userInfo->set_head_id(head_id);
    }
    SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_GET_FRIENDS_LIST | ID_ACK), 0, 0, ack.SerializeAsString(), clientid);
}

void CAuthService::GetWaitFriends(int user_id,int clientid)
{
    ACKWaitFriendsList ack;
    std::vector<int> wait_friend_ids;
    std::vector<std::string> times;
    int nresult = m_pSampleDBPool->GetWaitFriendIDs(user_id,wait_friend_ids,times);
    
    //根据id获取所有好友信息
    for(int i=0;i<wait_friend_ids.size()&&i<times.size();i++)
    {
        ack.add_times(times[i]);
        
        UserInfo *userInfo = ack.add_firendinfo();
        int friend_id = wait_friend_ids[i];
        std::string username;
        int32 wintimes=0;
        int32 losetimes=0;
        int32 score=0;
        int32 level=0;
        int32 xp=0;
        int32 head_id=0;
        int nresult = m_pSampleDBPool->QueryUserAllInfo(friend_id,username,wintimes,losetimes,score,level,xp,head_id);

       // 设置 UserInfo 中的字段值
       userInfo->set_id(friend_id);
       userInfo->set_username(username);
       userInfo->set_wintimes(wintimes);
       userInfo->set_losetimes(losetimes);
       userInfo->set_score(score);
       userInfo->set_level(level);
       userInfo->set_xp(xp);
       userInfo->set_head_id(head_id);
    }
    SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_GET_WAIT_FRIEND_LIST | ID_ACK), 0, 0, ack.SerializeAsString(), clientid);
}
void CAuthService::DeleteFriend(int user_id,int friend_id,int clientid)
{
    int nresult = m_pSampleDBPool->DeleteFriend(user_id,friend_id);
    ACKDeleteFriend ack;
    ack.set_code(nresult);
    if(IsNotZero(nresult))
    {
        ack.set_result_msg("删除好友失败");
    }
    else
    {
        ack.set_result_msg("删除好友成功");
    }
    SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_DELETE_FRIEND | ID_ACK), 0, 0, ack.SerializeAsString(), clientid);
}


bool CAuthService::OnReceiveQueueMsg(BYTE* pbyBuffer, int nBufferLen)
{
    mcgWriteLog("OnReceiveQueueMsg recv one msg");
    MsgPackage msg ;
    if(! msg.ParseFromArray(pbyBuffer, nBufferLen))
    {
        mcgWriteLog("CConnService::OnReceiveQueueMsg parse failed ");
        return false ;
    }
    //处理注册消息
    if(msg.msg_id() == (MSGID_REGISTER | ID_REQ))
    {
        REQRegister req ;
        req.ParseFromString(msg.msg());
        mcgWriteLog("注册消息: 账号 : %s 密码 : %s",req.account().c_str() , req.passwd().c_str());
        //UserRegister(req.account(),req.passwd(),msg.clientid());
        
        m_pThreadPool->enqueue([this,req,msg]() {
            UserRegister(req.account(),req.passwd(),msg.clientid());
        });
        
        return true;
    }
    //处理登录消息
    if(msg.msg_id() == (MSGID_LOGIN | ID_REQ))
    {
        REQLogin req ;
        req.ParsePartialFromString(msg.msg());
        mcgWriteLog("登录消息：账号 ： %s 密码 ： %s",req.account().c_str() , req.passwd().c_str());
        //UserLogin(req.account(),req.passwd(),msg.clientid());
        
        m_pThreadPool->enqueue([this,req,msg]() {
            UserLogin(req.account(),req.passwd(),msg.clientid());
        });
    }
    
    //处理更改头像消息
    if(msg.msg_id() == (MSGID_EDITHEAD | ID_REQ))
    {
        REQEditHead req ;
        req.ParsePartialFromString(msg.msg());
        mcgWriteLog("用户 %d要改为第%d个头像",req.user_id() , req.head_num());
        m_pThreadPool->enqueue([this,req,msg]() {
            ChangeHead(req.user_id(),req.head_num(),msg.clientid());
        });
    }
    
    if(msg.msg_id() == (RANDING_LIST | ID_REQ))
    {
        REQGetRandingList req ;
        req.ParsePartialFromString(msg.msg());
        std::cout<<"auth收到的获取排行榜的信息:"<<req.req_msg()<<" "<<req.limit()<<" "<<req.offset()<<std::endl;
        //GetRandingLists(req.limit(),req.offset(),msg.clientid());
        m_pThreadPool->enqueue([this,req,msg]() {
            GetRandingLists(req.limit(),req.offset(),msg.clientid());
        });
    }
    
    
    
    if(msg.msg_id() == (MSGID_GMAE_OVER | ID_REQ))
    {
        printf("更新winner和loser的信息开始\n");
        REQGameOver req ;
        req.ParsePartialFromString(msg.msg());
        //UpdateGameInfo(req);
        m_pThreadPool->enqueue([this,req,msg]() {
            UpdateGameInfo(req);
        });
    }
    
    //处理获取用户信息
    if(msg.msg_id() == (MSGID_GET_USER_INFO | ID_REQ))
    {
        printf("获取用户信息开始\n");
        REQGetUserInfo req ;
        req.ParsePartialFromString(msg.msg());
        //GetUserInfo(req.user_id(),msg.clientid());
        m_pThreadPool->enqueue([this,req,msg]() {
            GetUserInfo(req.user_id(),msg.clientid());
        });
    }
    
    
    
    if(msg.msg_id() == (MSGID_GET_HISTORY | ID_REQ))
    {
        printf("开始获取历史记录\n");
        REQGetGameHistory req ;
        req.ParsePartialFromString(msg.msg());
        //GetHistoryList(req.player_id(),req.limit(),req.offset(),msg.clientid());
        m_pThreadPool->enqueue([this,req,msg]() {
            GetHistoryList(req.player_id(),req.limit(),req.offset(),msg.clientid());
        });
    }
    
    if(msg.msg_id() == (MSGID_GET_FRIEND_INFO | ID_REQ))
    {
        printf("开始获取好友信息\n");
        REQGetFriendInfo req ;
        req.ParsePartialFromString(msg.msg());
        //GetFriendInfo(req.friend_name(),req.user_id(),msg.clientid());
        m_pThreadPool->enqueue([this,req,msg]() {
            GetFriendInfo(req.friend_name(),req.user_id(),msg.clientid());
        });
    }
    
    if(msg.msg_id() == (MSGID_ADD_FRIEND | ID_REQ))
    {
        printf("开始处理添加好友的请求\n");
        REQAddFriend req ;
        req.ParsePartialFromString(msg.msg());
        //AddWaitFriend(req.friend_id(),req.user_id(),msg.clientid());
        m_pThreadPool->enqueue([this,req,msg]() {
            AddWaitFriend(req.friend_id(),req.user_id(),msg.clientid());
        });
    }
    
    if(msg.msg_id() == (MSGID_IS_AGREE_ADD_FRIEND | ID_REQ))
    {
        printf("开始处理同意或拒绝好友申请的请求\n");
        REQIsAgreeFriend req ;
        req.ParsePartialFromString(msg.msg());
        //IsAgreeFriend(req.user_id(),req.friend_id(),req.is_agree(),msg.clientid());
        m_pThreadPool->enqueue([this,req,msg]() {
            IsAgreeFriend(req.user_id(),req.friend_id(),req.is_agree(),msg.clientid());
        });
    }
    
    if(msg.msg_id() == (MSGID_GET_FRIENDS_LIST | ID_REQ))
    {
        printf("开始处理获取好友列表的请求\n");
        REQFriendsList req ;
        req.ParsePartialFromString(msg.msg());
        //GetFriends(req.user_id(),msg.clientid());
        m_pThreadPool->enqueue([this,req,msg]() {
            GetFriends(req.user_id(),msg.clientid());
        });
    }
    if(msg.msg_id() == (MSGID_GET_WAIT_FRIEND_LIST | ID_REQ))
    {
        printf("开始处理获取wait好友列表的请求\n");
        REQWaitFriendsList req ;
        req.ParsePartialFromString(msg.msg());
        //GetWaitFriends(req.user_id(),msg.clientid());
        m_pThreadPool->enqueue([this,req,msg]() {
            GetWaitFriends(req.user_id(),msg.clientid());
        });
    }
    
    //删除好友
    if(msg.msg_id() == (MSGID_DELETE_FRIEND | ID_REQ))
    {
        printf("开始处理删除好友的请求\n");
        REQDeleteFriend req ;
        req.ParsePartialFromString(msg.msg());
        //DeleteFriend(req.user_id(),req.friend_id(),msg.clientid());
        m_pThreadPool->enqueue([this,req,msg]() {
            DeleteFriend(req.user_id(),req.friend_id(),msg.clientid());
        });
    }
    
    //购买商品
    if(msg.msg_id() == (MSGID_BUY | ID_REQ))
    {
        printf("开始处理购买商品的请求\n");
        REQBuy req ;
        req.ParsePartialFromString(msg.msg());
        m_pThreadPool->enqueue([this,req,msg]() {
            BuyItems(req.user_id(),req.item_id(),req.num(),req.price(),msg.clientid());
        });
    }
    //修改名字
    if(msg.msg_id() == (MSGID_EDIT_NAME | ID_REQ))
    {
        printf("开始处理修改名字的请求\n");
        REQEditUserName req ;
        req.ParsePartialFromString(msg.msg());
        m_pThreadPool->enqueue([this,req,msg]() {
            ChangeUserName(req.user_id(),req.username(),req.item_id(),msg.clientid());
        });
    }
    
    
    
    
    return true;
}
void CAuthService::ChangeUserName(int user_id,std::string username,int item_id,int clientid)
{
    ACKEditUserName ack;
    
    if(item_id>0)
    {
        //判断该用户的改名卡数量是否大于0，如果小于0则改名失败
        int num=0;
        int nresult = m_pSampleDBPool -> GetItemNum(user_id,item_id, num);
        if(num<=0)
        {
            ack.set_code(1);
            ack.set_username("改名卡不足");
            SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_EDIT_NAME | ID_ACK), 0, 0, ack.SerializeAsString(), clientid);
            return;
        }
        
    }
    
    int nresult = m_pSampleDBPool -> EditUserInfo(user_id,username);
    if(IsNotZero(nresult))
    {
        ack.set_code(1);
    }
    ack.set_code(0);
    ack.set_username(username);
    
    if(item_id>0)
    {
        m_pSampleDBPool -> ReduceItemNum(user_id, item_id, 1);
    }
    

    SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_EDIT_NAME | ID_ACK), 0, 0, ack.SerializeAsString(), clientid);
}



void CAuthService::BuyItems(int user_id,int item_id,int num,int all_price,int clientid)
{
    //先判断是否持有该道具
    bool is_has = false;
    int nresult = m_pSampleDBPool->CheckUserHasItem(user_id, item_id, is_has);
    ACKBuy ack;
    ack.set_code(nresult);
    if(IsNotZero(nresult))
    {
        ack.set_result_msg("购买失败");
        SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_BUY | ID_ACK), 0, 0, ack.SerializeAsString(), clientid);
        return;
    }
    
    if(is_has==false)
    {
        nresult = m_pSampleDBPool->AddItem(user_id, item_id, num);
        ack.set_code(nresult);
        if(IsNotZero(nresult))
        {
            ack.set_result_msg("购买失败");
            SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_BUY | ID_ACK), 0, 0, ack.SerializeAsString(), clientid);
            return;
        }
        nresult = m_pSampleDBPool->ReducedCoins(user_id, all_price);
        ack.set_code(nresult);
        if(IsNotZero(nresult))
        {
            ack.set_result_msg("购买失败");
            SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_BUY | ID_ACK), 0, 0, ack.SerializeAsString(), clientid);
            return;
        }
        ack.set_code(nresult);
        ack.set_result_msg("购买成功");
        ack.set_item_id(item_id);
        ack.set_price(all_price);
        ack.set_num(num);
        SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_BUY | ID_ACK), 0, 0, ack.SerializeAsString(), clientid);
        return;
    }
    else
    {
        nresult = m_pSampleDBPool->IncreaseItemNum(user_id, item_id, num);
        ack.set_code(nresult);
        if(IsNotZero(nresult))
        {
            ack.set_result_msg("购买失败");
            SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_BUY | ID_ACK), 0, 0, ack.SerializeAsString(), clientid);
            return;
        }
        nresult = m_pSampleDBPool->ReducedCoins(user_id, all_price);
        ack.set_code(nresult);
        if(IsNotZero(nresult))
        {
            ack.set_result_msg("购买失败");
            SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_BUY | ID_ACK), 0, 0, ack.SerializeAsString(), clientid);
            return;
        }
        ack.set_code(nresult);
        ack.set_result_msg("购买成功");
        ack.set_item_id(item_id);
        ack.set_price(all_price);
        ack.set_num(num);
        SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_BUY | ID_ACK), 0, 0, ack.SerializeAsString(), clientid);
        return;
    }
}

//获取该服务器的路由配置
std::string CAuthService::GetAuthSvrdMsgRoute()
{
    return "WZQ_MSG_QUEUE_AUTHSVRD_" + getServiceID();
}

void* CAuthService::RecvProc(void* pParam)
{
    CAuthService* pService = (CAuthService*)(pParam);
    if(! pService) return nullptr ;
    std::string key = pService ->GetAuthSvrdMsgRoute();
    printf("CAuthService::RecvProc key : %s \n",key.c_str());
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
        pService ->AddEventQueueMsg(strBuffer);
    }
    return nullptr ;
}

//获取连接服的路由信息
std::string CAuthService::GetConnSvrdMsgRoute()
{
    return "WZQ_MSG_QUEUE_CONNSVRD_conn101_01";
}

bool CAuthService::InitRecvMsgQueueThread()
{
    pthread_t nThreadID ;
    int ret = pthread_create(&nThreadID,NULL,CAuthService::RecvProc,this);
    return ret == 0 ;
}

bool CAuthService::OnInit()
{
    if (!CService::OnInit()) return false;
    //初始化读取消息队列的线程
    InitRecvMsgQueueThread();
    //初始化数据库
    InitDBService();
    //初始化排行榜句柄
    InitRandingList();
    return true;
}

bool CAuthService::InitRandingList()
{
    m_pRandingList ->StartRandingListTimer();
    return true;
}

bool CAuthService::InitDBService()
{
    char *szHost = g_oLuaIni.GetString(DB_HOST);
    int nDBPort = g_oLuaIni.GetInt(DB_PORT);
    char *szDBUserName = g_oLuaIni.GetString(DB_USERNAME);
    char *szDBPassword = g_oLuaIni.GetString(DB_PASSWORD);
    char *szDBName = g_oLuaIni.GetString(DB_NAME);
    int nDBConnNum = g_oLuaIni.GetInt(DB_CONN_NUM);
    if ((szHost == NULL) || (szDBUserName == NULL) || (szDBPassword == NULL) || (szDBName == NULL))
    {
        return false;
    }

    m_pSampleDBPool = new CSampleDBPool();
    if (!m_pSampleDBPool)
    {
        mcgWriteLog("new db sample pool failed");
        return false;
    }

     if (0 != m_pSampleDBPool->Connect(szHost, szDBUserName, szDBPassword, szDBName, nDBPort))
    {
        mcgWriteLog("init db sample pool failed");
        delete m_pSampleDBPool;
        m_pSampleDBPool = NULL;
        return false;
    }

    mcgWriteLog("InitDBService (IP=%s, Port=%d, UserName=%s, Pwd=%s, DBName=%s, ConnNum=%d) success",
                szHost, nDBPort, szDBUserName, szDBPassword, szDBName, nDBConnNum);
    return true;
}

int CAuthService::SendSvrdMsg(const std::string& msgroute , int64 msg_id , int64 seq_id , int64 user_id , const std::string& msg,int64 nClientID)
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
