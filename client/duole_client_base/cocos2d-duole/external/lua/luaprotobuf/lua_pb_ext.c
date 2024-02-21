
#include "lua_pb_ext.h"

#if __cplusplus
extern "C" {
#endif

// pb
#include "pb.h"


static luaL_Reg luax_pb[] = {
    {"pb", luaopen_pb},

    {NULL, NULL}
};

//#include "tolua_fix.h"

void register_lua_pb(lua_State *L)
{
    luaL_Reg* lib = luax_pb;
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
