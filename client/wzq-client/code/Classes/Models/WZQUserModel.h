//
//  UserModel.h
//  GouJi
//
//  Created by 曾琪 on 14-9-26.
//
//

#pragma once

#include "WZQModel.h"
#include "Core/AppDef.h"
#include "WZQWaitingLayer.h"
#include <map>
#include <vector>

class WZQUserModel : public WZQModel
{
public:
    CREATE_MODEL(WZQUserModel);
    WZQUserModel();
    ~WZQUserModel();
    
    void init();
    virtual void reset() override;
    // 移除被踢玩家
    void removeKickUser(const unsigned long long& nUserId);
    // 清除其他玩家数据
    void clear();
    // 清除玩家是否刷新头像
    void removeUpdateGame();
    //------------------玩家信息--------------------------
    // 更新玩家信息（如果没有存在，插入map中）
    void updateUserInfo(const KKUSERINFO& oInfo);
    // 设置自己的信息
    void setMyUserInfo(const KKUSERINFO& oInfo);
    // 获得自己的信息
    KKUSERINFO getMyUserInfo();
    // 获得玩家信息
    KKUSERINFO getUserInfo(unsigned long long nUserId);
    // 获得排行榜信息
    std::vector<KKUSERINFO> getRankingList();
    // 设置排行榜信息
    void setRankingList(std::vector<KKUSERINFO> rankingList);
    
    // 记录登录账号
    void setRecordUser(std::string sName,std::string sPassword);
    void getRecordUser(std::string& sName,std::string& sPassword);
    void deleteRecordUser();
    
    //获取历史记录
    void setHistory(std::vector<HISTORYINFO> historyList);
    std::vector<HISTORYINFO> getHistoryList();
    void resetHistory();
    
    void setOppUserInfo(const KKUSERINFO &oInfo);
    
    //获取好友列表
    std::vector<KKUSERINFO> getFriendList();
    //设置好友列表信息
    void setFriendList(std::vector<KKUSERINFO> friendList);
    //设置待同意好友列表
    void setWaitFriendlist(std::vector<KKUSERINFO> waitfriendList);
    //获取待同意好友列表
    std::vector<KKUSERINFO> getWaitFriendList();
    //设置
    void setSetupConf(SETUPCONF &setups);
    //获取搜索朋友的信息
    KKUSERINFO getFriendinSearch();
    //获取当前主题的路径
    std::string getThemePath();
    
public:
    CC_SYNTHESIZE(unsigned long long,   m_nMyUserId,      MyUserId);
    unsigned long long   m_nOppUserId;
    std::map<unsigned long long, KKUSERINFO>   m_oUsers;
    WZQWaitingLayer* m_pWaiting;
    std::vector<HISTORYINFO> m_HistoryList;
    SETUPCONF m_setupConf;
    KKUSERINFO m_friendInfo;
    
    int m_historyId;
    std::string m_frined_name;
    std::map<THEMECONF, unsigned> m_themeMap;
    
private:
    std::string     m_sRecordLoginName;
    std::string     m_sRecordLoginPassword;
    std::vector<KKUSERINFO> m_RankingList;
    std::vector<KKUSERINFO> m_FriendList;
    std::vector<KKUSERINFO> m_WaitFriendList;
    std::string m_nowTheme;
    
};
