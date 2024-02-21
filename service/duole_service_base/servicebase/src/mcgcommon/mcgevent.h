
#ifndef  __EVENT_H_20131002__
#define __EVENT_H_20131002__

#include <pthread.h>
#include <semaphore.h>

//_____________________________________________________________________________________________________________
//事件常是定义
#define WAIT_OBJECT_0	 			0
#define WAIT_ABANDONED_0		0
#define INFINITE						0xFFFFFFFF
#define WAIT_TIMEOUT				0x00000102L
#define WAIT_FAILED					0xFFFFFFFF
#define BIT_EVTTYPE_EVT		0x01
#define BIT_EVTTYPE_SEM		0x02
//_____________________________________________________________________________________________________________
//事件结构定义
typedef struct _tagEVTHANDLE
{
    unsigned char		bType;
    union{
		sem_t	sem;
		struct{
			pthread_mutex_t	mtx;
			pthread_cond_t		cond;
		}evt;
    }obj;
    bool						manual_reset;
    bool						signaled;
}
EVTHANDLE, *HEVTHANDLE;

//_____________________________________________________________________________________________________________
//得到当前毫秒数
long long mcgGetTickCount();

//得到当前毫秒数(use CLOCK_MONOTONIC)
long long mcgGetTickCountUseMONOTONIC();
//_____________________________________________________________________________________________________________
//创建事件量
HEVTHANDLE mcgCreateEvent(bool bManualReset, bool bInitialState);
//置位事件量
void mcgSetEvent(HEVTHANDLE hEvent);
//复位事件量
void mcgResetEvent(HEVTHANDLE hEvent);

//_____________________________________________________________________________________________________________
//创建信号量
HEVTHANDLE mcgCreateSemaphore(unsigned int nInitVal=0);
//置位信号量
bool mcgReleaseSemaphore(HEVTHANDLE hEvent);

//_____________________________________________________________________________________________________________
//删除事件对象
void mcgCloseEvtHandle(HEVTHANDLE hEvent);

//_____________________________________________________________________________________________________________
//等待事件
int mcgWaitForSingleObject(HEVTHANDLE hEvent, unsigned int timeout);
//等待多个事件
int mcgWaitMultipleObjects(int nCount, HEVTHANDLE* pEvents, bool bWaitAll, unsigned int timeout);

//获取时间种子方法
unsigned int GenerateSeed();

#endif //__EVENT_H_20131002__
