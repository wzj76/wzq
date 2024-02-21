//
//  WZQInGameController.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/30.
//

#include "WZQInGameController.hpp"
#include "AppDelegate.h"
#include "Core/AppEvent.h"
#include "Core/AppVersion.h"
#include "Core/AppModel.h"
#include"AppEvent.h"
#include "PopTips.hpp"
#include <iostream>
#include <vector>



USING_NS_CC;
using namespace std;

WZQInGameController::WZQInGameController()
:
WZQController("WZQInGameController")
{
}

void WZQInGameController::listEventNames()
{
    m_oEventNames =
    {
        EVT_PLACE_A_STONE,
        ACK_PLACE_A_STONE,
        EVT_EXIT_GAME,
        ACK_EXIT_GAME,
        REQ_ASK_USER_UNDO_CHESS,
        ACK_ASK_USER_UNDO_CHESS,
        EVT_CHAT,
        ACK_CHAT,
        EVT_MSGID_CONTINUE_GAME,
        EVT_ACK_CONTINUE_GAME,
        EVT_REQ_ASK_CONTINUE_GAME,
        ACK_REQ_ASK_CONTINUE_GAME,
        ACK_REFUSE_CONTINUE_GAME,
    };
}

void WZQInGameController::onEventData(const string &sName, void *pUserData)
{
    //玩家落子
    if(sName == EVT_PLACE_A_STONE){
        doChess(pUserData);
    }
    else if(sName == ACK_PLACE_A_STONE)
    {
        ACKAction* pAck = static_cast<ACKAction*>(pUserData);
        if (pAck)
        {
            string ack_type = pAck->result_msg();
            if (ack_type == "put_chess_sucess")
            {
                auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
                if(pAck -> action_id()!= usrinfo.nUserId){
                    log("对方落子结束");
                    AppModel::getInstance() -> m_pInGameModel -> m_IsMe = true;
                    
                }
                else{
                    log("我方落子结束");
                    AppModel::getInstance()->m_pInGameModel-> m_chessNum ++;//我方落子+1
                    AppModel::getInstance() -> m_pInGameModel -> m_IsMe = false;
                }
                log("玩家%d在棋盘%d行%d列处落子成功",pAck -> action_id(),pAck -> action_board_row(), pAck -> action_board_col());
                dispatchEvent(EVT_PLACE_A_STONE_SUCCESS, pAck);
                if(pAck -> is_win() == true)
                {
                    log("获胜玩家是:%d , 失败玩家是:%d",pAck -> win_id(), pAck ->lose_id());
                    if (AppModel::getInstance()->m_pInGameModel->getMyUserId() == pAck -> win_id())
                    {
                        AppModel::getInstance()->m_pInGameModel->m_IsMeWin = true;
                        //设置自己和对手积分
                        auto userinfo = AppModel::getInstance()->m_pUserModel->getMyUserInfo();
                        userinfo.Score += AppModel::getInstance() ->m_pInGameModel->m_score;
                        userinfo.coins += AppModel::getInstance() ->m_pInGameModel->m_coins;
                        AppModel::getInstance()->m_pUserModel->setMyUserInfo(userinfo);
                        userinfo=AppModel::getInstance()->m_pInGameModel->getOppoUserInfo();
                        userinfo.Score -= AppModel::getInstance() ->m_pInGameModel->m_score;
                        AppModel::getInstance()->m_pInGameModel->setOppoUserInfo(userinfo);
                        
                        if(pAck->win_msg()!="")
                        {
                            cocos2d::MessageBox("对手超时", "游戏胜利");
                        }
                    }
                    else if(pAck -> win_id() == -1)
                    {
                        // TODO 平局逻辑
                        cocos2d::MessageBox("游戏结束", "平局");
                        AppModel::getInstance()->m_pInGameModel->m_IsMeWin = false;
                    }
                    else
                    {
                        AppModel::getInstance()->m_pInGameModel->m_IsMeWin = false;
                        
                        //设置自己和对手积分
                        auto userinfo = AppModel::getInstance()->m_pUserModel->getMyUserInfo();
                        userinfo.Score -= AppModel::getInstance() ->m_pInGameModel->m_score;
                        AppModel::getInstance()->m_pUserModel->setMyUserInfo(userinfo);
                        userinfo=AppModel::getInstance()->m_pInGameModel->getOppoUserInfo();
                        userinfo.Score += AppModel::getInstance() ->m_pInGameModel->m_score;
                        AppModel::getInstance()->m_pInGameModel->setOppoUserInfo(userinfo);
                        if(pAck->win_msg()!="")
                        {
                            cocos2d::MessageBox("下棋超时", "游戏失败");
                        }
                    }
                    dispatchEvent(EVT_GAME_OVER); //给客户端发游戏结束此时需要跳出结算界面
                }
                else{
                }
            }
            else if(ack_type == "undo_chess_failed")
            {
                auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
                if(pAck->action_id()==usrinfo.nUserId)
                {
                    log("悔棋失败，对方不同意悔棋");
                    dispatchEvent(EVT_UNDO_CHESS_FAIL);
                    dispatchEvent(REMOVE_WAIT_ACK_LAYER);
                }
                else
                {
                    dispatchEvent(REMOVE_LAYER);
                }
//                log("悔棋失败，对方不同意悔棋");
//                dispatchEvent(EVT_UNDO_CHESS_FAIL);
//                dispatchEvent(REMOVE_WAIT_ACK_LAYER);
//                dispatchEvent(REMOVE_LAYER);
            }
            else if(ack_type == "undo_chess_sucess")
            {
                log("悔棋成功，对方同意悔棋");
                auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
                if(pAck -> action_id()!= usrinfo.nUserId){
                    log("对方悔棋结束");
                    AppModel::getInstance() -> m_pInGameModel -> m_IsMe = false;
                    
                }
                else{
                    log("我方悔棋结束");
                    AppModel::getInstance()->m_pInGameModel-> m_chessNum --;//我方落子-1
                    AppModel::getInstance() -> m_pInGameModel -> m_IsMe = true;
                }
                log("玩家%d在棋盘%d行%d列处悔棋成功",pAck -> action_id(),pAck -> action_board_row(), pAck -> action_board_col());
                dispatchEvent(EVT_UNDO_CHESS_SUCCESS,pAck);
                dispatchEvent(REMOVE_WAIT_ACK_LAYER);
            }
            else if(ack_type == "undo_chess_wait")
            {
                log("悔棋消息发送成功，等待对手同意悔棋");
                dispatchEvent(EVT_UNDO_CHESS_WAIT,pAck);
            }
            
            
        }
    }
    else if(sName == EVT_EXIT_GAME)
    {
        doExitGame(pUserData);
    }
    else if(sName ==ACK_EXIT_GAME)
    {
        std::cout<<"debug++++++ 对手退出"<<std::endl;
        AppModel::getInstance() -> m_pInGameModel -> m_IsMeWin = true;
        
        //设置自己和对手积分
        auto userinfo = AppModel::getInstance()->m_pUserModel->getMyUserInfo();
        userinfo.Score += AppModel::getInstance() ->m_pInGameModel->m_score;
        userinfo.coins += AppModel::getInstance() ->m_pInGameModel->m_coins;
        AppModel::getInstance()->m_pUserModel->setMyUserInfo(userinfo);
        userinfo=AppModel::getInstance()->m_pInGameModel->getOppoUserInfo();
        userinfo.Score -= AppModel::getInstance() ->m_pInGameModel->m_score;
        AppModel::getInstance()->m_pInGameModel->setOppoUserInfo(userinfo);
        
        dispatchEvent(EVT_GAME_OVER);
    }
    else if(sName == REQ_ASK_USER_UNDO_CHESS)
    {
        printf("收到询问对手是否悔棋的消息\n");
        REQAskUndoChess* pAsk = static_cast<REQAskUndoChess*>(pUserData);
        dispatchEvent(ASK_USER_UNDO_CHESS, pAsk);
    }
    else if(sName == ACK_ASK_USER_UNDO_CHESS){
        printf("收到确认对手是否悔棋的消息\n");
        doAckundo(pUserData);
    }
    //聊天
    else if(sName == EVT_CHAT)
    {
        doChat(pUserData);
    }
    else if(sName == ACK_CHAT)
    {
        ACKChatText* pAck = static_cast<ACKChatText*>(pUserData);
        if(pAck){
            string ack_text = pAck-> text();
            auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
            if(pAck -> user_id() != usrinfo.nUserId){
                cout<<"接收方收到了消息:"<<ack_text<<endl;
            }
            else{
                cout<<"我方发送了消息:"<<ack_text<<"成功"<<endl;
            }
            dispatchEvent(ACK_CHAT_MESS,pAck);
        }
    }
    
    //继续游戏
    if(sName == EVT_MSGID_CONTINUE_GAME)
    {
        printf("开始发送继续游戏的请求\n");
        doContinueGame(pUserData);
    }
    else if(sName == EVT_ACK_CONTINUE_GAME)
    {
        printf("继续游戏的请求收到响应\n");
        ACKContinueGame* pAck = static_cast<ACKContinueGame*>(pUserData);
        std::cout<<pAck ->code()<<" "<<pAck ->result_msg()<<std::endl;
        if(pAck -> code()!=0)
        {
            cocos2d::MessageBox(pAck -> result_msg().c_str(), "继续对局失败");
        }
        else if(pAck -> code()==0)
        {
            //弹出一个等待继续的窗口
            dispatchEvent(ACK_WAIT_CONTINUE_GAME,pAck);
        }
    }
    //收到询问是否继续游戏的请求
    if(sName == EVT_REQ_ASK_CONTINUE_GAME)
    {
        printf("收到询问是否继续游戏的请求\n");
        REQAskContinueGame* pReq = static_cast<REQAskContinueGame*>(pUserData);
        dispatchEvent(ASK_CONTINUE_GAME,pReq);
    }
    else if(sName == ACK_REQ_ASK_CONTINUE_GAME)
    {
        printf("发送同意或拒绝的消息\n");
        doSendAgreeContinueGame(pUserData);
    }
    if(sName == ACK_REFUSE_CONTINUE_GAME)
    {
        printf("对方拒绝继续游戏\n");
        ACKRefuseContinueGame* pAck = static_cast<ACKRefuseContinueGame*>(pUserData);
        cocos2d::MessageBox(pAck->result_msg().c_str(), "继续对局失败");
        dispatchEvent(REMOVE_WAIT_ACK_LAYER);
    }

    
    
}

