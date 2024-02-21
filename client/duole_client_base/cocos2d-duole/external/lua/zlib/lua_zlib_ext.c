
#include "lua_zlib_ext.h"

#if __cplusplus
extern "C" {
#endif

// zlib
#include "lua_zlib.h"


static luaL_Reg luax_zlib[] = {
    {"zlib", luaopen_zlib},
    {NULL, NULL}
};

//#include "tolua_fix.h"

void register_lua_zlib(lua_State *L)
{
    luaL_Reg* lib = luax_zlib;
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    for (; lib->func; lib++)
    {
        lua_pushcfunction(L, lib->func);
        lua_setfield(L, -2, lib->name);
    }
    lua_pop(L, 2);
}

#if __cplusplus
} // extern "C"
#endif
