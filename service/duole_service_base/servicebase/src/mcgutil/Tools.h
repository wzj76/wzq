#ifndef __TOOLS_h__
#define __TOOLS_h__

#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <map>

extern std::string format(const char* szFormat,...);
extern int str2int(const std::string& str);
extern std::string int2str(int n);
extern std::string uint2str(size_t n);
extern long long str2longlong(const std::string& str);
extern std::string longlong2str(long long n);
extern float str2float(const std::string& str) ;
extern std::string float2str(float n);
extern std::string map2jsonstr(std::map<std::string, std::string>& map_info);

template<typename T>
std::string integer2str(T n)
{
    std::stringstream strStream;
	strStream << n;
	return strStream.str();
}

std::string bool2str(bool val);
int parseVersion(std::string clientVer);
// 格式化输出当前时间
std::string strfcurrtime(const std::string& format , time_t timestamp);
// 格式化输出年月日(2016_8_2)
std::string strymdCurrtime();
// 通过uuid库获得唯一id
std::string getuniqueid();
// base64编码和解码
bool Base64Encode(const std::string& input, std::string& output);
bool Base64Decode(const std::string& input, std::string& output);

std::string Base64Encode(const std::string& input);
std::string Base64Decode(const std::string& input);
std::string getCalculateMoney(unsigned long long nMoney);
std::string str_replace_all(const std::string& src , const std::string& oldval , const std::string& newval);

//md5 加密
std::string md5(const std::string& src);

//格式化json 字符串
std::string strFormatJson(const std::map<std::string , std::string>& values);

int CorrectTo2DecimalPlaces(int dividend, int divisor);
#endif
