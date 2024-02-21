//
//  LoginLayer.cpp
//  wzqgame-mobile
//
//  Created by admin on 2022/11/28.
//

#include <stdio.h>
#include <iostream>
#include "LoginLayer.h"
#include "SigninLayer.hpp"
#include "RegistLayer.h"
#include "ReminderWindow.hpp"
#include "Core/AppEvent.h"
#include "Core/AppFunc.h"
#include "Core/AppModel.h"
#include "Controllers/WZQLoginController.h"
#include "ReminderWindow.hpp"
#include "SimpleAudioEngine.h"
#include "WZQDeviceSize.hpp"
#include "LobbyScene.hpp"
#include "MenuButton.hpp"
#include "PopTips.hpp"
#include "CandidateBox.hpp"
#include "AppDelegate.h"
USING_NS_CC;
using namespace std;
Scene* LoginLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = LoginLayer::create();
    scene->addChild(layer);
    return scene;
}
//auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
bool LoginLayer::init()
{
    if(!Layer::init())
    {
        return false;
    }
    // 初始化按钮音效
    CocosDenshion::SimpleAudioEngine::getInstance() -> preloadEffect(SOUND_BT);
    // 初始化登录背景音乐
    CocosDenshion::SimpleAudioEngine::getInstance() -> preloadBackgroundMusic(SOUND_LOGIN_BACKGROUND);
    CocosDenshion::SimpleAudioEngine::getInstance() -> playBackgroundMusic(SOUND_LOGIN_BACKGROUND);
    
    
    auto vsize = Director::getInstance()->getVisibleSize();
    Sprite* bg = Sprite::create(BG_LOGIN);
    bg->setAnchorPoint(Vec2(0.5,0.5));
    bg->setPosition(vsize.width/2, vsize.height/2);
    float s1 = vsize.width/(bg->getContentSize().width);
    float s2 = vsize.height/(bg->getContentSize().height);
    bg->setScale(s1, s2);
    addChild(bg);
    
    Sprite* pLogo = Sprite::create(BG_LOGO);
    pLogo -> setAnchorPoint(Vec2(0.5f, 0.5f));
    pLogo -> setPosition(vsize.width / 2, 3 * vsize.height / 5);
    pLogo -> setScale(vsize.width/pLogo -> getContentSize().width * 0.5, vsize.width/pLogo -> getContentSize().width * 0.5);
    addChild(pLogo);
    
    auto pMenuItemBtnSubmit = MenuButton::create("登录",  CC_CALLBACK_1(LoginLayer::onMenuLogin, this));
    pMenuItemBtnSubmit->setPosition(Vec2(vsize.width * 0.4, vsize.height * 0.1));

    auto pMenuItemBtnRegister = MenuButton::create("注册",  CC_CALLBACK_1(LoginLayer::onMenuRegister, this));
    pMenuItemBtnRegister->setPosition(Vec2(vsize.width * 0.6, vsize.height * 0.1));
    
    auto pMenu = Menu::create(pMenuItemBtnSubmit, pMenuItemBtnRegister, NULL);
    pMenu->setPosition(Vec2::ZERO);
    pMenu->setAnchorPoint(Vec2::ZERO);
    addChild(pMenu);
    
    m_oMenuVector.pushBack(pMenuItemBtnSubmit);
    m_oMenuVector.pushBack(pMenuItemBtnRegister);
    
    auto pSoundBox = CandidateBox::create(BT_SOUNDUP, BT_SOUNDDOWN);
    pSoundBox->setScale(WZQDeviceSize::getContentScaleInHeight(pSoundBox, 0.1f));
    pSoundBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    pSoundBox->setPosition(0,vsize.height);
    addChild(pSoundBox);
    
    pSoundBox->setSelectedCallback([](Ref* pSender){
        CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    });
    
    pSoundBox->setUnselectedCallback([](Ref* pSender){
        CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    });
    
    return true;
}
void LoginLayer::onCloseRegisterLayer(EventCustom* pEvent)
{
    for(auto&e:m_oMenuVector)
    {
        e->setVisible(true);
    }
}
void LoginLayer::fromRegistToSignin(EventCustom* pEvent)
{
    for(auto&e:m_oMenuVector)
    {
        e->setVisible(false);
    }
    auto tc = SigninLayer::create();
   addChild(tc);
}

