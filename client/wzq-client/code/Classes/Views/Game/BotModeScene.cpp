//
//  BotModeScene.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/1.
//

#include "BotModeScene.hpp"
#include "Core/AppFunc.h"
#include "Core/AppEvent.h"
#include "Core/AppModel.h"
#include "Controllers/WZQLoginController.h"
#include <iostream>
#include <sstream>
#include "WZQDeviceSize.hpp"
#include "Core/AppRes.h"
#include "Core/AppModel.h"
#include "UserInfoLable.hpp"
#include "paohuziProtocol.pb.h"
#include "AckUndoLayer.hpp"
#include "SimpleAudioEngine.h"
#include <iostream>
#include "LoginLayer.h"
#include "WZQBotGameModel.hpp"
USING_NS_CC;
using namespace std;

Scene* BotModeScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = BotModeScene::create();
    scene->addChild(layer);
    return scene;
}

bool BotModeScene::init()
{

    if(!BaseInGameScene::init())
    {
        return false;
    }

    m_robot = Robot(m_chessBoardSize + 1);
    
    if(AppModel::getInstance()->m_pBotGameModel->m_balck_id == AppModel::getInstance()->m_pBotGameModel->m_robotid)
    {
        log("机器人是黑棋");
        m_robot.m_color_id = BLACK_CHESS;
    }
    else
    {
        log("机器人是白棋");
        m_robot.m_color_id = WHITE_CHESS;
    }
    
    if(AppModel::getInstance()->m_pBotGameModel->m_modeChoosed == KEY_EASY)
    {
        m_robot.m_difficulty = KEY_EASY;
    }
    else if(AppModel::getInstance()->m_pBotGameModel->m_modeChoosed == KEY_NORMAL)
    {
        m_robot.m_difficulty = KEY_NORMAL;
    }
    else
    {
        m_robot.m_difficulty = KEY_HARD;
    }
    
    
    
    m_pScrollLayer -> setVisible(false);
    
    CocosDenshion::SimpleAudioEngine::getInstance() -> preloadEffect(SOUND_WARNING);
    auto vsize = Director::getInstance() -> getVisibleSize();

    m_board.resize(m_chessBoardSize + 1,std::vector<int>(m_chessBoardSize + 1,0));

    pMenuItmUndo = MenuButton::create("悔棋", CC_CALLBACK_1( BotModeScene::onUndoClicked, this));
    pMenuItmUndo -> setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    pMenuItmUndo -> setPosition(vsize.width, 0);
    
    m_pMenu = Menu::create(pMenuItmUndo, NULL);
    m_pMenu->setPosition(Vec2::ZERO);
    m_pMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(m_pMenu);

    // 注册触摸事件监听器
    m_touchListener = EventListenerTouchOneByOne::create();
    m_touchListener->setSwallowTouches(true);
    m_touchListener->onTouchBegan = CC_CALLBACK_2(BotModeScene::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_touchListener, this);


    pauseGame(false);
    
    m_pOppoInfo->delScoreLabel();
    m_pMyInfo->delScoreLabel();
    AppModel::getInstance() -> m_pInGameModel -> m_IsGaming = true;
    return true;
}

void BotModeScene::onEnter()
{
    Layer::onEnter();
    ADD_OBSERVER(BotModeScene::onACKChess, EVT_PLACE_A_STONE_SUCCESS);
    ADD_OBSERVER(BotModeScene::doUndo, EVT_UNDO_CHESS_SUCCESS);
    
    ADD_OBSERVER(BotModeScene::replaceLogin, EVT_REPLACE_LOGIN);
    ADD_OBSERVER(BotModeScene::connFailed, CONN_FAILED);
    if(AppModel::getInstance()->m_pBotGameModel->m_curid == AppModel::getInstance()->m_pBotGameModel->m_robotid)
    {
        log("机器人开始下棋");
        //机器人下棋
        randChess();
    }
}
void BotModeScene::connFailed(EventCustom *pEvent)
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

void BotModeScene::replaceLogin(EventCustom *pEvent)
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
void BotModeScene::onExit()
{
    Layer::onExit();
}

BotModeScene::BotModeScene()
{
}


BotModeScene::~BotModeScene()
{
}

