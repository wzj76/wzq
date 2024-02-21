//
//  ConnectProxy.cpp
//  game
//
//  Created by Joi Yao on 16/10/17.
//
//

#include "WZQConnectProxy.h"
#include "Core/AppEvent.h"
#include "Core/AppDef.h"
#include "Core/AppVersion.h"
#include "AppDelegate.h"
#include "Views/Common/WZQWaitingLayer.h"
#include "Util/Utils.h"
#include "AppDelegate.h"
#include "Core/AppModel.h"
#include "WZQInGameModel.hpp"

#define REGFUNC(_MSGID_, _TEMPLATECLASS_, _FUNC_)\
m_pSocket->RegisterMsg(_MSGID_, new _TEMPLATECLASS_(), this, onMsgHandler_selector(_FUNC_));

#define CONN_LIMIT  3           //尝试重连几次
#define CONN_MAX    10          //最多尝试重连几次
#define CONN_TIME_INTERVAL 5    //重连时间间隔
#define TCPTIMEOUT  10

USING_NS_CC;
using namespace std;
using namespace util;

void WZQConnectProxy::Configure()
{
    WZQMsgService::Configure();
    // 注册
    REGFUNC(MSGID_REGISTER | MSGID_ACK,    ACKUserRegister,       WZQConnectProxy::OnACKUserRegister);
    REGFUNC(MSGID_LOGIN | MSGID_ACK,    ACKUserLogin,       WZQConnectProxy::OnACKUserLogin);
    REGFUNC(MSGID_MATCH | MSGID_ACK,    ACKRoom,       WZQConnectProxy::OnACKMatch);
    REGFUNC(RANDING_LIST | MSGID_ACK,    ACKGetRandingList,       WZQConnectProxy::OnACKRandingList);
    REGFUNC(MSGID_CREATE_GAME | MSGID_ACK,    ACKCreatGame,       WZQConnectProxy::OnACKACKCreatGame);
    REGFUNC(MSGID_ACTION | MSGID_ACK,    ACKAction,       WZQConnectProxy::OnACKSendChess);
    REGFUNC(MSGID_EXIT_GAME | MSGID_ACK,    ACKExitGame,       WZQConnectProxy::OnACKSendExitGame);
    REGFUNC(MSGID_GET_USER_INFO | MSGID_ACK,    ACKGetUserInfo,       WZQConnectProxy::OnAckSendGetUserInfo);
    
    REGFUNC(MSGID_ASK_UNDO_CHESS | MSGID_REQ,    REQAskUndoChess,       WZQConnectProxy::OnREQAskUndoChess);
    
    REGFUNC(MSGID_GET_HISTORY | MSGID_ACK,    ACKGetGameHistory,       WZQConnectProxy::OnAckSendGetGameHistory);
    
    REGFUNC(MSGID_LOGIN_DIFFERENT | MSGID_REQ,    REQExitAccount,       WZQConnectProxy::OnReqDuplicateLogins);
    REGFUNC(MSGID_CHAT | MSGID_ACK,  ACKChatText,  WZQConnectProxy::OnAckChat);
    REGFUNC(MSGID_CONTINUE_GAME | MSGID_ACK,  ACKContinueGame,  WZQConnectProxy::OnACKContinueGame);
    REGFUNC(MSGID_ASK_CONTINUE_GAME | MSGID_REQ,  REQAskContinueGame,  WZQConnectProxy::OnREQAskContinueGame);
    REGFUNC(MSGID_REFUSE_GAME | MSGID_ACK,  ACKRefuseContinueGame,  WZQConnectProxy::OnAckRefuseContinueGame);
    
    
    REGFUNC(MSGID_GET_FRIENDS_LIST | MSGID_ACK,  ACKFriendsList,  WZQConnectProxy::OnAckGetFriendList);
    REGFUNC(MSGID_GET_FRIEND_INFO | MSGID_ACK,  ACKGetFriendInfo,  WZQConnectProxy::OnAckSearchFriend);
    REGFUNC(MSGID_ADD_FRIEND | MSGID_ACK,  ACKAddFriend,  WZQConnectProxy::OnAckAddFriend);
    REGFUNC(MSGID_IS_AGREE_ADD_FRIEND | MSGID_ACK,  ACKIsAgreeFriend,  WZQConnectProxy::OnAckIsAgreeFriend);
    REGFUNC(MSGID_REQ_FRIEND | MSGID_REQ,  REQRequestFriend,  WZQConnectProxy::onAckNfriend);
    REGFUNC(MSGID_GET_WAIT_FRIEND_LIST | MSGID_ACK,  ACKWaitFriendsList,  WZQConnectProxy::OnAckGetWaitFriendList);
    
    REGFUNC(MSGID_DELETE_FRIEND | MSGID_ACK,  ACKDeleteFriend,  WZQConnectProxy::OnAckDeleteFriend);
    
    //邀请好友
    REGFUNC(MSGID_INVITE_FRIEND_GAME | MSGID_ACK,  ACKInviteFriendGame,  WZQConnectProxy::OnAckInviteFriendGame);
    REGFUNC(MSGID_ASK_INVITE_FRIEND_GAME | MSGID_REQ,  REQAskInviteFriendGame,  WZQConnectProxy::OnREQAskInviteFriendGame);
    REGFUNC(MSGID_REFUSE_INVITE_FRIEND_GAME | MSGID_ACK,  ACKRefuseInviteFriendGame,  WZQConnectProxy::OnACKRefuseInviteFriendGame);
    //确认购买
    REGFUNC(MSGID_BUY | MSGID_ACK,  ACKBuy ,WZQConnectProxy::OnACKBuyItems);
    
    //修改头像和修改名字
    REGFUNC(MSGID_EDIT | MSGID_ACK,    ACKEditHead,       WZQConnectProxy::OnACKEdit);
    REGFUNC(MSGID_EDIT_NAME | MSGID_ACK,    ACKEditUserName,       WZQConnectProxy::OnACKEditUserName);
}

