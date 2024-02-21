/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "AppController.h"
#import "platform/ios/CCEAGLView-ios.h"
#import "cocos2d.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#import "Udesk.h"
#import <Launcher/DLLauncher.h>
#import <Launcher/FirstInViewController.h>
#import <ShareSDK/ShareSDK.h>
#import <MOBFoundation/MobSDK+Privacy.h>

@interface AppController()<DLPrivacyCallback>
@end

@implementation AppController

#pragma mark -
#pragma mark Application lifecycle

#define IOS_7_OR_LATER ([[[UIDevice currentDevice] systemVersion] floatValue] >= 7.0)
#define IOS_8_OR_LATER ([[[UIDevice currentDevice] systemVersion] floatValue] >= 8.0)

// cocos2d application instance
static UIViewController *s_pCurrentViewController;
//系统window名称，写在@implementation AppController下⾯
@synthesize window = _window;

@synthesize viewController = _viewController;
NSDictionary * pLaunchOptions = nullptr;

+ (UIViewController *)currentViewController {
    return s_pCurrentViewController;
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    pLaunchOptions = launchOptions;
    
    // Add the view controller's view to the window and display.
    _window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    
    // 多乐Launcher初始化 START===============
    //更改启动根视图控制器
    UIViewController *firstVC = [[FirstInViewController alloc] init];
    [_window setRootViewController:firstVC];
    [_window makeKeyAndVisible];
    [DLLauncher playVideo:self];
    // 多乐Launcher初始化 end===============
    
    return YES;
}

#pragma mark Launcher
- (void)onPrivacySuccess {
    //将AppDelegate实例化，避免同意隐私政策前有其他操作
    static AppDelegate s_sharedApplication;
    
//    int nDir = util::PHZReaderWriter::LoadIntegerFromXML("KEY_SCENE_DIRECTION", -1);
    // 暂时屏蔽竖版
    int nDir = 0;
    
    if (nDir == 0)
    {
        g_bPortrait = false;
    }
    else if (nDir == 1)
    {
        g_bPortrait = true;
    }
    [RootViewController setIsPortrait:g_bPortrait];
    
    cocos2d::Application *app = cocos2d::Application::getInstance();
    app->initGLContextAttrs();
    cocos2d::GLViewImpl::convertAttrs();
    
    // Use RootViewController manage CCEAGLView
    _viewController = [[RootViewController alloc] init];

    // Set RootViewController to window
    if ([[UIDevice currentDevice].systemVersion floatValue] < 6.0) {
        // warning: addSubView doesn't work on iOS6
        [_window addSubview: _viewController.view];
    } else {
        // use this method on ios6
        [_window setRootViewController:_viewController];
    }

    [_window makeKeyAndVisible];
    
    // Launcher：显示遮罩图片
    [DLLauncher showShadeImageView];
    
    // 防止屏幕睡眠
    [UIApplication sharedApplication].idleTimerDisabled = YES;
    
    s_pCurrentViewController = _viewController;

    // IMPORTANT: Setting the GLView should be done after creating the RootViewController
//    cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView(eaglView);
    cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView((__bridge void *)_viewController.view);

    cocos2d::Director::getInstance()->setOpenGLView(glview);
    
    // ShareSDK分享
    [MobSDK uploadPrivacyPermissionStatus:YES onResult:^(BOOL success) {}];

    app->run();
}

- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
     //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->pause(); */
    if (![[UIApplication sharedApplication].keyWindow.rootViewController isKindOfClass:[FirstInViewController class]])
    {
        cocos2d::Application::getInstance()->applicationDidEnterBackground();
        
        // Udesk客服离线
        [UdeskManager setupCustomerOffline];
    }
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
     //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->resume(); */
    if (![[UIApplication sharedApplication].keyWindow.rootViewController isKindOfClass:[FirstInViewController class]])
    {
        cocos2d::Application::getInstance()->applicationWillEnterForeground();
        
        // Udesk客服在线
        [UdeskManager setupCustomerOnline];
    }
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    if (![[UIApplication sharedApplication].keyWindow.rootViewController isKindOfClass:[FirstInViewController class]]) {
    }
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    if (![[UIApplication sharedApplication].keyWindow.rootViewController isKindOfClass:[FirstInViewController class]]) {
    }
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
    if (![[UIApplication sharedApplication].keyWindow.rootViewController isKindOfClass:[FirstInViewController class]])
    {
    }
}

- (BOOL)application:(UIApplication *)application continueUserActivity:(NSUserActivity *)userActivity restorationHandler:(void(^)(NSArray<id<UIUserActivityRestoring>> * __nullable restorableObjects))restorationHandler {
    if ([WXApi handleOpenUniversalLink:userActivity delegate:self]) {
        return YES;
    }
    return NO;
}

- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {

}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo {

}

