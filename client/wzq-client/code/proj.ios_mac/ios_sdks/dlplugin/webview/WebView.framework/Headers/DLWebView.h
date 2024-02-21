//
//  DLWebView.h
//  Webview
//
//  Created by duole on 2021/8/11.
//

#import <Foundation/Foundation.h>
#import "DLWebViewCallback.h"

NS_ASSUME_NONNULL_BEGIN

@interface DLWebView : NSObject
/**
 * 显示全屏webview，根据传入的方向展示网页
 * loadUrl：加载网页地址
 * isLandscape：是否横屏
 * callback：回调监听
 */
+ (void)openWebView:(NSString *)loadUrl isLandscape:(BOOL)isLandscape callback:(id<DLWebViewCallback>)callback;
/**
 * 关闭webview
 */
+ (void)closeWebView;
/**
 * 隐藏关闭按钮
 * 默认显示原生关闭按钮，调用此方法隐藏
 */
+ (void)hideCloseButton;

//==============================回传接口==============================//
/**
 * canOpenApp回传接口
 * isSuccess：返回值，YES：成功   NO：失败
 */
+ (void)canOpenAppComplete:(BOOL)isSuccess;
/**
 * getCopyText回传接口
 * isSuccess：返回值，YES：成功   NO：失败
 * text：剪切板信息
 */
+ (void)getCopyTextComplete:(BOOL)isSuccess text:(NSString *)text;
/**
 * getSharingPlatform回传接口
 * isSuccess：返回值，YES：成功   NO：失败
 * platformArray：所有可以分享的平台数组
 */
+ (void)getSharingPlatformComplete:(BOOL)isSuccess platformArray:(NSArray *)platformArray;
/**
 * shareImage回传接口
 * isSuccess：返回值，YES：成功   NO：失败
 * callId：透传ID
 */
+ (void)shareImageComplete:(BOOL)isSuccess callId:(NSString *)callId;
/**
 * shareLink回传接口
 * isSuccess：返回值，YES：成功   NO：失败
 * callId：透传ID
 */
+ (void)shareLinkComplete:(BOOL)isSuccess callId:(NSString *)callId;

/**
 * getMusicSwitch回传接口
 * isOpen：返回值，YES：开启   NO：关闭
 * callId：透传ID
 */
+ (void)getMusicSwitchComplete:(BOOL)isOpen;

/**
 * getSoundEffectSwitch回传接口
 * isOpen：返回值，YES：开启   NO：关闭
 * callId：透传ID
 */
+ (void)getSoundEffectSwitchComplete:(BOOL)isOpen;

/**
 * 其他扩展接口回传接口，不适用目前已封装的回调接口
 * value：回传的json字符串
 * callId：透传ID
 */
+ (void)doActionComplete:(NSString *)jsonString callId:(NSString *)callId;

@end

NS_ASSUME_NONNULL_END
