//
//  AppConfig.hpp
//  gamebase
//
//  Created by 张建民 on 2022/2/22.
//

/**
 多乐客户端基础信息配置文件读取工具类，配置文件需以密文形式放入资源包；
  
 示例:
 {
     "app_name": "多乐保皇",
     "channel": "duole",
     "channel_group": "duole",
     "app_version": "1.0.0",
     "clientbase_version": "1.0.0_beta4",
     "launcher_version": "1.1.0_beta2" ,
     "unit_test_obj" : {
         "obj_value1" : 1,
         "obj_value2" : 2
     },
     "unit_test_ary": [
         "ary_value1",
         "ary_value2",
         "ary_value3"
     ]
 }
 Usage:
 // 读取配置信息
 std::string _appname = APPCONFIG.getString("app_name");
 std::string _channel = APPCONFIG.getString("channel");
 std::string _channel_group = APPCONFIG.getString("channel_group");
 std::string _app_version = APPCONFIG.getString("app_version");
 std::string _clientbase_version = APPCONFIG.getString("clientbase_version");
 std::string _launcher_version = APPCONFIG.getString("launcher_version");
 // 获取 Document对象自行解析；
 rapidjson::Document &dom = APPCONFIG.getJsonDocument();
 _appname = duole::readJsonValueString(dom, "app_name");
 // 对象及数组；
 const rapidjson::Value &obj = APPCONFIG.getObject("unit_test_obj");
 const rapidjson::Value &ary = APPCONFIG.getObject("unit_test_ary");
 int obj_value1 = duole::readJsonValueInt32(obj, "obj_value1");
 std::string ary_value1;
 if (ary.IsArray()) {
    ary_value1 = ary[2].GetString();
 }

 */
#ifndef AppConfig_hpp
#define AppConfig_hpp

#include <stdio.h>
#include "JsonParser.h"
#include "Singleton.h"
#include "XXTeaCrypto.hpp"

namespace duole {

class AppConfig {
public:
    AppConfig();
    ~AppConfig();
public:
    inline int32_t getInt32(const char * name) { return readJsonValueInt32(_objJson, name); };
    inline uint32_t getUint32(const char * name) { return readJsonValueUint32(_objJson, name); };
    inline int64_t getInt64(const char * name) { return readJsonValueInt64(_objJson, name); };
    inline uint64_t getUint64(const char * name) { return readJsonValueUint64(_objJson, name); };
    inline double getDouble(const char * name) { return readJsonValueDouble(_objJson, name); };
    inline bool getBool(const char * name) { return readJsonValueBool(_objJson, name); };
    inline std::string getString(const char * name) { return readJsonValueString(_objJson, name); };

    inline rapidjson::Document &getJsonDocument() { return _objJson; };
    inline const rapidjson::Value &getObject(const char * name) { return readJsonValueObject(_objJson, name); };
    inline const rapidjson::Value &getArray(const char * name) { return readJsonValueArray(_objJson, name); };
    //  打印配置文件
    void print();
private:
    /// 初始化配置管理模块，可指定文件及加密参数；
    /// @param file 文件名
    /// @param key 秘钥
    /// @param keyLen 秘钥长度
    /// @param sign 签名串
    /// @param signLen 签名长度
    void init(const char *file, const char *key, int keyLen, const char *sign = nullptr, int signLen = 0);
    // 加载和解析配置文件；
    void loadConfigFile(const std::string &file);
private:
    rapidjson::Document _objJson;
    XXTeaCrypto _crypto;
    bool _ready;  //  初始化完毕
};

}
//  AppConfig 单例对象；
#define APPCONFIG duole::CSingleton<duole::AppConfig>::Instance()

#endif /* AppConfig_hpp */
