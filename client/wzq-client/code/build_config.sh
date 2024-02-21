# 配置文件手动处理脚本
# (注意，仅以下两种情形需手动设置相关参数并执行此脚本)
# 1、Android：采用 Android Studio 以调试模式进行打包；
# 2、iOS：苹果打包目前为手动打包；
# 
# usage：
# sh build_config.sh

APP_HOME="`pwd -P`"
BUILD_TOOLS_PATH=${APP_HOME}/../../duole_client_base/tools

# 以下参数按实际情况填写；
CHANNEL_GROUP="duole"
CHANNEL="App Store RC"
APP_BUNDLEID="com.duole.rc.paohuzihd"
APP_VERSION="2.1.9"
APP_VERSION_CODE="2010900"
APP_NAME="多乐跑胡子"
# [android, ios]
PLATFORM="ios"

sh ${BUILD_TOOLS_PATH}/duole_build_appconfig.sh \
-p "${PLATFORM}" -n "${APP_NAME}" \
-c "${CHANNEL}" -g "${CHANNEL_GROUP}" \
-b "${APP_BUNDLEID}" -v "${APP_VERSION}" -d "${APP_VERSION_CODE}"
