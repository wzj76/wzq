//
//  RuleLayer.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/18.
//

#ifndef RuleLayer_hpp
#define RuleLayer_hpp

#include <stdio.h>
#include "WZQEventWrapper.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"

USING_NS_CC;

class RuleLayer: public Layer, WZQEventWrapper
{
public:
    RuleLayer();
    ~RuleLayer();
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(RuleLayer);
    
    void onMenuEsc(Ref* pSender);
    
private:
    Sprite* m_pBg;
    ui::ScrollView* m_scrollView;
    int m_scrollHeight;
    Menu* m_pMenu;
};



#endif /* RuleLayer_hpp */
