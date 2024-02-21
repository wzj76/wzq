//
//  LuaHttpRequest.cpp
//  DuoLeMJ
//
//  Created by weiran on 2020/6/2.
//

#include "LuaHttpRequest.h"

USING_NS_CC;
using namespace cocos2d::network;
using namespace std;

namespace duole {

LuaHttpRequest::LuaHttpRequest()
:
m_sUrl(""),
m_eMethod(HttpRequest::Type::UNKNOWN),
m_eScriptHandler(ScriptHandlerMgr::HandlerType::NODE),
m_sRequestData(""),
m_sResponseData(""),
m_sErrorBuffer(""),
m_oRequestHeader({}),
m_nResponseCode(0),
m_bSucceed(false)
{}

LuaHttpRequest::~LuaHttpRequest()
{}

void LuaHttpRequest::send()
{
    sendRequest();
}

void LuaHttpRequest::sendImmediate()
{
    sendRequest(true);
}

void LuaHttpRequest::onHttpRespone(HttpClient* pSender, HttpResponse* pResponse)
{
    if (pResponse == nullptr || !pResponse->isSucceed())
    {
        m_bSucceed = false;
        m_nResponseCode = 0;
        m_sResponseData = "";
        if (pResponse)
        {
            m_sErrorBuffer = pResponse->getErrorBuffer();
        }
    }
    else
    {
        m_bSucceed = true;
        m_nResponseCode = pResponse->getResponseCode();
        
        m_sResponseData = "";
        vector<char> *pBuffer = pResponse->getResponseData();
        m_sResponseData.assign(pBuffer->begin(), pBuffer->end());
    }
    
    int handler = cocos2d::ScriptHandlerMgr::getInstance()->getObjectHandler((void*)this, m_eScriptHandler);
    if (0 != handler)
    {
        CommonScriptData data(handler, "");
        ScriptEvent event(cocos2d::ScriptEventType::kCommonEvent, (void*)&data);
        ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
    }
    
    release();
}

void LuaHttpRequest::setUrl(const string& sUrl)
{
    m_sUrl = sUrl;
}

string LuaHttpRequest::getUrl()
{
    return m_sUrl;
}

void LuaHttpRequest::setMethod(const string& sMethod)
{
    m_eMethod = HttpRequest::Type::UNKNOWN;
    if (sMethod.compare("post") == 0 || sMethod.compare("POST") == 0)
    {
        m_eMethod = HttpRequest::Type::POST;
    }
    else if (sMethod.compare("get") == 0 || sMethod.compare("GET") == 0)
    {
        m_eMethod = HttpRequest::Type::GET;
    }
}

void LuaHttpRequest::setRequestData(const string& sData)
{
    m_sRequestData = sData;
}

void LuaHttpRequest::setScriptHandler(ScriptHandlerMgr::HandlerType eHandler)
{
    m_eScriptHandler = eHandler;
}

ScriptHandlerMgr::HandlerType LuaHttpRequest::getScriptHandler()
{
    return m_eScriptHandler;
}

string LuaHttpRequest::getResponseData()
{
    return m_sResponseData;
}

Ref* LuaHttpRequest::autorelease(void)
{
    CCASSERT(false, "HttpResponse is used between network thread and ui thread \
             therefore, autorelease is forbidden here");
    return nullptr;
}

void LuaHttpRequest::configureRequest(HttpRequest *pRequest, const std::string& sUrl, const string& sData, HttpRequest::Type eMethod)
{
    pRequest->setRequestType(eMethod);
    if (eMethod == HttpRequest::Type::POST)
    {
        pRequest->setRequestData(sData.c_str(), sData.size());
    }
    pRequest->setHeaders(getHttpRequestHeader());
    pRequest->setUrl(sUrl.c_str());
    pRequest->setResponseCallback(CC_CALLBACK_2(LuaHttpRequest::onHttpRespone, this));
}

/**
 *  @brief Set Request header for next call.
 *  @param pField  Name of the Header to be set.
 *  @param pValue  Value of the Headerfield
 */
void LuaHttpRequest::setRequestHeader(const char* pField, const char* pValue)
{
    std::stringstream value_s;
    
    auto iter = m_oRequestHeader.find(pField);
    
    // Concatenate values when header exists.
    if (iter != m_oRequestHeader.end())
    {
        value_s << iter->second << "," << pValue;
    }
    else
    {
        value_s << pValue;
    }
    
    m_oRequestHeader[pField] = value_s.str();
}

/**
 * @brief  If headers has been set, pass them to curl.
 *
 */
std::vector<std::string> LuaHttpRequest::getHttpRequestHeader()
{
    std::vector<std::string> header = {};
    
    for (auto it = m_oRequestHeader.begin(); it != m_oRequestHeader.end(); ++it)
    {
        header.push_back(it->first + ": " + it->second);
    }
    
    return header;
}

long LuaHttpRequest::getResponseCode()
{
    return m_nResponseCode;
}

bool LuaHttpRequest::isSucceed()
{
    return m_bSucceed;
}

string LuaHttpRequest::getErrorBuffer()
{
    return m_sErrorBuffer;
}

void LuaHttpRequest::sendRequest(bool bImmediate)
{
    HttpRequest* pRequest = new HttpRequest();
    configureRequest(pRequest, m_sUrl, m_sRequestData, m_eMethod);
    
    if (bImmediate)
    {
        network::HttpClient::getInstance()->sendImmediate(pRequest);
    }
    else
    {
        network::HttpClient::getInstance()->send(pRequest);
    }
    pRequest->release();
    
    retain();
}
}
