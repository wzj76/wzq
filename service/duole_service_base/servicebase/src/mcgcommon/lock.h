
#ifndef  __LOCK_H_20131002__
#define __LOCK_H_20131002__

#include <pthread.h>



typedef pthread_mutex_t CRITICAL_SECTION;
typedef pthread_mutex_t* PCRITICAL_SECTION;

//初始化临界区
void mcgInitializeCriticalSection(PCRITICAL_SECTION pcs);

//删除临界区
void mcgDeleteCriticalSection(PCRITICAL_SECTION pcs);

//进入临界区
void mcgEnterCriticalSection(PCRITICAL_SECTION pcs);

//尝试进入临界区，如果成功返回true，否则返回false
bool mcgTryEnterCriticalSection(PCRITICAL_SECTION pcs);

//退出临界区
void mcgLeaveCriticalSection(PCRITICAL_SECTION pcs);



#endif //__LOCK_H_20131002__
