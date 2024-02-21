#!/bin/bash -ilex 
# 编译；
# build 目录
if [ ! -d "build" ]
then
	mkdir -p build
fi

cd ./build
rm -rf *
cmake ..
make
