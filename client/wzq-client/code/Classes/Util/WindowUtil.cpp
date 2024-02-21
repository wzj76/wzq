#include "WindowUtil.h"
#include "ReaderWriter.h"
#include "Core/AppModel.h"

USING_NS_CC;

namespace util
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
//    Size PHZWindowUtil::sizeDesign(720, 1280); // 暂时屏蔽竖版
    Size WindowUtil::sizeDesign(1280, 720);
#else
    Size PHZWindowUtil::sizeDesign(1280, 720);
#endif
    Size WindowUtil::sizeDevice(0, 0);
    Size WindowUtil::sizeAdapt(0, 0);
    float WindowUtil::adaptX    = 0;
    float WindowUtil::scaleX    = 1.0;
    float WindowUtil::scaleY    = 1.0;
    float WindowUtil::scaleFit  = 1.0;
    float WindowUtil::zeroValue = 0;

    void WindowUtil::initialize()
    {
        sizeDesign = Size(1280, 720);
        Director::getInstance()->getOpenGLView()->setDesignResolutionSize(sizeDesign.width, sizeDesign.height, ResolutionPolicy::FIXED_HEIGHT);

        sizeDevice = Director::getInstance()->getWinSizeInPixels();
        sizeAdapt = sizeDevice;
        scaleX = sizeDevice.width / sizeDesign.width;
        scaleY = sizeDevice.height / sizeDesign.height;
        // 适配缩放值
        scaleFit = MIN(scaleX, scaleY);
        // 设备尺寸长边
        float fDeviceLongSide = sizeDevice.width > sizeDevice.height ? sizeDevice.width : sizeDevice.height;
        // 设备尺寸短边
        float fDeviceShortSide = sizeDevice.width < sizeDevice.height ? sizeDevice.width : sizeDevice.height;
        // 正常屏最大尺寸
        float fMaxRete = 2.00;
        // 异形屏长边空出来的距离（设备绝对值）
        zeroValue = fDeviceLongSide / fDeviceShortSide < fMaxRete ? 0 : ((fDeviceLongSide - fDeviceShortSide * fMaxRete) / 2);
        // 适配
        if (sizeDevice.width / sizeDevice.height > 2)
        {
            adaptX = (sizeDevice.width / 2 - sizeDevice.height);
            sizeAdapt.width = sizeDevice.width - 2 * adaptX;
        }
    }
    
    void WindowUtil::resetInit(bool bPortrait)
    {
        if (bPortrait) {
            sizeDesign = Size(720, 1280);
        }
        else {
            sizeDesign = Size(1280, 720);
        }
        initialize();
    }
}
