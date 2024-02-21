//
//  AskContinueGameLayer.cpp
//  wzqgame-mobile
//
//  Created by duole on 2024/2/6.
//

#include "AskContinueGameLayer.hpp"
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

bool AskContinueGameLayer::init()
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
    
    auto pUndoLable = Label::createWithTTF( "对方希望继续和你游戏，是否同意", FONT_GUHUANG, WZQDeviceSize::getFontSize() + 10);
    pUndoLable -> setColor(Color3B::ORANGE);
    pUndoLable -> setPosition(WZQDeviceSize::getViewCenterPos().x,  WZQDeviceSize::getViewCenterPos().y + 4 * m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 10);
    addChild(pUndoLable);
    
    // 确定按钮
    auto MenuButtonComfirm = MenuButton::create("同意", CC_CALLBACK_1(AskContinueGameLayer::onAgreeClicked, this));
    MenuButtonComfirm -> setPosition(Vec2(vsize.width * 0.4, vsize.height * 0.1));
    
    auto MenuButtonRefuse = MenuButton::create("拒绝", CC_CALLBACK_1(AskContinueGameLayer::onRefuseClicked, this));
    MenuButtonRefuse -> setPosition(Vec2(vsize.width * 0.6, vsize.height * 0.1));
    m_pMenu = Menu::create(MenuButtonComfirm,MenuButtonRefuse, NULL);

    m_pMenu->setPosition(Vec2::ZERO);
    m_pMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    addChild(m_pMenu);
    this -> scheduleOnce(schedule_selector(AskContinueGameLayer::AutoRefuse),10);
    
    return true;
}

void AskContinueGameLayer::onEnter()
{
    Layer::onEnter();
    
}

void AskContinueGameLayer::onExit()
{
    Layer::onExit();
}

AskContinueGameLayer::AskContinueGameLayer()
{
}

AskContinueGameLayer::~AskContinueGameLayer() noexcept
{
}

void AskContinueGameLayer::onAgreeClicked(cocos2d::Ref *pSender) {
    log("点击同意按钮");
    ACKAskContinueGame ack;
    ack.set_is_agree_continue_game(true);
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    ack.set_user_id(usrinfo.nUserId);
    ack.set_enemy_user_id(m_enemy_id);
    ack.set_table_id(AppModel::getInstance() -> m_pInGameModel -> m_gameConf.iRoomId);
    
    dispatchEvent(ACK_REQ_ASK_CONTINUE_GAME,&ack);
    removeFromParent();
}

void AskContinueGameLayer::onRefuseClicked(cocos2d::Ref *pSender)
{
    log("点击拒绝按钮");
    ACKAskContinueGame ack;
    ack.set_is_agree_continue_game(false);
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    ack.set_user_id(usrinfo.nUserId);
    ack.set_enemy_user_id(m_enemy_id);
    ack.set_table_id(AppModel::getInstance() -> m_pInGameModel -> m_gameConf.iRoomId);
    dispatchEvent(ACK_REQ_ASK_CONTINUE_GAME,&ack);
    removeFromParent();
}

void AskContinueGameLayer::AutoRefuse(float t)
{
    
    log("点击拒绝按钮");
    ACKAskContinueGame ack;
    ack.set_is_agree_continue_game(false);
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    ack.set_user_id(usrinfo.nUserId);
    ack.set_enemy_user_id(m_enemy_id);
    ack.set_table_id(AppModel::getInstance() -> m_pInGameModel -> m_gameConf.iRoomId);
    dispatchEvent(ACK_REQ_ASK_CONTINUE_GAME,&ack);
    removeFromParent();
}
