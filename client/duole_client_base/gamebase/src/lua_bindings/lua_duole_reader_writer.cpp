#include "lua_duole_reader_writer.h"
#include "common/ReaderWriter.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"

using namespace duole;

int lua_duole_ReaderWriter_IsBase64Char(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"ReaderWriter",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        int32_t arg0;
        ok &= luaval_to_int32(tolua_S, 2,&arg0, "ReaderWriter:IsBase64Char");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_ReaderWriter_IsBase64Char'", nullptr);
            return 0;
        }
        bool ret = ReaderWriter::IsBase64Char(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "ReaderWriter:IsBase64Char",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_ReaderWriter_IsBase64Char'.",&tolua_err);
#endif
    return 0;
}
int lua_duole_ReaderWriter_SaveStringForKey(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"ReaderWriter",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        std::string arg0;
        std::string arg1;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "ReaderWriter:SaveStringForKey");
        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "ReaderWriter:SaveStringForKey");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_ReaderWriter_SaveStringForKey'", nullptr);
            return 0;
        }
        ReaderWriter::SaveStringForKey(arg0, arg1);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "ReaderWriter:SaveStringForKey",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_ReaderWriter_SaveStringForKey'.",&tolua_err);
#endif
    return 0;
}
int lua_duole_ReaderWriter_SaveBooleanToXML(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"ReaderWriter",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        std::string arg0;
        bool arg1;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "ReaderWriter:SaveBooleanToXML");
        ok &= luaval_to_boolean(tolua_S, 3,&arg1, "ReaderWriter:SaveBooleanToXML");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_ReaderWriter_SaveBooleanToXML'", nullptr);
            return 0;
        }
        ReaderWriter::SaveBooleanToXML(arg0, arg1);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "ReaderWriter:SaveBooleanToXML",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_ReaderWriter_SaveBooleanToXML'.",&tolua_err);
#endif
    return 0;
}
int lua_duole_ReaderWriter_LoadIntegerFromXML(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"ReaderWriter",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        std::string arg0;
        int arg1;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "ReaderWriter:LoadIntegerFromXML");
        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "ReaderWriter:LoadIntegerFromXML");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_ReaderWriter_LoadIntegerFromXML'", nullptr);
            return 0;
        }
        int ret = ReaderWriter::LoadIntegerFromXML(arg0, arg1);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "ReaderWriter:LoadIntegerFromXML",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_ReaderWriter_LoadIntegerFromXML'.",&tolua_err);
#endif
    return 0;
}
int lua_duole_ReaderWriter_SaveFloatFromXML(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"ReaderWriter",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        std::string arg0;
        double arg1;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "ReaderWriter:SaveFloatFromXML");
        ok &= luaval_to_number(tolua_S, 3,&arg1, "ReaderWriter:SaveFloatFromXML");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_ReaderWriter_SaveFloatFromXML'", nullptr);
            return 0;
        }
        ReaderWriter::SaveFloatFromXML(arg0, arg1);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "ReaderWriter:SaveFloatFromXML",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_ReaderWriter_SaveFloatFromXML'.",&tolua_err);
#endif
    return 0;
}
int lua_duole_ReaderWriter_LoadStringFromXML(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"ReaderWriter",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        std::string arg0;
        std::string arg1;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "ReaderWriter:LoadStringFromXML");
        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "ReaderWriter:LoadStringFromXML");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_ReaderWriter_LoadStringFromXML'", nullptr);
            return 0;
        }
        std::string ret = ReaderWriter::LoadStringFromXML(arg0, arg1);
        lua_pushlstring(tolua_S,ret.c_str(),ret.length());
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "ReaderWriter:LoadStringFromXML",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_ReaderWriter_LoadStringFromXML'.",&tolua_err);
#endif
    return 0;
}
int lua_duole_ReaderWriter_LoadFloatFromXML(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"ReaderWriter",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        std::string arg0;
        double arg1;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "ReaderWriter:LoadFloatFromXML");
        ok &= luaval_to_number(tolua_S, 3,&arg1, "ReaderWriter:LoadFloatFromXML");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_ReaderWriter_LoadFloatFromXML'", nullptr);
            return 0;
        }
        double ret = ReaderWriter::LoadFloatFromXML(arg0, arg1);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "ReaderWriter:LoadFloatFromXML",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_ReaderWriter_LoadFloatFromXML'.",&tolua_err);
