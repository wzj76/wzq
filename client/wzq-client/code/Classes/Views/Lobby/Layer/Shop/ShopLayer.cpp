//
//  ShopLayer.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/19.
//

#include "ShopLayer.hpp"
#include "WZQDeviceSize.hpp"
#include "MenuButton.hpp"
#include "AppEvent.h"
#include "AppModel.h"
#include "WZQEventWrapper.h"
#include <iostream>
#include <algorithm>
#include "paohuziProtocol.pb.h"

bool ShopLayer::init()
{
    if(!Layer::init())
    {
        return false;
    }
    m_pMenu = Menu::create();
    m_pMenu->setPosition(Vec2::ZERO);
    addChild(m_pMenu);
    
    m_pBg = Sprite::create(AppModel::getInstance()->m_pUserModel->getThemePath() + BG_POP);
    auto size = m_pBg->getContentSize();
    m_pBg->setAnchorPoint(Vec2(0.5,0));
    addChild(m_pBg,-1);
    auto vsize = Director::getInstance()->getVisibleSize();
    m_pBg->setPosition(Vec2(vsize.width/2,vsize.height*0.15));
    m_pBg -> setScale(WZQDeviceSize::getContentScaleInHeight(m_pBg, 0.7));
    
    // 退出
    auto pMenuButtonExit = MenuButton::create("",ESC_BT,[this](Ref* pSender){
        dispatchEvent(BUTTONS_BACK);
        removeFromParent();
    });
    WZQDeviceSize::setPosAtNodeRightTop(pMenuButtonExit, m_pBg);
    m_pMenu->addChild(pMenuButtonExit);
    
    auto pLable = Label::createWithTTF("商 城", FONT_GUHUANG, WZQDeviceSize::getFontSize() + 10);
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
    
    m_pScrollMenu = Menu::create();
    m_pScrollMenu->setPosition(Vec2::ZERO);
    m_scrollView->addChild(m_pScrollMenu);
    auto shopinfo = AppModel::getInstance()->m_pShopModel;
    for(auto& i:shopinfo->m_items){
        m_items.push_back(i);
    }
    int flag = 0;
    for(auto& i : AppModel::getInstance()->m_pShopModel->m_allItems)
    {
        for(int k=0;k<m_items.size();k++)
        {
            if(m_items[k].first.ID == i.ID){
                addScrollViewInfo(i,m_items[k].second);
                flag=1;
            }
        }
        if(!flag){
            addScrollViewInfo(i,-1);
        }
        flag=0;
        
    }
    
    updateScrollView();
    
    return true;
}

ShopLayer::ShopLayer()
{
    //你自己写哪些是可以重复的
    for(auto& i : AppModel::getInstance()->m_pShopModel->m_allItems){
        if(i.ID == 17){
            m_repeated_items.push_back(i);
        }
    }
    m_themeMap["诱惑魅紫"] = purple;
    m_themeMap["夏日暖黄"] = yellow;
    m_themeMap["热情大红"] = red;
    m_themeMap["苍翠深绿"] = green;
    m_themeMap["冰封天蓝"] = blue;
}

