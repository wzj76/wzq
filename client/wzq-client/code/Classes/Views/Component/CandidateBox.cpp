//
//  CandidateBox.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/6.
//

#include "CandidateBox.hpp"

bool CandidateBox::init(const std::string unselected, const std::string selected)
{
    if (!Sprite::init())
    {
        return false;
    }
    
    unselectedImage_ = std::string(unselected);
    selectedImage_ = std::string(selected);
    setTexture(unselected);
    selected_ = false;

    // 注册触摸事件监听器
    auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(false);
    touchListener->onTouchBegan = CC_CALLBACK_2(CandidateBox::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

CandidateBox* CandidateBox::create(const std::string unselected, const std::string selected)
{
    CandidateBox* ret = new (std::nothrow) CandidateBox();
    if (ret && ret->init(unselected, selected))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        delete ret;
        return nullptr;
    }
}