//落子
void WZQInGameController::doChess(void *pUserData)
{
    printf("debug-----------:WZQInGameController发了一条落子或悔棋的消息\n");
    app->m_pConnect->SendChess(pUserData);
}

//退出游戏
void WZQInGameController::doExitGame(void *pUserData)
{
    printf("debug-----------:WZQInGameController发了一条退出游戏消息消息\n");
    app->m_pConnect->SendExitGame(pUserData);
}

void WZQInGameController::doAckundo(void *pUserData){
    printf("debug-----------:WZQInGameController发了一条确认对手悔棋决定的消息\n");
    app->m_pConnect->SendAckUndo(pUserData);
    
}

void WZQInGameController::doChat(void *pUserData){
    printf("debug-----------:WZQInGameController发了一条消息\n");
    app->m_pConnect->SendChat(pUserData);
}



void WZQInGameController::doContinueGame(void *pUserData)
{
    printf("debug-----------:WZQInGameController发了一条继续匹配的消息\n");
    app->m_pConnect->SendContinueGame(pUserData);
    
}

void WZQInGameController::doSendAgreeContinueGame(void *pUserData)
{
    printf("debug-----------:WZQInGameController发了一条同意或拒绝继续匹配的消息\n");
    app->m_pConnect->SendAgreeContinueGame(pUserData);
    
}


//void WZQInGameController::doChat(void *pUserData){
//    printf("debug-----------:WZQInGameController发了一条消息\n");
//    app->m_pConnect->SendChat(pUserData);
//}
