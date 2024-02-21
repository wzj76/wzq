
#ifndef __LUA_PB_H_
#define __LUA_PB_H_

#if defined(_USRDLL)
    #define LUA_PB_DLL     __declspec(dllexport)
#else         /* use a DLL library */
    #define LUA_PB_DLL
#endif

#if __cplusplus
extern "C" {
#endif

#include "lauxlib.h"

void LUA_PB_DLL register_lua_pb(lua_State *L);
    
#if __cplusplus
}
#endif

#endif /* __LUA_PB_H_ */
