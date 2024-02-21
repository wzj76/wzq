//
//  ShowHistoryScene.hpp
//  wzqgame-mobile
//
//  Created by duole on 2024/2/2.
//

#ifndef ShowHistoryScene_hpp
#define ShowHistoryScene_hpp

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
class ShowHistoryScene : public BaseInGameScene, WZQEventWrapper
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(ShowHistoryScene);
    
    ShowHistoryScene();
    ~ShowHistoryScene();
    //游戏结束
    void onPreClicked(Ref* pSender);
    void onNxtClicked(Ref* pSender);
    Sprite* drawChess(cocos2d::Vec2 roundedLos, bool isBlack);
    virtual void onExitGameClicked(cocos2d::Ref *pSender);
private:
    int m_black_step;
    int m_white_step;
    int m_step;
    int m_sum_step;
    KKUSERINFO m_oppinfo;
    HISTORYINFO m_history;
    std::vector<BLACKPOSITION> black_position;
    std::vector<WHITEPOSITION> white_positon;
    Menu* m_pMenu;
    Vector<Sprite*> m_pChess;
    cocos2d::Vector<cocos2d::Sprite*> m_ChessVector;
    MenuButton* m_pMenuItmPre;
    MenuButton* m_pMenuItmNxt;
    MenuButton* m_pMenuItmExit;
    
};
#endif /* ShowHistoryScene_hpp */
