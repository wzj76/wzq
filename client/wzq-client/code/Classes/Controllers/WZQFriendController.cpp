//
//  WZQFriendController.cpp
//  wzqgame-mobile
//
//  Created by duole on 2024/2/4.
//

#include "WZQFriendController.hpp"
#include "BeatingModeScene.hpp"
#include "AppDelegate.h"
#include "Core/AppEvent.h"
#include "Core/AppVersion.h"
#include "Core/AppModel.h"
#include"AppEvent.h"
#include <iostream>
#include <vector>

USING_NS_CC;

WZQFriendController::WZQFriendController()
:
WZQController("WZQFriendController")
{
}

void WZQFriendController::listEventNames()
{
    m_oEventNames =
    {
        EVT_GET_FRIEND_LIST,
        ACK_GET_FRIEND_LIST,
        EVT_SEARCH_FRIEND,
        ACK_SEARCH_FRIEND,
        EVT_ADD_FRIEND,
        ACK_ADD_FRIEND,
        EVT_UPDATE_WAIT_LIST,
        ACK_UPDATE_WAIT_LIST,
        EVT_IS_AGREE_FRIEND,
        ACK_IS_AGREE_FRIEND,
        EVT_DELETE_FRIEND,
        ACK_DELETE_FRIEND,
        EVT_INVITE_FRIEND_GAME,
        ACK_INVITE_FRIEND_GAME,
        EVT_REQ_ASK_INVITE_FRIEND_GAME,
        EVT_ACK_REQ_ASK_INVITE_FRIEND_GAME,
        EVT_ACK_REFUSE_INVITE_FRIEND_GAME,
        ADD_FRIEND_COMFIRM
    };
}

