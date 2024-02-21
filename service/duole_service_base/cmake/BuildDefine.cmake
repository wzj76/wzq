# 系统类型设定：(注意: APPLE也属于UNIX)
if (UNIX AND NOT APPLE)	
    MESSAGE(STATUS "linux")
    SET(SYSTEM_OS "linux")
elseif (WIN32)
    MESSAGE(STATUS "windows")
    SET(SYSTEM_OS "windows")
elseif (APPLE)
    MESSAGE(STATUS "mac")
    SET(SYSTEM_OS "mac")
endif ()

# cmake 模块路径设置；
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#设置编译器（可选的有gcc，g++）
SET(CMAKE_C_COMPILER "gcc")
SET(CMAKE_CXX_COMPILER "g++")

# 编译参数设置: 针对所有编译器，c、c++
list(APPEND CMAKE_C_FLAGS "-std=gnu99")
list(APPEND CMAKE_CXX_FLAGS "-g -Wall -fPIC -D__DAEMON__ -std=gnu++11")
