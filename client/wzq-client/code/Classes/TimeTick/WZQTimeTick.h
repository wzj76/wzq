//
//  WZQTimeTick.h
//  ShengJi
//
//  Created by Joi Yao on 15/10/30.
//
//

#pragma once

#include "cocos2d.h"

class WZQTimeTick
{
public:
    WZQTimeTick();
    virtual ~WZQTimeTick();
    
protected:
    void tick(cocos2d::SEL_SCHEDULE selector, cocos2d::Ref* target, float interval);
    void tickOnce(cocos2d::SEL_SCHEDULE selector, cocos2d::Ref* target, float delay);
    void unTick(cocos2d::SEL_SCHEDULE selector, cocos2d::Ref* target);
    void unTickAll();
    
protected:
    cocos2d::Scheduler*                                m_pScheduler;
    cocos2d::Vector<cocos2d::Ref*>                     m_oTargets;
};

