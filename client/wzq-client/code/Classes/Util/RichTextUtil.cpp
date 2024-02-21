//
//  RichTextUtil.cpp
//  BaoHuang
//
//  Created by 姚佳 on 2019/10/11.
//

#include "RichTextUtil.h"

USING_NS_CC;
using namespace std;

Size RichTextUtil::setDimention(cocos2d::ui::RichText *pText, float fMaxWidth, float fVSpace)
{
    pText->formatText();
    Size oSize = pText->getVirtualRendererSize();
    if (oSize.width >= fMaxWidth)
    {
        int nLine = ceil(oSize.width / fMaxWidth);
        oSize.width = fMaxWidth;
        oSize.height = oSize.height * nLine + fVSpace * (nLine + 1);
        pText->setVerticalSpace(fVSpace);
        pText->setContentSize(oSize);
        pText->ignoreContentAdaptWithSize(false);
    }
    return oSize;
}
