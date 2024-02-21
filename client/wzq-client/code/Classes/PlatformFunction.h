//
//  PlatformFunction.h
//  GouJi
//
//  Created by mtt on 14-9-29.
//
//

#ifndef __GouJi__PlatformFunction__
#define __GouJi__PlatformFunction__

#include <string>
#include "Core/AppDef.h"

class PlatformFunction
{
public:
    // Launcher壳相关 START========================
    static void removeLaunchShadeImage();
    static void setPrivacyFlag(const int &nFlag); // 设置用户勾选或取消勾选隐私政策 0 -- 未勾选 1-- 勾选
    static void showPrivacyAgreement();
    static void showPrivacyGuide();
    static void showPrivacyGuideChildren();
    // Launcher壳相关 END========================
    // 退出游戏
    static void exitGame();
};

#endif /* defined(__GouJi__PlatformFunction__) */
