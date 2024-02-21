//
//  WZQInGameModel.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/29.
//

#include "WZQInGameModel.hpp"
#include "Core/AppEvent.h"
#include "Util/Utils.h"
#include "AppDelegate.h"
#include "Util/ReaderWriter.h"
#include "WZQDeviceSize.hpp"
#include "BaseInGameScene.hpp"
#include "MatchingScene.hpp"
#include "BeatingModeScene.hpp"
#include "BotModeScene.hpp"

USING_NS_CC;

WZQInGameModel::WZQInGameModel()
:
WZQModel("WZQInGameModel"),
m_nMyUserId(0),
m_oUsers({})
{
    m_pWaiting = WZQWaitingLayer::sharedInstance();
    m_pWaiting -> setPosition(WZQDeviceSize::getViewCenterPos());
}

WZQInGameModel::~WZQInGameModel()
{
   
}

void WZQInGameModel::init()
{
}

void WZQInGameModel::reset()
{
    m_nMyUserId = 0;
    m_oUsers.clear();
}

void WZQInGameModel::clear()
{
    for (map<unsigned long long, KKUSERINFO>::iterator i = m_oUsers.begin(); i != m_oUsers.end();)
    {
        if (i->first != m_nMyUserId)
        {
           i = m_oUsers.erase(i);
        }
        else
        {
            i++;
        }
    }
}
void WZQInGameModel::updateUserInfo(const KKUSERINFO &oInfo)
{
    if (oInfo.nUserId == 0)
    {
        log("UserModel::update >>> invalid userid.");
        return;
    }
    map<unsigned long long, KKUSERINFO>::iterator iUser = m_oUsers.find(oInfo.nUserId);
    if (iUser != m_oUsers.end())
    {
        m_oUsers.at(oInfo.nUserId) = oInfo;
    }
    else
    {
        m_oUsers.insert(make_pair(oInfo.nUserId, oInfo));
    }
    if (oInfo.nUserId == m_nMyUserId)
    {
//        dispatchEvent(NTF_MY_INFO_UPDATE);
    }
    else
    {
        Value oData = Value((int)oInfo.nUserId);
//        dispatchEvent(NTF_INFO_UPDATE, &oData);
    }
}

void WZQInGameModel::setMyUserInfo(const KKUSERINFO &oInfo)
{
    m_nMyUserId = oInfo.nUserId;
    updateUserInfo(oInfo);
    m_gameConf.oMyInfo = oInfo;
}

void WZQInGameModel::setOppoUserInfo(const KKUSERINFO& oInfo)
{
    m_nOppoUserId = oInfo.nUserId;
    updateUserInfo(oInfo);
    m_gameConf.oOppoInfo = oInfo;
}

KKUSERINFO WZQInGameModel::getMyUserInfo()
{
    KKUSERINFO oMyInfo = {};
    auto oMapped = m_oUsers.find(m_nMyUserId);
    if (oMapped != m_oUsers.end())
    {
        oMyInfo = oMapped->second;
    }
    return oMyInfo;
}

KKUSERINFO WZQInGameModel::getUserInfo(unsigned long long nUserId)
{
    KKUSERINFO oRet = {};
    map<unsigned long long, KKUSERINFO>::iterator iUser = m_oUsers.find(nUserId);
    if (iUser != m_oUsers.end())
    {
        oRet = iUser->second;
    }
    return oRet;
}

KKUSERINFO WZQInGameModel::getOppoUserInfo()
{
    KKUSERINFO oMyInfo = {};
    auto oMapped = m_oUsers.find(m_nOppoUserId);
    if (oMapped != m_oUsers.end())
    {
        oMyInfo = oMapped->second;
    }
    return oMyInfo;
}

void WZQInGameModel::setGameMode(GAMEMODE gameMode)
{
    switch (gameMode) {
        case MatchingMode:
            // TODO: 将匹配模式的游戏配置移动到这里。
            break;
            
        case BeatingMode:
            {
                m_gameConf.gamemode = BeatingMode;
                
                // TODO: 设置多种等待时间
                m_gameConf.iWaitingTime = 20;
                
                KKUSERINFO oOppoInfo = getMyUserInfo();
                oOppoInfo.sUser = "玩家1";
                setMyUserInfo(oOppoInfo);
                oOppoInfo.nUserId = 0;
                oOppoInfo.sUser = "玩家2";
                setOppoUserInfo(oOppoInfo);
                m_gameConf.iRoomId = 0;
                m_gameConf.llBlackId = getMyUserId();
                m_gameConf.llWhiteId = 0;
                m_gameConf.llActionId = getMyUserId();
                m_IsMe = true;
                
                //auto inGameScene = BaseInGameScene::createScene();
                auto inGameScene = BeatingModeScene::createScene();
                Director::getInstance() -> replaceScene(inGameScene);
            }
            break;
        case BotMode:
        {
            m_gameConf.gamemode = BotMode;
            
            // TODO: 设置多种等待时间
            m_gameConf.iWaitingTime = 20;
            
            KKUSERINFO oOppoInfo = getMyUserInfo();
            setMyUserInfo(oOppoInfo);
            oOppoInfo.nUserId = 0;
            oOppoInfo.sUser = "机器人";
            setOppoUserInfo(oOppoInfo);
            m_gameConf.iRoomId = 0;

            //m_gameConf.llActionId = getMyUserId();
//            m_IsMe = true;
            
            //auto inGameScene = BaseInGameScene::createScene();
            auto inGameScene = BotModeScene::createScene();
            Director::getInstance() -> replaceScene(inGameScene);
        }
            break;
        default:
            break;
    }
}





