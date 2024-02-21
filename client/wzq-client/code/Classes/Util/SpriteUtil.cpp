#include "SpriteUtil.h"
#include "MacroUtil.h"

USING_NS_CC;

namespace util
{
    
    Sprite* GetSpriteFromFile(const char* file_name)
    {
        return CreateSprite(file_name);
    }
    
    Sprite* GetSpriteFromPlist(const char* plist_name)
    {
        SpriteFrame *frame = GetFrameFromPlist(plist_name);
        if (!frame)
        {
            return nullptr;
        }
        return Sprite::createWithSpriteFrame(frame);
    }
    
    SpriteFrame *GetFrameFromFile(const char* file_name)
    {
        Sprite *sprite = GetSpriteFromFile(file_name);
        if (!sprite)
        {
            return nullptr;
        }
        return sprite->getSpriteFrame();
    }
    
    SpriteFrame *GetFrameFromPlist(const char* plist_name)
    {
        return SpriteFrameCache::getInstance()->getSpriteFrameByName(plist_name);
    }
    
    Label* GetLabel(float size, const char* label_str)
    {
        auto label = Label::createWithSystemFont(label_str, "黑体", size);
        label->setHorizontalAlignment(TextHAlignment::CENTER);
        label->setColor(Color3B::WHITE);
        return label;
    }
    
    // 获取准圆Label
    Label* GetZyLabel(float size, const char* label_str)
    {
        auto label = Label::createWithTTF(label_str, "fonts/font_zhunyun.ttf", size);
        label->setHorizontalAlignment(TextHAlignment::CENTER);
        return label;
    }
    
    Sprite* GetLabelSpriteFromFile(const char* label_str, float size, const char* file_name, float offset)
    {
        Sprite* sprite = GetSpriteFromFile(file_name);
        return GetLabelSpriteFromSprite(label_str, size, sprite, offset);;
    }
    
    Sprite* GetLabelSpriteFromPlist(const char* label_str, float size, const char* plist_name, float offset)
    {
        Sprite* sprite = GetSpriteFromPlist(plist_name);
        return GetLabelSpriteFromSprite(label_str, size, sprite, offset);
    }
    
    Sprite *GetLabelSpriteFromSprite(const char* label_str, float size, Sprite* sprite, float offset)
    {
        Label* label = GetLabel(size, label_str);
        label->setPosition(Vec2(sprite->getContentSize().width / 2,
                               sprite->getContentSize().height / 2 + offset));
        sprite->addChild(label);
        return sprite;
    }
    
    Rect GetNodeWorldRect(Node* pNode, const Size &size)
    {
        Vec2 screenPos = pNode->convertToWorldSpace(Vec2::ZERO);
        
        float scaleX = 1;
        float scaleY = 1;
        
        for (Node *p = pNode; p != nullptr; p = p->getParent())
        {
            scaleX *= p->getScaleX();
            scaleY *= p->getScaleY();
        }
        
        // Support negative scaling. Not doing so causes intersectsRect calls
        // (eg: to check if the touch was within the bounds) to return false.
        // Note, CCNode::getScale will assert if X and Y scales are different.
        if (scaleX < 0.f)
        {
            screenPos.x += size.width * scaleX;
            scaleX = -scaleX;
        }
        if (scaleY<0.f)
        {
            screenPos.y += size.height * scaleY;
            scaleY = -scaleY;
        }
        
        return Rect(screenPos.x, screenPos.y, size.width * scaleX, size.height * scaleY);
    }
    
    ui::Scale9Sprite* GetScale9Sprite(const std::string& sRes,
                                     const int& nPixels,
                                     const cocos2d::Size& oSizeFinal)
    {
        // 简化后代码
        ui::Scale9Sprite* pRet = CreateScale9Sprite(sRes);
        pRet->setContentSize(oSizeFinal);
        return pRet;
    }
    
    ui::Scale9Sprite* GetScale9Sprite(const std::string& sRes,
                                      const cocos2d::Size oSize)
    {
        ui::Scale9Sprite* pRet = CreateScale9Sprite(sRes);
        Size oSizeBg = pRet->getContentSize();
        Rect oCapInsets = Rect(oSizeBg.width / 2 - 1, oSizeBg.height / 2 - 1, 3, 3);
        pRet->setCapInsets(oCapInsets);
        pRet->setContentSize(oSize);
        return pRet;
    }
    
