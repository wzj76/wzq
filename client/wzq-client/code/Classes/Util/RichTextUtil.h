//
//  RichTextUtil.h
//  BaoHuang
//
//  Created by 姚佳 on 2019/10/11.
//

#ifndef RichTextUtil_h
#define RichTextUtil_h

#include "ui/UIRichText.h"

class RichTextUtil
{
public:
    static cocos2d::Size setDimention(cocos2d::ui::RichText* pText, float fMaxWidth, float fVSpace = 0.0);
};

#endif /* RichTextUtil_h */
