//
//  AssetsDownloader.h
//  ZhuoJiMJ
//
//  Created by weiran on 2017/8/26.
//
//

#ifndef AssetsDownloader_h
#define AssetsDownloader_h

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "curl/include/ios/curl/curl.h"
#include "curl/include/ios/curl/easy.h"
#else
#include "curl/include/android/curl/curl.h"
#include "curl/include/android/curl/easy.h"
#endif

#include "cocos2d/LuaScriptHandlerMgr.h"

namespace duole {
class AssetsDownloader : public cocos2d::Ref
{
public:
    enum class ErrorCode
    {
        kErrorDownLoad = 0,
        kErrorDecompress,
        kErrorUrl
    };
    
    typedef std::function<void(int, AssetsDownloader*)> SuccessCallback;
    typedef std::function<void(AssetsDownloader::ErrorCode, int, AssetsDownloader*)> ErrorCallback;
    typedef std::function<void(double, double, int, AssetsDownloader*)> ProgressCallback;
    
    static AssetsDownloader* create();
    bool init();
    
    void downloadAsync(const std::string& sUrl, const std::string& sFilePath);
    
    void unzipResAsync(const std::string& sSourceFile, const std::string& sFolderName, const std::string&  sPassword = "");
    
    void decompressDownLoadZip(const std::string& sFileName,const std::string& sPassword = "");
    
    void setProgressCallback(const ProgressCallback& pCallback);
    ProgressCallback getProgressCallback();
    
    void setSuccessCallback(const SuccessCallback& pCallback);
    SuccessCallback getSuccessCallback();
    
    void setErrorCallback(const ErrorCallback& pCallback);
    ErrorCallback getErrorCallback();
    
    void setTag(int nTag);
    int getTag();
    
    void setUserData(const std::string& sUserData);
    std::string getUserData();
    
    void setStop(bool bStop);
    bool isStop();
    
    void setScriptHandler(cocos2d::ScriptHandlerMgr::HandlerType eType);
    cocos2d::ScriptHandlerMgr::HandlerType getScriptHandler();
    
    void onProgress(double fTotal, double fCur);
    double getProgressTotal();
    double getProgressCur();
    std::string getErrorCode();
    
private:
    void unzipRes(const std::string& sSourceFile, const std::string& sFolderName, const std::string& sPassword);
    void download(const std::string& sUrl, const std::string& sFilePath);
    bool processDownloadTask(const std::string& sPackageUrl);
    
    std::string basename(const std::string& sPath);
    std::string getExtension(const std::string& sString);
    bool decompress(const std::string& sFileName, const std::string& sPassword = "");
    
    void onError(AssetsDownloader::ErrorCode eErrorCode);
    void onSuccess();
    std::string errorCode(AssetsDownloader::ErrorCode eErrorCode);
    
    AssetsDownloader();
    ~AssetsDownloader();
    
private:
    std::mutex m_pCurlMutex;
    std::mutex m_pFileMutex;
    CURL *m_pCurl;
    FILE *m_pFile;
    bool m_bStop;
    double m_fTotal;
    double m_fDownloaded;
    
    std::vector<char>   m_oResponseData;
    int                 m_nErrorCode;
    long                m_nResponseCode;
    int                 m_nTag;
    std::string         m_sUserData;
    std::string         m_sErrorCode;
    ProgressCallback    m_pOnProgress;
    SuccessCallback     m_pOnSuccess;
    ErrorCallback       m_pOnError;
    cocos2d::ScriptHandlerMgr::HandlerType  m_eScriptHandler;
};
}

#endif /* AssetsDownloader_h */
