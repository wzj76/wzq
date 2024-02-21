//
//  Controller.h
//  GouJi
//
//  Created by 曾琪 on 14-9-26.
//
//

#ifndef GouJi_Controller_h
#define GouJi_Controller_h

#include "cocos2d.h"

class WZQController : public cocos2d::Ref
{
public:
    WZQController(const std::string &sName);
    virtual ~WZQController();
    virtual void listEventNames() = 0;
    virtual void execute(const std::string &sName, void *pUserData = nullptr);
    virtual void onEventData(const std::string& sName, void *pUserData) = 0;
    std::string getValue(const cocos2d::ValueMap& obOption, const std::string& sKey);
    
    CC_SYNTHESIZE_READONLY(std::vector<std::string>, m_oEventNames, EventNames);
    
private:
    
protected:
    std::string     m_sName;
};

#endif
