/*******************************************************************************
 一些宏定义。
 kInvalidValue: int类型，表示无效值。
 kInvalidPair:  pair<int, int>类型，表示无效pair。
 NO_BT = 0, YES_BT = 1: 表示确认按钮和取消按钮所扮演的角色（role）。
 fps_to_time：fla文件中的帧数对应的时间（可能要具体情况具体设置，并且不同的fla文件有可能设置的不一样）。
 str_format: 代替sprintf调用。
 char2str, int2str, float2str等： char、int、float等类型转成const char *。
 *******************************************************************************/

#pragma once

#include "cocos2d.h"

namespace util
{
    // 无效的值
    const int kInvalidValue = -0xffff;
    const std::pair<int, int> kInvalidPair(kInvalidValue, kInvalidValue);
    
    // 菜单上的“否”角色按钮的tag和“是”角色按钮的tag
    enum { NO_BT, YES_BT };
    
    // fla文件中的帧数转成秒，注意不同的fla文件很有可能设置的fps不一样。
    inline float fps_to_time(int frame_num)
    {
        return float(frame_num) / 30;
    }
    
    // 代替sprintf调用
    inline const char *str_format(const char *format, ...)
    {
        const int kMaxStringLen = 100 * 1024;  // 最多100K
        char s[kMaxStringLen] = {0};
        va_list ap;
        va_start(ap, format);  //得到变参的起始地址
        vsnprintf(s, kMaxStringLen, format, ap);
        va_end(ap);
        return cocos2d::CCString::create(s)->getCString();
    }
    
    // string转成int
    inline int str2int(std::string str)
    {
        return atoi(str.c_str());
    }
    
    // string转成bool
    inline bool str2bool(const std::string &str)
    {
        if(strcmp(str.c_str(), "true") == 0)
            return true;
        
        return false;
    }
    
    // char转成const char *
    inline const char *char2str(char ch)
    {
        return str_format("%c", ch);
    }
    
    // int转成const char *
    inline const char *int2str(int num)
    {
        return str_format("%d", num);
    }
    
    // long转成const char *
    inline const char *long2str(long num)
    {
        return str_format("%ld", num);
    }
    
    // unsigned long long转成const char *
    inline const char *ull2str(unsigned long long num)
    {
        return str_format("%llu", num);
    }
    
    // float转成const char *，小数点后1位
    inline const char *float2str(float num)
    {
        return str_format("%.1f", num);
    }
    
    // float转成const char *，小数点后n位
    inline const char *float2str(float num, int n_number_after_dot)
    {
        return str_format("%.*f", n_number_after_dot, num);
    }
    
    // double转成const char *，小数点后1位
    inline const char *double2str(double num)
    {
        return str_format("%.1lf", num);
    }
    
    // double转成const char *，小数点后n位
    inline const char *double2str(double num, int n_number_after_dot)
    {
        return str_format("%.*lf", n_number_after_dot, num);
    }
    
    inline std::string formatNumber(int nNum)
    {
        std::string sText = "";
        if (nNum < 10000)
        {
            sText = int2str(nNum);
        }
        else if (nNum >= 10000)
        {
            if (nNum % 10000 == 0)
            {
                sText = int2str((nNum / 10000));
            } else
            {
                sText = float2str((nNum / 10000.0f));
            }
            sText.append("万");
        }
        return sText;
    }
    
    inline char digit2hex(const char cValue)
    {
        return (cValue > 9) ? (cValue - 10 + 'A') : (cValue + '0');
    }
    
    inline std::string shareUrlEncode(const std::string &sValue)
    {
        std::string sRet;
        for(std::string::size_type i = 0; i < sValue.size(); i++)
        {
            char c = sValue[i];
            if(c == ' ')
            {
                sRet.append("%");
                sRet.append(1, digit2hex((c & 0xF0) >> 4));
                sRet.append(1, digit2hex(c & 0x0F));
            }
            else
            {
                sRet.append(1, c);
            }
        }
        return sRet;
    }
}
