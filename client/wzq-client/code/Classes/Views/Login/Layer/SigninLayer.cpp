//
//  RegistLayer.cpp
//  wzqgame-mobile
//
//  Created by admin on 2022/11/28.
//
#include "SigninLayer.hpp"
#include "Core/AppFunc.h"
#include "Core/AppEvent.h"
#include "Controllers/WZQLoginController.h"
#include "WZQDeviceSize.hpp"
#include "Core/AppModel.h"
#include "MenuButton.hpp"
#include<iostream>
USING_NS_CC;




bool SigninLayer::init()
{
    AppModel::getInstance() -> m_pUserModel -> m_pWaiting -> hide();
    if(!Layer::init())
    {
        return false;
    }
    
    
    std::string username,password;
    AppModel::getInstance()->m_pUserModel->getRecordUser(username, password);
    
    auto bg = Sprite::create(AppModel::getInstance()->m_pUserModel->getThemePath() + BG_POP);
    auto size = bg->getContentSize();
    bg->setAnchorPoint(Vec2(0.5,0));
    bg -> setScale(WZQDeviceSize::getContentScaleInHeight(bg, 0.7));
    addChild(bg,-1);
    auto vsize = Director::getInstance()->getVisibleSize();
    bg->setPosition(Vec2(vsize.width/2,vsize.height*0.15));
    auto userNameInput = EditBox::create(Size(600,60), Scale9Sprite::create());
    userNameInput->setFontSize(WZQDeviceSize::getFontSize());
    userNameInput->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    userNameInput->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DEFAULT);
    userNameInput->setDelegate(this);
    userNameInput->setPosition(Vec2(vsize.width/3*2,vsize.height/8*5));
    userNameInput->setTag(0);
    userNameInput->setPlaceHolder("请输入用户名");
    userNameInput->setText(username.c_str()); // 设置默认用户名
    value[0] = username;
    
    userNameInput->setPlaceholderFontSize(WZQDeviceSize::getFontSize());
    addChild(userNameInput,1);
    auto pwdInput = EditBox::create(Size(600,60), Scale9Sprite::create());
    pwdInput->setFontSize(WZQDeviceSize::getFontSize());
    pwdInput->setPosition(Vec2(vsize.width/3*2,vsize.height/8*4));
    pwdInput->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    pwdInput->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
    pwdInput->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DEFAULT);
    pwdInput->setDelegate(this);
    pwdInput->setTag(1);
    pwdInput->setPlaceHolder("请输入密码");
    pwdInput->setText(password.c_str()); // 设置默认密码
    value[1] = password;
    
    pwdInput->setPlaceholderFontSize(WZQDeviceSize::getFontSize());
    addChild(pwdInput,1);

    auto nameLabel = Label::createWithTTF("用户名:", FONT_GUHUANG, WZQDeviceSize::getFontSize());
    nameLabel->setAnchorPoint(Vec2(0,0.5));
    nameLabel->setPosition(Vec2(vsize.width/2-vsize.width * 0.15,userNameInput->getPositionY()));
    addChild(nameLabel);
    auto pwdLabel = Label::createWithTTF("密  码:", FONT_GUHUANG, WZQDeviceSize::getFontSize());
    pwdLabel->setAnchorPoint(Vec2(0,0.5));
    pwdLabel->setPosition(Vec2(vsize.width/2-vsize.width * 0.15,pwdInput->getPositionY()));
    addChild(pwdLabel);
    
    auto pMenuItemBtnSubmit = MenuButton::create("提交", CC_CALLBACK_1(SigninLayer::onMenuSubmit, this));
    pMenuItemBtnSubmit->setPosition(Vec2(vsize.width * 0.4, vsize.height * 0.1));
    
    auto pMenuItemBtnCancel = MenuButton::create("取消", CC_CALLBACK_1(SigninLayer::onMenuCancel, this));
    pMenuItemBtnCancel->setPosition(Vec2(vsize.width * 0.6, vsize.height * 0.1));
    
    
    auto pMenu = Menu::create(pMenuItemBtnSubmit, pMenuItemBtnCancel, NULL);
    pMenu->setPosition(Vec2::ZERO);
    pMenu->setAnchorPoint(Vec2::ZERO);
    addChild(pMenu);
    
    
    //触发一次自动登录
    //defaultSignin();

    
    return true;
}
void SigninLayer::onMenuCancel(cocos2d::Ref *pSender)
{
    dispatchEvent(EVT_CLOSE_REGISTE_LAYER);
    removeFromParent();
}
void SigninLayer::onMenuSubmit(Ref* pSender)
{
    printf("登录信息:%s\n%s\n",value[0].c_str(),value[1].c_str());
    std::string username=value[0];
    std::string password=value[1];
    //对登录信息进行校验
    if(username==""||password=="")
    {
        cocos2d::MessageBox("用户名或密码为空", "重新输入");
    }
    else
    {
        AppModel::getInstance() -> m_pUserModel -> m_pWaiting -> show();
        ValueMap oData;
        oData.insert(make_pair("name",value[0]));
        oData.insert(make_pair("pw",value[1]));
        dispatchEvent(EVT_LOGIN,&oData);
        std::string username=value[0];
        std::string password=value[1];
    }
}



void SigninLayer::onEnter()
{
    Layer::onEnter();
}
void SigninLayer::onExit()
{
    Layer::onExit();
}

void SigninLayer::editBoxTextChanged(EditBox *editBox, const std::string &text)
{
    int index = editBox->getTag();
    value[index] = text;
}
void SigninLayer::editBoxEditingDidBegin(EditBox* editBox)
{
    
}
void SigninLayer::editBoxEditingDidEnd(EditBox* editBox)
{
    int index = editBox->getTag();
    value[index] = editBox->getText();
   
}
void SigninLayer::editBoxReturn(EditBox* editBox)
{
    int index = editBox->getTag();
    value[index] = editBox->getText();
    printf("%d %s\n",index,value[index].c_str());
}
