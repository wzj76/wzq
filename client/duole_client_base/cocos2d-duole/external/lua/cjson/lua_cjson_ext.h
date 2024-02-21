
#ifndef __LUA_CJSONEXT_H_
#define __LUA_CJSONEXT_H_

#if defined(_USRDLL)
    #define LUA_CJSON_DLL     __declspec(dllexport)
#else         /* use a DLL library */
    #define LUA_CJSON_DLL
#endif

#if __cplusplus
extern "C" {
#endif

#include "lauxlib.h"

void LUA_CJSON_DLL register_lua_cjson(lua_State *L);
    
#if __cplusplus
}
#endif

#endif /* __LUA_CJSONEXT_H_ */
