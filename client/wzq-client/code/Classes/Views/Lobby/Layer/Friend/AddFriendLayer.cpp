//
//  AddFriendLayer.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/19.
//

#include "AddFriendLayer.hpp"
#include "WZQDeviceSize.hpp"
#include "MenuButton.hpp"
#include "AppRes.h"
#include "AppEvent.h"
#include "FriendListLayer.hpp"
#include "WaitFriendLayer.hpp"
#include <iostream>


bool AddFriendLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    m_pMenu = Menu::create();
    m_pMenu->setPosition(Vec2::ZERO);
    addChild(m_pMenu);
    
    m_pBg = Sprite::create(AppModel::getInstance()->m_pUserModel->getThemePath() + BG_POP);
    auto size = m_pBg->getContentSize();
    m_pBg->setAnchorPoint(Vec2(0.5,0));
    addChild(m_pBg,-1);
    auto vsize = Director::getInstance()->getVisibleSize();
    m_pBg->setPosition(Vec2(vsize.width/2,vsize.height*0.15));
    m_pBg -> setScale(WZQDeviceSize::getContentScaleInHeight(m_pBg, 0.7));
    
    // 退出按钮
    auto pMenuButtonExit = MenuButton::create("",ESC_BT,[this](Ref* pSender){
        dispatchEvent(BUTTONS_BACK);
        removeFromParent();
    });
    WZQDeviceSize::setPosAtNodeRightTop(pMenuButtonExit, m_pBg);
    m_pMenu->addChild(pMenuButtonExit);
    
    auto pLable = Label::createWithTTF("添加好友", FONT_GUHUANG, WZQDeviceSize::getFontSize() + 10);
    pLable -> setColor(Color3B::ORANGE);
    pLable -> setPosition(WZQDeviceSize::getViewCenterPos().x,  WZQDeviceSize::getViewCenterPos().y + 4 * m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 10);
    addChild(pLable);
    
    // 创建 EditBox
    Size editBoxSize = Size(vsize.width/4, vsize.height/10);

    m_Editbox = ui::EditBox::create(editBoxSize, ui::Scale9Sprite::create());
    m_Editbox->setPosition(Vec2(WZQDeviceSize::getViewCenterPos().x,  WZQDeviceSize::getViewCenterPos().y + 2 * m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 10));
    WZQDeviceSize::setMoveNode(Vec2(-vsize.width/20,0), m_Editbox);
    m_Editbox->setFontColor(Color3B::BLACK);
    m_Editbox->setFontSize(25);
    m_Editbox->setPlaceHolder("请输入用户名");
    m_Editbox->setPlaceholderFontColor(Color3B::GRAY);
    m_Editbox->setPlaceholderFontSize(25);
    m_Editbox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    
    addChild(m_Editbox);
    
    auto pComfirmBtn = MenuButton::create("确定", CC_CALLBACK_1(AddFriendLayer::onConfirmClicked, this));
    pComfirmBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    WZQDeviceSize::setPosAtNodeRightMiddle(pComfirmBtn, m_Editbox);
    m_pMenu->addChild(pComfirmBtn);
    
    //好友列表按钮
    auto m_pMenuButtonFriendList = MenuButton::create("好友列表", CC_CALLBACK_1(AddFriendLayer::onFriendListClicked, this));
    m_pMenuButtonFriendList->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    WZQDeviceSize::setPosAtNodeLeftTop(m_pMenuButtonFriendList, m_pBg);
    m_pMenu->addChild(m_pMenuButtonFriendList);
    
    //添加好友按钮
    auto m_pMenuButtonAddFriend = MenuButton::create("添加好友", CC_CALLBACK_1(AddFriendLayer::onAddFriendClicked, this));
    m_pMenuButtonAddFriend->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    WZQDeviceSize::setPosAtNodeRightBottom(m_pMenuButtonAddFriend, m_pMenuButtonFriendList);
    m_pMenu->addChild(m_pMenuButtonAddFriend);
    m_pMenuButtonAddFriend->setEnabled(false);
    m_pMenuButtonAddFriend->setSelected();
    
    //等待添加好友按钮
    auto m_pMenuButtonWaitFriend = MenuButton::create("好友申请", CC_CALLBACK_1(AddFriendLayer::onWaitFriendClicked, this));
    m_pMenuButtonWaitFriend->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    WZQDeviceSize::setPosAtNodeRightBottom(m_pMenuButtonWaitFriend, m_pMenuButtonAddFriend);
    m_pMenu->addChild(m_pMenuButtonWaitFriend);
    
    return true;
}

void AddFriendLayer::onFriendListClicked(cocos2d::Ref *pSender)
{
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    ValueMap oData;
    oData.insert(std::make_pair("user_id",(int)usrinfo.nUserId));
    dispatchEvent(EVT_GET_FRIEND_LIST,&oData);
    auto Layer = FriendListLayer::create();
    this->getParent()->addChild(Layer);
    removeFromParent();
}

void AddFriendLayer::onAddFriendClicked(cocos2d::Ref *pSender)
{
    log("已经在该页面，无效点击！");
}

