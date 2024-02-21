//
//  lua_my_AssetsDownloader.cpp
//  duolemjgame-mobile
//
//  Created by lx on 2022/3/20.
//

#include "lua_duole_assets_downloader.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"
#include "cocos2d/LuaScriptHandlerMgr.h"
#include "common/AssetsDownloader.h"

using namespace duole;

int lua_duole_AssetsDownloader_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AssetsDownloader",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_AssetsDownloader_create'", nullptr);
            return 0;
        }
        AssetsDownloader* ret = AssetsDownloader::create();
        object_to_luaval<AssetsDownloader>(tolua_S, "AssetsDownloader",(AssetsDownloader*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AssetsDownloader:create",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_AssetsDownloader_create'.",&tolua_err);
#endif
    return 0;
}

static int lua_duole_AssetsDownloader_downloadAsync(lua_State* L)
{
    int argc = 0;
    AssetsDownloader* self = nullptr;

    std::string responseheader = "";

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L,1,"AssetsDownloader",0,&tolua_err)) goto tolua_lerror;
#endif

    self = (AssetsDownloader*)  tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(L,"invalid 'self' in function 'lua_duole_AssetsDownloader_downloadAsync'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(L) - 1;

    if (2 == argc)
    {
        bool ok  = true;
        std::string arg1;
        ok &= luaval_to_std_string(L, 2, &arg1);
        
        std::string arg2;
        ok &= luaval_to_std_string(L, 3, &arg2);
        
        self->downloadAsync(arg1, arg2);
        if (ok) {
            return 0;
        }
    }

    luaL_error(L, "'downloadAsync' function of AssetsDownloader wrong number of arguments: %d, was expecting %d\n", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
        tolua_error(L,"#ferror in function 'lua_duole_AssetsDownloader_downloadAsync'.",&tolua_err);
    return 0;
#endif
}

static int lua_duole_AssetsDownloader_unzipResAsync(lua_State* L)
{
    int argc = 0;
    AssetsDownloader* self = nullptr;

    std::string responseheader = "";

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L,1,"AssetsDownloader",0,&tolua_err)) goto tolua_lerror;
#endif

    self = (AssetsDownloader*)  tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(L,"invalid 'self' in function 'lua_duole_AssetsDownloader_downloadAsync'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(L) - 1;

    if (2 == argc)
    {
        bool ok  = true;
        std::string arg1;
        ok &= luaval_to_std_string(L, 2, &arg1);
        
        std::string arg2;
        ok &= luaval_to_std_string(L, 3, &arg2);
        
        self->unzipResAsync(arg1, arg2);
        if (ok) {
            return 0;
        }
    }
    
    if (3 == argc)
    {
        bool ok  = true;
        std::string arg1;
        ok &= luaval_to_std_string(L, 2, &arg1);
        
        std::string arg2;
        ok &= luaval_to_std_string(L, 3, &arg2);
        
        std::string arg3;
        ok &= luaval_to_std_string(L, 4, &arg3);
        
        self->unzipResAsync(arg1, arg2, arg3);
        if (ok) {
            return 0;
        }
    }

    luaL_error(L, "'downloadAsync' function of AssetsDownloader wrong number of arguments: %d, was expecting %d\n", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
        tolua_error(L,"#ferror in function 'lua_duole_AssetsDownloader_downloadAsync'.",&tolua_err);
    return 0;
#endif
}

static int lua_duole_AssetsDownloader_decompressDownLoadZip(lua_State* L)
{
    int argc = 0;
    AssetsDownloader* self = nullptr;

    std::string responseheader = "";

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L,1,"AssetsDownloader",0,&tolua_err)) goto tolua_lerror;
#endif

    self = (AssetsDownloader*)  tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(L,"invalid 'self' in function 'lua_duole_AssetsDownloader_downloadAsync'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(L) - 1;

    if (1 == argc)
    {
        bool ok  = true;
        std::string arg1;
        ok &= luaval_to_std_string(L, 2, &arg1);
        
        self->decompressDownLoadZip(arg1);
        if (ok) {
            return 0;
        }
    }

    luaL_error(L, "'decompressDownLoadZip' function of AssetsDownloader wrong number of arguments: %d, was expecting %d\n", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
        tolua_error(L,"#ferror in function 'lua_duole_AssetsDownloader_downloadAsync'.",&tolua_err);
    return 0;
#endif
}

int lua_duole_AssetsDownloader_getProgressCur(lua_State* tolua_S)
{
    int argc = 0;
    AssetsDownloader* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AssetsDownloader",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AssetsDownloader*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_duole_AssetsDownloader_getProgressCur'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_AssetsDownloader_getProgressCur'", nullptr);
            return 0;
        }
        double ret = cobj->getProgressCur();
        tolua_pushnumber(tolua_S, (lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AssetsDownloader:getHuDatas",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_AssetsDownloader_getProgressCur'.",&tolua_err);
#endif

    return 0;
}

