CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(WZQ_SERVICE)

# 基类库路径设置
set(DUOLE_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../duole_service_base)

# 通用配置
list(APPEND CMAKE_MODULE_PATH ${DUOLE_ROOT_PATH}/cmake)
include(BuildSettings)
include(DuoleDependencies)

# 基类库引用
# duole_service_base
add_subdirectory(${DUOLE_ROOT_PATH} duole_servicebase/servicebase_static)

# 服务器编译，静态库或动态库方式引用 service_cpp 库；
# 项目组工程放在 service 目录；
ADD_SUBDIRECTORY(service/wzqconn)
ADD_SUBDIRECTORY(service/wzqauth)
ADD_SUBDIRECTORY(service/wzqgame)
ADD_SUBDIRECTORY(service/wzqroom)