#endif
    return 0;
}
int lua_duole_ReaderWriter_Decode(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"ReaderWriter",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "ReaderWriter:Decode");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_ReaderWriter_Decode'", nullptr);
            return 0;
        }
        std::string ret = ReaderWriter::Decode(arg0);
        lua_pushlstring(tolua_S,ret.c_str(),ret.length());
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "ReaderWriter:Decode",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_ReaderWriter_Decode'.",&tolua_err);
#endif
    return 0;
}
int lua_duole_ReaderWriter_GetStringForKey(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"ReaderWriter",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        std::string arg0;
        std::string arg1;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "ReaderWriter:GetStringForKey");
        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "ReaderWriter:GetStringForKey");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_ReaderWriter_GetStringForKey'", nullptr);
            return 0;
        }
        std::string ret = ReaderWriter::GetStringForKey(arg0, arg1);
        lua_pushlstring(tolua_S,ret.c_str(),ret.length());
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "ReaderWriter:GetStringForKey",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_ReaderWriter_GetStringForKey'.",&tolua_err);
#endif
    return 0;
}
int lua_duole_ReaderWriter_Flush(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"ReaderWriter",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_ReaderWriter_Flush'", nullptr);
            return 0;
        }
        ReaderWriter::Flush();
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "ReaderWriter:Flush",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_ReaderWriter_Flush'.",&tolua_err);
#endif
    return 0;
}
int lua_duole_ReaderWriter_SaveStringToXML(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"ReaderWriter",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        std::string arg0;
        std::string arg1;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "ReaderWriter:SaveStringToXML");
        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "ReaderWriter:SaveStringToXML");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_ReaderWriter_SaveStringToXML'", nullptr);
            return 0;
        }
        ReaderWriter::SaveStringToXML(arg0, arg1);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "ReaderWriter:SaveStringToXML",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_ReaderWriter_SaveStringToXML'.",&tolua_err);
#endif
    return 0;
}
int lua_duole_ReaderWriter_Encode(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"ReaderWriter",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "ReaderWriter:Encode");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_ReaderWriter_Encode'", nullptr);
            return 0;
        }
        std::string ret = ReaderWriter::Encode(arg0);
        lua_pushlstring(tolua_S,ret.c_str(),ret.length());
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "ReaderWriter:Encode",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_ReaderWriter_Encode'.",&tolua_err);
#endif
    return 0;
}
int lua_duole_ReaderWriter_LoadBooleanFromXML(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"ReaderWriter",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        std::string arg0;
        bool arg1;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "ReaderWriter:LoadBooleanFromXML");
        ok &= luaval_to_boolean(tolua_S, 3,&arg1, "ReaderWriter:LoadBooleanFromXML");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_ReaderWriter_LoadBooleanFromXML'", nullptr);
            return 0;
        }
        bool ret = ReaderWriter::LoadBooleanFromXML(arg0, arg1);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "ReaderWriter:LoadBooleanFromXML",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_ReaderWriter_LoadBooleanFromXML'.",&tolua_err);
#endif
    return 0;
}
int lua_duole_ReaderWriter_SaveIntegerToXML(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"ReaderWriter",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        std::string arg0;
        int arg1;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "ReaderWriter:SaveIntegerToXML");
        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "ReaderWriter:SaveIntegerToXML");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_duole_ReaderWriter_SaveIntegerToXML'", nullptr);
            return 0;
        }
        ReaderWriter::SaveIntegerToXML(arg0, arg1);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "ReaderWriter:SaveIntegerToXML",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_duole_ReaderWriter_SaveIntegerToXML'.",&tolua_err);
#endif
    return 0;
}
static int lua_duole_ReaderWriter_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (ReaderWriter)");
    return 0;
}

int lua_register_duole_ReaderWriter(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"ReaderWriter");
    tolua_cclass(tolua_S,"ReaderWriter","ReaderWriter","",nullptr);

    tolua_beginmodule(tolua_S,"ReaderWriter");
        tolua_function(tolua_S,"IsBase64Char", lua_duole_ReaderWriter_IsBase64Char);
        tolua_function(tolua_S,"SaveStringForKey", lua_duole_ReaderWriter_SaveStringForKey);
        tolua_function(tolua_S,"SaveBooleanToXML", lua_duole_ReaderWriter_SaveBooleanToXML);
        tolua_function(tolua_S,"LoadIntegerFromXML", lua_duole_ReaderWriter_LoadIntegerFromXML);
        tolua_function(tolua_S,"SaveFloatFromXML", lua_duole_ReaderWriter_SaveFloatFromXML);
        tolua_function(tolua_S,"LoadStringFromXML", lua_duole_ReaderWriter_LoadStringFromXML);
        tolua_function(tolua_S,"LoadFloatFromXML", lua_duole_ReaderWriter_LoadFloatFromXML);
        tolua_function(tolua_S,"Decode", lua_duole_ReaderWriter_Decode);
        tolua_function(tolua_S,"GetStringForKey", lua_duole_ReaderWriter_GetStringForKey);
        tolua_function(tolua_S,"Flush", lua_duole_ReaderWriter_Flush);
        tolua_function(tolua_S,"SaveStringToXML", lua_duole_ReaderWriter_SaveStringToXML);
        tolua_function(tolua_S,"Encode", lua_duole_ReaderWriter_Encode);
        tolua_function(tolua_S,"LoadBooleanFromXML", lua_duole_ReaderWriter_LoadBooleanFromXML);
        tolua_function(tolua_S,"SaveIntegerToXML", lua_duole_ReaderWriter_SaveIntegerToXML);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ReaderWriter).name();
    g_luaType[typeName] = "ReaderWriter";
    g_typeCast["ReaderWriter"] = "ReaderWriter";
    return 1;
}
TOLUA_API int register_duole_reader_writer(lua_State* tolua_S)
{
    lua_getglobal(tolua_S, "_G");

	tolua_open(tolua_S);
	
	tolua_module(tolua_S,"dl",0);
	tolua_beginmodule(tolua_S,"dl");

	lua_register_duole_ReaderWriter(tolua_S);

	tolua_endmodule(tolua_S);
    
    lua_pop(tolua_S, 1);
	return 1;
}

