#ifndef __GLOBALFUNC_H__
#define __GLOBALFUNC_H__

#define MAX_PATH		512		// 全路径长度

#include "datatype.h"
#include <string>

char* GetLocalStringIP();
void LongIP2StringIP(DWORD i_dwIP, char *o_strIP);	// long IP转换成string IP(xxx.xxx.xxx.xxx)
DWORD StringIP2LongIP(char *i_strIP);				// string IP(xxx.xxx.xxx.xxx)转换成long IP

void GetCurrentDir(char *o_szCurrentDir);
void GetFullPath(char *o_szFullPath);
void GetAppName(char *o_szAppName);
std::string GenGUID();									//生成GUID
std::string GetPWD();
#endif // __GLOBALFUNC_H__
