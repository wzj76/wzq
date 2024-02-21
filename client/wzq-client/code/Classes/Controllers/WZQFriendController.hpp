//
//  WZQFriendController.hpp
//  wzqgame-mobile
//
//  Created by duole on 2024/2/4.
//

#ifndef WZQFriendController_hpp
#define WZQFriendController_hpp

#include <stdio.h>
#include "WZQController.h"
#include "Event/WZQEventWrapper.h"
#include "TimeTick/WZQTimeTick.h"

class WZQFriendController : public WZQController, public WZQEventWrapper, public WZQTimeTick
{
public:
    WZQFriendController();
    virtual void listEventNames();
    virtual void onEventData(const std::string& sName, void *pUserData);
private:
    //请求好友列表
    void doGetFriendList(void *pUserData);
    void doSearchFriend(void *pUserData);
    void doAddFriend(void *pUserData);
    void doGetWaitFriendList(void *pUserData);
    void doIsAgreeFriend(void *pUserData);
    void doDeleteFriend(void *pUserData);
    void doInviteFriendGame(void *pUserData);
    void doSendAckInviteFriendGame(void *pUserData);
};
#endif /* WZQFriendController_hpp */
