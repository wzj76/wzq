//
//  WZQLobbyController.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/30.
//

#ifndef WZQLobbyController_hpp
#define WZQLobbyController_hpp

#include "WZQController.h"
#include "Event/WZQEventWrapper.h"
#include "TimeTick/WZQTimeTick.h"

class WZQLobbyController : public WZQController, public WZQEventWrapper, public WZQTimeTick
{
public:
    WZQLobbyController();
    virtual void listEventNames();
    virtual void onEventData(const std::string& sName, void *pUserData);
private:
    // 获取排行榜
    void doGetRandingList(void *pUserData);
    // 匹配
    void doMatch(std::string message);
    // 编辑头像
    void doEdit(int headnum);
    void doEditName(std::string username);
    // 获取用户信息
    void doGetUserInfo(void *pUserData);
    //获取历史记录
    void doGetGameHistory(void *pUserData);
    //请求退出
    void doExitAccount(void *pUserData);
    //请求购买
    void doBuyItems(void *pUserData);
};

#endif /* WZQLobbyController_hpp */
