//
//  Bubble.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/4.
//

//
//  Bubble.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/4.
//

#include "Bubble.hpp"
#include <stdio.h>
#include "Core/AppFunc.h"
#include "Core/AppEvent.h"
#include "Core/AppModel.h"
#include "Core/AppRes.h"
#include "WZQDeviceSize.hpp"
#include "Controllers/WZQLoginController.h"
#include <iostream>
#include <string>
#include <sstream>
USING_NS_CC;

bool Bubble::init()
{
    if(!Sprite::init())
    {
        return false;
    }
    setTexture(CHAT_BUBBLE);
    this->setAnchorPoint(Vec2(0.2f, 0));

    return true;
}

void Bubble::onEnter()
{
    Sprite::onEnter();
}

void Bubble::onExit()
{
    Sprite::onExit();
}


Bubble::Bubble()
{
}

Bubble::~Bubble() noexcept
{
}

Bubble *Bubble::create()
{
    Bubble* ret = new (std::nothrow)Bubble;
    if (ret && ret->init())
    {
        // 背景

        ret->setScale(0.5f);
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void Bubble::setText(const std::string text) {
    m_pText->setString(text);
}

void Bubble::pop(Node* parent, const std::string text, Vec2 location, bool isMe) {
    
    // 创建 Bubble 实例
    auto bubble = Bubble::create();
    bubble->setScale(WZQDeviceSize::getContentScaleInWidth(bubble, 0.25f));
    bubble->setAnchorPoint(Vec2(0.2f, 0.0f));
    bubble->setPosition(location);
    
    if (!isMe)
        // 上下反转
        bubble->setScale(bubble->getScaleX(), -bubble->getScaleY());
    
    bool flag = false;
    for (auto& i: AppModel::getInstance()->m_pShopModel->m_emojiStringList)
    {
        if (i.second == text)
        {
            flag = true;
        }
    }
    
    auto pLable = cocos2d::ui::RichText::create();
    WZQDeviceSize::setPosAtNodeMiddle(pLable, bubble);
    // 发送的是表情
    if(flag == true)
    {
        auto emojiElement = cocos2d::ui::RichElementImage::create(1, Color3B::WHITE, 255, AppModel::getInstance()->m_pShopModel->m_String2EmojiList[text]);
        pLable->pushBackElement(emojiElement);
    }
    else
    {
        auto textElement1 = cocos2d::ui::RichElementText::create(1, Color3B::BLACK, 255, text, "黑体", WZQDeviceSize::getFontSize());
        pLable->pushBackElement(textElement1);
    }
    
    // 设置初始透明度为0
    bubble->setOpacity(0);
    pLable->setOpacity(0);

    // 创建渐入动作
    auto fadeIn = FadeIn::create(0.5f);

    // 创建延时动作，保持显示状态1秒
    auto delay = DelayTime::create(2.0f);

    // 创建渐出动作
    auto fadeOut = FadeOut::create(0.5f);

    // 创建动作序列
    auto sequence = Sequence::create(fadeIn, delay, fadeOut, nullptr);

    // 运行动作
    bubble->runAction(sequence->clone()); // 使用 clone() 因为一个动作不能被多个节点共享
    pLable->runAction(sequence->clone());

    // 将节点添加到父节点
    parent->addChild(bubble);
    parent->addChild(pLable);
}
