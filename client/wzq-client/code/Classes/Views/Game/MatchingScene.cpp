//
//  MatchingScene.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/31.
//

#include "MatchingScene.hpp"
#include "Core/AppFunc.h"
#include "Core/AppEvent.h"
#include "Core/AppModel.h"
#include "Controllers/WZQLoginController.h"
#include <iostream>
#include <sstream>
#include "SigninLayer.hpp"
#include "WZQDeviceSize.hpp"
#include "Core/AppRes.h"
#include "Core/AppModel.h"
#include "LobbyScene.hpp"
#include "UserInfoLable.hpp"
#include "paohuziProtocol.pb.h"
#include "WinOrLoseLayer.hpp"
#include "AckUndoLayer.hpp"
#include "WaitAckLayer.hpp"
#include "SimpleAudioEngine.h"
#include <iostream>
#include "LoginLayer.h"
#include "PopTips.hpp"
#include "Bubble.hpp"
#include "AppFunc.h"
#include <memory>
#include "AskContinueGameLayer.hpp"
#include "paohuziProtocol.pb.h"


USING_NS_CC;
using namespace std;

Scene* MatchingScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MatchingScene::create();
    scene->addChild(layer);
    return scene;
}

bool MatchingScene::init()
{
    if(!BaseInGameScene::init())
    {
        return false;
    }
    CocosDenshion::SimpleAudioEngine::getInstance() -> preloadEffect(SOUND_WARNING);
    auto vsize = Director::getInstance() -> getVisibleSize();
    
    pMenuItmUndo = MenuButton::create("悔棋", CC_CALLBACK_1( MatchingScene::onUndoClicked, this));
    pMenuItmUndo -> setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    pMenuItmUndo -> setPosition(vsize.width, 0);
    
    
    m_pMenuItmChat = MenuButton::create("", BT_CHAT, CC_CALLBACK_1(MatchingScene::onChatClicked, this));
    m_pMenuItmChat -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    WZQDeviceSize::setPosAtNodeLeftMiddle(m_pMenuItmChat, pMenuItmUndo);
    
    m_pSendBtn = MenuButton::create("", BT_CHAT, CC_CALLBACK_1(MatchingScene::onChatClicked, this));
    m_pSendBtn -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    WZQDeviceSize::setPosAtNodeLeftMiddle(m_pSendBtn, pMenuItmUndo);
    m_pSendBtn -> setVisible(false);
    pMenuItmUndo -> setVisible(AppModel::getInstance() -> m_pInGameModel -> m_IsMe ? false : true);
    

    
    // CC_SAFE_FREE(pMenuItmExitGame);
    pMenuItmExitGame -> setVisible(false);
    pMenuItmExitGame = MenuButton::create("临时退出游戏", CC_CALLBACK_1( MatchingScene::onExitGameClicked, this));
    pMenuItmExitGame -> setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    pMenuItmExitGame -> setPosition(vsize.width, vsize.height);
    
    m_pMenu = Menu::create(pMenuItmExitGame, m_pMenuItmChat, m_pSendBtn, pMenuItmUndo, NULL);
    m_pMenu->setPosition(Vec2::ZERO);
    m_pMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(m_pMenu);
    
    // 注册触摸事件监听器
    m_touchListener = EventListenerTouchOneByOne::create();
    m_touchListener->setSwallowTouches(true);
    m_touchListener->onTouchBegan = CC_CALLBACK_2(MatchingScene::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_touchListener, this);
    
    this -> schedule(schedule_selector(MatchingScene::animUpdate),1,13*13*20+1,1);
    
    auto pBtnSend = MenuButton::create("发送", [this](Ref* pSender){
        log("点击了确认发送按钮\n");
        // 发送信息逻辑
        KKUSERINFO oMyInfo = AppModel::getInstance() -> m_pUserModel -> getMyUserInfo();
        KKUSERINFO oOpponentInfo = AppModel::getInstance() -> m_pInGameModel -> m_gameConf.oOppoInfo;
        string inputText = m_pScrollLayer -> getEditboxText();
        if (inputText.size() > 20 && !isTextSendCorrect(inputText))
        {
            MessageBox("发送的字符不能超过20个或者不合法。","发送失败");
            return;
        }
        ValueMap oData;
        oData.insert(make_pair("user_id" ,Value((int)oMyInfo.nUserId)));
        oData.insert(make_pair("table_id" ,Value(AppModel::getInstance() -> m_pInGameModel -> m_gameConf.iRoomId)));
        oData.insert(make_pair("opp_id" ,Value((int)oOpponentInfo.nUserId)));
        oData.insert(make_pair("text" , Value(inputText)));
        std::cout<<inputText<<std::endl;
        dispatchEvent(EVT_CHAT, &oData);
    });
    
    m_pScrollLayer -> setButton(pBtnSend);
    
    //m_pMenu -> addChild(pBtnSend);
    
    m_pScrollLayer -> setVisible(false);
    m_pSendBtn -> setVisible(false);
    AppModel::getInstance() -> m_pInGameModel -> m_IsGaming = true;
    return true;
}

