//
//  EmojiLayer.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/21.
//

#include "EmojiLayer.hpp"
#include "Core/AppFunc.h"
#include "Core/AppEvent.h"
#include "Core/AppModel.h"
#include "Controllers/WZQLoginController.h"
#include <iostream>
#include <sstream>
#include "SigninLayer.hpp"
#include "WZQDeviceSize.hpp"
#include "RegistLayer.h"
#include "MenuButton.hpp"

USING_NS_CC;

bool EmojiLayer::init()
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
    m_pBg->setAnchorPoint(Vec2(0.5,0));
    addChild(m_pBg,-1);
    auto vsize = Director::getInstance()->getVisibleSize();
    m_pBg->setPosition(Vec2(vsize.width/2,vsize.height*0.15));
    m_pBg -> setScale(WZQDeviceSize::getContentScaleInHeight(m_pBg, 0.7));
    
    auto pMenuItemBtnSubmit = MenuButton::create("发送", CC_CALLBACK_1(EmojiLayer::onMenuSubmit, this));
    pMenuItemBtnSubmit->setPosition(Vec2(vsize.width * 0.4, vsize.height * 0.1));
    
    auto pMenuItemBtnCancel = MenuButton::create("取消", CC_CALLBACK_1(EmojiLayer::onMenuCancel, this));
    pMenuItemBtnCancel->setPosition(Vec2(vsize.width * 0.6, vsize.height * 0.1));
    
    m_pMenu->addChild(pMenuItemBtnSubmit);
    m_pMenu->addChild(pMenuItemBtnCancel);
    
    // 创建一个滚动视图
    m_scrollView = ui::ScrollView::create();
    m_scrollView->setContentSize(Size(m_pBg->getContentSize().width *
                                    m_pBg->getScaleX() * 0.8,
                                    m_pBg->getContentSize().height *
                                    m_pBg->getScaleY() * 0.7));
    m_scrollView->setDirection(ui::ScrollView::Direction::VERTICAL); // 垂直滚动
    WZQDeviceSize::setPosAtNodeLeftBottom(m_scrollView, m_pBg);
    WZQDeviceSize::setMoveNode(Vec2(m_pBg->getContentSize().width *
                                    m_pBg->getScaleX() * 0.12,
                                    m_pBg->getContentSize().height *
                                    m_pBg->getScaleY() * 0.10), m_scrollView);
    m_scrollView->setBounceEnabled(true); // 开启弹性效果
    
    m_scrollHeight = m_scrollView->getContentSize().height *
                     m_scrollView->getScaleY();

    // 设置滚动视图的内部容器大小
    m_scrollView->setInnerContainerSize(Size(m_pBg->getContentSize().width * m_pBg->getScaleX(), m_scrollHeight));
    
    
    // 将滚动视图添加到当前场景
    addChild(m_scrollView);
    
    m_pScrollMenu = Menu::create();
    m_pScrollMenu->setPosition(Vec2::ZERO);
    m_scrollView->addChild(m_pScrollMenu);
    
    m_pHeadSelect = Sprite::create(HEADSELECT);
    m_pHeadSelect->setScale(WZQDeviceSize::getContentScaleInHeight(m_pHeadSelect,0.2f));
    m_pHeadSelect->setVisible(false);
    m_scrollView->addChild(m_pHeadSelect);
    
    for (int i = 0; i < m_AvailableList.size(); i+=3)
    {
        addScrollViewInfo(i);
    }
    
    if (m_AvailableList.size() < 1)
    {
        auto errorLabel = Label::createWithSystemFont("你还没有表情，从商城购买一个吧！", "黑体", WZQDeviceSize::getFontSize());
        errorLabel->setScale(WZQDeviceSize::getContentScaleInHeight(errorLabel, 0.04f));
        errorLabel->setPosition(WZQDeviceSize::getViewCenterPos());
        addChild(errorLabel);
    }
    
    updateScrollView();
    
    return true;
}

