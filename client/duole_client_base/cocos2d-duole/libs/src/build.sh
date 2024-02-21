#!/bin/bash -ilex 

# build mode: default debug
build_mode=$1
if [[ ${build_mode} != "release" ]]; then
  build_mode="debug"
fi

function create_dir() {
  if [ ! -d $1 ]
  then
    mkdir -p $1
  fi
}

BUILD_HOME="`pwd -P`"
# 编译；
# build 目录
create_dir build

# android_abi
arr=("armeabi-v7a" "arm64-v8a")
for value in ${arr[@]}
do
  cd ${BUILD_HOME}/build
  # 按cpu架构创建目录；
  create_dir $value
  cd $value
  rm -rf *

  cmake ../.. \
  -DANDROID_ABI=$value \
  -DANDROID_STL=c++_static \
  -DANDROID_NDK=/opt/android-sdk/ndk/16.1.4479499 \
  -DCMAKE_BUILD_TYPE=${build_mode} \
  -DCMAKE_TOOLCHAIN_FILE=/opt/android-sdk/ndk/16.1.4479499/build/cmake/android.toolchain.cmake

  make install
done
