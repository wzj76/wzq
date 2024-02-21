#ifndef __BaoHuang__EffectUtil__
#define __BaoHuang__EffectUtil__

#include <iostream>
#include "cocos2d.h"
/*
 * author mtt
 */
class EffectUtil
{
public:
    //先开启子结点透明度2随父节点变化的状态
    static void setEnableRecursiveCascading(cocos2d::Node* pNode, bool bEnable);
    /*
     *pNode      要做动作的对象
     *fTime      动作持续时间
     *fValue     透明度变化值
     *fDistence  往上移动的距离
     *pCallBackFunc 最后的回调函数
     */
    static void doFadeInAndMoveAction(cocos2d::Node* pNode, float fTime = 0.3f, GLubyte fValue = 0xFF,
                                      float fDistence = 20.0f, cocos2d::CallFunc* pCallBackFunc = nullptr);
    
    /*
     *pNode      要做动作的对象
     *fTime      动作持续时间
     *fValue     透明度变化值
     *pCallBackFunc 最后的回调函数
     */
    static void doFadeOutAction(cocos2d::Node* pNode, float fTime, GLubyte fValue, cocos2d::CallFunc* pCallBackFuc);
    
    static void doFadeInAndScaleTo(cocos2d::Node* pNode, float fTime, GLubyte fValue, float fScaleTo, float fScale);
    
};


#endif