void ShopLayer::addScrollViewInfo(SHOPITEM item,int num)
{
    
    // 图标位置
    auto iconStartPos = Vec2(0, m_scrollHeight);
    // 按钮起始位置
    auto BtnStartPos = Vec2(m_scrollView->getContentSize().width *
                              m_scrollView->getScaleX() * 2 / 3,
                              m_scrollHeight);
    
    // 名称起始位置
    auto NameStartPos = Vec2(m_scrollView->getContentSize().width *
                             m_scrollView->getScaleX() * 1 / 5,
                             m_scrollHeight);
    
    
    // 间隔
    int spacing = m_scrollView->getContentSize().height *
                  m_scrollView->getScaleY() / 10;
    
    // 一行的标准高度
    int standardHeight = m_scrollView->getContentSize().height*m_scrollView->getScaleY()/5;
    
    auto itemIcon = Sprite::create(item.imgPath);
    auto itemSort = Label::createWithSystemFont(item.sort, "黑体", WZQDeviceSize::getFontSize());
    auto itemName = Label::createWithSystemFont(item.name, "黑体", WZQDeviceSize::getFontSize());
    auto itemPrice = Label::createWithSystemFont(std::to_string(item.price) + "金币", "黑体", WZQDeviceSize::getFontSize());
    
    // 道具图标
    itemIcon->setScale(WZQDeviceSize::getCertainPixelScaleInHeight(itemIcon, standardHeight));
    
    itemIcon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    itemIcon->setPosition(iconStartPos);
    
    m_scrollView->addChild(itemIcon);
    m_scrollContent.push_back(std::make_pair(itemIcon, iconStartPos));
    
    // 道具名
    itemName->setScale(WZQDeviceSize::getCertainPixelScaleInHeight(itemName, standardHeight * 0.4));
    itemName->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    //itemName->setPosition(sUserStartPos);
    itemName->setPosition(NameStartPos);
    
    m_scrollView->addChild(itemName);
    m_scrollContent.push_back(std::make_pair(itemName, itemName->getPosition()));
    
    // 类别名
    itemSort->setScale(WZQDeviceSize::getCertainPixelScaleInHeight(itemSort, standardHeight * 0.3));
    itemSort->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    //itemName->setPosition(sUserStartPos);
    WZQDeviceSize::setPosAtNodeLeftBottom(itemSort, itemName);
    
    m_scrollView->addChild(itemSort);
    m_scrollContent.push_back(std::make_pair(itemSort, itemSort->getPosition()));
    
    // 购买按钮
//    message REQBuy{
//        required int32 user_id = 1;
//        required int32 item_id = 2;
//        required int32 price = 3;
//        required int32 num = 4;
//    }
    MenuButton* PurchaseBtn;
    
    int flag = 0;
    for(auto i:m_repeated_items){
        if(i.ID == item.ID){
            flag = 1;
        }
    }
    if(num == -1 || flag){
        std::string text = "购买";
        if(item.sort == "道具")
        {
            text += "("+std::to_string(AppModel::getInstance()->m_pShopModel->getItemCount(item.ID))+")";
        }
        PurchaseBtn = MenuButton::create(text, [item, this](Ref* pSender){
            std::cout<<"用户购买"<<item.name<<std::endl;
            ValueMap oData;
            auto usrinfo=AppModel::getInstance()->m_pUserModel->getMyUserInfo();
            if(usrinfo.coins<item.price){
                cocos2d::MessageBox("金币不足!", "");
                return;
            }
            oData.insert(std::make_pair("user_id", Value((int)usrinfo.nUserId)));
            oData.insert(std::make_pair("item_id", Value((int)item.ID)));
            oData.insert(std::make_pair("price", Value((int)item.price)));
            oData.insert(std::make_pair("num", 1));//目前功能仅支持1
            dispatchEvent(EVT_BUY,&oData);
            
            
        });
    }
    else{
        if (item.sort != "主题")
        {
            PurchaseBtn = MenuButton::create("已拥有",[item, this](Ref* pSender){
                std::cout<<"已拥有"<<item.name<<std::endl;
                ((MenuButton*)pSender) -> setEnabled(false);
                ((MenuButton*)pSender) -> setSelected();
            });
            PurchaseBtn->setEnabled(false);
            PurchaseBtn->setSelected();
        }
        else
        {
            PurchaseBtn = MenuButton::create(" 设置为\n当前主题",[item, this](Ref* pSender){
                std::cout<<"设置主题为："<<item.name<<std::endl;
                auto &setup = AppModel::getInstance()->m_pUserModel->m_setupConf;
                setup.theme_color = AppModel::getInstance()->m_pUserModel->m_themeMap[m_themeMap[item.name]];
                AppModel::getInstance()->m_pUserModel->setSetupConf(setup);
                MessageBox("重启游戏后主题生效。", "主题更换成功");
            });
        }
    }
    PurchaseBtn->setScale(WZQDeviceSize::getCertainPixelScaleInHeight(PurchaseBtn, standardHeight * 0.9));
    PurchaseBtn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    //WZQDeviceSize::setPosAtNodeBottomMiddle(PurchaseBtn, itemName);
    PurchaseBtn->setPosition(BtnStartPos);
    WZQDeviceSize::setAnchorNoMove(PurchaseBtn);
    m_pScrollMenu->addChild(PurchaseBtn);
    m_scrollContent.push_back(std::make_pair(PurchaseBtn, PurchaseBtn->getPosition()));
    
    // 金币数字
    itemPrice->setScale(WZQDeviceSize::getCertainPixelScaleInHeight(itemPrice, standardHeight * 0.3));
    itemPrice->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    itemPrice->setColor(Color3B::YELLOW);
    WZQDeviceSize::setPosAtNodeLeftMiddle(itemPrice, PurchaseBtn);
    m_scrollView->addChild(itemPrice);
    m_scrollContent.push_back(std::make_pair(itemPrice, itemPrice->getPosition()));
    
    // 更新高度
    m_scrollHeight -= (standardHeight + spacing);
}

void ShopLayer::onEnter()
{
    Layer::onEnter();
    ADD_OBSERVER(ShopLayer::BuySuccess, EVT_BUY_SUCCESS);
}

void ShopLayer::onExit()
{
    Layer::onExit();
}

ShopLayer::~ShopLayer() noexcept
{
}

void ShopLayer::updateScrollView()
{
    if (m_scrollContent.size() == 0)
        return;
    
    // 一行的标准高度
    int standardHeight = m_scrollView->getContentSize().height*m_scrollView->getScaleY()/5;
    
    // 找到最小的纵坐标
    std::pair<Node*, Vec2> minHeight = *std::min_element(m_scrollContent.begin(), m_scrollContent.end(), [](std::pair<Node*, Vec2> p1, std::pair<Node*, Vec2> p2) {
        return p1.second.y < p2.second.y;
    });
    
    // 如果最小元素纵坐标小于0，所有元素的纵坐标减去最小元素的纵坐标
    if (minHeight.second.y < 0)
    {
        for (auto& i: m_scrollContent)
        {
            i.first -> setPosition(i.second.x, i.second.y - minHeight.second.y + standardHeight);
            i.second = Vec2(i.second.x, i.second.y - minHeight.second.y + standardHeight);
        }
        // 更新高度
        m_scrollView->setInnerContainerSize(Size(m_pBg->getContentSize().width * m_pBg->getScaleX() * 0.9, m_scrollView->getContentSize().height * m_scrollView->getScaleY() - minHeight.second.y + standardHeight));
    }
}

void ShopLayer::BuySuccess(EventCustom* pEvent){
    dispatchEvent(UPDATE_COINS);
    log("购买成功");
    ACKBuy* ack =static_cast<ACKBuy*>(pEvent->getUserData());
    for(int i=0;i<m_scrollContent.size();i++){
        if (dynamic_cast<MenuButton*>(m_scrollContent[i].first) && ack->item_id()!=17 && (i-3)/5+1 == ack->item_id()){
            ((MenuButton*)m_scrollContent[i].first) -> setText("已拥有");
            ((MenuButton*)m_scrollContent[i].first) -> setEnabled(false);
        }
    }
    cocos2d::MessageBox("购买成功!", "");
    
    auto layer = ShopLayer::create();
    getParent() -> addChild(layer);
    removeFromParent();
}
