//
//  AckUndoLayer.hpp
//  wzqgame-mobile
//
//  Created by duole on 2024/1/31.
//

#ifndef AckUndoLayer_hpp
#define AckUndoLayer_hpp
#include "cocos2d.h"
#include "AppRes.h"
#include "Core/AppModel.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "Event/WZQEventWrapper.h"
#include "MenuButton.hpp"
#include "WZQWaitingLayer.h"
#include<string>
#include <stdio.h>
USING_NS_CC;
USING_NS_CC_EXT;
class AckUndoLayer : public cocos2d::Layer,WZQEventWrapper
{
public:
    AckUndoLayer();
    ~AckUndoLayer();
    
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(AckUndoLayer);
    
    // 确定按钮回调函数
    void onAgreeClicked(Ref* pSender);
    void onRefuseClicked(Ref* pSender);
    void AutoRefuse(float t);
    void Reomve(cocos2d::Ref *pSender);
private:
    Menu* m_pMenu;
    MenuButton* MenuButtonRefuse;
    int* remainTime;
};
#endif /* AckUndoLayer_hpp */
