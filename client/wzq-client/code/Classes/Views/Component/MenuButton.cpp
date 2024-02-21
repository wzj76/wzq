//
//  MenuButton.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/24.
//

#include "MenuButton.hpp"
#include "WZQDeviceSize.hpp"
#include "AppRes.h"
#include "AppModel.h"
#include "SimpleAudioEngine.h"


MenuButton::MenuButton(): m_textSelect(nullptr), m_textNormal(nullptr),m_imagePath(""), m_Normal(nullptr), m_Selected(nullptr),m_isSelected(false),m_isEnabled(true)
{
}

MenuButton::~MenuButton() noexcept
{
    
}

bool MenuButton::init() {
    if(!MenuItemSprite::init())
        return false;
    
    return true;
}



void MenuButton::addText(const std::string text, const std::string fontName, const int fontSize)
{
    m_textNormal->setString(text);
    m_textSelect->setString(text);
    m_textNormal->setSystemFontName(fontName);
    m_textSelect->setSystemFontName(fontName);
    m_textNormal->setSystemFontSize(fontSize);
    m_textSelect->setSystemFontSize(fontSize);
}

void MenuButton::addImage(const std::string imagePath)
{
    m_Normal->setTexture(imagePath);
    m_Selected->setTexture(imagePath);
    m_textNormal->setPosition(WZQDeviceSize::getContentCenterPos(m_Normal));
    m_textSelect->setPosition(WZQDeviceSize::getContentCenterPos(m_Selected));
}


MenuButton *MenuButton::create(const std::string text,const std::string imagePath ,const ccMenuCallback &callback)
{
    
    MenuButton *ret = new MenuButton();
    
    ret->m_callback = callback;
    ret->m_Normal = Sprite::create(imagePath);
    ret->m_Normal->setAnchorPoint(Vec2(0.5f,0.5f));
    //ret->m_Normal->setScale(WZQDeviceSize::getMenuButtonScale(ret->m_Normal));
//    ret->m_textNormal = Label::createWithSystemFont(text, "黑体", 25);
    ret->m_textNormal = Label::createWithTTF(text, FONT_GUHUANG, 30);
    ret->m_textNormal->setPosition(WZQDeviceSize::getContentCenterPos(ret->m_Normal));
    ret->m_Normal->addChild(ret->m_textNormal);
    
    ret->m_Selected = Sprite::create(imagePath);
    ret->m_Selected->setAnchorPoint(Vec2(0.5f,0.5f));
    ret->m_Selected->setColor(Color3B::GRAY);
    ret->m_Selected->setScale(0.9);
    ret->m_textSelect = Label::createWithTTF(text, FONT_GUHUANG, 30);
    ret->m_textSelect->setPosition(WZQDeviceSize::getContentCenterPos(ret->m_Selected));
    ret->m_Selected->addChild(ret->m_textSelect);
    ret->initWithNormalSprite(ret->m_Normal, ret->m_Selected, nullptr, callback);
    ret->autorelease();
    ret->setScale(WZQDeviceSize::getContentScaleInHeight(ret, 0.1f));
    return ret;
}

MenuButton *MenuButton::create() {
    return MenuButton::create("",nullptr);
}

MenuButton *MenuButton::create(const std::string text, const cocos2d::ccMenuCallback &callback)
{
    return MenuButton::create(text,AppModel::getInstance()->m_pUserModel->getThemePath() + BT_BASEONE,callback);
}

void MenuButton::setText(const std::string text) { 
    m_textNormal->setString(text);
    m_textSelect->setString(text);
}

void MenuButton::onEnter()
{
    MenuItemSprite::onEnter();
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(MenuButton::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(MenuButton::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
}

void MenuButton::onExit()
{
    _eventDispatcher->removeEventListenersForTarget(this);
    MenuItemSprite::onExit();
}

bool MenuButton::onTouchBegan(Touch* touch, Event* event)
{
    if (this->getBoundingBox().containsPoint(touch->getLocation())&&this->getParent()->isVisible()&&this->isVisible()&&m_isEnabled)
    {
        
        if(AppModel::getInstance()->m_pUserModel->m_setupConf.button_effect)
        {
            // 播放按下时的动画
            auto scaleToSmall = ScaleTo::create(0.05f, m_scale * 0.8f);
            this->runAction(scaleToSmall);
            
            // 按钮变灰
            auto tintToGray = TintTo::create(0.05f, 128, 128, 128);
            this->runAction(tintToGray);
        }
        else
        {
            this->setScale(m_scale * 0.8f, false);
            this->setColor(Color3B::GRAY);
        }
        return true;
    }

    return false; // 触摸点不在按钮范围内，不处理触摸事件
}

void MenuButton::onTouchEnded(Touch* touch, Event* event)
{
    if(AppModel::getInstance()->m_pUserModel->m_setupConf.button_effect)
    {
        // 播放松开时的动画
        auto scaleToBig = ScaleTo::create(0.1f, m_scale);
        this->runAction(scaleToBig);
        auto tintToOriginal = TintTo::create(0.1f, m_color.r, m_color.g, m_color.b);
        this->runAction(tintToOriginal);
    }
    else
    {
        this->setScale(m_scale, false);
        this->setColor(m_color);
    }
    
    // 调用回调函数
    if (this->getBoundingBox().containsPoint(touch->getLocation())&&this->getParent()->isVisible()&&this->isVisible()&&m_isEnabled)
    {
        if (m_callback)
        {
            if(AppModel::getInstance()->m_pUserModel->m_setupConf.sound_effect)
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_BT);
            m_callback(this);
        }
        
    }
}

void MenuButton::setSelected() {
    m_isSelected = true;
    this->setScale(m_scale);
    this->setColor(Color3B::GRAY);

}

void MenuButton::setRealesed() {
    m_isSelected = false;
    this->setScale(m_scale);
    this->setColor(m_color);
}

void MenuButton::setScale(float scale, bool isOverrideScale)
{
    MenuItemSprite::setScale(scale);
    
    if (isOverrideScale)
    {
        m_scale = this->getScaleX();
        m_color = this->getColor();
    }
}

void MenuButton::setEnable(bool flag) {
    m_isEnabled = flag;
}






