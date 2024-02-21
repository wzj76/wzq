//
//  PopTips.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/4.
//

#ifndef PopTips_hpp
#define PopTips_hpp
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
class PopTips : public cocos2d::Sprite
{
public:
    PopTips();
    ~PopTips();
    
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    static PopTips* create();
    void setText(const std::string text);
    static void pop(Node* parent, const std::string text, float time = 3.0f);
    
private:
    Label* m_pText;
};

#endif /* PopTips_hpp */
