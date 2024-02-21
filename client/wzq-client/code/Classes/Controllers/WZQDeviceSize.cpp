//
//  WZQDeviceSize.cpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/23.
//

#include "WZQDeviceSize.hpp"

int WZQDeviceSize::getFontSize()
{
    // 获取当前屏幕的尺寸
    auto screenSize = Director::getInstance()->getWinSize();
    // 设定设计分辨率的高度（这应该是你在游戏中设置的设计分辨率）
    float designHeight = 720.0f; // 例如，720p作为设计分辨率的高度
    
    // 计算当前屏幕高度与设计分辨率高度的比例
    float scaleFactor = screenSize.height / designHeight;

    // 返回基准字体大小乘以比例因子
    return 25 * scaleFactor;
}

int WZQDeviceSize::getBigFontSize()
{
    // 获取当前屏幕的尺寸
    auto screenSize = Director::getInstance()->getWinSize();
    // 设定设计分辨率的高度（这应该是你在游戏中设置的设计分辨率）
    float designHeight = 720.0f; // 例如，720p作为设计分辨率的高度
    
    // 计算当前屏幕高度与设计分辨率高度的比例
    float scaleFactor = screenSize.height / designHeight;

    // 返回基准字体大小乘以比例因子
    return 55 * scaleFactor;
}

float WZQDeviceSize::getMenuButtonScale(Sprite* menuButton)
{
    return Director::getInstance() -> getVisibleSize().height/menuButton ->getContentSize().height * 0.1;
}

float WZQDeviceSize::getContentScaleInHeight(Node* pNode, float scale)
{
    return Director::getInstance() -> getVisibleSize().height / pNode -> getContentSize().height * scale;
}

float WZQDeviceSize::getContentScaleInWidth(Node* pNode, float scale)
{
    return Director::getInstance() -> getVisibleSize().width / pNode -> getContentSize().width * scale;
}

cocos2d::Vec2 WZQDeviceSize::getViewCenterPos()
{
    return Vec2(Director::getInstance() -> getVisibleSize().width / 2, Director::getInstance() -> getVisibleSize().height / 2);
}

cocos2d::Vec2 WZQDeviceSize::getContentCenterPos(Node* pNode)
{
    return Vec2(pNode -> getContentSize().width / 2, pNode -> getContentSize().height / 2);
}

cocos2d::Vec2 WZQDeviceSize::getViewLeftTopPos()
{
    return Vec2(0.0f, Director::getInstance() -> getVisibleSize().height);
}

cocos2d::Vec2 WZQDeviceSize::getViewRightBottomPos()
{
    return Vec2(Director::getInstance() -> getVisibleSize().width, 0.0f);;
}

cocos2d::Vec2 WZQDeviceSize::getViewLeftBottomPos() {
    return Vec2(0.0f, 0.0f);
}

cocos2d::Vec2 WZQDeviceSize::getFullScreenScale(Node* pNode) {
    return Vec2(Director::getInstance() -> getVisibleSize().width / pNode -> getContentSize().width, Director::getInstance() -> getVisibleSize().height / pNode -> getContentSize().height);
}

float WZQDeviceSize::getCertainPixelScale(cocos2d::Node *pNode, unsigned int pixels)
{
    // 假设 node 是你要缩放的节点
    auto nodeSize = pNode->getContentSize();


    // 计算宽度和高度的缩放比例
    float scaleX = pixels / nodeSize.width;
    float scaleY = pixels / nodeSize.height;

    // 为了保持等比缩放，我们取两者中较小的一个
    float scaleFactor = std::min(scaleX, scaleY);

    return scaleFactor;
}

float WZQDeviceSize::getCertainPixelScaleInHeight(cocos2d::Node *pNode, unsigned int pixels)
{
    // 假设 node 是你要缩放的节点
    auto nodeSize = pNode->getContentSize();

    return pixels / nodeSize.height;
}

void WZQDeviceSize::setPosAtNodeRightTop(cocos2d::Node *thisNode, cocos2d::Node *background) {
    // 获取背景节点的大小和位置
    cocos2d::Size backgroundSize = background->getContentSize();
    cocos2d::Vec2 backgroundPosition = background->getPosition();
    float backgroundAnchorX = background->getAnchorPoint().x;
    float backgroundAnchorY = background->getAnchorPoint().y;
    cocos2d::Vec2 scale = Vec2(background->getScaleX(), background->getScaleY());

    // 计算背景节点的世界坐标系右上角位置
    float worldX = backgroundPosition.x + (backgroundSize.width * scale.x * (1 - backgroundAnchorX));
    float worldY = backgroundPosition.y + (backgroundSize.height * scale.y * (1 - backgroundAnchorY));

    // 将前景节点的位置设置为背景节点的右上角
    // 这里假设background和thisNode有相同的父节点
    thisNode->setPosition(cocos2d::Vec2(worldX, worldY));
}

