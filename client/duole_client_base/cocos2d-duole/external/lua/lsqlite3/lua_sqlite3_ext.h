
#ifndef __LUA_SQL3EXT_H_
#define __LUA_SQL3EXT_H_

#if defined(_USRDLL)
    #define LUA_SQLITE3_DLL     __declspec(dllexport)
#else         /* use a DLL library */
    #define LUA_SQLITE3_DLL
#endif

#if __cplusplus
extern "C" {
#endif

#include "lauxlib.h"

void LUA_SQLITE3_DLL register_lua_sqlite3(lua_State *L);
    
#if __cplusplus
}
#endif

#endif /* __LUA_SQL3EXT_H_ */
