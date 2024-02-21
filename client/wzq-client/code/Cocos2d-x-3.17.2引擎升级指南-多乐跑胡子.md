# 目录结构
```
workspace                 //  工作空间目录
  cocos2d                 //  cocos2d引擎
  third-sdks               //  第三方 SDK：bugly、umeng等 ，iOS/Android Native编译部分；
  Libraries               //  多乐现有公共库  
  phzgame                 //  游戏项目：模板工程
    dist_common    
      Classes             //  源代码
      Resources           //  资源目录
      proj.android        //  android 工程
      proj.ios            //  iOS 工程
      clean.sh            //  清理脚本
      distribute.sh       //  编译打包脚本
```

# 一、环境准备
## 1、开发环境设置
### 第一步：
* Android: 
```shell
export COCOS2DX_ROOT=/opt/cocos2d-x-3.17.2
export NDK_ROOT=/opt/android-ndk-r16
export ANDROID_SDK_ROOT=/opt/adt-bundle-mac-x86_64-custom-8.1.0/sdk
```
```Groovy
// android 全面屏适配：SdkVersion 28(android.view.DisplayCutout)
compileSdkVersion 28
buildToolsVersion 29.0.3
minSdkVersion 19  
targetSdkVersion 26
```

* iOS:
XCode 版本：12.4
最低支持：iOS 9.0


## 2、拷贝模板工程 duolegame, 修改目录名为 phzgame;
* iOS 
  - Identity and Type ,修改工程名为 phzgame;
  - Manage Schemes, duolegame-mobile 修改为 phzgame-mobile;
* Android: 搜索替换 duolegame 为 phzgame;

# 二、代码迁移
  源工程: dev_2.0.x 
  新工程: dist_common
* 业务代码和资源拷贝：
    dev_2.0.x/Classes -> dist_common/classes
    dev_2.0.x/Resources -> dist_common/Resources
    dev_2.0.x/must_copy -> dist_common/must_copy
    
* iOS 代码和资源：
    proj.ios_mac/ios -> proj.ios_mac/ios
    proj.ios_mac/OpenSDK1.8.7_NoPay -> proj.ios_mac/OpenSDK1.8.7_NoPay
    proj.ios_mac/UdeskSDK -> proj.ios_mac/UdeskSDK
    proj.ios_mac/umsdk -> proj.ios_mac/umsdk
    proj.ios_mac/KeychainAccessGroups.plist -> proj.ios_mac/KeychainAccessGroups.plist
    
* Android 代码和资源：
    proj.android-studio/sdk_udesksdkui -> proj.android/sdk_udesksdkui
    proj.android-studio/app/libs -> proj.android/app/libs
    proj.android-studio/app/src/main/AndroidManifest.xml -> proj.android/app/AndroidManifest.xml

    proj.android-studio/app/src/main/res -> proj.android/app/src/main/res
    proj.android-studio/app/src/main/jni/cocos2dcpp/PlatformFunction.cpp -> proj.android/app/src/main/jni/cocos2dcpp/PlatformFunction.cpp
    proj.android-studio/app/src/main/java/com -> proj.android/app/src/main/java/com
    删除新工程 proj.android/app/src/main/java/org 目录

# 三、工程配置
* iOS


* Android
1.修改 proj.android/settings.gradle
```
include ':sdk_udesksdkui'
project(':sdk_udesksdkui').projectDir = new File(settingsDir, 'sdk_udesksdkui')
rootProject.name = "phzgame"
```
2.修改 proj.android/app/build.gradle
```
    applicationId "com.duole.paohuzihd"
    versionCode 2000700
    versionName "2.0.7"
    manifestPlaceholders= [qqappid: "1106072314"]

```

```
    // Udesk客服SDK依赖 BEGIN
    implementation project(':sdk_udesksdkui')
    // Udesk客服SDK依赖 END
    implementation 'com.android.support:support-v4:27.1.1'
    // 微信SDK依赖 BEGIN
    implementation 'com.tencent.mm.opensdk:wechat-sdk-android-without-mta:6.6.4'
    // 微信SDK依赖 END

    // 友盟统计SDK依赖 BEGIN
    implementation  'com.umeng.umsdk:common:9.3.8'// 必选
    implementation  'com.umeng.umsdk:asms:1.2.2'// 必选
    // 友盟统计SDK依赖 END

    // 友盟分享SDK依赖 BEGIN
    implementation  'com.umeng.umsdk:share-core:7.1.4'//分享核心库，必选
    implementation  'com.umeng.umsdk:share-qq:7.1.4'
    implementation  'com.tencent.tauth:qqopensdk:3.51.2'//QQ官方SDK依赖库
    implementation  'com.umeng.umsdk:share-wx:7.1.4'
    implementation  'com.tencent.mm.opensdk:wechat-sdk-android-without-mta:6.6.5'//微信官方SDK依赖库
    // 友盟分享SDK依赖 END    
```

3.修改 app/AndroidManifest.xml
```
<meta-data android:name="android.app.lib_name"
                   android:value="phzgame" />

```


# 编译错误处理
- CocosDenshion
```
error: use of undeclared identifier 'CocosDenshion' 
修改：
    // #define USE_AUDIO_ENGINE 1
    #define USE_SIMPLE_AUDIO_ENGINE 1

    #if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
    #error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
    #endif

    #if USE_AUDIO_ENGINE
    #include "audio/include/AudioEngine.h"
    using namespace cocos2d::experimental;
    #elif USE_SIMPLE_AUDIO_ENGINE
    #include "audio/include/SimpleAudioEngine.h"
    using namespace CocosDenshion;
    #endif
```

