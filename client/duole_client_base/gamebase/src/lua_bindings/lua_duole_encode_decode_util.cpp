#include "lua_duole_encode_decode_util.h"
#include "common/EncodeDecodeUtil.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"

using namespace duole;

int lua_duole_EncodeDecodeUtil_getMd5Encode(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"EncodeDecodeUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "EncodeDecodeUtil:getMd5Encode");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_EncodeDecodeUtil_getMd5Encode'", nullptr);
            return 0;
        }
        const std::string& ret = EncodeDecodeUtil::getMd5Encode(arg0);
        lua_pushlstring(tolua_S,ret.c_str(),ret.length());
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "EncodeDecodeUtil:getMd5Encode",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_EncodeDecodeUtil_getMd5Encode'.",&tolua_err);
#endif
    return 0;
}

int lua_duole_EncodeDecodeUtil_getBase64Encode(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"EncodeDecodeUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "EncodeDecodeUtil:getBase64Encode");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_EncodeDecodeUtil_getBase64Encode'", nullptr);
            return 0;
        }
        const std::string& ret = EncodeDecodeUtil::getBase64Encode(arg0);
        lua_pushlstring(tolua_S,ret.c_str(),ret.length());
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "EncodeDecodeUtil:getBase64Encode",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_EncodeDecodeUtil_getBase64Encode'.",&tolua_err);
#endif
    return 0;
}

int lua_duole_EncodeDecodeUtil_getBase64Decode(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"EncodeDecodeUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "EncodeDecodeUtil:getBase64Decode");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_EncodeDecodeUtil_getBase64Decode'", nullptr);
            return 0;
        }
        const std::string& ret = EncodeDecodeUtil::getBase64Decode(arg0);
        lua_pushlstring(tolua_S,ret.c_str(),ret.length());
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "EncodeDecodeUtil:getBase64Decode",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_EncodeDecodeUtil_getBase64Decode'.",&tolua_err);
#endif
    return 0;
}

int lua_duole_EncodeDecodeUtil_getBase64UrlEncode(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"EncodeDecodeUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "EncodeDecodeUtil:getBase64UrlEncode");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_EncodeDecodeUtil_getBase64UrlEncode'", nullptr);
            return 0;
        }
        const std::string& ret = EncodeDecodeUtil::getBase64UrlEncode(arg0);
        lua_pushlstring(tolua_S,ret.c_str(),ret.length());
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "EncodeDecodeUtil:getBase64UrlEncode",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_EncodeDecodeUtil_getBase64UrlEncode'.",&tolua_err);
#endif
    return 0;
}

int lua_duole_EncodeDecodeUtil_getBase64UrlDecode(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"EncodeDecodeUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "EncodeDecodeUtil:getBase64UrlDecode");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_EncodeDecodeUtil_getBase64UrlDecode'", nullptr);
            return 0;
        }
        const std::string& ret = EncodeDecodeUtil::getBase64UrlDecode(arg0);
        lua_pushlstring(tolua_S,ret.c_str(),ret.length());
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "EncodeDecodeUtil:getBase64UrlDecode",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_EncodeDecodeUtil_getBase64UrlDecode'.",&tolua_err);
#endif
    return 0;
}

int lua_duole_EncodeDecodeUtil_getXorEncode(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"EncodeDecodeUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        std::string arg0;
        std::string arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "EncodeDecodeUtil:getXorEncode");
        
        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "EncodeDecodeUtil:getXorEncode");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_EncodeDecodeUtil_getXorEncode'", nullptr);
            return 0;
        }
        const std::string& ret = EncodeDecodeUtil::getXorEncode(arg0, arg1);
        lua_pushlstring(tolua_S,ret.c_str(),ret.length());
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "EncodeDecodeUtil:getXorEncode",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_EncodeDecodeUtil_getXorEncode'.",&tolua_err);
#endif
    return 0;
}

int lua_duole_EncodeDecodeUtil_getRot47Encode(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"EncodeDecodeUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "EncodeDecodeUtil:getRot47Encode");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_EncodeDecodeUtil_getRot47Encode'", nullptr);
            return 0;
        }
        const std::string& ret = EncodeDecodeUtil::getRot47Encode(arg0);
        lua_pushlstring(tolua_S,ret.c_str(),ret.length());
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "EncodeDecodeUtil:getRot47Encode",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_EncodeDecodeUtil_getRot47Encode'.",&tolua_err);
#endif
    return 0;
}

int lua_duole_EncodeDecodeUtil_getCustomEncodedUrl(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"EncodeDecodeUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "EncodeDecodeUtil:getCustomEncodedUrl");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_EncodeDecodeUtil_getCustomEncodedUrl'", nullptr);
            return 0;
        }
        const std::string& ret = EncodeDecodeUtil::getCustomEncodedUrl(arg0);
        lua_pushlstring(tolua_S,ret.c_str(),ret.length());
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "EncodeDecodeUtil:getCustomEncodedUrl",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_EncodeDecodeUtil_getCustomEncodedUrl'.",&tolua_err);
#endif
    return 0;
}

