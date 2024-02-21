# 功能描述：
#	多乐游戏客户端通用配置文件处理脚本；
# 	配置文件处理过程：
# 	1. 配置文件模板（appconfig.json）放在项目根目录，由开发者完成个性化设置；
# 	2. 脚本处理过程和内容：
# 		a. 复制模板并生成 app.properties 属性文件；
# 		b. 替换 clientbase 及其关联模块版本信息；（clientbase、cocos2d-x、bugly ）
# 		c. 替换 app 基本属性信息；（channel、channel_group、bundleid、version、version_code ）
# 		d. xxtea方式加密 app.properties，生成加密文件 Resources/appconfig.dat;
# 	3. app启动时可通过配置管理模块对加密的配置文件进行读取；
#	
#	约定：
#		1.此脚本为通用脚本，用于对基于多乐客户端框架（duole_client_base）构建的客户端程序
# 		  配置文件进行处理：渠道信息替换、文件加密；
#    	2.配置文件名称：appconfig.json, 加密后的文件名：appconfig.dat;
# 		3.配置文件位置：
# 			原文件位置：xxx-client/code/appconfig.json
# 			加密后：xxx-client/code/Resources/appconfig.dat
# 			明文属性文件：xxx-client/code/Resources/app.properties
# 		4.生成 app.properties(只读，方便业务工程维护者识别app依赖的相关组件版本)
#
# 脚本参数说明：
# 	-p [android, ios], 目标平台
# 	-c [common, oppo, vivo ...], 渠道号
# 	-g [duole, ...], 渠道分组
# 	-b bundleid
# 	-v app_version
# 	-d app_version_code
#   -n app_name
# Usage:
# 	sh tools/duole_build_appconfig.sh -c common -g duole -b com.duole.xxx -v 1.0.0 -d 100300 多乐跑胡子
# 

#!/bin/bash -ilex 
echo "Running command: duole_build_appconfigv2"

# 配置文件名称；
SRC_CFG_FILE="appconfig.json"
# 加密后的文件名；
DST_ENCRYPT_FILE="appconfig.dat"
# app 属性文件名；
DST_APP_PROPERTIES_FILE="app.properties"
# 资源目录名
CFG_FILE_PATH="Resources"
# 秘钥和签名
XXTEA_KEY="zhaoxin"
XXTEA_SIGN="7k7kgame"

CLIENT_BASE_ROOT="../../duole_client_base"
APP_ANDROID_HOME="proj.android"

# version.json 中的字段名
KEY_CLIENTBASE="clientbase"
KEY_COCOS2DX="cocos2d-x"
KEY_BUGLY_IOS="bugly-ios"
KEY_BUGLY_ANDROID="bugly-android"

# 基类库及随基类库发布的组件信息（version.json）
# 根据传入的 key ,从 version.json 获取对应的 value
# 注意去掉行尾逗号，去除头尾双引号；
function getValueByKey() {
    value=`awk '$1=$1' ${CLIENT_BASE_ROOT}/version.json | awk -F ":" '{ if($1 == "\"'${1}'\"") print $2}' | sed 's/,$//' | sed 's/\"//g'`
    echo $value
}

# 根据平台类型[iOS, Android], 读取不同的配置
function do_clientbase_info() {
	local _platform=$1
	local _dst_file=$2
    local VALUE_CLIENT_BASE=$(getValueByKey ${KEY_CLIENTBASE})
    local VALUE_COCOS2DX=$(getValueByKey ${KEY_COCOS2DX})
	# bugly 的版本信息，需要区分 iOS、Android
	local VALUE_BUGLY=" "
	if [[ ${_platform} = "iOS" ]] || [[ ${_platform} = "ios" ]]; then
		VALUE_BUGLY=$(getValueByKey ${KEY_BUGLY_IOS})
	elif [[ ${_platform} = "Android" ]] || [[ ${_platform} = "android" ]]; then
		VALUE_BUGLY=$(getValueByKey ${KEY_BUGLY_ANDROID})
	else
		echo "Invalid args !"
		Usage
	fi

	# 如果目标文件存在，执行替换
	if [ -f "${_dst_file}" ];then
		# 替换 clientbase 及其关联组件信息
		sed -i '' "s/\${CLIENTBASE_VERSION}/${VALUE_CLIENT_BASE}/g" "${_dst_file}"
		sed -i '' "s/\${COCOS2DX-VERSION}/${VALUE_COCOS2DX}/g" "${_dst_file}"
		sed -i '' "s/\${BUGLY_VERSION}/${VALUE_BUGLY}/g" "${_dst_file}"
	else
		echo "do_clientbase_info : ${_dst_file} 文件不存在!"
	fi	
}

