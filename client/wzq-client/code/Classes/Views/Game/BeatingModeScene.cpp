//
//  BeatingModeScene.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/31.
//

#include "BeatingModeScene.hpp"
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
#include "SimpleAudioEngine.h"
#include <iostream>
#include "LoginLayer.h"
USING_NS_CC;
using namespace std;

Scene* BeatingModeScene::createScene()
{
    auto scene = Scene::create();
    auto layer = BeatingModeScene::create();
    scene->addChild(layer);
    return scene;
}

bool BeatingModeScene::init()
{
    if(!BaseInGameScene::init())
    {
        return false;
    }
    
    m_pScrollLayer -> setVisible(false);
    CocosDenshion::SimpleAudioEngine::getInstance() -> preloadEffect(SOUND_WARNING);
    auto vsize = Director::getInstance() -> getVisibleSize();
    
    m_board.resize(m_chessBoardSize + 1,std::vector<int>(m_chessBoardSize + 1,0));
    
    pMenuItmUndo = MenuButton::create("悔棋", CC_CALLBACK_1( BeatingModeScene::onUndoClicked, this));
    pMenuItmUndo -> setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    pMenuItmUndo -> setPosition(vsize.width, 0);
    
    m_pMenu = Menu::create(pMenuItmUndo, NULL);
    m_pMenu->setPosition(Vec2::ZERO);
    m_pMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(m_pMenu);
    
    // 注册触摸事件监听器
    m_touchListener = EventListenerTouchOneByOne::create();
    m_touchListener->setSwallowTouches(true);
    m_touchListener->onTouchBegan = CC_CALLBACK_2(BeatingModeScene::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_touchListener, this);
    
    this -> schedule(schedule_selector(BeatingModeScene::animUpdate), 1, 13 * 13 * 20 + 1, 1);
    
    pauseGame(false);
    m_pOppoInfo->delScoreLabel();
    m_pMyInfo->delScoreLabel();
    AppModel::getInstance() -> m_pInGameModel -> m_IsGaming = true;
    return true;
}

void BeatingModeScene::onEnter()
{
    Layer::onEnter();
    ADD_OBSERVER(BeatingModeScene::onACKChess, EVT_PLACE_A_STONE_SUCCESS);
    ADD_OBSERVER(BeatingModeScene::doUndo, EVT_UNDO_CHESS_SUCCESS);
    ADD_OBSERVER(BeatingModeScene::replaceLogin, EVT_REPLACE_LOGIN);
    ADD_OBSERVER(BeatingModeScene::connFailed, CONN_FAILED);
}
void BeatingModeScene::connFailed(EventCustom *pEvent)
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



