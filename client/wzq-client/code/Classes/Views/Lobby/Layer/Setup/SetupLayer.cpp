//
//  SetupLayer.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/6.
//

#include "SetupLayer.hpp"
#include "ChooseHeadLayer.hpp"
#include "Core/AppFunc.h"
#include "Core/AppEvent.h"
#include "Core/AppModel.h"
#include "Controllers/WZQLoginController.h"
#include "WZQDeviceSize.hpp"
#include "LobbyScene.hpp"
#include "MenuButton.hpp"
#include "LoginLayer.h"
#include "RuleLayer.hpp"

#include <iomanip>
USING_NS_CC;

bool SetupLayer::init()
{
    if(!Layer::init())
    {
        return false;
    }
    m_pMenu = Menu::create();
    m_pMenu->setPosition(Vec2::ZERO);
    addChild(m_pMenu);
    
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    m_pBg = Sprite::create(AppModel::getInstance()->m_pUserModel->getThemePath() + BG_POP);
    auto size = m_pBg->getContentSize();
    addChild(m_pBg,-1);
    auto vsize = Director::getInstance()->getVisibleSize();
    m_pBg->setPosition(WZQDeviceSize::getViewCenterPos());
    m_pBg->setScale(WZQDeviceSize::getContentScaleInHeight(m_pBg, 0.7));
    
    m_pTitleLable = Label::createWithTTF( "设  置", FONT_GUHUANG, WZQDeviceSize::getFontSize() + 10);
    m_pTitleLable -> setColor(Color3B::ORANGE);
    m_pTitleLable -> setPosition(WZQDeviceSize::getViewCenterPos().x,  WZQDeviceSize::getViewCenterPos().y + 4 * m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 10);
    addChild(m_pTitleLable);
    
    auto pMenuButtonExit = MenuButton::create("",ESC_BT, CC_CALLBACK_1(SetupLayer::onCloseClicked, this));
    
    
    m_pMenu -> addChild(pMenuButtonExit);
    WZQDeviceSize::setPosAtNodeRightTop(pMenuButtonExit, m_pBg);
    
    Vec2 startPos = Vec2(m_pBg->getPosition().x -
                         m_pBg->getContentSize().width *
                         m_pBg->getScaleX() * 3 / 8,
                         m_pBg->getPosition().y +
                         m_pBg->getContentSize().height *
                         m_pBg->getScaleY() * 1 / 4);
    
    Vec2 startPos2 = Vec2(m_pBg->getPosition().x +
                         m_pBg->getContentSize().width *
                         m_pBg->getScaleX() * 0 / 8,
                         m_pBg->getPosition().y +
                         m_pBg->getContentSize().height *
                         m_pBg->getScaleY() * 1 / 4);
    
    int spacing = m_pBg->getContentSize().height *
    m_pBg->getScaleY() * 1 / 10;
    
    auto pTitle1 = Label::createWithSystemFont("音效", "黑体", WZQDeviceSize::getFontSize());
    pTitle1->setScale(WZQDeviceSize::getContentScaleInHeight(pTitle1, 0.05f));
    pTitle1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    pTitle1->setPosition(startPos);
    addChild(pTitle1);
    
    auto pSubtitle1 = Label::createWithSystemFont("背景音乐", "黑体", WZQDeviceSize::getFontSize());
    pSubtitle1->setScale(WZQDeviceSize::getContentScaleInHeight(pTitle1, 0.04f));
    pSubtitle1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    WZQDeviceSize::setPosAtNodeLeftBottom(pSubtitle1, pTitle1);
    addChild(pSubtitle1);
    
    bgmusic = CandidateBox::create();
    bgmusic->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    WZQDeviceSize::setPosAtNodeRightMiddle(bgmusic, pSubtitle1);
    bgmusic->setScale(WZQDeviceSize::getContentScaleInHeight(bgmusic, 0.04f));
    bgmusic->setSelectedCallback([](Ref* pSender){
        log("选中了复选框");
    });
    bgmusic->setUnselectedCallback([](Ref* pSender){
        log("取消了复选框");
    });
    addChild(bgmusic);
    
    auto pSubtitle12 = Label::createWithSystemFont("按钮及落子音效", "黑体", WZQDeviceSize::getFontSize());
    pSubtitle12->setScale(WZQDeviceSize::getContentScaleInHeight(pSubtitle12, 0.04f));
    pSubtitle12->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    pSubtitle12->setPosition(Vec2(startPos2.x,
                                  pSubtitle1->getPosition().y));
    addChild(pSubtitle12);
    
    bgeff = CandidateBox::create();
    bgeff->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    WZQDeviceSize::setPosAtNodeRightMiddle(bgeff, pSubtitle12);
    bgeff->setScale(WZQDeviceSize::getContentScaleInHeight(bgeff, 0.04f));
    bgeff->setSelectedCallback([](Ref* pSender){
        log("选中了复选框");
    });
    bgeff->setUnselectedCallback([](Ref* pSender){
        log("取消了复选框");
    });
    addChild(bgeff);
    
    auto pTitle2 = Label::createWithSystemFont("视效", "黑体", WZQDeviceSize::getFontSize());
    pTitle2->setScale(WZQDeviceSize::getContentScaleInHeight(pTitle2, 0.05f));
    pTitle2->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    WZQDeviceSize::setPosAtNodeLeftBottom(pTitle2, pSubtitle1);
    WZQDeviceSize::setMoveNode(Vec2(0, -spacing), pTitle2);
    addChild(pTitle2);
    
    auto pSubtitle2 = Label::createWithSystemFont("按钮动画", "黑体", WZQDeviceSize::getFontSize());
    pSubtitle2->setScale(WZQDeviceSize::getContentScaleInHeight(pTitle2, 0.04f));
    pSubtitle2->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    WZQDeviceSize::setPosAtNodeLeftBottom(pSubtitle2, pTitle2);
    addChild(pSubtitle2);
    
    btsetup = CandidateBox::create();
    btsetup->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    WZQDeviceSize::setPosAtNodeRightMiddle(btsetup, pSubtitle2);
    btsetup->setScale(WZQDeviceSize::getContentScaleInHeight(btsetup, 0.04f));
    btsetup->setSelectedCallback([](Ref* pSender){
        log("选中了复选框");
    });
    btsetup->setUnselectedCallback([](Ref* pSender){
        log("取消了复选框");
    });
    addChild(btsetup);
    
    auto pSubtitle22 = Label::createWithSystemFont("落子动画", "黑体", WZQDeviceSize::getFontSize());
    pSubtitle22->setScale(WZQDeviceSize::getContentScaleInHeight(pSubtitle22, 0.04f));
    pSubtitle22->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    pSubtitle22->setPosition(Vec2(startPos2.x,
                                  pSubtitle2->getPosition().y));
    addChild(pSubtitle22);
    
    putchessbtn = CandidateBox::create();
    putchessbtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    WZQDeviceSize::setPosAtNodeRightMiddle(putchessbtn, pSubtitle22);
    putchessbtn->setScale(WZQDeviceSize::getContentScaleInHeight(putchessbtn, 0.04f));
    putchessbtn->setSelectedCallback([](Ref* pSender){
        log("选中了复选框");
    });
    putchessbtn->setUnselectedCallback([](Ref* pSender){
        log("取消了复选框");
    });
    addChild(putchessbtn);

    auto privacyPolicyBtn = MenuButton::create("隐私协议", [](Ref* pSender){
        log("点击了隐私协议");
        PlatformFunction::showPrivacyGuide();
    });
    privacyPolicyBtn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    WZQDeviceSize::setPosAtNodeLeftBottom(privacyPolicyBtn, pSubtitle2);
    WZQDeviceSize::setMoveNode(Vec2(0, -spacing), privacyPolicyBtn);
    WZQDeviceSize::setAnchorNoMove(privacyPolicyBtn);
    privacyPolicyBtn->setPosition(Vec2(m_pBg->getPosition().x, privacyPolicyBtn->getPositionY()));
    m_pMenu->addChild(privacyPolicyBtn);
    
    auto logOutBtn = MenuButton::create("退出登录", CC_CALLBACK_1(SetupLayer::onExitGameClicked, this));
    logOutBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    WZQDeviceSize::setPosAtNodeRightMiddle(logOutBtn, privacyPolicyBtn);
    WZQDeviceSize::setMoveNode(Vec2(spacing/2, 0), logOutBtn);
    m_pMenu->addChild(logOutBtn);
    
    auto ruleBtn = MenuButton::create("游戏规则介绍", CC_CALLBACK_1(SetupLayer::onRuleBtnClicked, this));
    ruleBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    WZQDeviceSize::setPosAtNodeLeftMiddle(ruleBtn, privacyPolicyBtn);
    WZQDeviceSize::setMoveNode(Vec2(-spacing/2, 0), ruleBtn);
    m_pMenu->addChild(ruleBtn);
    

    WZQDeviceSize::setAnchorNoMove(logOutBtn);
    WZQDeviceSize::setAnchorNoMove(ruleBtn);
    
    updateInfo();
    
    return true;
}

