#!/bin/bash

# usage: 
# 示例:
# sh upload_symbol.sh

# 目录定义
APP_HOME="`pwd -P`"
APP_ANDROID_HOME=${APP_HOME}/proj.android

# bugly config: 
# 08_PHZ_Android 
APP_ID="04f6e9df3d"
APP_KEY="a98785d9-b621-4255-971d-62f5c6654a13"

# 版本和bundleId，直接从gradle文件获取；
APP_VERSION=`awk '$1=="versionName" {print $2}' ${APP_ANDROID_HOME}/app/build.gradle`
APP_BUNDLEID=`awk '$1=="applicationId" {print $2}' ${APP_ANDROID_HOME}/app/build.gradle`
echo "The APP_VERSION is ${APP_VERSION}"
echo "The APP_BUNDLEID is ${APP_BUNDLEID}"

# 定义 SO 目录；
APP_ANDROID_SO_PATH="${APP_ANDROID_HOME}/app/build"

# 执行符号文件上传命令；
cd ${APP_HOME}/../../duole_client_base/tools/buglyqq-upload-symbol
java -jar buglyqq-upload-symbol.jar \
                    -appid ${APP_ID} -appkey ${APP_KEY} \
                    -bundleid ${APP_BUNDLEID} \
                    -version ${APP_VERSION} \
                    -platform Android \
                    -inputSymbol ${APP_ANDROID_SO_PATH}