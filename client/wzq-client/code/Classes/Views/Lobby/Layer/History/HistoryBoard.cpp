//
//  HistoryBoard.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/1.
//

#include "HistoryBoard.hpp"
#include "ShowHistoryScene.hpp"
#include <stdio.h>
#include "Core/AppFunc.h"
#include "Core/AppEvent.h"
#include "Core/AppModel.h"
#include "WZQDeviceSize.hpp"
#include "Controllers/WZQLoginController.h"
#include "ShowHistoryScene.hpp"
#include <iostream>
#include <string>
#include <sstream>
USING_NS_CC;

bool HistoryBoard::init()
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
    m_pBg -> setScale(WZQDeviceSize::getContentScaleInWidth(m_pBg, 0.6), WZQDeviceSize::getContentScaleInHeight(m_pBg, 0.7));
    
    // 退出按钮
    auto pMenuButtonExit = MenuButton::create("",ESC_BT, CC_CALLBACK_1(HistoryBoard::onMenuEsc, this));
    WZQDeviceSize::setPosAtNodeRightTop(pMenuButtonExit, m_pBg);
    
    // 历史对局几个字
    auto pRankingLable = Label::createWithTTF("历史对局", FONT_GUHUANG, WZQDeviceSize::getFontSize() + 10);
    pRankingLable -> setColor(Color3B::ORANGE);
    pRankingLable -> setPosition(WZQDeviceSize::getViewCenterPos().x,  WZQDeviceSize::getViewCenterPos().y + 4 * m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 10);
    addChild(pRankingLable);
    
    // 翻页按钮
    m_pMenuButtonNextPage = MenuButton::create("下一页", CC_CALLBACK_1(HistoryBoard::onNextPageClicked, this));
    m_pMenuButtonNextPage -> setPosition(WZQDeviceSize::getViewCenterPos().x + m_pBg -> getContentSize().width * m_pBg -> getScaleX() / 4, WZQDeviceSize::getViewCenterPos().y - m_pBg -> getContentSize().height * m_pBg -> getScaleY() /2);
    
    m_pMenuButtonLastPage = MenuButton::create("上一页", CC_CALLBACK_1(HistoryBoard::onLastPageClicked, this));
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
    
    Event* pEvent;
    
    return true;
}

void HistoryBoard::onEnter()
{
    Layer::onEnter();
    ADD_OBSERVER(HistoryBoard::updateBoard, RCV_HISTORY);
}

void HistoryBoard::onExit()
{
    Layer::onExit();
}

HistoryBoard::HistoryBoard()
{
    m_Pageid = 1;
    m_pTime = new Vector<Label*>;
    m_pIsWin = new Vector<Label*>;
    m_pOppo = new Vector<Label*>;
    m_pEnter = new Vector<MenuButton*>;
    m_pWaiting = WZQWaitingLayer::sharedInstance();
    m_pWaiting -> setPosition(WZQDeviceSize::getViewCenterPos());
    for (int i = 0; i < 6; i++)
    {
        auto LabelTmp = Label::createWithSystemFont("", "黑体", WZQDeviceSize::getFontSize());
        LabelTmp -> setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        m_pTime -> pushBack(LabelTmp);
        
        if (i == 0) LabelTmp -> setString("时间");
    }

    for (int i = 0; i < 6; i++)
    {
        auto LabelTmp = Label::createWithSystemFont("", "黑体", WZQDeviceSize::getFontSize());
        LabelTmp -> setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        m_pIsWin -> pushBack(LabelTmp);
        
        if (i == 0) LabelTmp -> setString("战绩");
    }
    for (int i = 0; i < 6; i++)
    {
        auto LabelTmp = Label::createWithSystemFont("", "黑体", WZQDeviceSize::getFontSize());
        LabelTmp -> setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        m_pOppo -> pushBack(LabelTmp);
        
        if (i == 0) LabelTmp -> setString("对手");
    }
    for (int i = 0; i < 6; i++)
    {
        auto LabelTmp = MenuButton::create("回放", [i](Ref* pSender){
            std::cout << "按钮" << i << "被调用。" << std::endl;
            if(i==0){
                return;
            }
            auto history = AppModel::getInstance()->m_pUserModel -> getHistoryList()[i-1];
            KKUSERINFO oppinfo = history.enemy_userinfo;
            AppModel::getInstance()->m_pUserModel -> setOppUserInfo(oppinfo);
            AppModel::getInstance()->m_pUserModel -> m_historyId = i-1;
            auto pShowhistoryScene = ShowHistoryScene::createScene();
            Director::getInstance() -> replaceScene(pShowhistoryScene);
            log("进入对局查看");
        });
        LabelTmp -> setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        m_pEnter -> pushBack(LabelTmp);
        
        if (i == 0) LabelTmp -> setVisible(false);
    }
}