void AddFriendLayer::onWaitFriendClicked(cocos2d::Ref *pSender)
{
    log("等待添加的用户页面\n");
    KKUSERINFO oMyInfo = AppModel::getInstance() -> m_pUserModel -> getMyUserInfo();
    ValueMap oData;
    oData.insert(std::make_pair("user_id" ,Value((int)oMyInfo.nUserId)));
    dispatchEvent(EVT_UPDATE_WAIT_LIST,&oData);
    auto Layer = WaitFriendLayer::create();
    this->getParent()->addChild(Layer);
    removeFromParent();
}

void AddFriendLayer::onEnter()
{
    Layer::onEnter();
    ADD_OBSERVER(AddFriendLayer::AddSuccess, ADD_FRIEND_SUCCESS);
    ADD_OBSERVER(AddFriendLayer::IsFriend, IS_FRIEND);
    ADD_OBSERVER(AddFriendLayer::NotExist, NOT_EXIST);
    ADD_OBSERVER(AddFriendLayer::updateFriendInfo, SEARCH_FRIEND_SUCCESS);
}


void AddFriendLayer::AddSuccess(EventCustom* pEvent){
    dispatchEvent(EVT_QUIT_SEARCH);
    cocos2d::MessageBox("您的请求已发送!", "");
}

void AddFriendLayer::IsFriend(EventCustom* pEvent){
    cocos2d::MessageBox("你们已经是好友了!", "");
}

void AddFriendLayer::NotExist(EventCustom* pEvent){
    cocos2d::MessageBox("该用户不存在!", "");
}

void AddFriendLayer::onConfirmClicked(Ref* pSender)
{
    log("确认添加该用户\n");
    const char* inputText = m_Editbox->getText();
    KKUSERINFO oMyInfo = AppModel::getInstance() -> m_pUserModel -> getMyUserInfo();
    ValueMap oData;
    oData.insert(std::make_pair("user_id" ,Value((int)oMyInfo.nUserId)));
    oData.insert(std::make_pair("friend_name" ,Value(inputText)));
    dispatchEvent(EVT_SEARCH_FRIEND,&oData);
    
}

void AddFriendLayer::updateFriendInfo(cocos2d::EventCustom *pEvent)
{
    for(auto& i : m_friendInfo)
    {
        i->removeFromParent();
    }
    
    m_friendInfo.clear();
    
    // 头像开始位置
    auto headStartPos = Vec2(WZQDeviceSize::getViewCenterPos().x - m_pBg -> getContentSize().width * 4 *
        m_pBg -> getScaleX() / 10,
        WZQDeviceSize::getViewCenterPos().y - 5 *
        m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 30);
    
    // 发送请求起始位置
    auto sendStartPos = Vec2(WZQDeviceSize::getViewCenterPos().x +
        m_pBg -> getContentSize().width * 5 *
        m_pBg -> getScaleX() / 15,
        WZQDeviceSize::getViewCenterPos().y - 6 *
        m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 30);
    
    KKUSERINFO friendInfo = AppModel::getInstance()->m_pUserModel->m_friendInfo;
    std::stringstream touxiang;
    int head_num=friendInfo.head;
    touxiang<<"touxiang/head"<<head_num<<".png";
    
    int standardHeight = m_pBg->getContentSize().height *
                         m_pBg->getScaleY() / 7;
    
    // 用户头像
    auto headSprite = Sprite::create(touxiang.str());
    headSprite->setScale(WZQDeviceSize::getCertainPixelScale(headSprite, standardHeight));
    headSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    headSprite->setPosition(headStartPos);
    
    addChild(headSprite);
    m_friendInfo.push_back(headSprite);
    
    // 用户名
    std::string name = friendInfo.sUser + "(" +friendInfo.account+")";
    auto sUserLabel = Label::createWithSystemFont(name, "黑体", WZQDeviceSize::getFontSize());
    sUserLabel->setScale(WZQDeviceSize::getCertainPixelScaleInHeight(sUserLabel, standardHeight * 0.4));
    sUserLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    WZQDeviceSize::setPosAtNodeRightTop(sUserLabel, headSprite);
    
    addChild(sUserLabel);
    m_friendInfo.push_back(sUserLabel);
    
    // ID
    auto sIDLabel = Label::createWithSystemFont("ID: " + std::to_string(friendInfo.nUserId), "黑体", WZQDeviceSize::getFontSize());
    sIDLabel->setScale(WZQDeviceSize::getCertainPixelScaleInHeight(sUserLabel, standardHeight * 0.3));
    sIDLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    WZQDeviceSize::setPosAtNodeLeftBottom(sIDLabel, sUserLabel);
    
    addChild(sIDLabel);
    m_friendInfo.push_back(sIDLabel);
    
    // 添加按钮
    auto addBtn = MenuButton::create("添加好友", [friendInfo, this](Ref* pSender)
    {
        std::cout<<"确定添加好友："<<friendInfo.sUser<<std::endl;
        dispatchEvent(ADD_FRIEND_COMFIRM);
        auto button = static_cast<MenuButton*>(pSender);
        
        button->setSelected();
        button->setEnabled(false);
        button->setText("已发送");
    });
    addBtn->setPosition(sendStartPos);
    m_friendInfo.push_back(addBtn);
    
    m_pMenu->addChild(addBtn);
}

