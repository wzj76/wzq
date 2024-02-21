#ifndef __DATA_TYPE_H__
#define __DATA_TYPE_H__

#include <map>
#include <deque>
#include <vector>
#include <string>
#include <set>
#include <string>
#include <algorithm>
#include "mcgutil/Tools.h"

//  常用判空定义；
#define IsNULL(p)        ((p) == NULL)
#define IsNotNULL(p)    ((p) != NULL)
#define IsZero(n)        ((n) == 0)
#define IsNotZero(n)    ((n) != 0)
#define IsGeZero(n)     ((n) >= 0)

//  通用数据类型定义；
typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef bool bit;
typedef unsigned long long ullong;
typedef long long llong;

typedef int32_t int32;
typedef int64_t int64;

#define DWORD	unsigned int
#define WORD	unsigned short
#define UINT	unsigned int
#define BOOL	int
#define BYTE	unsigned char
#define HANDLE	void*
#define LPVOID	void*
#define UCHAR 	unsigned char
#define FALSE	0
#define TRUE	1
#define LPBYTE	BYTE *
#define LPCBYTE	const BYTE *
#define LPCSTR	const char *
#define LPSTR	char *
#define ULONG	unsigned int
#define INT64	long
#define INT		int
#define CALLBACK
#define IN
#define OUT

//  通用数据结构定义；
//  Redis 配置
struct REDIS_CONFIG {
    std::string ip;     //  ip地址
    int port;           //  端口
    std::string pwd;    //  密码
    int tt_connect;     //  连接超时
    int tt_rw;          //  读写超时；
};

#endif // __DATA_TYPE_H__