void WZQFriendController::onEventData(const std::string &sName, void *pUserData)
{
    if(sName == EVT_GET_FRIEND_LIST)
    {
        printf("收到请求好友列表的请求\n");
        doGetFriendList(pUserData);
    }
    else if(sName == ACK_GET_FRIEND_LIST)
    {
        ACKFriendsList* pAck = static_cast<ACKFriendsList*>(pUserData);
        if (pAck)
        {
            printf("确认得到好友列表\n");
            std::vector<KKUSERINFO> friendList;
            for (int i = 0; i < pAck->firendinfo_size(); ++i)
            {
                const UserInfo& userInfo = pAck->firendinfo(i);
                KKUSERINFO user;
                user.Score = userInfo.score();
                user.nUserId = userInfo.id();
                printf("好友id:%d\n",user.nUserId);
                user.sUser = userInfo.username();
                printf("好友name:%s\n",user.sUser.c_str());
                user.Level = userInfo.level();
                user.Wintimes = userInfo.wintimes();
                user.Losetimes = userInfo.losetimes();
                user.Xp = userInfo.xp();
                user.head=userInfo.head_id();
                user.isOnline = userInfo.is_online();
                friendList.push_back(user);
            }
            std::sort(friendList.begin(), friendList.end(), [](const KKUSERINFO& a, const KKUSERINFO& b) {
                   return a.isOnline > b.isOnline;  // 将在线的用户放在前面
               });
            AppModel::getInstance()->m_pUserModel->setFriendList(friendList);
            dispatchEvent(RCV_FRIEND_LIST);
        }
    }
    if(sName == EVT_SEARCH_FRIEND)
    {
        printf("查找好友请求发出\n");
        doSearchFriend(pUserData);
    }
    else if(sName == ACK_SEARCH_FRIEND)
    {
        ACKGetFriendInfo* pAck = static_cast<ACKGetFriendInfo*>(pUserData);
        if(pAck)
        {
            printf("成功得到搜索好友结果\n");
            if(pAck -> is_exist() && !pAck -> is_my_friend())
            {
                printf("满足添加好友的条件\n");
                KKUSERINFO myFriend;
                myFriend.sUser = pAck -> firendinfo().username();
                myFriend.nUserId = pAck->firendinfo().id();
                myFriend.head = pAck->firendinfo().head_id();
                myFriend.account = pAck->firendinfo().account();
                AppModel::getInstance()->m_pUserModel->m_friendInfo = myFriend;
                dispatchEvent(SEARCH_FRIEND_SUCCESS);

            }
            else if(pAck -> is_my_friend())
            {
                dispatchEvent(IS_FRIEND);
            }
            else
            {
                dispatchEvent(NOT_EXIST);
            }
        }
 
    }
    if(sName == EVT_ADD_FRIEND){
        REQRequestFriend* padd = static_cast<REQRequestFriend*>(pUserData);
        int num = padd -> num();
        printf("有%d个玩家想添加您为好友\n",num);
        dispatchEvent(EVT_NEW_FRIEND,padd);
    }
    if(sName == ADD_FRIEND_COMFIRM)
    {
        auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
        int friend_id = AppModel::getInstance()->m_pUserModel-> m_friendInfo.nUserId;
        REQAddFriend* req = new(REQAddFriend);
        req -> set_user_id(usrinfo.nUserId);
        req -> set_friend_id(friend_id);
        printf("用户%d想要添加用户%d的好友\n",req->user_id(),req->friend_id());
        doAddFriend((void*)req);
        delete req;
    }
    if(sName == ACK_ADD_FRIEND){
        printf("添加好友的请求已成功发出\n");
        dispatchEvent(ADD_FRIEND_SUCCESS);
    }
    if(sName ==EVT_UPDATE_WAIT_LIST)
    {
        printf("请求更新待同意的好友列表\n");
        doGetWaitFriendList(pUserData);
    }
    else if(sName == ACK_UPDATE_WAIT_LIST)
    {
        ACKWaitFriendsList* pAck = static_cast<ACKWaitFriendsList*>(pUserData);
        if (pAck)
        {
            printf("确认得到待同意列表\n");
            std::vector<KKUSERINFO> friendList;
            for (int i = 0; i < pAck->firendinfo_size(); ++i)
            {
                const UserInfo& userInfo = pAck->firendinfo(i);
                KKUSERINFO user;
                user.Score = userInfo.score();
                user.nUserId = userInfo.id();
                printf("好友id:%d\n",user.nUserId);
                user.sUser = userInfo.username();
                printf("好友name:%s\n",user.sUser.c_str());
                user.Level = userInfo.level();
                user.Wintimes = userInfo.wintimes();
                user.Losetimes = userInfo.losetimes();
                user.Xp = userInfo.xp();
                user.head=userInfo.head_id();
                friendList.push_back(user);
            }
            AppModel::getInstance()->m_pUserModel->setWaitFriendlist(friendList);
            dispatchEvent(RCV_WAIT_FRIEND_LIST);
        }
    }
    
    if(sName == EVT_IS_AGREE_FRIEND)
    {
        printf("已发送是否同意好友的请求");
        doIsAgreeFriend(pUserData);
    }
    else if(sName ==ACK_IS_AGREE_FRIEND)
    {
        printf("进入sName ==ACK_IS_AGREE_FRIEND\n");
        ACKIsAgreeFriend* pAck = static_cast<ACKIsAgreeFriend*>(pUserData);
        if(pAck -> code() ==0)
        {
            cocos2d::MessageBox(pAck->result_msg().c_str(), "发送成功");
        }
        else
        {
            cocos2d::MessageBox(pAck->result_msg().c_str(), "发送失败");
        }
    }
    
    if(sName == EVT_DELETE_FRIEND)
    {
        printf("发送删除好友的请求");
        doDeleteFriend(pUserData);
    }
    else if(sName == ACK_DELETE_FRIEND)
    {
        printf("收到删除好友的ack\n");
        ACKDeleteFriend* pAck = static_cast<ACKDeleteFriend*>(pUserData);
        if(pAck -> code() !=0)
        {
            cocos2d::MessageBox(pAck->result_msg().c_str(), "删除失败");
        }
        else
        {
            cocos2d::MessageBox(pAck->result_msg().c_str(), "删除成功");
            dispatchEvent(DELETE_FRIEND_SUCCESS);
//            auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
//            ValueMap oData;
//            oData.insert(make_pair("user_id",(int)usrinfo.nUserId));
//            doGetFriendList(&oData);
        }
        
    }
    
    if(sName == EVT_INVITE_FRIEND_GAME)
    {
        doInviteFriendGame(pUserData);
    }
    else if(sName == ACK_INVITE_FRIEND_GAME)
    {
        printf("进入sName ==ACK_INVITE_FRIEND_GAME\n");
        ACKInviteFriendGame* pAck = static_cast<ACKInviteFriendGame*>(pUserData);
        if(pAck -> code() ==0)
        {
            //cocos2d::MessageBox(pAck->result_msg().c_str(), "邀请成功，等待响应");
            //显示一个弹窗，等待对手响应
            dispatchEvent(WINDOW_INVITE_FRIEND_GAME,pAck);
            
        }
        else
        {
            cocos2d::MessageBox(pAck->result_msg().c_str(), "邀请失败");
        }
    }
    
    if(sName == EVT_REQ_ASK_INVITE_FRIEND_GAME)
    {
        //弹出选择窗口
        printf("收到询问是否同意邀请的请求\n");
        REQAskInviteFriendGame* pReq = static_cast<REQAskInviteFriendGame*>(pUserData);
        
        if(AppModel::getInstance()->m_pInGameModel->m_IsGaming == false)
        {
            dispatchEvent(WINDOW_REQ_ASK_INVITE_FRIEND_GAME,pReq);
        }
        else
        {
            //自动发送拒绝请求，此时用户正在游戏中
            ACKAskInviteFriendGame ack;
            ack.set_is_agree_invite_game(false);
            auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
            UserInfo user;
            user.set_id(usrinfo.nUserId);
            user.set_username(usrinfo.sUser);
            user.set_wintimes(usrinfo.Wintimes);
            user.set_losetimes(usrinfo.Losetimes);
            user.set_score(usrinfo.Score);
            user.set_head_id(usrinfo.head);
            ack.mutable_userinfo()->CopyFrom(user);
            
            ack.set_friend_id(pReq->friend_info().id());
            ack.set_result_msg("对方正在游戏中，拒绝邀请");
            dispatchEvent(EVT_ACK_REQ_ASK_INVITE_FRIEND_GAME,&ack);
        }
    }
    if(sName == EVT_ACK_REQ_ASK_INVITE_FRIEND_GAME)
    {
        doSendAckInviteFriendGame(pUserData);
    }
    if(sName == EVT_ACK_REFUSE_INVITE_FRIEND_GAME)
    {
        printf("收到拒绝邀请游戏的应答\n");
        ACKRefuseInviteFriendGame* pAck = static_cast<ACKRefuseInviteFriendGame*>(pUserData);
        cocos2d::MessageBox(pAck->result_msg().c_str(), "对方拒绝");
        dispatchEvent(REMOVE_WAIT_INVITE_FRIEND_GAME_LAYER);
    }
    
    
}

