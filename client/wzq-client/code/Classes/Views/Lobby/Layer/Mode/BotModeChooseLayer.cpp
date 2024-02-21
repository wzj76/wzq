//
//  BotModeChooseLayer.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/7.
//

#include "BotModeChooseLayer.hpp"
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
#include <iostream>
USING_NS_CC;


#define KEY_EASY "easymode"
#define KEY_NORMAL "normalmode"
#define KEY_HARD "hardmode"
#define KEY_BLACK "black"
#define KEY_WHITE "white"

bool BotModeChooseLayer::init()
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
    
    m_pTitleLable = Label::createWithTTF( "人机设置", FONT_GUHUANG, WZQDeviceSize::getFontSize() + 10);
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
    m_pBg->getScaleY() * 1 / 15;
    
    auto pTitle1 = Label::createWithSystemFont("难度选择", "黑体", WZQDeviceSize::getFontSize());
    pTitle1->setScale(WZQDeviceSize::getContentScaleInHeight(pTitle1, 0.05f));
    pTitle1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    pTitle1->setPosition(startPos);
    addChild(pTitle1);
    
    auto pSubtitle1 = Label::createWithSystemFont("简单", "黑体", WZQDeviceSize::getFontSize());
    pSubtitle1->setScale(WZQDeviceSize::getContentScaleInHeight(pTitle1, 0.04f));
    pSubtitle1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    WZQDeviceSize::setPosAtNodeLeftBottom(pSubtitle1, pTitle1);
    addChild(pSubtitle1);
    
    auto easyMode = CandidateBox::create();
    easyMode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    WZQDeviceSize::setPosAtNodeRightMiddle(easyMode, pSubtitle1);
    easyMode->setScale(WZQDeviceSize::getContentScaleInHeight(easyMode, 0.04f));
    easyMode->setSelectedCallback([this](Ref* pSender){
        onChooseHard(KEY_EASY);
    });
    easyMode->setUnselectedCallback([this](Ref* pSender){
        onChooseHard(KEY_EASY);
    });
    addChild(easyMode);
    m_HardChooseList.push_back(std::make_pair(KEY_EASY, easyMode));
    
    auto pSubtitle12 = Label::createWithSystemFont("一般", "黑体", WZQDeviceSize::getFontSize());
    pSubtitle12->setScale(WZQDeviceSize::getContentScaleInHeight(pSubtitle12, 0.04f));
    pSubtitle12->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    pSubtitle12->setPosition(Vec2(startPos2.x,
                                  pSubtitle1->getPosition().y));
    addChild(pSubtitle12);
    
    auto normalMode = CandidateBox::create();
    normalMode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    WZQDeviceSize::setPosAtNodeRightMiddle(normalMode, pSubtitle12);
    normalMode->setScale(WZQDeviceSize::getContentScaleInHeight(normalMode, 0.04f));
    normalMode->setSelectedCallback([this](Ref* pSender){
        onChooseHard(KEY_NORMAL);
    });
    normalMode->setUnselectedCallback([this](Ref* pSender){
        onChooseHard(KEY_NORMAL);
    });
    addChild(normalMode);
    m_HardChooseList.push_back(std::make_pair(KEY_NORMAL, normalMode));
    
    auto pSubtitle13 = Label::createWithSystemFont("困难", "黑体", WZQDeviceSize::getFontSize());
    pSubtitle13->setScale(WZQDeviceSize::getContentScaleInHeight(pSubtitle13, 0.04f));
    pSubtitle13->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    pSubtitle13->setPosition(Vec2(startPos3.x,
                                  pSubtitle1->getPosition().y));
    addChild(pSubtitle13);
    
    auto hardMode = CandidateBox::create();
    hardMode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    WZQDeviceSize::setPosAtNodeRightMiddle(hardMode, pSubtitle13);
    hardMode->setScale(WZQDeviceSize::getContentScaleInHeight(hardMode, 0.04f));
    hardMode->setSelectedCallback([this](Ref* pSender){
        onChooseHard(KEY_HARD);
    });
    hardMode->setUnselectedCallback([this](Ref* pSender){
        onChooseHard(KEY_HARD);
    });
    addChild(hardMode);
    hardMode->setSelected(true);
    m_HardChooseList.push_back(std::make_pair(KEY_HARD, hardMode));
    
    auto pTitle2 = Label::createWithSystemFont("黑方白方选择", "黑体", WZQDeviceSize::getFontSize());
    pTitle2->setScale(WZQDeviceSize::getContentScaleInHeight(pTitle2, 0.05f));
    pTitle2->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    WZQDeviceSize::setPosAtNodeLeftBottom(pTitle2, pSubtitle1);
    WZQDeviceSize::setMoveNode(Vec2(0, -spacing), pTitle2);
    addChild(pTitle2);
    
    auto pSubtitle2 = Label::createWithSystemFont("黑方", "黑体", WZQDeviceSize::getFontSize());
    pSubtitle2->setScale(WZQDeviceSize::getContentScaleInHeight(pTitle2, 0.04f));
    pSubtitle2->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    WZQDeviceSize::setPosAtNodeLeftBottom(pSubtitle2, pTitle2);
    addChild(pSubtitle2);
    
    auto black = CandidateBox::create();
    black->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    WZQDeviceSize::setPosAtNodeRightMiddle(black, pSubtitle2);
    black->setScale(WZQDeviceSize::getContentScaleInHeight(black, 0.04f));
    black->setSelectedCallback([this](Ref* pSender){
        onChooseSide(KEY_BLACK);
    });
    black->setUnselectedCallback([this](Ref* pSender){
        onChooseSide(KEY_BLACK);
    });
    addChild(black);
    black->setSelected(true);
    m_SideChooseList.push_back(std::make_pair(KEY_BLACK, black));
    
    auto pSubtitle22 = Label::createWithSystemFont("白方", "黑体", WZQDeviceSize::getFontSize());
    pSubtitle22->setScale(WZQDeviceSize::getContentScaleInHeight(pSubtitle22, 0.04f));
    pSubtitle22->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    pSubtitle22->setPosition(Vec2(startPos3.x,
                                  pSubtitle2->getPosition().y));
    addChild(pSubtitle22);
    
    auto white = CandidateBox::create();
    white->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    WZQDeviceSize::setPosAtNodeRightMiddle(white, pSubtitle22);
    white->setScale(WZQDeviceSize::getContentScaleInHeight(white, 0.04f));
    white->setSelectedCallback([this](Ref* pSender){
        onChooseSide(KEY_WHITE);
    });
    white->setUnselectedCallback([this](Ref* pSender){
        onChooseSide(KEY_WHITE);
    });
    addChild(white);
    m_SideChooseList.push_back(std::make_pair(KEY_WHITE, white));
    
    auto pTitle3 = Label::createWithSystemFont("棋盘尺寸", "黑体", WZQDeviceSize::getFontSize());
    pTitle3->setScale(WZQDeviceSize::getContentScaleInHeight(pTitle3, 0.05f));
    pTitle3->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    WZQDeviceSize::setPosAtNodeLeftBottom(pTitle3, pSubtitle2);
    WZQDeviceSize::setMoveNode(Vec2(0, -spacing), pTitle3);
    addChild(pTitle3);
    
    auto pSubtitle31 = Label::createWithSystemFont("13x13", "黑体", WZQDeviceSize::getFontSize());
    pSubtitle31->setScale(WZQDeviceSize::getContentScaleInHeight(pSubtitle31, 0.04f));
    pSubtitle31->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    WZQDeviceSize::setPosAtNodeLeftBottom(pSubtitle31, pTitle3);
    addChild(pSubtitle31);
    
    auto size13 = CandidateBox::create();
    size13->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    WZQDeviceSize::setPosAtNodeRightMiddle(size13, pSubtitle31);
    size13->setScale(WZQDeviceSize::getContentScaleInHeight(size13, 0.04f));
    size13->setSelectedCallback([this](Ref* pSender){
        onChooseSize(Size13x13);
    });
    size13->setUnselectedCallback([this](Ref* pSender){
        onChooseSize(Size13x13);
    });
    addChild(size13);
    size13->setSelected(true);
    m_SizeChooseList.push_back(std::make_pair(Size13x13, size13));
    
    auto pSubtitle32 = Label::createWithSystemFont("15x15", "黑体", WZQDeviceSize::getFontSize());
    pSubtitle32->setScale(WZQDeviceSize::getContentScaleInHeight(pTitle2, 0.04f));
    pSubtitle32->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    pSubtitle32->setPosition(Vec2(startPos3.x,
                                  pSubtitle31->getPosition().y));
    addChild(pSubtitle32);
    
    auto size15 = CandidateBox::create();
    size15->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    WZQDeviceSize::setPosAtNodeRightMiddle(size15, pSubtitle32);
    size15->setScale(WZQDeviceSize::getContentScaleInHeight(size15, 0.04f));
    size15->setSelectedCallback([this](Ref* pSender){
        onChooseSize(Size15x15);
    });
    size15->setUnselectedCallback([this](Ref* pSender){
        onChooseSize(Size15x15);
    });
    addChild(size15);
    m_SizeChooseList.push_back(std::make_pair(Size15x15, size15));
    
    auto pSubtitle33 = Label::createWithSystemFont("17x17", "黑体", WZQDeviceSize::getFontSize());
    pSubtitle33->setScale(WZQDeviceSize::getContentScaleInHeight(pSubtitle33, 0.04f));
    pSubtitle33->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    WZQDeviceSize::setPosAtNodeLeftBottom(pSubtitle33, pSubtitle31);
    WZQDeviceSize::setMoveNode(Vec2(0, -spacing/2), pSubtitle33);
    addChild(pSubtitle33);
    
    auto size17 = CandidateBox::create();
    size17->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    WZQDeviceSize::setPosAtNodeRightMiddle(size17, pSubtitle33);
    size17->setScale(WZQDeviceSize::getContentScaleInHeight(size17, 0.04f));
    size17->setSelectedCallback([this](Ref* pSender){
        onChooseSize(Size17x17);
    });
    size17->setUnselectedCallback([this](Ref* pSender){
        onChooseSize(Size17x17);
    });
    addChild(size17);
    m_SizeChooseList.push_back(std::make_pair(Size17x17, size17));
    
    auto pSubtitle34 = Label::createWithSystemFont("19x19", "黑体", WZQDeviceSize::getFontSize());
    pSubtitle34->setScale(WZQDeviceSize::getContentScaleInHeight(pSubtitle34, 0.04f));
    pSubtitle34->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    pSubtitle34->setPosition(Vec2(startPos3.x,
                                  pSubtitle33->getPosition().y));
    addChild(pSubtitle34);
    
    auto size19 = CandidateBox::create();
    size19->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    WZQDeviceSize::setPosAtNodeRightMiddle(size19, pSubtitle34);
    size19->setScale(WZQDeviceSize::getContentScaleInHeight(size19, 0.04f));
    size19->setSelectedCallback([this](Ref* pSender){
        onChooseSize(Size19x19);
    });
    size19->setUnselectedCallback([this](Ref* pSender){
        onChooseSize(Size19x19);
    });
    addChild(size19);
    m_SizeChooseList.push_back(std::make_pair(Size19x19, size19));
    
    
    auto pMenuButtonExit = MenuButton::create("", ESC_BT, [this](Ref* pSender){
        dispatchEvent(BUTTONS_BACK);
        removeFromParent();
    });
    
    
    m_pMenu -> addChild(pMenuButtonExit);
    WZQDeviceSize::setPosAtNodeRightTop(pMenuButtonExit, m_pBg);
    
    auto pMenuButtonComfirm = MenuButton::create("确认", [this](Ref* pSender){
        log("进入人机模式");
        AppModel::getInstance() -> m_pInGameModel -> m_gameConf.boardSize = m_chessSize;
        
        
        if (m_sideChoosed == KEY_BLACK)
        {
            AppModel::getInstance()->m_pInGameModel->m_gameConf.llBlackId = AppModel::getInstance()->m_pUserModel->getMyUserId();
            std::cout<<"设置黑方id:"<<AppModel::getInstance()->m_pUserModel->getMyUserId()<<std::endl;
            AppModel::getInstance()->m_pInGameModel->m_gameConf.llWhiteId = 0;
        }
        else
        {
            AppModel::getInstance()->m_pInGameModel->m_gameConf.llBlackId = 0;
            AppModel::getInstance()->m_pInGameModel->m_gameConf.llWhiteId =  AppModel::getInstance()->m_pUserModel->getMyUserId();
        }
        AppModel::getInstance() -> m_pInGameModel ->setGameMode(BotMode);
    });
    
    WZQDeviceSize::setPosAtNodeBottomMiddle(pMenuButtonComfirm, m_pBg);
    WZQDeviceSize::setMoveNode(Vec2(0, spacing), pMenuButtonComfirm);
    m_pMenu -> addChild(pMenuButtonComfirm);
    
    return true;
}

