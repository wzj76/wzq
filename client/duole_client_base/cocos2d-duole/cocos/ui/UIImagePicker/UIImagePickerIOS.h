//
//  UIImagePickerIOS.h
//  cocos2d_libs
//
//  Created by Joi Yao on 17/2/8.
//
//

#include "platform/CCPlatformConfig.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "UIImagePickerImpl.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@protocol ImagePickerDelegate;
@interface ImagePickerView : UIViewController<UIActionSheetDelegate, UIImagePickerControllerDelegate,
UINavigationControllerDelegate, UIPopoverPresentationControllerDelegate>
{
    void* imagePicker_;
    BOOL actionSheetExists_;
    BOOL cleared_;
    BOOL allowsEditing_;
    int ratioX_;
    int ratioY_;
    int width_;
    int height_;
}

@property(nonatomic, assign) void*                      imagePicker;
@property(nonatomic, assign) BOOL                       cleared;
@property(nonatomic, assign) BOOL                       allowsEditing;
@property(nonatomic, assign) int                        ratioX;
@property(nonatomic, assign) int                        ratioY;
@property(nonatomic, assign) int                        width;
@property(nonatomic, assign) int                        height;


@end
NS_CC_BEGIN

namespace ui {
    
    class ImagePicker;
    
    class ImagePickerImplIOS : public ImagePickerImpl
    {
    public:
        ImagePickerImplIOS(ImagePicker* pImagePicker);
        virtual ~ImagePickerImplIOS();
        virtual void show(const int nRatioX,
                          const int nRatioY,
                          const int nWidth,
                          const int nHeight,
                          const bool bAllowsEditing);
        void clearView();
        
    private:
        ImagePickerView* m_pImagePickerView;
    };
}

NS_CC_END
#endif
