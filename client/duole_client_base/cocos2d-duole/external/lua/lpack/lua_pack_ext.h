
#ifndef __LUA_PACK_H_
#define __LUA_PACK_H_

#if defined(_USRDLL)
    #define LUA_PACK_DLL     __declspec(dllexport)
#else         /* use a DLL library */
    #define LUA_PACK_DLL
#endif

#if __cplusplus
extern "C" {
#endif

#include "lauxlib.h"

void LUA_PACK_DLL register_lua_pack(lua_State *L);
    
#if __cplusplus
}
#endif

#endif /* __LUA_PACK_H_ */
