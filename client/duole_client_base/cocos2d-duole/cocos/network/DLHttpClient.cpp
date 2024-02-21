/****************************************************************************
 Copyright (c) 2012      greathqy
 Copyright (c) 2012      cocos2d-x.org
 Copyright (c) 2013-2014 Chukong Technologies Inc.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "DLHttpClient.h"

#include <thread>
#include <queue>
#include <condition_variable>

#include <errno.h>

//#include <curl/curl.h>
#include "curl/curl.h"

#include "base/CCVector.h"
#include "base/CCDirector.h"
#include "base/CCScheduler.h"

#include "platform/CCFileUtils.h"

NS_CC_BEGIN

namespace network {
    
    static std::mutex       s_dlRequestQueueMutex;
    static std::mutex       s_dlResponseQueueMutex;
    
    static std::condition_variable_any s_dlSleepCondition;
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    typedef int int32_t;
#endif
    
    static Vector<DLHttpRequest*>*  s_dlRequestQueue = nullptr;
    static Vector<DLHttpResponse*>* s_dlResponseQueue = nullptr;
    
    static DLHttpClient *s_pDlHttpClient = nullptr; // pointer to singleton
    
    static char s_dlErrorBuffer[CURL_ERROR_SIZE] = {0};
    
    typedef size_t (*write_callback)(void *ptr, size_t size, size_t nmemb, void *stream);
    
    static std::string s_dlCookieFilename = "";
    
    static std::string s_dlSslCaFilename = "";
    
    // Callback function used by libcurl for collect response data
    static size_t writeData(void *ptr, size_t size, size_t nmemb, void *stream)
    {
        std::vector<char> *recvBuffer = (std::vector<char>*)stream;
        size_t sizes = size * nmemb;
        
        // add data to the end of recvBuffer
        // write data maybe called more than once in a single request
        recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+sizes);
        
        return sizes;
    }
    
    // Callback function used by libcurl for collect header data
    static size_t writeHeaderData(void *ptr, size_t size, size_t nmemb, void *stream)
    {
        std::vector<char> *recvBuffer = (std::vector<char>*)stream;
        size_t sizes = size * nmemb;
        
        // add data to the end of recvBuffer
        // write data maybe called more than once in a single request
        recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+sizes);
        
        return sizes;
    }
    
    
    static int processGetTask(DLHttpRequest *request, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream, char *errorBuffer);
    static int processPostTask(DLHttpRequest *request, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream, char *errorBuffer);
    static int processPutTask(DLHttpRequest *request, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream, char *errorBuffer);
    static int processDeleteTask(DLHttpRequest *request, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream, char *errorBuffer);
    // int processDownloadTask(HttpRequest *task, write_callback callback, void *stream, int32_t *errorCode);
    static void processResponse(DLHttpResponse* response, char* errorBuffer);
    
    static DLHttpRequest *s_requestSentinel = new DLHttpRequest;
    
    // Worker thread
    void DLHttpClient::networkThread()
    {
        auto scheduler = Director::getInstance()->getScheduler();
        
        while (true)
        {
            DLHttpRequest *request;
            
            // step 1: send http request if the requestQueue isn't empty
            {
                std::lock_guard<std::mutex> lock(s_dlRequestQueueMutex);
                while (s_dlRequestQueue->empty()) {
                    s_dlSleepCondition.wait(s_dlRequestQueueMutex);
                }
                request = s_dlRequestQueue->at(0);
                s_dlRequestQueue->erase(0);
            }
            
            if (request == s_requestSentinel) {
                break;
            }
            
            // step 2: libcurl sync access
            
            // Create a HttpResponse object, the default setting is http access failed
            DLHttpResponse *response = new (std::nothrow) DLHttpResponse(request);
            
            processResponse(response, s_dlErrorBuffer);
            
            
            // add response packet into queue
            s_dlResponseQueueMutex.lock();
            s_dlResponseQueue->pushBack(response);
            s_dlResponseQueueMutex.unlock();
            
            if (nullptr != s_pDlHttpClient) {
                scheduler->performFunctionInCocosThread(CC_CALLBACK_0(DLHttpClient::dispatchResponseCallbacks, this));
            }
        }
        
        // cleanup: if worker thread received quit signal, clean up un-completed request queue
        s_dlRequestQueueMutex.lock();
        s_dlRequestQueue->clear();
        s_dlRequestQueueMutex.unlock();
        
        
        if (s_dlRequestQueue != nullptr) {
            delete s_dlRequestQueue;
            s_dlRequestQueue = nullptr;
            delete s_dlResponseQueue;
            s_dlResponseQueue = nullptr;
        }
        
    }
    
    // Worker thread
    void DLHttpClient::networkThreadAlone(DLHttpRequest* request)
    {
        // Create a HttpResponse object, the default setting is http access failed
        DLHttpResponse *response = new (std::nothrow) DLHttpResponse(request);
        char errorBuffer[CURL_ERROR_SIZE] = { 0 };
        processResponse(response, errorBuffer);
        
        auto scheduler = Director::getInstance()->getScheduler();
        scheduler->performFunctionInCocosThread([response, request]{
            const ccDLHttpRequestCallback& callback = request->getCallback();
            Ref* pTarget = request->getTarget();
            SEL_DLHttpResponse pSelector = request->getSelector();
            
            if (callback != nullptr)
            {
                callback(s_pDlHttpClient, response);
            }
            else if (pTarget && pSelector)
            {
                (pTarget->*pSelector)(s_pDlHttpClient, response);
            }
            response->release();
            // do not release in other thread
            request->release();
        });
    }
    
    //Configure curl's timeout property
    static bool configureCURL(CURL *handle, char *errorBuffer)
    {
        if (!handle) {
            return false;
        }
        
        int32_t code;
        code = curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, errorBuffer);
        if (code != CURLE_OK) {
            return false;
        }
        code = curl_easy_setopt(handle, CURLOPT_TIMEOUT, DLHttpClient::getInstance()->getTimeoutForRead());
        if (code != CURLE_OK) {
            return false;
        }
        code = curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, DLHttpClient::getInstance()->getTimeoutForConnect());
        if (code != CURLE_OK) {
            return false;
        }
        if (s_dlSslCaFilename.empty()) {
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0L);
        } else {
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 1L);
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 2L);
            curl_easy_setopt(handle, CURLOPT_CAINFO, s_dlSslCaFilename.c_str());
        }
        
        // FIXED #3224: The subthread of CCHttpClient interrupts main thread if timeout comes.
        // Document is here: http://curl.haxx.se/libcurl/c/curl_easy_setopt.html#CURLOPTNOSIGNAL
        curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1L);
        
        return true;
    }
    
    class CURLRaii
    {
        /// Instance of CURL
        CURL *_curl;
        /// Keeps custom header data
        curl_slist *_headers;
    public:
        CURLRaii()
        : _curl(curl_easy_init())
        , _headers(nullptr)
        {
        }

        ~CURLRaii()
        {
            if (_curl)
                curl_easy_cleanup(_curl);
            /* free the linked list for header data */
            if (_headers)
                curl_slist_free_all(_headers);
        }

        template <class T>
        bool setOption(CURLoption option, T data)
        {
            return CURLE_OK == curl_easy_setopt(_curl, option, data);
        }

        /**
         * @brief Inits CURL instance for common usage
         * @param request Null not allowed
         * @param callback Response write callback
         * @param stream Response write stream
         */
        bool init(DLHttpRequest *request, write_callback callback, void *stream, write_callback headerCallback, void *headerStream, char *errorBuffer)
        {
            if (!_curl)
                return false;
            if (!configureCURL(_curl, errorBuffer))
                return false;

            /* get custom header data (if set) */
            std::vector<std::string> headers=request->getHeaders();
            if(!headers.empty())
            {
                /* append custom headers one by one */
                for (std::vector<std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
                    _headers = curl_slist_append(_headers,it->c_str());
                /* set custom headers for curl */
                if (!setOption(CURLOPT_HTTPHEADER, _headers))
                    return false;
            }
            if (!s_dlCookieFilename.empty()) {
                if (!setOption(CURLOPT_COOKIEFILE, s_dlCookieFilename.c_str())) {
                    return false;
                }
                if (!setOption(CURLOPT_COOKIEJAR, s_dlCookieFilename.c_str())) {
                    return false;
                }
            }

            return setOption(CURLOPT_URL, request->getUrl())
            && setOption(CURLOPT_WRITEFUNCTION, callback)
            && setOption(CURLOPT_WRITEDATA, stream)
            && setOption(CURLOPT_HEADERFUNCTION, headerCallback)
            && setOption(CURLOPT_HEADERDATA, headerStream);

        }

        /// @param responseCode Null not allowed
        bool perform(long *responseCode)
        {
            if (CURLE_OK != curl_easy_perform(_curl))
                return false;
            CURLcode code = curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, responseCode);
            if (code != CURLE_OK || !(*responseCode >= 200 && *responseCode < 300)) {
                CCLOGERROR("Curl curl_easy_getinfo failed: %s", curl_easy_strerror(code));
                return false;
            }
            // Get some mor data.

            return true;
        }
    };

    //Process Get Request
    static int processGetTask(DLHttpRequest *request, write_callback callback, void *stream, long *responseCode, write_callback headerCallback, void *headerStream, char *errorBuffer)
    {
        CURLRaii curl;
        bool ok = curl.init(request, callback, stream, headerCallback, headerStream, errorBuffer)
        && curl.setOption(CURLOPT_FOLLOWLOCATION, true)
        && curl.perform(responseCode);
        return ok ? 0 : 1;
    }

    //Process POST Request
    static int processPostTask(DLHttpRequest *request, write_callback callback, void *stream, long *responseCode, write_callback headerCallback, void *headerStream, char *errorBuffer)
    {
        CURLRaii curl;
        bool ok = curl.init(request, callback, stream, headerCallback, headerStream, errorBuffer)
        && curl.setOption(CURLOPT_POST, 1)
        && curl.setOption(CURLOPT_POSTFIELDS, request->getRequestData())
        && curl.setOption(CURLOPT_POSTFIELDSIZE, request->getRequestDataSize())
        && curl.perform(responseCode);
        return ok ? 0 : 1;
    }

    //Process PUT Request
    static int processPutTask(DLHttpRequest *request, write_callback callback, void *stream, long *responseCode, write_callback headerCallback, void *headerStream, char *errorBuffer)
    {
        CURLRaii curl;
        bool ok = curl.init(request, callback, stream, headerCallback, headerStream, errorBuffer)
        && curl.setOption(CURLOPT_CUSTOMREQUEST, "PUT")
        && curl.setOption(CURLOPT_POSTFIELDS, request->getRequestData())
        && curl.setOption(CURLOPT_POSTFIELDSIZE, request->getRequestDataSize())
        && curl.perform(responseCode);
        return ok ? 0 : 1;
    }

    //Process DELETE Request
    static int processDeleteTask(DLHttpRequest *request, write_callback callback, void *stream, long *responseCode, write_callback headerCallback, void *headerStream, char *errorBuffer)
    {
        CURLRaii curl;
        bool ok = curl.init(request, callback, stream, headerCallback, headerStream, errorBuffer)
        && curl.setOption(CURLOPT_CUSTOMREQUEST, "DELETE")
        && curl.setOption(CURLOPT_FOLLOWLOCATION, true)
        && curl.perform(responseCode);
        return ok ? 0 : 1;
    }


    // Process Response
    static void processResponse(DLHttpResponse* response, char* errorBuffer)
    {
        auto request = response->getHttpRequest();
        long responseCode = -1;
        int retValue = 0;

        // Process the request -> get response packet
        switch (request->getRequestType())
        {
            case DLHttpRequest::Type::GET: // HTTP GET
                retValue = processGetTask(request,
                                          writeData,
                                          response->getResponseData(),
                                          &responseCode,
                                          writeHeaderData,
                                          response->getResponseHeader(),
                                          errorBuffer);
                break;

            case DLHttpRequest::Type::POST: // HTTP POST
                retValue = processPostTask(request,
                                           writeData,
                                           response->getResponseData(),
                                           &responseCode,
                                           writeHeaderData,
                                           response->getResponseHeader(),
                                           errorBuffer);
                break;

            case DLHttpRequest::Type::PUT:
                retValue = processPutTask(request,
                                          writeData,
                                          response->getResponseData(),
                                          &responseCode,
                                          writeHeaderData,
                                          response->getResponseHeader(),
                                          errorBuffer);
                break;

            case DLHttpRequest::Type::DELETE:
                retValue = processDeleteTask(request,
                                             writeData,
                                             response->getResponseData(),
                                             &responseCode,
                                             writeHeaderData,
                                             response->getResponseHeader(),
                                             errorBuffer);
                break;

            default:
                CCASSERT(true, "CCHttpClient: unkown request type, only GET and POSt are supported");
                break;
        }

        // write data to HttpResponse
        response->setResponseCode(responseCode);

        if (retValue != 0)
        {
            response->setSucceed(false);
            response->setErrorBuffer(errorBuffer);
        }
        else
        {
            response->setSucceed(true);
        }
    }

    // HttpClient implementation
    DLHttpClient* DLHttpClient::getInstance()
    {
        if (s_pDlHttpClient == nullptr) {
            s_pDlHttpClient = new (std::nothrow) DLHttpClient();
        }

        return s_pDlHttpClient;
    }
    
    void DLHttpClient::destroyInstance()
    {
        CC_SAFE_DELETE(s_pDlHttpClient);
    }

    void DLHttpClient::enableCookies(const char* cookieFile) {
        if (cookieFile) {
            s_dlCookieFilename = std::string(cookieFile);
        }
        else {
            s_dlCookieFilename = (FileUtils::getInstance()->getWritablePath() + "cookieFile_dl.txt");
        }
    }
    
    void DLHttpClient::setSSLVerification(const std::string& caFile)
    {
        s_dlSslCaFilename = caFile;
    }
    
    DLHttpClient::DLHttpClient()
    : _timeoutForConnect(30)
    , _timeoutForRead(60)
    {
    }
    
    DLHttpClient::~DLHttpClient()
    {
        if (s_dlRequestQueue != nullptr) {
            {
                std::lock_guard<std::mutex> lock(s_dlRequestQueueMutex);
                s_dlRequestQueue->pushBack(s_requestSentinel);
            }
            s_dlSleepCondition.notify_one();
        }
        
        s_pDlHttpClient = nullptr;
    }
    
    //Lazy create semaphore & mutex & thread
    bool DLHttpClient::lazyInitThreadSemphore()
    {
        if (s_dlRequestQueue != nullptr) {
            return true;
        } else {
            
            s_dlRequestQueue = new (std::nothrow) Vector<DLHttpRequest*>();
            s_dlResponseQueue = new (std::nothrow) Vector<DLHttpResponse*>();
            
            auto t = std::thread(CC_CALLBACK_0(DLHttpClient::networkThread, this));
            t.detach();
        }
        
        return true;
    }
    
    //Add a get task to queue
    void DLHttpClient::send(DLHttpRequest* request)
    {    
        if (false == lazyInitThreadSemphore()) 
        {
            return;
        }
        
        if (!request)
        {
            return;
        }
        
        request->retain();
        
        if (nullptr != s_dlRequestQueue) {
            s_dlRequestQueueMutex.lock();
            s_dlRequestQueue->pushBack(request);
            s_dlRequestQueueMutex.unlock();
            
            // Notify thread start to work
            s_dlSleepCondition.notify_one();
        }
    }
    
    void DLHttpClient::sendImmediate(DLHttpRequest* request)
    {
        if(!request)
        {
            return;
        }
        
        request->retain();
        auto t = std::thread(&DLHttpClient::networkThreadAlone, this, request);
        t.detach();
    }
    
    // Poll and notify main thread if responses exists in queue
    void DLHttpClient::dispatchResponseCallbacks()
    {
        // log("CCHttpClient::dispatchResponseCallbacks is running");
        //occurs when cocos thread fires but the network thread has already quited
        if (nullptr == s_dlResponseQueue) {
            return;
        }
        DLHttpResponse* response = nullptr;
        
        s_dlResponseQueueMutex.lock();
        
        if (!s_dlResponseQueue->empty())
        {
            response = s_dlResponseQueue->at(0);
            s_dlResponseQueue->erase(0);
        }
        
        s_dlResponseQueueMutex.unlock();
        
        if (response)
        {
            DLHttpRequest *request = response->getHttpRequest();
            const ccDLHttpRequestCallback& callback = request->getCallback();
            Ref* pTarget = request->getTarget();
            SEL_DLHttpResponse pSelector = request->getSelector();
            
            if (callback != nullptr)
            {
                callback(this, response);
            }
            else if (pTarget && pSelector)
            {
                (pTarget->*pSelector)(this, response);
            }
            
            response->release();
            // do not release in other thread
            request->release();
        }
    }
    
}

NS_CC_END
