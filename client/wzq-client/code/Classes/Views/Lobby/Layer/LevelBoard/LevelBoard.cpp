//
//  LevelBoard.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/26.
//

#include "LevelBoard.hpp"
#include <stdio.h>
#include "Core/AppFunc.h"
#include "Core/AppEvent.h"
#include "Core/AppModel.h"
#include "WZQDeviceSize.hpp"
#include "Controllers/WZQLoginController.h"
#include <iostream>
#include <string>
#include <sstream>
#include <codecvt>
USING_NS_CC;

bool LevelBoard::init()
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
    
    // 退出按钮
    auto pMenuButtonExit = MenuButton::create("",ESC_BT, CC_CALLBACK_1(LevelBoard::onMenuEsc, this));
    WZQDeviceSize::setPosAtNodeRightTop(pMenuButtonExit, m_pBg);
    
    // 排行榜几个字
    auto pRankingLable = Label::createWithTTF("排 行 榜", FONT_GUHUANG, WZQDeviceSize::getFontSize() + 10);
    pRankingLable -> setColor(Color3B::ORANGE);
    pRankingLable -> setPosition(WZQDeviceSize::getViewCenterPos().x,  WZQDeviceSize::getViewCenterPos().y + 4 * m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 10);
    addChild(pRankingLable);
    
    // 翻页按钮
    m_pMenuButtonNextPage = MenuButton::create("下一页", CC_CALLBACK_1(LevelBoard::onNextPageClicked, this));
    m_pMenuButtonNextPage -> setPosition(WZQDeviceSize::getViewCenterPos().x + m_pBg -> getContentSize().width * m_pBg -> getScaleX() / 4, WZQDeviceSize::getViewCenterPos().y - m_pBg -> getContentSize().height * m_pBg -> getScaleY() /2);
    
    m_pMenuButtonLastPage = MenuButton::create("上一页", CC_CALLBACK_1(LevelBoard::onLastPageClicked, this));
    m_pMenuButtonLastPage -> setPosition(WZQDeviceSize::getViewCenterPos().x - m_pBg -> getContentSize().width * m_pBg -> getScaleX() / 4, WZQDeviceSize::getViewCenterPos().y - m_pBg -> getContentSize().height * m_pBg -> getScaleY() /2);
    
    // 展示第几页
    std::stringstream lableText;
    lableText << "第"<<m_Pageid<<"页";
    m_pLablePageId = Label::createWithTTF(lableText.str(), FONT_GUHUANG, WZQDeviceSize::getFontSize());
    m_pLablePageId -> setPosition(WZQDeviceSize::getViewCenterPos().x, WZQDeviceSize::getViewCenterPos().y - m_pBg -> getContentSize().height * m_pBg -> getScaleY() /2);
    
    addChild(m_pLablePageId);
    
    m_pMenu = Menu::create(pMenuButtonExit, m_pMenuButtonNextPage,  m_pMenuButtonLastPage, NULL);

    m_pMenu->setPosition(Vec2::ZERO);
    m_pMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    addChild(m_pMenu);
    
    m_pNodataLabel = Label::createWithSystemFont("没有更多数据了……", "黑体", WZQDeviceSize::getFontSize());
    m_pNodataLabel->setScale(WZQDeviceSize::getContentScaleInHeight(m_pNodataLabel, 0.05f));
    m_pNodataLabel->setPosition(WZQDeviceSize::getViewCenterPos());
    addChild(m_pNodataLabel);
    m_pNodataLabel->setVisible(false);
    
    
    return true;
}

void LevelBoard::onEnter()
{
    Layer::onEnter();
    ADD_OBSERVER(LevelBoard::updateBoard, RCV_RANDING);
}

void LevelBoard::onExit()
{
    Layer::onExit();
}

