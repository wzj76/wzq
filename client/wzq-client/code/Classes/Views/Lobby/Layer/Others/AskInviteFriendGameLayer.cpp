//
//  AskInviteFriendGameLayer.cpp
//  wzqgame-mobile
//
//  Created by duole on 2024/2/18.
//

#include "AskInviteFriendGameLayer.hpp"
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

bool AskInviteFriendGameLayer::init()
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
    
    std::string str = AppModel::getInstance()->m_pUserModel->m_frined_name +" 邀请你游戏，是否同意";
    auto pUndoLable = Label::createWithTTF(str.c_str(), FONT_GUHUANG, WZQDeviceSize::getFontSize() + 10);
    pUndoLable -> setColor(Color3B::ORANGE);
    pUndoLable -> setPosition(WZQDeviceSize::getViewCenterPos().x,  WZQDeviceSize::getViewCenterPos().y + 4 * m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 10);
    addChild(pUndoLable);
    
    // 确定按钮
    auto MenuButtonComfirm = MenuButton::create("同意", CC_CALLBACK_1(AskInviteFriendGameLayer::onAgreeClicked, this));
    MenuButtonComfirm -> setPosition(Vec2(vsize.width * 0.4, vsize.height * 0.1));
    
    auto MenuButtonRefuse = MenuButton::create("拒绝", CC_CALLBACK_1(AskInviteFriendGameLayer::onRefuseClicked, this));
    MenuButtonRefuse -> setPosition(Vec2(vsize.width * 0.6, vsize.height * 0.1));
    m_pMenu = Menu::create(MenuButtonComfirm,MenuButtonRefuse, NULL);

    m_pMenu->setPosition(Vec2::ZERO);
    m_pMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    addChild(m_pMenu);
    this -> scheduleOnce(schedule_selector(AskInviteFriendGameLayer::AutoRefuse),10);
    
    return true;
}

void AskInviteFriendGameLayer::onEnter()
{
    Layer::onEnter();
    
}

void AskInviteFriendGameLayer::onExit()
{
    Layer::onExit();
}

AskInviteFriendGameLayer::AskInviteFriendGameLayer()
{
}

AskInviteFriendGameLayer::~AskInviteFriendGameLayer() noexcept
{
}

void AskInviteFriendGameLayer::onAgreeClicked(cocos2d::Ref *pSender) {
    log("点击同意按钮");
    ACKAskInviteFriendGame ack;
    ack.set_is_agree_invite_game(true);
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    UserInfo user;
    user.set_id(usrinfo.nUserId);
    user.set_username(usrinfo.sUser);
    user.set_wintimes(usrinfo.Wintimes);
    user.set_losetimes(usrinfo.Losetimes);
    user.set_score(usrinfo.Score);
    user.set_head_id(usrinfo.head);
    ack.mutable_userinfo()->CopyFrom(user);
    
    ack.set_friend_id(m_friend_id);
    ack.set_result_msg("同意邀请");
    dispatchEvent(EVT_ACK_REQ_ASK_INVITE_FRIEND_GAME,&ack);
    removeFromParent();
}

void AskInviteFriendGameLayer::onRefuseClicked(cocos2d::Ref *pSender)
{
    log("点击拒绝按钮");
    ACKAskInviteFriendGame ack;
    ack.set_is_agree_invite_game(false);
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    UserInfo user;
    user.set_id(usrinfo.nUserId);
    user.set_username(usrinfo.sUser);
    user.set_wintimes(usrinfo.Wintimes);
    user.set_losetimes(usrinfo.Losetimes);
    user.set_score(usrinfo.Score);
    user.set_head_id(usrinfo.head);
    ack.mutable_userinfo()->CopyFrom(user);
    
    ack.set_friend_id(m_friend_id);
    ack.set_result_msg("拒绝邀请");
    dispatchEvent(EVT_ACK_REQ_ASK_INVITE_FRIEND_GAME,&ack);
    removeFromParent();
}

void AskInviteFriendGameLayer::AutoRefuse(float t)
{
    
    log("点击拒绝按钮");
    ACKAskInviteFriendGame ack;
    ack.set_is_agree_invite_game(false);
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    UserInfo user;
    user.set_id(usrinfo.nUserId);
    user.set_username(usrinfo.sUser);
    user.set_wintimes(usrinfo.Wintimes);
    user.set_losetimes(usrinfo.Losetimes);
    user.set_score(usrinfo.Score);
    user.set_head_id(usrinfo.head);
    ack.mutable_userinfo()->CopyFrom(user);
    
    ack.set_friend_id(m_friend_id);
    ack.set_result_msg("拒绝邀请");
    dispatchEvent(EVT_ACK_REQ_ASK_INVITE_FRIEND_GAME,&ack);
    removeFromParent();
}