void WZQConnectProxy::OnServiceConnected(bool bConnected)
{
    log("ConnectProxy::OnServiceConnected = %s", bConnected? "true": "false");
    m_bConnect = bConnected;
    if(m_bConnect == false)
    {
        BroadcastRrops();
    }
    
    WZQMsgService::OnServiceConnected(bConnected);
    unTickAll();
}
void WZQConnectProxy::BroadcastRrops()
{
    dispatchEvent(CONN_FAILED);
}
bool WZQConnectProxy::IsConnect()
{
    return m_bConnect;
}


void WZQConnectProxy::OnServiceClosed()
{
    log("ConnectProxy::OnServiceClosed");
    CheckConnect();
}

// 开始等待
void WZQConnectProxy::BeginConnectWait()
{
    EndConnectWait();
    tickOnce(CC_SCHEDULE_SELECTOR(WZQConnectProxy::OnConnectTimeout), this, TCPTIMEOUT);
}

// 结束等待
void WZQConnectProxy::EndConnectWait()
{
    unTick(CC_SCHEDULE_SELECTOR(WZQConnectProxy::OnConnectTimeout), this);
    WZQWaitingLayer::sharedInstance()->hide();
}

// 连接超时
void WZQConnectProxy::OnConnectTimeout(float dt)
{
    Close();
    OnServiceConnected(false);
}

// 连接
void WZQConnectProxy::DoConnect()
{
    log("ConnectProxy::DoConnect");
    ConnectServer();
}

void WZQConnectProxy::CheckConnect()
{
    log("ConnectProxy::CheckConnect");
    DoConnect();
}

void WZQConnectProxy::CloseConnect()
{
    log("ConnectProxy::CloseConnect");
    EndConnectWait();
    unTickAll();
    Close();
}

void WZQConnectProxy::ConnectServer()
{
    CloseConnect();
    BeginConnectWait();
    log("ConnectProxy::Connect:%s::%d", HOST_TCP, PORT_TCP);
    Connect(HOST_TCP, PORT_TCP);
}

void WZQConnectProxy::ConnectFail()
{
    log("ConnectProxy::ConnectFail");
    ClearStandbyMsg();
    CloseConnect();
}

void WZQConnectProxy::SendUserRegister(const std::string& sUserName, const std::string& sPassWord)
{
    REQUserRegister req;
    req.set_user_name(sUserName);
    req.set_user_pwd(sPassWord);
    SendMsg(&req, MSGID_REGISTER | MSGID_REQ);
    

}


