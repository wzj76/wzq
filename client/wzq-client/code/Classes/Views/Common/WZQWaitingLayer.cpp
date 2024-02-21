//
//  PHZWaitingLayer.cpp
//  GouJiProject
//
//  Created by 张旭 on 14-9-19.
//
//

#include "WZQWaitingLayer.h"
#include "Util/Utils.h"
#include "Core/AppEvent.h"

USING_NS_CC;
using namespace util;

static WZQWaitingLayer *s_pSharedInstance = nullptr;

WZQWaitingLayer::WZQWaitingLayer()
:
m_bWaiting(false),
m_pLoading(nullptr),
m_pLabel(nullptr)
{
}

WZQWaitingLayer::~WZQWaitingLayer()
{
    CC_SAFE_RELEASE_NULL(m_pLoading);
}

WZQWaitingLayer* WZQWaitingLayer::sharedInstance()
{
    if (s_pSharedInstance == nullptr)
    {
        s_pSharedInstance = new WZQWaitingLayer();
        s_pSharedInstance->init();
    }
    return s_pSharedInstance;
}

bool WZQWaitingLayer::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!Layer::init());
        
        m_pLoading = CreateSprite("bg/bg_loading.png");
        m_pLoading->retain();
        m_pLoading->setScale(SCALE_FIT);
        m_pLoading->setPosition(Vec2(DEVICE_W / 2, DEVICE_H / 2));
        addChild(m_pLoading);
        
        m_pLabel = GetLabel(26, "");
        m_pLabel->setColor(cocos2d::Color3B(0xff, 0xfc, 0x00));
        m_pLabel->retain();
        m_pLabel->setScale(SCALE_FIT);
        m_pLabel->setPosition(DEVICE_W / 2,m_pLoading->getPositionY() + m_pLoading->getContentSize().height/2 + m_pLabel->getContentSize().height/2);
        addChild(m_pLabel);
        
        auto pEventToucher = EventListenerTouchOneByOne::create();
        pEventToucher->setSwallowTouches(true);
        pEventToucher->onTouchBegan = CC_CALLBACK_2(WZQWaitingLayer::onTouchBegan, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(pEventToucher, this);
        
        // 物理键屏蔽
        auto obKeyBackListener = EventListenerKeyboard::create();
        obKeyBackListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event)
        {
            if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
            {
                event->stopPropagation();
            }
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(obKeyBackListener, this);
        
        bRet = true;
    } while (0);
    
    return bRet;
}

void WZQWaitingLayer::onEnter()
{
    Layer::onEnter();
    
    scheduleUpdate();
}

void WZQWaitingLayer::onExit()
{
    hide();
    unscheduleUpdate();
    
    Layer::onExit();
}

void WZQWaitingLayer::update(float delta)
{
    float fRotation = m_pLoading->getRotation() + 6;
    if (fRotation >= 360)
    {
        fRotation -= 360;
    }
    m_pLoading->setRotation(fRotation);
}

bool WZQWaitingLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    return true;
}

void WZQWaitingLayer::show(int nTimeout,std::string sTxt)
{
    if (m_bWaiting)
    {
        hide();
    }
    m_bWaiting = true;
    Director::getInstance()->getRunningScene()->addChild(this, INT_MAX);
    m_pLoading->setPosition(Vec2(DEVICE_W / 2, DEVICE_H / 2));
    m_pLabel->setPosition(DEVICE_W / 2,m_pLoading->getPositionY() + m_pLoading->getContentSize().height/2 + m_pLabel->getContentSize().height/2);
    if (nTimeout > 0)
    {
        scheduleOnce(schedule_selector(WZQWaitingLayer::onTimeout), nTimeout);
    }
    m_pLabel->setString(sTxt);
}

void WZQWaitingLayer::hide()
{
    if (!m_bWaiting)
    {
        return;
    }
    m_bWaiting = false;
    removeFromParent();
}

void WZQWaitingLayer::onTimeout(float delta)
{
//    Value obData = Value(E_NETWORK_TIMEOUT);
//    dispatchEvent(EVT_COMMON_ERROR, &obData);
    hide();
}
