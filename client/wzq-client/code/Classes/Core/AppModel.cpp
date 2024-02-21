//
//  AppModel.cpp
//  game
//
//  Created by Joi Yao on 16/10/12.
//
//

#include "AppModel.h"

USING_NS_CC;

static AppModel* s_pInstance = nullptr;

AppModel* AppModel::getInstance()
{
    if (!s_pInstance)
    {
        s_pInstance = new AppModel();
    }
    return s_pInstance;
}

void AppModel::initialize()
{
    if (m_bInitialized)
    {
        return;
    }
    // 初始化用户相关数据
    m_pUserModel = WZQUserModel::create();
    m_pInGameModel = WZQInGameModel::create();
    m_pBotGameModel = WZQBotGameModel::create();
    m_pShopModel = WZQShopModel::create();
    m_bInitialized = true;
}


AppModel::AppModel()
:
m_bInitialized(false),
m_pUserModel(nullptr),
m_pInGameModel(nullptr)
{
}

AppModel::~AppModel()
{
}