HistoryBoard::~HistoryBoard() noexcept
{
    delete m_pTime;
    m_pTime = nullptr;
    delete m_pOppo;
    m_pOppo = nullptr;
    delete m_pIsWin;
    m_pIsWin = nullptr;
    delete m_pEnter;
    m_pEnter = nullptr;
}

void HistoryBoard::onMenuEsc(cocos2d::Ref *pSender)
{
    dispatchEvent(BUTTONS_BACK);
    removeFromParent();
}

// TODO: 添加等待界面，否则网络超时会导致应用崩溃
void HistoryBoard::updateBoard(cocos2d::Event *pEvent)
{
    log("更新历史记录");
    m_pWaiting -> show();
    auto history = AppModel::getInstance()->m_pUserModel->getHistoryList();
    if (m_Pageid > 1) m_pMenuButtonLastPage -> setVisible(true);
    else m_pMenuButtonLastPage -> setVisible(false);
    
    auto historyList = AppModel::getInstance() -> m_pUserModel -> getHistoryList();
    if(historyList.size() == 0){
        printf("无历史记录\n");
        AppModel::getInstance()->m_pUserModel->m_pWaiting->hide();
        return;
    }
    // 对局时间起始位置
    Vec2 timeStartPosition = Vec2(WZQDeviceSize::getViewCenterPos().x - m_pBg -> getContentSize().width * 4 *
        m_pBg -> getScaleX() / 10,
        WZQDeviceSize::getViewCenterPos().y + 9 *
        m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 30);
    
    // 对局对手起始位置
    Vec2 opposantStartPosition = Vec2(WZQDeviceSize::getViewCenterPos().x -
        m_pBg -> getContentSize().width * 3 *
        m_pBg -> getScaleX() / 15,
        WZQDeviceSize::getViewCenterPos().y + 9 *
        m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 30);
    
    // 输赢初始位置
    Vec2 winsStartPosition = Vec2(WZQDeviceSize::getViewCenterPos().x +
        m_pBg -> getContentSize().width * 1 *
        m_pBg -> getScaleX() / 10,
        WZQDeviceSize::getViewCenterPos().y + 9 *
        m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 30);
    
    // 分数初始位置
    Vec2 scoreStartPosition = Vec2(WZQDeviceSize::getViewCenterPos().x +
        m_pBg -> getContentSize().width * 3 *
        m_pBg -> getScaleX() / 15,
        WZQDeviceSize::getViewCenterPos().y + 10 *
        m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 30);
    
    int count = historyList.size();
    int bet = m_pEnter -> front() -> getContentSize().height *
              m_pEnter -> front() -> getScaleY() * 0.9;
    int height = 0;
    int flag = 0;
    
    if (count < 5)
    {
        // 最后一页，不再显示下一页
        m_pMenuButtonNextPage -> setVisible(false);
    }
    else if (count == 0)
    {
        // TODO: 待处理
        AppModel::getInstance()->m_pUserModel->m_pWaiting->hide();
        return;
    }
    else m_pMenuButtonNextPage -> setVisible(true);
    
    for (auto& i : *m_pTime)
    {
        // 略过第一个
        if (i == (*m_pTime).front())
        {
            i -> setPosition(timeStartPosition.x, timeStartPosition.y - height);
            addChild(i);
            height += bet;
            continue;
        }
        std::string sTime = historyList[flag].game_time.substr(5,11);
        i -> setPosition(timeStartPosition.x, timeStartPosition.y - height);
        i -> setString(sTime);
        addChild(i);
        height += bet;
        if (flag < count - 1) flag ++;
        else break;
    }
    
    height = 0;
    flag = 0;
    
    for (auto& i : *m_pOppo)
    {
        // 略过第一个
        if (i == (*m_pOppo).front())
        {
            i -> setPosition(opposantStartPosition.x, opposantStartPosition.y - height);
            addChild(i);
            height += bet;
            continue;
        }
        // 避免用户名过长，截断到第13个字符
        std::string username = historyList[flag].enemy_userinfo.sUser;
        
        if (username.size() > 13) username.replace(8, std::string::npos, "...");
        i -> setPosition(opposantStartPosition.x, opposantStartPosition.y - height);
        i -> setString(username);
        addChild(i);
        height += bet;
        if (flag < count - 1) flag ++;
        else break;
    }
    
    height = 0;
    flag = 0;
    
    for (auto& i : *m_pIsWin)
    {
        // 略过第一个
        if (i == (*m_pIsWin).front())
        {
            i -> setPosition(winsStartPosition.x, winsStartPosition.y - height);
            addChild(i);
            height += bet;
            continue;
        }
        std::string wins = historyList[flag].winner_id ==
                               historyList[flag].enemy_userinfo.nUserId ?
                               "失败" : "胜利";
        
        i -> setPosition(winsStartPosition.x, winsStartPosition.y - height);
        i -> setString(wins);
        addChild(i);
        height += bet;
        if (flag < count - 1) flag ++;
        else break;
    }
    
    height = 0;
    flag = 0;
    auto pMenu = Menu::create();
    
    pMenu -> setAnchorPoint(Vec2::ZERO);
    pMenu -> setPosition(Vec2::ZERO);
    
    for (auto& i : *m_pEnter)
    {
        // 略过第一个
        if (i == (*m_pEnter).front())
        {
            i -> setPosition(scoreStartPosition.x, scoreStartPosition.y - height);
            addChild(i);
            height += bet;
            continue;
        }
        
        i -> setPosition(scoreStartPosition.x, scoreStartPosition.y - height);
        i -> setScale(0.6f);
        pMenu -> addChild(i);
        height += bet;
        if (flag < count - 1) flag ++;
        else break;
    }

    addChild(pMenu);
    AppModel::getInstance()->m_pUserModel->m_pWaiting->hide();
}

