# 功能描述：
#	多乐游戏客户端编译残留数据清理脚本；
#	
#	约定：
#		1.此脚本为通用脚本，用于对基于多乐客户端框架（duole_client_base）构建的客户端程序
# 		  进行残留编译数据清理，支持 Android、iOS平台；
# 		2.脚本部署目录为：duolegame/dist_xxx/tools;
#
# 脚本参数说明：
# 	-p [android, ios], 指定目标平台
# 
# Usage:
# 	sh tools/duole_clean.sh -p android
# 

#!/bin/bash -ilex 
echo "Running command: duole_clean"

# 临时环境变量设置：Android
export NDK_ROOT=/opt/android-sdk/ndk/16.1.4479499
export ANDROID_SDK_ROOT=/opt/android-sdk
export PATH=$NDK_ROOT:$PATH
export PATH=$ANDROID_SDK_ROOT:$PATH
export PATH=$ANDROID_SDK_ROOT/tools:$ANDROID_SDK_ROOT/platform-tools:$PATH

# iOS

# 用法
function Usage() {
	echo "Usage:"
	echo " sh tools/duole_clean.sh -p platform"
	echo " platform : 平台类型, [android, ios]"
}
# 清理 iOS 打包环境
function clean_ios() {
	echo "TODO: 清理iOS编译内容"
}

# 清理 android 打包环境
function clean_android() {
	cd proj.android
	./gradlew clean
	# 清理 Native 编译相关内容；
	# .cxx、build
	cd app
	rm -rf .cxx
	rm -rf build
	cd ../..
}

# 清理 gradle 缓存
function clean_gradle_cache() {
	GRADLE_PATH="/Users/`whoami`/.gradle"
	CACHE_PATH="${GRADLE_PATH}"/caches
	# clean: 组件同版本更新时需清理缓存；
	find ${CACHE_PATH} -name "com.duole.*" |xargs rm -Rf
}

# get the optional arguments
while getopts p: value
do
    case $value in
        p )
            platform=$OPTARG   # 从$OPTARG中获得参数值
            echo "platform = ${platform}"
            ;;
    esac
done

if [[ ${platform} = "ios" ]]; then
	clean_ios
elif [[ ${platform} = "android" ]]; then
	clean_android
	clean_gradle_cache
else
	echo "Invalid args !"
	Usage
fi
