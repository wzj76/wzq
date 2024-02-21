//
//  ReminderWindow.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/19.
//

#include "ReminderWindow.hpp"
#include "Core/AppFunc.h"
#include "Core/AppEvent.h"
#include "Core/AppModel.h"
#include "WZQDeviceSize.hpp"
#include "Controllers/WZQLoginController.h"
#include "LobbyScene.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include "paohuziProtocol.pb.h"

USING_NS_CC;

bool ReminderWindow::init()
{
    if(!Layer::init())
    {
        return false;
    }
    m_pBg = Sprite::create(AppModel::getInstance()->m_pUserModel->getThemePath() + BG_POP);
    m_pBg->setAnchorPoint(Vec2(0.5,0));
    addChild(m_pBg,-1);
    auto vsize = Director::getInstance()->getVisibleSize();
    m_pBg->setPosition(Vec2(vsize.width/2,vsize.height*0.15));
    m_pBg -> setScale(WZQDeviceSize::getContentScaleInHeight(m_pBg, 0.7));
    
    m_pMenu = Menu::create();
    m_pTitleLable = Label::createWithTTF( "自定义弹窗", FONT_GUHUANG, WZQDeviceSize::getFontSize() + 10);
    m_pTitleLable -> setColor(Color3B::ORANGE);
    m_pTitleLable -> setPosition(WZQDeviceSize::getViewCenterPos().x,  WZQDeviceSize::getViewCenterPos().y + 4 * m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 10);
    addChild(m_pTitleLable);
    
    m_pContentLable = Label::createWithSystemFont( "自定义文本", "黑体", WZQDeviceSize::getFontSize());
    m_pContentLable -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    m_pContentLable -> setPosition(WZQDeviceSize::getViewCenterPos().x,  WZQDeviceSize::getViewCenterPos().y + 1 * m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 5);
    addChild(m_pContentLable);
    
    addChild(m_pMenu);
    
    return true;
}

void ReminderWindow::onEnter()
{
    Layer::onEnter();
    ADD_OBSERVER(ReminderWindow::AddSuccess, ADD_FRIEND_SUCCESS);
    ADD_OBSERVER(ReminderWindow::IsFriend, IS_FRIEND);
    ADD_OBSERVER(ReminderWindow::NotExist, NOT_EXIST);

}

void ReminderWindow::onExit()
{
    Layer::onExit();
}

ReminderWindow::ReminderWindow(): iButtonNum(0)
{
}

ReminderWindow::~ReminderWindow() noexcept
{
}

void ReminderWindow::setText(std::string title, std::string content) { 
    m_pTitleLable -> setString(title);
    m_pContentLable -> setString(content);
}

void ReminderWindow::addButton(MenuButton *button) {
    iButtonNum++;
    m_Buttons.pushBack(button);
    float diff = m_pBg -> getContentSize().width * m_pBg -> getScaleX() / (1 + iButtonNum);
    int flag = 0;
    for (auto& i : m_Buttons)
    {
        i -> setPosition(WZQDeviceSize::getViewCenterPos().x -
                         m_pBg -> getContentSize().width *
                         m_pBg -> getScaleX() / 2 + (++flag) * diff,
                         WZQDeviceSize::getViewCenterPos().y -
                         m_pBg -> getContentSize().height *
                         m_pBg -> getScaleY() / 3);
    }
}

EditBox* ReminderWindow::addEditBox(std::string placeHoldText)
{
    // 创建 EditBox 的大小
    auto vsize = Director::getInstance() -> getVisibleSize();
    Size editBoxSize = Size(vsize.width/3, vsize.height/10);

    // 创建一个 EditBox
    m_Editbox = ui::EditBox::create(editBoxSize, ui::Scale9Sprite::create());

    // 设置 EditBox 的位置
    m_Editbox->setPosition(WZQDeviceSize::getContentCenterPos(this));

    // 设置输入框的字体大小和颜色
    m_Editbox->setFontColor(Color3B::BLACK);
    m_Editbox->setFontSize(25);

    // 设置占位符的文本和颜色
    m_Editbox->setPlaceHolder(placeHoldText.c_str());
    m_Editbox->setPlaceholderFontColor(Color3B::GRAY);
    m_Editbox->setPlaceholderFontSize(25);

    // 设置输入模式
    m_Editbox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);

    // 设置 EditBox 的委托对象，以处理输入事件
    m_Editbox->setDelegate(this);
    
    this -> addChild(m_Editbox);
    return m_Editbox;


}

void ReminderWindow::editBoxEditingDidBegin(cocos2d::extension::EditBox *editBox) { 
    // 输入框开始编辑
}

void ReminderWindow::editBoxEditingDidEnd(cocos2d::extension::EditBox *editBox) { 
    // 输入框编辑完成
}

void ReminderWindow::editBoxTextChanged(cocos2d::extension::EditBox *editBox, const std::string &text) { 
    // 输入框变化
}

void ReminderWindow::editBoxReturn(cocos2d::extension::EditBox *editBox) { 
    // 输入框退出
}

void ReminderWindow::initButtons() {
    m_pMenu -> removeAllChildren();
    m_pMenu -> setAnchorPoint(Vec2::ZERO);
    m_pMenu -> setPosition(Vec2::ZERO);
    for (auto& i : m_Buttons)
    {
        m_pMenu -> addChild(i);
    }
}

void ReminderWindow::AddSuccess(EventCustom* pEvent){
    removeFromParent();
}
void ReminderWindow::addCloseButton()
{
    auto pMenuButtonExit = MenuButton::create("",ESC_BT, [this](Ref* pSender){
        dispatchEvent(BUTTONS_BACK);
        removeFromParent();
    });
    WZQDeviceSize::setPosAtNodeRightTop(pMenuButtonExit, m_pBg);
    
    m_pMenu -> addChild(pMenuButtonExit);
}



void ReminderWindow::IsFriend(EventCustom* pEvent){
    cocos2d::MessageBox("你们已经是好友了!", "");
}

void ReminderWindow::NotExist(EventCustom* pEvent){
    cocos2d::MessageBox("该用户不存在!", "");
}

void ReminderWindow::delButton(MenuButton *button) { 
    log("删除按钮");
    m_Buttons.erase(std::remove(m_Buttons.begin(), m_Buttons.end(), button), m_Buttons.end());
    iButtonNum--;
    float diff = m_pBg -> getContentSize().width * m_pBg -> getScaleX() / (1 + iButtonNum);
    int flag = 0;
    for (auto& i : m_Buttons)
    {
        i -> setPosition(WZQDeviceSize::getViewCenterPos().x -
                         m_pBg -> getContentSize().width *
                         m_pBg -> getScaleX() / 2 + (++flag) * diff,
                         WZQDeviceSize::getViewCenterPos().y -
                         m_pBg -> getContentSize().height *
                         m_pBg -> getScaleY() / 3);
    }
    initButtons();
}















