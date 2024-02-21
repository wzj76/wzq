cmake_minimum_required(VERSION 3.6)

set(lib_name gamebase)
set(target_name duole_${lib_name})
project(${lib_name})

set(DUOLE_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(COCOS2DX_ROOT_PATH ${DUOLE_ROOT_PATH}/cocos2d-duole)
set(EXTERNAL_ROOT_PATH ${DUOLE_ROOT_PATH}/external)

set(${target_name}_src 
# eff_comm
    eff_comm/ptrary.cpp
    eff_comm/ptrlist.cpp
    eff_comm/ptrqueue.cpp
    eff_comm/workqueue.cpp
# socketx
    socketx/ccsocketx.cpp
    socketx/ccgamesocket.cpp
    socketx/ccasyncsocketx.cpp
    socketx/rounddatabuff.cpp
# common
    common/AppConfig.cpp
    common/XXTeaCrypto.cpp
    common/WebVariables.cpp
    common/AppConfig.cpp
    common/md5.cpp
    common/base64.cpp
    common/ReaderWriter.cpp
    common/EncodeDecodeUtil.cpp
    common/AssetsDownloader.cpp
# lua_bindings
    lua_bindings/LuaHttpRequest.cpp
    lua_bindings/LuaEditArea.cpp
    lua_bindings/LuaImagePicker.cpp
    lua_bindings/lua_duole_edit_area.cpp
    lua_bindings/lua_duole_http_request.cpp
    lua_bindings/lua_duole_image_picker.cpp
    lua_bindings/lua_duole_reader_writer.cpp
    lua_bindings/lua_duole_encode_decode_util.cpp
    lua_bindings/lua_duole_assets_downloader.cpp
)

add_library(${target_name} STATIC
  ${${target_name}_src}
)

target_include_directories(${target_name}
    PUBLIC .
    PRIVATE ${EXTERNAL_ROOT_PATH}/src/google
    PRIVATE ${COCOS2DX_ROOT_PATH}/cocos
    PRIVATE ${COCOS2DX_ROOT_PATH}/external
    PRIVATE ${COCOS2DX_ROOT_PATH}/external/lua/lua
    PRIVATE ${COCOS2DX_ROOT_PATH}/external/lua/tolua
    PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/scripting/lua-bindings/manual
)

set(_path_prefix ${CMAKE_CURRENT_SOURCE_DIR}/../prebuilt/)
set(platform_spec_path android/${ANDROID_ABI})
set(platform_spec_path "${_path_prefix}${platform_spec_path}")

set_target_properties(${target_name}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${platform_spec_path}"
)
