# 打包脚本；
# usage：
# sh distribute-android.sh debug
# sh distribute-android.sh release

APP_HOME="`pwd -P`"
BUILD_TOOLS_PATH=${APP_HOME}/../../duole_client_base/tools

BUILD_MODE=${1}
CHANNEL_GROUP="duole"

# -c yes 表示需要清理上次打包残留文件；
# -c no 表示不需要清理，直接打包（不推荐）；
sh ${BUILD_TOOLS_PATH}/duole_distribute.sh \
-m ${BUILD_MODE} -p android -g ${CHANNEL_GROUP} -c yes