void WZQConnectProxy::OnACKUserRegister(Message *pMsg)
{
    EndConnectWait();
    ACKUserRegister* pAck = static_cast<ACKUserRegister*>(pMsg);
    
    int id = pAck->result_id();
    std::string result = pAck->result_msg();

    
    log("ConnectProxy::OnACKUserRegister >>> result=%d", pAck->result_id());
    ValueMap oData;
    oData.insert (make_pair("resultid" ,Value(id)));
    oData.insert(make_pair("resultmessage" , Value(result)));
          
          
    dispatchEvent(ACK_REGISTER, &oData);
}
void WZQConnectProxy::SendMatch(std::string matchMessage)
{
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    REQRoom req;
    req.set_req_msg(matchMessage);
    UserInfo pb_userinfo;
    pb_userinfo.set_username(usrinfo.sUser);
    pb_userinfo.set_id(usrinfo.nUserId);
    pb_userinfo.set_score(usrinfo.Score);
    pb_userinfo.set_wintimes(usrinfo.Wintimes);
    pb_userinfo.set_losetimes(usrinfo.Losetimes);
    pb_userinfo.set_head_id(usrinfo.head);
    req.mutable_userinfo()->CopyFrom(pb_userinfo);
    SendMsg(&req, MSGID_MATCH | MSGID_REQ);
}
void WZQConnectProxy::OnACKMatch(Message *pMsg)
{
    EndConnectWait();
    ACKRoom* pAck = static_cast<ACKRoom*>(pMsg);
    
    std::string result = pAck->result_msg();

    
    log("ConnectProxy::OnACKUserRoom >>> result=%s", result.c_str());
          
          
    dispatchEvent(ACK_MATCH, pMsg);
}

void WZQConnectProxy::SendEdit(int headNum){
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    REQEditHead pEdit_Head;
    pEdit_Head.set_head_num(headNum);
    pEdit_Head.set_user_id(usrinfo.nUserId);
    printf("代理收到了用户%d的修改第%d个头像的通知\n",pEdit_Head.user_id(),pEdit_Head.head_num());
    SendMsg(&pEdit_Head, MSGID_EDIT|MSGID_REQ);
}
void WZQConnectProxy::SendEditName(std::string username)
{
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    REQEditUserName req;
    req.set_username(username);
    req.set_user_id(usrinfo.nUserId);
    req.set_item_id(17);
    
    
    printf("代理收到了用户%d的修改%s 名字的通知\n",req.user_id(),req.username().c_str());
    SendMsg(&req, MSGID_EDIT_NAME|MSGID_REQ);
}



void WZQConnectProxy::OnACKEdit(Message *pMsg)
{
    EndConnectWait();
    ACKEditHead* pAck = static_cast <ACKEditHead*> (pMsg);
    int num = pAck ->code();
    if(num == 0){
        log("ConnectProxy::OnACKUserRoom >>> 修改头像成功");
    }
    else{
        log("ConnectProxy::OnACKUserRoom >>> 修改头像失败");
    }
}
void WZQConnectProxy::OnACKEditUserName(Message *pMsg)
{
    dispatchEvent(ACK_EXIT_USER_NAME, pMsg);
}


// 登录
void WZQConnectProxy::SendUserLogin(const std::string& sUserName, const std::string& sPassWord)
{
    REQUserLogin req;
    req.set_user_name(sUserName);
    req.set_user_pwd(sPassWord);
    SendMsg(&req, MSGID_LOGIN | MSGID_REQ);
}

void WZQConnectProxy::OnACKUserLogin(Message *pMsg)
{
    EndConnectWait();
    ACKUserLogin* pAck = static_cast<ACKUserLogin*>(pMsg);
    log("ConnectProxy::OnACKUserLogin >>> result=%d", pAck->result_id());

    ACKUserLogin oData = *pAck;
    dispatchEvent(ACK_LOGIN, &oData);
}

//optional int32 id = 1;
//optional bytes username = 2;
//optional int32 wintimes = 3;
//optional int32 losetimes = 4;
//optional int32 score = 5;
//optional int32 level = 6;
//optional int32 xp = 7;

// 注册消息
void WZQConnectProxy::registerMsg(unsigned int nMsgId, google::protobuf::Message *pTemplate, cocos2d::Ref *pTarget, SEL_OnMsgHandler pFunc)
{
    m_pSocket->RegisterMsg(nMsgId, pTemplate, pTarget, pFunc);
}