void WZQDeviceSize::setPosAtNodeRightMiddle(cocos2d::Node *thisNode, cocos2d::Node *background) {
    // 获取背景节点的大小和位置
    cocos2d::Size backgroundSize = background->getContentSize();
    cocos2d::Vec2 backgroundPosition = background->getPosition();
    float backgroundAnchorX = background->getAnchorPoint().x;
    float backgroundAnchorY = background->getAnchorPoint().y;
    cocos2d::Vec2 scale = Vec2(background->getScaleX(), background->getScaleY());

    // 计算背景节点的世界坐标系右位置
    float worldX = backgroundPosition.x + (backgroundSize.width * scale.x * (1 - backgroundAnchorX));
    float worldY = backgroundPosition.y + (backgroundSize.height * scale.y * (0.5 - backgroundAnchorY));

    // 将前景节点的位置设置为背景节点的右上角
    // 这里假设background和thisNode有相同的父节点
    thisNode->setPosition(cocos2d::Vec2(worldX, worldY));
}

void WZQDeviceSize::setPosAtNodeBottomMiddle(cocos2d::Node *thisNode, cocos2d::Node *background) {
    // 获取背景节点的大小和位置
    cocos2d::Size backgroundSize = background->getContentSize();
    cocos2d::Vec2 backgroundPosition = background->getPosition();
    float backgroundAnchorX = background->getAnchorPoint().x;
    float backgroundAnchorY = background->getAnchorPoint().y;
    cocos2d::Vec2 scale = Vec2(background->getScaleX(), background->getScaleY());

    // 计算背景节点的世界坐标系下位置
    float worldX = backgroundPosition.x + (backgroundSize.width * scale.x * (0.5 - backgroundAnchorX));
    float worldY = backgroundPosition.y + (backgroundSize.height * scale.y * (0 - backgroundAnchorY));

    // 将前景节点的位置设置为背景节点的下位置
    // 这里假设background和thisNode有相同的父节点
    thisNode->setPosition(cocos2d::Vec2(worldX, worldY));
}

void WZQDeviceSize::setPosAtNodeTopMiddle(cocos2d::Node *thisNode, cocos2d::Node *background) {
    // 获取背景节点的大小和位置
    cocos2d::Size backgroundSize = background->getContentSize();
    cocos2d::Vec2 backgroundPosition = background->getPosition();
    float backgroundAnchorX = background->getAnchorPoint().x;
    float backgroundAnchorY = background->getAnchorPoint().y;
    cocos2d::Vec2 scale = Vec2(background->getScaleX(), background->getScaleY());

    // 计算背景节点的世界坐标系上位置
    float worldX = backgroundPosition.x + (backgroundSize.width * scale.x * (0.5 - backgroundAnchorX));
    float worldY = backgroundPosition.y + (backgroundSize.height * scale.y * (1 - backgroundAnchorY));

    // 将前景节点的位置设置为背景节点的上位置
    // 这里假设background和thisNode有相同的父节点
    thisNode->setPosition(cocos2d::Vec2(worldX, worldY));
}

void WZQDeviceSize::setMoveNode(cocos2d::Vec2 direction, cocos2d::Node *pNode)
{
    if (pNode != nullptr) // 确保传入的节点不是空指针
    {
        // 获取当前节点的位置
        cocos2d::Vec2 currentPosition = pNode->getPosition();
        // 计算新的位置
        cocos2d::Vec2 newPosition = currentPosition + direction;
        // 设置节点的新位置
        pNode->setPosition(newPosition);
    }
}

void WZQDeviceSize::setPosAtNodeLeftTop(cocos2d::Node *thisNode, cocos2d::Node *background) {
    // 获取背景节点的大小和位置
    cocos2d::Size backgroundSize = background->getContentSize();
    cocos2d::Vec2 backgroundPosition = background->getPosition();
    float backgroundAnchorX = background->getAnchorPoint().x;
    float backgroundAnchorY = background->getAnchorPoint().y;
    cocos2d::Vec2 scale = Vec2(background->getScaleX(), background->getScaleY());

    // 计算背景节点的世界坐标系左上位置
    float worldX = backgroundPosition.x + (backgroundSize.width * scale.x * (0 - backgroundAnchorX));
    float worldY = backgroundPosition.y + (backgroundSize.height * scale.y * (1 - backgroundAnchorY));

    // 将前景节点的位置设置为背景节点的左上位置
    // 这里假设background和thisNode有相同的父节点
    thisNode->setPosition(cocos2d::Vec2(worldX, worldY));
}