void BotModeChooseLayer::onEnter()
{
    Layer::onEnter();
    AppModel::getInstance()->m_pBotGameModel->m_curid = AppModel::getInstance()->m_pBotGameModel-> m_userid;
    AppModel::getInstance()->m_pBotGameModel->m_balck_id = AppModel::getInstance()->m_pBotGameModel-> m_userid;
    AppModel::getInstance()->m_pBotGameModel->m_white_id = AppModel::getInstance()->m_pBotGameModel-> m_robotid;
    AppModel::getInstance()->m_pBotGameModel-> m_is_user = true;
    AppModel::getInstance()->m_pBotGameModel->m_put_num = 0;
    AppModel::getInstance()->m_pBotGameModel->m_modeChoosed = KEY_HARD;
}

void BotModeChooseLayer::onExit()
{
    Layer::onExit();
}

BotModeChooseLayer::BotModeChooseLayer():m_modeChoosed(KEY_HARD),m_sideChoosed(KEY_BLACK),m_chessSize(Size13x13)
{
    AppModel::getInstance()->m_pBotGameModel->m_curid = AppModel::getInstance()->m_pBotGameModel-> m_userid;
    AppModel::getInstance()->m_pBotGameModel->m_balck_id = AppModel::getInstance()->m_pBotGameModel-> m_userid;
    AppModel::getInstance()->m_pBotGameModel->m_white_id = AppModel::getInstance()->m_pBotGameModel-> m_robotid;
    AppModel::getInstance()->m_pBotGameModel-> m_is_user = true;
    AppModel::getInstance()->m_pBotGameModel->m_put_num = 0;
    AppModel::getInstance()->m_pBotGameModel->m_sideChoosed = KEY_BLACK;
    AppModel::getInstance()->m_pBotGameModel->m_modeChoosed = KEY_HARD;
    AppModel::getInstance()->m_pInGameModel->m_gameConf.boardSize = Size13x13;
}

