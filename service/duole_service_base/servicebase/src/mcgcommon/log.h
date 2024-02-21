#ifndef __LOG_H__
#define __LOG_H__

#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/ndc.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include "ILog.h"
using namespace log4cplus;
using namespace log4cplus::helpers;

//初始化log
bool mcgInitLog(const char* pszServiceName, const char *pszPath=NULL);

//初始化记录消息处理时间日志
bool OnMsgTimeInitLog(const char* pszServiceName, const char *pszPath=NULL);
void OnMsgTimeWriteLog(const char* pszFmt,...);

//功能说明：是描述性的日志，主要是为了测试说明或正常的提示说明而用。
//			往一般log文件中写字符串，行首添加date time ThreadID，行尾添加回车换行。
void mcgWriteLog(const char* pszFmt,...);

/////////////////////////////
bool mcgInitChipLog(const char* pszServiceName, const char *pszPath=NULL);
bool mcgInitHandLog(const char* pszServiceName, const char *pszPath=NULL);
void mcgWriteChipLog(const char* pszFmt,...);
void mcgWriteHandLog(const char* pszFmt,...);

class CLog:public ILog
{
public:
	CLog();
	virtual ~CLog();
public:
	bool Init(const char* pszServiceName, const char *pszPath,int nLevel);
	int SetLevel(int nLevel);
	int GetLevel();
	int UpLevel();
	int DownLevel();
	virtual int Trace(const char* pszFmt,...);
	virtual int Debug(const char* pszFmt,...);
	virtual int Info(const char* pszFmt,...);
	virtual int Warning(const char* pszFmt,...);
	virtual int Error(const char* pszFmt,...);
	virtual int Fatal(const char* pszFmt,...);
private:
	Logger m_logger;
};

#endif //__LOG_H__
