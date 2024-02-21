//
//  WZQLobbyController.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/30.
//

#include "WZQLobbyController.hpp"
#include "BeatingModeScene.hpp"
#include "AppDelegate.h"
#include "Core/AppEvent.h"
#include "Core/AppVersion.h"
#include "Core/AppModel.h"
#include"AppEvent.h"
#include <iostream>
#include <vector>



USING_NS_CC;
using namespace std;

WZQLobbyController::WZQLobbyController()
:
WZQController("WZQLobbyController")
{
}

void WZQLobbyController::listEventNames()
{
    m_oEventNames =
    {
        EVT_RANDING_LIST,
        ACK_RANDING_LIST,
        EVT_MATCH,
        ACK_MATCH,
        EVT_EDIT_HEAD,
        ACK_EDIT_HEAD,
        EVT_EDIT_USER_NAME,
        ACK_EXIT_USER_NAME,
        ACK_CREATE_GAME,
        EVT_GET_USER_INFO,
        ACK_GET_USER_INFO,
        EVT_GET_HISTORY,
        ACK_GET_HISTORY,
        EVT_EXIT_ACCOUNT,
        EVT_DUPLICATE_LOGINS,
        SETUP_COMFIRM,
        EVT_BUY,
        ACK_BUY,
    };
}

