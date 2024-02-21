/****************************************************************************
 Copyright (c) 2013-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "network/HttpCookie.h"
#include "platform/CCFileUtils.h"
#include <sstream>
#include <stdio.h>

void HttpCookie::readFile()
{
    std::string inString = cocos2d::FileUtils::getInstance()->getStringFromFile(_cookieFileName);
    if(!inString.empty())
    {
        std::vector<std::string> cookiesVec;
        cookiesVec.clear();

        std::stringstream stream(inString);
        std::string item;
        while(std::getline(stream, item, '\n'))
        {
            cookiesVec.push_back(item);
        }

        if(cookiesVec.empty())
            return;

        _cookies.clear();

        for(auto& cookie : cookiesVec)
        {
            if(cookie.length() == 0)
                continue;

            if(cookie.find("#HttpOnly_") != std::string::npos)
            {
                cookie = cookie.substr(10);
            }

            if(cookie.at(0) == '#')
                continue;

            CookiesInfo co;
            std::stringstream streamInfo(cookie);
            std::vector<std::string> elems;
            std::string elemsItem;

            while (std::getline(streamInfo, elemsItem, '\t'))
            {
                elems.push_back(elemsItem);
            }

            co.domain = elems[0];
            if (co.domain.at(0) == '.')
            {
                co.domain = co.domain.substr(1);
            }
            co.tailmatch = strcmp("TRUE", elems[1].c_str())?true: false;
            co.path   = elems[2];
            co.secure = strcmp("TRUE", elems[3].c_str())?true: false;
            co.expires = elems[4];
            co.name = elems[5];
            co.value = elems[6];
            _cookies.push_back(co);
        }
    }
}

const std::vector<CookiesInfo>* HttpCookie::getCookies() const
{
    return &_cookies;
}

const CookiesInfo* HttpCookie::getMatchCookie(const std::string& url) const
{
    for(auto& cookie : _cookies)
    {
        if(url.find(cookie.domain) != std::string::npos)
            return &cookie;
    }

    return nullptr;
}

void HttpCookie::updateOrAddCookie(CookiesInfo* cookie, bool isMulti)
{
    for(auto& _cookie : _cookies)
    {
        if (isMulti) {
            if(cookie->domain == _cookie.domain
            && cookie->name == _cookie.name)
            {
                _cookie = *cookie;
                return;
            }
        } else {
            if(cookie->domain == _cookie.domain)
            {
                _cookie = *cookie;
                return;
            }
        }
    }
    _cookies.push_back(*cookie);
}

void HttpCookie::updateOrAddCookie(const std::string& cookieInString) {
    if (cookieInString.empty()) {
        return;
    } else {
        std::vector<std::string> cookiesVec;
        cookiesVec.clear();

        std::stringstream stream(cookieInString);
        std::string item;
        while(std::getline(stream, item, '\n'))
        {
            cookiesVec.push_back(item);
        }

        if(cookiesVec.empty())
            return;

        for(auto& __cookie : cookiesVec)
        {
            if(__cookie.length() == 0)
                continue;

            if(__cookie.find("#HttpOnly_") != std::string::npos)
            {
                __cookie = __cookie.substr(10);
            }

            if(__cookie.at(0) == '#')
                continue;

            CookiesInfo co;
            std::stringstream streamInfo(__cookie);
            std::vector<std::string> elems;
            std::string elemsItem;

            while (std::getline(streamInfo, elemsItem, '\t'))
            {
                elems.push_back(elemsItem);
            }

            co.domain = elems[0];
            if (co.domain.at(0) == '.')
            {
                co.domain = co.domain.substr(1);
            }
            co.tailmatch = strcmp("TRUE", elems[1].c_str())?true: false;
            co.path   = elems[2];
            co.secure = strcmp("TRUE", elems[3].c_str())?true: false;
            co.expires = elems[4];
            co.name = elems[5];
            co.value = elems[6];
            // 更新
            updateOrAddCookie(&co, true);
        }
    }
}

void HttpCookie::writeFile()
{
    FILE *out;
    out = fopen(_cookieFileName.c_str(), "w");
    fputs("# Netscape HTTP Cookie File\n"
          "# http://curl.haxx.se/docs/http-cookies.html\n"
          "# This file was generated by cocos2d-x! Edit at your own risk.\n"
          "# Test cocos2d-x cookie write.\n\n",
          out);

    std::string line;
    for(auto& cookie : _cookies)
    {
        line.clear();
        line.append(cookie.domain);
        line.append(1, '\t');
        cookie.tailmatch ? line.append("TRUE") : line.append("FALSE");
        line.append(1, '\t');
        line.append(cookie.path);
        line.append(1, '\t');
        cookie.secure ? line.append("TRUE") : line.append("FALSE");
        line.append(1, '\t');
        line.append(cookie.expires);
        line.append(1, '\t');
        line.append(cookie.name);
        line.append(1, '\t');
        line.append(cookie.value);
        //line.append(1, '\n');

        fprintf(out, "%s\n", line.c_str());
    }

    fclose(out);
}

void HttpCookie::setCookieFileName(const std::string& filename)
{
    _cookieFileName = filename;
}

