//
//  ControllerManager.h
//  GouJi
//
//  Created by 曾琪 on 14-9-26.
//
//

#ifndef GouJi_ControllerManager_h
#define GouJi_ControllerManager_h

#include "cocos2d.h"
#include "WZQController.h"
#include "Event/WZQEventWrapper.h"

class WZQControllerManager : public WZQEventWrapper
{
public:
    WZQControllerManager();
    ~WZQControllerManager();
    
    void initialize();
    
private:
    void registerController(WZQController* pController);
    void mapController(const std::string &sEventName, WZQController *pController);
    
private:
    void onEventReceived(cocos2d::EventCustom *pEvent);
    
private:
    std::vector<std::pair<std::string, WZQController*>> m_obControllerMap;
    
};

#endif