void WZQLobbyController::onEventData(const string &sName, void *pUserData)
{
    if(sName == EVT_GET_USER_INFO)
    {
        doGetUserInfo(pUserData);
    }
    else if(sName == ACK_GET_USER_INFO)
    {
        std::cout<<"收到用户信息成功"<<std::endl;
        ACKGetUserInfo* pAck = static_cast<ACKGetUserInfo*>(pUserData);
        std::cout<<pAck->userinfo().id()<<" win_times:"<<pAck->userinfo().wintimes()
        <<" score:"<<pAck->userinfo().score()<<std::endl;
        
        KKUSERINFO oUserInfo = AppModel::getInstance() -> m_pUserModel -> getMyUserInfo();
        oUserInfo.sUser = pAck->userinfo().username();
        oUserInfo.nUserId = pAck -> userinfo().id();
        oUserInfo.Wintimes = pAck -> userinfo().wintimes();
        oUserInfo.Score = pAck -> userinfo().score();
        oUserInfo.Losetimes = pAck -> userinfo().losetimes();
        oUserInfo.Xp = pAck -> userinfo().xp();
        oUserInfo.coins = pAck->userinfo().coins();
        AppModel::getInstance()->m_pUserModel->setMyUserInfo(oUserInfo);
        
        dispatchEvent(GET_USER_INFO_SUCCESS);
        
    }
    
    if(sName == EVT_RANDING_LIST)
    {
        doGetRandingList(pUserData);
    }
    else if(sName == ACK_RANDING_LIST)
    {
        ACKGetRandingList* pAck = static_cast<ACKGetRandingList*>(pUserData);
        std::cout<<"排行榜的ack消息:"<<pAck->result_msg()<<" "<<pAck->limit()<<" "<<pAck->offset()<<std::endl;
        if (pAck)
        {
            log("获取排行榜信息成功");
            std::vector<KKUSERINFO> rankingList;
            for (int i = 0; i < pAck->userinfo_list_size(); ++i)
            {
                const UserInfo& userInfo = pAck->userinfo_list(i);
                KKUSERINFO user;
                user.Score = userInfo.score();
                user.nUserId = userInfo.id();
                user.sUser = userInfo.username();
                user.Level = userInfo.level();
                user.Wintimes = userInfo.wintimes();
                user.Losetimes = userInfo.losetimes();
                user.Xp = userInfo.xp();
                user.head=userInfo.head_id();
                rankingList.push_back(user);
            }
            AppModel::getInstance()->m_pUserModel->setRankingList(rankingList);
            dispatchEvent(RCV_RANDING);
        }
    }
    //匹配
    if(sName == EVT_MATCH)
    {
        ValueMap vm = *static_cast<ValueMap*>(pUserData);
        string message = vm.at("message").asString();
        doMatch(message);
    }
    else if(sName == ACK_MATCH)
    {
        ACKRoom* pAck = static_cast<ACKRoom*>(pUserData);
        if(pAck->result_msg()=="ack_match_sucess")
        {
            // 匹配成功
            // 保存对手的信息
            KKUSERINFO oOppoInfo;
            oOppoInfo.head = pAck -> userinfo().head_id();
            oOppoInfo.sUser = pAck ->userinfo().username();
            oOppoInfo.Score = pAck -> userinfo().score();
            oOppoInfo.nUserId = pAck -> userinfo().id();
            AppModel::getInstance() -> m_pInGameModel -> setOppoUserInfo(oOppoInfo);
            AppModel::getInstance() -> m_pInGameModel -> m_gameConf.oOppoInfo = oOppoInfo;
            std::cout<<"对手信息:"<<pAck->userinfo().username()<<" "<<pAck->userinfo().score()<<std::endl;

        }
    }
    //编辑头像
    if(sName == EVT_EDIT_HEAD){
        Value v =*static_cast<Value*>(pUserData);
        int num = v.asInt();
        doEdit(num);
    }
    else if(sName == ACK_EDIT_HEAD){
        ACKEditHead * pAck = static_cast<ACKEditHead*>(pUserData);
        if(pAck->code() == 0 ){
            log("头像修改成功！！！");
            dispatchEvent(EVT_EDIT_HEAD_SUCCESS);
        }
    }
    if(sName == EVT_EDIT_USER_NAME)
    {
        Value v =*static_cast<Value*>(pUserData);
        string username = v.asString();
        doEditName(username);
    }
    else if(sName == ACK_EXIT_USER_NAME)
    {
        ACKEditUserName* pAck = static_cast<ACKEditUserName*>(pUserData);
        if(pAck->code()==0)
        {
            cocos2d::MessageBox(pAck->username().c_str(), "修改成功");
            KKUSERINFO oUserInfo = AppModel::getInstance() -> m_pUserModel -> getMyUserInfo();
            oUserInfo.sUser = pAck->username();
            //改名卡数量-1 TODO0
            AppModel::getInstance()->m_pUserModel->setMyUserInfo(oUserInfo);
            dispatchEvent(EDIT_NAME_SUCCESS);
        }
        else
        {
            cocos2d::MessageBox(pAck->username().c_str(), "修改失败");
        }
    }
    
    //处理创建成功的逻辑
    if(sName == ACK_CREATE_GAME)
    {
        AppModel::getInstance()->m_pInGameModel-> m_chessNum = 0;
        ACKCreatGame* pAck = static_cast<ACKCreatGame*>(pUserData);
        // 存储自己的信息
        auto oMyinfo = AppModel::getInstance() -> m_pUserModel -> getMyUserInfo();
        AppModel::getInstance() -> m_pInGameModel -> setMyUserInfo(oMyinfo);
        AppModel::getInstance() -> m_pInGameModel -> m_gameConf.oMyInfo = oMyinfo;
        
        // 存储对局信息
        AppModel::getInstance() -> m_pInGameModel -> m_gameConf.llBlackId = pAck -> black_id();
        AppModel::getInstance() -> m_pInGameModel -> m_gameConf.llWhiteId = pAck -> white_id();
        AppModel::getInstance() -> m_pInGameModel -> m_gameConf.iRoomId = pAck -> room_id();
        AppModel::getInstance()->m_pInGameModel->m_score = pAck ->score();
        AppModel::getInstance()->m_pInGameModel->m_coins = pAck ->coins();
        auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
        if(AppModel::getInstance() -> m_pInGameModel -> m_gameConf.llBlackId == usrinfo.nUserId){
            AppModel::getInstance() -> m_pInGameModel -> m_IsMe = true;
            std::cout<<"我方开始下棋"<<std::endl;
        }
        else
        {
            AppModel::getInstance() -> m_pInGameModel -> m_IsMe = false;
            std::cout<<"对手开始下棋"<<std::endl;
        }
        
        printf("debug，table创建成功，用户需要保存table信息,房间号是%d，返回的信息:%s\n",pAck->room_id(),pAck->result_msg().c_str());
        dispatchEvent(EVT_MATCH_SUCESS);
    }
    
    //获取历史记录
    if(sName == EVT_GET_HISTORY)
    {
        doGetGameHistory(pUserData);
    }
    else if(sName == ACK_GET_HISTORY)
    {
        printf("debug++++ 获取历史记录成功\n");
        ACKGetGameHistory* pAck = static_cast<ACKGetGameHistory*>(pUserData);
        std::cout<<"获取的历史记录的id:"<<pAck->player_id()<<std::endl;
        std::cout<<"获取的历史记录数量:"<<pAck->game_info_list_size()<<std::endl;
        std::vector<HISTORYINFO> historyList;
        std::vector<BLACKPOSITION> blackpositon;
        std::vector<WHITEPOSITION> whiteposition;
        for(int i=0;i<pAck ->game_info_list_size();i++)
        {
            const GameInfo& game_info = pAck->game_info_list(i);
            std::cout<<"对手信息:"<<game_info.enemy_userinfo().username()<<" "<<game_info.enemy_userinfo().id()<<std::endl;
            std::cout<<"winner_id:"<<game_info.winner_id()<<" loser_id:"<<game_info.loser_id()<<std::endl;
            std::cout<<"black_id:"<<game_info.black_id()<<" white_id:"<<game_info.white_id()<<std::endl;
            std::cout<<"game time:"<<game_info.game_time()<<std::endl;
            HISTORYINFO history;
            KKUSERINFO user;
            user.Score = game_info.enemy_userinfo().score();
            user.nUserId = game_info.enemy_userinfo().id();
            user.sUser = game_info.enemy_userinfo().username();
            user.Level = game_info.enemy_userinfo().level();
            user.Wintimes = game_info.enemy_userinfo().wintimes();
            user.Losetimes = game_info.enemy_userinfo().losetimes();
            user.Xp = game_info.enemy_userinfo().xp();
            user.head = game_info.enemy_userinfo().head_id();
            history.enemy_userinfo = user;
            history.game_id = game_info.game_id();
            history.winner_id = game_info.winner_id();
            history.loser_id = game_info.loser_id();
            history.score = game_info.score();
            history.black_id = game_info.black_id();
            history.white_id = game_info.white_id();
            history.is_no_winner = game_info.is_no_winner();
            history.is_force_exit = game_info.is_force_exit();
            history.game_time = game_info.game_time();
            std::cout<<"黑色棋子下棋信息:"<<std::endl;;
            for(int j=0;j<game_info.black_positions_size();j++)
            {
                const Position& black_position = game_info.black_positions(j);
                cout<<" row:"<<black_position.row()<<" col:"<<black_position.col()<<std::endl;
                BLACKPOSITION black;
                black.row = black_position.row();
                black.col = black_position.col();
                blackpositon.push_back(black);
            }
            history.black_position = blackpositon;
            std::cout<<"白色棋子下棋信息:"<<std::endl;;
            for(int j=0;j<game_info.white_positions_size();j++)
            {
                const Position& white_position = game_info.white_positions(j);
                cout<<" row:"<<white_position.row()<<" col:"<<white_position.col()<<std::endl;
                WHITEPOSITION white;
                white.row = white_position.row();
                white.col = white_position.col();
                whiteposition.push_back(white);
            }
            history.white_positon = whiteposition;
            historyList.push_back(history);
            blackpositon.clear();
            whiteposition.clear();
        }
        AppModel::getInstance()->m_pUserModel->setHistory(historyList);
        dispatchEvent(RCV_HISTORY);
    }
    if(sName == EVT_EXIT_ACCOUNT){
        doExitAccount(pUserData);
    }
    if(sName == EVT_DUPLICATE_LOGINS)
    {
        printf("收到异地登录的消息\n");
        dispatchEvent(EVT_REPLACE_LOGIN);
        
    }
    
    // 确定设置
    if(sName == SETUP_COMFIRM)
    {
        
    }
    
    //请求购买
    if(sName == EVT_BUY){
        doBuyItems(pUserData);
    }
    else if(sName ==ACK_BUY){
        ACKBuy* pAck = static_cast<ACKBuy*>(pUserData);
        if(pAck->result_msg()=="购买成功")
        {
            //购买成功
            int price = pAck->price();
            int num = pAck->num();
            int id = pAck->item_id();
            auto userinfo = AppModel::getInstance()->m_pUserModel->getMyUserInfo();
            userinfo.coins -= price*num;
            AppModel::getInstance()->m_pUserModel->setMyUserInfo(userinfo);
            num += AppModel::getInstance()->m_pShopModel->getItemCount(id);
            AppModel::getInstance()->m_pShopModel->setItems(id, num);
            dispatchEvent(EVT_BUY_SUCCESS,pAck);
        }
    }
    
}
void WZQLobbyController::doGetGameHistory(void *pUserData)
{
    printf("debug-----------:WZQLobbyController发了一条获取历史记录消息\n");
    app->m_pConnect->SendGetGameHistory(pUserData);
}

