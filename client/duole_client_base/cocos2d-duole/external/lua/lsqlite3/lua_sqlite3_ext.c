
#include "lua_sqlite3_ext.h"

#if __cplusplus
extern "C" {
#endif

// sqlite3
#include "lsqlite3.h"


static luaL_Reg luax_sqlite3[] = {
    {"sqlite3", luaopen_lsqlite3},

    {NULL, NULL}
};

//#include "tolua_fix.h"

void register_lua_sqlite3(lua_State *L)
{
    luaL_Reg* lib = luax_sqlite3;
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