void MatchingScene::onEnter()
{
    Layer::onEnter();
    ADD_OBSERVER(MatchingScene::Chess, EVT_CLICK_CHESS_BOARD);
    ADD_OBSERVER(MatchingScene::onACKChess, EVT_PLACE_A_STONE_SUCCESS);
    ADD_OBSERVER(MatchingScene::GameOver, EVT_GAME_OVER);
    ADD_OBSERVER(MatchingScene::onACKUndo, ASK_USER_UNDO_CHESS);
    ADD_OBSERVER(MatchingScene::doUndo, EVT_UNDO_CHESS_SUCCESS);
    ADD_OBSERVER(MatchingScene::doUndoWait, EVT_UNDO_CHESS_WAIT);
    ADD_OBSERVER(MatchingScene::doUndoFaill, EVT_UNDO_CHESS_FAIL);
    ADD_OBSERVER(MatchingScene::replaceLogin, EVT_REPLACE_LOGIN);
    ADD_OBSERVER(MatchingScene::showChat, ACK_CHAT_MESS);
    ADD_OBSERVER(MatchingScene::doWaitContinueGame, ACK_WAIT_CONTINUE_GAME);
    ADD_OBSERVER(MatchingScene::onAskContinueGame, ASK_CONTINUE_GAME);
    ADD_OBSERVER(MatchingScene::matchSuccess, EVT_MATCH_SUCESS);
    ADD_OBSERVER(MatchingScene::connFailed, CONN_FAILED);
}
void MatchingScene::connFailed(EventCustom *pEvent)
{
    //连接掉线
    auto reminder = ReminderWindow::create();
    reminder -> setText("连接断线", "回到登录界面！");
    
    auto pComfirm = MenuButton::create("确定", [](Ref* pSender){
        log("回到登录界面");
        auto LoginScene = LoginLayer :: createScene();
        Director::getInstance() -> replaceScene(LoginScene);
    });
    
    reminder -> addButton(pComfirm);
    
    reminder -> initButtons();
    
    addChild(reminder);
}


void MatchingScene::replaceLogin(EventCustom *pEvent)
{
    log("用户异地登录，回到主界面");
    ValueMap oData;
    oData.insert (make_pair("req_msg" ,Value("exit_game")));
    oData.insert (make_pair("exit_table_id" ,Value(AppModel::getInstance() -> m_pInGameModel -> m_gameConf.iRoomId)));
    KKUSERINFO oMyInfo = AppModel::getInstance() -> m_pUserModel -> getMyUserInfo();
    oData.insert (make_pair("exit_user_id" ,Value((int)oMyInfo.nUserId)));

    dispatchEvent(EVT_EXIT_GAME, &oData);
    
    
    auto reminder = ReminderWindow::create();
    reminder -> setText("登录异常", "账号在其他地方已经登录！");
    
    auto pComfirm = MenuButton::create("确定", [](Ref* pSender){
        log("回到大厅界面");
        auto LoginScene = LoginLayer :: createScene();
        Director::getInstance() -> replaceScene(LoginScene);
    });
    
    reminder -> addButton(pComfirm);
    
    reminder -> initButtons();
    
    addChild(reminder);
}




void MatchingScene::onExit()
{
    Layer::onExit();
    removeAllEventListeners();
}

MatchingScene::MatchingScene():m_isTouchEnabled(true)
{
}

MatchingScene::~MatchingScene()
{
}

void MatchingScene::Chess(cocos2d::EventCustom *pEvent)
{
    printf("（匹配模式）玩家已落子\n");
    cocos2d::ValueMap* eventData = static_cast<cocos2d::ValueMap*>(pEvent->getUserData());
    dispatchEvent(EVT_PLACE_A_STONE, eventData);
}