void WZQConnectProxy::SendRandingList(void *pUserData)
{
    
    
    ValueMap vm = *static_cast<ValueMap*>(pUserData);
    string message = vm.at("message").asString();
    int limit = vm.at("limit").asInt();
    int offset = vm.at("offset").asInt();
    
    
    REQGetRandingList req;
    
    req.set_req_msg(message);
    req.set_limit(limit);
    req.set_offset(offset);
    printf("debug------发出一条获取排行榜信息的消息\n");
    SendMsg(&req, RANDING_LIST | MSGID_REQ);
}

void WZQConnectProxy::OnACKRandingList(Message *pMsg)
{
    EndConnectWait();
    printf("成功拿到排行榜信息\n");
    dispatchEvent(ACK_RANDING_LIST, pMsg);
    
}

//给服务端发落子消息
void WZQConnectProxy::SendChess(void *pUserData)
{
    std::cout<<"进入WZQConnectProxy::SendChess"<<std::endl;
    REQAction act;
    ValueMap vm = *static_cast<ValueMap*>(pUserData);
    string message = vm.at("req_msg").asString();
    int table_id = vm.at("table_id").asInt();
    int action_id = vm.at("action_id").asInt();
    int action_board_row = vm.at("action_board_row").asInt();
    int action_board_col = vm.at("action_board_col").asInt();
    int remain_time = vm.at("remain_time").asInt();
    act.set_req_msg(message);
    act.set_table_id(table_id );
    act.set_action_id(action_id);
    act.set_action_board_col(action_board_row);
    act.set_action_board_row(action_board_col);
    act.set_remain_time(remain_time);
    cout<<"代理已拿到要落子或悔棋的消息"<<"桌子："<<act.table_id()<<"用户："<<act.action_id()<<"落子列数："<<act.action_board_col()<<"落子行数："<<act.action_board_row()<<endl;
    SendMsg(&act, MSGID_ACTION|MSGID_REQ);
}

void WZQConnectProxy::OnACKSendChess(Message *pMsg){
    
    printf("收到了一个ACK_ACTION消息\n");
    dispatchEvent(ACK_PLACE_A_STONE , pMsg);
}

void WZQConnectProxy::OnACKACKCreatGame(Message *pMsg)
{
    EndConnectWait();
    dispatchEvent(ACK_CREATE_GAME, pMsg);
    
}

//给game服务器发送玩家退出的消息
//给服务端发落子消息
void WZQConnectProxy::SendExitGame(void *pUserData)
{
    std::cout<<"进入WZQConnectProxy::SendExitGame"<<std::endl;
    REQExitGame req;
    ValueMap vm = *static_cast<ValueMap*>(pUserData);
    string message = vm.at("req_msg").asString();
    int table_id = vm.at("exit_table_id").asInt();
    int action_id = vm.at("exit_user_id").asInt();
    int enemy_user_id =vm.at("enemy_user_id").asInt();
    req.set_req_msg(message);
    req.set_exit_table_id(table_id );
    req.set_exit_user_id(action_id);
    req.set_enemy_user_id(enemy_user_id);

    SendMsg(&req, MSGID_EXIT_GAME|MSGID_REQ);
}
void WZQConnectProxy::OnACKSendExitGame(Message *pMsg)
{
    std::cout<<"收到用户退出的消息"<<std::endl;
    EndConnectWait();
    dispatchEvent(ACK_EXIT_GAME, pMsg);
    
}

void WZQConnectProxy::SendGetUserInfo(void *pUserData)
{
    std::cout<<"进入WZQConnectProxy::SendGetUserInfo"<<std::endl;
    REQGetUserInfo req;
    ValueMap vm = *static_cast<ValueMap*>(pUserData);

    int user_id = vm.at("user_id").asInt();
    req.set_user_id(user_id);
    SendMsg(&req, MSGID_GET_USER_INFO|MSGID_REQ);

}
void WZQConnectProxy::OnAckSendGetUserInfo(Message *pMsg)
{
    std::cout<<"进入WZQConnectProxy::OnAckSendGetUserInfo"<<std::endl;
    EndConnectWait();
    dispatchEvent(ACK_GET_USER_INFO, pMsg);
}

void WZQConnectProxy::OnREQAskUndoChess(Message *pMsg)
{
    std::cout<<"进入WZQConnectProxy::OnREQAskUndoChess"<<std::endl;
    EndConnectWait();
    dispatchEvent(REQ_ASK_USER_UNDO_CHESS, pMsg);
}

