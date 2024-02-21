//
//  MatchWaitting.cpp
//  wzqgame-mobile
//
//  Created by duole on 2024/1/25.
//

#include "MatchWaitting.hpp"

#include "Core/AppFunc.h"
#include "Core/AppEvent.h"
#include "Core/AppModel.h"
#include "Controllers/WZQLoginController.h"
#include <iostream>
#include "SigninLayer.hpp"
#include "WZQDeviceSize.hpp"
#include <sstream>
#include <string>
#include <iomanip>
#include "MatchingScene.hpp"



USING_NS_CC;

bool MatchWaittingLayer::init()
{
    if(!Layer::init())
    {
        return false;
    }
    m_fTimeElapsed = 0.0f;
    
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    auto bg = Sprite::create(AppModel::getInstance()->m_pUserModel->getThemePath() + BG_POP);
    auto size = bg->getContentSize();
    bg->setAnchorPoint(Vec2(0.5,0));
    addChild(bg,-1);
    auto vsize = Director::getInstance()->getVisibleSize();
    bg->setPosition(Vec2(vsize.width/2,vsize.height*0.15));
    bg -> setScale(WZQDeviceSize::getContentScaleInHeight(bg, 0.7));
    
    
    Sprite* pBtNormalnEsc = Sprite::create(ESC_BT);
    Sprite* pBtnSelectEsc = Sprite::create(ESC_BT);
    pBtnSelectEsc->setColor(Color3B::GRAY);
    
    auto pMenuItemBtnEsc = MenuItemSprite::create(pBtNormalnEsc,pBtnSelectEsc, CC_CALLBACK_1(MatchWaittingLayer::onMenuEsc, this));
    pMenuItemBtnEsc->setScale(WZQDeviceSize::getContentScaleInHeight(pMenuItemBtnEsc,0.1f));
    WZQDeviceSize::setPosAtNodeRightTop(pMenuItemBtnEsc, bg);
    
    
    auto pMenu = Menu::create(pMenuItemBtnEsc, NULL);
    pMenu->setPosition(Vec2::ZERO);
    pMenu->setAnchorPoint(Vec2::ZERO);
    addChild(pMenu);
    
    m_pTimerLabel = Label::createWithSystemFont("正在匹配", "黑体", WZQDeviceSize::getFontSize());
    m_pTimerLabel -> setPosition(WZQDeviceSize::getViewCenterPos());
    addChild(m_pTimerLabel);
    
    this->schedule(CC_SCHEDULE_SELECTOR(MatchWaittingLayer::updateTimer), 1.0f);
    
    return true;
}

void MatchWaittingLayer::onMenuEsc(Ref* pSender){
    //发送一个取消匹配的消息
    printf("发送取消匹配的信息");
    ValueMap oData;
    oData.insert(std::make_pair("message","req_match_stop"));
    dispatchEvent(BUTTONS_BACK);
    dispatchEvent(EVT_MATCH,&oData);
    removeFromParent();
}
void MatchWaittingLayer::matchSuccess(EventCustom* pEvent)
{
    printf("处理匹配成功的逻辑\n");
    // 跳转到游戏中
    auto inGame = MatchingScene::createScene();
    // auto inGame = InGameScene::createScene();
    Director::getInstance() -> replaceScene(inGame);
}

void MatchWaittingLayer::onEnter()
{
    Layer::onEnter();
    ADD_OBSERVER(MatchWaittingLayer::matchSuccess, EVT_MATCH_SUCESS);
}

void MatchWaittingLayer::onExit()
{
    Layer::onExit();
}

void MatchWaittingLayer::updateTimer(float dt) {
    m_fTimeElapsed += dt; // 增加经过的时间

    std::stringstream label;
    label<<"正在匹配,\n\n\n已经过去了"<<std::fixed<<std::setprecision(0)<<m_fTimeElapsed<<"秒……";
    m_pTimerLabel->setString(label.str());
}


