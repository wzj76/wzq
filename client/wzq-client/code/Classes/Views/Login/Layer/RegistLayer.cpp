//
//  RegistLayer.cpp
//  wzqgame-mobile
//
//  Created by admin on 2022/11/28.
//
#include "RegistLayer.h"
#include "Core/AppFunc.h"
#include "Core/AppEvent.h"
#include "Controllers/WZQLoginController.h"
#include <iostream>
#include "SigninLayer.hpp"
#include "WZQDeviceSize.hpp"
#include "MenuButton.hpp"
#include "AppModel.h"
USING_NS_CC;

bool RegistLayer::init()
{
    if(!Layer::init())
    {
        return false;
    }
    auto bg = Sprite::create(AppModel::getInstance()->m_pUserModel->getThemePath() + BG_POP);
    auto size = bg->getContentSize();
    bg->setAnchorPoint(Vec2(0.5,0));
    addChild(bg,-1);
    auto vsize = Director::getInstance()->getVisibleSize();
    bg->setPosition(Vec2(vsize.width/2,vsize.height*0.15));
    bg -> setScale(WZQDeviceSize::getContentScaleInHeight(bg, 0.7));
    auto userNameInput = EditBox::create(Size(600,60), Scale9Sprite::create());
    userNameInput->setFontSize(WZQDeviceSize::getFontSize());
    userNameInput->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    userNameInput->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DEFAULT);
    userNameInput->setDelegate(this);
    userNameInput->setPosition(Vec2(vsize.width/3*2,vsize.height/8*5));
    userNameInput->setTag(0);
    userNameInput->setPlaceHolder("请输入用户名");
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
    pwdInput->setPlaceholderFontSize(WZQDeviceSize::getFontSize());
    addChild(pwdInput,1);
    auto pwdRepeatInput = EditBox::create(Size(600,60), Scale9Sprite::create());
    pwdRepeatInput->setFontSize(WZQDeviceSize::getFontSize());
    pwdRepeatInput->setPosition(Vec2(vsize.width/3*2,vsize.height/8*3));
    pwdRepeatInput->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    pwdRepeatInput->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
    pwdRepeatInput->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DEFAULT);
    pwdRepeatInput->setDelegate(this);
    pwdRepeatInput->setTag(2);
    pwdRepeatInput->setPlaceHolder("请再次输入密码");
    pwdRepeatInput->setPlaceholderFontSize(WZQDeviceSize::getFontSize());
    addChild(pwdRepeatInput,1);
    auto nameLabel = Label::createWithTTF("用户名:", FONT_GUHUANG, WZQDeviceSize::getFontSize());
    nameLabel->setAnchorPoint(Vec2(0,0.5));
    nameLabel->setPosition(Vec2(vsize.width/2-vsize.width * 0.15,userNameInput->getPositionY()));
    addChild(nameLabel);
    auto pwdLabel =Label::createWithTTF("密  码:", FONT_GUHUANG, WZQDeviceSize::getFontSize());
    pwdLabel->setAnchorPoint(Vec2(0,0.5));
    pwdLabel->setPosition(Vec2(vsize.width/2-vsize.width * 0.15,pwdInput->getPositionY()));
    addChild(pwdLabel);
    auto pwdRepeatLabel = Label::createWithTTF("确认密码:", FONT_GUHUANG, WZQDeviceSize::getFontSize());
    pwdRepeatLabel->setAnchorPoint(Vec2(0,0.5));
    pwdRepeatLabel->setPosition(Vec2(vsize.width/2-vsize.width * 0.15,pwdRepeatInput->getPositionY())); //vsize.width/3*1.1
    addChild(pwdRepeatLabel);
    
    
//    Sprite* pBtnNormalSubmit = Sprite::create(BT_BASEONE);
//    Label* pLabelNormalSubmit = Label::createWithSystemFont("提交", "黑体", 25);
//    pBtnNormalSubmit -> setScale(WZQDeviceSize::getMenuButtonScale(pBtnNormalSubmit));
//    pLabelNormalSubmit->setPosition(pBtnNormalSubmit->getContentSize().width / 2, pBtnNormalSubmit->getContentSize().height / 2);
//    pBtnNormalSubmit->addChild(pLabelNormalSubmit);
//
//    Sprite* pBtnSelectSubmit = Sprite::create(BT_BASEONE);
//    pBtnSelectSubmit->setColor(Color3B::GRAY);
//    Label* pLabelSelectSubmit = Label::createWithSystemFont("提交", "黑体", 25);
//    pBtnSelectSubmit -> setScale(WZQDeviceSize::getMenuButtonScale(pBtnSelectSubmit) * 0.9f);
//    pLabelSelectSubmit->setPosition(pBtnSelectSubmit->getContentSize().width / 2, pBtnSelectSubmit->getContentSize().height / 2);
//    pBtnSelectSubmit->addChild(pLabelSelectSubmit);
//
//    auto pMenuItemBtnSubmit = MenuItemSprite::create(pBtnNormalSubmit, pBtnSelectSubmit, CC_CALLBACK_1(RegistLayer::onMenuSubmit, this));
//    pMenuItemBtnSubmit->setPosition(Vec2(vsize.width * 0.4, vsize.height * 0.1));
    
    auto pMenuItemBtnSubmit = MenuButton::create("提交", CC_CALLBACK_1(RegistLayer::onMenuSubmit, this));
    pMenuItemBtnSubmit->setPosition(Vec2(vsize.width * 0.4, vsize.height * 0.1));
    
