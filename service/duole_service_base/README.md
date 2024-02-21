# duole_service_base

多乐游戏服务器基类库
https://code.duole.com/common/duole_service_base.git
## 一、模块说明

- servicebase 
  多乐内部使用的通用 C++ 模块，目前包含：mcgcommon、mcgservice、mcgutil、mcghttp; 
- external  
  多乐集成使用的第三方开源代码库：libwebsockets; 
- unit_test 
  单元测试用例； 
- tools  
  通用工具脚本; 
- doc  
  存放基类库相关文档; 

## 二、运行环境安装

* 支持 Linux、Mac 环境下运行；
* Linux、Mac下都使用静态库方式发布；
* external 目录中的类库需要自行编译安装；

**Linux：**
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

**Mac：**
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

## 三、编译和发布 

# 1、servicebase 
servicebase 以静态库方式应用到项目中，所以，对 servicebase 类库的修改，需要重新编译、发布。 
```shell
# 进入编译脚本所在的目录
cd servicebase/src
# 编译结果自动被发布到 servicebase/prebuilt 目录
sh build.sh 
```

# 2、libwebsockets 
```shell
cd external
# 3.0.1版本（默认）
sh build_lws_3.0.1.sh
# 3.1.0版本
sh build_lws_3.1.0.sh
```

## 四、工程配置

**Xcode工程配置：** 

工程文件：projects_xcode/servicebase.xcodeproj

```shell
Header Search Paths：
/usr/local/include
/usr/local/include/lua
/usr/local/opt/openssl/include
$(SRCROOT)/../servicebase/src
$(SRCROOT)/../external/libwebsockets/include/mac

Library Search Paths:
/usr/local/lib
/usr/local/opt/openssl/lib
$(SRCROOT)/../external/libwebsockets/prebuilt/mac

C Language Dialect        GNU99[-std=gnu99]
C++ Language Dialect      GNU++11[-std=gnu++11]
```

**cmake 配置文件：** 

servicebase/src/CMakeLists.txt
