//
//  PHZEncodeDecodeUtil.cpp
//  BaoHuang
//
//  Created by 姚佳 on 2021/7/23.
//

#include "EncodeDecodeUtil.h"
#include "System/Base64.h"
#include "System/md5.h"
#include "xxtea/xxtea.h"

USING_NS_CC;
using namespace std;

namespace util
{
    string getMd5Encode(const string &sData)
    {
        return MD5(sData).toString();
    }

    string getBase64Encode(const string &sData)
    {
        string sRet = "";
        char* encoded = nullptr;
        base64Encode((const unsigned char *)sData.c_str(), (unsigned int)sData.size(), &encoded);
        sRet = encoded;
        free(encoded);
        return sRet;
    }

    string getBase64Decode(const string &sData)
    {
        unsigned char *sDecodedData = nullptr;
        int nLength = base64Decode((const unsigned char *)sData.c_str(), (unsigned int)sData.size(), &sDecodedData);
        string sRet(sDecodedData, sDecodedData + nLength);
        free(sDecodedData);
        return sRet;
    }

    string getBase64UrlEncode(const string &sData)
    {
        return CBase64::UrlEncode(sData);
    }

    string getBase64UrlDecode(const string &sData)
    {
        return CBase64::UrlDecode(sData);
    }

    string getXorEncode(const string &sData, const string &sKey)
    {
        string sRet = sData;
        size_t nKeyLen = sKey.size();
        for (size_t i = 0; i < sRet.size(); i ++)
        {
            sRet[i] = sRet[i] ^ sKey[i % nKeyLen];
        }
        return sRet;
    }

    string getRot47Encode(const string &sData)
    {
        string sRet = sData;
        for (size_t i = 0; i < sRet.length(); i ++)
        {
            char ch = sRet.at(i);
            if (ch >= 33 && ch <= 126)
            {
                ch = 33 + (ch + 14) % 94;
                sRet.at(i) = ch;
            }
        }
        return sRet;
    }

    string getCustomEncodedUrl(const string &sUrl)
    {
        string sRet = sUrl;
        string sSeparator = "?";
        size_t nIndex = sUrl.find(sSeparator);
        if (nIndex != string::npos)
        {
            string sParams = sUrl.substr(nIndex + 1);
            sRet = sUrl.substr(0, nIndex);
            sRet.append(sSeparator);
            sRet.append("k=");
            sRet.append(getBase64UrlEncode(sParams));
        }
        return sRet;
    }

    unsigned char* getFileDataXxteaEncrypt(const string &sFilePath, const string &sKey, ssize_t* pSize)
    {
        unsigned char* pRet = nullptr;
        if (!sKey.empty())
        {
            size_t nXxteaKeyLen = sKey.size();
            FileUtils* pUtils = FileUtils::getInstance();
            if (pUtils->isFileExist(sFilePath))
            {
                ssize_t nSize = 0;
                unsigned char *pFileData = pUtils->getFileData(sFilePath.c_str(), "rb", &nSize);
                if (pFileData)
                {
                    xxtea_long nLen = 0;
                    pRet = xxtea_encrypt(pFileData,
                                         (xxtea_long)nSize,
                                         (unsigned char*)sKey.c_str(),
                                         (xxtea_long)nXxteaKeyLen,
                                         &nLen);
                    *pSize = nLen;
                    free(pFileData);
                    pFileData = nullptr;
                }
            }
        }
        return pRet;
    }

    unsigned char* getFileDataXxteaDecrypt(const string &sFilePath, const string &sKey, ssize_t* pSize)
    {
        unsigned char* pRet = nullptr;
        if (!sKey.empty())
        {
            size_t nXxteaKeyLen = sKey.size();
            FileUtils* pUtils = FileUtils::getInstance();
            if (pUtils->isFileExist(sFilePath))
            {
                ssize_t nSize = 0;
                unsigned char *pFileData = pUtils->getFileData(sFilePath.c_str(), "rb", &nSize);
                if (pFileData)
                {
                    xxtea_long nLen = 0;
                    pRet = xxtea_decrypt(pFileData,
                                         (xxtea_long)nSize,
                                         (unsigned char*)sKey.c_str(),
                                         (xxtea_long)nXxteaKeyLen,
                                         &nLen);
                    *pSize = nLen;
                    free(pFileData);
                    pFileData = nullptr;
                }
            }
        }
        return pRet;
    }

    void generateXxteaEncryptFile(const string &sSrcFile, const string &sFileName, const string &sKey)
    {
        ssize_t nSize = 0;
        FileUtils* pUtils = FileUtils::getInstance();
        string sFullPath = pUtils->fullPathForFilename(sSrcFile);
        unsigned char* pFileData = getFileDataXxteaEncrypt(sFullPath, sKey, &nSize);
        if (pFileData)
        {
            
            string sDstPath = pUtils->getWritablePath() + sFileName;
            FILE* pFile = fopen(sDstPath.c_str(), "wb+");
            if (!pFile)
            {
                log("Generate XXTea file[%s] failed.", sFileName.c_str());
            }
            else
            {
                fwrite(pFileData, sizeof(char), nSize, pFile);
                fclose(pFile);
            }
            free(pFileData);
        }
        pFileData = nullptr;
    }

    void generateXxteaDecryptFile(const string &sSrcFile, const string &sFileName, const string &sKey)
    {
        ssize_t nSize = 0;
        FileUtils* pUtils = FileUtils::getInstance();
        string sFullPath = pUtils->fullPathForFilename(sSrcFile);
        unsigned char* pFileData = getFileDataXxteaDecrypt(sFullPath, sKey, &nSize);
        if (pFileData)
        {
            
            string sDstPath = pUtils->getWritablePath() + sFileName;
            FILE* pFile = fopen(sDstPath.c_str(), "wb+");
            if (!pFile)
            {
                log("Generate XXTea file[%s] failed.", sFileName.c_str());
            }
            else
            {
                fwrite(pFileData, sizeof(char), nSize, pFile);
                fclose(pFile);
            }
            free(pFileData);
        }
        pFileData = nullptr;
    }
}
