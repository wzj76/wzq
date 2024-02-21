//
//  WZQDeviceSize.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/23.
//

#ifndef WZQDeviceSize_hpp
#define WZQDeviceSize_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;


class WZQDeviceSize
{
public:
    // 获取正常大小的字体值
    static int getFontSize();
    
    // 过去普通按钮大小的缩放值
    static float getMenuButtonScale(Sprite* menuButton);
    
    // 相对于竖直的缩放
    static float getContentScaleInHeight(Node* pNode, float scale = 0.5f);
    
    // 相对于水平的缩放
    static float getContentScaleInWidth(Node* pNode, float scale = 0.5f);
    
    // 获取当前屏幕位置的中心
    static Vec2 getViewCenterPos();
    
    // 获取某个组件的中心
    static Vec2 getContentCenterPos(Node* pNode);
    
    // 获取屏幕位置的左上角
    static Vec2 getViewLeftTopPos();
    
    // 获取屏幕位置的右下角
    static Vec2 getViewRightBottomPos();
    
    // 获取屏幕位置的左下角
    static Vec2 getViewLeftBottomPos();
    
    // 铺满屏幕
    static Vec2 getFullScreenScale(Node* pNode);
    
    // 等比缩放Node至固定大小
    static float getCertainPixelScale(Node* pNode, unsigned int pixels);
    
    // 将组件放置到背景组件的右上角
    static void setPosAtNodeRightTop(Node* thisNode, Node* background);
    
    // 将组件放置到背景组件的右方
    static void setPosAtNodeRightMiddle(Node* thisNode, Node* background);
    
    // 将组件放置到背景组件的下方
    static void setPosAtNodeBottomMiddle(Node* thisNode, Node* background);
    
    // 将组件放置到背景组件的上方
    static void setPosAtNodeTopMiddle(Node* thisNode, Node* background);
    
    // 将组件移动一些位置
    static void setMoveNode(Vec2 direction, Node* pNode);
    
    // 将组件放置到背景组件的左上角
    static void setPosAtNodeLeftTop(Node* thisNode, Node* background);
    
    // 将组件放置到背景组件的左下角
    static void setPosAtNodeLeftBottom(Node* thisNode, Node* background);
    
    // 将组件放置到背景组件的左方
    static void setPosAtNodeLeftMiddle(Node* thisNode, Node* background);
    
    // 将组件放置到背景组件的正中间
    static void setPosAtNodeMiddle(Node* thisNode, Node* background);
    
    // 将组件放置到背景组件的右下角
    static void setPosAtNodeRightBottom(Node* thisNode, Node* background);
    
    static int getBigFontSize();
    
    // 调整锚点到中心并且不改变组件的位置，并返回原来的锚点
    static Vec2 setAnchorNoMove(Node* thisNode);
    
    static float getCertainPixelScaleInHeight(cocos2d::Node *pNode, unsigned int pixels);
};



#endif /* WZQDeviceSize_hpp */