void SetupLayer::onEnter()
{
    Layer::onEnter();
}

void SetupLayer::onExit()
{
    Layer::onExit();
}

SetupLayer::SetupLayer()
{
}

SetupLayer::~SetupLayer() noexcept
{
}

void SetupLayer::onExitGameClicked(cocos2d::Ref *pSender)
{
    log("点击退出游戏");
    
    
    auto LoginScene = LoginLayer :: createScene();
    Director::getInstance() -> replaceScene(LoginScene);
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    ValueMap oData;
    oData.insert(std::make_pair("user_id",(int)usrinfo.nUserId));
    dispatchEvent(EVT_EXIT_ACCOUNT,&oData);
}

void SetupLayer::onCloseClicked(cocos2d::Ref *pSender)
{
    SETUPCONF conf;
//    ValueMap oData;
//    oData.insert(std::make_pair("background_music", bgmusic -> isSelected()));
//    oData.insert(std::make_pair("sound_effect", bgeff -> isSelected()));
//    oData.insert(std::make_pair("button_effect", btsetup -> isSelected()));
//    oData.insert(std::make_pair("chess_effect", putchessbtn -> isSelected()));
    conf.background_music = bgmusic->isSelected();
    conf.sound_effect = bgeff->isSelected();
    conf.button_effect = btsetup->isSelected();
    conf.chess_effect = putchessbtn->isSelected();
    AppModel::getInstance()->m_pUserModel->setSetupConf(conf);
    dispatchEvent(BUTTONS_BACK);
    removeFromParent();

}

void SetupLayer::updateInfo()
{
    SETUPCONF conf = AppModel::getInstance()->m_pUserModel->m_setupConf;
    if (conf.background_music)
        bgmusic->setSelected(true);
    if (conf.sound_effect)
        bgeff->setSelected(true);
    if (conf.button_effect)
        btsetup->setSelected(true);
    if (conf.chess_effect)
        putchessbtn->setSelected(true);
}

void SetupLayer::onRuleBtnClicked(cocos2d::Ref *pSender) {
    log("点击了规则介绍按钮");
    auto layer = RuleLayer::create();
    addChild(layer);
}



