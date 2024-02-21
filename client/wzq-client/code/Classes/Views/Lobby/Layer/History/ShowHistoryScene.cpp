//
//  ShowHistoryScene.cpp
//  wzqgame-mobile
//
//  Created by duole on 2024/2/2.
//

#include "ShowHistoryScene.hpp"
#include "Core/AppFunc.h"
#include "Core/AppEvent.h"
#include "Core/AppModel.h"
#include "Controllers/WZQLoginController.h"
#include <iostream>
#include <sstream>
#include "SigninLayer.hpp"
#include "WZQDeviceSize.hpp"
#include "Core/AppRes.h"
#include "Core/AppModel.h"
#include "LobbyScene.hpp"
#include "UserInfoLable.hpp"
#include "paohuziProtocol.pb.h"
#include "WinOrLoseLayer.hpp"
#include "AckUndoLayer.hpp"
#include "WaitAckLayer.hpp"
#include "SimpleAudioEngine.h"
#include "PopTips.hpp"
#include <iostream>
USING_NS_CC;
using namespace std;

Scene* ShowHistoryScene::createScene()
{
    auto scene = Scene::create();
    auto layer = ShowHistoryScene::create();
    scene->addChild(layer);
    return scene;
}

bool ShowHistoryScene::init()
{
    if(!Layer::init())
    {
        return false;
    }
    CocosDenshion::SimpleAudioEngine::getInstance() -> preloadEffect(SOUND_PUTCHESS);
    
    auto vsize = Director::getInstance()->getVisibleSize();
    Sprite* bg = Sprite::create(BG_LOBBY);
    bg->setAnchorPoint(Vec2(0.5,0.5));
    bg->setPosition(WZQDeviceSize::getViewCenterPos());
    bg->setScale(WZQDeviceSize::getContentScaleInWidth(bg, 1.0f));
    addChild(bg);
    
    //获取对方信息
    unsigned long long opp_id = AppModel::getInstance()->m_pUserModel -> m_nOppUserId;
    m_oppinfo = AppModel::getInstance()->m_pUserModel -> getUserInfo(opp_id);
    m_history =AppModel::getInstance()->m_pUserModel -> m_HistoryList[AppModel::getInstance() -> m_pUserModel -> m_historyId];
    printf("测试：第%d局\n",AppModel::getInstance() -> m_pUserModel -> m_historyId);
    m_sum_step = m_history.black_position.size() + m_history.white_positon.size();
    printf("总步数:%d",m_sum_step);
    printf("黑棋步数：%d,白棋步数：%d",m_history.black_position.size(),m_history.white_positon.size());
    // 获取我方信息
    KKUSERINFO oMyInfo = AppModel::getInstance() -> m_pUserModel -> getMyUserInfo();
    
    // 获取对手信息
    cout<<"对手信息: 名字:"<<m_oppinfo.sUser<<"头像 :"<<m_oppinfo.head<<"分数 :"<<m_oppinfo.Score<<endl;
    
    // 对方信息
    auto pChessBoardBack = Sprite::create(CHESSBACK);
    pChessBoardBack -> setPosition(WZQDeviceSize::getViewCenterPos());
    pChessBoardBack -> setScale(WZQDeviceSize::getContentScaleInHeight(pChessBoardBack, 1.0f));
    addChild(pChessBoardBack);
    CocosDenshion::SimpleAudioEngine::getInstance() -> preloadEffect(SOUND_WARNING);
    m_pChessBoardImage = Sprite::create(CHESSBOARD13);
    m_pChessBoardImage -> setPosition(WZQDeviceSize::getViewCenterPos());
    m_pChessBoardImage -> setScale(WZQDeviceSize::getContentScaleInHeight(m_pChessBoardImage, 0.8f));
    addChild(m_pChessBoardImage);
    
    auto pOppoInfo = UserInfoLable::create();
    pOppoInfo -> setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    pOppoInfo -> setPosition(0, WZQDeviceSize::getViewCenterPos().y * 2);
    pOppoInfo -> setScale(WZQDeviceSize::getContentScaleInWidth(pOppoInfo, 0.2f));
    pOppoInfo -> setInfo(m_oppinfo.head, m_oppinfo.sUser, m_oppinfo.Score);
    m_oppoPos = Vec2(pOppoInfo -> getPosition().x +
                     pOppoInfo -> getContentSize().width *
                     pOppoInfo -> getScaleX() / 2,
                     pOppoInfo -> getPosition().y -
                     pOppoInfo -> getContentSize().height *
                     pOppoInfo -> getScaleY() * 1.5);
    addChild(pOppoInfo);
//
    cout<<"我方信息: 名字:"<<oMyInfo.sUser<<"头像 :"<<oMyInfo.head<<"分数 :"<<oMyInfo.Score<<endl;
    auto pMyInfo = UserInfoLable::create();
    pMyInfo -> setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    pMyInfo -> setPosition(0, 0);
    pMyInfo -> setScale(WZQDeviceSize::getContentScaleInWidth(pOppoInfo, 0.2f));
    pMyInfo -> setInfo(oMyInfo.head, oMyInfo.sUser, oMyInfo.Score);
    m_myPos = Vec2(pMyInfo -> getPosition().x +
                   pMyInfo -> getContentSize().width *
                   pMyInfo -> getScaleX() / 2,
                   pMyInfo -> getPosition().y +
                   pMyInfo -> getContentSize().height *
                   pMyInfo -> getScaleY() * 1.5);
    addChild(pMyInfo);
    
    //上一步按钮
    m_pMenuItmPre = MenuButton::create("上一步", CC_CALLBACK_1(ShowHistoryScene::onPreClicked, this));
    m_pMenuItmPre -> setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    m_pMenuItmPre -> setPosition(vsize.width, vsize.height*2/3);
    
    //下一步按钮
    m_pMenuItmNxt = MenuButton::create("下一步", CC_CALLBACK_1(ShowHistoryScene::onNxtClicked, this));
    m_pMenuItmNxt -> setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    m_pMenuItmNxt -> setPosition(vsize.width, vsize.height*1/3);
    
    //退出回放按钮
    m_pMenuItmExit = MenuButton::create("退出回放", CC_CALLBACK_1( ShowHistoryScene::onExitGameClicked, this));
    m_pMenuItmExit -> setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    m_pMenuItmExit -> setPosition(vsize.width, vsize.height);
    
    m_pMenu = Menu::create(m_pMenuItmPre, m_pMenuItmNxt,m_pMenuItmExit, NULL);
    m_pMenu->setPosition(Vec2::ZERO);
    m_pMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(m_pMenu);
    
    m_LB = Vec2(m_pChessBoardImage -> getPosition().x -
                m_pChessBoardImage -> getContentSize().width *
                m_pChessBoardImage -> getScaleX() / 2,
                m_pChessBoardImage -> getPosition().y -
                m_pChessBoardImage -> getContentSize().height *
                m_pChessBoardImage -> getScaleY() / 2);
    
    m_RT = Vec2(m_pChessBoardImage -> getPosition().x +
                m_pChessBoardImage -> getContentSize().width *
                m_pChessBoardImage -> getScaleX() / 2,
                m_pChessBoardImage -> getPosition().y +
                m_pChessBoardImage -> getContentSize().height *
                m_pChessBoardImage -> getScaleY() / 2);

    
    float diff = m_pChessBoardImage -> getContentSize().width *
                 m_pChessBoardImage -> getScaleX() / 12;
    m_diff = diff;
    
    // 放置黑棋白旗罐
    auto pWhiteImage = Sprite::create(WHITEBOWL);
    auto pBlackImage = Sprite::create(BLACKBOWL);
    pWhiteImage -> setScale(WZQDeviceSize::getContentScaleInWidth(pWhiteImage ,0.1f));
    pWhiteImage -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    pBlackImage -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    pBlackImage -> setScale(WZQDeviceSize::getContentScaleInWidth(pBlackImage ,0.1f));
    addChild(pWhiteImage);
    addChild(pBlackImage);
    
    if(m_history.black_id == m_history.enemy_userinfo.nUserId)
    {
        pBlackImage -> setPosition(m_oppoPos);
        pWhiteImage -> setPosition(m_myPos);
    }
    else
    {
        pBlackImage -> setPosition(m_myPos);
        pWhiteImage -> setPosition(m_oppoPos);

    }
    
    return true;
}

