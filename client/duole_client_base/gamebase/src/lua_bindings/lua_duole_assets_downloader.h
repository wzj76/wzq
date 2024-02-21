//
//  lua_my_AssetsDownloader.hpp
//  duolemjgame-mobile
//
//  Created by lx on 2022/3/20.
//

#include "base/ccConfig.h"
#ifndef lua_duole_assets_downloader_h
#define lua_duole_assets_downloader_h

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

int register_duole_assets_downloader(lua_State* tolua_S);

#endif /* lua_duole_assets_downloader_h */
