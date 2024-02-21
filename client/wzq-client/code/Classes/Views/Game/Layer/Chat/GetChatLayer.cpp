//
//  GetChatLayer.cpp
//  wzqgame-mobile
//
//  Created by duole on 2024/2/4.
//

#include "GetChatLayer.hpp"
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


bool GetChatLayer::init()
{
    if(!Layer::init())
    {
        return false;
    }
    auto m_pBg = Sprite::create(CHAT_BUBBLE);
    auto size = m_pBg->getContentSize();
    m_pBg->setAnchorPoint(Vec2(0.5,0));
    addChild(m_pBg,-1);
    auto vsize = Director::getInstance()->getVisibleSize();
    m_pBg->setPosition(Vec2(0, WZQDeviceSize::getViewCenterPos().y));
    m_pBg -> setScale(WZQDeviceSize::getContentScaleInHeight(m_pBg, 0.7));
    
    return true;
}

void GetChatLayer::onEnter()
{
    Layer::onEnter();
    
}

void GetChatLayer::onExit()
{
    Layer::onExit();
}

GetChatLayer::GetChatLayer()
{
}

GetChatLayer::~GetChatLayer() noexcept
{
}

