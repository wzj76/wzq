# 创建项目目录
if [ ! -d "/duole/gameclient" ]; then
        mkdir -p "/duole/gameclient"
fi

# 分支首次打包，需要先检出分支代码，统一使用 gitlab_jenkins 账户；
# 分支目录不存在，需要先 checkout 分支；
if [ ! -d "/duole/gameclient/baohuang-client" ]; then
    cd /duole/gameclient
    git clone https://code.duole.com/gj/baohuang-client.git
fi

# 进入分支目录，更新代码
cd /duole/gameclient/baohuang-client
# 分支切换；
# 放弃本地修改
git checkout .
git fetch
git checkout develop
git pull origin develop

# 进入工程目录
cd code
# 调用jenkins构建脚本；
# arg1 : gitlab分支目录名[develop master]；
# arg2 : 平台类型，目前支持android；
# arg3 : jenkins构建编号；
# arg4 : jenkins作业名称；
# arg5 = 打包类型[debug,release]
sh jenkins_build.sh develop android $BUILD_ID $JOB_NAME release