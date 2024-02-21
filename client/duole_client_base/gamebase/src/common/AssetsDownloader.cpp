//
//  AssetsDownloader.cpp
//  ZhuoJiMJ
//
//  Created by weiran on 2017/8/26.
//
//

#include "AssetsDownloader.h"
#include "unzip/unzip.h"
#include <sys/stat.h>
#include <regex>

USING_NS_CC;
using namespace std;

#define ZIP_EXT     ".zip"

#define BUFFER_SIZE         8192
#define MAX_FILENAME        512
#define DEFAULT_TIMEOUT     5
#define LOW_SPEED_LIMIT     1L
#define LOW_SPEED_TIME      5L
#define INVALID_TAG         -1

namespace duole {

size_t fileWriteFunc(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    std::vector<char> *recvBuffer = (std::vector<char>*)userdata;
    size_t sizes = size * nmemb;
    
    // add data to the end of recvBuffer
    // write data maybe called more than once in a single request
    recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+sizes);
    
    return sizes;
}

int downloadProgressFunc(AssetsDownloader *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
    if (ptr->isStop())
    {
        log("ProgressFunc >>> %f/%f", nowDownloaded, totalToDownload);
        return -2;
    }

    if (totalToDownload < 0 || nowDownloaded < 0)
    {
        return 0;
    }
    
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]{
        if (ptr && ptr->getProgressCallback())
        {
            ptr->getProgressCallback()(totalToDownload, nowDownloaded, ptr->getTag(), ptr);
        }
        ptr->onProgress(totalToDownload, nowDownloaded);
    });
    
    return 0;
}