void MatchingScene::onACKChess(cocos2d::EventCustom *pEvent)
{
    if (m_pSign)
        {
            m_pSign -> removeFromParent();
            m_pSign = nullptr;
        }
    printf("（匹配模式）开始渲染棋子\n");
    ACKAction* ack =static_cast<ACKAction*>(pEvent->getUserData());
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    int row = ack->action_board_row();
    int col = ack->action_board_col();
    
    pMenuItmUndo -> setVisible(AppModel::getInstance() -> m_pInGameModel -> m_IsMe ? false : true);
    
    //黑色
    if(ack -> action_id() == AppModel::getInstance() -> m_pInGameModel -> m_gameConf.llBlackId)
    {
        printf("（匹配模式）开始渲染黑色棋子，坐标%d %d\n",row,col);
        cocos2d::Vec2 roundedLos = Vec2(col,row);
        auto pChess = drawChess(roundedLos,true);
        m_ChessVector.pushBack(pChess);
        AppModel::getInstance() -> m_pUserModel -> m_pWaiting -> hide();
    }
    //白色
    if(ack-> action_id() == AppModel::getInstance() -> m_pInGameModel -> m_gameConf.llWhiteId)
    {
        printf("（匹配模式）开始渲染白色棋子，坐标%d %d\n",row,col);
        cocos2d::Vec2 roundedLos = Vec2(col,row);
        auto pChess = drawChess(roundedLos,false);
        m_ChessVector.pushBack(pChess);
        AppModel::getInstance() -> m_pUserModel -> m_pWaiting -> hide();
    }
    if(ack -> action_id() == AppModel::getInstance() -> m_pInGameModel -> m_gameConf.oMyInfo.nUserId)
    {
        delSign();
    }
    
    log("（匹配模式）移动轮到谁的提示");

    auto moveAction = cocos2d::MoveTo::create(0.5f, AppModel::getInstance() -> m_pInGameModel -> m_IsMe ? m_myPos : m_oppoPos);
    
    m_pTipsShow = cocos2d::Sequence::create(moveAction, nullptr);
    
    m_pTips -> runAction(m_pTipsShow);
}

void MatchingScene::animUpdate (float t)
{
    //己方倒计时
    if(AppModel::getInstance() -> m_pInGameModel -> m_IsMe)
    {
        m_my_remain -> setVisible(true);
        m_opp_remain -> setVisible(false);
        m_opp_time = 21;
        m_my_time -=1;
        if(m_my_time <0){
            printf("玩家已超时,将为您随机落子\n");
            //TODO 超时弹窗
            randChess();
            m_my_time = 20;
        }
        if(m_my_time == 5)
        {
            CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect(SOUND_WARNING);
        }
        std::string new_time = to_string(m_my_time);
        m_my_remain -> setString(new_time);
    }
    //对手倒计时
    else{
        m_my_remain -> setVisible(false);
        m_opp_remain -> setVisible(true);
        m_my_time = 21;
        m_opp_time -=1;
        if(m_opp_time <0){
            printf("对方已超时");
            m_opp_time = 20;
        }
        std::string new_time = to_string(m_opp_time);
        m_opp_remain -> setString(new_time);
    }
}

void MatchingScene::GameOver(cocos2d::EventCustom *pEvent)
{
    printf("（匹配模式）客户端生成游戏结束页面\n");

    pauseGame(true);
    gameOverLayer(AppModel::getInstance()->m_pInGameModel->m_IsMeWin ? "你赢了！\n分数：+100":"你输了……\n分数：-100");
    int* remainTime = new int(10);
    auto pContinueBtn = MenuButton::create("继续", [remainTime,this](Ref* pSender){
        unschedule("executeEverySecond");
        delete remainTime;
        log("点击继续游戏按钮。");
        //处理继续游戏的逻辑，发送一条继续游戏的消息
        auto button = dynamic_cast<MenuButton*>(pSender);
        dispatchEvent(EVT_MSGID_CONTINUE_GAME, nullptr);
        m_gameOverLayer -> delButton(button);
        
    });
    m_gameOverLayer -> addButton(pContinueBtn);
    
    m_gameOverLayer -> initButtons();
    
    // 更新按钮文本
    schedule([this, pContinueBtn, remainTime](float dt){
        if (*remainTime >= 0)
        {
            std::string text = "继续(" + to_string((*remainTime)--) +")";
            pContinueBtn -> setText(text);
        }
        // 10秒后执行的程序
        else
        {
            m_gameOverLayer -> delButton(pContinueBtn);
            unschedule("executeEverySecond");
        }
    }, 1.0f, CC_REPEAT_FOREVER, 0.0f, "executeEverySecond");
}

