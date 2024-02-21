//
//  AppModel.h
//  game
//
//  Created by Joi Yao on 16/10/12.
//
//

#pragma once

#include "Models/WZQUserModel.h"
#include "Models/WZQInGameModel.hpp"
#include "Models/WZQBotGameModel.hpp"
#include "Models/WZQShopModel.hpp"

class AppModel : public cocos2d::Ref
{
public:
    static AppModel* getInstance();
    void initialize();
    
    CC_SYNTHESIZE(bool, m_bAppActive, AppActive);
private:
    AppModel();
    ~AppModel();
    
public:
    WZQUserModel*          m_pUserModel;
    WZQInGameModel*        m_pInGameModel;
    WZQBotGameModel*       m_pBotGameModel;
    WZQShopModel*          m_pShopModel;
private:
    bool        m_bInitialized;
};
