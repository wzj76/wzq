//
//  ControllerManager.cpp
//  GouJi
//
//  Created by 曾琪 on 14-9-26.
//
//

#include "WZQControllerManager.h"
#include "Core/AppEvent.h"
#include "WZQLoginController.h"
#include "WZQLobbyController.hpp"
#include "WZQInGameController.hpp"
#include "WZQFriendController.hpp"

USING_NS_CC;
using namespace std;

WZQControllerManager::WZQControllerManager()
{
}

WZQControllerManager::~WZQControllerManager()
{
    m_obControllerMap.clear();
}

void WZQControllerManager::initialize()
{
    // 账号业务
    registerController(new WZQLoginController());
    
    // 大厅业务
    registerController(new WZQLobbyController());
    
    // 游戏中业务
    registerController(new WZQInGameController());
    
    //好友业务
    registerController(new WZQFriendController());
}

void WZQControllerManager::registerController(WZQController *pController)
{
    if (nullptr == pController)
    {
        return;
    }
    pController->listEventNames();
    vector<string> oEventNames = pController->getEventNames();
    for (size_t i = 0; i < oEventNames.size(); i ++)
    {
        mapController(oEventNames[i], pController);
    }
}

void WZQControllerManager::mapController(const string &sEventName, WZQController *pController)
{
    pair<std::string, WZQController*> pairEvent;
    pairEvent.first = sEventName;
    pairEvent.second = pController;
    m_obControllerMap.push_back(pairEvent);
    addEventListener(sEventName, CC_CALLBACK_1(WZQControllerManager::onEventReceived, this));
}

void WZQControllerManager::onEventReceived(EventCustom *pEvent)
{
    for (auto pEventControler : m_obControllerMap)
    {
        auto sEventName = pEvent->getEventName();
        auto pController = pEventControler.second;
        if (pController != nullptr && pEventControler.first == sEventName)
        {
            pController->execute(sEventName, static_cast<ValueMap *>(pEvent->getUserData()));
        }
    }
}