void MatchingScene::onExitGameClicked(cocos2d::Ref *pSender)
{
    log("（匹配模式）点击退出游戏");
    // 发送一个退出游戏的消息
    log("点击退出游戏");
    // 弹出是否退出游戏的弹窗
    auto layer = ReminderWindow::create();
    layer -> setText("温馨提示", "确定是否要退出游戏？");
    // 确定按钮
    auto MenuButtonComfirm = MenuButton::create("取消", [layer](Ref* sender){
        log("取消退出游戏。");
        layer -> removeFromParent();
    });
    
    auto MenuButtonRefuse = MenuButton::create("确认", [=](Ref* sender){
        log("确定退出游戏。");
        if(AppModel::getInstance() -> m_pInGameModel -> m_IsGaming == true)
        {
            //游戏中退出
            ValueMap oData;
            oData.insert (make_pair("req_msg" ,Value("exit_game")));
            oData.insert (make_pair("exit_table_id" ,Value(AppModel::getInstance() -> m_pInGameModel -> m_gameConf.iRoomId)));
            KKUSERINFO oMyInfo = AppModel::getInstance() -> m_pUserModel -> getMyUserInfo();
            KKUSERINFO OppoMyInfo = AppModel::getInstance() -> m_pInGameModel -> getOppoUserInfo();
            oData.insert (make_pair("exit_user_id" ,Value((int)oMyInfo.nUserId)));
            oData.insert (make_pair("enemy_user_id" ,Value((int)OppoMyInfo.nUserId)));
            dispatchEvent(EVT_EXIT_GAME, &oData);
            auto pLobbyScene = LobbyScene::createScene();
            Director::getInstance() -> replaceScene(pLobbyScene);
        }
        else
        {
            //游戏结束后退出
            ValueMap oData;
            oData.insert (make_pair("req_msg" ,Value("exit_room")));
            oData.insert (make_pair("exit_table_id" ,Value(AppModel::getInstance() -> m_pInGameModel -> m_gameConf.iRoomId)));
            KKUSERINFO OppoMyInfo = AppModel::getInstance() -> m_pInGameModel -> getOppoUserInfo();
            KKUSERINFO oMyInfo = AppModel::getInstance() -> m_pUserModel -> getMyUserInfo();
            oData.insert (make_pair("exit_user_id" ,Value((int)oMyInfo.nUserId)));
            oData.insert (make_pair("enemy_user_id" ,Value((int)OppoMyInfo.nUserId)));
            dispatchEvent(EVT_EXIT_GAME, &oData);
            auto pLobbyScene = LobbyScene::createScene();
            Director::getInstance() -> replaceScene(pLobbyScene);
        }
    });
    
    layer -> addButton(MenuButtonComfirm);
    layer -> addButton(MenuButtonRefuse);
    
    auto pMenu = Menu::create(MenuButtonComfirm,MenuButtonRefuse, NULL);
    
    pMenu->setPosition(Vec2::ZERO);
    pMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    

    addChild(layer);
    layer -> addChild(pMenu);
}

bool MatchingScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (!m_isTouchEnabled)
        return true;

    std::cout<<"（匹配模式）点击位置：("<<touch -> getLocation().x<<", "<<touch->getLocation().y<<")"<<std::endl;
    
    Vec2 touchLos = touch -> getLocation();
    
    if (m_LB.x < touchLos.x && touchLos.x < m_RT.x && m_LB.y < touchLos.y && touchLos.y < m_RT.y)
        {
            std::cout<< "（匹配模式）点击棋盘相对位置：("
                     <<(touchLos.x - m_LB.x)/(m_RT.x - m_LB.x)
                     <<","
                     <<(touchLos.y - m_LB.y)/(m_RT.y - m_LB.y)
                     <<")"
                     <<std::endl;
            Vec2 reCoord = toRelativeCoord(Vec2((touchLos.x - m_LB.x) / (m_RT.x - m_LB.x), (touchLos.y - m_LB.y)/(m_RT.y - m_LB.y)));
            
            std::cout<< reCoord.x << m_Choosed.x
                     << reCoord.y << m_Choosed.y << std::endl;
            
            if (m_isComfirm &&
                (int)reCoord.x == (int)m_Choosed.x &&
                (int)reCoord.y == (int)m_Choosed.y)
            {
                m_isComfirm = false;
                putChess(Vec2((touchLos.x - m_LB.x)/(m_RT.x - m_LB.x), (touchLos.y - m_LB.y)/(m_RT.y - m_LB.y)));
            }
            else
            {
                putSign(Vec2((touchLos.x - m_LB.x)/(m_RT.x - m_LB.x), (touchLos.y - m_LB.y)/(m_RT.y - m_LB.y)));
                m_isComfirm = true;
            }
        }
    return true;
}