int lua_duole_AssetsDownloader_getProgressTotal(lua_State* tolua_S)
{
    int argc = 0;
    AssetsDownloader* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AssetsDownloader",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AssetsDownloader*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_duole_AssetsDownloader_getProgressTotal'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_AssetsDownloader_getProgressTotal'", nullptr);
            return 0;
        }
        double ret = cobj->getProgressTotal();
        tolua_pushnumber(tolua_S, (lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AssetsDownloader:getProgressTotal",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_AssetsDownloader_getProgressTotal'.",&tolua_err);
#endif

    return 0;
}

static int lua_duole_AssetsDownloader_setTag(lua_State* tolua_S)
{
    int argc = 0;
    AssetsDownloader* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AssetsDownloader",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AssetsDownloader*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_duole_AssetsDownloader_setTag'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1)
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,&arg0, "AssetsDownloader:setTag");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_AssetsDownloader_setTag'", nullptr);
            return 0;
        }
        cobj->setTag(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AssetsDownloader:setTag",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_AssetsDownloader_setTag'.",&tolua_err);
#endif

    return 0;
}

int lua_duole_AssetsDownloader_getTag(lua_State* tolua_S)
{
    int argc = 0;
    AssetsDownloader* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AssetsDownloader",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AssetsDownloader*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_duole_AssetsDownloader_getTag'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_AssetsDownloader_getTag'", nullptr);
            return 0;
        }
        double ret = cobj->getTag();
        tolua_pushnumber(tolua_S, (lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AssetsDownloader:getTag",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_AssetsDownloader_getTag'.",&tolua_err);
#endif

    return 0;
}

static int lua_duole_AssetsDownloader_setUserData(lua_State* tolua_S)
{
    int argc = 0;
    AssetsDownloader* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AssetsDownloader",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AssetsDownloader*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_duole_AssetsDownloader_setUserData'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2, &arg0);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_AssetsDownloader_setUserData'", nullptr);
            return 0;
        }
        cobj->setUserData(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AssetsDownloader:setUserData",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_AssetsDownloader_setUserData'.",&tolua_err);
#endif

    return 0;
}

int lua_duole_AssetsDownloader_getUserData(lua_State* tolua_S)
{
    int argc = 0;
    AssetsDownloader* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AssetsDownloader",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AssetsDownloader*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_duole_AssetsDownloader_getUserData'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_AssetsDownloader_getUserData'", nullptr);
            return 0;
        }
        const std::string& ret = cobj->getUserData();
        lua_pushlstring(tolua_S,ret.c_str(),ret.length());
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AssetsDownloader:getUserData",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_AssetsDownloader_getUserData'.",&tolua_err);
#endif

    return 0;
}

static int lua_duole_AssetsDownloader_setStop(lua_State* tolua_S)
{
    int argc = 0;
    AssetsDownloader* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AssetsDownloader",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AssetsDownloader*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_duole_AssetsDownloader_setStop'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1)
    {
        bool arg0;
        ok &= luaval_to_boolean(tolua_S, 2, &arg0);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_AssetsDownloader_setStop'", nullptr);
            return 0;
        }
        cobj->setStop(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AssetsDownloader:setStop",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_AssetsDownloader_setStop'.",&tolua_err);
#endif

    return 0;
}

int lua_duole_AssetsDownloader_isStop(lua_State* tolua_S)
{
    int argc = 0;
    AssetsDownloader* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AssetsDownloader",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AssetsDownloader*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_duole_AssetsDownloader_isStop'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_AssetsDownloader_isStop'", nullptr);
            return 0;
        }
        bool ret = cobj->isStop();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AssetsDownloader:isStop",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_AssetsDownloader_isStop'.",&tolua_err);
#endif

    return 0;
}

