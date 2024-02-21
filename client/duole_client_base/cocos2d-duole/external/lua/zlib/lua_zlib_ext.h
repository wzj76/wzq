
#ifndef __LUA_ZLIB_EXT_H_
#define __LUA_ZLIB_EXT_H_

#if defined(_USRDLL)
    #define LUA_ZLIB_DLL     __declspec(dllexport)
#else         /* use a DLL library */
    #define LUA_ZLIB_DLL
#endif

#if __cplusplus
extern "C" {
#endif

#include "lauxlib.h"

void LUA_ZLIB_DLL register_lua_zlib(lua_State *L);
    
#if __cplusplus
}
#endif

#endif /* __LUA_ZLIB_EXT_H_ */
