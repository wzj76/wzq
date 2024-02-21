//
//  BaseBaseInGameScene.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/31.
//

#include "BaseInGameScene.hpp"
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
#include "paohuziProtocol.pb.h"
#include "WinOrLoseLayer.hpp"
#include "SimpleAudioEngine.h"
#include "Bubble.hpp"


#include <iostream>
USING_NS_CC;
using namespace std;

Scene* BaseInGameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = BaseInGameScene::create();
    scene->addChild(layer);
    return scene;
}

bool BaseInGameScene::init()
{
    if(!Layer::init())
    {
        return false;
    }
    CocosDenshion::SimpleAudioEngine::getInstance() -> preloadEffect(SOUND_PUTCHESS);
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(SOUND_INGAMEBGM);
    if(AppModel::getInstance()->m_pUserModel->m_setupConf.background_music)
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(SOUND_INGAMEBGM);
    else
        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    auto vsize = Director::getInstance()->getVisibleSize();
    Sprite* bg = Sprite::create(BG_LOBBY);
    bg->setAnchorPoint(Vec2(0.5,0.5));
    bg->setPosition(WZQDeviceSize::getViewCenterPos());
    bg->setScale(WZQDeviceSize::getContentScaleInWidth(bg, 1.0f));
    addChild(bg);

    // 获取我方信息
    KKUSERINFO oMyInfo = AppModel::getInstance() -> m_pInGameModel -> m_gameConf.oMyInfo;
    
    // 获取对手信息
    KKUSERINFO oOpponentInfo = AppModel::getInstance() -> m_pInGameModel -> m_gameConf.oOppoInfo;
    cout<<"对手信息: 名字:"<<oOpponentInfo.sUser<<"头像 :"<<oOpponentInfo.head<<"分数 :"<<oOpponentInfo.Score<<endl;
    
    // 对方信息
    auto pChessBoardBack = Sprite::create(CHESSBACK);
    pChessBoardBack -> setPosition(WZQDeviceSize::getViewCenterPos());
    pChessBoardBack -> setScale(WZQDeviceSize::getContentScaleInHeight(pChessBoardBack, 1.0f));
    addChild(pChessBoardBack);
    
    // logo
    auto pLogo = Sprite::create(BG_LOGO);
    pLogo -> setPosition(WZQDeviceSize::getViewCenterPos());
    pLogo -> setAnchorPoint(Vec2(0.5f, 0.4f));
    pLogo -> setScale(WZQDeviceSize::getContentScaleInHeight(pLogo, 0.6f));
    pLogo -> setOpacity(64);
    addChild(pLogo);
    
    // 根据游戏信息设置棋盘大小
    switch (AppModel::getInstance()->m_pInGameModel->m_gameConf.boardSize) {
        case Size13x13:
        {
            m_pChessBoardImage = Sprite::create(CHESSBOARD13);
            m_chessBoardSize = 12;
            break;
        }
        
        case Size15x15:
        {
            m_pChessBoardImage = Sprite::create(CHESSBOARD15);
            m_chessBoardSize = 14;
            break;
        }
        case Size17x17:
        {
            m_pChessBoardImage = Sprite::create(CHESSBOARD17);
            m_chessBoardSize = 16;
            break;
        }
        case Size19x19:
        {
            m_pChessBoardImage = Sprite::create(CHESSBOARD19);
            m_chessBoardSize = 18;
            break;
        }
        default:
        {
            CCASSERT(false, "未找到棋盘大小信息！");
            break;
        }
    }
    
    m_pChessBoardImage -> setPosition(WZQDeviceSize::getViewCenterPos());
    m_pChessBoardImage -> setScale(WZQDeviceSize::getContentScaleInHeight(m_pChessBoardImage, 0.8f));
    addChild(m_pChessBoardImage);
    
    pMenuItmExitGame = MenuButton::create("临时退出游戏", CC_CALLBACK_1( BaseInGameScene::onExitGameClicked, this));
    pMenuItmExitGame -> setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    pMenuItmExitGame -> setPosition(vsize.width, vsize.height);

    m_pMenu = Menu::create(pMenuItmExitGame, NULL);

    m_pMenu->setPosition(Vec2::ZERO);
    m_pMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    m_pOppoInfo = UserInfoLable::create();
    m_pOppoInfo -> setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    m_pOppoInfo -> setPosition(0, WZQDeviceSize::getViewCenterPos().y * 2);
    m_pOppoInfo -> setScale(WZQDeviceSize::getContentScaleInWidth(m_pOppoInfo, 0.2f));
    m_pOppoInfo -> setInfo(oOpponentInfo.head, oOpponentInfo.sUser, oOpponentInfo.Score);
    m_oppoPos = Vec2(m_pOppoInfo -> getPosition().x +
                     m_pOppoInfo -> getContentSize().width *
                     m_pOppoInfo -> getScaleX() / 2,
                     m_pOppoInfo -> getPosition().y -
                     m_pOppoInfo -> getContentSize().height *
                     m_pOppoInfo -> getScaleY() * 1.5);
    addChild(m_pOppoInfo);
//
    m_pMyInfo = UserInfoLable::create();
    m_pMyInfo -> setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    m_pMyInfo -> setPosition(0, 0);
    m_pMyInfo -> setScale(WZQDeviceSize::getContentScaleInWidth(m_pOppoInfo, 0.2f));
    m_pMyInfo -> setInfo(oMyInfo.head, oMyInfo.sUser, oMyInfo.Score);
    m_myPos = Vec2(m_pMyInfo -> getPosition().x +
                   m_pMyInfo -> getContentSize().width *
                   m_pMyInfo -> getScaleX() / 2,
                   m_pMyInfo -> getPosition().y +
                   m_pMyInfo -> getContentSize().height *
                   m_pMyInfo -> getScaleY() * 1.5);
    addChild(m_pMyInfo);
    
    addChild(m_pMenu);
    
    // 轮到谁的提示
    if (AppModel::getInstance() -> m_pInGameModel -> m_gameConf.gamemode != BotMode)
    {
        m_pTips = Sprite::create(REDSIGN);
        m_pTips -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        m_pTips -> setScale(WZQDeviceSize::getContentScaleInHeight(m_pTips, 0.1f));
        m_pTips -> setPosition(AppModel::getInstance() -> m_pInGameModel -> m_IsMe ?
                               m_myPos : m_oppoPos);
        addChild(m_pTips);
    //倒计时
    m_my_time = 20;
    m_opp_time = 20;
    m_my_remain = Label::createWithSystemFont("20", "黑体", 20);
    m_my_remain -> setPosition(m_pTips -> getContentSize().width / 2,
                               m_pTips -> getContentSize().height / 2);

    m_opp_remain = Label::createWithSystemFont("20", "黑体", 20);
    m_opp_remain -> setPosition(m_pTips -> getContentSize().width / 2,
                                m_pTips -> getContentSize().height / 2);
    m_pTips -> addChild(m_my_remain);
    m_pTips -> addChild(m_opp_remain);
    
    
    if(AppModel::getInstance() -> m_pInGameModel -> m_IsMe)
    {
        m_opp_remain ->setVisible(false);

    }
    else
    {
        m_my_remain ->setVisible(false);
    }
    
    }
    // 放置黑棋白旗罐
    auto pWhiteImage = Sprite::create(WHITEBOWL);
    auto pBlackImage = Sprite::create(BLACKBOWL);
    pWhiteImage -> setScale(WZQDeviceSize::getContentScaleInWidth(pWhiteImage ,0.1f));
    pWhiteImage -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    pBlackImage -> setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    pBlackImage -> setScale(WZQDeviceSize::getContentScaleInWidth(pBlackImage ,0.1f));
    addChild(pWhiteImage);
    addChild(pBlackImage);
    
    cout<<"黑方id" <<AppModel::getInstance() -> m_pInGameModel -> m_gameConf.llBlackId<<std::endl;
    if(AppModel::getInstance() -> m_pInGameModel -> m_gameConf.llBlackId == oMyInfo.nUserId)
    {
        cout<<"黑方："<<oMyInfo.nUserId<<std::endl;
        pBlackImage -> setPosition(m_myPos);
        pWhiteImage -> setPosition(m_oppoPos);
    }
    else
    {
        cout<<"黑方："<<oOpponentInfo.nUserId<<std::endl;
        pBlackImage -> setPosition(m_oppoPos);
        pWhiteImage -> setPosition(m_myPos);
    }
    
    m_pScrollLayer = ScrollLayer::create();
    addChild(m_pScrollLayer);
    
    
    // 注册触摸事件监听器
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(BaseInGameScene::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
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
                 m_pChessBoardImage -> getScaleX() / m_chessBoardSize;
    m_diff = diff;
    
    return true;
}

void BaseInGameScene::onEnter()
{
    Layer::onEnter();
    
}

void BaseInGameScene::onExit()
{
    Layer::onExit();
}

BaseInGameScene::BaseInGameScene():m_pMenu(nullptr), m_isComfirm(false), m_pSign(nullptr)
{

}

BaseInGameScene::~BaseInGameScene()
{
}

void BaseInGameScene::onExitGameClicked(cocos2d::Ref *pSender)
{
    log("点击退出游戏");
    // 弹出是否退出游戏的弹窗
    auto layer = ReminderWindow::create();
    layer -> setText("温馨提示", "确定是否要退出游戏？");
    // 确定按钮
    auto MenuButtonComfirm = MenuButton::create("取消", [layer](Ref* sender){
        log("取消退出游戏。");
        layer -> removeFromParent();
    });
    
    auto MenuButtonRefuse = MenuButton::create("确认", [=](Ref* sender){
        log("确定退出游戏。");
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
    
//    auto lobbyScene = LobbyScene::createScene();
//    Director::getInstance() -> replaceScene(lobbyScene);
}

bool BaseInGameScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{

    std::cout<<"点击位置：("<<touch -> getLocation().x<<", "<<touch->getLocation().y<<")"<<std::endl;
    
    Vec2 touchLos = touch -> getLocation();
    
    if (m_LB.x < touchLos.x && touchLos.x < m_RT.x && m_LB.y < touchLos.y && touchLos.y < m_RT.y)
        {
            std::cout<< "点击棋盘相对位置：("
                     <<(touchLos.x - m_LB.x)/(m_RT.x - m_LB.x)
                     <<","
                     <<(touchLos.y - m_LB.y)/(m_RT.y - m_LB.y)
                     <<")"
                     <<std::endl;
            Vec2 reCoord = toRelativeCoord(Vec2((touchLos.x - m_LB.x) / (m_RT.x - m_LB.x), (touchLos.y - m_LB.y)/(m_RT.y - m_LB.y)));
            
            std::cout<< reCoord.x << m_Choosed.x
                     << reCoord.y << m_Choosed.y << std::endl;
            
            if (m_isComfirm &&
                (int)reCoord.x == (int)m_Choosed.x &&
                (int)reCoord.y == (int)m_Choosed.y)
            {
                m_isComfirm = false;
                putChess(Vec2((touchLos.x - m_LB.x)/(m_RT.x - m_LB.x), (touchLos.y - m_LB.y)/(m_RT.y - m_LB.y)));
            }
            else
            {
                putSign(Vec2((touchLos.x - m_LB.x)/(m_RT.x - m_LB.x), (touchLos.y - m_LB.y)/(m_RT.y - m_LB.y)));
                m_isComfirm = true;
            }
        }
    return true;
}

void BaseInGameScene::putChess(cocos2d::Vec2 location)
{
    Vec2 roundedLos(int(round((location * m_chessBoardSize).x)), int(round((location * m_chessBoardSize).y)));
    std::cout<< "确定棋盘坐标为：("<<roundedLos.x<<", "<<roundedLos.y<<")"<<std::endl;
    if(m_pSign)
    {
        m_pSign -> removeFromParent();
        m_pSign = nullptr;
    }
    if(AppModel::getInstance() -> m_pInGameModel -> m_IsMe == false)
    {
        std::cout<<"没到你的回合"<<std::endl;
        return;
    }

    if (m_isAlreadyPut[roundedLos])
    {
        log("已经放置棋子。");
        return;
    }
//    AppModel::getInstance() -> m_pUserModel -> m_pWaiting -> show(10, "等待服务器响应……");
//    ValueMap oData;
//    oData.insert(make_pair("req_msg" ,Value("put_chess")));
//    oData.insert(make_pair("table_id" ,Value(AppModel::getInstance() -> m_pInGameModel -> m_gameConf.iRoomId)));
//    KKUSERINFO oMyInfo = AppModel::getInstance() -> m_pUserModel -> getMyUserInfo();
//    oData.insert(make_pair("action_id" ,Value((int)oMyInfo.nUserId)));
//    oData.insert(make_pair("action_board_row" ,Value(roundedLos.x)));
//    oData.insert(make_pair("action_board_col" , Value(roundedLos.y)));
//    dispatchEvent(EVT_PLACE_A_STONE, &oData);
}

Sprite* BaseInGameScene::drawChess(cocos2d::Vec2 roundedLos, bool isBlack)
{
    delSign();
    Sprite* pChess;
    m_isAlreadyPut[roundedLos] = true;
    if (isBlack)
        pChess = Sprite::create(BLACKCHESS);
    else
        pChess = Sprite::create(WHITECHESS);
    
    pChess -> setScale(WZQDeviceSize::getCertainPixelScale(pChess, int(m_diff)) * 1.2f);
    pChess -> setPosition(m_LB.x + roundedLos.x * m_diff, m_LB.y + roundedLos.y * m_diff);
    addChild(pChess);
    if(AppModel::getInstance()->m_pUserModel->m_setupConf.sound_effect)
        CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect(SOUND_PUTCHESS);
    
    auto TurnSmall = ScaleTo::create(0.3f, WZQDeviceSize::getCertainPixelScale(pChess, int(m_diff)));
    if(AppModel::getInstance()->m_pUserModel->m_setupConf.chess_effect)
        pChess -> runAction(TurnSmall);
    else
        pChess -> setScale(WZQDeviceSize::getCertainPixelScale(pChess, int(m_diff)) * 1.0f);
    drawSign(roundedLos);
    return pChess;
}

void BaseInGameScene::randChess()
{
    cocos2d::Vec2 randLos;
    for(int x=0;x<13;x++)
    {
        for(int y=0;y<13;y++){
            randLos = Vec2(x,y);
            if(m_isAlreadyPut[randLos] == false){
                randLos = Vec2((x * m_diff)/(m_RT.x - m_LB.x),( y * m_diff)/(m_RT.y - m_LB.y));
                putChess(randLos);
                return;
            }
        }
    }
}

void BaseInGameScene::animUpdate(float t)
{
    //己方倒计时
    if(AppModel::getInstance() -> m_pInGameModel -> m_IsMe)
    {
        m_my_remain -> setVisible(true);
        m_opp_remain -> setVisible(false);
        m_opp_time = 21;
        m_my_time -=1;
        if(m_my_time <0){
            printf("玩家已超时,将为您随机落子\n");
            //TODO 超时弹窗
            randChess();
            m_my_time = 20;
        }
        std::string new_time = to_string(m_my_time);
        m_my_remain -> setString(new_time);
    }
    //对手倒计时
    else{
        m_my_remain -> setVisible(false);
        m_opp_remain -> setVisible(true);
        m_my_time = 21;
        m_opp_time -=1;
        if(m_opp_time <0){
            printf("对方已超时");
            m_opp_time = 20;
        }
        std::string new_time = to_string(m_opp_time);
        m_opp_remain -> setString(new_time);
    }
}

cocos2d::Vec2 BaseInGameScene::toRelativeCoord(cocos2d::Vec2 location)
{
    return Vec2(int(round((location * m_chessBoardSize).x)), int(round((location * m_chessBoardSize).y)));
}

void BaseInGameScene::putSign(cocos2d::Vec2 location)
{
    if (m_pSign)
    {
        m_pSign -> removeFromParent();
        m_pSign = nullptr;
    }
    Vec2 roundedLos(int(round((location * m_chessBoardSize).x)), int(round((location * m_chessBoardSize).y)));
    std::cout<< "确定棋盘坐标为：("<<roundedLos.x<<", "<<roundedLos.y<<")"<<std::endl;
    
    if(AppModel::getInstance() -> m_pInGameModel -> m_IsMe == false)
    {
        std::cout<<"没到你的回合"<<std::endl;
        return;
    }

    if (m_isAlreadyPut[roundedLos])
    {
        log("已经放置棋子。");
        return;
    }
    drawSign(roundedLos);
    m_Choosed = roundedLos;
    
}

void BaseInGameScene::drawSign(cocos2d::Vec2 roundedLos)
{
    m_pSign = Sprite::create(COMFIRMSIGN);
    
    m_pSign -> setScale(WZQDeviceSize::getCertainPixelScale(m_pSign, int(m_diff)) * 0.5);
    m_pSign -> setPosition(m_LB.x + roundedLos.x * m_diff, m_LB.y + roundedLos.y * m_diff);
    addChild(m_pSign);
}

void BaseInGameScene::delSign()
{
    if (m_pSign)
    {
        m_pSign -> removeFromParent();
        m_pSign = nullptr;
    }
}

void BaseInGameScene::gameOverLayer(std::string tips)
{
    m_gameOverLayer = ReminderWindow::create();
    m_gameOverLayer-> setText("游戏结束",tips);
    auto pComfirm = MenuButton::create("退出", [this](Ref* pSender){
        log("游戏结束，退出。");
        auto pLobby = LobbyScene::createScene();
        Director::getInstance() -> replaceScene(pLobby);
    });
    
    
    auto pBack = MenuButton::create("查看残局", [this](Ref* pSender){
        auto pGoBack = MenuButton::create("返回", [this](Ref* sender){
            m_gameOverLayer -> setVisible(true);
            auto* menuButton = dynamic_cast<MenuButton*>(sender);
            menuButton -> setVisible(false);
        });
        pGoBack -> setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        pGoBack -> setPosition(WZQDeviceSize::getViewCenterPos().x * 2, WZQDeviceSize::getViewCenterPos().y * 2);
        
        auto pMenu = Menu::create(pGoBack, NULL);
        pMenu -> setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        pMenu -> setPosition(Vec2::ZERO);
        
        this -> addChild(pMenu);
        
        log("游戏结束，查看残局。");
        m_gameOverLayer -> setVisible(false);
    });
    
    m_gameOverLayer -> addButton(pComfirm);
    m_gameOverLayer -> addButton(pBack);

    m_gameOverLayer -> initButtons();
    addChild(m_gameOverLayer);
}








