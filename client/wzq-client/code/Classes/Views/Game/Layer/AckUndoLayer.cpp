//
//  AckUndoLayer.cpp
//  wzqgame-mobile
//
//  Created by duole on 2024/1/31.
//

#include "AckUndoLayer.hpp"
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

bool AckUndoLayer::init()
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
    
    auto pUndoLable = Label::createWithTTF( "对象希望悔棋是否同意", FONT_GUHUANG, WZQDeviceSize::getFontSize() + 10);
    pUndoLable -> setColor(Color3B::ORANGE);
    pUndoLable -> setPosition(WZQDeviceSize::getViewCenterPos().x,  WZQDeviceSize::getViewCenterPos().y + 4 * m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 10);
    addChild(pUndoLable);
    
    // 确定按钮
    auto MenuButtonComfirm = MenuButton::create("同意", CC_CALLBACK_1(AckUndoLayer::onAgreeClicked, this));
    MenuButtonComfirm -> setPosition(Vec2(vsize.width * 0.4, vsize.height * 0.1));
    
    MenuButtonRefuse = MenuButton::create("拒绝", CC_CALLBACK_1(AckUndoLayer::onRefuseClicked, this));
    MenuButtonRefuse -> setPosition(Vec2(vsize.width * 0.6, vsize.height * 0.1));
    m_pMenu = Menu::create(MenuButtonComfirm,MenuButtonRefuse, NULL);

    m_pMenu->setPosition(Vec2::ZERO);
    m_pMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    addChild(m_pMenu);
    
    // 更新按钮文本
    schedule([this](float dt){
        if (*remainTime >= 0)
        {
            std::string text = "拒绝(" + std::to_string((*remainTime)--) +")";
            MenuButtonRefuse -> setText(text);
        }
        // 10秒后执行的程序
        else
        {
            AutoRefuse(1);
        }
    }, 1.0f, CC_REPEAT_FOREVER, 0.0f, "refuse");
    
    return true;
}

void AckUndoLayer::onEnter()
{
    Layer::onEnter();
    //防止无法删除
    ADD_OBSERVER(AckUndoLayer::Reomve, REMOVE_LAYER);
}

void AckUndoLayer::onExit()
{
    Layer::onExit();
}

AckUndoLayer::AckUndoLayer()
{
    remainTime = new int(10);
}

AckUndoLayer::~AckUndoLayer() noexcept
{
    delete remainTime;
    remainTime = nullptr;
}

void AckUndoLayer::onAgreeClicked(cocos2d::Ref *pSender) {
    log("点击同意按钮");
    ACKAskUndoChess ask;
    ask.set_table_id(AppModel::getInstance() -> m_pInGameModel -> m_gameConf.iRoomId);
    ask.set_action_id(AppModel::getInstance() -> m_pInGameModel -> getOppoUserInfo().nUserId);
    ask.set_is_agree_undo_chess(true);
    dispatchEvent(ACK_ASK_USER_UNDO_CHESS,&ask);
    removeFromParent();
}

void AckUndoLayer::onRefuseClicked(cocos2d::Ref *pSender) {
    log("点击拒绝按钮");
    ACKAskUndoChess ask;
    ask.set_table_id(AppModel::getInstance() -> m_pInGameModel -> m_gameConf.iRoomId);
    ask.set_action_id(AppModel::getInstance() -> m_pInGameModel -> getOppoUserInfo().nUserId);
    ask.set_is_agree_undo_chess(false);
    dispatchEvent(ACK_ASK_USER_UNDO_CHESS,&ask);
    //dispatchEvent(EVT_UNDO_CHESS_FAIL);
    //removeFromParent();
}

void AckUndoLayer::AutoRefuse(float t){
    
    log("点击拒绝按钮");
    ACKAskUndoChess ask;
    ask.set_table_id(AppModel::getInstance() -> m_pInGameModel -> m_gameConf.iRoomId);
    ask.set_action_id(AppModel::getInstance() -> m_pInGameModel -> getOppoUserInfo().nUserId);
    ask.set_is_agree_undo_chess(false);
    dispatchEvent(ACK_ASK_USER_UNDO_CHESS,&ask);
    //dispatchEvent(EVT_UNDO_CHESS_FAIL);
    //removeFromParent();
}

void AckUndoLayer::Reomve(cocos2d::Ref *pSender)
{
    dispatchEvent(EVT_UNDO_CHESS_FAIL);
    removeFromParent();
}
