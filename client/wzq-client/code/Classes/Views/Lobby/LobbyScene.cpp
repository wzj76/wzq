//
//  LobbyScene.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/24.
//

#include "LobbyScene.hpp"
#include <stdio.h>
#include <iostream>
#include "RegistLayer.h"
#include "ReminderWindow.hpp"
#include "Core/AppEvent.h"
#include "Core/AppFunc.h"
#include "Core/AppModel.h"
#include "Controllers/WZQLoginController.h"
#include "paohuziProtocol.pb.h"
#include "ReminderWindow.hpp"
#include "SimpleAudioEngine.h"
#include "WZQDeviceSize.hpp"
#include "UserInfoLayer.hpp"
#include "MenuButton.hpp"
#include "FriendListLayer.hpp"
#include "LoginLayer.h"
#include "MatchWaitting.hpp"
#include "LevelBoard.hpp"
#include "HistoryBoard.hpp"
#include "PopTips.hpp"
#include "SetupLayer.hpp"
#include "BotModeChooseLayer.hpp"
#include "BeatModeChooseLayer.hpp"
#include "AskInviteFriendGameLayer.hpp"
#include "MatchingScene.hpp"
#include "ShopLayer.hpp"
#include <unistd.h>
USING_NS_CC;
using namespace std;

Scene* LobbyScene::createScene()
{
    auto scene = Scene::create();
    auto layer = LobbyScene::create();
    scene->addChild(layer);
    return scene;
}

