//
//  ScrollLayer.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/5.
//

#ifndef ScrollLayer_hpp
#define ScrollLayer_hpp

#include "cocos2d.h"
#include "AppRes.h"
#include "Core/AppModel.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "Event/WZQEventWrapper.h"
#include "MenuButton.hpp"
#include "WZQWaitingLayer.h"
#include<string>
#include <stdio.h>
USING_NS_CC;
USING_NS_CC_EXT;
class ScrollLayer : public cocos2d::Layer,WZQEventWrapper
{
public:
    ScrollLayer();
    ~ScrollLayer();
    
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(ScrollLayer);
    
    void addText(const std::string text);
    void setButton(MenuButton* button);
    std::string getEditboxText();
    
private:
    Menu* m_menu;
    Sprite* m_pBg;
    ui::ScrollView* m_scrollView;
    int m_scrollHeight;
    EditBox* m_Editbox;
};


#endif /* ScrollLayer_hpp */
