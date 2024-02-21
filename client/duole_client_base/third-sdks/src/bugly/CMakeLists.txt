cmake_minimum_required(VERSION 3.6)

set(lib_name bugly)
set(target_name duole_${lib_name})
project(${lib_name})
# path
set(DUOLE_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
set(COCOS2DX_ROOT_PATH ${DUOLE_ROOT_PATH}/cocos2d-duole)
set(THIRDSDKS_ROOT_PATH ${DUOLE_ROOT_PATH}/third-sdks)

set(BUGLY_SOURCE)
set(BUGLY_HEADER)
# c++
list(APPEND BUGLY_SOURCE
  plugin/CrashReport.mm
)
list(APPEND BUGLY_HEADER
  plugin/CrashReport.h
)
# lua
list(APPEND BUGLY_SOURCE
  plugin/lua/BuglyLuaAgent.cpp
)
list(APPEND BUGLY_HEADER
  plugin/lua/BuglyLuaAgent.h
)
# js 
# list(APPEND BUGLY_SOURCE
#   plugin/js/BuglyJSAgent.cpp
# )
# list(APPEND BUGLY_HEADER
#   plugin/js/BuglyJSAgent.h
# )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -x c++")
set(${target_name}_src ${BUGLY_SOURCE})
add_library(${target_name} STATIC ${${target_name}_src})

target_include_directories(${target_name}
    PUBLIC ../../
    PUBLIC ../
    PUBLIC .
    PUBLIC plugin
    PUBLIC plugin/lua
    PRIVATE ${COCOS2DX_ROOT_PATH}/cocos
    PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/base
    PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/platform/android
    PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/scripting/lua-bindings/manual
    PRIVATE ${COCOS2DX_ROOT_PATH}/external
    PRIVATE ${COCOS2DX_ROOT_PATH}/external/lua/luajit/include
    PRIVATE ${COCOS2DX_ROOT_PATH}/external/lua/tolua
    PRIVATE ${COCOS2DX_ROOT_PATH}/external/lua/lua
)

set(_path_prefix ${CMAKE_CURRENT_SOURCE_DIR}/../../bugly/prebuilt/)
set(platform_name android)
set(platform_spec_path android/${ANDROID_ABI})
set(platform_spec_path "${_path_prefix}${platform_spec_path}")

set_target_properties(${target_name}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${platform_spec_path}"
)