# 设置 App 渠道及版本信息；
# arg1: 平台类型
# arg2: 目标文件
# arg3: 渠道组
# arg4: 渠道号
# arg5: bundleid
# arg6: version
# arg7: version_code
# arg8: app_name
function do_app_info() {
	local _platform=$1
	local _dst_file=$2
	local _channel=$3
	local _channel_group=$4
	local _bundleid=$5
	local _version=$6
	local _version_code=$7
	local _app_name=$8
	# 如果目标文件存在，执行替换
	if [ -f "${_dst_file}" ];then
		# 替换 app 相关属性信息
		sed -i '' "s/\${DUOLE_CHANNEL_GROUP}/${_channel_group}/g" "${_dst_file}"
		sed -i '' "s/\${DUOLE_CHANNEL}/${_channel}/g" "${_dst_file}"
		sed -i '' "s/\${DUOLE_APP_BUNDLEID}/${_bundleid}/g" "${_dst_file}"
		sed -i '' "s/\${DUOLE_APP_VERSION}/${_version}/g" "${_dst_file}"
		sed -i '' "s/\${DUOLE_APP_VERCODE}/${_version_code}/g" "${_dst_file}"
		sed -i '' "s/\${DUOLE_APP_NAME}/${_app_name}/g" "${_dst_file}"
	else
		echo "do_app_info : ${_dst_file} 文件不存在!"
	fi	
}

# 从 build.gradle 中提取指定sdk 版本信息
SDKS=("launcher" "webview" "login")
function do_sdk_info_android() {
	local _dst_file=$2
	echo ${_dst_file}
	local _gradle_file="${APP_ANDROID_HOME}/app/build.gradle"
	echo ${_gradle_file}

	for value in ${SDKS[@]}
	do
		sdk_version=`awk -F "'" '{print $2}' ${_gradle_file} | awk -F ":" '$2=="'${value}'" {print $3}' `
		echo $value":"$sdk_version
		# 替换
		if [[ ${value} = "launcher" ]]; then
			sed -i '' "s/\${LAUNCHER_VERSION}/${sdk_version}/g" "${_dst_file}"
		elif [[ ${value} = "webview" ]]; then
			sed -i '' "s/\${WEBVIEW_VERSION}/${sdk_version}/g" "${_dst_file}"
		elif [[ ${value} = "login" ]]; then
			sed -i '' "s/\${LOGIN_VERSION}/${sdk_version}/g" "${_dst_file}"
		fi
	done
}

function build_appconfig() {
	if [ -f "${SRC_CFG_FILE}" ]; then
		# 拷贝 appconfig.json, 重命名为 app.properties ;
		cp -rf ${SRC_CFG_FILE} ${DST_APP_PROPERTIES_FILE}
		# 设置基类库信息
		do_clientbase_info $6 ${DST_APP_PROPERTIES_FILE}		
		# 设置 app 基本信息
		do_app_info $6 ${DST_APP_PROPERTIES_FILE} "$1" "$2" "$3" "$4" "$5" "$7"
		# 设置 sdk 信息
		do_sdk_info_android $6 ${DST_APP_PROPERTIES_FILE}
		# 加密并将加密文件挪到目标位置
		local _dst_encrypt_file="${CFG_FILE_PATH}/${DST_ENCRYPT_FILE}"
		python ${CLIENT_BASE_ROOT}/tools/xxtea_crypto.py -m encrypt \
		-k ${XXTEA_KEY} -s ${XXTEA_SIGN} -i ${DST_APP_PROPERTIES_FILE} -o ${_dst_encrypt_file}
	else
		echo "build : ${SRC_CFG_FILE} 文件不存在!"
	fi
}

function Usage() {
	echo "Usage:"
	echo " sh tools/duole_build_appconfig.sh -c common -g duole -b com.duole.xxx -v 1.0.0 -d 100300 android"
	echo " -c    : 渠道，[common, oppo, vivo ...]"
	echo " -g    : 渠道分组，[duole, ...]"
	echo " -b    : bundleid"
	echo " -v    : app_version"
	echo " -d    : app_version_code"
	echo " -n    : app_name"
	echo " -p    : platform,[ios, android]"
}

# get the optional arguments
while getopts c:g:b:v:d:p:n: value
do
    case $value in
        p )
            PLATFORM=$OPTARG   
            echo "platform = ${PLATFORM}"
            ;;
        c )
            CHANNEL=$OPTARG   # 从$OPTARG中获得参数值
            echo "channel = ${CHANNEL}"
            ;;
        g )
            CHANNEL_GROUP=$OPTARG
            echo "channel_group = ${CHANNEL_GROUP}"
            ;;
        b )
            APP_BUNDLEID=$OPTARG
            echo "app_bundleid = ${APP_BUNDLEID}"
            ;;
        v )
            APP_VERSION=$OPTARG
            echo "app_version = ${APP_VERSION}"
            ;;
        d )
            APP_VERSION_CODE=$OPTARG
            echo "app_version_code = ${APP_VERSION_CODE}"
            ;;
        n )
            APP_NAME=$OPTARG
            echo "app_name = ${APP_NAME}"
            ;;
    esac
done

build_appconfig "${CHANNEL}" "${CHANNEL_GROUP}" \
"${APP_BUNDLEID}" "${APP_VERSION}" "${APP_VERSION_CODE}" "${PLATFORM}" "${APP_NAME}"
