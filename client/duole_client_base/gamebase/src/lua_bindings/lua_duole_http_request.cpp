#include "lua_duole_http_request.h"
#include "LuaHttpRequest.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"

using namespace std;
using namespace duole;

static int lua_duole_LuaHttpRequest_setRequestHeader(lua_State* L)
{
    int argc = 0;
    LuaHttpRequest* self = nullptr;
    const char* field = "";
    const char* value = "";
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L,1,"LuaHttpRequest",0,&tolua_err)) goto tolua_lerror;
#endif
    
    self = (LuaHttpRequest*)  tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(L,"invalid 'self' in function 'lua_duole_LuaHttpRequest_setRequestHeader'\n", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(L) - 1;
    
    if ( 2 == argc )
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(L, 2, 0, &tolua_err) ||
            !tolua_isstring(L, 3, 0, &tolua_err) )
            goto tolua_lerror;
#endif
        
        field = tolua_tostring(L, 2, "");
        value = tolua_tostring(L, 3, "");
        self->setRequestHeader(field, value);
        return 0;
    }
    
    luaL_error(L, "'setRequestHeader' function of LuaHttpRequest wrong number of arguments: %d, was expecting %d\n", argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(L,"#ferror in function 'lua_duole_LuaHttpRequest_setRequestHeader'.",&tolua_err);
    return 0;
#endif
}
int lua_duole_LuaHttpRequest_getErrorBuffer(lua_State* tolua_S)
{
    int argc = 0;
    LuaHttpRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaHttpRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaHttpRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_duole_LuaHttpRequest_getErrorBuffer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_LuaHttpRequest_getErrorBuffer'", nullptr);
            return 0;
        }
        std::string ret = cobj->getErrorBuffer();
        lua_pushlstring(tolua_S,ret.c_str(),ret.length());
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaHttpRequest:getErrorBuffer",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_LuaHttpRequest_getErrorBuffer'.",&tolua_err);
#endif

    return 0;
}
int lua_duole_LuaHttpRequest_isSucceed(lua_State* tolua_S)
{
    int argc = 0;
    LuaHttpRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaHttpRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaHttpRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_duole_LuaHttpRequest_isSucceed'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_LuaHttpRequest_isSucceed'", nullptr);
            return 0;
        }
        bool ret = cobj->isSucceed();
        tolua_pushboolean(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaHttpRequest:isSucceed",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_LuaHttpRequest_isSucceed'.",&tolua_err);
#endif

    return 0;
}
int lua_duole_LuaHttpRequest_getResponseCode(lua_State* tolua_S)
{
    int argc = 0;
    LuaHttpRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaHttpRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaHttpRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_duole_LuaHttpRequest_getResponseCode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_LuaHttpRequest_getResponseCode'", nullptr);
            return 0;
        }
        long ret = cobj->getResponseCode();
        tolua_pushnumber(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaHttpRequest:getResponseCode",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_LuaHttpRequest_getResponseCode'.",&tolua_err);
#endif

    return 0;
}
int lua_duole_LuaHttpRequest_getUrl(lua_State* tolua_S)
{
    int argc = 0;
    LuaHttpRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaHttpRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaHttpRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_duole_LuaHttpRequest_getUrl'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_LuaHttpRequest_getUrl'", nullptr);
            return 0;
        }
        std::string ret = cobj->getUrl();
        lua_pushlstring(tolua_S,ret.c_str(),ret.length());
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaHttpRequest:getUrl",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_LuaHttpRequest_getUrl'.",&tolua_err);
#endif

    return 0;
}
int lua_duole_LuaHttpRequest_setUrl(lua_State* tolua_S)
{
    int argc = 0;
    LuaHttpRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaHttpRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaHttpRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_duole_LuaHttpRequest_setUrl'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "LuaHttpRequest:setUrl");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_LuaHttpRequest_setUrl'", nullptr);
            return 0;
        }
        cobj->setUrl(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaHttpRequest:setUrl",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_LuaHttpRequest_setUrl'.",&tolua_err);
#endif

    return 0;
}
int lua_duole_LuaHttpRequest_send(lua_State* tolua_S)
{
    int argc = 0;
    LuaHttpRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaHttpRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaHttpRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_duole_LuaHttpRequest_send'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_LuaHttpRequest_send'", nullptr);
            return 0;
        }
        cobj->send();
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaHttpRequest:send",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_LuaHttpRequest_send'.",&tolua_err);
#endif

    return 0;
}
int lua_duole_LuaHttpRequest_sendImmediate(lua_State* tolua_S)
{
    int argc = 0;
    LuaHttpRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaHttpRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaHttpRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_duole_LuaHttpRequest_sendImmediate'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_LuaHttpRequest_sendImmediate'", nullptr);
            return 0;
        }
        cobj->sendImmediate();
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaHttpRequest:sendImmediate",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_LuaHttpRequest_sendImmediate'.",&tolua_err);
#endif

    return 0;
}
int lua_duole_LuaHttpRequest_setMethod(lua_State* tolua_S)
{
    int argc = 0;
    LuaHttpRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaHttpRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaHttpRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_duole_LuaHttpRequest_setMethod'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "LuaHttpRequest:setMethod");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_LuaHttpRequest_setMethod'", nullptr);
            return 0;
        }
        cobj->setMethod(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaHttpRequest:setMethod",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_LuaHttpRequest_setMethod'.",&tolua_err);
#endif

    return 0;
}
int lua_duole_LuaHttpRequest_setRequestData(lua_State* tolua_S)
{
    int argc = 0;
    LuaHttpRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaHttpRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaHttpRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_duole_LuaHttpRequest_setRequestData'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "LuaHttpRequest:setRequestData");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_LuaHttpRequest_setRequestData'", nullptr);
            return 0;
        }
        cobj->setRequestData(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaHttpRequest:setRequestData",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_LuaHttpRequest_setRequestData'.",&tolua_err);
