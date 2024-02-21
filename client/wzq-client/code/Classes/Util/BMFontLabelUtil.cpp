//
//  PHZBMFontLabelUtil.cpp
//  PaoHuZi
//
//  Created by zj on 17/4/12.
//
//

#include "BMFontLabelUtil.h"
#include "MacroUtil.h"

USING_NS_CC;

namespace util
{
    Label* GetBMLabel(const std::string &fontFile,const std::string& fontStr,const float & fScale)
    {
        auto label = Label::createWithBMFont(fontFile, fontStr);
        label->setScale(fScale);
        return label;
    }
}