LevelBoard::LevelBoard()
{
    m_Pageid = 1;
    m_pUserIDs = new Vector<Label*>;
    m_pUserWins = new Vector<Label*>;
    m_pUserScores = new Vector<Label*>;
    m_pRankings = new Vector<Label*>;
    m_pWaiting = WZQWaitingLayer::sharedInstance();
    m_pWaiting -> setPosition(WZQDeviceSize::getViewCenterPos());
    
    for (int i = 0; i < 11; i++)
    {
        auto LabelTmp = Label::createWithSystemFont("", "黑体", WZQDeviceSize::getFontSize());
        LabelTmp -> setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        m_pUserIDs -> pushBack(LabelTmp);
        
        if (i == 0)
        {
            LabelTmp -> setString("用户名");
            LabelTmp -> setColor(Color3B::GRAY);
        }
    }
    m_pUserWins = new Vector<Label*>;
    for (int i = 0; i < 11; i++)
    {
        auto LabelTmp = Label::createWithSystemFont("", "黑体", WZQDeviceSize::getFontSize());
        LabelTmp -> setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        m_pUserWins -> pushBack(LabelTmp);
        
        if (i == 0) {
            LabelTmp -> setString("胜场数");
            LabelTmp -> setColor(Color3B::GRAY);
        }
    }
    m_pUserScores = new Vector<Label*>;
    for (int i = 0; i < 11; i++)
    {
        auto LabelTmp = Label::createWithSystemFont("", "黑体", WZQDeviceSize::getFontSize());
        LabelTmp -> setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        m_pUserScores -> pushBack(LabelTmp);
        
        if (i == 0)
        {
            LabelTmp -> setString("分数");
            LabelTmp -> setColor(Color3B::GRAY);
        }
    }
    for (int i = 0; i < 11; i++)
    {
        auto LabelTmp = Label::createWithSystemFont("", "黑体", WZQDeviceSize::getFontSize());
        LabelTmp -> setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        m_pRankings -> pushBack(LabelTmp);
        
        if (i == 0)
        {
            LabelTmp -> setString("名次");
            LabelTmp -> setColor(Color3B::GRAY);
        }
    }
}

LevelBoard::~LevelBoard() noexcept
{ 
    delete m_pUserIDs;
    m_pUserIDs = nullptr;
    delete m_pUserWins;
    m_pUserWins = nullptr;
    delete m_pUserScores;
    m_pUserScores = nullptr;
    delete m_pRankings;
    m_pRankings = nullptr;
}

void LevelBoard::onMenuEsc(cocos2d::Ref *pSender)
{
    dispatchEvent(BUTTONS_BACK);
    removeFromParent();
}

// TODO: 添加等待界面，否则网络超时会导致应用崩溃
void LevelBoard::updateBoard(cocos2d::Event *pEvent)
{
    log("更新排行榜");
    m_pWaiting -> show();
    if (m_Pageid > 1) m_pMenuButtonLastPage -> setVisible(true);
    else m_pMenuButtonLastPage -> setVisible(false);

    auto rankingList = AppModel::getInstance() -> m_pUserModel -> getRankingList();
    for (auto& i : rankingList)
    {
        std::cout<<i.nUserId<<" "<<i.sUser<<" "<<i.Wintimes<<" "<<i.Score<<std::endl;
    }
    int count = rankingList.size();
    std::cout << "获得"<< count <<"条数据。"<<std::endl;
    int bet = m_pUserIDs -> front() -> getContentSize().height;
    int height = 0;
    int flag = 0;
    
    if(m_Pageid==10)
    {
        m_pMenuButtonNextPage -> setVisible(false);
    }
    
    if (count < 10 && count > 0)
    {
        // 最后一页，不再显示下一页
        AppModel::getInstance()->m_pUserModel->m_pWaiting->hide();
        m_pMenuButtonNextPage -> setVisible(false);
    }
    else if(count == 0)
    {
        AppModel::getInstance()->m_pUserModel->m_pWaiting->hide();
        m_pMenuButtonNextPage -> setVisible(false);
        m_pNodataLabel->setVisible(true);
        return;
    }
    else
    {
        m_pMenuButtonNextPage -> setVisible(true);
        m_pNodataLabel->setVisible(false);
    }
    
    // 名次起始位置
    Vec2 rankingsStartPosition = Vec2(WZQDeviceSize::getViewCenterPos().x - m_pBg -> getContentSize().width * 2 * m_pBg -> getScaleX() / 5, WZQDeviceSize::getViewCenterPos().y + 8 * m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 30);
    
    for (auto& i : *m_pRankings)
    {
        // 略过第一个
        if (i == (*m_pRankings).front())
        {
            i -> setPosition(rankingsStartPosition.x, rankingsStartPosition.y - height);
            addChild(i);
            height += bet;
            continue;
        }
        i -> setPosition(rankingsStartPosition.x, rankingsStartPosition.y - height);
        i -> setString(std::to_string((flag + 1) + ((m_Pageid - 1) * 10)));
        addChild(i);
        height += bet;
        if (flag < count - 1) flag ++;
        else break;
    }
    
    // 用户名起始位置
    Vec2 startPosition = Vec2(WZQDeviceSize::getViewCenterPos().x - m_pBg -> getContentSize().width * 3 * m_pBg -> getScaleX() / 10, WZQDeviceSize::getViewCenterPos().y + 8 * m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 30);
    
    height = 0;
    flag = 0;
    
    for (auto& i : *m_pUserIDs)
    {
        // 略过第一个
        if (i == (*m_pUserIDs).front())
        {
            i -> setPosition(startPosition.x, startPosition.y - height);
            addChild(i);
            height += bet;
            continue;
        }
        // 避免用户名过长，截断到第8个字符
        std::string username = rankingList[flag].sUser;
        
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring username_converter = converter.from_bytes(username); // 转换为 std::wstring


        if (username_converter.size() > 6) {
            username_converter.replace(6, std::wstring::npos, L"..."); // 截断到第8个字符，并添加省略号
        }
       
        username = converter.to_bytes(username_converter); // 转换回 UTF-8 编码
        
        i -> setPosition(startPosition.x, startPosition.y - height);
        i -> setString(username);
        addChild(i);
        height += bet;
        if (flag < count - 1) flag ++;
        else break;
    }
    
    // 胜场数初始位置
    Vec2 winsStartPosition = Vec2(WZQDeviceSize::getViewCenterPos().x,  WZQDeviceSize::getViewCenterPos().y + 8 * m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 30);
    
    height = 0;
    flag = 0;
    
    for (auto& i : *m_pUserWins)
    {
        if (i == (*m_pUserWins).front())
        {
            i -> setPosition(winsStartPosition.x, winsStartPosition.y - height);
            addChild(i);
            height += bet;
            continue;
        }
        i -> setPosition(winsStartPosition.x, winsStartPosition.y - height);
        i -> setString(std::to_string(rankingList[flag].Wintimes));
        addChild(i);
        height += bet;
        if (flag < count - 1) flag ++;
        else break;
    }
    
    // 分数初始位置
    Vec2 scoreStartPosition = Vec2(WZQDeviceSize::getViewCenterPos().x + m_pBg -> getContentSize().width * m_pBg -> getScaleX() / 5,  WZQDeviceSize::getViewCenterPos().y + 8 * m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 30);
    
    height = 0;
    flag = 0;
    
    for (auto& i : *m_pUserScores)
    {
        // 略过第一个
        if (i == (*m_pUserScores).front())
        {
            i -> setPosition(scoreStartPosition.x, scoreStartPosition.y - height);
            addChild(i);
            height += bet;
            continue;
        }
        
        i -> setPosition(scoreStartPosition.x, scoreStartPosition.y - height);
        i -> setString(std::to_string(rankingList[flag].Score));
        addChild(i);
        height += bet;
        if (flag < count - 1) flag ++;
        else break;
    }
    AppModel::getInstance()->m_pUserModel->m_pWaiting->hide();
}

