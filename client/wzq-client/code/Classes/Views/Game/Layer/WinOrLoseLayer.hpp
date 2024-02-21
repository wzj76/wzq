//
//  WinOrLoseLayer.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/1/30.
//

#ifndef WinOrLoseLayer_hpp
#define WinOrLoseLayer_hpp


#include "cocos2d.h"
#include "AppRes.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "Event/WZQEventWrapper.h"
#include "MenuButton.hpp"
#include "WZQWaitingLayer.h"
#include<string>
USING_NS_CC;

USING_NS_CC_EXT;
class WinOrLoseLayer : public cocos2d::Layer,WZQEventWrapper
{
public:
    WinOrLoseLayer();
    ~WinOrLoseLayer();
    
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(WinOrLoseLayer);
    
    // 确定按钮回调函数
    void onComfrimClicked(Ref* pSender);
    void goBackToLobby(EventCustom* pEvent);
    
    
private:
    Menu* m_pMenu;
};


#endif /* WinOrLoseLayer_hpp */
