//
//  LoginLayer.h
//  phzgame
//
//  Created by admin on 2022/11/28.
//

#ifndef LoginLayer_h
#define LoginLayer_h

#include "cocos2d.h"
#include "AppRes.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "Event/WZQEventWrapper.h"
#include "WZQWaitingLayer.h"
#include<string>
USING_NS_CC_EXT;
class LoginLayer : public cocos2d::Layer, WZQEventWrapper,EditBoxDelegate
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);
    CREATE_FUNC(LoginLayer);
    
    
    LoginLayer();
    ~LoginLayer();
public:
    std::string username;
    std::string password;
    std::string copyword;
    std::string value[3];
    void onMenuLogin(Ref* pSender);
    void onMenuRegister(Ref* pSender);
    void onCloseRegisterLayer(cocos2d::EventCustom* pEvent);
    void onExitGame(Ref* pSender);

    void fromRegistToSignin(cocos2d::EventCustom* pEvent);
    void transferToLoginSence(cocos2d::EventCustom* pEvent);
    void connFailed(cocos2d::EventCustom *pEvent);
    void generateButtons(cocos2d::EventCustom *pEvent);
private:
    cocos2d::Vector<cocos2d::MenuItemSprite*> m_oMenuVector;
    bool m_buttonflag;
    
    
};

#endif /* LoginLayer_h */
