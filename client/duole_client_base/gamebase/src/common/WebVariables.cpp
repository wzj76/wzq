//
//  WebVariables.cpp
//  gamebase
//
//  Created by 曾琪 on 14-9-23.
//
//

#include "WebVariables.h"
#include <sstream>

using namespace std;
namespace duole {
void WebVariables::input(const string &sKey, const int nValue)
{
    ostringstream obStream;
    obStream << nValue;
    input(sKey, obStream.str());
}

void WebVariables::input(const string &sKey, const unsigned int nValue)
{
    ostringstream obStream;
    obStream << nValue;
    input(sKey, obStream.str());
}

void WebVariables::input(const string &sKey, const long nValue)
{
    ostringstream obStream;
    obStream << nValue;
    input(sKey, obStream.str());
}

void WebVariables::input(const string &sKey, const long long nValue)
{
    ostringstream obStream;
    obStream << nValue;
    input(sKey, obStream.str());
}

void WebVariables::input(const string &sKey, const unsigned long nValue)
{
    ostringstream obStream;
    obStream << nValue;
    input(sKey, obStream.str());
}

void WebVariables::input(const string &sKey, const unsigned long long nValue)
{
    ostringstream obStream;
    obStream << nValue;
    input(sKey, obStream.str());
}

void WebVariables::input(const string &sKey, const float fValue)
{
    ostringstream obStream;
    obStream << fValue;
    input(sKey, obStream.str());
}

void WebVariables::input(const string &sKey, const double fValue)
{
    ostringstream obStream;
    obStream << fValue;
    input(sKey, obStream.str());
}

void WebVariables::input(const string &sKey, const bool nValue)
{
    input(sKey, nValue ? "1" : "0");
}

void WebVariables::input(const string &sKey, const char* sValue)
{
    input(sKey, string(sValue));
}

void WebVariables::input(const string &sKey, const string &sValue)
{
    m_vInputs.push_back(make_pair(sKey, sValue));
}

void WebVariables::reset()
{
    m_vInputs.clear();
}

string WebVariables::toURLEncodedString()
{
    ostringstream obStream;
    
    for (vector<pair<string, string>>::size_type i = 0; i < m_vInputs.size(); i++)
    {
        obStream
        << (i > 0 ? "&" : "")
        << m_vInputs[i].first
        << "="
        << encode(m_vInputs[i].second);
    }
    
    return obStream.str();
}

char WebVariables::digit2hex(const char cValue)
{
    return (cValue > 9) ? (cValue - 10 + 'A') : (cValue + '0');
}

string WebVariables::encode(const string &sValue)
{
    string sRet;
    for(string::size_type i = 0; i < sValue.size(); i++)
    {
        char c = sValue[i];
        if ((c >= '0' && c <= '9')
            || (c >= 'A' && c <= 'Z')
            || (c >= 'a' && c <= 'z')
            || (c == '~')
            || (c == '-')
            || (c == '_')
            || (c == '.'))
        {
            sRet.append(1, c);
        }
        else
        {
            sRet.append("%");
            sRet.append(1, digit2hex((c & 0xF0) >> 4));
            sRet.append(1, digit2hex(c & 0x0F));
        }
    }
    return sRet;
}
}