- rapidjson
```
error: no member named 'MemberonBegin' in 'rapidjson
修改 :
    MemberonBegin、MemberonEnd 修改为：MemberBegin、MemberEnd；
```
```
问题：[]方式读取字段时，如果json中不存在此字段，就会导致崩溃；
修改：
    []操作前，先调用 HasMember 方法进行判断，存在才读取；
    示例：
    if (obJson.HasMember("erroptions")) {
        const rapidjson::Value& obOption = obJson["erroptions"];
    }
```
- coco2d::ui::Button
```
no type named 'Button' in namespace 'cocos2d::ui'
修改:
    报错文件中增加 #include "ui/UIButton.h"
```
- sys/timeb.h
```
fatal error: 'sys/timeb.h' file not found
修改:
    未使用 ftime 及 timeb，且API-Level > 19时，已取消这个文件，直接注释掉；
    // #include <sys/timeb.h>
```
- register_ui_moudle
```
error: use of undeclared identifier 'register_ui_moudle'; did you mean 'register_ui_module'
修改: 
    register_ui_moudle 修改为 register_ui_module;

error: variable has incomplete type 'const struct luaL_reg'
修改：
    lua->luaprotobuf下的 pb.c修改：luaL_reg 修改为 luaL_Reg
```
- machine/endian.h
```
fatal error: 'machine/endian.h' file not found
修改:
    pb.c 文件增加 _ALLBSD_SOURCE 宏定义；
```
- DLHttpClient 
```
fatal error: 'network/DLHttpClient.h' file not found
修改：
    从原项目 cocos2dx/cocos/network下拷贝 DLHttpClient.h、DLHttpClient.cpp、DLHttpRequest.h、DLHttpResponse.h到新项目相同位置；
```

- AssetsManagerEx
```
error: call to non-static member function without an object argument
修改：
    cocos2d::extension::AssetsManagerEx::decompress
    增加静态方法 decompress_ex、basename
    static bool decompress_ex(const std::string &filename);
    static std::string basename(const std::string& path) ;
```

- extensions/assets-manager/Downloader.h
```
fatal error: 'extensions/assets-manager/Downloader.h' file not found
修改：
    从原项目 cocos2d/extensions/assets-manager/下拷贝 Downloader.h 、Downloader.cpp到新项目相同位置；
    cocos2d/extensions/Android.mk :
    LOCAL_SRC_FILES += assets-manager/Downloader.cpp 
```

- EditBox
```
error: no member named 'INTIAL_CAPS_ALL_CHARACTERS' in 'cocos2d::ui::EditBox::InputFlag'; did you mean 'INITIAL_CAPS_ALL_CHARACTERS'?
修改：
    INTIAL_CAPS_ALL_CHARACTERS 修改为：INITIAL_CAPS_ALL_CHARACTERS
```

- Shake
```
error: use of undeclared identifier 'Shake'
修改：
    同步原工程 CCActionInterval.h、CCActionInterval.cpp文件与 Shake相关内容到新项目；
```

- Spine
**Spine兼容性问题待处理，暂时用测试资源替换**
```
error: no viable conversion from '__bind<void (PHZSkeletonGirl::*)(int), PHZSkeletonGirl *, const std::__ndk1::placeholders::__ph<1> &>' to 'const spine::EndListener' (aka 'const function<void (spTrackEntry *)>')
修改：
    void onGirlEnd(int nTrackIndex);
    void onGirlComplete(int nTrackIndex, int nLoopCount);
    修改为：
    void onGirlEnd(spTrackEntry* entry);
    void onGirlComplete(spTrackEntry* entry);    
```
```shell
Spine 2.1.27 动画在 Spine runtime 3.6 播放：
spine使用时运行异常  Assert failed: Unknown attachment type: skinnedmesh
修改：        
    骨骼文件导出的json中type为skinnedmesh，改为mesh
# 查找脚本
find . -type f -path "*.json" |xargs grep 'skinnedmesh'
# 替换脚本
find . -type f -path "*.json" |xargs sed -i '' 's/skinnedmesh/mesh/g'
```
```
龙骨导出的 Spine 格式动画需要在skeleton头中 额外增加版本号信息；
"spine": "2.1.27"
"skeleton":{
    "hash":"3/ao7IB2csmEDr6MvZE5T/TBOYY",
    "spine":"2.1.27",
    "width":475.75,
    "height":209.15,
    "images":"./images/"
}
```

- libcurl
```
fatal error: 'curl/curl.h' file not found
修改：
    cocos2d/cocos/Android.mk,
    LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../curl/include/android 
    LOCAL_STATIC_LIBRARIES += ext_curl
    $(call import-module,curl/prebuilt/android)
```

- ScrollView
```
问题：item 点触失效；
修改：  
    m_pScrollView = ScrollView::create();
    m_pScrollView->setSwallowTouches(false);
```


- resetOriginalRect（by lzq）
```
问题：横竖版切换(iOS) ,resetOriginalRect 未定义导致崩溃；
修改：
    CCEAGLView-ios.mm、CCEAGLView-ios.mm
    - (id) resetOriginalRect:(CGRect)frame;
    
    [self.view resetOriginalRect:[UIScreen mainScreen].bounds];
```

- Android Studio Debug 
无法以调试模式启动app
```
错误信息：
    Error while starting native debug session: com.intellij.execution.ExecutionException: Unsupported device. This device cannot be debugged using the native debugger. See log file for details.
解决办法：
    Android Studio中【Run】->【Edit Configurations...】->点击【app】，看右边【debuger】->【debug type】

    选择java only就可以了（根据自己调试代码情况，也可以选择其他的，如Native only等）
```
