//
//  WaitAckLayer.hpp
//  wzqgame-mobile
//
//  Created by duole on 2024/1/31.
//

#ifndef WaitAckLayer_hpp
#define WaitAckLayer_hpp
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
class WaitAckLayer : public cocos2d::Layer,WZQEventWrapper
{
public:
    WaitAckLayer();
    ~WaitAckLayer();
    
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(WaitAckLayer);
    void RemoveLayer(EventCustom* pEvent);
    void AutoRemove(float t);
    
private:
};
#endif /* WaitAckLayer_hpp */
