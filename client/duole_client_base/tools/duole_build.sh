# 功能描述：
#	多乐游戏客户端通用打包脚本；
#	
#	约定：
#		1.此脚本为通用脚本，用于对基于多乐客户端框架（duole_client_base）构建的客户端程序
# 		  进行打包，支持 Android、iOS平台；
# 		2.脚本部署目录为：duolegame/dist_xxx/tools;
#
# 脚本参数说明：
#   -m [debug, release], 编译模式
# 	-p [android, ios], 目标平台
# 	-c [common, oppo, vivo ...], 渠道号
# 	-g [duole, ...], 渠道分组
# 
# Usage:
# 	sh tools/duole_build_tools.sh -p android -c common -g duole
# 

#!/bin/bash -ilex 
echo "Running command: duole_build"

# 临时环境变量设置
export NDK_ROOT=/opt/android-sdk/ndk/16.1.4479499
export ANDROID_SDK_ROOT=/opt/android-sdk
export PATH=$NDK_ROOT:$PATH
export PATH=$ANDROID_SDK_ROOT:$PATH
export PATH=$ANDROID_SDK_ROOT/tools:$ANDROID_SDK_ROOT/platform-tools:$PATH

# ndk-build
# 定制版 cocos2d-console
# usage: --help 查看帮助
# ../cocos2d-duole/tools/cocos2d-console/bin/cocos compile \
# 	--help 
# 新增参数说明：
# -c 渠道号 channel
# -g 渠道分组 channel group

function Usage() {
	echo "Usage:"
	echo " sh tools/duole_build -m release -p android -c common -g duole"
	echo " -m    : 编译模式，[release, debug]"
	echo " -p    : 目标平台，[android, ios]"
	echo " -a	 : APP_ABI属性，[armeabi-v7a, arm64-v8a], 可以使用':'分隔多个值" 
	echo " -c    : 渠道，[common, oppo, vivo ...]"
	echo " -g    : 渠道分组，[duole, ...]"
}

function build() {
	_build_mode=$1
	_platform=$2
	_app_abi=$3
	_channel=$4
	_channel_group=$5

	# 编译命令
	../../duole_client_base/cocos2d-duole/tools/cocos2d-console/bin/cocos compile \
		-p ${_platform} \
		-s . \
		--build-type cmake \
		--app-abi ${_app_abi} \
		-m ${_build_mode} \
		-c ${_channel} \
		-g ${_channel_group}
} 

# get the optional arguments
while getopts m:p:a:c:g: value
do
    case $value in
        m )
            build_mode=$OPTARG   # 从$OPTARG中获得参数值
            echo "build_mode = ${build_mode}"
            ;;
        p )
            platform=$OPTARG   # 从$OPTARG中获得参数值
            echo "platform = ${platform}"
            ;;
        a )
            app_abi=$OPTARG   # 从$OPTARG中获得参数值
            echo "app_abi = ${app_abi}"
            ;;
        c )
            channel=$OPTARG   # 从$OPTARG中获得参数值
            echo "channel = ${channel}"
            ;;
        g )
            channel_group=$OPTARG
            echo "channel_group = ${channel_group}"
            ;;
    esac
done

build ${build_mode} ${platform} ${app_abi} ${channel} ${channel_group}