void LevelBoard::onNextPageClicked(cocos2d::Ref *pSender)
{
    AppModel::getInstance()->m_pUserModel->m_pWaiting->show(15, "请稍等……");
    log("排行榜点击下一页");
    m_pMenuButtonNextPage -> setVisible(false);
    m_pMenuButtonLastPage -> setVisible(false);
    m_Pageid++;
    ValueMap oData;
    oData.insert(std::make_pair("message","req_get_randing_list"));
    oData.insert(std::make_pair("limit",10));
    oData.insert(std::make_pair("offset",0 + 10 * (m_Pageid - 1)));
    dispatchEvent(EVT_RANDING_LIST,&oData);
    for (auto& i:(*m_pRankings)) i -> removeFromParent();
    for (auto& i:(*m_pUserIDs)) i -> removeFromParent();
    for (auto& i:(*m_pUserScores)) i -> removeFromParent();
    for (auto& i:(*m_pUserWins)) i -> removeFromParent();
    std::stringstream lableText;
    lableText << "第"<<m_Pageid<<"页";
    m_pLablePageId -> setString(lableText.str());
}

void LevelBoard::onLastPageClicked(cocos2d::Ref *pSender)
{
    log("排行榜点击上一页");
    m_pMenuButtonNextPage -> setVisible(false);
    m_pMenuButtonLastPage -> setVisible(false);
    AppModel::getInstance()->m_pUserModel->m_pWaiting->show(15, "请稍等……");
    if (m_Pageid > 1)
    {
        m_Pageid--;
        ValueMap oData;
        oData.insert(std::make_pair("message","req_get_randing_list"));
        oData.insert(std::make_pair("limit",10));
        oData.insert(std::make_pair("offset",0 + 10 * (m_Pageid - 1)));
        dispatchEvent(EVT_RANDING_LIST,&oData);
        for (auto& i:(*m_pRankings)) i -> removeFromParent();
        for (auto& i:(*m_pUserIDs)) i -> removeFromParent();
        for (auto& i:(*m_pUserScores)) i -> removeFromParent();
        for (auto& i:(*m_pUserWins)) i -> removeFromParent();
        std::stringstream lableText;
        lableText << "第"<<m_Pageid<<"页";
        m_pLablePageId -> setString(lableText.str());
    }
}