void WZQConnectProxy::SendAckUndo(void *pUserData){
    
    std::cout<<"进入WZQConnectProxy::SendAckUndo"<<std::endl;
    ACKAskUndoChess ack = *static_cast<ACKAskUndoChess*>(pUserData);
    SendMsg(&ack, MSGID_ASK_UNDO_CHESS|MSGID_ACK);
}



void WZQConnectProxy::SendGetGameHistory(void *pUserData)
{
    ValueMap vm = *static_cast<ValueMap*>(pUserData);
    int player_id = vm.at("player_id").asInt();
    int limit = vm.at("limit").asInt();
    int offset = vm.at("offset").asInt();
    REQGetGameHistory req;

    req.set_player_id(player_id);
    req.set_limit(limit);
    req.set_offset(offset);
    printf("debug------发出一条获取历史记录的消息\n");
    SendMsg(&req, MSGID_GET_HISTORY | MSGID_REQ);
}
void WZQConnectProxy::OnAckSendGetGameHistory(Message *pMsg)
{
    std::cout<<"进入WZQConnectProxy::OnAckSendGetGameHistory"<<std::endl;
    EndConnectWait();
    dispatchEvent(ACK_GET_HISTORY, pMsg);
}

void WZQConnectProxy::SendExitAccount(void * pUserData){
    ValueMap vm = *static_cast<ValueMap*>(pUserData);
    int user_id = vm.at("user_id").asInt();
    REQExitAccount exit;
    exit.set_user_id(user_id);
    printf("debug------用户:%d发出一条请求注销账号的消息\n",exit.user_id());
    SendMsg(&exit, MSGID_EXIT_ACCOUNT| MSGID_REQ);
}


void WZQConnectProxy::OnReqDuplicateLogins(Message *pMsg)
{
    std::cout<<"进入WZQConnectProxy::OnReqDuplicateLogins(Message *pMsg)"<<std::endl;
    EndConnectWait();
    dispatchEvent(EVT_DUPLICATE_LOGINS, pMsg);
}


void WZQConnectProxy::SendChat(void * pUserData){
    ValueMap vm = *static_cast<ValueMap*>(pUserData);
    int user_id = vm.at("user_id").asInt();
    int table_id = vm.at("table_id").asInt();
    int opp_id = vm.at("opp_id").asInt();
    std::string text = vm.at("text").asString();
    REQChat chat;
    chat.set_user_id(user_id);
    chat.set_table_id(table_id);
    chat.set_opp_id(opp_id);
    chat.set_text(text);
    printf("debug------用户:%d发出一条信息\n",chat.user_id());
    cout<<"消息是:"<<chat.text()<<endl;
    SendMsg(&chat, MSGID_CHAT| MSGID_REQ);
}

void WZQConnectProxy::OnAckChat(Message *pMsg){
    std::cout<<"进入WZQConnectProxy::OnAckChat"<<std::endl;
    EndConnectWait();
    dispatchEvent(ACK_CHAT, pMsg);
}

//发送继续游戏的请求
void WZQConnectProxy::SendContinueGame(void * pUserData)
{
    REQContinueGame req;
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    req.set_user_id(usrinfo.nUserId);
    
    int enemy_id = AppModel::getInstance() -> m_pInGameModel -> getOppoUserInfo().nUserId;
    req.set_enemy_user_id(enemy_id);
    int table_id = AppModel::getInstance() -> m_pInGameModel -> m_gameConf.iRoomId;
    req.set_table_id(table_id);
    SendMsg(&req, MSGID_CONTINUE_GAME| MSGID_REQ);
}
void WZQConnectProxy::SendAgreeContinueGame(void * pUserData)
{
    ACKAskContinueGame* req = static_cast<ACKAskContinueGame*>(pUserData);
    std::cout<<"进入WZQConnectProxy::SendAgreeContinueGame"<<std::endl;
    SendMsg(req, MSGID_ASK_CONTINUE_GAME| MSGID_ACK);
}


void WZQConnectProxy::OnACKContinueGame(Message *pMsg)
{
    std::cout<<"进入WZQConnectProxy::onACKContinueGame"<<std::endl;
    EndConnectWait();
    dispatchEvent(EVT_ACK_CONTINUE_GAME, pMsg);
}

void WZQConnectProxy::OnREQAskContinueGame(Message *pMsg)
{
    std::cout<<"进入WZQConnectProxy::OnREQAskContinueGame"<<std::endl;
    dispatchEvent(EVT_REQ_ASK_CONTINUE_GAME, pMsg);
}

