//
//  AppConfig.cpp
//  gamebase
//
//  Created by 张建民 on 2022/2/22.
//

#include "AppConfig.hpp"
#include "platform/CCFileUtils.h"

using namespace cocos2d;
//using namespace rapidjson;
namespace duole {

// 默认的配置文件名；
#define FILE_NAME_APPCONFIG "appconfig.dat"
// 秘钥和签名；
const char g_xxtea_key[] = "zhaoxin";
const char g_xxtea_sign[] = "7k7kgame";

AppConfig::AppConfig() {
    _ready = false;
    init(FILE_NAME_APPCONFIG, g_xxtea_key, strlen(g_xxtea_key), g_xxtea_sign, strlen(g_xxtea_sign));
}
AppConfig::~AppConfig() {
}

void AppConfig::init(const char *file, const char *key, int keyLen, const char *sign , int signLen) {
    XXTEACRYPTO.setXXTEAKeyAndSign(key, keyLen, sign, signLen);
    std::string srcFile = file;
    loadConfigFile(srcFile);
}

void AppConfig::loadConfigFile(const std::string &file) {
    ssize_t nSize = 0;
    unsigned char *pJsonData = XXTEACRYPTO.xxteaDecrypt(file, &nSize);
    if (pJsonData) {
        _objJson.Parse<rapidjson::kParseDefaultFlags>((char *)pJsonData);
        free(pJsonData);
        pJsonData = nullptr;
        if (_objJson.HasParseError()) {
            log("AppConfig: loadConfigFile failed, _objJson.Parse failed!");
            return;
        }
        _ready = true;
    } else {
        log("AppConfig: loadConfigFile failed, pJsonFile is nullptr!");
    }
}

void AppConfig::print() {
    if (_ready && !_objJson.HasParseError()) {
        log("{");
        for (rapidjson::Value::ConstMemberIterator itr = _objJson.MemberBegin(); itr != _objJson.MemberEnd(); itr++) {
            rapidjson::Value jKey;
            rapidjson::Value jValue;
            rapidjson::Document::AllocatorType allocator;
            jKey.CopyFrom(itr->name, allocator);
            jValue.CopyFrom(itr->value, allocator);
            if (jKey.IsString()) {
                std::string sKey = jKey.GetString();
                if (jValue.IsString()) {
                    log("  %s: %s", sKey.c_str(), jValue.GetString());
                } else if (jValue.IsInt()) {
                    log("  %s: %d", sKey.c_str(), jValue.GetInt());
                }
            }
        }
        log("}");
    }
}

}

