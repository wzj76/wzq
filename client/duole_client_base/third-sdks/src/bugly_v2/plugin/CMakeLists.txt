cmake_minimum_required(VERSION 3.6)

set(lib_name buglycrashreport)
set(target_name duole_${lib_name})

project(${lib_name})

set(${target_name}_src 
    ${DUOLE_ROOT_PATH}/third-sdks/bugly/plugin/CrashReport.mm
)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -x c++")
add_library(${target_name} STATIC ${${target_name}_src})

target_include_directories(${target_name}
    PUBLIC ../../
    PUBLIC ../
    PUBLIC .
    PRIVATE ${COCOS2DX_ROOT_PATH}/cocos
    PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/base
    PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/platform/android
    PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/scripting/lua-bindings/manual
    PRIVATE ${COCOS2DX_ROOT_PATH}/external
    PRIVATE ${COCOS2DX_ROOT_PATH}/external/lua/luajit/include
    PRIVATE ${COCOS2DX_ROOT_PATH}/external/lua/tolua
    PRIVATE ${COCOS2DX_ROOT_PATH}/external/lua/lua
)

set(_path_prefix ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/)
set(platform_name android)
set(platform_spec_path android/${ANDROID_ABI})
set(platform_spec_path "${_path_prefix}${platform_spec_path}")

set_target_properties(${target_name}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${platform_spec_path}"
)
