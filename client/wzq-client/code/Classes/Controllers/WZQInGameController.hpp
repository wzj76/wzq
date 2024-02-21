//
//  WZQInGameController.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/30.
//

#ifndef WZQInGameController_hpp
#define WZQInGameController_hpp

#include "WZQController.h"
#include "Event/WZQEventWrapper.h"
#include "TimeTick/WZQTimeTick.h"

class WZQInGameController : public WZQController, public WZQEventWrapper, public WZQTimeTick
{
public:
    WZQInGameController();
    virtual void listEventNames();
    virtual void onEventData(const std::string& sName, void *pUserData);
private:
    //落子
    void doChess(void *pUserData);
    void doExitGame(void *pUserData);
    void doAckundo(void *pUserData);
    //聊天
    void doChat(void *pUserData);
    //继续游戏
    void doContinueGame(void *pUserData);
    void doSendAgreeContinueGame(void *pUserData);
};

#endif /* WZQInGameController_hpp */
