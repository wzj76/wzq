//
//  MatchWaitting.hpp
//  wzqgame-mobile
//
//  Created by duole on 2024/1/25.
//

#ifndef MatchWaitting_hpp
#define MatchWaitting_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "AppRes.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "Event/WZQEventWrapper.h"
#include<string>
USING_NS_CC;
USING_NS_CC_EXT;
class MatchWaittingLayer : public cocos2d::Layer,WZQEventWrapper
{
public:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(MatchWaittingLayer);
private:
    void onMenuCancel(Ref* pSender);
    void onMenuSubmit(Ref* pSender);
    float m_fTimeElapsed; // 用于跟踪经过的时间
    Label* m_pTimerLabel; // 显示时间的标签
public:
    void onMenuEsc(Ref* pSender);
    
    void matchSuccess(cocos2d::EventCustom* pEvent);
    
    void updateTimer(float dt);
};
#endif /* MatchWaitting_hpp */
