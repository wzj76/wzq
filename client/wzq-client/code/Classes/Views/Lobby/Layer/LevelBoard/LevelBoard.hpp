//
//  LevelBoard.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/26.
//

#ifndef LevelBoard_hpp
#define LevelBoard_hpp

#include "cocos2d.h"
#include "AppRes.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "Event/WZQEventWrapper.h"
#include "MenuButton.hpp"
#include "WZQWaitingLayer.h"
#include<string>
USING_NS_CC;

USING_NS_CC_EXT;
class LevelBoard : public cocos2d::Layer,WZQEventWrapper
{
public:
    LevelBoard();
    ~LevelBoard();
    
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(LevelBoard);
    
    void onMenuEsc(Ref* pSender);
    void updateBoard(Event* pEvent);
    void onNextPageClicked(Ref* pSender);
    void onLastPageClicked(Ref* pSender);
    
private:
    Menu* m_pMenu;
    Vector<Label*>* m_pUserIDs;
    Vector<Label*>* m_pUserWins;
    Vector<Label*>* m_pUserScores;
    Vector<Label*>* m_pRankings;
    MenuButton* m_pMenuButtonNextPage;
    MenuButton* m_pMenuButtonLastPage;
    Label* m_pLablePageId;
    Label* m_pNodataLabel;
    
    Sprite* m_pBg;
    unsigned int m_Pageid;
    WZQWaitingLayer* m_pWaiting;
};

#endif /* LevelBoard_hpp */