void EmojiLayer::onMenuSubmit(Ref* pSender)
{
    if(EmojiLayer::m_EmojiNum==-1){
        //代表还没选
        cocos2d::MessageBox("请选择一个头像","重新选择\n");
        return ;
    }
    else{
        // 发送信息逻辑
        KKUSERINFO oMyInfo = AppModel::getInstance() -> m_pUserModel -> getMyUserInfo();
        KKUSERINFO oOpponentInfo = AppModel::getInstance() -> m_pInGameModel -> m_gameConf.oOppoInfo;
        std::string inputText = AppModel::getInstance()->m_pShopModel->m_emojiStringList[m_EmojiNum];
        ValueMap oData;
        oData.insert(std::make_pair("user_id" ,Value((int)oMyInfo.nUserId)));
        oData.insert(std::make_pair("table_id" ,Value(AppModel::getInstance() -> m_pInGameModel -> m_gameConf.iRoomId)));
        oData.insert(std::make_pair("opp_id" ,Value((int)oOpponentInfo.nUserId)));
        oData.insert(std::make_pair("text" , Value(inputText)));
        std::cout<<inputText<<std::endl;
        dispatchEvent(EVT_CHAT, &oData);
        removeFromParent();
    }
    return;
}

//取消按键
void EmojiLayer::onMenuCancel(Ref* pSender)
{
    EmojiLayer::m_EmojiNum=-1;
    printf("没有进行选择，保持原来头像\n");
    dispatchEvent(NOT_CHOOSE);
    removeFromParent();
    return;
}

void EmojiLayer::onEnter()
{
    Layer::onEnter();
    
}

void EmojiLayer::onExit()
{
    Layer::onExit();
}

EmojiLayer::EmojiLayer():m_EmojiNum(-1)
{
    // 5个id为1-5的金币表情
    for (int i = 1; i < 6; i++)
    {
        auto p = AppModel::getInstance()->m_pShopModel->findItem(i);
        if (p != nullptr)
        {
            m_AvailableList.push_back(p->imgPath);
            std::cout<<"付费表情"<<p->name<<"已添加。"<<std::endl;
        }
    }
}

void EmojiLayer::chooseHead(Vec2 pos)
{
    m_pHeadSelect->setVisible(true);
    m_pHeadSelect->setPosition(pos);
}