BotModeChooseLayer::~BotModeChooseLayer() noexcept
{
}

void BotModeChooseLayer::onChooseHard(std::string mode)
{
    AppModel::getInstance()->m_pBotGameModel->m_modeChoosed = mode;
    printf("选择的难度%s\n",mode.c_str());
    m_modeChoosed = mode;
    for(auto&i : m_HardChooseList)
    {
        if(i.first != mode)
        {
            i.second -> setSelected(false);
        }
        else
        {
            i.second -> setSelected(true);
        }
    }
}

void BotModeChooseLayer::onChooseSide(std::string side) {
    AppModel::getInstance()->m_pBotGameModel->m_sideChoosed = side;
    if(side == KEY_BLACK)
    {
        AppModel::getInstance()->m_pBotGameModel->m_curid = AppModel::getInstance()->m_pBotGameModel-> m_userid;
        AppModel::getInstance()->m_pBotGameModel->m_balck_id = AppModel::getInstance()->m_pBotGameModel-> m_userid;
        AppModel::getInstance()->m_pBotGameModel->m_white_id = AppModel::getInstance()->m_pBotGameModel-> m_robotid;
        AppModel::getInstance()->m_pBotGameModel-> m_is_user = true;
        AppModel::getInstance()->m_pBotGameModel->m_put_num = 0;
    }
    else if(side == KEY_WHITE)
    {
        AppModel::getInstance()->m_pBotGameModel->m_curid = AppModel::getInstance()->m_pBotGameModel-> m_robotid;
        AppModel::getInstance()->m_pBotGameModel->m_balck_id = AppModel::getInstance()->m_pBotGameModel-> m_robotid;
        AppModel::getInstance()->m_pBotGameModel->m_white_id = AppModel::getInstance()->m_pBotGameModel-> m_userid;
        AppModel::getInstance()->m_pBotGameModel-> m_is_user = false;
        AppModel::getInstance()->m_pBotGameModel->m_put_num = 0;
        
    }
    else
    {
        printf("无效选择%s\n",side.c_str());
        return;
    }
    
    m_sideChoosed = side;
    for(auto&i : m_SideChooseList)
    {
        if(i.first != side)
        {
            i.second -> setSelected(false);
        }
        else
        {
            i.second -> setSelected(true);
        }
    }
}

void BotModeChooseLayer::onChooseSize(BOARDSIZE size) {
    m_chessSize = size;
    for(auto&i : m_SizeChooseList)
    {
        if(i.first != size)
        {
            i.second -> setSelected(false);
        }
        else
        {
            i.second -> setSelected(true);
        }
    }
}




