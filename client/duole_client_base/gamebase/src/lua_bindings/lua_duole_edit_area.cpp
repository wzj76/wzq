#include "lua_duole_edit_area.h"
#include "LuaEditArea.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"

USING_NS_CC;
using namespace ui;
using namespace std;
using namespace duole;

int lua_duole_LuaEditArea_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"LuaEditArea",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 7)
    {
        string arg0;
        string arg1;
        string arg2;
        int arg3;
        double arg4;
        int arg5;
        bool arg6;
        
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "LuaEditArea:create");
        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "LuaEditArea:create");
        ok &= luaval_to_std_string(tolua_S, 4,&arg2, "LuaEditArea:create");
        ok &= luaval_to_int32(tolua_S, 5,&arg3, "LuaEditArea:create");
        ok &= luaval_to_number(tolua_S, 6,&arg4, "LuaEditArea:create");
        ok &= luaval_to_int32(tolua_S, 7,&arg5, "LuaEditArea:create");
        ok &= luaval_to_boolean(tolua_S, 8,&arg6, "LuaEditArea:create");
        
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_edit_area_create'", nullptr);
            return 0;
        }

        LuaEditArea* ret = LuaEditArea::create(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
        object_to_luaval<LuaEditArea>(tolua_S, "LuaEditArea",(LuaEditArea*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "LuaEditArea:create",argc, 8);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_edit_area_create'.",&tolua_err);
#endif
    return 0;
}

static int lua_duole_LuaEditArea_registerScriptHandler(lua_State* L)
{
    int argc = 0;
    LuaEditArea* self = nullptr;

    std::string responseheader = "";

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(L,1,"LuaEditArea",0,&tolua_err)) goto tolua_lerror;
#endif

    self = (LuaEditArea*)  tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(L,"invalid 'self' in function 'lua_duole_LuaEditArea_registerScriptHandler'\n", nullptr);
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

    luaL_error(L, "'registerScriptHandler' function of LuaEditArea wrong number of arguments: %d, was expecting %d\n", argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
        tolua_error(L,"#ferror in function 'lua_duole_LuaEditArea_registerScriptHandler'.",&tolua_err);
    return 0;
#endif
}

int lua_duole_LuaEditArea_getText(lua_State* tolua_S)
{
    int argc = 0;
    LuaEditArea* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"LuaEditArea",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaEditArea*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_duole_LuaEditArea_getText'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_LuaEditArea_getText'", nullptr);
            return 0;
        }
        
        const std::string& ret = cobj->getText();
        lua_pushlstring(tolua_S,ret.c_str(),ret.length());
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "LuaEditArea:getText",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_LuaEditArea_getText'.",&tolua_err);
#endif

    return 0;
}

static int lua_duole_LuaEditArea_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (LuaEditArea)");
    return 0;
}

int lua_register_duole_LuaEditArea(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"LuaEditArea");
    tolua_cclass(tolua_S,"LuaEditArea","LuaEditArea","cc.Ref",nullptr);

    tolua_beginmodule(tolua_S,"LuaEditArea");
        tolua_function(tolua_S,"create", lua_duole_LuaEditArea_create);
        tolua_function(tolua_S,"registerScriptHandler",lua_duole_LuaEditArea_registerScriptHandler);
        tolua_function(tolua_S,"getText",lua_duole_LuaEditArea_getText);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(LuaEditArea).name();
    g_luaType[typeName] = "LuaEditArea";
    g_typeCast["LuaEditArea"] = "LuaEditArea";
    return 1;
}

TOLUA_API int register_duole_edit_area(lua_State* tolua_S)
{
    lua_getglobal(tolua_S, "_G");
    tolua_open(tolua_S);

    tolua_module(tolua_S,"dl",0);
    tolua_beginmodule(tolua_S,"dl");

    lua_register_duole_LuaEditArea(tolua_S);

    tolua_endmodule(tolua_S);
    lua_pop(tolua_S, 1);
    return 1;
}