void EmojiLayer::addScrollViewInfo(int flag)
{
    // 头像位置
    auto headStartPos = Vec2(0, m_scrollHeight);
    
    // 间隔
    int spacing = m_scrollView->getContentSize().height *
                  m_scrollView->getScaleY() / 10;
    
    // 头像1
    if (m_AvailableList.size() <= flag)
        return;
    
    auto pMenuItemBtnHead1 = MenuButton::create("", m_AvailableList[flag],[this, flag](Ref* pSender){
        auto button = static_cast<MenuButton*>(pSender);
        WZQDeviceSize::setAnchorNoMove(button);
        chooseHead(button->getPosition());
        EmojiLayer::m_EmojiNum=AppModel::getInstance()->m_pShopModel->m_emojiList[m_AvailableList[flag]];
        printf("已选择表情: %d\n",m_EmojiNum);
    });
    pMenuItemBtnHead1->setScale(WZQDeviceSize::getContentScaleInHeight(pMenuItemBtnHead1,0.17f));
    pMenuItemBtnHead1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    pMenuItemBtnHead1->setPosition(headStartPos);
    
    //m_scrollView->addChild(pMenuItemBtnHead1);
    m_scrollContent.push_back(std::make_pair(pMenuItemBtnHead1, pMenuItemBtnHead1->getPosition()));
    m_pScrollMenu->addChild(pMenuItemBtnHead1);
    flag++;
    // 头像2
    if (m_AvailableList.size() <= flag)
        return;
    
    auto pMenuItemBtnHead2 = MenuButton::create("", m_AvailableList[flag],[this, flag](Ref* pSender){
        auto button = static_cast<MenuButton*>(pSender);
        WZQDeviceSize::setAnchorNoMove(button);
        chooseHead(button->getPosition());
        EmojiLayer::m_EmojiNum=AppModel::getInstance()->m_pShopModel->m_emojiList[m_AvailableList[flag]];
        printf("已选择表情: %d\n",m_EmojiNum);
    });
    pMenuItemBtnHead2->setScale(WZQDeviceSize::getContentScaleInHeight(pMenuItemBtnHead2,0.17f));
    pMenuItemBtnHead2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    WZQDeviceSize::setPosAtNodeRightMiddle(pMenuItemBtnHead2, pMenuItemBtnHead1);
    WZQDeviceSize::setMoveNode(Vec2(m_pBg -> getContentSize().width * m_pBg -> getScaleX() / 20, 0), pMenuItemBtnHead2);
    
    //m_scrollView->addChild(pMenuItemBtnHead2);
    m_scrollContent.push_back(std::make_pair(pMenuItemBtnHead2, pMenuItemBtnHead2->getPosition()));
    m_pScrollMenu->addChild(pMenuItemBtnHead2);
    flag++;
    
    // 头像3
    if (m_AvailableList.size() <= flag)
        return;
    
    auto pMenuItemBtnHead3 = MenuButton::create("", m_AvailableList[flag],[this, flag](Ref* pSender){
        auto button = static_cast<MenuButton*>(pSender);
        WZQDeviceSize::setAnchorNoMove(button);
        chooseHead(button->getPosition());
        EmojiLayer::m_EmojiNum=AppModel::getInstance()->m_pShopModel->m_emojiList[m_AvailableList[flag]];
        printf("已选择表情: %d\n",m_EmojiNum);
    });
    pMenuItemBtnHead3->setScale(WZQDeviceSize::getContentScaleInHeight(pMenuItemBtnHead3,0.17f));
    pMenuItemBtnHead3->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    WZQDeviceSize::setPosAtNodeRightMiddle(pMenuItemBtnHead3, pMenuItemBtnHead2);
    WZQDeviceSize::setMoveNode(Vec2(m_pBg -> getContentSize().width * m_pBg -> getScaleX() / 20, 0), pMenuItemBtnHead3);
    
    //m_scrollView->addChild(pMenuItemBtnHead3);
    m_scrollContent.push_back(std::make_pair(pMenuItemBtnHead3, pMenuItemBtnHead3->getPosition()));
    m_pScrollMenu->addChild(pMenuItemBtnHead3);
    
    auto standardHeight = pMenuItemBtnHead1->getContentSize().height *
                          pMenuItemBtnHead1->getScaleY();
    
    // 更新高度
    m_scrollHeight -= (standardHeight + spacing);
}

void EmojiLayer::updateScrollView()
{
    if (m_scrollContent.size() == 0)
        return;
    
    // 一行的标准高度
    int standardHeight = m_scrollView->getContentSize().height*m_scrollView->getScaleY()/5;
    
    // 找到最小的纵坐标
    std::pair<Node*, Vec2> minHeight = *std::min_element(m_scrollContent.begin(), m_scrollContent.end(), [](std::pair<Node*, Vec2> p1, std::pair<Node*, Vec2> p2) {
        return p1.second.y < p2.second.y;
    });
    
    // 如果最小元素纵坐标小于0，所有元素的纵坐标减去最小元素的纵坐标
    if (minHeight.second.y < 0)
    {
        for (auto& i: m_scrollContent)
        {
            i.first -> setPosition(i.second.x, i.second.y - minHeight.second.y + standardHeight);
            i.second = Vec2(i.second.x, i.second.y - minHeight.second.y + standardHeight);
        }
        // 更新高度
        m_scrollView->setInnerContainerSize(Size(m_pBg->getContentSize().width * m_pBg->getScaleX() * 0.9, m_scrollView->getContentSize().height * m_scrollView->getScaleY() - minHeight.second.y + standardHeight));
    }
}