bool LobbyScene::init()
{
    if(!Layer::init())
    {
        return false;
    }
    // 初始化按钮音效
    CocosDenshion::SimpleAudioEngine::getInstance() -> preloadEffect(SOUND_BT);
    CocosDenshion::SimpleAudioEngine::getInstance() -> stopBackgroundMusic();
    
    std::string sThemePath = AppModel::getInstance()->m_pUserModel->getThemePath();
    
    auto vsize = Director::getInstance()->getVisibleSize();
    Sprite* bg = Sprite::create(BG_LOBBY);
    bg->setAnchorPoint(Vec2(0.5,0.5));
    bg->setPosition(WZQDeviceSize::getViewCenterPos());
    bg->setScale(WZQDeviceSize::getContentScaleInWidth(bg, 1.0f));
    addChild(bg);
    
    // 上边
    auto TopmoveAction = MoveTo::create(1.0f, WZQDeviceSize::getViewLeftTopPos());
    auto TopeaseAction = EaseBackOut::create(TopmoveAction);
    
    auto pLobbyTopImage = Sprite::create(sThemePath+LOBBY_TOP);
    pLobbyTopImage -> setAnchorPoint(Vec2(0.0f, 1.0f));
    pLobbyTopImage -> setScale(WZQDeviceSize::getContentScaleInWidth(pLobbyTopImage, 1.0f));
    pLobbyTopImage -> setPosition(WZQDeviceSize::getViewLeftTopPos().x, WZQDeviceSize::getViewLeftTopPos().y + 200);
    
    pLobbyTopImage -> runAction(TopeaseAction);
    addChild(pLobbyTopImage);
    
    // 上边显示用户名
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    m_pUserNameLable = Label::createWithTTF(usrinfo.sUser, FONT_GUHUANG, WZQDeviceSize::getFontSize());
    m_pUserNameLable -> setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    m_pUserNameLable -> setPosition(Vec2(vsize.width * 0.1f, vsize.height * 0.95f));
    addChild(m_pUserNameLable);
    // 用户名下面显示用户id
    m_pUserIDLable = Label::createWithTTF("ID: " + to_string(usrinfo.nUserId), FONT_GUHUANG, WZQDeviceSize::getFontSize());
    m_pUserIDLable -> setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    m_pUserIDLable -> setPosition(m_pUserNameLable -> getPosition());
    addChild(m_pUserIDLable);
    // TODO: 显示分数, 金币
    
    // 用户头像
    std::stringstream touxiang;
    int head_num=usrinfo.head;
    std::cout<<"大厅界面加载的头像编号为："<<head_num<<std::endl;
    touxiang<<"touxiang/head"<<head_num<<".png";
//    auto pUserImage = Sprite::create(touxiang.str());
    m_pUserImageMI = MenuButton::create("", touxiang.str(),  CC_CALLBACK_1(LobbyScene::onUserBtnClicked, this));
    m_pUserImageMI -> setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    m_pUserImageMI -> setScale(WZQDeviceSize::getCertainPixelScale(m_pUserImageMI, m_pUserNameLable -> getContentSize().height + m_pUserIDLable -> getContentSize().height));
    m_pUserImageMI -> setPosition(m_pUserIDLable -> getPosition().x, m_pUserIDLable -> getPosition().y + m_pUserIDLable -> getContentSize().height);
    WZQDeviceSize::setAnchorNoMove(m_pUserImageMI);
    
    // 金币数
    std::string coin = "金币：";
    coin += std::to_string(usrinfo.coins);
    m_pCoinLabel = Label::createWithSystemFont(coin, "黑体", WZQDeviceSize::getFontSize());
    m_pCoinLabel->setScale(WZQDeviceSize::getContentScaleInHeight(m_pCoinLabel, 0.05f));
    
    m_pCoinLabel->setPosition(Vec2(vsize.width/2, m_pUserImageMI->getPosition().y));
    addChild(m_pCoinLabel);
    
    // 下边
    auto BottomMoveAction = MoveTo::create(1.0f, Vec2(0.0f, 0.0f));
    auto BottomEaseAction = EaseBackOut::create(BottomMoveAction);
    
    auto pLobbyBottomImage = Sprite::create(sThemePath+LOBBY_BOTTOM);
    pLobbyBottomImage -> setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    pLobbyBottomImage -> setScale(WZQDeviceSize::getContentScaleInWidth(pLobbyBottomImage, 1.0f));
    pLobbyBottomImage -> Node::setPosition(0, 0 - 200);
    
    pLobbyBottomImage -> runAction(BottomEaseAction);
    addChild(pLobbyBottomImage);
    
    // 设置按钮
    auto pMenuItemBtnSetup = MenuButton::create("" ,LOBBY_SETUP, CC_CALLBACK_1(LobbyScene::onSetupClicked, this));
    pMenuItemBtnSetup->setPosition(Vec2(vsize.width * 0.9, vsize.height * 0.95));
    
    
    // 匹配按钮
    m_pMenuItemBtnMatching = MenuButton::create("", MATCHING_BUTTON, CC_CALLBACK_1(LobbyScene::onMatchingClicked, this));
    m_pMenuItemBtnMatching -> setAnchorPoint(Vec2(1.0f, 1.0f));
    m_pMenuItemBtnMatching -> setScale(WZQDeviceSize::getContentScaleInHeight(m_pMenuItemBtnMatching, 0.4f));
    m_pMenuItemBtnMatching->setPosition(vsize.width/2 ,3 * vsize.height/4 - 50);
    
    // 双人对战按钮
    auto pMenuItemBtnBeating = MenuButton::create("", BEATING_BUTTON, CC_CALLBACK_1(LobbyScene::onBeatingClicked, this));
    pMenuItemBtnBeating -> setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    pMenuItemBtnBeating -> setScale(WZQDeviceSize::getContentScaleInHeight(pMenuItemBtnBeating, 0.2f));
    pMenuItemBtnBeating -> setPosition(m_pMenuItemBtnMatching -> getPosition());
    
    // 人机模式按钮
    auto pMenuItemBtnBotMode = MenuButton::create("", BOTMODE_BUTTON, CC_CALLBACK_1(LobbyScene::onBotModeClicked, this));
    pMenuItemBtnBotMode -> setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    pMenuItemBtnBotMode -> setScale(WZQDeviceSize::getContentScaleInHeight(pMenuItemBtnBotMode, 0.2f));
    pMenuItemBtnBotMode -> setPosition(pMenuItemBtnBeating -> getPosition().x, (pMenuItemBtnBeating -> getPosition().y - pMenuItemBtnBeating -> getContentSize().height * pMenuItemBtnBeating -> getScaleY()));
    
    
    // 商城按钮
    auto pMenuItemBtnShop = MenuButton::create("", BT_SHOP,  CC_CALLBACK_1(LobbyScene::onShopClicked, this));
    pMenuItemBtnShop -> setPosition(Vec2(vsize.width * 0.1f, vsize.height * 0.07f));
    
    // 排行榜按钮
    auto pMenuItemBtnLevel = MenuButton::create("", BT_LEADERBOARD, CC_CALLBACK_1(LobbyScene::onLevelBroadClicked, this));
    pMenuItemBtnLevel -> setPosition(Vec2(vsize.width * (0.1f + 0.26f), vsize.height * 0.07f));
    
    // 历史战绩按钮
    auto pMenuItemBtnHistory = MenuButton::create("", BT_HISTORY,  CC_CALLBACK_1(LobbyScene::onHistoryGameClicked, this));
    pMenuItemBtnHistory -> setPosition(Vec2(vsize.width * (0.1f + 2 * 0.26f), vsize.height * 0.07f));
    
    // 好友按钮
    auto pMenuItemBtnFriend = MenuButton::create("", BT_FRIENDS,  CC_CALLBACK_1(LobbyScene::onFriendClicked, this));
    pMenuItemBtnFriend -> setPosition(Vec2(vsize.width * (0.1f + 3 * 0.26f), vsize.height * 0.07f));
    
    m_pMenu = Menu::create(pMenuItemBtnSetup, m_pMenuItemBtnMatching, pMenuItemBtnShop, pMenuItemBtnLevel, pMenuItemBtnFriend, pMenuItemBtnHistory, pMenuItemBtnBeating, pMenuItemBtnBotMode, m_pUserImageMI, NULL);
    m_pMenu->setPosition(Vec2::ZERO);
    m_pMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    WZQDeviceSize::setAnchorNoMove(m_pMenuItemBtnMatching);
    WZQDeviceSize::setAnchorNoMove(pMenuItemBtnBeating);
    WZQDeviceSize::setAnchorNoMove(pMenuItemBtnBotMode);
    
    
    this->scheduleOnce( schedule_selector(LobbyScene::Test), 5.0f );
    this->unschedule(schedule_selector(LobbyScene::Test));
    addChild(m_pMenu);
    
    AppModel::getInstance() -> m_pInGameModel -> m_IsGaming = false;
    return true;
}