void BeatingModeScene::replaceLogin(EventCustom *pEvent)
{
    log("用户异地登录，回到主界面");
    
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
void BeatingModeScene::onExit()
{
    Layer::onExit();
}

BeatingModeScene::BeatingModeScene()
{

}


BeatingModeScene::~BeatingModeScene()
{
}

bool BeatingModeScene::five(std::vector<std::vector<int>>&board,int row ,int col , int row_off , int col_off , int color)
{
  int count = 1 ;
  int search_row = row + row_off ;
  int search_col = col + col_off ;
  //检索前一半
  while(search_row >= 0 && search_row < m_chessBoardSize + 1 &&
      search_col >=0 && search_col < m_chessBoardSize + 1 &&
      board[search_row][search_col] == color)
  {
    count++;
    search_row += row_off;
    search_col += col_off;
  }
  
  //检索后一半
  search_row = row - row_off ;
  search_col = col - col_off ;
  while(search_row >= 0 && search_row < m_chessBoardSize + 1 &&
      search_col >=0 && search_col < m_chessBoardSize + 1 &&
      board[search_row][search_col] == color)
  {
    count++;
    search_row -= row_off;
    search_col -= col_off;
  }
  return (count >= 5);
}


int BeatingModeScene::CheckWin(std::vector<std::vector<int>>&board,int row,int col,int color)
{
    //任意一方向达到五子
    if (five(board,row,col,1,0,color) ||
    five(board,row,col,0,1,color)  ||
    five(board,row,col,-1,-1,color) ||
    five(board,row,col,-1,1,color) )
    {
      int winner_id = (color == WHITE_CHESS ? WHITE_CHESS : BLACK_CHESS);
      return winner_id;
    }
    return 0;
}





void BeatingModeScene::onACKChess(cocos2d::EventCustom *pEvent)
{
    delSign();
    printf("(双人模式)开始渲染棋子\n");
    ValueMap* ack =static_cast<ValueMap*>(pEvent->getUserData());
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    int row = ack -> at("action_board_row").asInt();
    int col = ack -> at("action_board_col").asInt();
    unsigned long long action_id = AppModel::getInstance()->m_pInGameModel -> m_gameConf.llActionId;
    AppModel::getInstance() -> m_pInGameModel -> m_gameConf.llActionId =
    AppModel::getInstance() -> m_pInGameModel -> m_gameConf.oMyInfo.nUserId == action_id ?
    AppModel::getInstance() -> m_pInGameModel -> m_gameConf.oOppoInfo.nUserId :
    AppModel::getInstance() -> m_pInGameModel -> m_gameConf.oMyInfo.nUserId;
    
    // 交换isme
    AppModel::getInstance() -> m_pInGameModel -> m_IsMe =
    AppModel::getInstance() -> m_pInGameModel -> m_IsMe ? false : true;
    
    //黑色
    if(action_id == AppModel::getInstance() -> m_pInGameModel -> m_gameConf.llBlackId)
    {
        printf("(双人模式)开始渲染黑色棋子，坐标%d %d\n",row,col);
        cocos2d::Vec2 roundedLos = Vec2(col,row);
        auto pChess = drawChess(roundedLos,true);
        m_pChess[roundedLos] = pChess;
        m_board[col][row] = BLACK_CHESS;
        m_blackPlayerActions.push_back({col,row});
        int winner_id = CheckWin(m_board,col,row,BLACK_CHESS);
        if(winner_id ==BLACK_CHESS)
        {
            pauseGame(true);
            gameOverLayer("玩家1获胜！");
        }

    }
    //白色
    if(action_id == AppModel::getInstance() -> m_pInGameModel -> m_gameConf.llWhiteId)
    {
        printf("(双人模式)开始渲染白色棋子，坐标%d %d\n",row,col);
        cocos2d::Vec2 roundedLos = Vec2(col,row);
        auto pChess = drawChess(roundedLos,false);
        m_pChess[roundedLos] = pChess;
        m_board[col][row] = WHITE_CHESS;
        m_whitePlayerActions.push_back({col,row});
        int winner_id = CheckWin(m_board,col,row,WHITE_CHESS);
        if(winner_id ==WHITE_CHESS)
        {
            pauseGame(true);
            gameOverLayer("玩家2获胜！");
        }
    }
    //判断平局
    if((m_whitePlayerActions.size() + m_blackPlayerActions.size()) == (m_board[0].size() * m_board.size()))
    {
        pauseGame(true);
        gameOverLayer("双方平局！");
    }
    
    log("(双人模式)移动轮到谁的提示");

    auto moveAction = cocos2d::MoveTo::create(0.5f, AppModel::getInstance() -> m_pInGameModel -> m_IsMe ? m_myPos : m_oppoPos);

    m_pTipsShow = cocos2d::Sequence::create(moveAction, nullptr);
    
    m_pTips -> runAction(m_pTipsShow);
}

bool BeatingModeScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{

    std::cout<<"(双人模式)点击位置：("<<touch -> getLocation().x<<", "<<touch->getLocation().y<<")"<<std::endl;
    
    Vec2 touchLos = touch -> getLocation();
    
    if (m_LB.x < touchLos.x && touchLos.x < m_RT.x && m_LB.y < touchLos.y && touchLos.y < m_RT.y)
        {
            std::cout<< "(双人模式)点击棋盘相对位置：("
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

void BeatingModeScene::putSign(cocos2d::Vec2 location)
{
    delSign();
    Vec2 roundedLos(int(round((location * m_chessBoardSize).x)), int(round((location * m_chessBoardSize).y)));
    std::cout<< "(双人模式)确定棋盘坐标为：("<<roundedLos.x<<", "<<roundedLos.y<<")"<<std::endl;

    if (m_isAlreadyPut[roundedLos])
    {
        log("(双人模式)已经放置棋子。");
        return;
    }
    drawSign(roundedLos);
    m_Choosed = roundedLos;
    
}
void BeatingModeScene::putChess(cocos2d::Vec2 location) {
    Vec2 roundedLos(int(round((location * m_chessBoardSize).x)), int(round((location * m_chessBoardSize).y)));
    std::cout<< "(双人模式)确定棋盘坐标为：("<<roundedLos.x<<", "<<roundedLos.y<<")"<<std::endl;
    if(m_pSign)
    {
        m_pSign -> removeFromParent();
        m_pSign = nullptr;
    }
    

    if (m_isAlreadyPut[roundedLos])
    {
        log("(双人模式)已经放置棋子, 请重新放置.");
        return;
    }
    AppModel::getInstance() -> m_pInGameModel -> m_chessNum++;
    ValueMap oData;
    oData.insert(make_pair("req_msg" ,Value("put_chess")));
    oData.insert(make_pair("table_id" ,Value(AppModel::getInstance() -> m_pInGameModel -> m_gameConf.iRoomId)));
    KKUSERINFO oMyInfo = AppModel::getInstance() -> m_pUserModel -> getMyUserInfo();
    oData.insert(make_pair("action_id" ,Value((int)oMyInfo.nUserId)));
    oData.insert(make_pair("action_board_row" ,Value(roundedLos.y)));
    oData.insert(make_pair("action_board_col" , Value(roundedLos.x)));
    dispatchEvent(EVT_PLACE_A_STONE_SUCCESS, &oData);
}

//倒计时每过一秒刷新一下
//超时随机落子
void BeatingModeScene::randChess(){
    cocos2d::Vec2 randLos;
    log("(双人模式)开始随机放子");
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
void BeatingModeScene::animUpdate (float t){
    //己方倒计时
    if(AppModel::getInstance() -> m_pInGameModel -> m_IsMe)
    {
        m_my_remain -> setVisible(true);
        m_opp_remain -> setVisible(false);
        m_opp_time = 21;
        m_my_time -=1;
        if(m_my_time == 5)
        {
            CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect(SOUND_WARNING);
        }
        if(m_my_time <0){
            printf("玩家已超时,将为您随机落子");
            //TODO 超时弹窗
            randChess();
            m_my_time = 20;
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
            randChess();
            m_opp_time = 20;
        }
        if(m_opp_time == 5)
        {
            CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect(SOUND_WARNING);
        }
        std::string new_time = to_string(m_opp_time);
        m_opp_remain -> setString(new_time);
    }
}


void BeatingModeScene::onUndoClicked(cocos2d::Ref *pSender) {
    delSign();
    log("(双人模式)点击了悔棋按钮");
    if(AppModel::getInstance()->m_pInGameModel-> m_chessNum <=0)
    {
        log("(双人模式)当前这个玩家没有下过棋，无法悔棋");
        return;
    }
//    if(AppModel::getInstance() -> m_pInGameModel -> m_IsMe == true)
//    {
//        log("(双人模式)无法在自己回合悔棋，悔棋失败");
//        return;
//    }
    // 处理悔棋逻辑
    if (m_pChess.empty())
    {
        log("棋盘上没有棋子，无法悔棋！");
        return;
    }
    
    EventCustom* pEvent;
    doUndo(pEvent);

}

void BeatingModeScene::doUndo(EventCustom* pEvent){
    AppModel::getInstance() -> m_pInGameModel -> m_chessNum--;
    printf("(双人模式)悔棋成功即将移除棋子");
    
    auto& action_id = AppModel::getInstance() -> m_pInGameModel -> m_gameConf.llActionId;
    int undo_col = 0;
    int undo_row = 0;
    if(action_id == AppModel::getInstance() -> m_pInGameModel -> m_gameConf.oMyInfo.nUserId)
    {
        //白色方悔棋
        auto pair = m_whitePlayerActions.back();
        m_whitePlayerActions.pop_back();
        undo_col = pair.first;
        undo_row = pair.second;
        m_board[undo_col][undo_row] = 0;
        drawSign(Vec2(undo_col, undo_row));
    }
    else
    {
        //黑色方悔棋
        auto pair = m_blackPlayerActions.back();
        m_blackPlayerActions.pop_back();
        undo_col = pair.first;
        undo_row = pair.second;
        m_board[undo_col][undo_row] = 0;
        drawSign(Vec2(undo_col, undo_row));
    }
    
    Vec2 position =Vec2(undo_col,undo_row);
    
    if(m_pChess.find(position) == m_pChess.end())
    {
        printf("error 找不到position: col %d, row %d\n",undo_col,undo_row);
        return;
    }
    auto lastChess = m_pChess[position];
    lastChess->removeFromParent();

//    m_ChessVector.back() -> removeFromParent();
//    m_ChessVector.popBack();
//    ACKAction* ack = static_cast<ACKAction*>(pEvent->getUserData());
//    int row = ack->action_board_row();
//    int col = ack->action_board_col();
//    cocos2d::Vec2 roundedLos = Vec2(col,row);
    m_isAlreadyPut[position] = false;
    m_pChess.erase(position);
    
    log("(双人模式)移动轮到谁的提示");
    
    // 交换isme
    AppModel::getInstance() -> m_pInGameModel -> m_IsMe =
    AppModel::getInstance() -> m_pInGameModel -> m_IsMe ? false : true;

    auto moveAction = cocos2d::MoveTo::create(0.5f, AppModel::getInstance() -> m_pInGameModel -> m_IsMe ? m_myPos : m_oppoPos);
    
    action_id = AppModel::getInstance()->m_pInGameModel -> m_gameConf.llActionId;
    AppModel::getInstance() -> m_pInGameModel -> m_gameConf.llActionId =
    AppModel::getInstance() -> m_pInGameModel -> m_gameConf.oMyInfo.nUserId == action_id ?
    AppModel::getInstance() -> m_pInGameModel -> m_gameConf.oOppoInfo.nUserId :
    AppModel::getInstance() -> m_pInGameModel -> m_gameConf.oMyInfo.nUserId;

    m_pTipsShow = cocos2d::Sequence::create(moveAction, nullptr);
    
    m_pTips -> runAction(m_pTipsShow);
    
}

void BeatingModeScene::pauseGame(bool flag)
{
    if (flag)
    {
        pMenuItmExitGame -> setVisible(false);
        pMenuItmUndo -> setVisible(false);
        this->pauseSchedulerAndActions();
        m_touchListener -> setEnabled(false);
    }
    else
    {
        pMenuItmExitGame -> setVisible(true);
        pMenuItmUndo -> setVisible(true);
        this->resumeSchedulerAndActions();
        m_touchListener -> setEnabled(true);
    }
}



