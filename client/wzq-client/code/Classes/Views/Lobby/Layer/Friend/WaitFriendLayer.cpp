//
//  WaitFriendLayer.cpp
//  wzqgame-mobile
//
//  Created by duole on 2024/2/5.
//

#include "WaitFriendLayer.hpp"
#include <stdio.h>
#include "Core/AppFunc.h"
#include "Core/AppEvent.h"
#include "Core/AppModel.h"
#include "WZQDeviceSize.hpp"
#include "Controllers/WZQLoginController.h"
#include "paohuziProtocol.pb.h"
#include "AddFriendLayer.hpp"
#include "FriendListLayer.hpp"
#include <iostream>
#include <string>
#include <sstream>
USING_NS_CC;


bool WaitFriendLayer::init()
{
    if(!Layer::init())
    {
        return false;
    }
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    m_pBg = Sprite::create(AppModel::getInstance()->m_pUserModel->getThemePath() + BG_POP);
    auto size = m_pBg->getContentSize();
    m_pBg->setAnchorPoint(Vec2(0.5,0));
    addChild(m_pBg,-1);
    auto vsize = Director::getInstance()->getVisibleSize();
    m_pBg->setPosition(Vec2(vsize.width/2,vsize.height*0.15));
    m_pBg -> setScale(WZQDeviceSize::getContentScaleInHeight(m_pBg, 0.7));
    
    // 好友列表
    auto pRankingLable = Label::createWithTTF("好友申请", FONT_GUHUANG, WZQDeviceSize::getFontSize() + 10);
    pRankingLable -> setColor(Color3B::ORANGE);
    pRankingLable -> setPosition(WZQDeviceSize::getViewCenterPos().x,  WZQDeviceSize::getViewCenterPos().y + 4 * m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 10);
    addChild(pRankingLable);
    
    // 退出按钮
    auto pMenuButtonExit = MenuButton::create("",ESC_BT, CC_CALLBACK_1(WaitFriendLayer::onMenuEsc, this));
    WZQDeviceSize::setPosAtNodeRightTop(pMenuButtonExit, m_pBg);
    //TODO 待同意列表
    
    m_pMenu = Menu::create(pMenuButtonExit, NULL);

    m_pMenu->setPosition(Vec2::ZERO);
    m_pMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    addChild(m_pMenu);
    
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
    
    //好友列表按钮
    auto m_pMenuButtonFriendList = MenuButton::create("好友列表", CC_CALLBACK_1(WaitFriendLayer::onFriendListClicked, this));
    m_pMenuButtonFriendList->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    WZQDeviceSize::setPosAtNodeLeftTop(m_pMenuButtonFriendList, m_pBg);
    m_pMenu->addChild(m_pMenuButtonFriendList);
    
    //添加好友按钮
    auto m_pMenuButtonAddFriend = MenuButton::create("添加好友", CC_CALLBACK_1(WaitFriendLayer::onAddFriendClicked, this));
    m_pMenuButtonAddFriend->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    WZQDeviceSize::setPosAtNodeRightBottom(m_pMenuButtonAddFriend, m_pMenuButtonFriendList);
    m_pMenu->addChild(m_pMenuButtonAddFriend);
    
    //等待添加好友按钮
    auto m_pMenuButtonWaitFriend = MenuButton::create("好友申请", CC_CALLBACK_1(WaitFriendLayer::onWaitFriendClicked, this));
    m_pMenuButtonWaitFriend->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    WZQDeviceSize::setPosAtNodeRightBottom(m_pMenuButtonWaitFriend, m_pMenuButtonAddFriend);
    m_pMenu->addChild(m_pMenuButtonWaitFriend);
    m_pMenuButtonWaitFriend->setEnabled(false);
    m_pMenuButtonWaitFriend->setSelected();
    
    return true;
}

void WaitFriendLayer::onEnter()
{
    Layer::onEnter();
    ADD_OBSERVER(WaitFriendLayer::updateWaitList, RCV_WAIT_FRIEND_LIST);
}

void WaitFriendLayer::onExit()
{
    Layer::onExit();
}

WaitFriendLayer::WaitFriendLayer()
{
}

WaitFriendLayer::~WaitFriendLayer() noexcept
{
}

void WaitFriendLayer::onMenuEsc(cocos2d::Ref *pSender)
{
    dispatchEvent(BUTTONS_BACK);
    removeFromParent();
}

void WaitFriendLayer::onConfirmClicked(cocos2d::Ref *pSender)
{
    printf("同意\n");
}

void WaitFriendLayer::onCancelClicked(cocos2d::Ref *pSender)
{
    printf("拒绝\n");
}

void WaitFriendLayer::updateWaitList(EventCustom* pEvent)
{
    auto waitfriendList = AppModel::getInstance() -> m_pUserModel -> getWaitFriendList();
    for (auto& i : waitfriendList)
    {
        std::cout<<"待同意的好友:"<<i.nUserId<<" "<<i.sUser<<" "<<i.Wintimes<<" "<<i.Score<<std::endl;
        addScrollViewInfo(i);
    }
    
    updateScrollView();
}