int lua_duole_EncodeDecodeUtil_getFileDataXxteaEncrypt(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"EncodeDecodeUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        std::string arg0;
        std::string arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "EncodeDecodeUtil:getFileDataXxteaEncrypt");
        
        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "EncodeDecodeUtil:getFileDataXxteaEncrypt");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_EncodeDecodeUtil_getFileDataXxteaEncrypt'", nullptr);
            return 0;
        }
        ssize_t nLen = 0;
        unsigned char* pData = EncodeDecodeUtil::getFileDataXxteaEncrypt(arg0, arg1, &nLen);
        if (!pData)
        {
            tolua_error(tolua_S,"Get file data xxtea encode failed in function 'lua_duole_EncodeDecodeUtil_getFileDataXxteaEncrypt'", nullptr);
            return 0;
        }
        const std::string& ret = std::string(pData, pData + nLen);
        lua_pushlstring(tolua_S,ret.c_str(),ret.length());
        free(pData);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "EncodeDecodeUtil:getFileDataXxteaEncrypt",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_EncodeDecodeUtil_getFileDataXxteaEncrypt'.",&tolua_err);
#endif
    return 0;
}

int lua_duole_EncodeDecodeUtil_getFileDataXxteaDecrypt(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"EncodeDecodeUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        std::string arg0;
        std::string arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "EncodeDecodeUtil:getFileDataXxteaDecrypt");
        
        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "EncodeDecodeUtil:getFileDataXxteaDecrypt");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_EncodeDecodeUtil_getFileDataXxteaDecrypt'", nullptr);
            return 0;
        }
        ssize_t nLen = 0;
        unsigned char* pData = EncodeDecodeUtil::getFileDataXxteaDecrypt(arg0, arg1, &nLen);
        if (!pData)
        {
            tolua_error(tolua_S,"Get file data xxtea decode failed in function 'lua_duole_EncodeDecodeUtil_getFileDataXxteaDecrypt'", nullptr);
            return 0;
        }
        const std::string& ret = std::string(pData, pData + nLen);
        lua_pushlstring(tolua_S,ret.c_str(),ret.length());
        free(pData);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "EncodeDecodeUtil:getFileDataXxteaDecrypt",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_EncodeDecodeUtil_getFileDataXxteaDecrypt'.",&tolua_err);
#endif
    return 0;
}

static int lua_duole_EncodeDecodeUtil_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (EncodeDecodeUtil)");
    return 0;
}

int lua_register_duole_EncodeDecodeUtil(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"EncodeDecodeUtil");
    tolua_cclass(tolua_S,"EncodeDecodeUtil","EncodeDecodeUtil","",nullptr);

    tolua_beginmodule(tolua_S,"EncodeDecodeUtil");
        tolua_function(tolua_S,"getMd5Encode",lua_duole_EncodeDecodeUtil_getMd5Encode);
        tolua_function(tolua_S,"getBase64Encode",lua_duole_EncodeDecodeUtil_getBase64Encode);
        tolua_function(tolua_S,"getBase64Decode",lua_duole_EncodeDecodeUtil_getBase64Decode);
        tolua_function(tolua_S,"getBase64UrlEncode",lua_duole_EncodeDecodeUtil_getBase64UrlEncode);
        tolua_function(tolua_S,"getBase64UrlDecode",lua_duole_EncodeDecodeUtil_getBase64UrlDecode);
        tolua_function(tolua_S,"getXorEncode",lua_duole_EncodeDecodeUtil_getXorEncode);
        tolua_function(tolua_S,"getRot47Encode",lua_duole_EncodeDecodeUtil_getRot47Encode);
        tolua_function(tolua_S,"getCustomEncodedUrl",lua_duole_EncodeDecodeUtil_getCustomEncodedUrl);
        tolua_function(tolua_S,"getFileDataXxteaEncrypt",lua_duole_EncodeDecodeUtil_getFileDataXxteaEncrypt);
        tolua_function(tolua_S,"getFileDataXxteaDecrypt",lua_duole_EncodeDecodeUtil_getFileDataXxteaDecrypt);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(EncodeDecodeUtil).name();
    g_luaType[typeName] = "EncodeDecodeUtil";
    g_typeCast["EncodeDecodeUtil"] = "EncodeDecodeUtil";
    return 1;
}

TOLUA_API int register_duole_encode_decode_util(lua_State* tolua_S)
{
    lua_getglobal(tolua_S, "_G");
    
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,"dl",0);
	tolua_beginmodule(tolua_S,"dl");

    lua_register_duole_EncodeDecodeUtil(tolua_S);

	tolua_endmodule(tolua_S);
    
    lua_pop(tolua_S, 1);
	return 1;
}

