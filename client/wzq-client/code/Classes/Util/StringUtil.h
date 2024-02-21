//
//  PHZStringUtil.h
//  PaoHuZi
//
//  Created by jzn on 2021/08/23.
//

#ifndef PHZStringUtil_h
#define PHZStringUtil_h

#include <string>

namespace util
{
    std::string getCutStringByLength(std::string sText, int nFontSize, float fMaxLength, std::string sExtraStr = "...");
    
    //获取加密手机号
    std::string getFormatTel(const std::string &sTelNumber);
};

#endif /* PHZStringUtil_h */
