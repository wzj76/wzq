//
//  WinOrLoseLayer.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/30.
//

#include "WinOrLoseLayer.hpp"
#include "Core/AppFunc.h"
#include "Core/AppEvent.h"
#include "Core/AppModel.h"
#include "WZQDeviceSize.hpp"
#include "Controllers/WZQLoginController.h"
#include "LobbyScene.hpp"
#include <iostream>
#include <string>
#include <sstream>
USING_NS_CC;

bool WinOrLoseLayer::init()
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
    
    // 你赢（输）了几个字
    auto pRankingLable = Label::createWithTTF(AppModel::getInstance()->m_pInGameModel -> m_IsMeWin ? "你 赢 了 ！" : "你 输 了 ……", FONT_GUHUANG, WZQDeviceSize::getFontSize() + 10);
    pRankingLable -> setColor(Color3B::ORANGE);
    pRankingLable -> setPosition(WZQDeviceSize::getViewCenterPos().x,  WZQDeviceSize::getViewCenterPos().y + 4 * m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 10);
    addChild(pRankingLable);
    
    // 分数变化情况
    Label* pScoreLable;
    if (AppModel::getInstance()->m_pInGameModel -> m_IsMeWin)
        pScoreLable = Label::createWithSystemFont("分数：" + std::to_string( AppModel::getInstance() -> m_pInGameModel -> getMyUserInfo().Score) + " + 100", "黑体", WZQDeviceSize::getFontSize() + 10);
    else
        pScoreLable = Label::createWithSystemFont("分数：" + std::to_string( AppModel::getInstance() -> m_pInGameModel -> getMyUserInfo().Score) + " - 100", "黑体", WZQDeviceSize::getFontSize() + 10);
    pScoreLable -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    WZQDeviceSize::setPosAtNodeBottomMiddle(pScoreLable, pRankingLable);
    WZQDeviceSize::setMoveNode(Vec2(0, - m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 4), pScoreLable);
    addChild(pScoreLable);
    
    
    // 确定按钮
    auto MenuButtonComfirm = MenuButton::create("确定", CC_CALLBACK_1(WinOrLoseLayer::onComfrimClicked, this));
    MenuButtonComfirm -> setPosition(WZQDeviceSize::getViewCenterPos().x, WZQDeviceSize::getViewCenterPos().y - m_pBg -> getContentSize().height * m_pBg -> getScaleY() /2);
    
    m_pMenu = Menu::create(MenuButtonComfirm, NULL);

    m_pMenu->setPosition(Vec2::ZERO);
    m_pMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    addChild(m_pMenu);
    
    return true;
}

void WinOrLoseLayer::onEnter()
{
    Layer::onEnter();
    ADD_OBSERVER(WinOrLoseLayer::goBackToLobby, LOGIN_SUCCESS);
}

void WinOrLoseLayer::onExit()
{
    Layer::onExit();
}

WinOrLoseLayer::WinOrLoseLayer()
{
}

WinOrLoseLayer::~WinOrLoseLayer() noexcept
{
}

void WinOrLoseLayer::onComfrimClicked(cocos2d::Ref *pSender) { 
    log("点击确认按钮");
    std::string username,password;
    AppModel::getInstance()->m_pUserModel->m_pWaiting -> show(10, "等待服务器响应……");
    
    // TODO: 更新Model
//    AppModel::getInstance()->m_pUserModel->getRecordUser(username, password);
//    if(username!=""&&password!="")
//    {
//        //自动登录更新model
//        ValueMap oData;
//        oData.insert(make_pair("name",username));
//        oData.insert(make_pair("pw",password));
//        dispatchEvent(EVT_LOGIN,&oData);
//    }
    EventCustom *pEvent;
    goBackToLobby(pEvent);
}

void WinOrLoseLayer::goBackToLobby(EventCustom *pEvent) {
    AppModel::getInstance()->m_pUserModel->m_pWaiting -> hide();
    auto lobby = LobbyScene::createScene();
    Director::getInstance() -> replaceScene(lobby);
}










