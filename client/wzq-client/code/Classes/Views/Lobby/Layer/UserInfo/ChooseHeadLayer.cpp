//
//  ChooseHead.cpp
//  wzqgame-mobile
//
//  Created by duole on 2024/1/25.
//

#include "ChooseHeadLayer.hpp"
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

bool ChooseHeadLayer::init()
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
    
    auto pMenuItemBtnSubmit = MenuButton::create("确认", CC_CALLBACK_1(ChooseHeadLayer::onMenuSubmit, this));
    pMenuItemBtnSubmit->setPosition(Vec2(vsize.width * 0.4, vsize.height * 0.1));
    
    auto pMenuItemBtnCancel = MenuButton::create("取消", CC_CALLBACK_1(ChooseHeadLayer::onMenuCancel, this));
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
    
//    auto pMenuItemBtnHead1 = MenuButton::create("", HEAD1, CC_CALLBACK_1(ChooseHeadLayer::ClickHead1, this));
//    pMenuItemBtnHead1->setScale(WZQDeviceSize::getContentScaleInHeight(pMenuItemBtnHead1,0.2f));
//    pMenuItemBtnHead1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
//    WZQDeviceSize::setPosAtNodeLeftTop(pMenuItemBtnHead1, m_pBg);
//    WZQDeviceSize::setMoveNode(Vec2(m_pBg -> getContentSize().width * m_pBg -> getScaleX() / 20, - m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 5), pMenuItemBtnHead1);
//
//    auto pMenuItemBtnHead2 = MenuButton::create("", HEAD2, CC_CALLBACK_1(ChooseHeadLayer::ClickHead2, this));
//    pMenuItemBtnHead2->setScale(WZQDeviceSize::getContentScaleInHeight(pMenuItemBtnHead2,0.2f));
//    pMenuItemBtnHead2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//    WZQDeviceSize::setPosAtNodeRightMiddle(pMenuItemBtnHead2, pMenuItemBtnHead1);
//    WZQDeviceSize::setMoveNode(Vec2(m_pBg -> getContentSize().width * m_pBg -> getScaleX() / 20, 0), pMenuItemBtnHead2);
//
//    auto pMenuItemBtnHead3 = MenuButton::create("", HEAD3, CC_CALLBACK_1(ChooseHeadLayer::ClickHead3, this));
//    pMenuItemBtnHead3->setScale(WZQDeviceSize::getContentScaleInHeight(pMenuItemBtnHead3,0.2f));
//    pMenuItemBtnHead3->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//    WZQDeviceSize::setPosAtNodeRightMiddle(pMenuItemBtnHead3, pMenuItemBtnHead2);
//    WZQDeviceSize::setMoveNode(Vec2(m_pBg -> getContentSize().width * m_pBg -> getScaleX() / 20, 0), pMenuItemBtnHead3);
//
//    auto pMenuItemBtnHead4 = MenuButton::create("", HEAD4, CC_CALLBACK_1(ChooseHeadLayer::ClickHead4, this));
//    pMenuItemBtnHead4->setScale(WZQDeviceSize::getContentScaleInHeight(pMenuItemBtnHead4,0.2f));
//    pMenuItemBtnHead4->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
//    WZQDeviceSize::setPosAtNodeBottomMiddle(pMenuItemBtnHead4, pMenuItemBtnHead1);
//    WZQDeviceSize::setMoveNode(Vec2(0, - m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 20), pMenuItemBtnHead4);
//
//    auto pMenuItemBtnHead5 = MenuButton::create("", HEAD5, CC_CALLBACK_1(ChooseHeadLayer::ClickHead5, this));
//    pMenuItemBtnHead5->setScale(WZQDeviceSize::getContentScaleInHeight(pMenuItemBtnHead5,0.2f));
//    pMenuItemBtnHead5->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//    WZQDeviceSize::setPosAtNodeRightMiddle(pMenuItemBtnHead5, pMenuItemBtnHead4);
//    WZQDeviceSize::setMoveNode(Vec2(m_pBg -> getContentSize().width * m_pBg -> getScaleX() / 20, 0), pMenuItemBtnHead5);
//
//    auto pMenu1 = Menu::create(pMenuItemBtnHead1, pMenuItemBtnHead2, pMenuItemBtnHead3,pMenuItemBtnHead4,pMenuItemBtnHead5,NULL);
//    pMenu1->setPosition(Vec2::ZERO);
//    pMenu1->setAnchorPoint(Vec2::ZERO);
//    addChild(pMenu1);
//
//    WZQDeviceSize::setAnchorNoMove(pMenuItemBtnHead1);
//    WZQDeviceSize::setAnchorNoMove(pMenuItemBtnHead2);
//    WZQDeviceSize::setAnchorNoMove(pMenuItemBtnHead3);
//    WZQDeviceSize::setAnchorNoMove(pMenuItemBtnHead4);
//    WZQDeviceSize::setAnchorNoMove(pMenuItemBtnHead5);
//    m_HeadList.push_back(pMenuItemBtnHead1);
//    m_HeadList.push_back(pMenuItemBtnHead2);
//    m_HeadList.push_back(pMenuItemBtnHead3);
//    m_HeadList.push_back(pMenuItemBtnHead4);
//    m_HeadList.push_back(pMenuItemBtnHead5);
    
    m_pHeadSelect = Sprite::create(HEADSELECT);
    m_pHeadSelect->setScale(WZQDeviceSize::getContentScaleInHeight(m_pHeadSelect,0.2f));
    m_pHeadSelect->setVisible(false);
    m_scrollView->addChild(m_pHeadSelect);
    
    for (int i = 0; i < m_AvailableList.size(); i+=3)
    {
        addScrollViewInfo(i);
    }
    
    updateScrollView();
    
    return true;
}

