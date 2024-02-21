//
//  LoginController.h
//  PaoDeKuaiPQ
//
//  Created by 曾琪 on 14-9-28.
//
//

#pragma once

#include "WZQController.h"
#include "Event/WZQEventWrapper.h"
#include "TimeTick/WZQTimeTick.h"

class WZQLoginController : public WZQController, public WZQEventWrapper, public WZQTimeTick
{
public:
    WZQLoginController();
    virtual void listEventNames();
    virtual void onEventData(const std::string& sName, void *pUserData);
private:
    // 注册
    void doRegistAccount(const std::string &sName, const std::string &sPassWord);
    // 登录
    void doLogin(const std::string &sName, const std::string &sPassWord);

};
