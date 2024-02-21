#include "lua_duole_image_picker.h"
#include "LuaImagePicker.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"

USING_NS_CC;
using namespace ui;
using namespace duole;

int lua_duole_LuaImagePicker_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"LuaImagePicker",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 5)
    {
        int arg0;
        int arg1;
        int arg2;
        int arg3;
        bool arg4;
        
        ok &= luaval_to_int32(tolua_S, 2,&arg0, "LuaImagePicker:create");
        ok &= luaval_to_int32(tolua_S, 3,&arg1, "LuaImagePicker:create");
        ok &= luaval_to_int32(tolua_S, 4,&arg2, "LuaImagePicker:create");
        ok &= luaval_to_int32(tolua_S, 5,&arg3, "LuaImagePicker:create");
        ok &= luaval_to_boolean(tolua_S, 6,&arg4, "LuaImagePicker:create");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_LuaImagePicker_create'", nullptr);
            return 0;
        }
        LuaImagePicker* ret = LuaImagePicker::create(arg0, arg1, arg2, arg3, arg4);
        object_to_luaval<LuaImagePicker>(tolua_S, "LuaImagePicker",(LuaImagePicker*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "LuaImagePicker:create",argc, 5);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_LuaImagePicker_create'.",&tolua_err);
#endif
    return 0;
}

static int lua_duole_LuaImagePicker_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (LuaImagePicker)");
    return 0;
}

static int lua_duole_LuaImagePicker_registerScriptHandler(lua_State* L)
{
    int argc = 0;
    LuaImagePicker* self = nullptr;

    std::string responseheader = "";

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L,1,"LuaImagePicker",0,&tolua_err)) goto tolua_lerror;
#endif

    self = (LuaImagePicker*)  tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(L,"invalid 'self' in function 'lua_duole_LuaImagePicker_registerScriptHandler'\n", nullptr);
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

    luaL_error(L, "'registerScriptHandler' function of LuaImagePicker wrong number of arguments: %d, was expecting %d\n", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
        tolua_error(L,"#ferror in function 'lua_duole_LuaImagePicker_registerScriptHandler'.",&tolua_err);
    return 0;
#endif
}

int lua_duole_LuaImagePicker_isSucceed(lua_State* tolua_S)
{
    int argc = 0;
    LuaImagePicker* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaImagePicker",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaImagePicker*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_duole_LuaImagePicker_isSucceed'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_LuaImagePicker_isSucceed'", nullptr);
            return 0;
        }
        bool ret = cobj->isSucceed();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaImagePicker:isSucceed",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_LuaImagePicker_isSucceed'.",&tolua_err);
#endif

    return 0;
}

int lua_duole_LuaImagePicker_getFilePath(lua_State* tolua_S)
{
    int argc = 0;
    LuaImagePicker* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaImagePicker",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaImagePicker*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_duole_LuaImagePicker_getFilePath'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_LuaImagePicker_getFilePath'", nullptr);
            return 0;
        }
        
        const std::string& ret = cobj->getFilePath();
        lua_pushlstring(tolua_S,ret.c_str(),ret.length());
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaImagePicker:getFilePath",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_LuaImagePicker_getFilePath'.",&tolua_err);
#endif

    return 0;
}

int lua_register_duole_LuaImagePicker(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"LuaImagePicker");
    tolua_cclass(tolua_S,"LuaImagePicker","LuaImagePicker","cc.Ref",nullptr);

    tolua_beginmodule(tolua_S,"LuaImagePicker");
        tolua_function(tolua_S,"create",lua_duole_LuaImagePicker_create);
        tolua_function(tolua_S,"registerScriptHandler",lua_duole_LuaImagePicker_registerScriptHandler);
        tolua_function(tolua_S,"getFilePath",lua_duole_LuaImagePicker_getFilePath);
        tolua_function(tolua_S,"isSucceed",lua_duole_LuaImagePicker_isSucceed);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(LuaImagePicker).name();
    g_luaType[typeName] = "LuaImagePicker";
    g_typeCast["LuaImagePicker"] = "LuaImagePicker";
    return 1;
}

TOLUA_API int register_duole_image_picker(lua_State* tolua_S)
{
    lua_getglobal(tolua_S, "_G");
    tolua_open(tolua_S);

    tolua_module(tolua_S,"dl",0);
    tolua_beginmodule(tolua_S,"dl");

    lua_register_duole_LuaImagePicker(tolua_S);

    tolua_endmodule(tolua_S);
    lua_pop(tolua_S, 1);
    return 1;
}