bool BotModeScene::five(std::vector<std::vector<int>>&board,int row ,int col , int row_off , int col_off , int color)
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


int BotModeScene::CheckWin(std::vector<std::vector<int>>&board,int row,int col,int color)
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





void BotModeScene::onACKChess(cocos2d::EventCustom *pEvent)
{
    delSign();
    printf("(人机模式)开始渲染棋子\n");
    ValueMap* ack =static_cast<ValueMap*>(pEvent->getUserData());
    int row = ack -> at("action_board_row").asInt();
    int col = ack -> at("action_board_col").asInt();
    
    
    int action_id = AppModel::getInstance()->m_pBotGameModel-> m_curid;
    log("action_id:%d\n",action_id);
    
    
    AppModel::getInstance()->m_pBotGameModel-> m_curid =(AppModel::getInstance()->m_pBotGameModel->m_curid == AppModel::getInstance()->m_pBotGameModel->m_userid ? AppModel::getInstance()->m_pBotGameModel->m_robotid : AppModel::getInstance()->m_pBotGameModel->m_userid);
    
    AppModel::getInstance()->m_pBotGameModel->m_is_user = (AppModel::getInstance()->m_pBotGameModel->m_is_user == true ? false : true);
    


    //黑色
    if(action_id == AppModel::getInstance()->m_pBotGameModel->m_balck_id)
    {
        printf("(人机模式)开始渲染黑色棋子，坐标%d %d\n",row,col);
        cocos2d::Vec2 roundedLos = Vec2(col,row);
        auto pChess = drawChess(roundedLos,true);
        m_pChess[roundedLos] = pChess;
        m_board[col][row] = BLACK_CHESS;
        m_blackPlayerActions.push_back({col,row});
        int winner_id = CheckWin(m_board,col,row,BLACK_CHESS);
        if(winner_id ==BLACK_CHESS)
        {
            pMenuItmUndo -> setVisible(false);
            pauseGame(true);
            std::string game_over_message;
            if(action_id ==AppModel::getInstance()->m_pBotGameModel->m_userid)
            {
                game_over_message = "玩家获胜!";
            }
            else
            {
                game_over_message = "机器人获胜!";
            }
            gameOverLayer(game_over_message);
            return;
        }

    }
    //白色
    if(action_id == AppModel::getInstance()->m_pBotGameModel->m_white_id)
    {
        printf("(人机模式)开始渲染白色棋子，坐标%d %d\n",row,col);
        cocos2d::Vec2 roundedLos = Vec2(col,row);
        auto pChess = drawChess(roundedLos,false);
        m_pChess[roundedLos] = pChess;
        m_board[col][row] = WHITE_CHESS;
        m_whitePlayerActions.push_back({col,row});
        int winner_id = CheckWin(m_board,col,row,WHITE_CHESS);
        if(winner_id ==WHITE_CHESS)
        {
            pMenuItmUndo -> setVisible(false);
            pauseGame(true);
            std::string game_over_message;
            if(action_id ==AppModel::getInstance()->m_pBotGameModel->m_userid)
            {
                game_over_message = "玩家获胜!";
            }
            else
            {
                game_over_message = "机器人获胜!";
            }
            gameOverLayer(game_over_message);
            return;
        }
    }
    //判断平局
    if((m_whitePlayerActions.size() + m_blackPlayerActions.size()) == (m_board[0].size() * m_board.size()))
    {
        pMenuItmUndo -> setVisible(false);
        pauseGame(true);
        gameOverLayer("双方平局！");
        return;
    }
    if(action_id == AppModel::getInstance()->m_pBotGameModel->m_userid)
    {
        AppModel::getInstance()->m_pBotGameModel->m_put_num ++;
    }
    
    
    
    if(AppModel::getInstance()->m_pBotGameModel->m_curid == AppModel::getInstance()->m_pBotGameModel->m_robotid)
    {
        //机器人下棋
        randChess();
    }
    

}

