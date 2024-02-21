#include "EffectUtil.h"

USING_NS_CC;

void EffectUtil::setEnableRecursiveCascading(Node *pNode, bool bEnable)
{
    if(!pNode)
    {
        return;
    }
    
    pNode->setCascadeColorEnabled(bEnable);
    pNode->setCascadeOpacityEnabled(bEnable);
    
    Vector<Node*> vChildren = pNode->getChildren();
    for(auto& child : vChildren)
    {
        Node* pChild = (Node*)child;
        setEnableRecursiveCascading(pChild, bEnable);
    }
}

void EffectUtil::doFadeInAndMoveAction(Node* pNode, float fTime, GLubyte fValue, float fDistenceY, CallFunc* pCallBackFuc)
{
    if (!pNode)
    {
        return;
    }
    pNode->setVisible(true);
    FadeTo *pFadeIn = FadeTo::create(fTime, fValue);
    pNode->setOpacity(0x00);
    pNode->setPositionY(pNode->getPositionY() - fDistenceY);
    MoveBy *pMoveBy = MoveBy::create(fTime, Vec2(0, fDistenceY));
    pNode->runAction(Sequence::create(Spawn::create(pMoveBy,
                                                    pFadeIn,
                                                    nullptr),
                                                    pCallBackFuc,
                                                    nullptr));
}

void EffectUtil::doFadeOutAction(Node *pNode, float fTime, GLubyte fValue, CallFunc* pCallBackFunc)
{
    FadeTo *pFadeOut = FadeTo::create(fTime, fValue);
    pNode->runAction(Sequence::create(pFadeOut,
                                        pCallBackFunc,
                                        nullptr));
}

void EffectUtil::doFadeInAndScaleTo(Node* pNode, float fTime, GLubyte fValue, float fScaleTo, float fScale)
{
    if (!pNode)
    {
        return;
    }
    pNode->setVisible(true);
    FadeTo *pFadeIn = FadeTo::create(fTime, fValue);
    pNode->setOpacity(0x00);
    pNode->setScale(fScaleTo);
    pNode->runAction(Sequence::create(Spawn::create(EaseSineIn::create(pFadeIn),
                                                    EaseBackOut::create(ScaleTo::create(fTime, fScale)),
                                                    nullptr),
                                      nullptr));
}
