void LobbyScene::Test(float dt)
{
    printf("定时器触发--------------\n");
}

void LobbyScene::onUserBtnClicked(cocos2d::Ref *pSender)
{
    
    //向auth发送一条获取用户信息的请求
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    ValueMap oData;
    oData.insert(make_pair("user_id",(int)usrinfo.nUserId));
    dispatchEvent(EVT_GET_USER_INFO,&oData);
    AppModel::getInstance() -> m_pUserModel -> m_pWaiting -> show();
    log("打开个人信息页面");
}

void LobbyScene::onMatchingClicked(cocos2d::Ref *pSender)
{
    AppModel::getInstance() -> m_pInGameModel -> m_gameConf.gamemode = MatchingMode;
    // TODO: 添加多种棋盘尺寸
    AppModel::getInstance() -> m_pInGameModel -> m_gameConf.boardSize = Size13x13;
    
    // TODO: 设置多种等待时间
    AppModel::getInstance() -> m_pInGameModel -> m_gameConf.iWaitingTime = 20;
    
    log("点击了匹配按钮");
    m_pBackgroundBlack = Sprite::create(BLACK_COVER);
    m_pBackgroundBlack -> setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    Vec2 scale = WZQDeviceSize::getFullScreenScale(m_pBackgroundBlack);
    m_pBackgroundBlack -> setScale(scale.x,scale.y);
    m_pMenu -> setVisible(false);
    addChild(m_pBackgroundBlack);
    
    //发出一条匹配请求，并弹出一个匹配窗口
    ValueMap oData;
    oData.insert(make_pair("message","req_match_start"));
    dispatchEvent(EVT_MATCH,&oData);
    
    auto p_matchWaittingLayer = MatchWaittingLayer::create();
    
    addChild(p_matchWaittingLayer);
}

