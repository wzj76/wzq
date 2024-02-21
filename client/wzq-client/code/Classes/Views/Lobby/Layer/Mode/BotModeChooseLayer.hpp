//
//  BotModeChooseLayer.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/7.
//

#ifndef BotModeChooseLayer_hpp
#define BotModeChooseLayer_hpp

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
class BotModeChooseLayer : public cocos2d::Layer,WZQEventWrapper
{
public:
    BotModeChooseLayer();
    ~BotModeChooseLayer();
    
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(BotModeChooseLayer);
    
    void onChooseHard(std::string mode);
    void onChooseSide(std::string side);
    void onChooseSize(BOARDSIZE size);


    
private:
    Menu* m_pMenu;
    Sprite* m_pBg;
    Label* m_pTitleLable;
    std::string m_modeChoosed;
    std::string m_sideChoosed;
    BOARDSIZE m_chessSize;
    std::vector<std::pair<std::string, CandidateBox*>> m_HardChooseList;
    std::vector<std::pair<std::string, CandidateBox*>> m_SideChooseList;
    std::vector<std::pair<BOARDSIZE, CandidateBox*>> m_SizeChooseList;
};

#endif /* BotModeChooseLayer_hpp */
