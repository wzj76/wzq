//
//  PHZStringUtil.cpp
//  PaoHuZi
//
//  Created by jzn on 2021/08/23.
//

#include "StringUtil.h"
#include "Util/Utils.h"
#include "AppFunc.h"

USING_NS_CC;
using namespace std;

namespace util
{
    string getCutStringByLength(string sText, int nFontSize, float fMaxLength, string sExtraStr)
    {
        string sResult = "";
        Label* pTempLabel = GetZyLabel(nFontSize, "");
        vector<string> vTxts = CharsetUtil::getUTF8CharVector(sText);
        for (int i = 0; i < (int)vTxts.size(); i ++)
        {
            pTempLabel->setString(sResult + vTxts.at(i));
            if (pTempLabel->getContentSize().width > fMaxLength)
            {
                break;
            }
            sResult += vTxts.at(i);
        }
        if (!sResult.empty() && (int)sResult.size() < (int)sText.size())
        {
            sResult += sExtraStr;
        }
        return sResult;
    }

    //获取加密手机号
    string getFormatTel(const string &sTelNumber)
    {
        // 非纯数字，不处理
        if (!IsNumbers(sTelNumber))
            return sTelNumber;
        
        string sNewTel = "";
        string sChar = "";
        if (sTelNumber.length() <= 7) {
            sNewTel = sTelNumber;
        } else {
            for (int i = 3; i < sTelNumber.length() - 4; i++) {
                sChar += "*";
            }
            sNewTel = sTelNumber.substr(0, 3) + sChar + sTelNumber.substr(sTelNumber.length() - 4);
        }
        return sNewTel;
    }
}
