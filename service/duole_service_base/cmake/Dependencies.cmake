# 系统库：线程库、动态库、zip
list(APPEND third_libs "-lpthread")
list(APPEND third_libs "-ldl")
list(APPEND third_libs "-lz")
# 第三方库：安装使用
list(APPEND third_libs "-levent")
list(APPEND third_libs "-llua")
list(APPEND third_libs "-lhiredis")
list(APPEND third_libs "-lprotobuf")
list(APPEND third_libs "-llog4cplus")
list(APPEND third_libs "-lboost_regex")
list(APPEND third_libs "-ljsoncpp")
list(APPEND third_libs "-luuid")
list(APPEND third_libs "-lcurl")
list(APPEND third_libs "-lssl")
list(APPEND third_libs "-lcrypto")
list(APPEND third_libs "-lmysqlclient")

# 定制的第三方类库：external/websockets;
INCLUDE_DIRECTORIES(${DUOLE_ROOT_PATH}/external/libwebsockets/include/${SYSTEM_OS})

# 定义依赖库目录：include 及 lib 所在目录;
# mac : brew install 安装
# linux : yum install 安装
# MAC 需要显式的设置第三方库include目录和lib库目录;
SET(THIRD_INCLUDE_PATH)
SET(THIRD_LIBS_PATH)

IF("${SYSTEM_OS}" STREQUAL "linux")
    list(APPEND THIRD_INCLUDE_PATH "/usr/include/jsoncpp")
    list(APPEND THIRD_INCLUDE_PATH "/usr/include/mysql")
    list(APPEND THIRD_LIBS_PATH "/usr/lib64/mysql")
ELSEIF("${SYSTEM_OS}" STREQUAL "mac")
    list(APPEND THIRD_INCLUDE_PATH "/usr/local/include")
    list(APPEND THIRD_LIBS_PATH "/usr/local/lib")
    list(APPEND THIRD_INCLUDE_PATH "/usr/local/opt/openssl/include")
    list(APPEND THIRD_LIBS_PATH "/usr/local/opt/openssl/lib")
    list(APPEND THIRD_INCLUDE_PATH "/usr/local/include/lua")    
    list(APPEND THIRD_INCLUDE_PATH "/usr/local/opt/mysql-client@5.7/include")
    list(APPEND THIRD_LIBS_PATH "/usr/local/opt/mysql-client@5.7/lib")
ENDIF()
INCLUDE_DIRECTORIES(${THIRD_INCLUDE_PATH})
LINK_DIRECTORIES(${THIRD_LIBS_PATH})


