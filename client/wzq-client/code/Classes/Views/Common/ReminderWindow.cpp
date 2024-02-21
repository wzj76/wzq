//
//  ReminderWindow.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/19.
//


#include "ReminderWindow.hpp"
#include "WZQDeviceSize.hpp"

ReminderWindow::ReminderWindow()
{
    m_callbackListener = NULL;
    m_callback = NULL;
}

ReminderWindow::~ReminderWindow() noexcept
{
    
}

bool ReminderWindow::init()
{
    if (!cocos2d::Layer::init()) return false;
    return true;
}

ReminderWindow *ReminderWindow::Create(const std::string backgroundImage, const std::string title, const std::string content)
{
    ReminderWindow* Reminder = ReminderWindow::create();
    auto bg = Sprite::create(backgroundImage);
    bg -> setScale(WZQDeviceSize::getContentScaleInHeight(bg, 0.7));
    
    Reminder -> addChild(bg, -1);
    auto pLtitle = Label::create();
    pLtitle -> createWithSystemFont(title, "黑体", 30);
    pLtitle -> setAnchorPoint(Vec2(0.5f, 0.5f));
    pLtitle -> setPosition(bg->getContentSize().width / 2, bg -> getContentSize().height * 0.9);
    bg->addChild(pLtitle);
    
    auto pLcontent = Label::createWithSystemFont(content, "黑体", 30);
    pLcontent -> setAnchorPoint(Vec2(0.5f, 0.5f));
    pLcontent -> setPosition(bg->getContentSize().width / 2, bg -> getContentSize().height * 0.7);
    bg->addChild(pLcontent);
    
    return Reminder;
}

void ReminderWindow::onEnter() { 
    Layer::onEnter();
}

void ReminderWindow::onExit() { 
    Layer::onExit();
}













