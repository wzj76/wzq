#ifndef __CHECKSUM_H__
#define __CHECKSUM_H__

#include "datatype.h"

//按DWORD求字符串和，忽略大小写，若最后不足字节，则高位按处理
//
//   s                  以结尾的字符串
DWORD mcgGetCheckSum(const char *s);

//按DWORD求内存和，若最后不足字节，则高位按处理
//
//   pBuf          缓冲区地址
//   len                缓冲区长度
DWORD mcgGetCheckSum(const void *pBuf,int len);

//按DWORD求字符串和，大小写敏感，若最后不足字节，则高位按处理
//
//   s                  以结尾的字符串
DWORD mcgGetCheckSumCaseOn(const char *s);

//按WORD求字符串和，忽略大小写，若最后不足字节，则高位按处理
//
//   s                  以结尾的字符串
WORD mcgGetWordCheckSum(const char *s);

#endif // __CHECKSUM_H__