void ShowHistoryScene::onEnter()
{
    
    Layer::onEnter();
}
void ShowHistoryScene::onExit()
{
    Layer::onExit();
}

ShowHistoryScene::ShowHistoryScene():m_pMenu(nullptr),m_black_step(0),m_white_step(0),m_step(0)
{
    printf("初始化m_step:%d",m_step);
}


ShowHistoryScene::~ShowHistoryScene()
{
}
void ShowHistoryScene::onPreClicked(Ref *pSender){
    if(m_step == 0 ){
        printf("没有上一步了\n");
    }
    else{
        if(m_step %2 ==0){
            printf("移除白棋子%d测试\n",m_white_step);
            m_white_step--;
            m_ChessVector.back() -> removeFromParent();
            m_ChessVector.popBack();
        }
        else{
            printf("移除黑棋子%d测试\n",m_black_step);
            m_black_step--;
            m_ChessVector.back() -> removeFromParent();
            m_ChessVector.popBack();
        }
        m_step--;
    }
}
void ShowHistoryScene::onNxtClicked(Ref *pSender){
    if(m_step == m_sum_step){


        string message = "所有步骤都已经演示!\n";
        if(m_history.is_force_exit)
        {
            if (m_history.enemy_userinfo.nUserId == m_history.loser_id)
                message += "对方异常退出，";
            else
                message += "我方异常退出，";
        }
        if(m_history.winner_id == m_history.black_id)
        {
            message += "黑方胜利。";
        }
        else
        {
            message += "白方胜利。";
        }
        
        PopTips::pop(this, message);
        
        printf("所有步骤都已经演示!\n");
    }
    else{
        if(m_step %2 ==0){
            int row = m_history.black_position[m_black_step].row;
            int col = m_history.black_position[m_black_step].col;
            printf("将演示黑棋第%d步位置是%d行,%d列\n",m_black_step+1,row,col);
            cocos2d::Vec2 roundedLos = Vec2(col,row);
            auto chess = drawChess(roundedLos, true);
            m_ChessVector.pushBack(chess);
            m_black_step ++;
        }
        else{
            int row = m_history.white_positon[m_white_step].row;
            int col = m_history.white_positon[m_white_step].col;
            printf("将演示白棋第%d步位置是%d行,%d列\n",m_white_step+1,row,col);
            cocos2d::Vec2 roundedLos = Vec2(col,row);
            auto chess = drawChess(roundedLos, false);
            m_ChessVector.pushBack(chess);
            m_white_step ++;
        }
        m_step++;
    }
}

