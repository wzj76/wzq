//
//  WaitFriendLayer.hpp
//  wzqgame-mobile
//
//  Created by duole on 2024/2/5.
//

#ifndef WaitFriendLayer_hpp
#define WaitFriendLayer_hpp

#include "cocos2d.h"
#include "AppRes.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "Event/WZQEventWrapper.h"
#include "MenuButton.hpp"
#include "WZQWaitingLayer.h"
#include "AppDef.h"
#include<string>
USING_NS_CC;

USING_NS_CC_EXT;
class WaitFriendLayer : public cocos2d::Layer,WZQEventWrapper
{
public:
    WaitFriendLayer();
    ~WaitFriendLayer();
    
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(WaitFriendLayer);
    
    void onMenuEsc(Ref* pSender);
    void updateList(Event* pEvent);
    //更新好友请求
    void updateWaitList(EventCustom* pEvent);
    void onConfirmClicked(Ref* pSender);
    void onCancelClicked(Ref* pSender);
    void resetScrollView();
    
private:
    Menu* m_pMenu;
    Menu* m_pScrollMenu;
    Sprite* m_pBg;
    MenuButton* m_pMenuButtonConfirm;
    MenuButton* m_pMenuButtonCancel;
    ui::ScrollView* m_scrollView;
    int m_scrollHeight;
    std::vector<std::pair<Node*, Vec2>> m_scrollContent;
    
    void addScrollViewInfo(KKUSERINFO friendInfo);
    void updateScrollView();
    void onFriendListClicked(Ref* pSender);
    void onAddFriendClicked(Ref* pSender);
    void onWaitFriendClicked(Ref* pSender);
    
};

#endif /* WaitFriendLayer_hpp */
