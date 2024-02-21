#include "Util/BaseLayerUtil.h"

USING_NS_CC;

namespace util
{
    
    bool BaseLayerUtil::init()
    {
        auto pEventToucher = EventListenerTouchOneByOne::create();
        pEventToucher->setSwallowTouches(true);
        pEventToucher->onTouchBegan = CC_CALLBACK_2(BaseLayerUtil::onTouchBegan, this);
        pEventToucher->onTouchEnded = CC_CALLBACK_2(BaseLayerUtil::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(pEventToucher, this);
        return true;
    }
    
    bool BaseLayerUtil::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
    {
        return true;
        
    }
    
    void BaseLayerUtil::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
    {
        
    }
    
    void BaseLayerUtil::onEnter()
    {
        Layer::onEnter();
    }
    
    void BaseLayerUtil::onExit()
    {
        Layer::onExit();
    }
}
