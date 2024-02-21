//
//  HistoryBoard.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/1.
//

#ifndef HistoryBoard_hpp
#define HistoryBoard_hpp


#include "cocos2d.h"
#include "AppRes.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "Event/WZQEventWrapper.h"
#include "MenuButton.hpp"
#include "WZQWaitingLayer.h"
#include<string>
#include <vector>
#include <map>


USING_NS_CC;

USING_NS_CC_EXT;



class HistoryBoard : public cocos2d::Layer,WZQEventWrapper
{
public:
    HistoryBoard();
    ~HistoryBoard();
    
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(HistoryBoard);
    
    void onMenuEsc(Ref* pSender);
    void updateBoard(Event* pEvent);
    void onNextPageClicked(Ref* pSender);
    void onLastPageClicked(Ref* pSender);
    void onEnterClicked(Ref* pSender);
    
private:
    Menu* m_pMenu;
    MenuButton* m_pMenuButtonNextPage;
    MenuButton* m_pMenuButtonLastPage;
    Label* m_pLablePageId;
    Vector<Label*>* m_pTime;
    Vector<Label*>* m_pOppo;
    Vector<Label*>* m_pIsWin;
    Vector<MenuButton*>* m_pEnter;
    Sprite* m_pBg;
    unsigned int m_Pageid;
    WZQWaitingLayer* m_pWaiting;
};


#endif /* HistoryBoard_hpp */
