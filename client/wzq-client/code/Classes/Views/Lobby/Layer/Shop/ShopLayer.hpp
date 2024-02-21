//
//  ShopLayer.hpp
//  wzqgame-mobile
//
//  Created by moe on 2024/2/19.
//

#ifndef ShopLayer_hpp
#define ShopLayer_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "cocos/ui/UIScrollView.h"
#include "AppRes.h"
#include "AppDef.h"
#include "WZQEventWrapper.h"
#include <unordered_map>
#include <vector>
#include <string>
USING_NS_CC;

class ShopLayer: public Layer, WZQEventWrapper
{
public:
    ShopLayer();
    ~ShopLayer();
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(ShopLayer);
    
    void addScrollViewInfo(SHOPITEM item,int num);
    void updateScrollView();
    void BuySuccess(EventCustom* pEvent);
    
private:
    Menu* m_pMenu;
    Menu* m_pScrollMenu;
    Sprite* m_pBg;
    ui::ScrollView* m_scrollView;
    int m_scrollHeight;
    std::vector<std::pair<SHOPITEM, unsigned>> m_items;
    //区分是否可重复购买
    std::vector<SHOPITEM> m_repeated_items;
    std::vector<std::pair<Node*, Vec2>> m_scrollContent;
    std::unordered_map<std::string, THEMECONF> m_themeMap;
    
};

#endif /* ShopLayer_hpp */
