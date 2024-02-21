//
//  WZQBotGameModel.hpp
//  wzqgame-mobile
//
//  Created by duole on 2024/2/7.
//

#ifndef WZQBotGameModel_hpp
#define WZQBotGameModel_hpp

#include <stdio.h>
#include "WZQModel.h"
#include "Core/AppDef.h"
#include "WZQWaitingLayer.h"
#include <vector>

class WZQBotGameModel : public WZQModel
{
public:
    CREATE_MODEL(WZQBotGameModel);
    WZQBotGameModel();
    ~WZQBotGameModel();
    
    void init();
    virtual void reset() override;
    
    
public:
    //游戏模式，简单，中等，困难
    std::string m_modeChoosed;
    //玩家的棋子颜色，黑棋或者白棋
    std::string m_sideChoosed;
    //玩家的id，默认是1
    int m_userid = 1;
    //机器人id，默认是-1
    int m_robotid = -1;
    //目前该下棋的id，1或者-1，默认是0
    int m_curid = 0;
    int m_balck_id = 0;
    int m_white_id = 0;
    
    //玩家下棋数量
    int m_put_num = 0;
    //是否轮到玩家下棋
    bool m_is_user = false;


};
#endif /* WZQBotGameModel_hpp */
