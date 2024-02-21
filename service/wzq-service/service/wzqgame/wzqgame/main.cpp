#include <stdio.h>
#include <unistd.h>
#include "GameService.h"
#include "log.h"
#include "mcgevent.h"

extern int total ;
//
CGameService* g_pService = NULL;

static void sig_svrd_close_handler(int no)
{
    g_pService ->OnSvrdClose() ;
}

int main(int argc, char **argv)
{
    for(int i = 0 ; i < 10 ; i ++)
    {
        printf("seed : %ld \n" , GenerateSeed());
    }
    srandom(time(nullptr));
#ifdef __DAEMON__
	if (IsNotZero(daemon(1,1)))
	{
		printf("call daemon failed in main\n");
		return 0;
	}
#endif
    if(SIG_ERR == signal(SIGPIPE, SIG_IGN))
    {
        printf("register SIGPIPE failed in main\n");
		return 0;
    }
    
	if (SIG_ERR == signal(SIGUSR1, sig_svrd_close_handler))
	{
		printf("register SIGUSR1 failed in main\n");
		return 0;
	}
	CGameService *pService = new CGameService;
	if (IsNULL(pService))
	{
		printf("New CGameService failed in main\n");
		return 0;
	}
	g_pService = pService;

	printf("Start service in main\n");
	if (!pService->StartService())
	{
		printf("Call StartService failed in main");
	}
	delete pService;
	printf("Stop service in main\n");
	return 0;
}


