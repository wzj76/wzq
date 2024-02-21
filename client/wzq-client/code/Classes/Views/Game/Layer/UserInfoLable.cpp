//
//  UserInfoLable.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/29.
//

#include "UserInfoLable.hpp"
#include <stdio.h>
#include "Core/AppFunc.h"
#include "Core/AppEvent.h"
#include "Core/AppModel.h"
#include "Core/AppRes.h"
#include "WZQDeviceSize.hpp"
#include "Controllers/WZQLoginController.h"
#include <iostream>
#include <string>
#include <sstream>
USING_NS_CC;

bool UserInfoLable::init()
{
    if(!Sprite::init())
    {
        return false;
    }
    setTexture(USERLABLE);
    // 用户头像
    std::stringstream touxiang;
    int head_num = m_imageid;
    touxiang<<"touxiang/head"<<head_num<<".png";
//    auto pUserImage = Sprite::create(touxiang.str());
    m_pOpposentImage = Sprite::create(touxiang.str());
    m_pOpposentImage -> setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    m_pOpposentImage -> setScale(WZQDeviceSize::getCertainPixelScale(m_pOpposentImage, this -> getContentSize().height * 3 / 5));
    m_pOpposentImage -> setPosition(0, this->getContentSize().height);
    WZQDeviceSize::setMoveNode(Vec2(getContentSize().width / 10, -getContentSize().height / 5), m_pOpposentImage);
    addChild(m_pOpposentImage);
    
    m_pOpposentNameLable = Label::createWithTTF(m_sUsername, FONT_GUHUANG, WZQDeviceSize::getBigFontSize());
    m_pOpposentNameLable -> setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    m_pOpposentNameLable->setScale(WZQDeviceSize::getCertainPixelScale(m_pOpposentNameLable, this->getContentSize().width * 0.5));
    WZQDeviceSize::setPosAtNodeRightTop(m_pOpposentNameLable, m_pOpposentImage);
    addChild(m_pOpposentNameLable);
    // 用户名下面显示用户分数
    m_pOpposentIDLable = Label::createWithSystemFont("分数: "+ std::to_string(m_scores), "黑体", WZQDeviceSize::getBigFontSize());
    m_pOpposentIDLable -> setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    m_pOpposentIDLable->setScale(WZQDeviceSize::getCertainPixelScale(m_pOpposentIDLable, this->getContentSize().width * 0.3));
    WZQDeviceSize::setPosAtNodeLeftBottom(m_pOpposentIDLable, m_pOpposentNameLable);
    addChild(m_pOpposentIDLable);
    
    
    return true;
}

void UserInfoLable::onEnter()
{
    Sprite::onEnter();
}

void UserInfoLable::onExit()
{
    Sprite::onExit();
}


UserInfoLable::UserInfoLable():m_imageid(1),m_sUsername("默认用户名"),m_scores(0)
{
}

UserInfoLable::~UserInfoLable() noexcept
{
}

UserInfoLable *UserInfoLable::create() {
    UserInfoLable* ret = new (std::nothrow)UserInfoLable;
    if (ret && ret -> init())
    {
        // 背景

        ret -> setScale(0.5f);
        ret -> autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


void UserInfoLable::setInfo(int imageId, const std::string Username, int scores)
{
    m_imageid = imageId;
    m_sUsername = Username;
    m_scores = scores;
    
    std::stringstream touxiang;
    int head_num = imageId;
    std::cout<<"头像编号为："<<head_num<<std::endl;
    touxiang<<"touxiang/head"<<head_num<<".png";
    m_pOpposentImage -> setTexture(touxiang.str());
    m_pOpposentImage -> setScale(WZQDeviceSize::getCertainPixelScale(m_pOpposentImage, this -> getContentSize().height * 3 / 5));
    m_pOpposentNameLable -> setString(Username);
    m_pOpposentIDLable -> setString("分数: "+ std::to_string(m_scores));
}

void UserInfoLable::delScoreLabel()
{
    m_pOpposentIDLable -> removeFromParent();
}