//    Sprite* pBtnNormalCancel = Sprite::create(BT_BASEONE);
//    Label* pLabelNormalCancel = Label::createWithSystemFont("取消", "黑体", 25);
//    pBtnNormalCancel -> setScale(WZQDeviceSize::getMenuButtonScale(pBtnNormalCancel));
//    pLabelNormalCancel->setPosition(pBtnNormalCancel->getContentSize().width / 2, pBtnNormalCancel->getContentSize().height / 2);
//    pBtnNormalCancel->addChild(pLabelNormalCancel);
//    Sprite* pBtnSelectCancel = Sprite::create(BT_BASEONE);
//    pBtnSelectCancel->setColor(Color3B::GRAY);
//    Label* pLabelSelectCancel = Label::createWithSystemFont("取消", "黑体", 25);
//    pBtnSelectCancel -> setScale(WZQDeviceSize::getMenuButtonScale(pBtnSelectCancel) * 0.9f);
//    pLabelSelectCancel->setPosition(pBtnSelectCancel->getContentSize().width / 2, pBtnSelectCancel->getContentSize().height / 2);
//    pBtnSelectCancel->addChild(pLabelSelectCancel);
//
//
//    auto pMenuItemBtnCancel = MenuItemSprite::create(pBtnNormalCancel, pBtnSelectCancel, CC_CALLBACK_1(RegistLayer::onMenuCancel, this));
//    pMenuItemBtnCancel->setPosition(Vec2(vsize.width * 0.6, vsize.height * 0.1));
    
    auto pMenuItemBtnCancel = MenuButton::create("取消", CC_CALLBACK_1(RegistLayer::onMenuCancel, this));
    pMenuItemBtnCancel->setPosition(Vec2(vsize.width * 0.6, vsize.height * 0.1));
    
    
    auto pMenu = Menu::create(pMenuItemBtnSubmit, pMenuItemBtnCancel, NULL);
    pMenu->setPosition(Vec2::ZERO);
    pMenu->setAnchorPoint(Vec2::ZERO);
    addChild(pMenu);
    
    return true;
}

void RegistLayer::onMenuCancel(cocos2d::Ref *pSender)
{
    dispatchEvent(EVT_CLOSE_REGISTE_LAYER);
    removeFromParent();
}
void RegistLayer::registSuccess(cocos2d::EventCustom* pEvent)
{
    MessageBox("点击跳转登录界面", "注册成功");
    scheduleOnce([this](float dt) {
        removeFromParent();
    }, 0.1f, "schedule_key");
}


void RegistLayer::onMenuSubmit(Ref* pSender)
{
    printf("用户输入的信息: %s\n%s\n%s\n",value[0].c_str(),value[1].c_str(),value[2].c_str());
    if(value[1]!=value[2])
    {
        MessageBox("两次密码不一致", "重新输入");
    }
    else if(!IsComplexPassword(value[1]))
    {
        MessageBox("密码过于简单", "重新输入");
    }
    else if(value[0]==""||value[1]==""||value[2]=="")
    {
        MessageBox("用户名或密码为空", "重新输入");
    }
    else if(!IsNumberAndChars(value[0]))
    {
        MessageBox("用户名只允许出现英文和数字", "重新输入");
    }
    else
    {
      ValueMap oData;
      oData.insert(make_pair("name",value[0]));
      oData.insert(make_pair("pw",value[1]));
      dispatchEvent(EVT_REGISTER,&oData);
    }
}

void RegistLayer::onEnter()
{
    Layer::onEnter();
    ADD_OBSERVER(RegistLayer::registSuccess, REGISTER_SUCCESS);
}
void RegistLayer::onExit()
{
    Layer::onExit();
}

void RegistLayer::editBoxTextChanged(EditBox *editBox, const std::string &text)
{
    int index = editBox->getTag();
    value[index] = text;
}
void RegistLayer::editBoxEditingDidBegin(EditBox* editBox)
{
    
}
void RegistLayer::editBoxEditingDidEnd(EditBox* editBox)
{
    int index = editBox->getTag();
    value[index] = editBox->getText();
   
}
void RegistLayer::editBoxReturn(EditBox* editBox)
{
    int index = editBox->getTag();
    value[index] = editBox->getText();
}
