//
//  PlatformFunction.cpp
//  GouJi
//
//  Created by mtt on 14-9-29.
//
//
#import <UIKit/UIKit.h>

#include "PlatformFunction.h"
#include "cocos2d.h"
#include "AppController.h"
#import <Launcher/DLLauncher.h>

USING_NS_CC;
using namespace std;

static std::string m_sEvntStr = "";
static std::string s_sEventAuthFinish = "";
static string m_sSchemePath = "";
static string m_sSchemeData = "";
// Launcher壳相关 START========================

void PlatformFunction::removeLaunchShadeImage()
{
    [DLLauncher removeShadeImageView];
}
/**
* 重置玩家隐私协议授权标记
* nFlag，0：取消隐私协议授权 1：同意隐私协议
*/
void PlatformFunction::setPrivacyFlag(const int &nFlag)
{
    [DLLauncher setPrivacyCheckBox:nFlag];
}
/**
* 服务条款接⼝
*/
void PlatformFunction::showPrivacyAgreement()
{
    [DLLauncher showPrivacyAgreement];
}
/**
* 隐私政策接⼝
*/
void PlatformFunction::showPrivacyGuide()
{
    [DLLauncher showPrivacyGuide];
}
/**
* 未成年隐私政策接⼝
*/
void PlatformFunction::showPrivacyGuideChildren()
{
    [DLLauncher showPrivacyGuideChildren];
}
// Launcher壳相关 END========================

void PlatformFunction::exitGame()
{
    cocos2d::Director::getInstance()->end();
}