void LobbyScene::onEnter()
{
    Layer::onEnter();
    ADD_OBSERVER(LobbyScene::buttonsBack, BUTTONS_BACK);
    ADD_OBSERVER(LobbyScene::ChangeHead, CHOOSE_DONE);
    ADD_OBSERVER(LobbyScene::putUserInfo, GET_USER_INFO_SUCCESS);
    this->schedule(schedule_selector(LobbyScene::buttonBreath), 2.0f);
    ADD_OBSERVER(LobbyScene::replaceLogin, EVT_REPLACE_LOGIN);
    ADD_OBSERVER(LobbyScene::editRed, EVT_NEW_FRIEND);
    
    ADD_OBSERVER(LobbyScene::askInviteFriendGame, WINDOW_REQ_ASK_INVITE_FRIEND_GAME);
    ADD_OBSERVER(LobbyScene::inviteFriendGameSuccess, EVT_MATCH_SUCESS);
    ADD_OBSERVER(LobbyScene::updateCoins, UPDATE_COINS);
    
    ADD_OBSERVER(LobbyScene::connFailed, CONN_FAILED);
}
void LobbyScene::connFailed(EventCustom *pEvent)
{
    //连接掉线
    auto reminder = ReminderWindow::create();
    reminder -> setText("连接断线", "回到登录界面！");
    
    auto pComfirm = MenuButton::create("确定", [](Ref* pSender){
        log("回到登录界面");
        auto LoginScene = LoginLayer :: createScene();
        Director::getInstance() -> replaceScene(LoginScene);
    });
    
    reminder -> addButton(pComfirm);
    
    reminder -> initButtons();
    
    addChild(reminder);
}
void LobbyScene::inviteFriendGameSuccess(EventCustom *pEvent)
{
    // 跳转到游戏中
    auto inGame = MatchingScene::createScene();
    // auto inGame = InGameScene::createScene();
    Director::getInstance() -> replaceScene(inGame);
}

void LobbyScene::askInviteFriendGame(EventCustom *pEvent)
{
    REQAskInviteFriendGame* pReq = static_cast<REQAskInviteFriendGame*>(pEvent->getUserData());

    auto p_askLayer = AskInviteFriendGameLayer::create();
    AppModel::getInstance()->m_pUserModel->m_frined_name =pReq->friend_info().username();
    p_askLayer->m_friend_id = pReq->friend_info().id();
    addChild(p_askLayer);
}


void LobbyScene::replaceLogin(EventCustom *pEvent)
{
    log("用户异地登录，回到主界面");
    
    auto reminder = ReminderWindow::create();
    reminder -> setText("登录异常", "账号在其他地方已经登录！");
    
    auto pComfirm = MenuButton::create("确定", [](Ref* pSender){
        log("回到登录界面");
        auto LoginScene = LoginLayer :: createScene();
        Director::getInstance() -> replaceScene(LoginScene);
    });
    
    reminder -> addButton(pComfirm);
    
    reminder -> initButtons();
    
    addChild(reminder);
}

void LobbyScene::onExit()
{
    Layer::onExit();
}

LobbyScene::LobbyScene():m_pMenu(nullptr), m_pBackgroundBlack(nullptr), m_pUserImageMI(nullptr), m_pUserNameLable(nullptr), m_pUserIDLable(nullptr)
{
    m_pWaiting = WZQWaitingLayer::sharedInstance();
}