void HistoryBoard::onNextPageClicked(cocos2d::Ref *pSender)
{
    AppModel::getInstance()->m_pUserModel->m_pWaiting->show(15, "请稍等……");
    log("历史记录点击下一页");
    m_pMenuButtonNextPage -> setVisible(false);
    m_pMenuButtonLastPage -> setVisible(false);
    m_Pageid++;
    // 移除
    for (auto& i:(*m_pTime)) i -> removeFromParent();
    for (auto& i:(*m_pOppo)) i -> removeFromParent();
    for (auto& i:(*m_pEnter)) i -> removeFromParent();
    for (auto& i:(*m_pIsWin)) i -> removeFromParent();
    ValueMap oData;
    auto usrinfo = AppModel::getInstance() -> m_pUserModel ->getMyUserInfo();
    oData.insert(std::make_pair("player_id",(int)usrinfo.nUserId));
    oData.insert(std::make_pair("limit",5));
    oData.insert(std::make_pair("offset",0 + 5 * (m_Pageid - 1)));
    dispatchEvent(EVT_GET_HISTORY,&oData);
    std::stringstream lableText;
    lableText << "第"<<m_Pageid<<"页";
    m_pLablePageId -> setString(lableText.str());
}

void HistoryBoard::onLastPageClicked(cocos2d::Ref *pSender)
{
    log("历史记录点击上一页");
    m_pMenuButtonNextPage -> setVisible(false);
    m_pMenuButtonLastPage -> setVisible(false);
    AppModel::getInstance()->m_pUserModel->m_pWaiting->show(15, "请稍等……");
    if (m_Pageid > 1)
    {
        m_Pageid--;
        // 移除
        for (auto& i:(*m_pTime)) i -> removeFromParent();
        for (auto& i:(*m_pOppo)) i -> removeFromParent();
        for (auto& i:(*m_pEnter)) i -> removeFromParent();
        for (auto& i:(*m_pIsWin)) i -> removeFromParent();
        ValueMap oData;
        auto usrinfo = AppModel::getInstance() -> m_pUserModel ->getMyUserInfo();
        oData.insert(std::make_pair("player_id",(int)usrinfo.nUserId));
        oData.insert(std::make_pair("limit",5));
        oData.insert(std::make_pair("offset",0 + 5 * (m_Pageid - 1)));
        dispatchEvent(EVT_GET_HISTORY,&oData);
        std::stringstream lableText;
        lableText << "第"<<m_Pageid<<"页";
        m_pLablePageId -> setString(lableText.str());
    }
}

void HistoryBoard::onEnterClicked(cocos2d::Ref *pSender)
{
//    auto history = AppModel::getInstance()->m_pUserModel -> getHistoryList()[i];
//    KKUSERINFO oppinfo = history.enemy_userinfo;
//    AppModel::getInstance()->m_pUserModel -> setOppUserInfo(oppinfo);
//    auto pShowhistoryScene = ShowHistoryScene::createScene();
//    Director::getInstance() -> replaceScene(pShowhistoryScene);
//    log("进入对局查看");
}








