//
//  PlatformUtil.cpp
//  platform
//
//  Created by 张建民 on 2022/3/16.
//

#include "../PlatformUtil.hpp"
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include "common/AppConfig.hpp"

using namespace cocos2d;
using namespace std;

#define CLASS_NAME "com/duole/common/PlatformUtil"
extern "C"
{
    JNIEXPORT jstring JNICALL Java_com_duole_common_PlatformUtil_getChannelId(JNIEnv* env, jobject thiz)
    {
        // 读取配置信息
        std::string _channel = APPCONFIG.getString("channel");
        return env->NewStringUTF(_channel.c_str());
    }
}

namespace duole {

//  获取渠道号
static string s_sChannelId = "";
const char *PlatformUtil::getChannelId() {
    if (s_sChannelId.empty()) {
        s_sChannelId = APPCONFIG.getString("channel");
    }
    return s_sChannelId.c_str();
}

}


#endif
