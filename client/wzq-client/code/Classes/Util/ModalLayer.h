/*******************************************************************************
 所有的模态对话框继承此类。
 继承此类 = 继承CCLayerColor + 是模态的。
 在此类的实现中，用到了递归 + dynamic_cast，不过因为这只是在页面开始展示的时候调用一次，
 并不会参与到游戏核心逻辑的计算，因此并无大碍。
 *******************************************************************************/

#ifndef UTIL_MODALLAYER_H_
#define UTIL_MODALLAYER_H_

#include "cocos2d.h"

namespace util
{
	// 模态框的触摸优先级 与 模态框上菜单的触摸优先级
    class ModalLayer : public cocos2d::LayerColor
    {
    public:
        virtual bool init();
        virtual bool initWithColor(const cocos2d::Color4B& color);
        virtual bool initWithColor(const cocos2d::Color4B& color, GLfloat width, GLfloat height);
        virtual void onEnter();
        virtual void onExit();
        virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
        virtual void onTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
        virtual void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
    };
}

#endif // UTIL_MODALLAYER_H_