void ChooseHeadLayer::onMenuSubmit(Ref* pSender)
{
    if(ChooseHeadLayer::m_HeadNum==-1){
        //代表还没选
        cocos2d::MessageBox("请选择一个头像","重新选择\n");
        return ;
    }
    else{
        //选择了
        Value oData =Value(ChooseHeadLayer::m_HeadNum);
        dispatchEvent(CHOOSE_DONE,&oData);
        dispatchEvent(EVT_EDIT_HEAD,&oData);
        
        removeFromParent();
    }
    return;
}

//取消按键
void ChooseHeadLayer::onMenuCancel(Ref* pSender)
{
    ChooseHeadLayer::m_HeadNum=-1;
    printf("没有进行选择，保持原来头像\n");
    dispatchEvent(NOT_CHOOSE);
    removeFromParent();
    return;
}


//int ChooseHeadLayer::ClickHead1(Ref* pSender)
//{
//    ChooseHeadLayer::m_HeadNum=1;
//    printf("已选择头像: %d\n",m_HeadNum);
//    chooseHead(1);
//    return 1;
//}
//int ChooseHeadLayer::ClickHead2(Ref* pSender)
//{
//    ChooseHeadLayer::m_HeadNum=2;
//    printf("已选择头像: %d\n",m_HeadNum);
//    chooseHead(2);
//    return 2;
//}
//int ChooseHeadLayer::ClickHead3(Ref* pSender)
//{
//    ChooseHeadLayer::m_HeadNum=3;
//    printf("已选择头像: %d\n",m_HeadNum);
//    chooseHead(3);
//    return 3;
//}
//int ChooseHeadLayer::ClickHead4(Ref* pSender)
//{
//    ChooseHeadLayer::m_HeadNum=4;
//    printf("已选择头像: %d\n",m_HeadNum);
//    chooseHead(4);
//    return 4;
//}
//int ChooseHeadLayer::ClickHead5(Ref* pSender)
//{
//    ChooseHeadLayer::m_HeadNum=5;
//    printf("已选择头像: %d\n",m_HeadNum);
//    chooseHead(5);
//    return 5;
//}

void ChooseHeadLayer::onEnter()
{
    Layer::onEnter();
    
}

void ChooseHeadLayer::onExit()
{
    Layer::onExit();
}

ChooseHeadLayer::ChooseHeadLayer():m_HeadNum(-1)
{
    // 5个默认头像
    m_AvailableList.push_back(HEAD1);
    m_AvailableList.push_back(HEAD2);
    m_AvailableList.push_back(HEAD3);
    m_AvailableList.push_back(HEAD4);
    m_AvailableList.push_back(HEAD5);
    
    
    // 6个id为6-11的金币头像
    for (int i = 6; i < 12; i++)
    {
        auto p = AppModel::getInstance()->m_pShopModel->findItem(i);
        if (p != nullptr)
        {
            m_AvailableList.push_back(p->imgPath);
            std::cout<<"付费头像"<<p->name<<"已添加。"<<std::endl;
        }
    }
}

void ChooseHeadLayer::chooseHead(Vec2 pos)
{
    m_pHeadSelect->setVisible(true);
    m_pHeadSelect->setPosition(pos);
}

void ChooseHeadLayer::addScrollViewInfo(int flag)
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
        ChooseHeadLayer::m_HeadNum=AppModel::getInstance()->m_pShopModel->m_headList[m_AvailableList[flag]];
        printf("已选择头像: %d\n",m_HeadNum);
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
        ChooseHeadLayer::m_HeadNum=AppModel::getInstance()->m_pShopModel->m_headList[m_AvailableList[flag]];
        printf("已选择头像: %d\n",m_HeadNum);
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
        ChooseHeadLayer::m_HeadNum=AppModel::getInstance()->m_pShopModel->m_headList[m_AvailableList[flag]];
        printf("已选择头像: %d\n",m_HeadNum);
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

void ChooseHeadLayer::updateScrollView()
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
