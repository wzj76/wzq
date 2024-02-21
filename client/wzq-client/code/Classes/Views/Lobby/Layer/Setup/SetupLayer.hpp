//
//  SetupLayer.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/6.
//

#ifndef SetupLayer_hpp
#define SetupLayer_hpp

#include "cocos2d.h"
#include "AppRes.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "Event/WZQEventWrapper.h"
#include "MenuButton.hpp"
#include "WZQWaitingLayer.h"
#include "CandidateBox.hpp"
#include "PlatformFunction.h"
#include<string>
USING_NS_CC;

USING_NS_CC_EXT;
class SetupLayer : public cocos2d::Layer,WZQEventWrapper
{
public:
    SetupLayer();
    ~SetupLayer();
    
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(SetupLayer);
    
    void onExitGameClicked(Ref *pSender);
    void updateInfo();
    void onCloseClicked(Ref* pSender);
    void onRuleBtnClicked(Ref* pSender);

    
private:
    Menu* m_pMenu;
    Sprite* m_pBg;
    Label* m_pTitleLable;
    CandidateBox* bgmusic;
    CandidateBox* bgeff;
    CandidateBox* btsetup;
    CandidateBox* putchessbtn;
};

#endif /* SetupLayer_hpp */
