//
//  FriendListLayer.hpp
//  wzqgame-mobile
//
//  Created by duole on 2024/2/5.
//

#ifndef FriendListLayer_hpp
#define FriendListLayer_hpp

#include "cocos2d.h"
#include "AppRes.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "Event/WZQEventWrapper.h"
#include "MenuButton.hpp"
#include "WZQWaitingLayer.h"
#include "AppDef.h"
#include "ReminderWindow.hpp"
#include<string>
#include <vector>
USING_NS_CC;

USING_NS_CC_EXT;
class FriendListLayer : public cocos2d::Layer,WZQEventWrapper
{
public:
    FriendListLayer();
    ~FriendListLayer();
    
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(FriendListLayer);
    
    void onMenuEsc(Ref* pSender);
    //更新好友列表
    void updateList(EventCustom* pEvent);
    //等待对方同意
    void waitFriendGame(EventCustom* pEvent);
    void onAddFriendClicked(Ref* pSender);
    void onConfirmClicked(Ref* pSender);
    void onWaitFriendClicked(Ref* pSender);
    void onFriendListClicked(Ref* pSender);
    void onNextPageClicked(Ref* pSender);
    void onLastPageClicked(Ref* pSender);
    void addScrollViewInfo(KKUSERINFO friendInfo);
    void eventQuitSearch(EventCustom* pEvent);
    void resetList(EventCustom* pEvent);
    
private:
    
    void updateScrollView();
    
    Menu* m_pMenu;
    Menu* m_pScrollMenu;
    Vector<Label*>* m_pUserIDs;
    Vector<Sprite*>* m_pUserHeads;
    EditBox* pEditBox;
    Sprite* m_pBg;
    MenuButton* m_pMenuButtonAddFriend;
    MenuButton* m_pMenuButtonWaitFriend;
    MenuButton* m_pMenuButtonFriendList;
    Label* m_pLablePageId;
    WZQWaitingLayer* m_pWaiting;
    ui::ScrollView* m_scrollView;
    int m_scrollHeight;
    std::vector<std::pair<Node*, Vec2>> m_scrollContent;
    Layer* waitLayer;
    Vector<MenuButton*> m_deleteFriendBtns;
    Vector<MenuButton*> m_inviteBtns;
};

#endif /* FriendListLayer_hpp */