void MatchingScene::putSign(cocos2d::Vec2 location)
{
    if (m_pSign)
    {
        m_pSign -> removeFromParent();
        m_pSign = nullptr;
    }
    Vec2 roundedLos(int(round((location * 12).x)), int(round((location * 12).y)));
    std::cout<< "（匹配模式）确定棋盘坐标为：("<<roundedLos.x<<", "<<roundedLos.y<<")"<<std::endl;
    
    if(AppModel::getInstance() -> m_pInGameModel -> m_IsMe == false)
    {
        std::cout<<"（匹配模式）没到你的回合"<<std::endl;
        return;
    }

    if (m_isAlreadyPut[roundedLos])
    {
        log("（匹配模式）已经放置棋子。");
        return;
    }
    drawSign(roundedLos);
    m_Choosed = roundedLos;
    
}

void MatchingScene::putChess(cocos2d::Vec2 location)
{
    Vec2 roundedLos(int(round((location * 12).x)), int(round((location * 12).y)));
    std::cout<< "(匹配模式)确定棋盘坐标为：("<<roundedLos.x<<", "<<roundedLos.y<<")"<<std::endl;
    if(m_pSign)
    {
        m_pSign -> removeFromParent();
        m_pSign = nullptr;
    }
    if(AppModel::getInstance() -> m_pInGameModel -> m_IsMe == false)
    {
        std::cout<<"（匹配模式）没到你的回合"<<std::endl;
        return;
    }

    if (m_isAlreadyPut[roundedLos])
    {
        log("（匹配模式）已经放置棋子, 请重新放置.");
        return;
    }
    AppModel::getInstance() -> m_pUserModel -> m_pWaiting -> show(2, "等待服务器响应……");
    ValueMap oData;
    oData.insert(make_pair("req_msg" ,Value("put_chess")));
    oData.insert(make_pair("table_id" ,Value(AppModel::getInstance() -> m_pInGameModel -> m_gameConf.iRoomId)));
    KKUSERINFO oMyInfo = AppModel::getInstance() -> m_pUserModel -> getMyUserInfo();
    oData.insert(make_pair("action_id" ,Value((int)oMyInfo.nUserId)));
    oData.insert(make_pair("action_board_row" ,Value(roundedLos.x)));
    oData.insert(make_pair("action_board_col" , Value(roundedLos.y)));
    oData.insert(make_pair("remain_time" , Value((int)0)));
    dispatchEvent(EVT_PLACE_A_STONE, &oData);
}

void MatchingScene::randChess()
{
    cocos2d::Vec2 randLos;
    for(int x=0;x<13;x++)
    {
        for(int y=0;y<13;y++){
            randLos = Vec2(x,y);
            if(m_isAlreadyPut[randLos] == false){
                randLos = Vec2((x * m_diff)/(m_RT.x - m_LB.x),( y * m_diff)/(m_RT.y - m_LB.y));
                putChess(randLos);
                return;
            }
        }
    }
}

void MatchingScene::onUndoClicked(cocos2d::Ref *pSender)
{
    log("（匹配模式）点击了悔棋按钮");
    if(AppModel::getInstance()->m_pInGameModel-> m_chessNum <=0)
    {
        log("（匹配模式）当前这个玩家没有下过棋，无法悔棋");
        return;
    }
    if(AppModel::getInstance() -> m_pInGameModel -> m_IsMe == true)
    {
        log("（匹配模式）无法在自己回合悔棋，悔棋失败");
        return;
    }
    
    // 处理悔棋逻辑
    ValueMap oData;
    oData.insert(make_pair("req_msg" ,Value("undo_chess")));
    oData.insert(make_pair("table_id" ,Value(AppModel::getInstance() -> m_pInGameModel -> m_gameConf.iRoomId)));
    KKUSERINFO oMyInfo = AppModel::getInstance() -> m_pUserModel -> getMyUserInfo();
    oData.insert(make_pair("action_id" ,Value((int)oMyInfo.nUserId)));
    oData.insert(make_pair("action_board_row" ,Value(-1)));
    oData.insert(make_pair("action_board_col" , Value(-1)));
    oData.insert(make_pair("remain_time" , Value((int)m_opp_time)));
    dispatchEvent(EVT_PLACE_A_STONE, &oData);
}

