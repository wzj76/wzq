//
//  BeatModeChooseLayer.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/18.
//

#include "BeatModeChooseLayer.hpp"
#include "ChooseHeadLayer.hpp"
#include "Core/AppFunc.h"
#include "Core/AppEvent.h"
#include "Core/AppModel.h"
#include "Controllers/WZQLoginController.h"
#include "WZQDeviceSize.hpp"
#include "LobbyScene.hpp"
#include "MenuButton.hpp"
#include "LoginLayer.h"
#include "BotModeScene.hpp"

#include <iomanip>
USING_NS_CC;

bool BeatModeChooseLayer::init()
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
    
    m_pTitleLable = Label::createWithTTF( "双人对战设置", FONT_GUHUANG, WZQDeviceSize::getFontSize() + 10);
    m_pTitleLable -> setColor(Color3B::ORANGE);
    m_pTitleLable -> setPosition(WZQDeviceSize::getViewCenterPos().x,  WZQDeviceSize::getViewCenterPos().y + 4 * m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 10);
    addChild(m_pTitleLable);
    
    Vec2 startPos = Vec2(m_pBg->getPosition().x -
                         m_pBg->getContentSize().width *
                         m_pBg->getScaleX() * 3 / 8,
                         m_pBg->getPosition().y +
                         m_pBg->getContentSize().height *
                         m_pBg->getScaleY() * 1 / 4);
    
    Vec2 startPos2 = Vec2(m_pBg->getPosition().x -
                         m_pBg->getContentSize().width *
                         m_pBg->getScaleX() * 1 / 8,
                         m_pBg->getPosition().y +
                         m_pBg->getContentSize().height *
                         m_pBg->getScaleY() * 1 / 4);
    
    Vec2 startPos3 = Vec2(m_pBg->getPosition().x +
                         m_pBg->getContentSize().width *
                         m_pBg->getScaleX() * 1 / 8,
                         m_pBg->getPosition().y +
                         m_pBg->getContentSize().height *
                         m_pBg->getScaleY() * 1 / 4);
    
    int spacing = m_pBg->getContentSize().height *
    m_pBg->getScaleY() * 1 / 10;
    
    auto pTitle1 = Label::createWithSystemFont("棋盘大小选择", "黑体", WZQDeviceSize::getFontSize());
    pTitle1->setScale(WZQDeviceSize::getContentScaleInHeight(pTitle1, 0.05f));
    pTitle1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    pTitle1->setPosition(startPos);
    addChild(pTitle1);
    
    auto pSubtitle1 = Label::createWithSystemFont("13x13", "黑体", WZQDeviceSize::getFontSize());
    pSubtitle1->setScale(WZQDeviceSize::getContentScaleInHeight(pTitle1, 0.04f));
    pSubtitle1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    WZQDeviceSize::setPosAtNodeLeftBottom(pSubtitle1, pTitle1);
    addChild(pSubtitle1);
    
    auto mode13 = CandidateBox::create();
    mode13->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    WZQDeviceSize::setPosAtNodeRightMiddle(mode13, pSubtitle1);
    mode13->setScale(WZQDeviceSize::getContentScaleInHeight(mode13, 0.04f));
    mode13->setSelectedCallback([this](Ref* pSender){
        onChooseSize(Size13x13);
    });
    mode13->setUnselectedCallback([this](Ref* pSender){
        onChooseSize(Size13x13);
    });
    addChild(mode13);
    mode13->setSelected(true);
    m_HardChooseList.push_back(std::make_pair(Size13x13, mode13));
    
    
    auto pSubtitle13 = Label::createWithSystemFont("15x15", "黑体", WZQDeviceSize::getFontSize());
    pSubtitle13->setScale(WZQDeviceSize::getContentScaleInHeight(pSubtitle13, 0.04f));
    pSubtitle13->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    pSubtitle13->setPosition(Vec2(startPos3.x,
                                  pSubtitle1->getPosition().y));
    addChild(pSubtitle13);
    
    auto mode15 = CandidateBox::create();
    mode15->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    WZQDeviceSize::setPosAtNodeRightMiddle(mode15, pSubtitle13);
    mode15->setScale(WZQDeviceSize::getContentScaleInHeight(mode15, 0.04f));
    mode15->setSelectedCallback([this](Ref* pSender){
        onChooseSize(Size15x15);
    });
    mode15->setUnselectedCallback([this](Ref* pSender){
        onChooseSize(Size15x15);
    });
    addChild(mode15);
    m_HardChooseList.push_back(std::make_pair(Size15x15, mode15));
    
    auto pSubtitle2 = Label::createWithSystemFont("17x17", "黑体", WZQDeviceSize::getFontSize());
    pSubtitle2->setScale(WZQDeviceSize::getContentScaleInHeight(pSubtitle2, 0.04f));
    pSubtitle2->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    WZQDeviceSize::setPosAtNodeLeftBottom(pSubtitle2, pSubtitle1);
    WZQDeviceSize::setMoveNode(Vec2(0, -spacing), pSubtitle2);
    addChild(pSubtitle2);
    
    auto mode17 = CandidateBox::create();
    mode17->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    WZQDeviceSize::setPosAtNodeRightMiddle(mode17, pSubtitle2);
    mode17->setScale(WZQDeviceSize::getContentScaleInHeight(mode17, 0.04f));
    mode17->setSelectedCallback([this](Ref* pSender){
        onChooseSize(Size17x17);
    });
    mode17->setUnselectedCallback([this](Ref* pSender){
        onChooseSize(Size17x17);
    });
    addChild(mode17);
    m_HardChooseList.push_back(std::make_pair(Size17x17, mode17));
    
    auto pSubtitle23 = Label::createWithSystemFont("19x19", "黑体", WZQDeviceSize::getFontSize());
    pSubtitle23->setScale(WZQDeviceSize::getContentScaleInHeight(pSubtitle23, 0.04f));
    pSubtitle23->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    pSubtitle23->setPosition(Vec2(startPos3.x,
                                  pSubtitle2->getPosition().y));
    addChild(pSubtitle23);
    
    auto mode19 = CandidateBox::create();
    mode19->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    WZQDeviceSize::setPosAtNodeRightMiddle(mode19, pSubtitle23);
    mode19->setScale(WZQDeviceSize::getContentScaleInHeight(mode19, 0.04f));
    mode19->setSelectedCallback([this](Ref* pSender){
        onChooseSize(Size19x19);
    });
    mode19->setUnselectedCallback([this](Ref* pSender){
        onChooseSize(Size19x19);
    });
    addChild(mode19);
    m_HardChooseList.push_back(std::make_pair(Size19x19, mode19));
    
    
    auto pMenuButtonExit = MenuButton::create("", ESC_BT, [this](Ref* pSender){
        dispatchEvent(BUTTONS_BACK);
        removeFromParent();
    });
    
    
    m_pMenu -> addChild(pMenuButtonExit);
    WZQDeviceSize::setPosAtNodeRightTop(pMenuButtonExit, m_pBg);
    
    auto pMenuButtonComfirm = MenuButton::create("确认", [this](Ref* pSender){
        log("进入人机模式");
        AppModel::getInstance()->m_pInGameModel->m_gameConf.boardSize = m_sizeChoosed;
        AppModel::getInstance() -> m_pInGameModel ->setGameMode(BeatingMode);
    });
    
    WZQDeviceSize::setPosAtNodeBottomMiddle(pMenuButtonComfirm, m_pBg);
    WZQDeviceSize::setMoveNode(Vec2(0, spacing), pMenuButtonComfirm);
    m_pMenu -> addChild(pMenuButtonComfirm);
    
    return true;
}

void BeatModeChooseLayer::onEnter()
{
    Layer::onEnter();
}

void BeatModeChooseLayer::onExit()
{
    Layer::onExit();
}

BeatModeChooseLayer::BeatModeChooseLayer():m_sizeChoosed(Size13x13)
{
}

BeatModeChooseLayer::~BeatModeChooseLayer() noexcept
{
}

void BeatModeChooseLayer::onChooseSize(BOARDSIZE boardSize)
{
    m_sizeChoosed = boardSize;
    for(auto&i : m_HardChooseList)
    {
        if(i.first != boardSize)
        {
            i.second -> setSelected(false);
        }
        else
        {
            i.second -> setSelected(true);
        }
    }
}



