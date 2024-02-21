//
//  ScrollLayer.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/5.
//

#include "ScrollLayer.hpp"
#include "Core/AppFunc.h"
#include "Core/AppEvent.h"
#include "Core/AppModel.h"
#include "WZQDeviceSize.hpp"
#include "Controllers/WZQLoginController.h"
#include "LobbyScene.hpp"
#include "EmojiLayer.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include "paohuziProtocol.pb.h"
bool ScrollLayer::init()
{
    if(!Layer::init())
    {
        return false;
    }
    std::cout << "创建ScrollLayer， 大小为"<<this->getContentSize().width<<this->getContentSize().height<<std::endl;
    m_pBg = Sprite::create(AppModel::getInstance()->m_pUserModel->getThemePath() + BG_POP);
    auto size = m_pBg->getContentSize();
    addChild(m_pBg,-1);
    auto vsize = Director::getInstance()->getVisibleSize();
    m_pBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    m_pBg->setPosition(vsize.width, vsize.height/2);
    m_pBg->setScale(WZQDeviceSize::getContentScaleInWidth(m_pBg, 0.2f), WZQDeviceSize::getContentScaleInHeight(m_pBg, 0.8f));
    
    // 创建一个滚动视图
    m_scrollView = ui::ScrollView::create();
    m_scrollView->setContentSize(Size(m_pBg->getContentSize().width *
                                    m_pBg->getScaleX() * 0.9,
                                    m_pBg->getContentSize().height *
                                    m_pBg->getScaleY() * 0.7));
    m_scrollView->setDirection(ui::ScrollView::Direction::VERTICAL); // 垂直滚动
    WZQDeviceSize::setPosAtNodeLeftBottom(m_scrollView, m_pBg);
    WZQDeviceSize::setMoveNode(Vec2(m_pBg->getContentSize().width *
                                    m_pBg->getScaleX() * 0.05,
                                    m_pBg->getContentSize().height *
                                    m_pBg->getScaleY() * 0.12), m_scrollView);
    m_scrollView->setBounceEnabled(true); // 开启弹性效果

    // 设置滚动视图的内部容器大小
    m_scrollView->setInnerContainerSize(Size(m_pBg->getContentSize().width * m_pBg->getScaleX(), m_scrollHeight));
    
    // 将滚动视图添加到当前场景
    addChild(m_scrollView);
    
    m_menu = Menu::create();
    m_menu->setPosition(Vec2::ZERO);
    m_menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    // 表情按钮
    auto pMenuEmoji = MenuButton::create("发送表情", [this](Ref* pSender){
        log("点击表情按钮。");
        auto layer = EmojiLayer::create();
        addChild(layer, INT_MAX);
    });
    pMenuEmoji->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    WZQDeviceSize::setPosAtNodeLeftTop(pMenuEmoji, m_pBg);
    m_menu->addChild(pMenuEmoji);

    addChild(m_menu);
    
    return true;
}

void ScrollLayer::onEnter()
{
    Layer::onEnter();
    
}

void ScrollLayer::onExit()
{
    Layer::onExit();
}

ScrollLayer::ScrollLayer():m_scrollHeight(0)
{
    
}

ScrollLayer::~ScrollLayer() noexcept
{
    
}

void ScrollLayer::addText(const std::string text)
{
    auto pLabel = Label::createWithSystemFont(text, "黑体", WZQDeviceSize::getFontSize());
    pLabel->setLineBreakWithoutSpace(true);
    pLabel->setWidth(m_pBg->getContentSize().width *
                       m_pBg->getScaleX() * 0.9);
    printf("标签宽度：%f\n", m_pBg->getContentSize().width *
           m_pBg->getScaleX() * 0.9);
    pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    m_scrollHeight += pLabel->getContentSize().height + 10;
    m_scrollView->setInnerContainerSize(Size(m_pBg->getContentSize().width * m_pBg->getScaleX() * 0.9, m_scrollHeight));
    printf("标签高度：%f", pLabel->getContentSize().height);
    pLabel->setPosition(Vec2(0, m_scrollHeight));
    pLabel->setAlignment(TextHAlignment::LEFT);


    m_scrollView->addChild(pLabel);

}

void ScrollLayer::setButton(MenuButton *button) {
    button->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    button->setScale(WZQDeviceSize::getContentScaleInHeight(button, 0.05f));
    WZQDeviceSize::setPosAtNodeRightBottom(button, m_pBg);
    WZQDeviceSize::setMoveNode(Vec2( - m_pBg->getContentSize().width * m_pBg->getScaleX() * 0.02f,m_pBg->getContentSize().height * m_pBg->getScaleY() * 0.05f), button);
    
    m_Editbox = EditBox::create(Size(m_pBg->getContentSize().width * m_pBg->getScaleX() - button->getContentSize().width * button->getScaleX() - 30, button->getContentSize().height * button->getScaleY()), Scale9Sprite::create());
    
    m_Editbox->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    WZQDeviceSize::setPosAtNodeLeftBottom(m_Editbox, button);
    m_Editbox->setFontColor(Color3B::WHITE);
    m_Editbox->setFontSize(WZQDeviceSize::getFontSize());
    m_Editbox->setPlaceHolder("请输入信息");
    m_Editbox->setPlaceholderFontColor(Color3B::GRAY);
    m_Editbox->setPlaceholderFontSize(WZQDeviceSize::getFontSize());
    m_Editbox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    
    addChild(m_Editbox);

    m_menu->addChild(button);
}

std::string ScrollLayer::getEditboxText()
{
    std::string text = m_Editbox->getText();
    m_Editbox->setText("");
    return text;
}






