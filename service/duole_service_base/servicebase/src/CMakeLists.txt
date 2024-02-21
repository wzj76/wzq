CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# 项目信息设置
set(PROJECT_NAME ServiceBase)
project(${PROJECT_NAME})
set(DUOLE_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../..)

# 通用配置
list(APPEND CMAKE_MODULE_PATH ${DUOLE_ROOT_PATH}/cmake)
include(BuildSettings)

# 输出目录
SET(LIBRARY_OUTPUT_PATH "${DUOLE_ROOT_PATH}/servicebase/prebuilt/${SYSTEM_OS}")
message(STATUS "LIBRARY_OUTPUT_PATH:" ${LIBRARY_OUTPUT_PATH})

# 设置基类库源代码目录
FILE(GLOB_RECURSE SOURCE_FILES 
	${PROJECT_SOURCE_DIR}/*.cpp
	${PROJECT_SOURCE_DIR}/*.c
	${PROJECT_SOURCE_DIR}/*.cc)
SET(ALL_SRCS ${SOURCE_FILES})

# 添加基类库头文件目录
include_sub_directories_recursively(${PROJECT_SOURCE_DIR}) 

# 静态库：libservice_base.a
ADD_LIBRARY(service_base_static STATIC ${ALL_SRCS})
SET_TARGET_PROPERTIES(service_base_static PROPERTIES 
	OUTPUT_NAME "service_base"
	ARCHIVE_OUTPUT_DIRECTORY "${LIBRARY_OUTPUT_PATH}"
)

