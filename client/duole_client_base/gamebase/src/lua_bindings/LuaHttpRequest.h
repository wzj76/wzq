//
//  LuaHttpRequest.h
//  DuoLeMJ
//
//  Created by weiran on 2020/6/2.
//

#pragma once

#include "cocos2d.h"
#include "network/HttpClient.h"
#include "cocos2d/LuaScriptHandlerMgr.h"

namespace duole {
class LuaHttpRequest : public cocos2d::Ref
{
public:
    void send();
    void sendImmediate();
    
    void setUrl(const std::string& sUrl);
    std::string getUrl();
    
    void setMethod(const std::string& sMethod);
    
    void setRequestData(const std::string& sData);
    
    void setScriptHandler(cocos2d::ScriptHandlerMgr::HandlerType eHandler);
    cocos2d::ScriptHandlerMgr::HandlerType getScriptHandler();
    
    std::string getResponseData();
    
    void setRequestHeader(const char* pField, const char* pValue);
    
    std::vector<std::string> getHttpRequestHeader();
    
    long getResponseCode();
    
    bool isSucceed();
    
    std::string getErrorBuffer();
    
    /** Override autorelease method to avoid developers to call it */
    cocos2d::Ref* autorelease(void);
    
    LuaHttpRequest();
    ~LuaHttpRequest();
    
private:
    void sendRequest(bool bImmediate = false);
    
    void configureRequest(cocos2d::network::HttpRequest *pRequest, const std::string& sUrl, const std::string& sData, cocos2d::network::HttpRequest::Type eMethod);
    void onHttpRespone(cocos2d::network::HttpClient* pSender, cocos2d::network::HttpResponse* pResponse);
    
private:
    cocos2d::ScriptHandlerMgr::HandlerType  m_eScriptHandler;
    std::string                             m_sUrl;
    std::string                             m_sRequestData;
    std::string                             m_sResponseData;
    std::string                             m_sErrorBuffer;
    cocos2d::network::HttpRequest::Type     m_eMethod;
    long                                    m_nResponseCode;
    bool                                    m_bSucceed;
    std::unordered_map<std::string, std::string>    m_oRequestHeader;
};
}
