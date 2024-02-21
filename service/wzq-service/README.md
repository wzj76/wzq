
# 快速开始

## 1、创建项目：
示例：
```shell
https://code.duole.com/mj/duolemj-service-v2.git
```
## 2、基类库获取：
```shell
git clone https://code.duole.com/common/duole_service_base.git
```
## 3、本地目录结构示例：

``` 
# 基类库目录：可从基类库发布仓库直接获取，不允许更改目录名称；
workspace                   本地工作空间目录；
  duole_service_base        基类库目录；
    bin                     可执行程序存放目录，自动生成；
    build                   cmake编译目录，自动生成；
    doc                     基类库开发文档；
    external                经过定制的第三方库；
    projects_xcode          xcode工程文件；
    servicebase             service基类库目录，包含源码和发布目录；
    tools                   工具、运维脚本存放目录；
    unit_test               单元测试和压测工具；
    CMakeLists.txt          cmake配置文件，支持mac、linux；业务通过此文件链接基类库；
    README.md               快速指南；

  duolemj-service-v2        业务工程存放根目录；
    bin                     可执行程序存放目录，自动生成；
    build                   cmake编译目录，自动生成；
    projects_xcode          xcode工程文件；
    service                 项目组 C++ 程序存放目录；
      mjconn                麻将连接服工程；
      mjroom                麻将房间服工程；
      mjgame                麻将游戏服工程；
      ......                其它工程；
    tools                   工具、运维脚本存放目录；
    build.sh                编译脚本；
    CMakeLists.txt          cmake配置文件，支持mac、linux；
    README.md               快速指南；
```
## 4、 运行环境安装：

* 支持 Linux、Mac 环境下运行；
* Linux、Mac下都使用静态库方式发布；
* external 目录中的类库需要自行编译安装；


**Linux环境安装：**
```shell
sudo yum install -y protobuf-devel
sudo yum install -y lua-devel
sudo yum install -y libevent-devel
sudo yum install -y hiredis-devel
sudo yum install -y log4cplus-devel
sudo yum install -y boost-devel
sudo yum install -y jsoncpp-devel
sudo yum install -y libuuid-devel
sudo yum install -y openssl-devel
sudo yum install -y libcurl-devel
# centos7的系统上安装mysql client的方式：
sudo yum install -y mariadb-devel
```
**Mac环境安装：**
```shell
# protobuf 手动安装，2.5.0版本；
brew install lua
brew install libevent
brew install hiredis
brew install log4cplus
brew install boost
brew install jsoncpp
brew install ossp-uuid
brew install openssl
brew install curl
brew install mysql-client@5.7
```

## 5. 工程配置

*以 service/mjconn 工程为例*

**Xcode工程配置：**

```shell
# Other Linker Flags:
  -lssl -lcrypto -lcurl -lhiredis -ljsoncpp -llua -lpthread -levent -lprotobuf -llog4cplus -luuid -ldl -lboost_regex -lz -lservice_base -lwebsockets

# Runpath Search Paths:
  $(SRCROOT)/../../../duole_service_base/external/libwebsockets/prebuilt/mac

# Header Search Paths：
  /usr/local/include
  /usr/local/include/lua
  /usr/local/opt/openssl/include
  $(SRCROOT)/../../../duole_service_base/servicebase/src
  $(SRCROOT)/../../../duole_service_base/servicebase/src/mcgcommon
  $(SRCROOT)/../../../duole_service_base/servicebase/src/mcgservice
  $(SRCROOT)/../../../duole_service_base/servicebase/src/mcgutil
  $(SRCROOT)/../../../duole_service_base/servicebase/src/mcghttp
  $(SRCROOT)/../../../duole_service_base/external/libwebsockets/include/mac

# Library Search Paths:
  /usr/local/lib
  /usr/local/opt/openssl/lib
  $(SRCROOT)/../../../duole_service_base/servicebase/prebuilt/mac
  $(SRCROOT)/../../../duole_service_base/external/libwebsockets/prebuilt/mac
  
# C Language Dialect        GNU99[-std=gnu99]
# C++ Language Dialect      GNU++11[-std=gnu++11]

```

**cmake 配置文件：**  

service/mjconn/CMakeLists.txt
```shell
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# 项目信息设置
set(PROJECT_NAME mjconn)
project(${PROJECT_NAME})
set(DUOLE_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../duole_service_base)

# 通用配置
list(APPEND CMAKE_MODULE_PATH ${DUOLE_ROOT_PATH}/cmake)
include(BuildSettings)
include(DuoleDependencies)

# 输出目录
SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../bin/service/${PROJECT_NAME}")
MESSAGE(STATUS "EXECUTABLE_OUTPUT_PATH: ${EXECUTABLE_OUTPUT_PATH}")

# 添加项目源文件
FILE(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/*.cc)

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

```
## 6. 启动和停止

**Xcode调试和运行：**
在程序输出目录设置程序配置信息(Products/mjconn)后，可直接从Xcode启动程序进行调试；

**命令行方式：**
* 修改cmake编译配置文件：duolemj-service-v2/CMakeLists.txt;
```shell
# 工程代码目录：service；
ADD_SUBDIRECTORY(service/mjconn)
```
* 编译程序
```shell
sh build.sh
```