AssetsDownloader* AssetsDownloader::create()
{
    AssetsDownloader* pRet = new AssetsDownloader();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool AssetsDownloader::init()
{
    return true;
}

void AssetsDownloader::downloadAsync(const std::string& sUrl, const std::string& sFilePath)
{
    if (sUrl.empty())
    {
        onError(ErrorCode::kErrorUrl);
        return;
    }
    
    string sImgExt = getExtension(sUrl);
    if (sImgExt.empty())
    {
        onError(ErrorCode::kErrorUrl);
        return;
    }
    
    auto t = std::thread(&AssetsDownloader::download, this, sUrl, sFilePath);
    t.detach();
}

void AssetsDownloader::unzipResAsync(const std::string& sSourceFile, const std::string& sFolderName, const std::string& sPassword)
{
    if (sSourceFile.empty())
    {
        onError(ErrorCode::kErrorUrl);
        return;
    }
    auto t = std::thread(&AssetsDownloader::unzipRes, this, sSourceFile, sFolderName, sPassword);
    t.detach();
}

void AssetsDownloader::unzipRes(const std::string& sSourceFile, const std::string& sFolderName, const std::string& sPassword)
{
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(sSourceFile);
    ssize_t len = 0;
    unsigned char* pData = FileUtils::getInstance()->getFileData(fullPath, "r", &len);
    string sDirFile = FileUtils::getInstance()->getWritablePath() + sFolderName + sSourceFile;
    
    FileUtils::getInstance()->createDirectory(FileUtils::getInstance()->getWritablePath() + sFolderName);
    FILE* fp = fopen(sDirFile.c_str(), "wb+");
    if(!fp)
    {
        log("file not found! %s", sDirFile.c_str());
    }
    fwrite(pData, sizeof(char), len, fp);
    fclose(fp);
    free(pData);
    pData = nullptr;
    
    decompressDownLoadZip(sDirFile, sPassword);
}

void AssetsDownloader::download(const std::string& sUrl, const std::string& sFilePath)
{
    m_bStop = false;
    bool bSuccess = processDownloadTask(sUrl);
    if (bSuccess)
    {
        std::vector<char> *pBuffer = &m_oResponseData;
        char* pData = (char*)malloc(pBuffer->size());
        std::copy(pBuffer->begin(), pBuffer->end(), pData);
        
        std::string sSaveDir = "", sExt = "";
        unsigned long found = sFilePath.find_last_of("/\\");
        if (found != std::string::npos)
        {
            sExt = getExtension(sFilePath);
            sSaveDir = sFilePath.substr(0, found+1);
        }
        else
        {
            onError(ErrorCode::kErrorDownLoad);
            return;
        }
        
        FileUtils::getInstance()->createDirectory(sSaveDir);
        
        m_pFile = fopen(sFilePath.c_str(), "wb+");
        
        if (m_pFile)
        {
            fwrite(pData, 1, pBuffer->size(), m_pFile);
            fclose(m_pFile);
        }
        
        free (pData);
        pData = nullptr;
        
        if (sExt == ZIP_EXT)
        {
            decompressDownLoadZip(sFilePath);
        }
        else
        {
            onSuccess();
        }
    }
    else
    {
        onError(ErrorCode::kErrorDownLoad);
    }
}

bool AssetsDownloader::processDownloadTask(const string& sPackageUrl)
{
    m_pCurl = curl_easy_init();
    if (!m_pCurl)
    {
        return false;
    }
    // Download pacakge
    curl_easy_setopt(m_pCurl, CURLOPT_URL, sPackageUrl.c_str());
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, fileWriteFunc);
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, &m_oResponseData);
    curl_easy_setopt(m_pCurl, CURLOPT_NOPROGRESS, false);
    curl_easy_setopt(m_pCurl, CURLOPT_PROGRESSFUNCTION, downloadProgressFunc);
    curl_easy_setopt(m_pCurl, CURLOPT_PROGRESSDATA, this);
    curl_easy_setopt(m_pCurl, CURLOPT_FAILONERROR, true);
    curl_easy_setopt(m_pCurl, CURLOPT_CONNECTTIMEOUT, DEFAULT_TIMEOUT);
    curl_easy_setopt(m_pCurl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(m_pCurl, CURLOPT_LOW_SPEED_LIMIT, LOW_SPEED_LIMIT);
    curl_easy_setopt(m_pCurl, CURLOPT_LOW_SPEED_TIME, LOW_SPEED_TIME);
    curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 0L);
    
    CURLcode res = curl_easy_perform(m_pCurl);
    if (res != CURLE_OK)
    {
        m_nErrorCode = res;
        CCLOGERROR("Curl curl_easy_perform error: %d", res);
        return false;
    }
    
    CURLcode code = curl_easy_getinfo(m_pCurl, CURLINFO_RESPONSE_CODE, &m_nResponseCode);
    if (code != CURLE_OK || !(m_nResponseCode >= 200 && m_nResponseCode < 300)) {
        CCLOGERROR("Curl curl_easy_getinfo failed: %s", curl_easy_strerror(code));
        return false;
    }
    
    curl_easy_cleanup(m_pCurl);
    
    return true;
}

void AssetsDownloader::setProgressCallback(const ProgressCallback& pCallback)
{
    m_pOnProgress = pCallback;
}

AssetsDownloader::ProgressCallback AssetsDownloader::getProgressCallback()
{
    return m_pOnProgress;
}

void AssetsDownloader::setSuccessCallback(const SuccessCallback& pCallback)
{
    m_pOnSuccess = pCallback;
}

AssetsDownloader::SuccessCallback AssetsDownloader::getSuccessCallback()
{
    return m_pOnSuccess;
}

void AssetsDownloader::setErrorCallback(const AssetsDownloader::ErrorCallback& pCallback)
{
    m_pOnError = pCallback;
}

AssetsDownloader::ErrorCallback AssetsDownloader::getErrorCallback()
{
    return m_pOnError;
}

void AssetsDownloader::setTag(int nTag)
{
    m_nTag = nTag;
}

int AssetsDownloader::getTag()
{
    return m_nTag;
}

void AssetsDownloader::setUserData(const string& sUserData)
{
    m_sUserData = sUserData;
}

