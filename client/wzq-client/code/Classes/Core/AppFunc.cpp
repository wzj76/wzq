#include "AppFunc.h"
#include "cocos2d.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex>
#include "Util/CharsetUtil.h"
#include "AppModel.h"

USING_NS_CC;
using namespace std;

//__________________________________________________________________________________________
// 过滤特殊字符
bool HasSpecialChars(string strText)
{
    if (strText.empty())
    {
        return false;
    }
    for (int i = 0; i < strText.size(); i++)
    {
        unsigned char c = strText.at(i);
        if (c <= 32 || c == 127)
        {
            // 空格、退格和特殊控制字符
            return true;
        }
    }
    return false;
}
//__________________________________________________________________________________________
// 验证邮箱格式
bool IsValidEmail(string strEmail)
{
    regex pattern("^[a-zA-Z0-9]+[a-zA-Z0-9_\\.\\-]*[a-zA-Z0-9]+@([a-zA-Z0-9]+\\.)+[a-zA-Z]{2,}$");
    return regex_match(strEmail, pattern);
}
//__________________________________________________________________________________________
// 验证密码复杂性
bool IsComplexPassword(std::string strText)
{
    regex pattern("^(?=.*[0-9])(?=.*[a-zA-Z]).{6,16}$");
    return regex_match(strText, pattern);
}
//__________________________________________________________________________________________
// 是否是纯粹的数字和字母
bool IsNumberAndChars(std::string strText)
{
    regex pattern("^[a-zA-Z0-9]+$");
    return regex_match(strText, pattern);
}
//__________________________________________________________________________________________
// 是否是纯粹的数字
bool IsNumbers(std::string strText)
{
    regex pattern("^[0-9]+$");
    return regex_match(strText, pattern);
}
//__________________________________________________________________________________________
// 是否有匹配上的
bool IsAnyMatch(std::string strText,std::vector<std::string> &keys)
{
    auto match = false;
    for(int i = 0; i < (int)keys.size();i++)
    {
        regex pattern(keys[i].c_str());
        if(regex_match(strText, pattern))
        {
            match = true;
            break;
        }
    }
    return match;
}
//__________________________________________________________________________________________
// 是否在字母表
bool IsInAlphabet(const char ch)
{
    return (ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122);
}
//__________________________________________________________________________________________
// 是否是数字
bool IsNumeric(const char ch)
{
    return (ch >= 48 && ch <= 57);
}
//__________________________________________________________________________________________
// 获取字符串的字符个数
int GetStringLength(std::string strText)
{
    vector<string> vec = CharsetUtil::getUTF8CharVector(strText);
    int count = 0;
    for (size_t i = 0; i < vec.size(); i++)
    {
        if (vec[i].size() > 1)
        {
            count += 2;
        }
        else
        {
            count += 1;
        }
    }
    return count;
}
//__________________________________________________________________________________________
//输出带豆号的数字串
string strCommaLL(long long llNum)
{
	string str;
	char szBuf[128] = {0};
	sprintf(szBuf, "%lld", llNum);
	int nLen = (int)strlen(szBuf);
	for(int i = 1; i <= nLen; i++)
	{
		str.insert(0, szBuf + (nLen - i), 1);
		if((i%3) == 0 && i != nLen && szBuf[nLen - i - 1] != '-')
			str.insert(0, ",", 1);
	}
	return str;
}
//__________________________________________________________________________________________
//输出带豆号的数字串
string strCommaULL(unsigned long long ullNum)
{
	string str;
	char szBuf[128] = {0};
	sprintf(szBuf, "%llu", ullNum);
	int nLen = (int)strlen(szBuf);
	for(int i = 1; i <= nLen; i++)
	{
		str.insert(0, szBuf + (nLen - i), 1);
		if((i % 3) == 0 && i != nLen && szBuf[nLen - i - 1] != '-')
			str.insert(0, ",", 1);
	}
	return str;
}
//_____________________________________________________________________________________
string strKULL(unsigned long long ullNum)
{
	string str;
	char szBuf[128] = {0};
	if(ullNum < 1000)
	{
		sprintf(szBuf, "%llu", ullNum);		
	}	
	else
	{
		double f = ullNum / 1000.0;
		long n = (long)f;
		int n1 = (int)((f*10.0) - n*10);		
		if(n1 > 0)
		{
			sprintf(szBuf, "%s.%1dK", strCommaULL(n).c_str(), n1);
		}
		else
		{
			sprintf(szBuf, "%sK", strCommaULL(n).c_str());	
		}
	}
	str.assign(szBuf);
	return str;
}
//_____________________________________________________________________________________
string strKULL2(unsigned long long ullNum)
{
	string str;
	char szBuf[128] = {0};
	if(ullNum < 10000)
	{
		return strCommaULL(ullNum);		
	}
	else
	{
		double f = ullNum / 1000.0;
		long n = (long)f;
		int n1 = (int)((f*10.0) - n*10);		
		if(n1 > 0)
		{
			sprintf(szBuf, "%s.%1dK", strCommaULL(n).c_str(), n1);
		}
		else
		{
			sprintf(szBuf, "%sK", strCommaULL(n).c_str());	
		}
	}
	str.assign(szBuf);
	return str;
}
//_____________________________________________________________________________________
//对字符串加密
string Encrypt(string strSrc)
{
	string str;	
	for(unsigned int i = 0; i < strSrc.length(); i++)
	{
		char c = strSrc.c_str()[i];
		unsigned char b = c ^ 0x8E;
		char buf[4] = {0};
		sprintf(buf, "%03d", b);		
		str.append(buf);
	}
	return str;
}
//_____________________________________________________________________________________
//对字符串加密
string Decrypt(string strSrc)
{
	string str;	
	for(unsigned int i=0; i<strSrc.length() / 3; i++)
	{
		char buf[4] = {0};
		strncpy(buf, strSrc.c_str() + i * 3, 3);
		int c = atoi(buf);		
		unsigned char b = c ^ 0x8E;				
		str.append(1, b);
	}
	return str;
}
//__________________________________________________________________________________________
void InitializeCriticalSection(PCRITICAL_SECTION pcs)
{
	if(pcs == 0) return;
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(pcs, &attr);
}
//__________________________________________________________________________________________
void DeleteCriticalSection(PCRITICAL_SECTION pcs)
{
	if(pcs == 0) return;
	pthread_mutex_destroy(pcs);
}
//__________________________________________________________________________________________
void EnterCriticalSection(PCRITICAL_SECTION pcs)
{
	if(pcs == 0) return;
	pthread_mutex_lock(pcs);
}
//__________________________________________________________________________________________
bool TryEnterCriticalSection(PCRITICAL_SECTION pcs)
{
	if(pcs == 0) return false;
	int nRet = pthread_mutex_trylock(pcs);
	return nRet==0 ? true : false;
}
//__________________________________________________________________________________________
void LeaveCriticalSection(PCRITICAL_SECTION pcs)
{
	if(pcs == 0) return;
	pthread_mutex_unlock(pcs);
}
//__________________________________________________________________________________________
string GetShort(string sOrgin)
{
    string sRet = "";
    if (sOrgin.empty())
    {
        return sRet;
    }
    vector<string> oLens = CharsetUtil::getUTF8CharVector(sOrgin);
    for (size_t i = 0; i < oLens.size(); i ++)
    {
        string sLen = oLens[i];
        if (sLen.size() > 1)
        {
            sRet = sLen;
            return sRet;
        }
        else if (sLen.size() == 1 && IsInAlphabet(sLen.front()))
        {
            sRet = sLen;
            for (size_t j = i + 1; j < oLens.size(); j ++)
            {
                string sSnd = oLens[j];
                if (sSnd.size() == 1 && IsInAlphabet(sSnd.front()))
                {
                    sRet += sSnd;
                    return sRet;
                }
            }
            break;
        }
    }
    return sRet;
}
//__________________________________________________________________________________________
bool IsValidId(string sId)
{
    bool bRet = false;
    if (sId.size() != 18)
    {
        return bRet;
    }
    vector<int> oW = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
    string sC = "10X98765432";
    unsigned long long nSum = 0;
    for (size_t i = 0; i < 17; i ++)
    {
        char ch = sId.at(i);
        if (!IsNumeric(ch))
        {
            return bRet;
        }
        int nNum = (int)ch - 48;
        nSum += nNum * oW[i];
    }
    int nRemain = nSum % 11;
    bRet = sC[nRemain] == sId[17];
    return bRet;
}
//__________________________________________________________________________________________
bool IsInChinese(string sText)
{
    regex pattern("[\u4e00-\u9fa5]+");
    return regex_match(sText, pattern);
}

