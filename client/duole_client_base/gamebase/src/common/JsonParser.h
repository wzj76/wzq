//
//  JsonParser.h
//  gamebase
//
//  Created by zhangxu on 2017/8/23.
//
//

#ifndef JsonParser_h
#define JsonParser_h

#include <string>
#include <functional>
#include "json/document.h"

namespace duole {

// --------------------
// 快捷读取Json数据
// --------------------
#define RAPIDJSON_READ_FUNC(_FUNC_NAME_, _NODE_TYPE_, _VALUE_TYPE_, _VALUE_METHOD_, _VALUE_DEFAULT_)\
inline _VALUE_TYPE_ _FUNC_NAME_(const _NODE_TYPE_ &node, const char* name, _VALUE_TYPE_ defaultValue = _VALUE_DEFAULT_)\
{\
return node.HasMember(name) ? node[name]._VALUE_METHOD_() : defaultValue;\
}

static const rapidjson::Value RAPIDJSON_DEFAULT_OBJECT(rapidjson::kObjectType);
#define RAPIDJSON_READ_FUNC_OBJECT(_FUNC_NAME_, _NODE_TYPE_)\
inline const rapidjson::Value &_FUNC_NAME_(const _NODE_TYPE_ &node, const char* name)\
{\
    return node.HasMember(name)? node[name]: RAPIDJSON_DEFAULT_OBJECT;\
}

static const rapidjson::Value RAPIDJSON_DEFAULT_ARRAY(rapidjson::kArrayType);
#define RAPIDJSON_READ_FUNC_ARRAY(_FUNC_NAME_, _NODE_TYPE_)\
inline const rapidjson::Value &_FUNC_NAME_(const _NODE_TYPE_ &node, const char* name)\
{\
    return node.HasMember(name)? node[name]: RAPIDJSON_DEFAULT_ARRAY;\
}

// 读取Json数据类型int
RAPIDJSON_READ_FUNC(readJsonValueInt32, rapidjson::Document, int32_t, GetInt, 0);
RAPIDJSON_READ_FUNC(readJsonValueInt32, rapidjson::Value, int32_t, GetInt, 0);

// 读取Json数据类型unsigned int
RAPIDJSON_READ_FUNC(readJsonValueUint32, rapidjson::Document, uint32_t, GetUint, 0);
RAPIDJSON_READ_FUNC(readJsonValueUint32, rapidjson::Value, uint32_t, GetUint, 0);

// 读取Json数据类型long long
RAPIDJSON_READ_FUNC(readJsonValueInt64, rapidjson::Document, int64_t, GetInt64, 0);
RAPIDJSON_READ_FUNC(readJsonValueInt64, rapidjson::Value, int64_t, GetInt64, 0);

// 读取Json数据类型unsigned long long
RAPIDJSON_READ_FUNC(readJsonValueUint64, rapidjson::Document, uint64_t, GetUint64, 0);
RAPIDJSON_READ_FUNC(readJsonValueUint64, rapidjson::Value, uint64_t, GetUint64, 0);

// 读取Json数据类型double
RAPIDJSON_READ_FUNC(readJsonValueDouble, rapidjson::Document, double, GetDouble, 0.0);
RAPIDJSON_READ_FUNC(readJsonValueDouble, rapidjson::Value, double, GetDouble, 0.0);

// 读取Json数据类型bool
RAPIDJSON_READ_FUNC(readJsonValueBool, rapidjson::Document, bool, GetBool, false);
RAPIDJSON_READ_FUNC(readJsonValueBool, rapidjson::Value, bool, GetBool, false);

// 读取Json数据类型string
RAPIDJSON_READ_FUNC(readJsonValueString, rapidjson::Document, std::string, GetString, "");
RAPIDJSON_READ_FUNC(readJsonValueString, rapidjson::Value, std::string, GetString, "");

// 读取Json数据类型object
RAPIDJSON_READ_FUNC_OBJECT(readJsonValueObject, rapidjson::Document);
RAPIDJSON_READ_FUNC_OBJECT(readJsonValueObject, rapidjson::Value);

// 读取Json数据类型array
RAPIDJSON_READ_FUNC_ARRAY(readJsonValueArray, rapidjson::Document);
RAPIDJSON_READ_FUNC_ARRAY(readJsonValueArray, rapidjson::Value);

}

#endif /* JsonParser_h */