void WZQConnectProxy::OnAckRefuseContinueGame(Message *pMsg)
{
    std::cout<<"进入WZQConnectProxy::OnAckRefuseContinueGame"<<std::endl;
    dispatchEvent(ACK_REFUSE_CONTINUE_GAME, pMsg);
}


void WZQConnectProxy::SendGetFriendList(void *pUserData){
    ValueMap vm = *static_cast<ValueMap*>(pUserData);
    int user_id = vm.at("user_id").asInt();
    REQFriendsList req;
    req.set_user_id(user_id);
    printf("debug------用户:%d发出一条请求好友列表的消息\n",req.user_id());
    SendMsg(&req, MSGID_GET_FRIENDS_LIST| MSGID_REQ);
}

void WZQConnectProxy::OnAckGetFriendList(Message *pMsg){
    std::cout<<"进入WZQConnectProxy::OnAckGetFriendList"<<std::endl;
    EndConnectWait();
    dispatchEvent(ACK_GET_FRIEND_LIST, pMsg);
}

void WZQConnectProxy::SendSearchFriend(void * pUserData){
    ValueMap vm = *static_cast<ValueMap*>(pUserData);
    int user_id = vm.at("user_id").asInt();
    string friend_name = vm.at("friend_name").asString();
    REQGetFriendInfo req;
    req.set_user_id(user_id);
    req.set_firend_name(friend_name);
    printf("debug------用户:%d发出一条请求搜索好友的消息\n",req.user_id());
    cout<<"要搜索的好友名字是:"<<req.firend_name()<<endl;
    SendMsg(&req, MSGID_GET_FRIEND_INFO| MSGID_REQ);
}

void WZQConnectProxy::OnAckSearchFriend(Message *pMsg){
    std::cout<<"进入WZQConnectProxy::OnAckSearchFriend"<<std::endl;
    EndConnectWait();
    dispatchEvent(ACK_SEARCH_FRIEND, pMsg);
}

void WZQConnectProxy::SendAddFriend(void * pUserData){
    REQAddFriend req = *static_cast<REQAddFriend*>(pUserData);
    printf("debug------用户:%d发出一条请求搜索好友的消息\n",req.user_id());
    cout<<"要添加的好友ID是:"<<req.friend_id()<<endl;
    SendMsg(&req, MSGID_ADD_FRIEND| MSGID_REQ);
}

void WZQConnectProxy::OnAckAddFriend(Message *pMsg){
    std::cout<<"进入WZQConnectProxy::OnAckAddFriend"<<std::endl;
    EndConnectWait();
    dispatchEvent(ACK_ADD_FRIEND, pMsg);
    
}

//void WZQConnectProxy::OnReqAddFriend(Message *pMsg){
//    printf("收到对方的请求好友请求\n");
//    EndConnectWait();
//    dispatchEvent(EVT_ADD_FRIEND, pMsg);
//}

void WZQConnectProxy::onAckNfriend(Message *pMsg){
    printf("收到n条好友请求\n");
    dispatchEvent(EVT_ADD_FRIEND,pMsg);
}

void WZQConnectProxy::SendGetWaitFriendList(void * pUserData){
    ValueMap vm = *static_cast<ValueMap*>(pUserData);
    int user_id = vm.at("user_id").asInt();
    REQWaitFriendsList req;
    req.set_user_id(user_id);
    printf("debug------用户:%d发出一条请求更新waitlist的消息\n",req.user_id());
    SendMsg(&req, MSGID_GET_WAIT_FRIEND_LIST| MSGID_REQ);
}

void WZQConnectProxy::OnAckGetWaitFriendList(Message *pMsg){
    printf("收到待同意好友列表的更新信息\n");
    dispatchEvent(ACK_UPDATE_WAIT_LIST,pMsg);
}

void WZQConnectProxy::SendIsAgreeFriend(void * pUserData){
    ValueMap vm = *static_cast<ValueMap*>(pUserData);
    int user_id = vm.at("user_id").asInt();
    int friend_id = vm.at("friend_id").asInt();
    bool is_agree = vm.at("is_agree").asBool();
    REQIsAgreeFriend req;
    req.set_user_id(user_id);
    req.set_friend_id(friend_id);
    req.set_is_agree(is_agree);
    printf("debug------用户:%d发出一条是否同意好友请求的消息\n",req.user_id());
    SendMsg(&req, MSGID_IS_AGREE_ADD_FRIEND| MSGID_REQ);
}

