//
//  BaseBaseInGameScene.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/31.
//

#ifndef BaseInGameScene_hpp
#define BaseInGameScene_hpp


#include "cocos2d.h"
#include "AppRes.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "Event/WZQEventWrapper.h"
#include "MenuButton.hpp"
#include "WZQUserModel.h"
#include "ReminderWindow.hpp"
#include "UserInfoLable.hpp"
#include "ScrollLayer.hpp"
#include <vector>
#include <string>
#include <map>
USING_NS_CC_EXT;
class BaseInGameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(BaseInGameScene);
    
    BaseInGameScene();
    ~BaseInGameScene();

    
    virtual void onExitGameClicked(Ref* pSender);
    virtual bool onTouchBegan(Touch* touch, Event* event);
    
    Vec2 toRelativeCoord(Vec2 location);
    void animUpdate (float t);
    void randChess();
    void putSign(Vec2 location);
    void putChess(Vec2 location);
    Sprite* drawChess(Vec2 roundedLos, bool isBlack);
    void drawSign(Vec2 roundedLos);
    void delSign();
    virtual void gameOverLayer(std::string tips);
    
    
    
    //倒计时
    Label* m_my_remain ;
    Label* m_opp_remain ;
    int m_my_time;
    int m_opp_time;
    
    

protected:
    MenuButton* pMenuItmExitGame;
    bool m_isComfirm;
    Vec2 m_position;
    Vec2 m_LB;
    Vec2 m_RT;
    Vec2 m_Choosed;
    Sprite* m_pChessBoardImage;
    Sprite* m_pSign;
    Sprite* m_pTips;
    std::map<Vec2, bool> m_isAlreadyPut;
    bool m_isBlack;
    float m_diff;
    Vector<Sprite*> m_pChess;
    Sequence* m_pTipsShow;
    Vec2 m_myPos;
    Vec2 m_oppoPos;
    EventListenerTouchOneByOne* m_touchListener;
    UserInfoLable* m_pOppoInfo;
    UserInfoLable* m_pMyInfo;
    ScrollLayer* m_pScrollLayer;
    unsigned int m_chessBoardSize;
    
protected:
    Menu* m_pMenu;
    ReminderWindow* m_gameOverLayer;
};

#endif /* BaseBaseInGameScene_hpp */
