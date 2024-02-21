//
//  ConnectProxy.h
//  game
//
//  Created by Joi Yao on 16/10/17.
//
//

#pragma once

#include "TimeTick/WZQTimeTick.h"
#include "Event/WZQEventWrapper.h"
#include "WZQCommonService.h"

class WZQConnectProxy : public WZQMsgService, public WZQTimeTick, public WZQEventWrapper
{
public:
    void Register(WZQCommonService* pService);
    void Configure();
    
protected:
    virtual void OnServiceConnected(bool bConnected);
    virtual void OnServiceClosed();
    
public:
    // 连接
    void DoConnect();
    void Reconnect();
    void CheckConnect();
    void CloseConnect();
    void ConnectServer();
    void ConnectFail();
        
    WZQConnectProxy()
    {
    }
    
    ~WZQConnectProxy()
    {
    }
    
    
public:
    // 注册
    void SendUserRegister(const std::string& sUserName, const std::string& sPassWord);
    // 登录
    void SendUserLogin(const std::string& sUserName, const std::string& sPassWord);
    // 匹配
    void SendMatch(std::string matchMessage);
    //编辑
    void SendEdit(int headNum);
    //修改名字
    void SendEditName(std::string username);
    //排行榜
    void SendRandingList(void *pUserData);
    //落子
    void SendChess(void *pUserData);
    //退出游戏
    void SendExitGame(void *pUserData);
    // 获取用户信息
    void SendGetUserInfo(void *pUserData);
    //发送确认悔棋请求
    void SendAckUndo(void *pUserData);
    //发送获取历史记录请求
    void SendGetGameHistory(void *pUserData);
    //发送退出账号
    void SendExitAccount(void * pUserData);
    //发送消息
    void SendChat(void * pUserData);
    //发送继续游戏的请求
    void SendContinueGame(void * pUserData);
    void SendAgreeContinueGame(void * pUserData);
    //发送请求好友列表
    void SendGetFriendList(void * pUserData);
    //发送查找好友
    void SendSearchFriend(void * pUserData);
    //发送添加好友
    void SendAddFriend(void * pUserData);
    //发送待同意好友列表
    void SendGetWaitFriendList(void * pUserData);
    //发送是否同意好友请求
    void SendIsAgreeFriend(void * pUserData);
    void SendDeleteFriend(void * pUserData);
    //发送邀请好友游戏
    void SendInviteFriendGame(void * pUserData);
    void SendAgreeInviteFriendGame(void * pUserData);
    //发送购买请求
    void SendBuyItems(void * pUserData);
    
    bool IsConnect();
private:
    void BeginConnectWait();
    void EndConnectWait();
    void OnConnectTimeout(float dt);
    
    void OnACKUserRegister(Message *pMsg);
    void OnACKUserLogin(Message *pMsg);
    void OnACKUMatch(Message *pMsg);
    void OnACKEdit(Message *pMsg);
    void OnACKEditUserName(Message *pMsg);
    void OnACKMatch(Message *pMsg);
    void OnACKRandingList(Message *pMsg);
    void OnACKACKCreatGame(Message *pMsg);
    void registerMsg(unsigned int nMsgId, Message *pTemplate, cocos2d::Ref *pTarget, SEL_OnMsgHandler pFunc);
    void OnACKSendChess(Message *pMsg);
    void OnACKSendExitGame(Message *pMsg);
    void OnACKContinueGame(Message *pMsg);
    void OnREQAskContinueGame(Message *pMsg);
    // 获取用户信息
    void OnAckSendGetUserInfo(Message *pMsg);
    // 接收询问是否同意悔棋
    void OnREQAskUndoChess(Message *pMsg);
    void OnAckSendGetGameHistory(Message *pMsg);
    void OnAckRefuseContinueGame(Message *pMsg);
    //异地登录
    void OnReqDuplicateLogins(Message *pMsg);
    //接收消息
    void OnAckChat(Message *pMsg);
    //接受好友列表
    void OnAckGetFriendList(Message *pMsg);
    //接受搜索的结果
    void OnAckSearchFriend(Message *pMsg);
//    //接受请求好友请求
//    void OnReqAddFriend(Message *pMsg);
    //接受好友确认结果
    void OnAckAddFriend(Message *pMsg);
    //收到n条好友请求
    void onAckNfriend(Message *pMsg);
    //收到待同意好友列表
    void OnAckGetWaitFriendList(Message *pMsg);
    void OnAckIsAgreeFriend(Message *pMsg);
    //删除好友的确认消息
    void OnAckDeleteFriend(Message *pMsg);
    void OnAckInviteFriendGame(Message *pMsg);
    void OnREQAskInviteFriendGame(Message *pMsg);
    void OnACKRefuseInviteFriendGame(Message *pMsg);
    //确认购买
    void OnACKBuyItems(Message *pMsg);
    
    //表示当前是否连接
    bool m_bConnect = false;
    void BroadcastRrops();
};
