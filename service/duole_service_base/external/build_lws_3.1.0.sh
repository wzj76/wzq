#!/bin/bash -ilex 
if [ "$(uname)" == "Darwin" ];
then
	# Mac OS X 操作系统
	SYSTEM_OS="mac"
	export PATH="/usr/local/opt/openssl/bin:$PATH"
	export OPENSSL_ROOT_DIR="/usr/local/opt/openssl"	
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ];
then   
	# GNU/Linux操作系统	
	SYSTEM_OS="linux"
fi

# 检查发布目录；
# extern/libwebsockets目录
if [ ! -d "libwebsockets" ]
then
	mkdir -p "libwebsockets/include/linux"
	mkdir -p "libwebsockets/include/mac"
	mkdir -p "libwebsockets/prebuilt/linux"
	mkdir -p "libwebsockets/prebuilt/mac"
fi

if [ ! -d "src" ]
then
	mkdir -p src
fi
# libwebsockets 3.1.0版本；
cd ./src
# 还没有下载源码：v3.1.0
if [ ! -d "libwebsockets" ]; then
    git clone -b v3.1.0 https://github.com/warmcat/libwebsockets
fi

cd libwebsockets

# build 目录
if [ ! -d "build" ]
then
	mkdir -p build
fi

cd build
rm -rf *
cmake .. -DLWS_WITH_LIBEVENT=1
make
# make install 

# copy lib/include
rm -rf ../../../libwebsockets/include/${SYSTEM_OS}/*
rm -rf ../../../libwebsockets/prebuilt/${SYSTEM_OS}/*

cp -r ./include/* ../../../libwebsockets/include/${SYSTEM_OS}/
cp -r ./lib/* ../../../libwebsockets/prebuilt/${SYSTEM_OS}/

# 清理 build 目录；
rm -rf *




