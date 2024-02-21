//
//  lua_duole_module_register.h
//  duole_clientbase_libs
//
//  Created by weiran on 2022/7/14.
//

#ifndef lua_module_register_duole_h
#define lua_module_register_duole_h

#include "lua_duole_reader_writer.h"
#include "lua_duole_http_request.h"
#include "lua_duole_image_picker.h"
#include "lua_duole_edit_area.h"
#include "lua_duole_assets_downloader.h"
#include "lua_duole_encode_decode_util.h"

int lua_module_register_duole(lua_State* L)
{
    register_duole_reader_writer(L);
    register_duole_http_request(L);
    register_duole_image_picker(L);
    register_duole_edit_area(L);
    register_duole_assets_downloader(L);
    register_duole_encode_decode_util(L);
    return 1;
}


#endif /* lua_duole_module_register_h */