void WZQLobbyController::doGetRandingList(void *pUserData)
{
    printf("debug-----------:WZQLobbyController发了一条获取排行榜消息\n");
    app->m_pConnect->SendRandingList(pUserData);
}

//匹配
void WZQLobbyController::doMatch(std::string message)
{
    printf("debug-----------:WZQLoginController发了一条匹配消息:%s\n",message.c_str());
    app->m_pConnect->SendMatch(message);
}

// 编辑头像
void WZQLobbyController::doEdit(int headnum){
    printf("debug:%d\n",headnum);
    app->m_pConnect->SendEdit(headnum);
}
void WZQLobbyController::doEditName(std::string username)
{
    printf("debug:%s\n",username.c_str());
    app->m_pConnect->SendEditName(username);
}


void WZQLobbyController::doGetUserInfo(void *pUserData)
{
    printf("debug-----------:WZQLobbyController发了一条获取用户信息的消息\n");
    app->m_pConnect->SendGetUserInfo(pUserData);
}

void WZQLobbyController::doExitAccount(void *pUserData){
    printf("debug-----------:WZQLobbyController发了一条请求退出账号的消息\n");
    app->m_pConnect->SendExitAccount(pUserData);
    
}

void WZQLobbyController::doBuyItems(void *pUserData){
    printf("debug-----------:WZQLobbyController发了一条请求购买的消息\n");
    app->m_pConnect->SendBuyItems(pUserData);
}
