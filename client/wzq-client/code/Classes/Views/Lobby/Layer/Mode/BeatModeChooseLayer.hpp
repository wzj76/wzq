//
//  BeatModeChooseLayer.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/18.
//

#ifndef BeatModeChooseLayer_hpp
#define BeatModeChooseLayer_hpp

#include "cocos2d.h"
#include "AppRes.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "Event/WZQEventWrapper.h"
#include "MenuButton.hpp"
#include "WZQWaitingLayer.h"
#include "CandidateBox.hpp"
#include<string>
USING_NS_CC;

USING_NS_CC_EXT;
class BeatModeChooseLayer : public cocos2d::Layer,WZQEventWrapper
{
public:
    BeatModeChooseLayer();
    ~BeatModeChooseLayer();
    
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(BeatModeChooseLayer);
    
    void onChooseSize(BOARDSIZE boardSize);


    
private:
    Menu* m_pMenu;
    Sprite* m_pBg;
    Label* m_pTitleLable;
    BOARDSIZE m_sizeChoosed;
    std::vector<std::pair<BOARDSIZE, CandidateBox*>> m_HardChooseList;
};

#endif /* BeatModeChooseLayer_hpp */
