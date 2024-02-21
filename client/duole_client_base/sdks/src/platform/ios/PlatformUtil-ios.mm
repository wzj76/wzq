//
//  PlatformUtil-ios.m
//  platform
//
//  Created by 张建民 on 2022/3/18.
//

#include "../PlatformUtil.hpp"
#include <string>
#include "common/AppConfig.hpp"

using namespace std;

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

