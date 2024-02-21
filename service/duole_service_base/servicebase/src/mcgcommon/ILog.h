#ifndef __LOG_INTERFACE_H__
#define __LOG_INTERFACE_H__

enum LOG_LEVEL
{
    /** \var const LogLevel OFF_LOG_LEVEL
     * The <code>OFF_LOG_LEVEL</code> LogLevel is used during configuration to
     * turn off logging. */
    LOG_LEVEL_OFF     = 60000,

    /** \var const LogLevel FATAL_LOG_LEVEL
     * The <code>FATAL_LOG_LEVEL</code> LogLevel designates very severe error
     * events that will presumably lead the application to abort.  */
    LOG_LEVEL_FATAL   = 50000,

    /** \var const LogLevel ERROR_LOG_LEVEL
     * The <code>ERROR_LOG_LEVEL</code> LogLevel designates error events that
     * might still allow the application to continue running. */
    LOG_LEVEL_ERROR   = 40000,

    /** \var const LogLevel WARN_LOG_LEVEL
     * The <code>WARN_LOG_LEVEL</code> LogLevel designates potentially harmful 
     * situations. */
    LOG_LEVEL_WARN    = 30000,

    /** \var const LogLevel INFO_LOG_LEVEL
     * The <code>INFO_LOG_LEVEL</code> LogLevel designates informational 
     * messages  that highlight the progress of the application at 
     * coarse-grained  level. */
    LOG_LEVEL_INFO    = 20000,

    /** \var const LogLevel DEBUG_LOG_LEVEL
     * The <code>DEBUG_LOG_LEVEL</code> LogLevel designates fine-grained
     * informational events that are most useful to debug an application. */
    LOG_LEVEL_DEBUGL   = 10000,

    /** \var const LogLevel TRACE_LOG_LEVEL
     * The <code>TRACE_LOG_LEVEL</code> LogLevel is used to "trace" entry
     * and exiting of methods. */
    LOG_LEVEL_TRACE   = 0,
    
    /** \var const LogLevel ALL_LOG_LEVEL
     * The <code>ALL_LOG_LEVEL</code> LogLevel is used during configuration to
     * turn on all logging. */
    LOG_LEVEL_ALL    = LOG_LEVEL_TRACE,

    /** \var const LogLevel NOT_SET_LOG_LEVEL
     * The <code>NOT_SET_LOG_LEVEL</code> LogLevel is used to indicated that
     * no particular LogLevel is desired and that the default should be used.
     */
    LOG_LEVEL_NOT_SET = -1,
};

class ILog
{
public:
	virtual ~ILog(){};
	virtual int SetLevel(int nLevel) = 0;
	virtual int Trace(const char* pszFmt,...) = 0;
	virtual int Debug(const char* pszFmt,...) = 0;
	virtual int Info(const char* pszFmt,...) = 0;
	virtual int Warning(const char* pszFmt,...) = 0;
	virtual int Error(const char* pszFmt,...) = 0;
	virtual int Fatal(const char* pszFmt,...) = 0;
};

#endif //__LOG_INTERFACE_H__
