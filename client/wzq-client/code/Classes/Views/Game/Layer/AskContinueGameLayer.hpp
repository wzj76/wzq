//
//  AskContinueGameLayer.hpp
//  wzqgame-mobile
//
//  Created by duole on 2024/2/6.
//

#ifndef AskContinueGameLayer_hpp
#define AskContinueGameLayer_hpp

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
class AskContinueGameLayer : public cocos2d::Layer,WZQEventWrapper
{
public:
    AskContinueGameLayer();
    ~AskContinueGameLayer();
    
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(AskContinueGameLayer);
    
    // 确定按钮回调函数
    void onAgreeClicked(Ref* pSender);
    void onRefuseClicked(Ref* pSender);
    void AutoRefuse(float t);
    
    int m_enemy_id;
    
private:
    Menu* m_pMenu;
};
#endif /* AskContinueGameLayer_hpp */
