//
//  UserModel.cpp
//  GouJi
//
//  Created by 曾琪 on 14-9-26.
//
//

#include "WZQUserModel.h"
#include "Core/AppEvent.h"
#include "Util/Utils.h"
#include "AppDelegate.h"
#include "Util/ReaderWriter.h"
#include "WZQDeviceSize.hpp"
#include <stdlib.h>

USING_NS_CC;
using namespace std;
using namespace util;

#define KEY_LOGIN_NAME   "username"
#define KEY_LOGIN_PASSWORD   "userpassword"
#define KEY_BACKGROUND_MUSIC "background_music"
#define KEY_SOUND_EFFECT "sound_effect"
#define KEY_BUTTON_EFFECT "button_effect"
#define KEY_CHESS_EFFECT "chess_effect"
#define KEY_THEME_COLOR "theme_color"
#define GREEN_PATH "theme/GREEN/"
#define BLUE_PATH "theme/BLUE/"
#define YELLOW_PATH "theme/YELLOW/"
#define PURPLE_PATH "theme/PURPLE/"
#define RED_PATH "theme/RED/"

WZQUserModel::WZQUserModel()
:
WZQModel("WZQUserModel"),
m_nMyUserId(0),
m_oUsers({})
{
    m_pWaiting = WZQWaitingLayer::sharedInstance();
    m_pWaiting -> setPosition(WZQDeviceSize::getViewCenterPos());
    m_themeMap[green] = 1;
    m_themeMap[blue] = 2;
    m_themeMap[yellow] = 3;
    m_themeMap[red] = 4;
    m_themeMap[purple] = 5;
}

WZQUserModel::~WZQUserModel()
{
   
}

void WZQUserModel::init()
{
    m_sRecordLoginName = ReaderWriter::LoadStringFromXML(KEY_LOGIN_NAME, "");
    m_sRecordLoginPassword = ReaderWriter::LoadStringFromXML(KEY_LOGIN_PASSWORD, "");
    m_setupConf.background_music = ReaderWriter::LoadBooleanFromXML(KEY_BACKGROUND_MUSIC, true);
    m_setupConf.sound_effect = ReaderWriter::LoadBooleanFromXML(KEY_SOUND_EFFECT, true);
    m_setupConf.button_effect = ReaderWriter::LoadBooleanFromXML(KEY_BUTTON_EFFECT, true);
    m_setupConf.chess_effect = ReaderWriter::LoadBooleanFromXML(KEY_CHESS_EFFECT, true);
    m_setupConf.theme_color =
        ReaderWriter::LoadIntegerFromXML(KEY_THEME_COLOR, 1);
    switch (m_setupConf.theme_color) {
        case 1: m_nowTheme = GREEN_PATH;  break;
        case 2: m_nowTheme = BLUE_PATH;   break;
        case 3: m_nowTheme = YELLOW_PATH; break;
        case 4: m_nowTheme = RED_PATH;    break;
        case 5: m_nowTheme = PURPLE_PATH; break;
        default: CCASSERT(false, "无法获取主题信息！");  break;
    }
}

void WZQUserModel::reset()
{
    m_nMyUserId = 0;
    m_oUsers.clear();
}

void WZQUserModel::clear()
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
void WZQUserModel::updateUserInfo(const KKUSERINFO &oInfo)
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

void WZQUserModel::setMyUserInfo(const KKUSERINFO &oInfo)
{
    m_nMyUserId = oInfo.nUserId;
    updateUserInfo(oInfo);
//    dispatchEvent(NTF_MY_INFO_UPDATE);
}

KKUSERINFO WZQUserModel::getMyUserInfo()
{
    KKUSERINFO oMyInfo = {};
    auto oMapped = m_oUsers.find(m_nMyUserId);
    if (oMapped != m_oUsers.end())
    {
        oMyInfo = oMapped->second;
    }
    return oMyInfo;
}

KKUSERINFO WZQUserModel::getUserInfo(unsigned long long nUserId)
{
    KKUSERINFO oRet = {};
    map<unsigned long long, KKUSERINFO>::iterator iUser = m_oUsers.find(nUserId);
    if (iUser != m_oUsers.end())
    {
        oRet = iUser->second;
    }
    return oRet;
}

std::vector<KKUSERINFO> WZQUserModel::getRankingList() {
    return m_RankingList;
}
std::vector<HISTORYINFO> WZQUserModel::getHistoryList(){
    return m_HistoryList;
}
std::vector<KKUSERINFO> WZQUserModel::getFriendList(){
    return m_FriendList;
}
std::vector<KKUSERINFO> WZQUserModel::getWaitFriendList(){
    return m_WaitFriendList;
}
void WZQUserModel::setRecordUser(std::string sName,std::string sPassword)
{
    m_sRecordLoginName = sName;
    m_sRecordLoginPassword = sPassword;
    ReaderWriter::SaveStringToXML(KEY_LOGIN_NAME, m_sRecordLoginName);
    ReaderWriter::SaveStringToXML(KEY_LOGIN_PASSWORD, m_sRecordLoginPassword);
}

void WZQUserModel::getRecordUser(std::string& sName,std::string& sPassword)
{
    sName = m_sRecordLoginName;
    sPassword = m_sRecordLoginPassword;
}

void WZQUserModel::deleteRecordUser()
{
    m_sRecordLoginName="";
    m_sRecordLoginPassword="";
}

void WZQUserModel::setRankingList(std::vector<KKUSERINFO> rankingList) { 
    m_RankingList = rankingList;
}

void WZQUserModel::setHistory(std::vector<HISTORYINFO> historyList){
    m_HistoryList = historyList;
}

void WZQUserModel::resetHistory() { 
    m_HistoryList.clear();
}

void WZQUserModel::setOppUserInfo(const KKUSERINFO &oInfo)
{
    m_nOppUserId = oInfo.nUserId;
    updateUserInfo(oInfo);
//    dispatchEvent(NTF_MY_INFO_UPDATE);
}

void WZQUserModel::setFriendList(std::vector<KKUSERINFO> friendList){
    m_FriendList = friendList;
}

void WZQUserModel::setWaitFriendlist(std::vector<KKUSERINFO> waitfriendList){
    m_WaitFriendList = waitfriendList;
}

void WZQUserModel::setSetupConf(SETUPCONF &setups) {
    ReaderWriter::SaveBooleanToXML(KEY_BACKGROUND_MUSIC, setups.background_music);
    ReaderWriter::SaveBooleanToXML(KEY_SOUND_EFFECT, setups.sound_effect);
    ReaderWriter::SaveBooleanToXML(KEY_CHESS_EFFECT, setups.chess_effect);
    ReaderWriter::SaveBooleanToXML(KEY_BUTTON_EFFECT, setups.button_effect);
    ReaderWriter::SaveIntegerToXML(KEY_THEME_COLOR, setups.theme_color);
    std::cout<<"设置主题为"<<setups.theme_color<<std::endl;
    m_setupConf = setups;
}

std::string WZQUserModel::getThemePath()
{
    return m_nowTheme;
}