// 支持所有iOS系统
- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
    // 接受传过来的参数
    NSString *pPath = [[url path] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    NSString *pQuery = [[url query] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    
    if ([WXApi handleOpenURL:url delegate:self]) {
        return YES;
    }
    
    return NO;
}

- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url {
    BOOL result = [WXApi handleOpenURL:url delegate:self];
    return result;
}

+(void) screenShotAction:(NSString *)imagePath{
    UIImage *saveImage = [UIImage imageWithContentsOfFile:imagePath];
    UIImageWriteToSavedPhotosAlbum(saveImage, self, @selector(savingImageIsFinished:didFinishSavingWithError:contextInfo:), nil);
}

+(void) savingImageIsFinished:(UIImage *)image didFinishSavingWithError:(NSError *)error contextInfo:(void *)contextInfo
{
    
}

#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


- (void)dealloc {
    [_window release];
    [super dealloc];
}

// 接收本地推送（AppDelegate.m中添加）
- (void)application:(UIApplication *)application didReceiveLocalNotification:(UILocalNotification *)notification{
    // 本地推送不做弹出通知
    //	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"标题" message:notification.alertBody delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil];
    //	[alert show];
    
    // 图标上的数字减1
    //	application.applicationIconBadgeNumber -= 1;
}

static bool g_bPortrait = false;

- (UIInterfaceOrientationMask)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(nullable UIWindow *)window
{
    if (!g_bPortrait) {
        //横屏
        [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeRight];
    }else{
        //竖屏
        [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortrait];
    }
    return UIInterfaceOrientationMaskAllButUpsideDown;
}

+(void)setOrientation:(bool)bIsPortrait{
    
    if(g_bPortrait == bIsPortrait){
        return ;
    }
    g_bPortrait = bIsPortrait;
    [RootViewController setIsPortrait:g_bPortrait];
    UIInterfaceOrientation interfaceOrientation = UIInterfaceOrientationUnknown;
    if(bIsPortrait){
        interfaceOrientation = UIInterfaceOrientationPortrait;
        CGAffineTransform at = CGAffineTransformMakeRotation(M_PI * 2);
        [[UIApplication sharedApplication].keyWindow setTransform:at];
    }
    else{
        interfaceOrientation = UIInterfaceOrientationLandscapeRight;
        CGAffineTransform at = CGAffineTransformMakeRotation(M_PI * 2);
        [[UIApplication sharedApplication].keyWindow setTransform:at];
    }
    [[UIApplication sharedApplication] setStatusBarOrientation:interfaceOrientation];
    if ([[UIDevice currentDevice] respondsToSelector:@selector(setOrientation:)]) {
        SEL selector = NSSelectorFromString(@"setOrientation:");
        NSInvocation *invocation = [NSInvocation invocationWithMethodSignature:[UIDevice
                                                                                instanceMethodSignatureForSelector:selector]];
        [invocation setSelector:selector];
        [invocation setTarget:[UIDevice currentDevice]];
        int val = interfaceOrientation;
        [invocation setArgument:&val atIndex:2];
        [invocation invoke];
    }
}

- (void)onReq:(BaseReq *)req {
}

- (void)onResp:(BaseResp *)resp {

}

#pragma mark - 压缩图片
+ (UIImage *)compressImage:(UIImage *)image toByte:(NSUInteger)maxLength {
    // Compress by quality
    CGFloat compression = 1;
    NSData *data = UIImageJPEGRepresentation(image, compression);
    if (data.length < maxLength) return image;
    
    CGFloat max = 1;
    CGFloat min = 0;
    for (int i = 0; i < 6; ++i) {
        compression = (max + min) / 2;
        data = UIImageJPEGRepresentation(image, compression);
        if (data.length < maxLength * 0.9) {
            min = compression;
        } else if (data.length > maxLength) {
            max = compression;
        } else {
            break;
        }
    }
    UIImage *resultImage = [UIImage imageWithData:data];
    if (data.length < maxLength) return resultImage;
    
    // Compress by size
    NSUInteger lastDataLength = 0;
    while (data.length > maxLength && data.length != lastDataLength) {
        lastDataLength = data.length;
        CGFloat ratio = (CGFloat)maxLength / data.length;
        CGSize size = CGSizeMake((NSUInteger)(resultImage.size.width * sqrtf(ratio)),
                                 (NSUInteger)(resultImage.size.height * sqrtf(ratio))); // Use NSUInteger to prevent white blank
        UIGraphicsBeginImageContext(size);
        [resultImage drawInRect:CGRectMake(0, 0, size.width, size.height)];
        resultImage = UIGraphicsGetImageFromCurrentImageContext();
        UIGraphicsEndImageContext();
        data = UIImageJPEGRepresentation(resultImage, compression);
    }
    
    return resultImage;
}

@end
