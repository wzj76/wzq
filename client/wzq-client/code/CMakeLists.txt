#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2015-2017 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

cmake_minimum_required(VERSION 3.6)

set(APP_NAME phzgame)

project(${APP_NAME})

set(DUOLE_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../duole_client_base)
set(COCOS2DX_ROOT_PATH ${DUOLE_ROOT_PATH}/cocos2d-duole)
set(GAMEBASE_ROOT_PATH ${DUOLE_ROOT_PATH}/gamebase)
set(EXTERNAL_ROOT_PATH ${DUOLE_ROOT_PATH}/external)
set(THIRDSDKS_ROOT_PATH ${DUOLE_ROOT_PATH}/third-sdks)
set(DUOLESDKS_ROOT_PATH ${DUOLE_ROOT_PATH}/sdks)
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

include(CocosBuildSet)
# 支持 lua
set(BUILD_LUA_LIBS ON)
# cocos2d-duole
add_subdirectory(${COCOS2DX_ROOT_PATH}/libs cocos2d-duole/cocos2d_static)
# clientbase
add_subdirectory(${DUOLE_ROOT_PATH} clientbase/clientbase_static)

# record sources, headers, resources...
set(GAME_SOURCE)
set(GAME_HEADER)

set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
    )
if(APPLE OR WINDOWS)
    cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif()

# add Classes source files and head files
set(GAME_CLASSES_SOURCE)
set(GAME_CLASSES_HEADER)
# source
FILE(GLOB_RECURSE GAME_CLASSES_SOURCE 
    ${CMAKE_CURRENT_SOURCE_DIR}/Classes/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Classes/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Classes/*.cc)
# header
include_sub_directories_recursively(${CMAKE_CURRENT_SOURCE_DIR}/Classes) 
list(APPEND GAME_SOURCE ${GAME_CLASSES_SOURCE})
list(APPEND GAME_HEADER ${GAME_CLASSES_HEADER})

if(ANDROID)
    # change APP_NAME to the share library name for Android, it's value depend on AndroidManifest.xml
    set(APP_NAME phzgame)
    list(APPEND GAME_SOURCE
        proj.android/app/src/main/jni/cocos2dcpp/main.cpp
        proj.android/app/src/main/jni/cocos2dcpp/PlatformFunction.cpp
        )
elseif(APPLE)
    if(IOS)
        list(APPEND GAME_HEADER
            proj.ios_mac/ios/AppController.h
            proj.ios_mac/ios/RootViewController.h
            )
        set(APP_UI_RES
            proj.ios_mac/ios/LaunchScreen.storyboard
            proj.ios_mac/ios/LaunchScreenBackground.png
            proj.ios_mac/ios/Images.xcassets
            )
        list(APPEND GAME_SOURCE
            proj.ios_mac/ios/main.m
            proj.ios_mac/ios/AppController.mm
            proj.ios_mac/ios/RootViewController.mm
            proj.ios_mac/ios/Prefix.pch
            ${APP_UI_RES}
            )
    elseif(MACOSX)
        set(APP_UI_RES
            proj.ios_mac/mac/Icon.icns
            proj.ios_mac/mac/Info.plist
            )
        list(APPEND GAME_SOURCE
            proj.ios_mac/mac/main.cpp
            proj.ios_mac/mac/Prefix.pch
            ${APP_UI_RES}
            )
    endif()
    list(APPEND GAME_SOURCE ${common_res_files})
endif()

# mark app complie info and libs info
set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
    )
if(NOT ANDROID)
    add_executable(${APP_NAME} ${all_code_files})
else()
    add_library(${APP_NAME} SHARED ${all_code_files})
    target_link_libraries(${APP_NAME} GLESv2 EGL log android OpenSLES)

    # link: clientbase_libs
    include(${DUOLE_ROOT_PATH}/cmake/DuoleClientBaseConfig.cmake)
    target_link_libraries(${APP_NAME} ${duole_clientbase_libs})
endif()
# link: cocos_static_libs
include(${COCOS2DX_ROOT_PATH}/libs/cmake/DuoleCocosConfig.cmake)
target_link_libraries(${APP_NAME} ${duole_cocos2d_libs})

target_include_directories(${APP_NAME}
    PRIVATE Classes
)

# mark app resources
setup_cocos_app_config(${APP_NAME})
if(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    if(MACOSX)
        set_target_properties(${APP_NAME} PROPERTIES
                              MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist"
                              )
    elseif(IOS)
        cocos_pak_xcode(${APP_NAME} INFO_PLIST "iOSBundleInfo.plist.in")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
        set_xcode_property(${APP_NAME} DEVELOPMENT_TEAM "")
        set_xcode_property(${APP_NAME} CODE_SIGN_IDENTITY "iPhone Developer")
    endif()
elseif(WINDOWS)
    cocos_copy_target_dll(${APP_NAME})
endif()

if(LINUX OR WINDOWS)
    set(APP_RES_DIR "$<TARGET_FILE_DIR:${APP_NAME}>/Resources")
    cocos_copy_target_res(${APP_NAME} COPY_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
endif()
