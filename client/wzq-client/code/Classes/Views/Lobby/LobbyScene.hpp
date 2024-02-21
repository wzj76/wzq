//
//  LobbyScene.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/24.
//

#ifndef LobbyScene_hpp
#define LobbyScene_hpp

#include "cocos2d.h"
#include "AppRes.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "Event/WZQEventWrapper.h"
#include "MenuButton.hpp"
#include "WZQWaitingLayer.h"
#include<string>
USING_NS_CC_EXT;
class LobbyScene : public cocos2d::Layer, WZQEventWrapper
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(LobbyScene);
    
    void onUserBtnClicked(Ref* pSender);
    void onMatchingClicked(Ref* pSender);
    void onShopClicked(Ref* pSender);
    void onFriendClicked(Ref* pSender);
    void onLevelBroadClicked(Ref* pSender);
    void onHistoryGameClicked(Ref* pSender);
    void onBeatingClicked(Ref* pSender);
    void onBotModeClicked(Ref* pSender);
    void onSetupClicked(Ref* pSender);
    
    void buttonsBack(EventCustom *pEvent);
    void ChangeHead(EventCustom *pEvent);
    void putUserInfo(cocos2d::EventCustom* pEvent);
    void buttonBreath(float dt);
    void replaceLogin(EventCustom *pEvent);
    void editRed(EventCustom *pEvent); //在好友图标旁增加显示新请求的红点
    void askInviteFriendGame(EventCustom *pEvent);
    void inviteFriendGameSuccess(EventCustom *pEvent);
    void connFailed(EventCustom *pEvent);
    // 更新金币数
    void updateCoins(EventCustom* pEvent);
    LobbyScene();
    ~LobbyScene();
    void Test(float dt);
private:
    Menu* m_pMenu;
    Sprite* m_pBackgroundBlack;
    //头像按钮
    MenuButton* m_pUserImageMI;
    //用户名
    Label * m_pUserNameLable;
    //ID
    Label * m_pUserIDLable;
    Label* m_pCoinLabel;
    WZQWaitingLayer* m_pWaiting;
    MenuButton* m_pMenuItemBtnMatching;

};


#endif /* LobbyScene_hpp */
