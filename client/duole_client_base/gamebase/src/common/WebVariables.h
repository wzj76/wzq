//
//  WebVariables.h
//  gamebase
//
//  Created by 曾琪 on 14-9-23.
//
//

#ifndef GouJi_WebVariables_h
#define GouJi_WebVariables_h

#include <string>
#include <vector>

namespace duole {
class WebVariables
{
public:
    // 添加参数
    void input(const std::string &sKey, const int nValue);
    void input(const std::string &sKey, const unsigned int nValue);
    void input(const std::string &sKey, const long nValue);
    void input(const std::string &sKey, const long long nValue);
    void input(const std::string &sKey, const unsigned long nValue);
    void input(const std::string &sKey, const unsigned long long nValue);
    void input(const std::string &sKey, const float fValue);
    void input(const std::string &sKey, const double fValue);
    void input(const std::string &sKey, const bool bValue);
    void input(const std::string &sKey, const char* sValue);
    void input(const std::string &sKey, const std::string &sValue);
    
    // 重置参数
    void reset();
    
    // 获取URL编码字符串
    std::string toURLEncodedString();
    
private:
    char digit2hex(const char cValue);
    std::string encode(const std::string &sValue);
    
private:
    std::vector<std::pair<std::string, std::string>> m_vInputs;
};
}
#endif