LobbyScene::~LobbyScene()
{
}

void LobbyScene::onShopClicked(cocos2d::Ref *pSender)
{
    log("点击商城按钮");
    m_pBackgroundBlack = Sprite::create(BLACK_COVER);
    m_pBackgroundBlack -> setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    Vec2 scale = WZQDeviceSize::getFullScreenScale(m_pBackgroundBlack);
    m_pBackgroundBlack -> setScale(scale.x,scale.y);
    m_pMenu -> setVisible(false);
    addChild(m_pBackgroundBlack);
    auto pShopLayer = ShopLayer::create();
    addChild(pShopLayer);
}

void LobbyScene::onLevelBroadClicked(cocos2d::Ref *pSender)
{
    log("点击排行榜按钮");
    //发出一条排行榜获取请求
    ValueMap oData;
    oData.insert(make_pair("message","req_get_randing_list"));
    oData.insert(make_pair("limit",10));
    oData.insert(make_pair("offset",0));
    dispatchEvent(EVT_RANDING_LIST,&oData);
    

    m_pBackgroundBlack = Sprite::create(BLACK_COVER);
    m_pBackgroundBlack -> setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    Vec2 scale = WZQDeviceSize::getFullScreenScale(m_pBackgroundBlack);
    m_pBackgroundBlack -> setScale(scale.x,scale.y);
    m_pMenu -> setVisible(false);
    addChild(m_pBackgroundBlack);
    auto pLevelBoardLayer = LevelBoard::create();
    addChild(pLevelBoardLayer);
}

void LobbyScene::onHistoryGameClicked(cocos2d::Ref *pSender)
{
    log("点击历史记录按钮");
    m_pBackgroundBlack = Sprite::create(BLACK_COVER);
    m_pBackgroundBlack -> setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    Vec2 scale = WZQDeviceSize::getFullScreenScale(m_pBackgroundBlack);
    m_pBackgroundBlack -> setScale(scale.x,scale.y);
    m_pMenu -> setVisible(false);
    addChild(m_pBackgroundBlack);
    
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    
    auto pHistory = HistoryBoard::create();
    addChild(pHistory);
    
    ValueMap oData;
    oData.insert(make_pair("player_id",(int)usrinfo.nUserId));
    oData.insert(make_pair("limit",5));
    oData.insert(make_pair("offset",0));
    dispatchEvent(EVT_GET_HISTORY,&oData);
    //cocos2d::MessageBox("功能暂未上线，敬请期待！", "温馨提示");
}

void LobbyScene::onFriendClicked(cocos2d::Ref *pSender)
{
    log("点击好友按钮");
    m_pBackgroundBlack = Sprite::create(BLACK_COVER);
    m_pBackgroundBlack -> setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    Vec2 scale = WZQDeviceSize::getFullScreenScale(m_pBackgroundBlack);
    m_pBackgroundBlack -> setScale(scale.x,scale.y);
    m_pMenu -> setVisible(false);
    addChild(m_pBackgroundBlack);
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    ValueMap oData;
    oData.insert(make_pair("user_id",(int)usrinfo.nUserId));
    dispatchEvent(EVT_GET_FRIEND_LIST,&oData);
    auto pFriendList = FriendListLayer::create();
    addChild(pFriendList);
    //TO DO 1.展示好友列表界面
    //TO DO 2.updatafriend
}

void LobbyScene::buttonsBack(cocos2d::EventCustom *pEvent)
{
    log("恢复所有的按钮");
    m_pBackgroundBlack -> removeFromParent();
    m_pMenu -> setVisible(true);
}

