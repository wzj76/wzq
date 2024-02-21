//
//  DLLauncher.h
//  Launcher
//  SDK对外所有功能入口
//  Created by duole on 2021/7/3.
//  Copyright © 2021 duole. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "DLPrivacyCallback.h"

NS_ASSUME_NONNULL_BEGIN

@interface DLLauncher : NSObject

/**
 * 展示《多乐游戏用户服务协议》
 */
+ (void)showPrivacyAgreement;

/**
 * 展示《多乐游戏隐私保护指引》
 */
+ (void)showPrivacyGuide;

/**
 * 展示《多乐游戏未成年人保护指引》
 */
+ (void)showPrivacyGuideChildren;

/**
 * 设置复选框状态（是否同意隐私协议）；
 * nFlag，0：取消隐私协议授权 1：同意隐私协议
 */
+ (void)setPrivacyCheckBox:(int)nFlag;

/**
 * 显示遮罩层imageview
 */
+ (void)showShadeImageView;

/**
 * 移除遮罩层imageview
 */
+ (void)removeShadeImageView;

/**
 * 开启debug模式
 */
+ (void)setDebugMode;

#pragma mark -------------iOS 独有接口--------------

/**
 * 播放视频接口
 */
+ (void)playVideo:(id<DLPrivacyCallback>)callBackDelegate;

@end

NS_ASSUME_NONNULL_END
