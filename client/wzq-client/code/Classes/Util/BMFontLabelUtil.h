//
//  PHZBMFontLabelUtil.h
//  PaoHuZi
//
//  Created by zj on 17/4/12.
//
//
/*******************************************************************************
 此文件中的方法用于取代标准的生成Label::createWithBMFont的方法。
 *******************************************************************************/

#pragma once

#include "cocos2d.h"

namespace util
{    
    // 获取Label
    cocos2d::Label* GetBMLabel(const std::string &fontFile,const std::string& fontStr,const float & fScale = 1.0f);
}
