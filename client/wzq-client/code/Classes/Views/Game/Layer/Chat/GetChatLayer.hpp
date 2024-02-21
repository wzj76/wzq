//
//  GetChatLayer.hpp
//  wzqgame-mobile
//
//  Created by duole on 2024/2/4.
//

#ifndef GetChatLayer_hpp
#define GetChatLayer_hpp

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
class GetChatLayer : public cocos2d::Layer,WZQEventWrapper
{
public:
    GetChatLayer();
    ~GetChatLayer();
    
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(GetChatLayer);
    
    
private:
    Menu* m_pMenu;
};

#endif /* GetChatLayer_hpp */