void WZQDeviceSize::setPosAtNodeLeftBottom(cocos2d::Node *thisNode, cocos2d::Node *background) { 
    // 获取背景节点的大小和位置
    cocos2d::Size backgroundSize = background->getContentSize();
    cocos2d::Vec2 backgroundPosition = background->getPosition();
    float backgroundAnchorX = background->getAnchorPoint().x;
    float backgroundAnchorY = background->getAnchorPoint().y;
    cocos2d::Vec2 scale = Vec2(background->getScaleX(), background->getScaleY());

    // 计算背景节点的世界坐标系左下位置
    float worldX = backgroundPosition.x + (backgroundSize.width * scale.x * (0 - backgroundAnchorX));
    float worldY = backgroundPosition.y + (backgroundSize.height * scale.y * (0 - backgroundAnchorY));

    // 将前景节点的位置设置为背景节点的右下位置
    // 这里假设background和thisNode有相同的父节点
    thisNode->setPosition(cocos2d::Vec2(worldX, worldY));
}

void WZQDeviceSize::setPosAtNodeLeftMiddle(cocos2d::Node *thisNode, cocos2d::Node *background) { 
    // 获取背景节点的大小和位置
    cocos2d::Size backgroundSize = background->getContentSize();
    cocos2d::Vec2 backgroundPosition = background->getPosition();
    float backgroundAnchorX = background->getAnchorPoint().x;
    float backgroundAnchorY = background->getAnchorPoint().y;
    cocos2d::Vec2 scale = Vec2(background->getScaleX(), background->getScaleY());

    // 计算背景节点的世界坐标系左位置
    float worldX = backgroundPosition.x + (backgroundSize.width * scale.x * (0 - backgroundAnchorX));
    float worldY = backgroundPosition.y + (backgroundSize.height * scale.y * (0.5 - backgroundAnchorY));

    // 将前景节点的位置设置为背景节点的右下位置
    // 这里假设background和thisNode有相同的父节点
    thisNode->setPosition(cocos2d::Vec2(worldX, worldY));
}

void WZQDeviceSize::setPosAtNodeMiddle(cocos2d::Node *thisNode, cocos2d::Node *background) { 
    // 获取背景节点的大小和位置
    cocos2d::Size backgroundSize = background->getContentSize();
    cocos2d::Vec2 backgroundPosition = background->getPosition();
    float backgroundAnchorX = background->getAnchorPoint().x;
    float backgroundAnchorY = background->getAnchorPoint().y;
    cocos2d::Vec2 scale = Vec2(background->getScaleX(), background->getScaleY());

    // 计算背景节点的世界坐标系
    float worldX = backgroundPosition.x + (backgroundSize.width * scale.x * (0.5 - backgroundAnchorX));
    float worldY = backgroundPosition.y + (backgroundSize.height * scale.y * (0.5 - backgroundAnchorY));

    // 将前景节点的位置设置为背景节点的正中间
    // 这里假设background和thisNode有相同的父节点
    thisNode->setPosition(cocos2d::Vec2(worldX, worldY));
}

void WZQDeviceSize::setPosAtNodeRightBottom(cocos2d::Node *thisNode, cocos2d::Node *background) { 
    // 获取背景节点的大小和位置
    cocos2d::Size backgroundSize = background->getContentSize();
    cocos2d::Vec2 backgroundPosition = background->getPosition();
    float backgroundAnchorX = background->getAnchorPoint().x;
    float backgroundAnchorY = background->getAnchorPoint().y;
    cocos2d::Vec2 scale = Vec2(background->getScaleX(), background->getScaleY());

    // 计算背景节点的世界坐标系
    float worldX = backgroundPosition.x + (backgroundSize.width * scale.x * (1 - backgroundAnchorX));
    float worldY = backgroundPosition.y + (backgroundSize.height * scale.y * (0 - backgroundAnchorY));

    // 将前景节点的位置设置为背景节点的右下角
    // 这里假设background和thisNode有相同的父节点
    thisNode->setPosition(cocos2d::Vec2(worldX, worldY));
}

cocos2d::Vec2 WZQDeviceSize::setAnchorNoMove(cocos2d::Node *thisNode)
{
    // 保存原始锚点
    cocos2d::Vec2 originalAnchor = thisNode->getAnchorPoint();

    // 获取节点当前的位置（基于原始锚点）
    cocos2d::Vec2 originalPosition = thisNode->getPosition();

    // 获取节点的缩放
    float scaleX = thisNode->getScaleX();
    float scaleY = thisNode->getScaleY();

    // 获取节点的大小
    cocos2d::Size contentSize = thisNode->getContentSize();

    // 计算原始锚点到中心锚点的偏移量（在节点的本地坐标系中）
    cocos2d::Vec2 anchorOffset(contentSize.width * (originalAnchor.x - 0.5f),
                               contentSize.height * (originalAnchor.y - 0.5f));

    // 将锚点偏移量转换为父节点坐标系下的实际偏移量
    cocos2d::Vec2 positionOffset(anchorOffset.x * scaleX, anchorOffset.y * scaleY);

    // 调整锚点到中心（0.5, 0.5）
    thisNode->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));

    // 根据锚点偏移量调整节点位置，以保持节点在屏幕上的相对位置不变
    thisNode->setPosition(originalPosition - positionOffset);

    // 返回原始锚点
    return originalAnchor;
}
















