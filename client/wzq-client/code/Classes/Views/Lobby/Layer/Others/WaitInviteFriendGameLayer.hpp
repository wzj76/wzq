//
//  WaitInviteFriendGameLayer.hpp
//  wzqgame-mobile
//
//  Created by duole on 2024/2/18.
//

#ifndef WaitInviteFriendGameLayer_hpp
#define WaitInviteFriendGameLayer_hpp
#include "cocos2d.h"
#include "AppRes.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "Event/WZQEventWrapper.h"
#include "MenuButton.hpp"
#include "WZQWaitingLayer.h"
#include<string>
#include <stdio.h>
USING_NS_CC;
USING_NS_CC_EXT;
class WaitInviteFriendGameLayer : public cocos2d::Layer,WZQEventWrapper
{
public:
    WaitInviteFriendGameLayer();
    ~WaitInviteFriendGameLayer();
    
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(WaitInviteFriendGameLayer);
    void RemoveLayer(EventCustom* pEvent);
    void AutoRemove(float t);
    
private:
};
#endif /* WaitInviteFriendGameLayer_hpp */
