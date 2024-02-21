//
//  MenuButton.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/24.
//

#ifndef MenuButton_hpp
#define MenuButton_hpp

#include <stdio.h>
#include <string>
#include "cocos2d.h"
USING_NS_CC;

class MenuButton : public MenuItemSprite
{
private:
    Label* m_textNormal;
    Label* m_textSelect;
    std::string m_imagePath;
    Ref* m_callFunc;
    ccMenuCallback m_callback;
    
public:
    Sprite* m_Normal;
    Sprite* m_Selected;
    float m_scale;
    Color3B m_color;
    bool m_isSelected;
    bool m_isEnabled;
    
    MenuButton();
    ~MenuButton();
    
    static MenuButton* create(const std::string text, const ccMenuCallback &callback);
    
    static MenuButton* create();
    static MenuButton* create(const std::string text,const std::string Imagepath ,const ccMenuCallback &callback);
    
    virtual bool init();
    void addText(const std::string text, const std::string fontName, const int fontSize);
    void addImage(const std::string imagePath);
    void setText(const std::string text);
    void addCallFunc();
    //void createButton(Ref* pSender, Ref* object);
    
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    virtual void onEnter();
    virtual void onExit();
    virtual void setScale(float scale, bool isOverrideScale = true);
    virtual void setEnable(bool flag);
    
    
    void setSelected();
    void setRealesed();
};


#endif /* MenuButton_hpp */
