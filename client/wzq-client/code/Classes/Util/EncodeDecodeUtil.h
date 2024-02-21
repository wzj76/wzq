//
//  PHZEncodeDecodeUtil.h
//  BaoHuang
//
//  Created by 姚佳 on 2021/7/23.
//

#ifndef PHZEncodeDecodeUtil_h
#define PHZEncodeDecodeUtil_h

#include "System/md5.h"
#include <string>
#include "cocos2d.h"

namespace util
{
    // 获取Md5加密字符串
    std::string getMd5Encode(const std::string &sData);
    // 获取Base64加密字符串
    std::string getBase64Encode(const std::string &sData);
    // 获取Base64解密字符串
    std::string getBase64Decode(const std::string &sData);
    // 获取Base64Url加密字符串
    std::string getBase64UrlEncode(const std::string &sData);
    // 获取Base64Url解密字符串
    std::string getBase64UrlDecode(const std::string &sData);
    // 获取Xor加密字符串
    std::string getXorEncode(const std::string &sData, const std::string &sKey);
    // 获取Rot47加密字符串
    std::string getRot47Encode(const std::string &sData);
    // 获取自定义加密URL
    std::string getCustomEncodedUrl(const std::string &sUrl);

    // 获取文件Xxtea加密后数据（需要手动free掉返回内容）
    unsigned char* getFileDataXxteaEncrypt(const std::string &sFilePath, const std::string &sKey, ssize_t* pSize);
    // 获取文件Xxtea解密后数据（需要手动free掉返回内容）
    unsigned char* getFileDataXxteaDecrypt(const std::string &sFilePath, const std::string &sKey, ssize_t* pSize);
    // 生成Xxtea加密文件
    void generateXxteaEncryptFile(const std::string &sSrcFile, const std::string &sFileName, const std::string &sKey);
    // 生成Xxtea解密文件
    void generateXxteaDecryptFile(const std::string &sSrcFile, const std::string &sFileName, const std::string &sKey);
}

#endif /* PHZEncodeDecodeUtil_h */