string AssetsDownloader::getUserData()
{
    return m_sUserData;
}

void AssetsDownloader::setStop(bool bStop)
{
    m_bStop = bStop;
}

bool AssetsDownloader::isStop()
{
    return m_bStop;
}

void AssetsDownloader::decompressDownLoadZip(const string& sFileName, const string& sPassword)
{
    log("decompressZip >>>");
    if (decompress(sFileName, sPassword))
    {
        FileUtils::getInstance()->removeFile(sFileName);
        onSuccess();
    }
    else
    {
        onError(ErrorCode::kErrorDecompress);
    }
}

void AssetsDownloader::onError(AssetsDownloader::ErrorCode eErrorCode)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]{
        if (m_pOnError)
        {
            m_pOnError(eErrorCode, m_nTag, this);
        }
        
        m_sErrorCode = errorCode(eErrorCode);
        int handler = cocos2d::ScriptHandlerMgr::getInstance()->getObjectHandler((void*)this, m_eScriptHandler);
        if(handler != 0)
        {
            CommonScriptData data(handler, "error", this);
            ScriptEvent event(cocos2d::ScriptEventType::kCommonEvent, (void*)&data);
            ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
        }
    });
}

void AssetsDownloader::onSuccess()
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]{
        if (m_pOnSuccess)
        {
            m_pOnSuccess(m_nTag, this);
        }
        int handler = cocos2d::ScriptHandlerMgr::getInstance()->getObjectHandler((void*)this, m_eScriptHandler);
        if(handler != 0)
        {
            CommonScriptData data(handler, "success", this);
            ScriptEvent event(cocos2d::ScriptEventType::kCommonEvent, (void*)&data);
            ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
        }
    });
}

void AssetsDownloader::onProgress(double fTotal, double fCur)
{
    m_fTotal = fTotal;
    m_fDownloaded = fCur;
    
    int handler = cocos2d::ScriptHandlerMgr::getInstance()->getObjectHandler((void*)this, m_eScriptHandler);
    if(handler != 0)
    {
        CommonScriptData data(handler, "progress", this);
        ScriptEvent event(cocos2d::ScriptEventType::kCommonEvent, (void*)&data);
        ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
    }
}

string AssetsDownloader::errorCode(AssetsDownloader::ErrorCode eErrorCode)
{
    string sErrorCode = "";
    switch (eErrorCode)
    {
        case ErrorCode::kErrorDownLoad:
            sErrorCode = "kErrorDownLoad";
            break;
        case ErrorCode::kErrorDecompress:
            sErrorCode = "kErrorDecompress";
            break;
        case ErrorCode::kErrorUrl:
            sErrorCode = "kErrorUrl";
            break;
        default:
            break;
    }
    return sErrorCode;
}

