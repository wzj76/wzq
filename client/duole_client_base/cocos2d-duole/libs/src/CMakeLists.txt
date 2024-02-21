cmake_minimum_required(VERSION 3.6)

# 输出目录
SET(DUOLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../prebuilt/android/${ANDROID_ABI}")

project(cocos2d_duole)
set(target_name cocos2d_duole)
add_library(${target_name} empty.cpp)

set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

include(CocosBuildSet)
# 编译 cocos2d-x
# 支持 lua
set(BUILD_LUA_LIBS ON)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos cocos2d-duole/cocos/core)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/platform/android cocos2d-duole/cocos/platform)
# lua
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/scripting/lua-bindings/proj.android cocos2d-duole/cocos/lua-android)

set_target_properties(${target_name}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    FOLDER "External"
)

# install
INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/lib DESTINATION ${DUOLE_OUTPUT_PATH})
INSTALL(FILES ${CMAKE_BINARY_DIR}/cocos2d-duole/cocos/platform/libcpp_android_spec.a DESTINATION ${DUOLE_OUTPUT_PATH}/lib)
INSTALL(FILES ${CMAKE_BINARY_DIR}/cocos2d-duole/cocos/lua-android/liblua_android_spec.a DESTINATION ${DUOLE_OUTPUT_PATH}/lib)