//
//  RuleLayer.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/18.
//

#include "RuleLayer.hpp"
#include "WZQDeviceSize.hpp"
#include "AppRes.h"
#include "AppEvent.h"
#include "MenuButton.hpp"
#include "AppModel.h"
USING_NS_CC_EXT;

bool RuleLayer::init()
{
    if(!Layer::init())
    {
        return false;
    }
    m_pMenu = Menu::create();
    m_pMenu->setPosition(Vec2::ZERO);
    
    m_pBg = Sprite::create(AppModel::getInstance()->m_pUserModel->getThemePath() + BG_POP);
    auto size = m_pBg->getContentSize();
    m_pBg->setAnchorPoint(Vec2(0.5,0));
    addChild(m_pBg,-1);
    auto vsize = Director::getInstance()->getVisibleSize();
    m_pBg->setPosition(Vec2(vsize.width/2,vsize.height*0.15));
    m_pBg -> setScale(WZQDeviceSize::getContentScaleInHeight(m_pBg, 0.7));
    
    // 退出
    auto pMenuButtonExit = MenuButton::create("",ESC_BT, CC_CALLBACK_1(RuleLayer::onMenuEsc, this));
    WZQDeviceSize::setPosAtNodeRightTop(pMenuButtonExit, m_pBg);
    m_pMenu->addChild(pMenuButtonExit);
    
    // 好友列表
    auto pLable = Label::createWithTTF("五子棋介绍", FONT_GUHUANG, WZQDeviceSize::getFontSize() + 10);
    pLable -> setColor(Color3B::ORANGE);
    pLable -> setPosition(WZQDeviceSize::getViewCenterPos().x,  WZQDeviceSize::getViewCenterPos().y + 4 * m_pBg -> getContentSize().height * m_pBg -> getScaleY() / 10);
    addChild(pLable);
    
    // 创建一个滚动视图
    m_scrollView = ui::ScrollView::create();
    m_scrollView->setContentSize(Size(m_pBg->getContentSize().width *
                                    m_pBg->getScaleX() * 0.8,
                                    m_pBg->getContentSize().height *
                                    m_pBg->getScaleY() * 0.7));
    m_scrollView->setDirection(ui::ScrollView::Direction::VERTICAL); // 垂直滚动
    WZQDeviceSize::setPosAtNodeLeftBottom(m_scrollView, m_pBg);
    WZQDeviceSize::setMoveNode(Vec2(m_pBg->getContentSize().width *
                                    m_pBg->getScaleX() * 0.12,
                                    m_pBg->getContentSize().height *
                                    m_pBg->getScaleY() * 0.10), m_scrollView);
    m_scrollView->setBounceEnabled(true); // 开启弹性效果
    
    m_scrollHeight = m_scrollView->getContentSize().height *
                     m_scrollView->getScaleY();

    // 设置滚动视图的内部容器大小
    m_scrollView->setInnerContainerSize(Size(m_pBg->getContentSize().width * m_pBg->getScaleX(), m_scrollHeight));
    
    
    // 将滚动视图添加到当前场景
    addChild(m_scrollView);
    
    // 起始位置
    auto sStartPos = Vec2(0, WZQDeviceSize::getFontSize() * 80);
    
    
    auto richText = cocos2d::ui::RichText::create();
    richText->ignoreContentAdaptWithSize(false);
    richText->setContentSize(Size(m_pBg->getContentSize().width * m_pBg->getScaleX() * 0.75, WZQDeviceSize::getFontSize() * 80)); // 根据需要调整大小
    richText->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    richText->setPosition(sStartPos);

    // "五子棋，又称为连珠、五连珠、五子连线等，是一种两人对弈的纯策略型棋类游戏。"
    auto textElement1 = cocos2d::ui::RichElementText::create(1, Color3B::WHITE, 255, "五子棋，又称为连珠、五连珠、五子连线等，是一种两人对弈的纯策略型棋类游戏。", "黑体", WZQDeviceSize::getFontSize());
    richText->pushBackElement(textElement1);

    // "它简单易学，但变化丰富，深受各年龄层玩家的喜爱。"
    auto textElement2 = cocos2d::ui::RichElementText::create(2, Color3B::WHITE, 255, "它简单易学，但变化丰富，深受各年龄层玩家的喜爱。", "黑体", WZQDeviceSize::getFontSize());
    richText->pushBackElement(textElement2);

    // "五子棋可以在无边界的纸上玩，也可以在固定边界的棋盘上进行，现代通常在15x15或19x19的棋盘上进行。"
    auto textElement3 = cocos2d::ui::RichElementText::create(3, Color3B::WHITE, 255, "五子棋可以在无边界的纸上玩，也可以在固定边界的棋盘上进行，现代通常在15x15或19x19的棋盘上进行。", "黑体", WZQDeviceSize::getFontSize());
    richText->pushBackElement(textElement3);

    // "五子棋的主要目标是率先在横、竖、斜线上形成连续的五个棋子。"
    auto textElement4 = cocos2d::ui::RichElementText::create(4, Color3B::WHITE, 255, "五子棋的主要目标是率先在横、竖、斜线上形成连续的五个棋子。\n\n ", "黑体", WZQDeviceSize::getFontSize());
    richText->pushBackElement(textElement4);

    // 基本规则标题
    auto basicRulesTitle = cocos2d::ui::RichElementText::create(5, Color3B::YELLOW, 255, "基本规则\n\n ", "黑体", WZQDeviceSize::getFontSize() * 1.3);
    richText->pushBackElement(basicRulesTitle);

    // 规则详情
    // 注意：实际使用时，每个规则项可以根据需要分成更细的元素来突出显示（如加粗、改变颜色等）
    auto basicRulesContent = cocos2d::ui::RichElementText::create(6, Color3B::WHITE, 255, "1. 棋盘和棋子：五子棋通常使用标准的围棋棋盘（19×19线），但也有使用15×15线棋盘的情况。两位玩家分别执黑白两色棋子。\n\n2. 开局：一般由执黑子的玩家先行（因为黑子有一定的先手优势），棋子放置在棋盘上的交叉点上。\n\n3. 轮流下棋：双方轮流在棋盘上放置自己的棋子，每次只能放置一个棋子。\n\n4. 胜负判定：谁先在棋盘上横、竖、斜任一方向形成连续的五个自己的棋子，谁就获胜。如果整个棋盘被下满而没有人形成连续的五个棋子，则判定为和棋。\n\n ", "黑体", WZQDeviceSize::getFontSize());
    richText->pushBackElement(basicRulesContent);

    // 变种规则标题
    auto variantRulesTitle = cocos2d::ui::RichElementText::create(7, Color3B::YELLOW, 255, "变种规则\n\n ", "黑体", WZQDeviceSize::getFontSize() * 1.3);
    richText->pushBackElement(variantRulesTitle);

    // 变种规则内容
    auto variantRulesContent = cocos2d::ui::RichElementText::create(8, Color3B::WHITE, 255, "1. 禁手：在一些比赛中，为了平衡黑白双方的先后手优势，对黑棋设置了一些特殊的禁手规则。最常见的禁手包括“三三禁手”（同时形成两个活三）和“四四禁手”（同时形成两个活四），还有“长连禁手”（连成六子及以上）。禁手规则仅适用于执黑棋的玩家。\n\n2. 开局变化：为了增加游戏的多样性和平衡性，一些比赛和游戏平台引入了不同的开局变化，如指定开局、双方轮流选择开局等。\n\n3. 计时规则：在正式比赛中，通常会对双方的思考时间进行限制，以增加比赛的观赏性和紧张感。\n\n五子棋虽然规则简单，但是博大精深，含有丰富的战术和策略。玩家需要不断学习和实践，提高自己的布局、攻防转换能力，以及对局势的判断力。", "黑体", WZQDeviceSize::getFontSize());
    richText->pushBackElement(variantRulesContent);
    
    m_scrollView->setInnerContainerSize(Size(m_pBg->getContentSize().width * m_pBg->getScaleX(), richText->getContentSize().height * richText->getScaleY()));
    // 将富文本添加到当前层或场景
    m_scrollView->addChild(richText);
    return true;
}

void RuleLayer::onMenuEsc(cocos2d::Ref *pSender) {
    dispatchEvent(BUTTONS_BACK);
    removeFromParent();
}

RuleLayer::RuleLayer()
{
}

RuleLayer::~RuleLayer() noexcept
{
    
}

void RuleLayer::onEnter()
{
    Layer::onEnter();
}

void RuleLayer::onExit()
{
    Layer::onExit();
}