void ShowHistoryScene::onExitGameClicked(cocos2d::Ref *pSender) {
    log("（对局回放）点击退出游戏");
    
    // 发送一个退出回放的弹窗
    auto layer = ReminderWindow::create();
    layer -> setText("温馨提示", "确定是否要退出回放吗？");
    // 确定按钮
    auto MenuButtonComfirm = MenuButton::create("取消", [layer](Ref* sender){
        log("取消退出游戏。");
        layer -> removeFromParent();
    });
    
    auto MenuButtonRefuse = MenuButton::create("确认", [=](Ref* sender){
        log("确定退出对局回放。");
        auto pLobbyScene = LobbyScene::createScene();
        Director::getInstance() -> replaceScene(pLobbyScene);
    });
    
    layer -> addButton(MenuButtonComfirm);
    layer -> addButton(MenuButtonRefuse);
    
    auto pMenu = Menu::create(MenuButtonComfirm,MenuButtonRefuse, NULL);
    
    pMenu->setPosition(Vec2::ZERO);
    pMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    

    addChild(layer);
    layer -> addChild(pMenu);
}

Sprite* ShowHistoryScene::drawChess(cocos2d::Vec2 roundedLos, bool isBlack)
{
    Sprite* pChess;
    if (isBlack)
        pChess = Sprite::create(BLACKCHESS);
    else
        pChess = Sprite::create(WHITECHESS);
    
    pChess -> setScale(WZQDeviceSize::getCertainPixelScale(pChess, int(m_diff)) * 1.2f);
    pChess -> setPosition(m_LB.x + roundedLos.x * m_diff, m_LB.y + roundedLos.y * m_diff);
    addChild(pChess);
    CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect(SOUND_PUTCHESS);
    
    auto TurnSmall = ScaleTo::create(0.3f, WZQDeviceSize::getCertainPixelScale(pChess, int(m_diff)));
    
    pChess -> runAction(TurnSmall);
    return pChess;
}
