//
//  PHZCharsetUtil.h
//  GouJi
//
//  Created by ZengQi on 15-3-19.
//
//

#ifndef __GouJi__CharsetUtil__
#define __GouJi__CharsetUtil__

#include <string>
#include <vector>

class CharsetUtil
{
public:
    // 判断是否合法的UTF8字符串
    static bool isValidUTF8String(const std::string &input);
    // 获取UTF8字符个数
    static size_t getUTF8CharCount(const std::string &input);
    // 获取UTF8单字符数组
    static std::vector<std::string> getUTF8CharVector(const std::string &input);
    // 获取截断的UTF8字符串
    static std::string getBlockedUTF8Text(const std::string &input, size_t limit);
    // 获取单个字符小于3字节的截断的UTF8字符串
    static std::string getBlockedUniversalUTF8Text(const std::string &input, size_t limit);
    
};

#endif /* defined(__GouJi__CharsetUtil__) */