bool AssetsDownloader::decompress(const string& sFileName, const string& sPassword)
{
    // Find root path for zip file
    size_t pos = sFileName.find_last_of("/\\");
    if (pos == std::string::npos)
    {
        return false;
    }
    const std::string rootPath = sFileName.substr(0, pos+1);
    
    // Open the zip file
    unzFile zipfile = unzOpen(sFileName.c_str());
    if (! zipfile)
    {
        return false;
    }
    
    // Get info about the zip file
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        unzClose(zipfile);
        return false;
    }
    
    // Buffer to hold data read from the zip file
    char readBuffer[BUFFER_SIZE];
    // Loop to extract all files.
    uLong i;
    
    for (i = 0; i < global_info.number_entry; ++i)
    {
        // Get info about current file.
        unz_file_info fileInfo;
        char fileName[MAX_FILENAME];
        if (unzGetCurrentFileInfo(zipfile,
                                  &fileInfo,
                                  fileName,
                                  MAX_FILENAME,
                                  NULL,
                                  0,
                                  NULL,
                                  0) != UNZ_OK)
        {
            unzClose(zipfile);
            return false;
        }
        const std::string fullPath = rootPath + fileName;
        
        // Check if this entry is a directory or a file.
        const size_t filenameLength = strlen(fileName);
        if (fileName[filenameLength-1] == '/')
        {
            //There are not directory entry in some case.
            //So we need to create directory when decompressing file entry
            if ( !FileUtils::getInstance()->createDirectory(basename(fullPath)) )
            {
                // Failed to create directory
                unzClose(zipfile);
                return false;
            }
        }
        else
        {
            // Entry is a file, so extract it.
            // Open current file.
            const char* pPassword = NULL;
            if (!sPassword.empty())
            {
                pPassword = sPassword.c_str();
            }
            if (unzOpenCurrentFilePassword(zipfile, pPassword) != UNZ_OK)
            {
                unzClose(zipfile);
                return false;
            }
            
            // Create a file to store current file.
            FILE *out = fopen(fullPath.c_str(), "wb");
            if (!out)
            {
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                return false;
            }
            
            // Write current file content to destinate file.
            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
                if (error < 0)
                {
                    fclose(out);
                    unzCloseCurrentFile(zipfile);
                    unzClose(zipfile);
                    return false;
                }
                
                if (error > 0)
                {
                    fwrite(readBuffer, error, 1, out);
                }
            } while(error > 0);
            
            fclose(out);
        }
        
        unzCloseCurrentFile(zipfile);
        
        // Goto next entry listed in the zip file.
        if ((i+1) < global_info.number_entry)
        {
            if (unzGoToNextFile(zipfile) != UNZ_OK)
            {
                unzClose(zipfile);
                return false;
            }
        }
    }
    
    unzClose(zipfile);
    return true;
}

string AssetsDownloader::basename(const string& sPath)
{
    size_t found = sPath.find_last_of("/\\");
    
    if (std::string::npos != found)
    {
        return sPath.substr(0, found);
    }
    else
    {
        return sPath;
    }
}

string AssetsDownloader::getExtension(const string& sString)
{
    string sRet = "";
    const char* pDot = strrchr(sString.c_str(), '.');
    if (pDot)
    {
        const char* pInvalid = strrchr(pDot, '/');
        if (nullptr == pInvalid)
        {
            sRet = pDot;
        }
    }
    return sRet;
}

void AssetsDownloader::setScriptHandler(cocos2d::ScriptHandlerMgr::HandlerType eType)
{
    m_eScriptHandler = eType;
}

cocos2d::ScriptHandlerMgr::HandlerType AssetsDownloader::getScriptHandler()
{
    return m_eScriptHandler;
}

double AssetsDownloader::getProgressTotal()
{
    return m_fTotal;
}

double AssetsDownloader::getProgressCur()
{
    return m_fDownloaded;
}

string AssetsDownloader::getErrorCode()
{
    return m_sErrorCode;
}

AssetsDownloader::AssetsDownloader()
:
m_pOnProgress(nullptr),
m_pOnSuccess(nullptr),
m_pOnError(nullptr),
m_oResponseData({}),
m_nErrorCode(0),
m_nResponseCode(0),
m_nTag(INVALID_TAG),
m_sUserData(""),
m_sErrorCode(""),
m_pCurl(nullptr),
m_pFile(nullptr),
m_bStop(true),
m_eScriptHandler(cocos2d::ScriptHandlerMgr::HandlerType::NODE),
m_fTotal(0.0f),
m_fDownloaded(0.0f)
{}

AssetsDownloader::~AssetsDownloader()
{
    m_pOnProgress = nullptr;
    m_pOnSuccess = nullptr;
    m_pOnError = nullptr;
    m_oResponseData = {};
    m_nErrorCode = 0;
    m_nTag = INVALID_TAG;
    m_sUserData = "";
    m_sErrorCode = "";
    m_pCurl = nullptr;
    m_pFile = nullptr;
    m_bStop = true;
}

}