void LobbyScene::onBeatingClicked(cocos2d::Ref *pSender)
{
    AppModel::getInstance() -> m_pInGameModel -> setMyUserInfo(AppModel::getInstance() -> m_pUserModel ->getMyUserInfo());
    log("点击双人对战按钮");
    m_pBackgroundBlack = Sprite::create(BLACK_COVER);
    m_pBackgroundBlack -> setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    Vec2 scale = WZQDeviceSize::getFullScreenScale(m_pBackgroundBlack);
    m_pBackgroundBlack -> setScale(scale.x,scale.y);
    m_pMenu -> setVisible(false);
    addChild(m_pBackgroundBlack);
    
    auto layer = BeatModeChooseLayer::create();
    
    addChild(layer);

}

void LobbyScene::onBotModeClicked(cocos2d::Ref *pSender)
{
    AppModel::getInstance() -> m_pInGameModel -> setMyUserInfo(AppModel::getInstance() -> m_pUserModel ->getMyUserInfo());
    log("点击人机模式按钮");
    m_pBackgroundBlack = Sprite::create(BLACK_COVER);
    m_pBackgroundBlack -> setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    Vec2 scale = WZQDeviceSize::getFullScreenScale(m_pBackgroundBlack);
    m_pBackgroundBlack -> setScale(scale.x,scale.y);
    m_pMenu -> setVisible(false);
    addChild(m_pBackgroundBlack);
    
    auto layer = BotModeChooseLayer::create();
    
    addChild(layer);
    
}

void LobbyScene::onSetupClicked(cocos2d::Ref *pSender)
{
    log("点击设置按钮");
    m_pBackgroundBlack = Sprite::create(BLACK_COVER);
    m_pBackgroundBlack -> setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    Vec2 scale = WZQDeviceSize::getFullScreenScale(m_pBackgroundBlack);
    m_pBackgroundBlack -> setScale(scale.x,scale.y);
    m_pMenu -> setVisible(false);
    addChild(m_pBackgroundBlack);
    
    auto setupLayer = SetupLayer::create();
    addChild(setupLayer);
}

void LobbyScene::ChangeHead(EventCustom *pEvent)
{
    //获取选择头像发来的信息
    cocos2d::Value* eventData = static_cast<cocos2d::Value*>(pEvent->getUserData());
    int num=eventData->asInt();
    auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
    // 修改 head 成员
    usrinfo.head = num; /* 新的头像值 */;
    printf("Model成员head已修改: %d \n",usrinfo.head);
    AppModel::getInstance()->m_pUserModel->setMyUserInfo(usrinfo);
}

void LobbyScene::putUserInfo(cocos2d::EventCustom *pEvent)
{
    m_pBackgroundBlack = Sprite::create(BLACK_COVER);
    m_pBackgroundBlack -> setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    Vec2 scale = WZQDeviceSize::getFullScreenScale(m_pBackgroundBlack);
    m_pBackgroundBlack -> setScale(scale.x,scale.y);
    m_pMenu -> setVisible(false);
    addChild(m_pBackgroundBlack);
    auto p_userInfoLayer = UserInfoLayer::create();
    addChild(p_userInfoLayer);
}

void LobbyScene::buttonBreath(float dt)
{
    float scale = m_pMenuItemBtnMatching -> m_scale;
    auto scaleUp = ScaleTo::create(0.9f, scale * 1.03f);
    auto scaleDown = ScaleTo::create(0.9f, scale);
    // 创建一个动画序列：放大后缩小
    auto sequence = Sequence::create(scaleUp, scaleDown, nullptr);
    m_pMenuItemBtnMatching -> runAction(sequence);
}

void LobbyScene::editRed(EventCustom *pEvent)
{
    //TODO 在大厅增加红点
    REQRequestFriend* req =static_cast<REQRequestFriend*>(pEvent->getUserData());
    printf("大厅好友按钮旁增加红点，显示人数为:%d",req ->num());
}

void LobbyScene::updateCoins(cocos2d::EventCustom *pEvent)
{
    std::string coin = "金币：";
    coin += std::to_string(AppModel::getInstance()->m_pUserModel->getMyUserInfo().coins);
    m_pCoinLabel->setString(coin);
}













