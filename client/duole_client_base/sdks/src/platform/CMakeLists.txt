cmake_minimum_required(VERSION 3.6)

set(lib_name sdks_platform)
set(target_name duole_${lib_name})
project(${lib_name})
# path
set(DUOLE_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
set(COCOS2DX_ROOT_PATH ${DUOLE_ROOT_PATH}/cocos2d-duole)
set(GAMEBASE_ROOT_PATH ${DUOLE_ROOT_PATH}/gamebase)

set(${target_name}_src 
    android/PlatformUtil-android.cpp
)
add_library(${target_name} STATIC ${${target_name}_src})

target_include_directories(${target_name}
    PUBLIC ../../
    PUBLIC ../
    PUBLIC .
    PRIVATE ${COCOS2DX_ROOT_PATH}/cocos
    PRIVATE ${COCOS2DX_ROOT_PATH}/external
    PRIVATE ${GAMEBASE_ROOT_PATH}/src
)

set(_path_prefix ${CMAKE_CURRENT_SOURCE_DIR}/../../platform/prebuilt/)
set(platform_name android)
set(platform_spec_path android/${ANDROID_ABI})
set(platform_spec_path "${_path_prefix}${platform_spec_path}")

set_target_properties(${target_name}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${platform_spec_path}"
)
