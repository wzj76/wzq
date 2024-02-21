CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# 项目信息设置
set(PROJECT_NAME redis_loadrunner)
project(${PROJECT_NAME})
set(DUOLE_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../..)

# 通用配置
list(APPEND CMAKE_MODULE_PATH ${DUOLE_ROOT_PATH}/cmake)
include(BuildSettings)
include(DuoleDependencies)

# 输出目录
SET(EXECUTABLE_OUTPUT_PATH "${DUOLE_ROOT_PATH}/bin/unit_tesst/${PROJECT_NAME}")
MESSAGE(STATUS "EXECUTABLE_OUTPUT_PATH: ${EXECUTABLE_OUTPUT_PATH}")

# 添加项目源文件
FILE(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/*.cpp)

# 添加项目头文件
include_sub_directories_recursively(${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}) 

# 基类库
# duole_service_base
add_subdirectory(${DUOLE_ROOT_PATH} duole_servicebase/servicebase_static)

# 生成可执行程序；
SET(ALL_SRCS ${SOURCE_FILES})
ADD_EXECUTABLE(${PROJECT_NAME} ${ALL_SRCS})

# 链接库设置；
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${duole_servicebase_libs} ${third_libs}) 

