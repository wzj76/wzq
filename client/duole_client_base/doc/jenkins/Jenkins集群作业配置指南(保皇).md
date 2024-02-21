# 0.约束条件
- 目前仅支持android项目打包；
- 打包脚本在 code 目录；
- published目录存放apk；
- gitlab 仓库根目录："https://code.duole.com/gj/baohuang-client.git"
- 打包服务器本地项目根目录：“/duole/gameclient//baohuang-client/"

# 1.jenkins_build.sh脚本说明
此脚本由 jenkins 作业调用，进行环境变量设置、代码更新、打包、包的管理；

**用法和步骤：**
- jenkins_build.sh 脚本拷贝到 code 目录下；
- 项目相关设置（保皇）
    ```shell
    # git 分组简称
    APP_GROUP="gj"
    # git 仓库简称
    APP_NAME="baohuang"
    # 基类库标签或者分支名称
    CLIENT_BASE_TAG="develop"
    ```
- 直接调用项目原有 distribute-android.sh 脚本打包，所以，需要在这个脚本里设置打包的渠道，并上传gitlab；

# 2.Jenkins作业设置

## 2.1 作业简介
```
    项目组：够级项目组（gj）
    项目：多乐保皇（baohuang）
    仓库目录：https://code.duole.com/gj/baohuang-client.git
    分支：develop
    功能：开发分支，辅助测试、开发人员打包；
```

## 2.2 作业构建脚本
```shell
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
# arg5 = jenkins作业工作目录
# arg6 = 打包类型[debug,release]
sh jenkins_cluster_build.sh develop android $BUILD_ID $JOB_NAME $WORKSPACE release
```

## 2.3 归档设置：（Archive the artifacts）
```
    用于存档的文件：“archive/*.apk, archive/*.txt”
```