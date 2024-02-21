//
//  WaitAckLayer.cpp
//  wzqgame-mobile
//
//  Created by duole on 2024/1/31.
//

#include "WaitAckLayer.hpp"
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

bool WaitAckLayer::init()
{
    if(!Layer::init())
    {
        return false;
    }
    auto m_pBg = Sprite::create(AppModel::getInstance()->m_pUserModel->getThemePath() + BG_POP);
    auto size = m_pBg->getContentSize();
    m_pBg->setAnchorPoint(Vec2(0.5,0));
    addChild(m_pBg,-1);
    auto vsize = Director::getInstance()->getVisibleSize();
    m_pBg->setPosition(Vec2(vsize.width/2,vsize.height*0.15));
    m_pBg -> setScale(WZQDeviceSize::getContentScaleInHeight(m_pBg, 0.7));
    
    auto pUndoLable = Label::createWithTTF( "正在等待对方的答复...", FONT_GUHUANG, WZQDeviceSize::getFontSize() + 10);
    pUndoLable -> setColor(Color3B::ORANGE);
    pUndoLable -> setPosition(WZQDeviceSize::getViewCenterPos().x,  WZQDeviceSize::getViewCenterPos().y + 4 * m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 10);
    addChild(pUndoLable);
    
    this -> scheduleOnce(schedule_selector(WaitAckLayer::AutoRemove),10);
    
    return true;
}

void WaitAckLayer::onEnter()
{
    Layer::onEnter();
    ADD_OBSERVER(WaitAckLayer::RemoveLayer, REMOVE_WAIT_ACK_LAYER);
}

void WaitAckLayer::onExit()
{
    Layer::onExit();
}

WaitAckLayer::WaitAckLayer()
{
}

WaitAckLayer::~WaitAckLayer() noexcept
{
}

void WaitAckLayer::RemoveLayer(EventCustom* pEvent){
    removeFromParent();
}
void WaitAckLayer::AutoRemove(float t)
{
    removeFromParent();
}
