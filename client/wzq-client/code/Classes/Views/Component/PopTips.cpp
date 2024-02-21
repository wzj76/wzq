//
//  PopTips.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/4.
//

#include "PopTips.hpp"
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

bool PopTips::init()
{
    if(!Sprite::init())
    {
        return false;
    }
    setTexture(BG_LPOP);
    

    return true;
}

void PopTips::onEnter()
{
    Sprite::onEnter();
}

void PopTips::onExit()
{
    Sprite::onExit();
}


PopTips::PopTips()
{
}

PopTips::~PopTips() noexcept
{
}

PopTips *PopTips::create()
{
    PopTips* ret = new (std::nothrow)PopTips;
    if (ret && ret -> init())
    {
        // 背景

        ret -> setScale(0.5f);
        ret -> autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void PopTips::setText(const std::string text) {
    m_pText -> setString(text);
}

void PopTips::pop(Node* parent, const std::string text, float time) {
    
    // 创建 PopTips 实例
    auto pPop = PopTips::create();
    pPop -> setPosition(WZQDeviceSize::getContentCenterPos(parent));


    // 设置文本
    auto pText = Label::createWithSystemFont(text, "黑体", WZQDeviceSize::getFontSize() * 2);
    pText->setColor(Color3B::RED);
    pText -> setPosition(WZQDeviceSize::getViewCenterPos().x,
                         WZQDeviceSize::getViewCenterPos().y/2);
    
    pText->enableOutline(Color4B::BLACK, 1);
    
    pPop -> setScale(WZQDeviceSize::getCertainPixelScale(pPop, pText -> getContentSize().width * 1.2),WZQDeviceSize::getCertainPixelScale(pPop, pText -> getContentSize().height) * 3);
    
    pText -> setOpacity(0);
    pPop -> setOpacity(0);
    
    parent -> addChild(pPop);
    parent -> addChild(pText, INT_MAX);

    // 创建一个动作序列：先让 PopTips 向上移动并淡入，等待指定的时间，然后淡出并移除
    float duration = 0.5f; // 动作持续时间，可以根据需要调整
    float moveHeight = 50; // 向上移动的距离，可以根据需要调整

    // 向上移动和淡入的复合动作
    auto moveBy = MoveBy::create(duration, Vec2(0, moveHeight));
    auto fadeIn = FadeIn::create(duration);
    auto spawn = Spawn::createWithTwoActions(moveBy, fadeIn);

    // 等待指定的时间
    auto delay = DelayTime::create(time);

    // 淡出并移除 PopTips 和文本标签的动作
    auto fadeOut = FadeOut::create(duration);
    auto removeText = CallFunc::create([pText]() {
        pText->removeFromParentAndCleanup(true);
    });
    
    auto removeBg = CallFunc::create([pPop]() {
        pPop->removeFromParentAndCleanup(true);
    });
    auto pPopsequence = Sequence::create(spawn, delay, fadeOut, removeBg, nullptr);
    
    auto Textsequence = Sequence::create(spawn, delay, fadeOut, removeText, nullptr);

    // 运行动作
    pPop -> runAction(pPopsequence);
    pText -> runAction(Textsequence);
}













