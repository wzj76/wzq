/*窗体尺寸与缩放*/
#pragma once

#include "cocos2d.h"

namespace util
{
    class WindowUtil
    {
    public:
        static void initialize();
        static void resetInit(bool bPortrait);
    public:
        static cocos2d::Size sizeDesign;  // 设计尺寸
        static cocos2d::Size sizeDevice;  // 设备尺寸
        static cocos2d::Size sizeAdapt;   // 适配尺寸
        static float adaptX;              // 适配起始横坐标
        static float scaleX;              // 相对于设计尺寸的X轴缩放
        static float scaleY;              // 相对于设计尺寸的Y轴缩放
        static float scaleFit;            // 设备适应尺寸
        static float zeroValue;           // 异形屏边界预留位置
    };
    // 统一宏定义
#define DESIGN_S    util::WindowUtil::sizeDesign
#define DESIGN_W    util::WindowUtil::sizeDesign.width
#define DESIGN_H    util::WindowUtil::sizeDesign.height
#define DEVICE_S    util::WindowUtil::sizeDevice
#define DEVICE_W    util::WindowUtil::sizeDevice.width
#define DEVICE_H    util::WindowUtil::sizeDevice.height
#define ADAPT_S     util::WindowUtil::sizeAdapt
#define ADAPT_W     util::WindowUtil::sizeAdapt.width
#define ADAPT_H     util::WindowUtil::sizeAdapt.height
#define ADAPT_X     util::WindowUtil::adaptX
#define SCALE_X     util::WindowUtil::scaleX
#define SCALE_Y     util::WindowUtil::scaleY
#define SCALE_FIT   util::WindowUtil::scaleFit
#define ZERO_VALUE  util::WindowUtil::zeroValue
}
