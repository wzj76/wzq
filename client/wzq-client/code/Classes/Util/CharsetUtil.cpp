//
//  PHZCharsetUtil.cpp
//  GouJi
//
//  Created by ZengQi on 15-3-19.
//
//

#include "CharsetUtil.h"

using namespace std;

bool CharsetUtil::isValidUTF8String(const string &input)
{
    bool valid = true;
    for (size_t i = 0, len = 0; i < input.length(); i += len)
    {
        unsigned char byte = input[i];
        if (byte >= 0xFC)      len = 6;
        else if (byte >= 0xF8) len = 5;
        else if (byte >= 0xF0) len = 4;
        else if (byte >= 0xE0) len = 3;
        else if (byte >= 0xC0) len = 2;
        else                   len = 1;
        if (len > 3)
        {
            valid = false;
            break;
        }
        for (size_t j = i + 1; j < i + len; j++)
        {
            byte = input[j];
            if ((byte & 0x80) != 0x80)
            {
                valid = false;
                break;
            }
        }
        if (!valid)
        {
            break;
        }
    }
    return valid;
}

size_t CharsetUtil::getUTF8CharCount(const string &input)
{
    size_t count = 0;
    for (size_t i = 0, len = 0; i < input.length(); i += len)
    {
        unsigned char byte = input[i];
        if (byte >= 0xFC)      len = 6;
        else if (byte >= 0xF8) len = 5;
        else if (byte >= 0xF0) len = 4;
        else if (byte >= 0xE0) len = 3;
        else if (byte >= 0xC0) len = 2;
        else                   len = 1;
        count++;
    }
    return count;
}

vector<string> CharsetUtil::getUTF8CharVector(const string &input)
{
    vector<string> vec;
    for (size_t i = 0, len = 0; i < input.length(); i += len)
    {
        unsigned char byte = input[i];
        if (byte >= 0xFC)      len = 6;
        else if (byte >= 0xF8) len = 5;
        else if (byte >= 0xF0) len = 4;
        else if (byte >= 0xE0) len = 3;
        else if (byte >= 0xC0) len = 2;
        else                   len = 1;
        vec.push_back(input.substr(i, len));
    }
    return vec;
}

string CharsetUtil::getBlockedUTF8Text(const string &input, size_t limit)
{
    string output;
    size_t count = getUTF8CharCount(input);
    if (count <= limit)
    {
        output = input;
    }
    else
    {
        vector<string> vec = CharsetUtil::getUTF8CharVector(input);
        for (size_t i = 0; i < limit; i++)
        {
            output.append(vec[i]);
        }
    }
    return output;
}

string CharsetUtil::getBlockedUniversalUTF8Text(const string &input, size_t limit)
{
    string output;
    vector<string> vec = CharsetUtil::getUTF8CharVector(input);
    for (size_t i = 0, count = 0; i < vec.size() && count <= limit; i++)
    {
        if (vec[i].length() > 3)
        {
            continue;
        }
        output.append(vec[i]);
        count++;
    }
    return output;
}
