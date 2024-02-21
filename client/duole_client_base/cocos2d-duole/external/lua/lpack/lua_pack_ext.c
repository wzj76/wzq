
#include "lua_pack_ext.h"

#if __cplusplus
extern "C" {
#endif

// pack
#include "lpack.h"


static luaL_Reg luax_pack[] = {
    {"pack", luaopen_pack},

    {NULL, NULL}
};

//#include "tolua_fix.h"

void register_lua_pack(lua_State *L)
{
    luaL_Reg* lib = luax_pack;
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
