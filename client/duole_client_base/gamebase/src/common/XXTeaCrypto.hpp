//
//  XXTeaCrypto.hpp
//  gamebase
//
//  Created by 张建民 on 2022/2/24.
//
/**
 多乐加密解密工具类(支持签名)；
 Usage:
 1.加密模块本地化使用：
 XXTeaCrypto _crypto;
 _crypto.setXXTEAKeyAndSign("1qa2ws3ed", strlen("1qa2ws3ed"), "duolegame", strlen("duolegame"));
 _crypto.xxteaDecrypt(srcFile, &nSize);
 
 2.单例模式
 XXTEACRYPTO.setXXTEAKeyAndSign("1qa2ws3ed", strlen("1qa2ws3ed"), "duolegame", strlen("duolegame"));
 XXTEACRYPTO.xxteaDecrypt(srcFile, &nSize);
 
 */

#ifndef XXTeaCrypto_hpp
#define XXTeaCrypto_hpp

#include <stdio.h>
#include <string>

namespace duole {

class XXTeaCrypto {
public:
    XXTeaCrypto();
    ~XXTeaCrypto();
        
    void setXXTEAKeyAndSign(const char *key, int keyLen, const char *sign = nullptr, int signLen = 0);
    
    /// 对文件进行 xxtea 加密并返回密文数据块（注意：需要手动free掉返回内容）
    /// @param srcFile 待加密文件
    /// @param size 密文长度，（不包含签名数据）
    unsigned char *xxteaEncrypt(const std::string &srcFile, ssize_t *size);
        
    /// 对文件进行 xxtea 加密，密文写入指定文件；
    /// @param srcFile 待加密文件
    /// @param destFile 加密后的文件（包含签名）
    bool xxteaEncrypt(const std::string &srcFile, const std::string &destFile);
        
    /// 对文件进行 xxtea 解密并返回明文数据块（注意：需要手动free掉返回内容）
    /// @param srcFile 待解密文件
    /// @param size 解密后的数据块长度（不包含签名）
    unsigned char *xxteaDecrypt(const std::string &srcFile, ssize_t *size);
        
    /// 对文件进行 xxtea 解密，明文写入指定文件；
    /// @param srcFile 待解密文件
    /// @param destFile 解密后的文件（不包含签名）
    bool xxteaDecrypt(const std::string &srcFile, const std::string &destFile);
    
private:
    void cleanupXXTEAKeyAndSign();
private:
    char* _xxteaKey;
    int   _xxteaKeyLen;
    char* _xxteaSign;
    int   _xxteaSignLen;
};
}
//  XXTeaCrypto 单例对象；
#define XXTEACRYPTO duole::CSingleton<duole::XXTeaCrypto>::Instance()
#endif /* XXTeaCrypto_hpp */