    /*
     * @brief        画圆角矩形
     * @param        width			宽
     * @param        height			搞
     * @param        radius			圆角半径
     * @param        color			颜色
     * @attention
     */
    DrawNode* GetRoundRect(float width, float height, float radius, cocos2d::Color4F color)
    {
        int segments = 20;
        
        //算出1/4圆
        
        const float coef    = 0.5f * (float)M_PI / segments;
        Point * vertices    = new Point[segments + 1];
        Point * thisVertices = vertices;
        for(unsigned int i = 0; i <= segments; ++i, ++thisVertices)
        {
            float rads        = (segments - i)*coef;
            thisVertices->x    = (int)(radius * sinf(rads));
            thisVertices->y    = (int)(radius * cosf(rads));
        }
        //
        Point tagCenter;
        float minX    = MIN(0, width);
        float maxX    = MAX(0, width);
        float minY    = MIN(0, height);
        float maxY    = MAX(0, height);
        
        unsigned int dwPolygonPtMax = (segments + 1) * 4;
        Point * pPolygonPtArr = new Point[dwPolygonPtMax];
        Point * thisPolygonPt = pPolygonPtArr;
        int aa = 0;
        //左上角
        tagCenter.x        = minX + radius;
        tagCenter.y        = maxY - radius;
        thisVertices    = vertices;
        for(unsigned int i = 0; i <= segments; ++i, ++thisPolygonPt, ++thisVertices)
        {
            thisPolygonPt->x    = tagCenter.x - thisVertices->x;
            thisPolygonPt->y    = tagCenter.y + thisVertices->y;
            ++aa;
        }
        //右上角
        tagCenter.x        = maxX - radius;
        tagCenter.y        = maxY - radius;
        thisVertices    = vertices + segments;
        for(unsigned int i = 0; i <= segments; ++i, ++thisPolygonPt, --thisVertices)
        {
            thisPolygonPt->x    = tagCenter.x + thisVertices->x;
            thisPolygonPt->y    = tagCenter.y + thisVertices->y;
            ++aa;
        }
        //右下角
        tagCenter.x        = maxX - radius;
        tagCenter.y        = minY + radius;
        thisVertices    = vertices;
        for(unsigned int i = 0; i <= segments; ++i, ++thisPolygonPt, ++thisVertices)
        {
            thisPolygonPt->x    = tagCenter.x + thisVertices->x;
            thisPolygonPt->y    = tagCenter.y - thisVertices->y;
            ++aa;
        }
        //左下角
        tagCenter.x        = minX + radius;
        tagCenter.y        = minY + radius;
        thisVertices    = vertices + segments;
        for(unsigned int i = 0; i <= segments; ++i, ++thisPolygonPt, --thisVertices)
        {
            thisPolygonPt->x    = tagCenter.x - thisVertices->x;
            thisPolygonPt->y    = tagCenter.y - thisVertices->y;
            ++aa;
        }
        
        DrawNode *pStencil=DrawNode::create();
        pStencil->drawPolygon(pPolygonPtArr, dwPolygonPtMax, color, 0, color);//绘制这个多边形
        
        CC_SAFE_DELETE_ARRAY(vertices);
        CC_SAFE_DELETE_ARRAY(pPolygonPtArr);
        
        return pStencil;
    }

    bool CheckResExist(const std::string& sFileName)
    {
        return FileUtils::getInstance()->isFileExist(sFileName);
    }

    std::string GetSpriteFrameName(const std::string& sFileName)
    {
        // 找到第一个/的索引
        unsigned long n = sFileName.find_first_of("/");
        if (n == std::string::npos || n == sFileName.length() - 1)
        {
            CCLOG("not has '/' >>> %s", sFileName.c_str());
            return "";
        }
            
        // 截取剩下的字符
        return sFileName.substr(n + 1, sFileName.length() - n - 1);;
    }

    cocos2d::Sprite* CreateSprite(const std::string& sFileName)
    {
        // 文件名为空 >>> 创建无图片精灵
        if (sFileName.empty())
        {
            return Sprite::create();
        }
        // 文件路径存在 >>> 直接使用图片创建
        if (CheckResExist(sFileName))
        {
            return Sprite::create(sFileName);
        }
        // 文件路径不存在 >>> 调整路径后使用图集创建
        std::string sPpriteFrameName = GetSpriteFrameName(sFileName);
        return Sprite::createWithSpriteFrameName(sPpriteFrameName);
    }


