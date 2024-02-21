
#ifndef __FIXTIMER_H_20131002__
#define __FIXTIMER_H_20131002__

#include "datatype.h"
#include "ptrlist.h"

#define MAX_DEFAULT_EVENTID 4294967295



typedef void* (* PTHREAD_START_ROUTINE)(LPVOID lpThreadParameter, DWORD dwEventID);
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;
 
class CMcgTimer 
{
private:
	//构造函数
	CMcgTimer();

public:
	//析构函数
	~CMcgTimer();				

public:

	static HANDLE CreateTimer(LPTHREAD_START_ROUTINE pcbf, void* pContext, DWORD DueTime, DWORD Period, DWORD dwEventID=MAX_DEFAULT_EVENTID);


	static int DeleteTimer(HANDLE hTimer, bool bForce);	


	static bool ChangeTimer(HANDLE hTimer, LPTHREAD_START_ROUTINE pcbf, void* pContext, DWORD DueTime, DWORD Period);

	
	static DWORD GetCount();

	
	static void DeleteAllTimer();

	
	static void TimerFunc();
	
private:	
	static CPtrlist	m_listTimer;					
};

#endif //__FIXTIMER_H_20131002__


