#include "ConnService.h"
#include "log.h"
#include <stdio.h>

CConnService *g_pService=NULL;

static void sig_svrd_close_hander(int no)
{
    g_pService ->OnSvrdClose() ;
    mcgWriteLog("step into sig_svrd_close_handler ");
}

int main(int argc, char **argv)
{
    if(SIG_ERR == signal(SIGPIPE, SIG_IGN))
    {
        printf("register SIGPIPE failed in main\n");
        return 0;
    }
    
    if(SIG_ERR == signal(SIGUSR2, sig_svrd_close_hander))
    {
        printf("register SIGHUP failed int main\n");
        return 0 ;
    }
    
	CConnService *pService = new CConnService();
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

