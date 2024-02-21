//
//  DLPrivacyCallback.h
//  Launcher
//
//  Created by 孙兴国 on 2021/7/4.
//  Copyright © 2021 duole. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol DLPrivacyCallback <NSObject>

@optional
-(void)onPrivacySuccess;//同意成功回调

@end

NS_ASSUME_NONNULL_END