void MatchingScene::onACKUndo(EventCustom *pEvent)
{
    printf("void MatchingScene::onACKUndo(EventCustom *pEvent)\n");
    unschedule(schedule_selector(MatchingScene::animUpdate));
    REQAskUndoChess* ask =static_cast<REQAskUndoChess*>(pEvent->getUserData());
    auto p_ackundoLayer = AckUndoLayer::create();
    addChild(p_ackundoLayer);
}

void MatchingScene::doUndo(EventCustom* pEvent)
{
    PopTips::pop(this,"悔棋成功");
    MatchingScene::resetTimer(pEvent);
    printf("（匹配模式）悔棋成功即将移除棋子");
    m_ChessVector.back() -> removeFromParent();
    m_ChessVector.popBack();
    ACKAction* ack =static_cast<ACKAction*>(pEvent->getUserData());
    int row = ack->action_board_row();
    int col = ack->action_board_col();
    cocos2d::Vec2 roundedLos = Vec2(col,row);
    m_isAlreadyPut[roundedLos] = false;
    
    log("（匹配模式）移动轮到谁的提示");

    auto moveAction = cocos2d::MoveTo::create(0.5f, AppModel::getInstance() -> m_pInGameModel -> m_IsMe ? m_myPos : m_oppoPos);

    m_pTipsShow = cocos2d::Sequence::create(moveAction, nullptr);
    
    m_pTips -> runAction(m_pTipsShow);
    
}

void MatchingScene::resetTimer(EventCustom* pEvent)
{
    printf("回复定时器\n");
    this -> schedule(schedule_selector(MatchingScene::animUpdate),1,13*13*20+1,1);
}
void MatchingScene::doUndoFaill(EventCustom* pEvent)
{
    PopTips::pop(this,"悔棋失败");
    MatchingScene::resetTimer(pEvent);
}



void MatchingScene::doUndoWait(EventCustom* pEvent)
{
    unschedule(schedule_selector(MatchingScene::animUpdate));
    auto p_requndoLayer = WaitAckLayer::create();
    addChild(p_requndoLayer);
    
}

void MatchingScene::pauseGame(bool flag)
{
    if (flag)
    {
        m_isTouchEnabled = false;
        pMenuItmExitGame -> setVisible(false);
        pMenuItmUndo -> setVisible(false);
        unschedule(schedule_selector(MatchingScene::animUpdate));
        m_pMenuItmChat -> setVisible(false);
    }
    else
    {
        m_isTouchEnabled = true;
        pMenuItmExitGame -> setVisible(true);
        pMenuItmUndo -> setVisible(true);
        this -> schedule(schedule_selector(MatchingScene::animUpdate),1,13*13*20+1,1);
        m_pMenuItmChat -> setVisible(true);
    }
}

void MatchingScene::onChatClicked(Ref *pSender)
{
    log("点击聊天弹窗");
    m_touchListener -> setEnabled(false);
    auto pButton = dynamic_cast<MenuButton*>(pSender);

    if (!isOpenChatBox)
    {
        isOpenChatBox = true;
        m_pScrollLayer -> setVisible(true);
        m_pSendBtn -> setVisible(true);
    }
    else
    {
        isOpenChatBox = false;
        m_pScrollLayer -> setVisible(false);
        m_pSendBtn -> setVisible(false);
    }

}

void MatchingScene::onSendChatClicked(Ref* pSender)
{

}

