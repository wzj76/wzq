//
//  DLWebViewPureCallback.h
//  WebView
//
//  Created by duole on 2021/8/12.
//  Copyright © 2021 duole. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol DLWebViewCallback <NSObject>

@required
/**
 * 检测能否打开App
 * packageName：APP包名
 */
-(void)canOpenApp:(NSString *)packageName;
/**
 * 关闭webview页面
 */
-(void)closeWebView;
/**
 * 拷贝文本内容
 */
-(void)getCopyText;
/**
 * 获取所有可以分享的平台
 */
-(void)getSharingPlatform;
/**
 * 获取音乐开关
 */
-(void)getMusicSwitch;
/**
 * 获取音效开关
 */
-(void)getSoundEffectSwitch;
/**
 * 打开App
 * packageName：APP包名
 * downloadUrl：下载链接
 */
-(void)openApp:(NSString *)packageName downloadUrl:(NSString *)downloadUrl;
/**
 * 图片分享
 * platform：    分享平台        0：微信   1：朋友圈   2：QQ   3：QQ空间   4：新浪微博
 * imageType：图片类型        0：数据   1：URL
 * image：        图片数据，根据imageType决定是否存在
 */
-(void)shareImage:(int)platform imageType:(int)imageType image:(NSString *)image callId:(NSString *)callId;
/**
 * 图文分享
 * platform：    分享平台        0：微信   1：朋友圈   2：QQ   3：QQ空间   4：新浪微博
 * link：            图文URL
 * title：            图文标题
 * content：     图文内容
 * thumb：      缩略图
 */
-(void)shareLink:(int)platform link:(NSString *)link title:(NSString *)title content:(NSString *)content thumb:(NSString *)thumb callId:(NSString *)callId;
/**
 * 拦截回调：duole开头的url
 */
-(void)onInterceptURL:(NSString *)url;

@optional
/**
 * JS通知回调
 */
-(void)onDoAction:(NSString *)key value:(NSString *)value callId:(NSString *)callId;

@end
