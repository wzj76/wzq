# 功能描述：
#	Jenkins 集群通用打包脚本；
#
# 	版本说明(脚本在下述环境测试通过)：
# 		ANDROID_HOME=/opt/android-sdk
#		NDK_ROOT=/opt/android-sdk/ndk/16.1.4479499
#	约定：
#		1.此脚本拷贝到对应的工程根目录（code）下执行；
#		2.published目录存放apk；
#		3.gitlab仓库根目录："https://code.duole.com/xx/xxx-client.git"
#		4.打包服务器本地项目根目录：“/duole/gameclient/xxx-client“
#
# 脚本参数说明[jenkins 集群模式]：
# 	arg1 = gitlab分支目录[develop dist_common]；
# 	arg2 = 平台类型[android iOS]
# 	arg3 = jenkins作业构建编号
# 	arg4 = jenkins作业名
# 	arg5 = jenkins作业工作目录
# 	arg6 = 打包类型[debug,release]
# 	arg7 = 项目组简称:[gj,mj,pdk,sj]
# 	arg8 = 项目简称:[paohz,duolechess,gouji,baohuang,shuangkou,duolemj,shengji,guandan]
# jenkins脚本执行时，加载shell环境变量；
#!/bin/bash -ilex 

# 临时环境变量设置
export NDK_ROOT=/opt/android-sdk/ndk/16.1.4479499
export ANDROID_SDK_ROOT=/opt/android-sdk
export PATH=$NDK_ROOT:$PATH
export PATH=$ANDROID_SDK_ROOT:$PATH
export PATH=$ANDROID_SDK_ROOT/tools:$ANDROID_SDK_ROOT/platform-tools:$PATH
export LANG=en_US.UTF-8
export LC_CTYPE="zh_CN.UTF-8"

# 确定项目组和产品名称；
arg7=${7}
APP_GROUP="${arg7}"
if [[ ${APP_GROUP} ]]; then
	echo "The Team is ${APP_GROUP}"
else
	echo "Please set app group !!!"
	exit
fi
arg8=${8}
APP_NAME="${arg8}"
if [[ ${APP_NAME} ]]; then
	echo "The App Name is ${APP_NAME}"
else
	echo "Please set app name !!!"
	exit
fi

GIT_REMOTE="https://code.duole.com/${APP_GROUP}/${APP_NAME}-client.git"
APP_HOME="`pwd -P`"
APP_ANDROID_HOME="${APP_HOME}/proj.android"

# 进入项目目录；
cd ${APP_HOME}
# 声明代码仓库分支名称：第一个参数
arg1=${1}
# 默认分支 develop
if [[ ${arg1} ]]; then
	echo "The branch is ${arg1}"
else
	arg1="develop"
	echo "The default branch is ${arg1}"
fi
# jenkins作业构建编号：BUILD_ID
arg3=${3}
# 默认00
if [[ ${arg3} ]]; then
	echo "The build_id is ${arg3}"
else
	arg3="0"
	echo "The default build_id is ${arg3}"
fi

# jenkins作业名：JOB_NAME
arg4=${4}
# 默认00
if [[ ${arg4} ]]; then
	echo "The JOB_NAME is ${arg4}"
else	
	echo "Error, Please check the param :JOB_NAME is nil"
	exit
fi
# jenkins作业工作目录：WORKSPACE
arg5=${5}
# 默认00
if [[ ${arg5} ]]; then
	echo "The JOB_WORKSPACE is ${arg5}"
else	
	echo "Error, Please check the param :JOB_WORKSPACE is nil"
	exit
fi

# 默认打包release
arg6=${6}
if [[ $arg6 != "debug" ]]; then
	arg6="release"		
fi

# 1.代码更新
echo "code update ready ... \n"
# 放弃本地修改
git checkout .
git fetch
# 切换到指定分支；
localBranch=$(git branch|awk '{print $2}')
if [[ ${localBranch} != ${arg1} ]]; then
	# 检查远程分支
    echo "switch branch to ${arg1} "
    git checkout ${arg1}
    if [[ $? != 0 ]]; then 
    	echo "git checkout failed !!!"
    	exit 
    fi	
fi

# 更新代码
echo "git pull ... ${arg1}"
git pull origin ${arg1}
if [[ $? != 0 ]]; then 
	echo "git pull failed !!!"
	exit 
fi	

# 显示指定分支最后提交的 SHA1 代码
# git rev-parse --verify develop
git_commit_sha1=$(git rev-parse --verify ${arg1})
if [[ $? != 0 ]]; then
	echo "error: the branch:${arg1} is not exist!!!"
	exit 
else
	echo $git_commit_sha1
fi

last_commit_info=$(git log --pretty=format:"\
sha1='%H'\n\
author='%an(%ae)'\n\
committer='%cn(%ce)'\n\
date='%ad'\n\
message='%s'\n" -1)
# last_commit_info=$(git log --pretty=format:"%H - %an(%ae),%cn(%ce), %ad : %s" -1)
echo "code update finished! \n"

# 2.打包
# 构建
chmod +x ./distribute-android.sh 
./distribute-android.sh ${arg6}

# 3.包输出
# apk目录
published_dir="${APP_HOME}/published"
if [ ! -d "$published_dir" ]
then
	mkdir -p $published_dir
fi

# 拷贝apk文件并从原目录删除
# 清理目录
rm -rf $published_dir/*.apk
rm -rf $published_dir/*.txt
mv ${APP_ANDROID_HOME}/dists/${arg6}/*.apk $published_dir/

# Archive the artifacts:jenkins归档，方便下载；
JOB_NAME=${arg4}
BUILD_ID=${arg3}
JOB_WORKSPACE=${arg5}
ARCHIVE_PATH=${JOB_WORKSPACE}/archive

if [ ! -d "$ARCHIVE_PATH" ]
then
	mkdir -p $ARCHIVE_PATH
fi
echo $ARCHIVE_PATH
rm -rf $ARCHIVE_PATH/*.*

# 包体信息提取
for file in $published_dir/*.apk; do
    echo $file
	tmp=${file##*/}
	# echo $tmp
	# echo ${tmp%.*}

	# 包体信息提取
	package_info_str=$(${ANDROID_SDK_ROOT}/build-tools/27.0.1/aapt dump badging $file)

	package_info_file=${published_dir}/${tmp%.*}.txt
	# 输出包体信息到本地文本文件，可选（默认输出）；
	rm -rf ${package_info_file}
	# 输出工程提交信息
	echo ${last_commit_info} >>${package_info_file}
	echo ${package_info_str} >>${package_info_file}

	# # upload
	# curl \
	#   -X POST \
	#   -H "Accept: application/json" \
	#   -F "app_group=${APP_GROUP}" \
	#   -F "app_name=${APP_NAME}" \
	#   -F "git_branch=${arg1}" \
	#   -F "commit_id=${git_commit_sha1}" \
	#   -F "package_info='${package_info_str}'" \
	#   -F "commit_info='${last_commit_info}'" \
	#   -F "package=@${file}" \
	#   http://140.143.196.146:9911/api/package/upload/v1
	
	# upload
	curl \
	  -X POST \
	  -H "Accept: application/json" \
	  -F "job_name=${arg4}" \
	  -F "build_id=${arg3}" \
	  -F "package_file=@${file}" \
	  https://package-audit.duolegame.net.cn/api/packageWarehouse/upload/v1

	# 拷贝输出文件到 jenkins 对应的作业目录；	
	cp ${published_dir}/${tmp%.*}.* $ARCHIVE_PATH/
done