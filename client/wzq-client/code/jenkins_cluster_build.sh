# jenkins脚本执行时，加载shell环境变量；
#!/bin/bash -ilex 
APP_HOME="`pwd -P`"
CLIENT_ROOT_PATH="${APP_HOME}/../.."
BUILD_TOOLS_PATH=${CLIENT_ROOT_PATH}/duole_client_base/tools

# 项目组简称:[gj,mj,pdk,sj]
APP_GROUP="pdk"
# 项目简称:[paohz,duolechess,gouji,baohuang,shuangkou,duolemj,shengji,guandan]
APP_NAME="paohz"

# 更新基类库
# 默认使用最新发布的版本，如有必要，可指定特定标签版本；
if [ ! -d "${CLIENT_ROOT_PATH}/duole_client_base" ]; then
    cd ${CLIENT_ROOT_PATH}
    git clone https://code.duole.com/common/duole_client_base.git
fi
cd ${CLIENT_ROOT_PATH}/duole_client_base
# 强制拉取远程分支和标签；
# 放弃本地修改
git checkout .
git fetch

# 获取最新的基类库版本标签
CLIENT_BASE_TAG=$(git describe --tags `git rev-list --tags --max-count=1`)

# 注释掉，如有必要可打开此项并自行指定版本标签；
# CLIENT_BASE_TAG="feature/use_bugly_v2"

echo "The client base : ${CLIENT_BASE_TAG}"

# 分支切换；
git checkout ${CLIENT_BASE_TAG}
git pull origin ${CLIENT_BASE_TAG}
echo "The client base pull done: ${CLIENT_BASE_TAG}"

# jenkins 集群打包
# 脚本参数说明：
# 	arg1 = gitlab分支目录[develop dist_common]；
# 	arg2 = 平台类型[android iOS]
# 	arg3 = jenkins作业构建编号
# 	arg4 = jenkins作业名
# 	arg5 = jenkins作业工作目录
# 	arg6 = 打包类型[debug,release]
# 	arg7 = 项目组简称:[gj,mj,pdk,sj]
# 	arg8 = 项目简称:[paohz,duolechess,gouji,baohuang,shuangkou,duolemj,shengji,guandan]
cd ${APP_HOME}
sh ${BUILD_TOOLS_PATH}/duole_jenkins_cluster_build.sh ${1} ${2} ${3} ${4} ${5} ${6} ${APP_GROUP} ${APP_NAME}