std::vector<std::string> getCertainWidthTexts(const std::string& sText, const int& nMaxWidth, const int& nFontSize)
{
    vector<string> sRet = {};
    auto oText = CharsetUtil::getUTF8CharVector(sText);
    if (nFontSize > 0)
    {
        string sLine = "";
        string sOtherLine = "";
        auto pLabel = Label::createWithSystemFont(sLine.c_str(), "黑体", nFontSize);
        for (size_t i = 0; i < oText.size(); i ++)
        {
            pLabel->setString(sLine);
            if (pLabel->getContentSize().width > nMaxWidth)
            {
                sRet.push_back(sLine);
                sLine = oText.at(i);
                if (i == oText.size() - 1)
                {
                    for (size_t j = i; j < oText.size(); j++)
                    {
                        sOtherLine += oText.at(j);
                    }
                    sRet.push_back(sOtherLine);
                }
            }
            else if (i == oText.size() - 1)
            {
                sLine += oText.at(i);
                sRet.push_back(sLine);
            }
            else
            {
                sLine += oText.at(i);
            }
        }
    }
    if (sRet.empty())
    {
        sRet.push_back(sText);
    }
    return sRet;
}

//字符串分割函数
std::vector<std::string> split(std::string str,std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;//扩展字符串以方便操作
    int size=str.size();
    
    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

std::string getCertainWidthOmitText(const std::string& sText, const int& nFontSize, const int& nNums)
{
    string sValid = "";
    vector<string> oTxts = CharsetUtil::getUTF8CharVector(sText);
    auto pLabel = Label::createWithSystemFont("", "黑体", nFontSize);
    for (size_t i = 0; i < oTxts.size(); i ++)
    {
        pLabel->setString(sValid + oTxts.at(i));
        if (pLabel->getContentSize().width > nFontSize * nNums)
        {
            break;
        }
        sValid += oTxts.at(i);
    }
    if (sValid.size() < sText.size())
    {
        sValid += "...";
    }
    return sValid;
}

// base64字符
static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// 是否是base64类型数据
bool is_base64(unsigned char c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

// 解析base64数据
std::string base64_decode(std::string const& encoded_string)
{
    int in_len = static_cast<int>(encoded_string.size());
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;
    
    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
    {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);
            
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            
            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }
    
    if (i)
    {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;
        
        for (j = 0; j <4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);
        
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        
        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }
    
    return ret;
}

bool isTextSendCorrect(std::string text)
{
    for (auto&i : AppModel::getInstance()->m_pShopModel->m_emojiStringList)
    {
        if (text == i.second)
            return false;
    }
    return true;
};
