#import <UIKit/UIKit.h>
#import "WXApi.h"

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate, NSURLConnectionDelegate, WXApiDelegate>

@property(nonatomic, readonly) RootViewController* viewController;

+ (UIViewController *) currentViewController;

+(void) screenShotAction:(NSString *)imagePath;
+(void) savingImageIsFinished:(UIImage *)image didFinishSavingWithError:(NSError *)error contextInfo:(void *)contextInfo;

+(void)setOrientation:(bool)bIsPortrait;

+ (UIImage *)compressImage:(UIImage *)image toByte:(NSUInteger)maxLength; // 压缩图片
@end