bool BotModeScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{

    std::cout<<"(人机模式)点击位置：("<<touch -> getLocation().x<<", "<<touch->getLocation().y<<")"<<std::endl;

    Vec2 touchLos = touch -> getLocation();

    if (m_LB.x < touchLos.x && touchLos.x < m_RT.x && m_LB.y < touchLos.y && touchLos.y < m_RT.y)
    {
        std::cout<< "(人机模式)点击棋盘相对位置：("
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

void BotModeScene::putSign(cocos2d::Vec2 location)
{
    delSign();
    Vec2 roundedLos(int(round((location * m_chessBoardSize).x)), int(round((location * m_chessBoardSize).y)));
    std::cout<< "(人机模式)确定棋盘坐标为：("<<roundedLos.x<<", "<<roundedLos.y<<")"<<std::endl;

    if (m_isAlreadyPut[roundedLos])
    {
        log("(人机模式)已经放置棋子。");
        return;
    }
    drawSign(roundedLos);
    m_Choosed = roundedLos;

}
void BotModeScene::putChess(cocos2d::Vec2 location) {
    Vec2 roundedLos(int(round((location * m_chessBoardSize).x)), int(round((location * m_chessBoardSize).y)));
    std::cout<< "(人机模式)确定棋盘坐标为：("<<roundedLos.x<<", "<<roundedLos.y<<")"<<std::endl;
    if(m_pSign)
    {
        m_pSign -> removeFromParent();
        m_pSign = nullptr;
    }


    if (m_isAlreadyPut[roundedLos])
    {
        log("(人机模式)已经放置棋子, 请重新放置.");
        return;
    }
    ValueMap oData;
    oData.insert(make_pair("action_board_row" ,Value(roundedLos.y)));
    oData.insert(make_pair("action_board_col" , Value(roundedLos.x)));
    dispatchEvent(EVT_PLACE_A_STONE_SUCCESS, &oData);
}

//倒计时每过一秒刷新一下
//超时随机落子
void BotModeScene::randChess(){
    cocos2d::Vec2 randLos;

    std::pair<int, int> position = m_robot.GetkeySetchess(m_board);

    log("机器人开始下棋，找到的坐标:%d,%d",position.second,position.first);
    randLos = Vec2(position.second,position.first);
    if(m_isAlreadyPut[randLos] == false){
        randLos = Vec2((randLos.x * m_diff)/(m_RT.x - m_LB.x),( randLos.y * m_diff)/(m_RT.y - m_LB.y));
        log("机器人转化后的坐标:%f, %f",randLos.x,randLos.y);
        putChess(randLos);
        return;
    }
}


void BotModeScene::onUndoClicked(cocos2d::Ref *pSender) {
    delSign();
    log("(人机模式)点击了悔棋按钮");
    log("当前下的棋子数量%d",AppModel::getInstance()->m_pBotGameModel->m_put_num);
    if(AppModel::getInstance()->m_pBotGameModel->m_put_num <=0)
    {
        log("(人机模式)当前这个玩家没有下过棋，无法悔棋");
        return;
    }
    if(AppModel::getInstance()->m_pBotGameModel->m_is_user != true)
    {
        log("(人机模式)无法在机器人回合悔棋，悔棋失败");
        return;
    }
    // 处理悔棋逻辑
    if (m_pChess.empty() || m_pChess.size() == 1)
    {
        log("棋盘上棋子不足，无法悔棋！");
        return;
    }
    AppModel::getInstance()->m_pBotGameModel->m_put_num --;
    EventCustom* pEvent;
    doUndo(pEvent);
    delSign();
    doUndo(pEvent);
    delSign();
}

void BotModeScene::doUndo(EventCustom* pEvent){
    printf("(人机模式)悔棋成功即将移除棋子");

    auto& action_id = AppModel::getInstance()->m_pBotGameModel->m_curid;
    int undo_col = 0;
    int undo_row = 0;
    if(action_id == AppModel::getInstance()->m_pBotGameModel->m_white_id)
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

    log("(人机模式)移动轮到谁的提示");

    // 交换isme
    AppModel::getInstance()->m_pBotGameModel->m_is_user =(AppModel::getInstance()->m_pBotGameModel->m_is_user == true ? false :true);


    AppModel::getInstance()->m_pBotGameModel-> m_curid =(AppModel::getInstance()->m_pBotGameModel->m_curid == AppModel::getInstance()->m_pBotGameModel->m_userid ? AppModel::getInstance()->m_pBotGameModel->m_robotid : AppModel::getInstance()->m_pBotGameModel->m_userid);

}

void BotModeScene::pauseGame(bool flag)
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