    cocos2d::ui::Button* CreateButton()
    {
        return ui::Button::create();
    }

    cocos2d::ui::Button* CreateButton(const std::string& sFileName1, const std::string& sFileName2, const std::string& sFileName3)
    {
        // 文件路径存在 >>> 直接使用图片创建
        if (CheckResExist(sFileName1))
        {
            return ui::Button::create(sFileName1, sFileName2, sFileName3);
        }
        // 文件路径不存在 >>> 调整路径后使用图集创建
        std::string sSpriteFrameName1 = GetSpriteFrameName(sFileName1);
        std::string sSpriteFrameName2 = GetSpriteFrameName(sFileName2);
        std::string sSpriteFrameName3 = GetSpriteFrameName(sFileName3);
        return ui::Button::create(sSpriteFrameName1, sSpriteFrameName2, sSpriteFrameName3, ui::Button::TextureResType::PLIST);
    }

    cocos2d::ui::Scale9Sprite* CreateScale9Sprite(const std::string& sFileName)
    {
        // 文件名为空 >>> 创建无图片精灵
        if (sFileName.empty())
        {
            return ui::Scale9Sprite::create();
        }
        // 文件路径存在 >>> 直接使用图片创建
        if (CheckResExist(sFileName))
        {
            return ui::Scale9Sprite::create(sFileName);
        }
        // 文件路径不存在 >>> 调整路径后使用图集创建
        std::string sSpriteFrameName = GetSpriteFrameName(sFileName);
        return ui::Scale9Sprite::createWithSpriteFrameName(sSpriteFrameName);
    }

    void ChangeSpriteFrame(Sprite* pSprite, const std::string& sFileName, const std::string& sDefaultRes)
    {
        // 文件名为空 >>> 创建无图片精灵
        if (pSprite == nullptr || sFileName.empty())
        {
            return;
        }
        // 文件路径存在 >>> 直接使用图片创建
        if (CheckResExist(sFileName))
        {
            pSprite->setTexture(sFileName);
            return;
        }
        // 文件路径不存在 >>> 调整路径后使用图集创建
        std::string sSpriteFrameName = GetSpriteFrameName(sFileName);
        SpriteFrame* pSpriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(sSpriteFrameName);
        if (pSpriteFrame)
        {
            pSprite->setSpriteFrame(pSpriteFrame);
        }
        else if (!sDefaultRes.empty())
        {
            ChangeSpriteFrame(pSprite, sDefaultRes);
        }
    }

    bool InitSpriteWithFile(Sprite* pSprite, const std::string& sFileName)
    {
        // 文件名为空 >>> 创建无图片精灵
        if (pSprite == nullptr || sFileName.empty())
        {
            return false;
        }
        // 文件路径存在 >>> 直接使用图片创建
        if (CheckResExist(sFileName))
        {
            return pSprite->initWithFile(sFileName);
        }
        // 文件路径不存在 >>> 调整路径后使用图集创建
        std::string sSpriteFrameName = GetSpriteFrameName(sFileName);
        return pSprite->initWithSpriteFrameName(sSpriteFrameName);
    }

    void LoadTexturesWithFile(cocos2d::ui::Button* pButton, const std::string& sFileNormal, const std::string& sFileSelected, const std::string& sFileDisabled)
    {
        // 文件名为空/button为空 >>> 跳过
        if (pButton == nullptr || sFileNormal.empty() || sFileSelected.empty())
        {
            return;
        }
        // 文件路径存在 >>> 直接使用图片替换
        if (CheckResExist(sFileNormal) && CheckResExist(sFileSelected))
        {
            pButton->loadTextures(sFileNormal, sFileSelected, sFileDisabled);
            return;
        }
        // 文件路径不存在 >>> 调整路径后使用图集替换
        std::string sNormalFrameName = GetSpriteFrameName(sFileNormal);
        std::string sSelectedFrameName = GetSpriteFrameName(sFileSelected);
        std::string sFileDisabledFrameName = GetSpriteFrameName(sFileDisabled);
        pButton->loadTextures(sNormalFrameName, sSelectedFrameName, sFileDisabledFrameName, ui::Button::TextureResType::PLIST);
    }
}
