//
//  WZQInGameModel.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/29.
//

#ifndef WZQInGameModel_hpp
#define WZQInGameModel_hpp

#include <stdio.h>

#include "WZQModel.h"
#include "Core/AppDef.h"
#include "WZQWaitingLayer.h"
#include <vector>

class WZQInGameModel : public WZQModel
{
public:
    CREATE_MODEL(WZQInGameModel);
    WZQInGameModel();
    ~WZQInGameModel();
    
    void init();
    virtual void reset() override;
    // 清除其他玩家数据
    void clear();
    //------------------玩家信息--------------------------
    // 更新玩家信息（如果没有存在，插入map中）
    void updateUserInfo(const KKUSERINFO& oInfo);
    // 设置自己的信息
    void setMyUserInfo(const KKUSERINFO& oInfo);
    // 设置对手的信息
    void setOppoUserInfo(const KKUSERINFO& oInfo);
    // 获得自己的信息
    KKUSERINFO getMyUserInfo();
    // 获得对手的信息
    KKUSERINFO getOppoUserInfo();
    // 获得玩家信息
    KKUSERINFO getUserInfo(unsigned long long nUserId);
    
    // 设置游戏模式
    void setGameMode(GAMEMODE gameMode);
    
public:
    CC_SYNTHESIZE(unsigned long long,   m_nMyUserId,      MyUserId);
    std::map<unsigned long long, KKUSERINFO>   m_oUsers;
    WZQWaitingLayer* m_pWaiting;
//    unsigned long long m_nBlackId;
//    unsigned long long m_nWhiteId;
//    int m_nRoomId;
    bool m_IsMe;
    bool m_IsMeWin;
    INGAMECONF m_gameConf;

    int m_chessNum;
    
    bool m_IsGaming = false ; //是否正在游戏
    
    int m_score = 100; //游戏分数
    int m_coins = 200; //游戏金币
    
private:
    float m_fRemainTime;
    unsigned long long m_nOppoUserId;



};

#endif /* WZQInGameModel_hpp */