void WZQConnectProxy::OnAckIsAgreeFriend(Message *pMsg)
{
    printf("收到同意或拒绝添加好友的ack\n");
    dispatchEvent(ACK_IS_AGREE_FRIEND,pMsg);
}

void WZQConnectProxy::SendDeleteFriend(void * pUserData)
{
    ValueMap vm = *static_cast<ValueMap*>(pUserData);
    int user_id = vm.at("user_id").asInt();
    int friend_id = vm.at("friend_id").asInt();
    REQDeleteFriend req;
    req.set_user_id(user_id);
    req.set_friend_id(friend_id);
    printf("debug------用户:%d发出一条删除好友的消息\n",req.user_id());
    SendMsg(&req, MSGID_DELETE_FRIEND| MSGID_REQ);
}

void WZQConnectProxy::OnAckDeleteFriend(Message *pMsg)
{
    printf("收到删除好友的ack\n");
    dispatchEvent(ACK_DELETE_FRIEND,pMsg);
}


void WZQConnectProxy::SendInviteFriendGame(void * pUserData)
{
    ValueMap vm = *static_cast<ValueMap*>(pUserData);
    int friend_id = vm.at("friend_id").asInt();
    REQInviteFriendGame req;
    req.set_friend_id(friend_id);
    UserInfo userinfo;
    
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    userinfo.set_username(usrinfo.sUser);
    userinfo.set_id(usrinfo.nUserId);
    userinfo.set_score(usrinfo.Score);
    userinfo.set_wintimes(usrinfo.Wintimes);
    userinfo.set_losetimes(usrinfo.Losetimes);
    userinfo.set_head_id(usrinfo.head);
    req.mutable_userinfo()->CopyFrom(userinfo);
    printf("debug------用户:%d发出一条邀请好友游戏的消息\n",usrinfo.nUserId);
    SendMsg(&req, MSGID_INVITE_FRIEND_GAME| MSGID_REQ);
}

void WZQConnectProxy::OnAckInviteFriendGame(Message *pMsg)
{
    printf("收到邀请好友的ack\n");
    dispatchEvent(ACK_INVITE_FRIEND_GAME,pMsg);
}


void WZQConnectProxy::OnREQAskInviteFriendGame(Message *pMsg)
{
    std::cout<<"进入WZQConnectProxy::OnREQAskInviteFriendGame"<<std::endl;
    dispatchEvent(EVT_REQ_ASK_INVITE_FRIEND_GAME, pMsg);
}


void WZQConnectProxy::SendAgreeInviteFriendGame(void * pUserData)
{
    std::cout<<"进入WZQConnectProxy::SendAgreeInviteFriendGame"<<std::endl;
    ACKAskContinueGame* pAck = static_cast<ACKAskContinueGame*>(pUserData);
    SendMsg(pAck, MSGID_ASK_INVITE_FRIEND_GAME| MSGID_ACK);
}

void WZQConnectProxy::OnACKRefuseInviteFriendGame(Message *pMsg)
{
    std::cout<<"进入WZQConnectProxy::OnACKRefuseInviteFriendGame"<<std::endl;
    dispatchEvent(EVT_ACK_REFUSE_INVITE_FRIEND_GAME, pMsg);
}

void WZQConnectProxy::SendBuyItems(void * pUserData){
    std::cout<<"进入WZQConnectProxy::SendAgreeInviteFriendGame"<<std::endl;
    ValueMap vm = *static_cast<ValueMap*>(pUserData);
    int user_id = vm.at("user_id").asInt();
    int item_id = vm.at("item_id").asInt();
    int price = vm.at("price").asInt();
    int num = vm.at("num").asInt();
    REQBuy req;
    req.set_user_id(user_id);
    req.set_item_id(item_id);
    req.set_price(price);
    req.set_num(num);
    printf("debug------用户:%d发出一条要购买数量为%d单格为%d的商品%d的消息\n",req.user_id(),req.num(),req.price(),req.item_id());
    SendMsg(&req, MSGID_BUY | MSGID_REQ);
}

void WZQConnectProxy::OnACKBuyItems(Message *pMsg){
    std::cout<<"进入WZQConnectProxy::OnACKBuyItems"<<std::endl;
    dispatchEvent(ACK_BUY, pMsg);
}
