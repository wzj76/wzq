//
//  XXTeaCrypto.cpp
//  gamebase
//
//  Created by 张建民 on 2022/2/24.
//

#include "XXTeaCrypto.hpp"
#include "xxtea/xxtea.h"
#include "platform/CCFileUtils.h"
using namespace cocos2d;
using namespace std;

namespace duole {

XXTeaCrypto::XXTeaCrypto() {
    _xxteaKey = nullptr;
    _xxteaKeyLen = 0;
    _xxteaSign = nullptr;
    _xxteaSignLen = 0;
}
XXTeaCrypto::~XXTeaCrypto() {
    
}
void XXTeaCrypto::setXXTEAKeyAndSign(const char *key, int keyLen, const char *sign, int signLen) {
    cleanupXXTEAKeyAndSign();

    if (key && keyLen )
    {
        _xxteaKey = (char *)malloc(keyLen);
        memcpy(_xxteaKey, key, keyLen);
        _xxteaKeyLen = keyLen;

        if (sign && signLen) {
            _xxteaSign = (char *)malloc(signLen);
            memcpy(_xxteaSign, sign, signLen);
            _xxteaSignLen = signLen;
        }
    }
}

void XXTeaCrypto::cleanupXXTEAKeyAndSign()
{
    if (_xxteaKey)
    {
        free(_xxteaKey);
        _xxteaKey = nullptr;
        _xxteaKeyLen = 0;
    }
    if (_xxteaSign)
    {
        free(_xxteaSign);
        _xxteaSign = nullptr;
        _xxteaSignLen = 0;
    }
}

unsigned char *XXTeaCrypto::xxteaEncrypt(const std::string &srcFile, ssize_t *size) {
    unsigned char *pRetData = nullptr;
    *size = 0;
    FileUtils *pUtils = FileUtils::getInstance();
    
    if (pUtils->isFileExist(srcFile))
    {
        Data fileData(pUtils->getDataFromFile(srcFile));
        unsigned char *pFileData = fileData.getBytes();
        ssize_t srcFileSize = fileData.getSize();
        if (pFileData)
        {
            xxtea_long nLen = 0;
            pRetData = xxtea_encrypt(pFileData,
                                 (xxtea_long)srcFileSize,
                                 (unsigned char*)_xxteaKey,
                                 (xxtea_long)_xxteaKeyLen,
                                 &nLen);
            *size = nLen;
//            free(pFileData);
//            pFileData = nullptr;
        }
    }
    return pRetData;
}
bool XXTeaCrypto::xxteaEncrypt(const std::string &srcFile, const std::string &destFile) {
    bool bRet = false;
    ssize_t nSize = 0;
    FileUtils *pUtils = FileUtils::getInstance();
    unsigned char *pFileData = xxteaEncrypt(srcFile, &nSize);
    if (pFileData)
    {
        string sDstPath = pUtils->getWritablePath() + destFile;
        FILE *pFile = fopen(sDstPath.c_str(), "wb+");
        if (pFile) {
            //  如果设置了签名，需要在文件首部写入签名；
            if (_xxteaSign && _xxteaSignLen > 0) {
                fwrite(_xxteaSign, sizeof(char), _xxteaSignLen, pFile);
            }
            fwrite(pFileData, sizeof(char), nSize, pFile);
            fclose(pFile);
            bRet = true;
        }
//        free(pFileData);
//        pFileData = nullptr;
    }
    return bRet;        
}
    

unsigned char *XXTeaCrypto::xxteaDecrypt(const std::string &srcFile, ssize_t *size) {
    unsigned char *pRetData = nullptr;
    *size = 0;
    FileUtils *pUtils = FileUtils::getInstance();
    if (pUtils->isFileExist(srcFile))
    {
        do {
            Data fileData(pUtils->getDataFromFile(srcFile));
            unsigned char *pFileData = fileData.getBytes();
            ssize_t srcFileSize = fileData.getSize();

            //  核对签名: 可以不签名；
            bool isXXTEA = srcFileSize >= _xxteaSignLen
                           && memcmp(_xxteaSign, pFileData, _xxteaSignLen) == 0;
            //  解密
            if (isXXTEA) {
                xxtea_long nLen = 0;
                pRetData = xxtea_decrypt(pFileData + _xxteaSignLen,
                                       (xxtea_long)srcFileSize - (xxtea_long)_xxteaSignLen,
                                       (unsigned char*)_xxteaKey,
                                       (xxtea_long)_xxteaKeyLen,
                                       &nLen);
                *size = nLen;
            }
            
//            if (pFileData) {
//                free(pFileData);
//                pFileData = nullptr;
//            }
            
        } while (0);
    }

    return pRetData;
}
    
bool XXTeaCrypto::xxteaDecrypt(const std::string &srcFile, const std::string &destFile) {
    bool bRet = false;
    ssize_t nSize = 0;
    FileUtils *pUtils = FileUtils::getInstance();
    unsigned char *pFileData = xxteaDecrypt(srcFile, &nSize);
    if (pFileData)
    {
        string sDstPath = pUtils->getWritablePath() + destFile;
        FILE *pFile = fopen(sDstPath.c_str(), "wb+");
        if (pFile) {
            fwrite(pFileData, sizeof(char), nSize, pFile);
            fclose(pFile);
            bRet = true;
        }
        free(pFileData);
        pFileData = nullptr;
    }
    return bRet;
}
}
