//
//  main.cpp
//  authservice
//
//  Created by 刘挺 on 2022/11/24.
//
#include "AuthService.hpp"

CAuthService *g_pService=nullptr;

static void sig_svrd_close_hander(int no)
{
    g_pService ->OnSvrdClose() ;
}

int main(int argc, const char * argv[]) {
    if(SIG_ERR == signal(SIGPIPE, SIG_IGN))
    {
        printf("register SIGPIPE failed in main\n");
        return 0;
    }
    
    if(SIG_ERR == signal(SIGUSR1, sig_svrd_close_hander))
    {
        printf("register SIGHUP failed int main\n");
        return 0 ;
    }
    srandom(time(nullptr));
    CAuthService *pService = new CAuthService();
    if (NULL == pService)
    {
        printf("New CUserHandInfoService failed in main\n");
        return 0;
    }
    g_pService = pService;

    bool bRet = false;
    printf("Start service in main()\n");
    bRet = pService->StartService();
    if (!bRet)
    {
        printf("Call StartService failed in main");
    }
    delete pService;
    printf("Stop service in main\n");
    return 0;
}