void LoginLayer::onMenuLogin(Ref* pSender)
{
    if(app->m_pConnect->IsConnect() ==false)
    {
        //尝试重新连接
        app->m_pConnect->DoConnect();
    }
    
    printf("app->m_pConnect->IsConnect():%d\n",app->m_pConnect->IsConnect());
    if(app->m_pConnect->IsConnect() ==true)
    {
        AppModel::getInstance() -> m_pUserModel -> m_pWaiting -> show();
        for(auto&e:m_oMenuVector)
        {
            e->setVisible(false);
        }
        auto tc = SigninLayer::create();
       addChild(tc);
    }

    
}
void LoginLayer::onMenuRegister(Ref* pSender)
{
    if(app->m_pConnect->IsConnect() ==false)
    {
        //尝试重新连接
        app->m_pConnect->DoConnect();
    }
    
    
    if(app->m_pConnect->IsConnect() ==true)
    {
        for(auto&e:m_oMenuVector)
        {
            e->setVisible(false);
        }
        auto tc = RegistLayer::create();
        addChild(tc);
    }

    
}
void LoginLayer::onExitGame(Ref* pSender)
{

}

void LoginLayer::onEnter()
{
    Layer::onEnter();
    ADD_OBSERVER(LoginLayer::onCloseRegisterLayer, EVT_CLOSE_REGISTE_LAYER);
    ADD_OBSERVER(LoginLayer::fromRegistToSignin, REGISTER_SUCCESS);
    ADD_OBSERVER(LoginLayer::transferToLoginSence, LOGIN_SUCCESS);
    ADD_OBSERVER(LoginLayer::connFailed, CONN_FAILED);
}
void LoginLayer::connFailed(cocos2d::EventCustom *pEvent)
{
    PopTips::pop(this, "无法连接服务器，请检查网络连接!");
}
void LoginLayer::onExit()
{
    removeAllEventListeners();
    Layer::onExit();
}
void LoginLayer::editBoxTextChanged(EditBox *editBox, const std::string &text)
{
    int index = editBox->getTag();
    value[index] = text;
}
void LoginLayer::editBoxEditingDidBegin(EditBox* editBox)
{
    
}
void LoginLayer::editBoxEditingDidEnd(EditBox* editBox)
{
    int index = editBox->getTag();
    value[index] = editBox->getText();
   
}
void LoginLayer::editBoxReturn(EditBox* editBox)
{
    int index = editBox->getTag();
    value[index] = editBox->getText();
    printf("%d %s\n",index,value[index].c_str());
}

void LoginLayer::transferToLoginSence(cocos2d::EventCustom *pEvent) {
//    string name=usrinfo.sUser;
//    cout<<"名字是："<<name<<endl;
    AppModel::getInstance() -> m_pUserModel -> m_pWaiting -> hide();
    auto LoginScene = LobbyScene::createScene();
    Director::getInstance() -> replaceScene(LoginScene);
}

LoginLayer::LoginLayer()
{
    m_oMenuVector.clear();
    m_buttonflag = false;
}


LoginLayer::~LoginLayer()
{
    m_oMenuVector.clear();
}

//void LoginLayer::reconnect(cocos2d::EventCustom *pEvent)
//{
//    PopTips::pop(this, "网络连接失败，请尝试重新连接！");
//    auto pReconnBtn = MenuButton::create("重新连接", [](Ref* pSender){
//        log("点击重新连接按钮。");
//    });
//    pReconnBtn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
//    WZQDeviceSize::setPosAtNodeLeftTop(pReconnBtn, this);
//    
//    auto pMenu = Menu::create(pReconnBtn, NULL);
//    pMenu->setPosition(Vec2::ZERO);
//    addChild(pMenu);
//}




