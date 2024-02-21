//
//  DLHttpClient.hpp
//  PHZBusinessSvrd
//
//  Created by 曹涛old on 2020/5/27.
//  Copyright © 2020年 曹涛. All rights reserved.
//
/*
 封装http的发送请求等
 */

#ifndef DLHttpClient_h
#define DLHttpClient_h

#include <stdio.h>
#include <string>
#include <map>

#include "HttpClient.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "json/json.h"

using namespace xnet;
using namespace std;

using RequestPtr	= shared_ptr<xnet::HttpRequest>;
using ResponsePtr	= shared_ptr<xnet::HttpResponse>;

struct DLHttpResponse {
    bool succeed;
    int code;
    string reason;
    Json::Reader r;
    Json::Value v;
};

class DLHttpClient {
private:
    static int hex2dec(char c);
    static char dec2hex(short int c);

protected:
    static string urlEncode(string url);
    
public:
    //异步发送接口
    static bool httpGet(const string url, const map<string, string> params, RequestCallback onMessage);
    
    static bool httpPost(const string url, const map<string, string> params, RequestCallback onMessage);
    
    //检查结果
    static DLHttpResponse getResponse(const std::shared_ptr<HttpResponse>& );
};


#endif /* DLHttpClient_h */