void WaitFriendLayer::addScrollViewInfo(KKUSERINFO friendInfo)
{
    
    // 头像位置
    auto headStartPos = Vec2(0, m_scrollHeight);
    // 用户名起始位置
    auto sUserStartPos = Vec2(m_scrollView->getContentSize().width *
                              m_scrollView->getScaleX() / 2,
                              m_scrollHeight);
    // 间隔
    int spacing = m_scrollView->getContentSize().height *
                  m_scrollView->getScaleY() / 10;
    
    // 一行的标准高度
    int standardHeight = m_scrollView->getContentSize().height*m_scrollView->getScaleY()/5;
    
    std::stringstream touxiang;
    int head_num=friendInfo.head;
    touxiang<<"touxiang/head"<<head_num<<".png";
    
    // 用户头像
    auto headSprite = Sprite::create(touxiang.str());
    headSprite->setScale(WZQDeviceSize::getCertainPixelScale(headSprite, standardHeight));
    headSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    headSprite->setPosition(headStartPos);
    
    m_scrollView->addChild(headSprite);
    m_scrollContent.push_back(std::make_pair(headSprite, headStartPos));
    
    // 用户名
    auto sUserLabel = Label::createWithSystemFont(friendInfo.sUser, "黑体", WZQDeviceSize::getFontSize());
    sUserLabel->setScale(WZQDeviceSize::getCertainPixelScaleInHeight(sUserLabel, standardHeight * 0.4));
    sUserLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    //sUserLabel->setPosition(sUserStartPos);
    WZQDeviceSize::setPosAtNodeRightTop(sUserLabel, headSprite);
    
    m_scrollView->addChild(sUserLabel);
    m_scrollContent.push_back(std::make_pair(sUserLabel, sUserLabel->getPosition()));
    
    // 同意
    auto deleteFriendBtn = MenuButton::create("同意", [friendInfo, this](Ref* pSender){
        KKUSERINFO oMyInfo =  AppModel::getInstance() -> m_pUserModel -> getMyUserInfo();
        int friend_id = friendInfo.nUserId;
        ValueMap oData;
        oData.insert(std::make_pair("user_id" ,Value((int)oMyInfo.nUserId)));
        oData.insert(std::make_pair("friend_id", Value(friend_id)));
        oData.insert(std::make_pair("is_agree",true));
        dispatchEvent(EVT_IS_AGREE_FRIEND,&oData);
        EventCustom* event;
        WaitFriendLayer::onFriendListClicked(event);
    });
    deleteFriendBtn->setScale(WZQDeviceSize::getCertainPixelScaleInHeight(deleteFriendBtn, standardHeight * 0.9));
    deleteFriendBtn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    //WZQDeviceSize::setPosAtNodeBottomMiddle(deleteFriendBtn, sUserLabel);
    deleteFriendBtn->setPosition(sUserStartPos);
    m_pScrollMenu->addChild(deleteFriendBtn);
    m_scrollContent.push_back(std::make_pair(deleteFriendBtn, deleteFriendBtn->getPosition()));
    
    // 拒绝
    auto inviteBtn = MenuButton::create("拒绝", [friendInfo, this](Ref* pSender){
        KKUSERINFO oMyInfo =  AppModel::getInstance() -> m_pUserModel -> getMyUserInfo();
        int friend_id = friendInfo.nUserId;
        ValueMap oData;
        oData.insert(std::make_pair("user_id" ,Value((int)oMyInfo.nUserId)));
        oData.insert(std::make_pair("friend_id", Value(friend_id)));
        oData.insert(std::make_pair("is_agree",false));
        dispatchEvent(EVT_IS_AGREE_FRIEND,&oData);
        EventCustom* event;
        WaitFriendLayer::onFriendListClicked(event);
    });
    
    inviteBtn->setScale(WZQDeviceSize::getCertainPixelScaleInHeight(inviteBtn, standardHeight * 0.9));
    inviteBtn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    WZQDeviceSize::setPosAtNodeRightTop(inviteBtn, deleteFriendBtn);
    m_pScrollMenu->addChild(inviteBtn);
    m_scrollContent.push_back(std::make_pair(inviteBtn, inviteBtn->getPosition()));
    
    // 更新高度
    m_scrollHeight -= (standardHeight + spacing);
    
    
}

void WaitFriendLayer::updateScrollView()
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

void WaitFriendLayer::resetScrollView() {
    for(auto& i : m_scrollContent)
    {
        i.first->removeFromParent();
    }
    m_scrollContent.clear();
    m_scrollHeight=m_scrollView->getContentSize().height *
    m_scrollView->getScaleY();;
    m_scrollView->setInnerContainerSize(Size(m_pBg->getContentSize().width * m_pBg->getScaleX(), m_scrollHeight));
}

void WaitFriendLayer::onFriendListClicked(cocos2d::Ref *pSender) {
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    ValueMap oData;
    oData.insert(std::make_pair("user_id",(int)usrinfo.nUserId));
    dispatchEvent(EVT_GET_FRIEND_LIST,&oData);
    auto Layer = FriendListLayer::create();
    this->getParent()->addChild(Layer);
    removeFromParent();
}

void WaitFriendLayer::onAddFriendClicked(cocos2d::Ref *pSender) {
    auto layer = AddFriendLayer::create();
    this->getParent()->addChild(layer);
    removeFromParent();
}

void WaitFriendLayer::onWaitFriendClicked(cocos2d::Ref *pSender) {
    log("已经在该界面，不用跳转！");
}






