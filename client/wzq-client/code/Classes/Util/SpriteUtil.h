/*******************************************************************************
 此文件中的方法用于取代标准的生成CCSprite和CCSpriteFrame的方法。
 1. is_use_plist返回默认情况下（即get_sprite和GetFrameFromFile不带有boool参数的那个函数）是否
 使用plist来生成CCCSprite和CCCSpriteFrame
 2. get_sprite用于生成CCSprite
 3. GetFrameFromFile用于生成CCCSpriteFrame
 *******************************************************************************/

#pragma once

#include "cocos2d.h"
#include "ui/UIScale9Sprite.h"
#include "ui/UIButton.h"
namespace util
{
    
    // 获取Sprite，根据全局变量is_use_plist()决定是否从plist中获取
    cocos2d::Sprite* GetSpriteFromFile(const char* file_name);
    
    // 获取Sprite，根据参数use_plist决定是否从plist中获取
    cocos2d::Sprite* GetSpriteFromPlist(const char* plist_name);
    
    // 获取SpriteFrame，根据全局变量is_use_plist()决定是否从plist中获取
    cocos2d::SpriteFrame* GetFrameFromFile(const char* file_name);
    
    // 获取SpriteFrame，根据参数use_plist决定是否从plist中获取
    cocos2d::SpriteFrame* GetFrameFromPlist(const char* plist_name);
    
    // 获取Label
    cocos2d::Label* GetLabel(float size, const char* label_str);
    
    // 获取准圆Label
    cocos2d::Label* GetZyLabel(float size, const char* label_str);
    
    // 个人需求，获取带有Label是Sprite，从文件中获取
    cocos2d::Sprite* GetLabelSpriteFromFile(const char* label_str,
                                              float size,
                                              const char* file_name,
                                              float offset = 0);
    // 个人需求，获取带有Label是sprite，从plist中获取
    cocos2d::Sprite* GetLabelSpriteFromPlist(const char* label_str,
                                               float size,
                                               const char* plist_name,
                                               float offset = 0);
    // 用于重载
    cocos2d::Sprite* GetLabelSpriteFromSprite(const char* label_str,
                                                float size,
                                                cocos2d::Sprite* sprite,
                                                float offset = 0);
    
    // 获取node的世界矩形
    // 此方法从scrollview的getViewRect copy
    cocos2d::Rect GetNodeWorldRect(cocos2d::Node* pNode, const cocos2d::Size &size);
    /** 获取9切片
     * @param  sRes       资源文件完整路径
     * @param  nPixels    可复制的区域像素
     * @param  oSizeTotal 最终大小
     * @return Scale9Sprite*
     */
    cocos2d::ui::Scale9Sprite* GetScale9Sprite(const std::string& sRes,
                                               const int& nPixels,
                                               const cocos2d::Size& oSizeFinal);
    
    cocos2d::ui::Scale9Sprite* GetScale9Sprite(const std::string& sRes,
                                               const cocos2d::Size oSize);
    /*
     * @brief        画圆角矩形
     * @param        width			宽
     * @param        height			搞
     * @param        radius			圆角半径
     * @param        color			颜色
     * @attention
     */
    cocos2d::DrawNode* GetRoundRect(float width, float height, float radius, cocos2d::Color4F color = {1,1,1,1});

    /*
     * 判断资源文件是否存在
     * @sFileName   文件名
     */
    bool CheckResExist(const std::string& sFileName);

    /*
     * 去掉一层地址，例如：abc/def/g/hijk/lmn.png ---> def/g/hijk/lmn.png
     * 需要注意plist文件内资源的目录应当比碎图资源少一层目录
     * @sFileName   文件名
     */
    std::string GetSpriteFrameName(const std::string& sFileName);

    /*
     * 创建Sprite
     * 根据文件名自动选择创建方式
     * @sFileName   文件名(兼容char*，std::string， 或缺省方式传入)
     */
    cocos2d::Sprite* CreateSprite(const std::string& sFileName = "");

    /*
     * 创建Button
     * 根据文件名自动选择创建方式
     * 缺省参数
     */
    cocos2d::ui::Button* CreateButton();

    /*
     * 创建Button
     * 根据文件名自动选择创建方式
     * @sFileName1   文件名(兼容char*，std::string)
     * @sFileName2   文件名(兼容char*，std::string， 或缺省方式传入)
     * @sFileName3   文件名(兼容char*，std::string， 或缺省方式传入)
     */
    cocos2d::ui::Button* CreateButton(const std::string& sFileName1, const std::string& sFileName2 = "", const std::string& sFileName3 = "");

    /*
     * 创建Scale9Sprite
     * 根据文件名自动选择创建方式
     * @sFileName   文件名(兼容char*，std::string， 或缺省方式传入)
     */
    cocos2d::ui::Scale9Sprite* CreateScale9Sprite(const std::string& sFileName = "");

    /*
     * 修改纹理
     * 根据文件名自动选择创建方式
     * @pSprite     待替换精灵
     * @sFileName   文件名(兼容char*，std::string)
     * @sDefaultRes 默认资源(找不到传入的资源时使用传入的本地默认资源)
     */
    void ChangeSpriteFrame(cocos2d::Sprite* pSprite, const std::string& sFileName, const std::string& sDefaultRes = "");

    /*
     * 通过文件初始化精灵
     * 根据文件名自动选择初始化方式
     * @pSprite     待初始化精灵
     * @sFileName   文件名(兼容char*，std::string)
     */
    bool InitSpriteWithFile(cocos2d::Sprite* pSprite, const std::string& sFileName);

    /*
     * 通过文件替换UIButton的纹理
     * 根据文件名自动选择初始化方式
     * @pButton     待替换Button
     * @sFileNormal   文件名(兼容char*，std::string)
     * @sFileSelected   文件名(兼容char*，std::string)
     * @sFileDisabled   文件名(兼容char*，std::string，缺省)
     */
    void LoadTexturesWithFile(cocos2d::ui::Button* pButton, const std::string& sFileNormal, const std::string& sFileSelected, const std::string& sFileDisabled = "");
}
