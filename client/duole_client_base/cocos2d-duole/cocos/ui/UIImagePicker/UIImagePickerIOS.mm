//
//  UIImagePickerIOS.m
//  cocos2d_libs
//
//  Created by Joi Yao on 17/2/8.
//
//


#include "UIImagePickerIOS.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "UIImagePicker.h"
#include "platform/CCFileUtils.h"
#import "platform/ios/CCEAGLView-ios.h"
#import <AVFoundation/AVFoundation.h>
#import <Photos/PHPhotoLibrary.h>

#define getImagePickerImplIOS() ((cocos2d::ui::ImagePickerImplIOS*)imagePicker_)

@implementation ImagePickerView

@synthesize imagePicker = imagePicker_;
@synthesize ratioX = ratioX_;
@synthesize ratioY = ratioY_;
@synthesize width = width_;
@synthesize height = height_;

- (void)setImagePicker:(void *)value
{
    imagePicker_ = value;
}

- (void)setRatioX:(int)value
{
    ratioX_ = value;
}

- (void)setRatioY:(int)value
{
    ratioY_ = value;
}
- (void)setWidth:(int)value
{
    width_ = value;
}

-(void)setHeight:(int)value
{
    height_ = value;
}

-(void)setActionSheetExists:(BOOL)value
{
    actionSheetExists_ = value;
}

-(void)setCleared:(BOOL)value
{
    cleared_ = value;
}

-(void)setAllowsEditing:(BOOL)value
{
    allowsEditing_ = value;
}

-(UIInterfaceOrientationMask)supportedInterfaceOrientations
{
     return UIInterfaceOrientationMaskAll;
}

-(void)viewDidLoad
{
    [super viewDidLoad];
    [self.view setBackgroundColor: [UIColor grayColor]];
}

-(void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    if (!cleared_)
    {
        [self createSelections];
    }
}

-(void)createSelections
{
    cleared_ = YES;
    UIAlertControllerStyle preferredStyle = UIAlertControllerStyleActionSheet;
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
    {
        preferredStyle = UIAlertControllerStyleAlert;
    }
    UIAlertController* alert = [UIAlertController alertControllerWithTitle:nil
                                                                   message:nil
                                                            preferredStyle:preferredStyle];
    [alert addAction:[UIAlertAction actionWithTitle:@"拍照"
                                              style:UIAlertActionStyleDefault
                                            handler:^(UIAlertAction * _Nonnull action)
    {
        [self handleAction: alert action:1];
    }]];
    [alert addAction:[UIAlertAction actionWithTitle:@"选取现有图片"
                                              style:UIAlertActionStyleDefault
                                            handler:^(UIAlertAction * _Nonnull action)
    {
        [self handleAction: alert action:2];
    }]];
    [alert addAction:[UIAlertAction actionWithTitle:@"暂时不"
                                              style:UIAlertActionStyleCancel
                                            handler:^(UIAlertAction * _Nonnull action)
    {
        [self handleAction: alert action:0];
    }]];
    [self presentViewController:alert animated:YES completion:nil];
    [alert release];
}

-(void)handleAction:(UIAlertController*)alert action:(int) action
{
    switch (action)
    {
        case 1:
        {
            AVAuthorizationStatus authStatus = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo];
            if (authStatus != AVAuthorizationStatusAuthorized && authStatus != AVAuthorizationStatusNotDetermined)
            {
                [self alertAuthorizationFailed:@"无法使用相机" message:@"请在\"设置-隐私-相机\"中允许访问相机。"];
            }
            else
            {
                if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera])
                {
                    [self showImagePicker:UIImagePickerControllerSourceTypeCamera];
                }
                else
                {
                    [self alertAuthorizationFailed:@"无法使用相机" message:@"该设备无法使用相机"];
                }
            }
        }
            break;
        case 2:
        {
            PHAuthorizationStatus authStatus = [PHPhotoLibrary authorizationStatus];
            if (authStatus == PHAuthorizationStatusRestricted || authStatus == PHAuthorizationStatusDenied)
            {
                [self alertAuthorizationFailed:@"无法使用相册" message:@"请在\"设置-隐私-相册\"中允许访问相机。"];
            }
            else
            {
                [self showImagePicker:UIImagePickerControllerSourceTypePhotoLibrary];
            }
        }
            break;
        default:
            [self picked:nil :NO];
            break;
    }
}

-(void)alertAuthorizationFailed:(nullable NSString *)title message:(nullable NSString *)message
{
    UIAlertController* alert = [UIAlertController alertControllerWithTitle:title
                                                                   message:message
                                                            preferredStyle:UIAlertControllerStyleAlert];
    [alert addAction:[UIAlertAction actionWithTitle:@"确定"
                                              style:UIAlertActionStyleDefault
                                            handler:^(UIAlertAction * _Nonnull action)
    {
        [alert dismissViewControllerAnimated:YES completion:nil];
        [alert release];
        [self picked:nil :NO];
    }]];
    [self presentViewController:alert animated:YES completion:nil];
}