void WZQFriendController::doGetFriendList(void *pUserData)
{
    printf("debug-----------:WZQFriendController发了一条请求好友列表的消息\n");
    app->m_pConnect->SendGetFriendList(pUserData);
}

void WZQFriendController::doSearchFriend(void *pUserData)
{
    printf("debug-----------:WZQFriendController发了一条查找好友的消息\n");
    app->m_pConnect->SendSearchFriend(pUserData);
}

void WZQFriendController::doAddFriend(void *pUserData)
{
    printf("debug-----------:WZQFriendController发了一条添加好友的消息\n");
    app->m_pConnect->SendAddFriend(pUserData);
}

void WZQFriendController::doGetWaitFriendList(void *pUserData)
{
    printf("debug-----------:WZQFriendController发了一条请求更新待同意好友列表信息\n");
    app->m_pConnect->SendGetWaitFriendList(pUserData);
}

void WZQFriendController::doIsAgreeFriend(void *pUserData)
{
    printf("debug-----------:WZQFriendController发了一条是否同意好友的请求\n");
    app->m_pConnect->SendIsAgreeFriend(pUserData);
}

void WZQFriendController::doDeleteFriend(void *pUserData)
{
    printf("debug-----------:WZQFriendController发了一条删除好友的请求\n");
    app->m_pConnect->SendDeleteFriend(pUserData);
}


void WZQFriendController::doInviteFriendGame(void *pUserData)
{
    printf("debug-----------:WZQFriendController发了一条邀请好友游戏的请求\n");
    app->m_pConnect->SendInviteFriendGame(pUserData);
}

void WZQFriendController::doSendAckInviteFriendGame(void *pUserData)
{
    printf("debug-----------:WZQFriendController发了一条同意或拒绝邀请好友游戏的请求\n");
    app->m_pConnect->SendAgreeInviteFriendGame(pUserData);
}
