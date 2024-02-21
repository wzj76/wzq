//
//  AskInviteFriendGameLayer.hpp
//  wzqgame-mobile
//
//  Created by duole on 2024/2/18.
//

#ifndef AskInviteFriendGameLayer_hpp
#define AskInviteFriendGameLayer_hpp


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
class AskInviteFriendGameLayer : public cocos2d::Layer,WZQEventWrapper
{
public:
    AskInviteFriendGameLayer();
    ~AskInviteFriendGameLayer();
    
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(AskInviteFriendGameLayer);
    
    // 确定按钮回调函数
    void onAgreeClicked(Ref* pSender);
    void onRefuseClicked(Ref* pSender);
    void AutoRefuse(float t);
    
    int m_friend_id;
    
private:
    Menu* m_pMenu;
};
#endif /* AskInviteFriendGameLayer_hpp */