-(void)showImagePicker:(UIImagePickerControllerSourceType) sourceType
{
    UIImagePickerController *imgPickerController = [[UIImagePickerController alloc] init];
    imgPickerController.modalPresentationStyle = UIModalPresentationFullScreen;
    imgPickerController.view.frame = self.view.frame;
    imgPickerController.delegate = self;
    imgPickerController.allowsEditing = allowsEditing_ ? YES : NO;
    imgPickerController.sourceType = sourceType;
    if (sourceType == UIImagePickerControllerSourceTypeCamera)
    {
        imgPickerController.showsCameraControls = YES;
    }
    
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
    {
        UIPopoverPresentationController* popover = imgPickerController.popoverPresentationController;
        if (popover)
        {
            popover.sourceView = self.view;
            popover.permittedArrowDirections = UIPopoverArrowDirectionDown;
            CGRect bounds = self.view.bounds;
            popover.sourceRect = CGRectMake(CGRectGetMidX(bounds), CGRectGetMaxY(bounds), 0, 0);
        }
    }
    [self presentViewController:imgPickerController animated:YES completion:nil];
}

-(void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    [picker dismissViewControllerAnimated:YES completion:nil];
    UIImage* image = nullptr;
    if (allowsEditing_)
    {
        image = [info objectForKey:UIImagePickerControllerEditedImage];
    }
    else
    {
        image = [info objectForKey:UIImagePickerControllerOriginalImage];
    }
    [self saveImage:image];
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    [picker dismissViewControllerAnimated:YES completion:nil];
    [self picked:nil :NO];
}

- (void)saveImage:(UIImage *)image
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *filepath = [paths objectAtIndex:0];
    BOOL succeed = NO;
    if (filepath.length > 0)
    {
        filepath = [filepath stringByAppendingString:@"/tmp.jpg"];
        CGFloat compressionQuality = 0.25;
        if (allowsEditing_)
        {
            compressionQuality = 0.75;
        }
        CGSize size = CGSizeMake(width_, height_);
        if (image.size.width < image.size.height)
        {
            size.width = height_;
            size.height = width_;
        }
        image  = [self scaleImage:image scaledToSize:size];
        NSData *imageData = UIImageJPEGRepresentation(image, compressionQuality);
        [[NSFileManager defaultManager]removeItemAtPath:filepath error:nil];
        succeed = [imageData writeToFile:filepath atomically:YES];
    }
    [self picked:filepath :succeed];
}

//压缩图片
- (UIImage*)scaleImage:(UIImage*)image scaledToSize:(CGSize)newSize
{
    UIGraphicsBeginImageContext(newSize);
    [image drawInRect:CGRectMake(0, 0, newSize.width, newSize.height)];
    UIImage* newImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return newImage;
}

-(void) picked:(NSString*)filepath :(BOOL)succeed
{
    std::string sPath = filepath? filepath.UTF8String: "";
    if (getImagePickerImplIOS())
    {
        getImagePickerImplIOS()->getDelegate()->picked(sPath, succeed);
        [self clearView];
    }
}

-(void) clearView
{
    [self dismissViewControllerAnimated:YES completion:nil];
    cleared_ = YES;
}
@end

NS_CC_BEGIN
namespace ui {
    
    ImagePickerImpl* __createSystemImagePicker(ImagePicker* pImagePicker)
    {
        return new ImagePickerImplIOS(pImagePicker);
    }
    
    ImagePickerImplIOS::ImagePickerImplIOS(ImagePicker* pImagePicker)
    : cocos2d::ui::ImagePickerImpl(pImagePicker)
    {
        m_pImagePickerView = nullptr;
    }
    
    ImagePickerImplIOS::~ImagePickerImplIOS()
    {
        if (m_pImagePickerView)
        {
            [m_pImagePickerView setImagePicker:nullptr];
        }
        clearView();
        m_pImagePickerView = nullptr;
        m_pDelegate = nullptr;
        m_pImagePicker = nullptr;
    }
    
    void ImagePickerImplIOS::show(const int nRatioX,
                                  const int nRatioY,
                                  const int nWidth,
                                  const int nHeight,
                                  const bool bAllowsEditing)
    {
        auto glview = cocos2d::Director::getInstance()->getOpenGLView();
        Size size = glview->getFrameSize();
        CGRect rect = CGRectMake(0, 0, size.width, size.height);
        
        CCEAGLView *eaglview = static_cast<CCEAGLView *>(glview->getEAGLView());
        float factor = eaglview.contentScaleFactor;
        rect.size.width /= factor;
        rect.size.height /= factor;
        
        clearView();
        
        m_pImagePickerView = [[ImagePickerView alloc] init];
        m_pImagePickerView.modalPresentationStyle = UIModalPresentationFullScreen;
        [m_pImagePickerView setImagePicker:this];
        [m_pImagePickerView setRatioX:nRatioX];
        [m_pImagePickerView setRatioY:nRatioY];
        [m_pImagePickerView setWidth:nWidth];
        [m_pImagePickerView setHeight:nHeight];
        [m_pImagePickerView setActionSheetExists:NO];
        [m_pImagePickerView setCleared:NO];
        [m_pImagePickerView setAllowsEditing:bAllowsEditing];
        [m_pImagePickerView retain];
        UIViewController *rootViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
        [rootViewController presentViewController:m_pImagePickerView animated:YES completion:nil];
    }
    
    void ImagePickerImplIOS::clearView()
    {
        if (m_pImagePickerView)
        {
            [m_pImagePickerView setImagePicker:nullptr];
            [m_pImagePickerView clearView];
            [m_pImagePickerView release];
            m_pImagePickerView = nullptr;
        }
    }
}
NS_CC_END
#endif