void MatchingScene::showChat(cocos2d::EventCustom *pEvent)
{
    auto messageInfo = static_cast<ACKChatText*>(pEvent -> getUserData());
    auto name = AppModel::getInstance()->m_pInGameModel ->getUserInfo(messageInfo -> user_id()).sUser;
    auto oppoLocation = Vec2(m_pOppoInfo -> getPosition().x +
                             m_pOppoInfo -> getContentSize().width *
                             m_pOppoInfo -> getScaleX() / 2 ,
                             m_pOppoInfo -> getPosition().y -
                             m_pOppoInfo -> getContentSize().height *
                             m_pOppoInfo -> getScaleY());
    auto MyLocation = Vec2(m_pMyInfo -> getPosition().x +
                           m_pMyInfo -> getContentSize().width *
                           m_pMyInfo -> getScaleX() / 2 ,
                           m_pMyInfo -> getPosition().y +
                           m_pMyInfo -> getContentSize().height *
                           m_pMyInfo -> getScaleY());
    if (messageInfo -> user_id() == AppModel::getInstance() -> m_pInGameModel -> m_gameConf.oMyInfo.nUserId)
        Bubble::pop(this, messageInfo -> text(), MyLocation);
    else
        Bubble::pop(this, messageInfo -> text(), oppoLocation, false);
    
    std::string chatText = AppModel::getInstance() -> m_pInGameModel -> getUserInfo(messageInfo -> user_id()).sUser + "：" + messageInfo -> text();
    
    m_pScrollLayer -> addText(chatText);
    
}


void MatchingScene::doWaitContinueGame(EventCustom* pEvent)
{
    auto p_requndoLayer = WaitAckLayer::create();
    addChild(p_requndoLayer);
}

void MatchingScene::onAskContinueGame(EventCustom *pEvent)
{
    printf("void MatchingScene::onAskContinueGame(EventCustom *pEvent)\n");
    
    REQAskContinueGame* pReq = static_cast<REQAskContinueGame*>(pEvent->getUserData());

    auto p_askLayer = AskContinueGameLayer::create();
    p_askLayer -> m_enemy_id =  pReq -> enemy_user_id();
    addChild(p_askLayer);
    
    
}


void MatchingScene::matchSuccess(EventCustom* pEvent)
{
    printf("处理匹配成功的逻辑\n");
    // 跳转到游戏中
    auto inGame = MatchingScene::createScene();
    // auto inGame = InGameScene::createScene();
    Director::getInstance() -> replaceScene(inGame);
}


void MatchingScene::gameOverLayer(std::string tips)
{
    m_gameOverLayer = ReminderWindow::create();
    m_gameOverLayer-> setText("游戏结束",tips);
    auto pComfirm = MenuButton::create("退出", [this](Ref* pSender){
        log("游戏结束，退出。");
        
        //游戏结束后退出
        ValueMap oData;
        oData.insert (make_pair("req_msg" ,Value("exit_room")));
        oData.insert (make_pair("exit_table_id" ,Value(AppModel::getInstance() -> m_pInGameModel -> m_gameConf.iRoomId)));
        KKUSERINFO OppoMyInfo = AppModel::getInstance() -> m_pInGameModel -> getOppoUserInfo();
        KKUSERINFO oMyInfo = AppModel::getInstance() -> m_pUserModel -> getMyUserInfo();
        oData.insert (make_pair("exit_user_id" ,Value((int)oMyInfo.nUserId)));
        oData.insert (make_pair("enemy_user_id" ,Value((int)OppoMyInfo.nUserId)));
        dispatchEvent(EVT_EXIT_GAME, &oData);
        auto pLobbyScene = LobbyScene::createScene();
        Director::getInstance() -> replaceScene(pLobbyScene);
        
        auto pLobby = LobbyScene::createScene();
        Director::getInstance() -> replaceScene(pLobby);
    });
    
    
    auto pBack = MenuButton::create("查看残局", [this](Ref* pSender){
        auto pGoBack = MenuButton::create("返回", [this](Ref* sender){
            m_gameOverLayer -> setVisible(true);
            auto* menuButton = dynamic_cast<MenuButton*>(sender);
            menuButton -> setVisible(false);
        });
        pGoBack -> setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        pGoBack -> setPosition(WZQDeviceSize::getViewCenterPos().x * 2, WZQDeviceSize::getViewCenterPos().y * 2);
        
        auto pMenu = Menu::create(pGoBack, NULL);
        pMenu -> setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        pMenu -> setPosition(Vec2::ZERO);
        
        this -> addChild(pMenu);
        
        log("游戏结束，查看残局。");
        m_gameOverLayer -> setVisible(false);
    });
    
    m_gameOverLayer -> addButton(pComfirm);
    m_gameOverLayer -> addButton(pBack);

    m_gameOverLayer -> initButtons();
    addChild(m_gameOverLayer);
}
