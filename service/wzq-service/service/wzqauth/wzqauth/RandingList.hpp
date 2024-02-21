//
//  RandingList.hpp
//  wzqauth
//
//  Created by duole on 2024/2/19.
//

#ifndef RandingList_hpp
#define RandingList_hpp

#include <stdio.h>
#include <vector>
#include "wzqProtocol.pb.h"
#include "mcgtimer.h"
#include "AuthService.hpp"
#include <mutex>
class CAuthService;
class RandingList
{
public:
    RandingList(CAuthService* authService);
    ~RandingList();
    
    void StartRandingListTimer();
    static void* AutoGetRandingList(void* ptr,unsigned int a);
    static void* AutoClearHistory(void* ptr,unsigned int a);
    void SetRandingList(int limit,int offset,std::vector<UserInfo>&out_userinfos);
    CAuthService* m_pAuthService;
    std::vector<UserInfo> m_randingList;
    HANDLE m_randingListTimer;
    HANDLE m_clearHistoryTimer;
    std::mutex m_mutex;
};

#endif /* RandingList_hpp */
