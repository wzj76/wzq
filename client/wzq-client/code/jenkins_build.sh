# jenkins脚本执行时，加载shell环境变量；
#!/bin/bash -ilex 
APP_HOME="`pwd -P`"
CLIENT_ROOT_PATH="${APP_HOME}/../.."
BUILD_TOOLS_PATH=${CLIENT_ROOT_PATH}/duole_client_base/tools

# 项目组简称:[gj,mj,pdk,sj]
APP_GROUP="pdk"
# 项目简称:[paohz,duolechess,gouji,baohuang,shuangkou,duolemj,shengji,guandan]
APP_NAME="paohz"
# 基类库标签或者分支名称
CLIENT_BASE_TAG="v1.0.0_beta3"

# 更新基类库
if [ ! -d "${CLIENT_ROOT_PATH}/duole_client_base" ]; then
    cd ${CLIENT_ROOT_PATH}
    git clone https://code.duole.com/common/duole_client_base.git
fi
# 进入指定分支
cd ${CLIENT_ROOT_PATH}/duole_client_base
# 分支切换；
# 放弃本地修改
git checkout .
git fetch
git checkout ${CLIENT_BASE_TAG}
git pull origin ${CLIENT_BASE_TAG}
echo "The client base pull done: ${CLIENT_BASE_TAG}"

# jenkins 打包
# 脚本参数说明：
# 	arg1 = gitlab分支目录[develop dist_common]；
# 	arg2 = 平台类型[android iOS]
# 	arg3 = jenkins作业构建编号
# 	arg4 = jenkins作业名
# 	arg5 = 打包类型[debug,release]
# 	arg6 = 项目组简称:[gj,mj,pdk,sj]
# 	arg7 = 项目简称:[paohz,duolechess,gouji,baohuang,shuangkou,duolemj,shengji,guandan]
cd ${APP_HOME}
sh ${BUILD_TOOLS_PATH}/duole_jenkins_build.sh ${1} ${2} ${3} ${4} ${5} ${APP_GROUP} ${APP_NAME} 
