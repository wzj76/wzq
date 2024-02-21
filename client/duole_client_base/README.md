# duole_client_base

多乐游戏客户端基类库
https://code.duole.com/common/duole_client_base.git
## 一、模块说明

- cocos2d-duole  
  基于 cocos2d-x-3.17.2 ，对引擎部分功能进行了修改和增强，所有多乐内部游戏项目基于此版本引擎进行构建;
- gamebase  
  多乐内部使用的通用 C++ 模块，目前包含：eff_comm、google-protobuf、rapidxml、socketx;
- third-sdks  
  多乐内部项目均需要引用的通用第三方类库，目前包含 bugly;
- sdks  
  多乐开发的公共组件，目前暂无内容;
- external  
  多乐集成使用的第三方开源代码库;
- tools  
  基类库通用工具脚本;
- doc  
  存放基类库相关文档;

## 二、基类库的发布

# 1、如何编译 cocos2d-duole
cocos2d-duole 以静态库方式应用到项目中，所以，对 cocos2d-duole 类库的修改，需要重新编译、发布。
```shell
# 进入编译脚本所在的目录
cd duole_client_base/libs/src
# 执行编译脚本[release, debug], 默认 debug
# 编译结果自动被发布到 libs/prebuilt 目录
sh build.sh release
```

# 2、编译结果发布
- 向基类库仓库提交源代码；
- 编译的静态库需要提交到 git 仓库；

## 三、开发环境

- Android:

```shell
export NDK_ROOT=/opt/android-sdk/ndk/16.1.4479499
export ANDROID_SDK_ROOT=/opt/android-sdk
```
```Groovy
// android 全面屏适配：SdkVersion 28(android.view.DisplayCutout)
compileSdkVersion 28
buildToolsVersion 29.0.3
minSdkVersion 19    // vivo 渠道为 21
targetSdkVersion 26
```

- iOS:  
```
XCode 版本：13.2.1
最低支持：iOS 9.0
```
