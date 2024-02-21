
#include "Util/ModalLayer.h"

USING_NS_CC;

namespace util
{
	 
    bool ModalLayer::init()
    {
        return initWithColor(Color4B(0x00, 0x00, 0x00, 0xff * 0.48));
    }
    
    bool ModalLayer::initWithColor(const Color4B &color)
    {
        Size s = Director::getInstance()->getWinSize();
        return initWithColor(color, s.width, s.height);
    }
    
    bool ModalLayer::initWithColor(const Color4B &color, GLfloat width, GLfloat height)
    {
        LayerColor::initWithColor(color, width, height);

        auto pEventToucher = EventListenerTouchOneByOne::create();
        pEventToucher->setSwallowTouches(true);
        pEventToucher->onTouchBegan = CC_CALLBACK_2(ModalLayer::onTouchBegan, this);
        pEventToucher->onTouchEnded = CC_CALLBACK_2(ModalLayer::onTouchEnded, this);
        pEventToucher->onTouchMoved = CC_CALLBACK_2(ModalLayer::onTouchMoved, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(pEventToucher, this);
        return true;
    }
    
    bool ModalLayer::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
    {
        return true;

    }
    
    void ModalLayer::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
    {
        
    }
    
    void ModalLayer::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
    {
        
    }
    
    void ModalLayer::onEnter()
    {
        LayerColor::onEnter();
    }
    
    void ModalLayer::onExit()
    {
        LayerColor::onExit();
    }
    
}
