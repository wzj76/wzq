//
//  BotModeScene.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/1.
//

#ifndef BotModeScene_hpp
#define BotModeScene_hpp

#include "cocos2d.h"
#include "AppRes.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "Event/WZQEventWrapper.h"
#include "MenuButton.hpp"
#include "WZQUserModel.h"
#include "BaseInGameScene.hpp"
#include <vector>
#include <string>
#include <map>
#include "robot.hpp"
USING_NS_CC_EXT;


#define BOARD_SIZE 13
#define BLACK_CHESS 1
#define WHITE_CHESS 2

#define KEY_EASY "easymode"
#define KEY_NORMAL "normalmode"
#define KEY_HARD "hardmode"

class BotModeScene : public BaseInGameScene, WZQEventWrapper
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(BotModeScene);
    
    BotModeScene();
    ~BotModeScene();
    //玩家下棋(测试用)
    void Chess(EventCustom* pEvent);
    //接受到服务端的动作确认
    void onACKChess(EventCustom* pEvent);
    //游戏结束
    void GameOver (EventCustom* pEvent);
    void onUndoClicked(Ref* pSender);
    void replaceLogin(EventCustom *pEvent);
    virtual void putSign(Vec2 location);
    virtual void putChess(Vec2 location);
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void putSign(Vec2 location, Vec2 LB);
    void doUndo(EventCustom* pEvent);
    void connFailed(EventCustom *pEvent);
    void putChess(Vec2 location, Vec2 LB);
    virtual void randChess();
    void pauseGame(bool flag);
    bool five(std::vector<std::vector<int>>&board,int row ,int col , int row_off , int col_off , int color);
    int CheckWin(std::vector<std::vector<int>>&board,int row,int col,int color);
    
    Robot m_robot;
    
private:
    MenuButton* pMenuItmUndo;
    MenuButton* pGoBack;
    std::map<Vec2, Sprite*> m_pChess;
    std::vector<std::vector<int>> m_board; //棋盘
    std::vector<std::pair<int,int>>m_blackPlayerActions; //记录黑棋玩家的下棋操作
    std::vector<std::pair<int,int>>m_whitePlayerActions; //记录棋玩家的下棋操作
};

#endif /* BotModeScene_hpp */
