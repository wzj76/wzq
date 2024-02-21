#pragma once

#include <string>
#include <pthread.h>
#include <vector>

typedef pthread_mutex_t		CRITICAL_SECTION;
typedef pthread_mutex_t*	PCRITICAL_SECTION;

// 过滤特殊字符
bool HasSpecialChars(std::string strText);
// 验证邮箱格式
bool IsValidEmail(std::string strEmail);
// 验证密码复杂性
bool IsComplexPassword(std::string strText);
// 是否是纯粹的数字和字母
bool IsNumberAndChars(std::string strText);
bool IsNumbers(std::string strText);
// 是否有匹配上的
bool IsAnyMatch(std::string strText,std::vector<std::string> &keys);
// 字符为字母
bool IsInAlphabet(const char ch);
// 字符为数字
bool IsNumeric(const char ch);
// 获取字符串的字符个数
int GetStringLength(std::string strText);

// 输出带豆号的数字串
std::string strCommaLL(long long llNum);
std::string strCommaULL(unsigned long long ullNum);

// 输出把千用K表示的数字串
std::string strKULL(unsigned long long ullNum);     // 一千以上用K表示
std::string strKULL2(unsigned long long ullNum);    // 一万以上用K表示

// 简单加解密
std::string Encrypt(std::string strSrc);
std::string Decrypt(std::string strSrc);

void InitializeCriticalSection(PCRITICAL_SECTION pcs);
void DeleteCriticalSection(PCRITICAL_SECTION pcs);
void EnterCriticalSection(PCRITICAL_SECTION pcs);
bool TryEnterCriticalSection(PCRITICAL_SECTION pcs);
void LeaveCriticalSection(PCRITICAL_SECTION pcs);

// 获取简称
std::string GetShort(std::string sOrgin);
// 身份证校验
bool IsValidId(std::string sId);
// 中文
bool IsInChinese(std::string sText);
// 字符串按照指定的宽度拆分
std::vector<std::string> getCertainWidthTexts(const std::string& sText, const int& nMaxWidth, const int& nFontSize);
// 字符串按照指定的字符切割
std::vector<std::string> split(std::string str,std::string pattern);
// 获取指定个数中文全角加省略号的字符串
std::string getCertainWidthOmitText(const std::string& sText, const int& nFontSize, const int& nNums);
// 是否是base64数据
bool is_base64(unsigned char c);
// 解析base64数据
std::string base64_decode(std::string const& encoded_string);

bool isTextSendCorrect(std::string text);
