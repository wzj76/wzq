//
//  PHZWaitingLayer.h
//  GouJiProject
//
//  Created by 张旭 on 14-9-19.
//  
//

#pragma once

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "Event/WZQEventWrapper.h"

class WZQWaitingLayer : public cocos2d::Layer, public WZQEventWrapper
{
public:
    static WZQWaitingLayer* sharedInstance();
    
    virtual bool init();
	virtual void onEnter();
	virtual void onExit();
    virtual void update(float delta);
    
    virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
    void show(int nTimeout = 15,std::string sTxt = "");
    void hide();
private:
    WZQWaitingLayer();
	~WZQWaitingLayer();
    
    void onTimeout(float delta);
private:
	bool                            m_bWaiting;
    cocos2d::Sprite*                m_pLoading;
    cocos2d::Label*                 m_pLabel;
};
