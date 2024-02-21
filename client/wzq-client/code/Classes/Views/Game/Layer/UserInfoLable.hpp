//
//  UserInfoLable.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/29.
//

#ifndef UserInfoLable_hpp
#define UserInfoLable_hpp

#include <stdio.h>

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
class UserInfoLable : public cocos2d::Sprite
{
public:
    UserInfoLable();
    ~UserInfoLable();
    
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    static UserInfoLable* create();
    void setInfo(int imageId, const std::string Username, int scores);
    void delScoreLabel();
    
private:
    int m_imageid;
    std::string m_sUsername;
    int m_scores;
    Sprite* m_pOpposentImage;
    Label* m_pOpposentNameLable;
    Label* m_pOpposentIDLable;
};

#endif /* UserInfoLable_hpp */