#endif

    return 0;
}
int lua_duole_LuaHttpRequest_getResponseData(lua_State* tolua_S)
{
    int argc = 0;
    LuaHttpRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaHttpRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaHttpRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_duole_LuaHttpRequest_getResponseData'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_LuaHttpRequest_getResponseData'", nullptr);
            return 0;
        }
        std::string ret = cobj->getResponseData();
        lua_pushlstring(tolua_S,ret.c_str(),ret.length());
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaHttpRequest:getResponseData",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_LuaHttpRequest_getResponseData'.",&tolua_err);
#endif

    return 0;
}
int lua_duole_LuaHttpRequest_constructor(lua_State* tolua_S)
{
    int argc = 0;
    LuaHttpRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_LuaHttpRequest_constructor'", nullptr);
            return 0;
        }
        cobj = new LuaHttpRequest();
        int ID =  (int)cobj->_ID ;
        int* luaID =  &cobj->_luaID ;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"LuaHttpRequest");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaHttpRequest:LuaHttpRequest",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_duole_LuaHttpRequest_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_duole_LuaHttpRequest_registerScriptHandler(lua_State* L)
{
    int argc = 0;
    LuaHttpRequest* self = nullptr;
    
    string responseheader = "";
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L,1,"LuaHttpRequest",0,&tolua_err)) goto tolua_lerror;
#endif
    
    self = (LuaHttpRequest*)  tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(L,"invalid 'self' in function 'lua_duole_LuaHttpRequest_registerScriptHandler'\n", nullptr);
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
        ScriptHandlerMgr::HandlerType eScriptHandler = cocos2d::ScriptHandlerMgr::getInstance()->addCustomHandler((void*)self, handler);
        self->setScriptHandler(eScriptHandler);
        return 0;
    }
    
    luaL_error(L, "'registerScriptHandler' function of LuaHttpRequest wrong number of arguments: %d, was expecting %d\n", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L,"#ferror in function 'lua_duole_LuaHttpRequest_registerScriptHandler'.",&tolua_err);
    return 0;
#endif
}

static int lua_duole_LuaHttpRequest_unregisterScriptHandler(lua_State* L)
{
    int argc = 0;
    LuaHttpRequest* self = nullptr;
    
    string responseheader = "";
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L,1,"LuaHttpRequest",0,&tolua_err)) goto tolua_lerror;
#endif
    
    self = (LuaHttpRequest*)  tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(L,"invalid 'self' in function 'lua_duole_LuaHttpRequest_unregisterScriptHandler'\n", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(L) - 1;
    
    if (0 == argc)
    {
        cocos2d::ScriptHandlerMgr::getInstance()->removeObjectHandler((void*)self, self->getScriptHandler());
        
        return 0;
    }
    
    luaL_error(L, "'unregisterScriptHandler' function of LuaHttpRequest wrong number of arguments: %d, was expecting %d\n", argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L,"#ferror in function 'lua_duole_LuaHttpRequest_unregisterScriptHandler'.",&tolua_err);
    return 0;
#endif
    
}

static int lua_duole_LuaHttpRequest_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (LuaHttpRequest)");
    return 0;
}

int lua_register_duole_LuaHttpRequest(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"LuaHttpRequest");
    tolua_cclass(tolua_S,"LuaHttpRequest","LuaHttpRequest","cc.Ref",nullptr);

    tolua_beginmodule(tolua_S,"LuaHttpRequest");
        tolua_function(tolua_S,"new",lua_duole_LuaHttpRequest_constructor);
        tolua_function(tolua_S,"getErrorBuffer", lua_duole_LuaHttpRequest_getErrorBuffer);
        tolua_function(tolua_S,"setRequestHeader", lua_duole_LuaHttpRequest_setRequestHeader);
        tolua_function(tolua_S,"isSucceed", lua_duole_LuaHttpRequest_isSucceed);
        tolua_function(tolua_S,"getResponseCode", lua_duole_LuaHttpRequest_getResponseCode);
        tolua_function(tolua_S,"getUrl",lua_duole_LuaHttpRequest_getUrl);
        tolua_function(tolua_S,"setUrl",lua_duole_LuaHttpRequest_setUrl);
        tolua_function(tolua_S,"send",lua_duole_LuaHttpRequest_send);
        tolua_function(tolua_S,"sendImmediate", lua_duole_LuaHttpRequest_sendImmediate);
        tolua_function(tolua_S,"setMethod",lua_duole_LuaHttpRequest_setMethod);
        tolua_function(tolua_S,"setRequestData",lua_duole_LuaHttpRequest_setRequestData);
        tolua_function(tolua_S,"getResponseData",lua_duole_LuaHttpRequest_getResponseData);
        tolua_function(tolua_S,"registerScriptHandler", lua_duole_LuaHttpRequest_registerScriptHandler);
        tolua_function(tolua_S,"unregisterScriptHandler", lua_duole_LuaHttpRequest_unregisterScriptHandler);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(LuaHttpRequest).name();
    g_luaType[typeName] = "LuaHttpRequest";
    g_typeCast["LuaHttpRequest"] = "LuaHttpRequest";
    return 1;
}
TOLUA_API int register_duole_http_request(lua_State* tolua_S)
{
    lua_getglobal(tolua_S, "_G");
    
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,"dl",0);
	tolua_beginmodule(tolua_S,"dl");

	lua_register_duole_LuaHttpRequest(tolua_S);

	tolua_endmodule(tolua_S);
    
    lua_pop(tolua_S, 1);
    
	return 1;
}