int lua_duole_AssetsDownloader_getErrorCode(lua_State* tolua_S)
{
    int argc = 0;
    AssetsDownloader* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AssetsDownloader",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (AssetsDownloader*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_duole_AssetsDownloader_getErrorCode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_AssetsDownloader_getErrorCode'", nullptr);
            return 0;
        }
        const std::string& ret = cobj->getErrorCode();
        lua_pushlstring(tolua_S,ret.c_str(),ret.length());
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AssetsDownloader:getErrorCode",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_AssetsDownloader_getErrorCode'.",&tolua_err);
#endif

    return 0;
}

static int lua_duole_AssetsDownloader_registerScriptHandler(lua_State* L)
{
    int argc = 0;
    AssetsDownloader* self = nullptr;

    std::string responseheader = "";

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L,1,"AssetsDownloader",0,&tolua_err)) goto tolua_lerror;
#endif

    self = (AssetsDownloader*)  tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(L,"invalid 'self' in function 'lua_duole_AssetsDownloader_registerScriptHandler'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(L) - 1;

    if (1 == argc)
    {
#if COCOS2D_DEBUG >= 1
        if (!toluafix_isfunction(L,2,"LUA_FUNCTION",0,&tolua_err))
            goto tolua_lerror;
#endif

        int handler = (  toluafix_ref_function(L,2,0));
        cocos2d::ScriptHandlerMgr::HandlerType eScriptHandler = cocos2d::ScriptHandlerMgr::getInstance()->addCustomHandler((void*)self, handler);
        self->setScriptHandler(eScriptHandler);
        return 0;
    }

    luaL_error(L, "'registerScriptHandler' function of AssetsDownloader wrong number of arguments: %d, was expecting %d\n", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
        tolua_error(L,"#ferror in function 'lua_duole_AssetsDownloader_registerScriptHandler'.",&tolua_err);
    return 0;
#endif
}

static int lua_duole_AssetsDownloader_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (AssetsDownloader)");
    return 0;
}

int lua_register_duole_AssetsDownloader(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"AssetsDownloader");
    tolua_cclass(tolua_S,"AssetsDownloader","AssetsDownloader","cc.Ref",nullptr);

    tolua_beginmodule(tolua_S,"AssetsDownloader");
        tolua_function(tolua_S,"create",lua_duole_AssetsDownloader_create);
        tolua_function(tolua_S,"downloadAsync",lua_duole_AssetsDownloader_downloadAsync);
        tolua_function(tolua_S,"unzipResAsync",lua_duole_AssetsDownloader_unzipResAsync);
        tolua_function(tolua_S,"decompressDownLoadZip",lua_duole_AssetsDownloader_decompressDownLoadZip);
        tolua_function(tolua_S,"getProgressTotal",lua_duole_AssetsDownloader_getProgressTotal);
        tolua_function(tolua_S,"getProgressCur",lua_duole_AssetsDownloader_getProgressCur);
        tolua_function(tolua_S,"setTag",lua_duole_AssetsDownloader_setTag);
        tolua_function(tolua_S,"getTag",lua_duole_AssetsDownloader_getTag);
        tolua_function(tolua_S,"setUserData",lua_duole_AssetsDownloader_setUserData);
        tolua_function(tolua_S,"getUserData",lua_duole_AssetsDownloader_getUserData);
        tolua_function(tolua_S,"setStop",lua_duole_AssetsDownloader_setStop);
        tolua_function(tolua_S,"isStop",lua_duole_AssetsDownloader_isStop);
        tolua_function(tolua_S,"getErrorCode",lua_duole_AssetsDownloader_getErrorCode);
        tolua_function(tolua_S,"registerScriptHandler",lua_duole_AssetsDownloader_registerScriptHandler);

    tolua_endmodule(tolua_S);
    std::string typeName = typeid(AssetsDownloader).name();
    g_luaType[typeName] = "AssetsDownloader";
    g_typeCast["AssetsDownloader"] = "AssetsDownloader";
    return 1;
}

TOLUA_API int register_duole_assets_downloader(lua_State* tolua_S)
{
    lua_getglobal(tolua_S, "_G");
    tolua_open(tolua_S);

    tolua_module(tolua_S,"dl",0);
    tolua_beginmodule(tolua_S,"dl");

    lua_register_duole_AssetsDownloader(tolua_S);

    tolua_endmodule(tolua_S);
    lua_pop(tolua_S, 1);
    return 1;
}
