//
//  MatchingScene.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/31.
//

#ifndef MatchingScene_hpp
#define MatchingScene_hpp

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
USING_NS_CC_EXT;
class MatchingScene : public BaseInGameScene, WZQEventWrapper
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(MatchingScene);
    
    MatchingScene();
    ~MatchingScene();
    void gameOverLayer(std::string tips);
    //玩家下棋(测试用)
    void Chess(EventCustom* pEvent);
    //接受到服务端的动作确认
    void onACKChess(EventCustom* pEvent);
    //游戏结束
    void GameOver (EventCustom* pEvent);
    void onUndoClicked(Ref* pSender);
    void onACKUndo(EventCustom* pEvent);
    void onChatClicked(Ref* pSender);
    void onSendChatClicked(Ref* pSender);
    void putSign(Vec2 location);
    void putChess(Vec2 location);
    virtual void onExitGameClicked(cocos2d::Ref *pSender);
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void putSign(Vec2 location, Vec2 LB);
    void doUndo(EventCustom* pEvent);
    void putChess(Vec2 location, Vec2 LB);
    void randChess();
    virtual void animUpdate(float t);
    void doUndoWait(EventCustom* pEvent);
    void doUndoFaill(EventCustom* pEvent);
    void connFailed(EventCustom *pEvent);
    //回复定时器
    void resetTimer(EventCustom* pEvent);
    
    void pauseGame(bool flag);
    
    void replaceLogin(EventCustom *pEvent);
    
    void showChat(EventCustom *pEvent);
    
    //等待对方回复是否继续游戏
    void doWaitContinueGame(EventCustom* pEvent);
    void onAskContinueGame(EventCustom *pEvent);
    void matchSuccess(EventCustom* pEvent);
private:
    Vector<Sprite*> m_pChess;
    cocos2d::Vector<cocos2d::Sprite*> m_ChessVector;
    MenuButton* pMenuItmUndo;
    EventListenerTouchOneByOne* m_touchListener;
    EditBox* pEditBox;
    bool isOpenChatBox;
    MenuButton* m_pMenuItmChat;
    MenuButton* m_pSendBtn;
    bool m_isTouchEnabled;
};


#endif /* MatchingScene_hpp */
