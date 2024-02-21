# 功能描述：
#	多乐游戏客户端通用发布脚本；
#	
#	约定：
#		此脚本为通用脚本，用于对基于多乐客户端框架（duole_client_base）构建的客户端程序
# 		进行打包，支持 Android、iOS平台；
#
# 脚本参数说明：
#   -m [debug, release], 编译模式
# 	-p [android, ios], 目标平台
# 	-g [duole, ...], 渠道分组
# 
# Usage:
# 	sh tools/duole_distribute.sh -m release -p android -g duole
# 

APP_HOME="`pwd -P`"
APP_ANDROID_HOME=${APP_HOME}/proj.android
BUILD_TOOLS_PATH=${APP_HOME}/../../duole_client_base/tools

# 提取 app 信息：Android
function getAndroidAppInfo() {
	# app_name: proj.android/app/src/main/res/values/strings.xml
	local strings_xml=${APP_ANDROID_HOME}/app/src/main/res/values/strings.xml
	APP_NAME=`awk -F '"' '{ if($2 == "app_name") print$3}' ${strings_xml} |awk -F "<" '{print $1}'|awk -F ">" '{print $2}'`

	# 版本信息和bundleId，直接从gradle文件获取；
	APP_BUNDLEID=`awk '$1=="applicationId" {print $2}' ${APP_ANDROID_HOME}/app/build.gradle`
	APP_VERSION=`awk '$1=="versionName" {print $2}' ${APP_ANDROID_HOME}/app/build.gradle`
	APP_VERSION_CODE=`awk '$1=="versionCode" {print $2}' ${APP_ANDROID_HOME}/app/build.gradle`
	# 去除头尾双引号
	APP_BUNDLEID=`echo ${APP_BUNDLEID} | sed 's/\"//g'`
	APP_VERSION=`echo ${APP_VERSION} | sed 's/\"//g'`
	echo "The APP_NAME is ${APP_NAME}"
	echo "The APP_BUNDLEID is ${APP_BUNDLEID}"
	echo "The APP_VERSION is ${APP_VERSION}"
	echo "The APP_VERSION_CODE is ${APP_VERSION_CODE}"
}

function getAppleAppInfo() {
	echo "getAppleAppInfo: unsupported!!!"
	# TODO: iOS AppInfo
}

# 清理
function clean_android() {
	if [[ $1 = "yes" ]]; then
		echo "clean (yes): ... "
		sh ${BUILD_TOOLS_PATH}/duole_clean.sh -p android	
	else
		echo "clean (no): Nothing to do !"
	fi		
}
# 向 git 仓库提交 app.properties 文件
function archive_app_properties() {
	echo "archive app.properties: ..."
	# 获取当前分支名称
	cur_branch_name=`git rev-parse --abbrev-ref HEAD`
	# 提交前先 pull 
	git pull origin ${cur_branch_name}
	# add && commit && push(app.properties)
	git add app.properties
	git commit -m "jenkins build: update app.properties ."
	git push origin ${cur_branch_name}
}

# 构建和打包
# arg1 : 渠道组
# arg2 : 渠道号
function build() {
	_channel_group=$1
	_channel=$2
	echo "Build ... ANDROID_CHANNEL_GROUP=${_channel_group} ANDROID_CHANNEL=${_channel}"
	# 配置文件处理；
	sh ${BUILD_TOOLS_PATH}/duole_build_appconfig.sh -c "${_channel}" -g "${_channel_group}"\
	-b "${APP_BUNDLEID}" -v "${APP_VERSION}" -d "${APP_VERSION_CODE}" -p "${PLATFORM}" -n "${APP_NAME}"
	sh ${BUILD_TOOLS_PATH}/duole_build.sh -m ${BUILD_MODE} -p android -a armeabi-v7a:arm64-v8a -c ${_channel} -g ${_channel_group}
}

# 发布：Release，一般不修改
# arg: 渠道组名称
function distribute() {
	_channel_group=${1}
	_channel_file="channels.txt"
	for _channel in `cat $_channel_file | grep -v "#" | grep -v "^$"`
	do
		if [ -n "$_channel" ]; then 
			echo "Distribute ... ANDROID_CHANNEL_GROUP=${_channel_group} ANDROID_CHANNEL=${_channel}"
			# 配置文件处理；
			sh ${BUILD_TOOLS_PATH}/duole_build_appconfig.sh -c "${_channel}" -g "${_channel_group}"\
			-b "${APP_BUNDLEID}" -v "${APP_VERSION}" -d "${APP_VERSION_CODE}" -p "${PLATFORM}" -n "${APP_NAME}"
			sh ${BUILD_TOOLS_PATH}/duole_build.sh -m release -p android -a armeabi-v7a:arm64-v8a -c "${_channel}" -g "${_channel_group}"
    	else
			echo "channel is empty string!"	
		fi
	done

	# 提交 app.properties 文件
	archive_app_properties
}

function Usage() {
	echo "Usage:"
	echo " sh tools/duole_distribute.sh -m release -g duole"
	echo " -m    : 编译模式，[release, debug]"
	echo " -p    : 目标平台，[android, ios]"
	echo " -g    : 渠道分组，[duole, ...]"
	echo " -c    : 编译前清理，[yes, no]"
}

# get the optional arguments
while getopts m:p:g:c: value
do
    case $value in
        m )
            BUILD_MODE=$OPTARG   # 从$OPTARG中获得参数值
            echo "build_mode = ${BUILD_MODE}"
            ;;
        p )
            PLATFORM=$OPTARG   # 从$OPTARG中获得参数值
            echo "platform = ${PLATFORM}"
            ;;
        g )
            CHANNEL_GROUP=$OPTARG
            echo "channel_group = ${CHANNEL_GROUP}"
            ;;
        c )
            CLEAN_MODE=$OPTARG   # 从$OPTARG中获得参数值
            echo "clean = ${CLEAN_MODE}"
            ;;
    esac
done

function doBuild() {
	# 清理
	clean_android ${CLEAN_MODE}
	# 获取 Android app 基本信息
	getAndroidAppInfo

	# 如果是 release, 须上传符号文件；
	if [[ ${BUILD_MODE} = "release" ]]; then
		distribute ${CHANNEL_GROUP}
		echo "Upload Symbol ... "
		sh ./upload_symbol.sh
	else
		# debug模式，默认渠道号：debugger
		build ${CHANNEL_GROUP} "debugger"
	fi
}

# 此脚本目前仅支持 android 打包；
if [[ ${PLATFORM} = "android" ]]; then
	doBuild
else
	echo "Wanging: iOS is unsupported!!!"
fi




